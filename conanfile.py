import os

from conan import ConanFile
from conan.tools.build import stdcpp_library
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, rmdir
from conan.tools.microsoft import is_msvc

required_conan_version = ">=2.0"


class LaslibConan(ConanFile):
    name = "laslib"
    # Matches the branch name. Products depend on this number, so it doesn't
    # follow LAStools' own versions. See README-Aquaveo.txt.
    version = "2022.22.3"
    description = "C++ library for lossless LiDAR compression, part of the LAStools project."
    license = "LGPL-2.1"
    topics = ("laszip", "las", "laz", "lidar", "compression", "decompression")
    homepage = "https://github.com/LAStools/LAStools"
    url = "https://github.com/Aquaveo/LAStools"

    settings = "os", "arch", "compiler", "build_type"

    # The recipe lives next to the source it builds, so the source travels
    # with the recipe instead of being downloaded. A local edit then gets its
    # own recipe revision, and the same commit always gets the same one.
    # LASlib/lib is where LASlib's CMake writes its output, so it is left out in
    # case someone built in this folder.
    exports_sources = (
        "CMakeLists.txt",
        "COPYING.txt",
        "LASlib/*",
        "LASzip/*",
        "src/*",
        "!LASlib/lib/*",
    )

    # Visual Studio used to default to `wchar_t=typedef`, but now it defaults
    # to `wchar_t=builtin` instead. The old default was nonstandard and should
    # be avoided where possible.
    #
    # Unfortunately, XMS links to a pre-built library that uses
    # `wchar_t=typedef`, so everything else it links to has to do the same.
    #
    # Prefer `wchar_t=builtin` wherever it works, and only use `wchar_t=typedef`
    # for XMS or if you can't rebuild an old library and have to get around
    # linker errors due to `wchar_t<=>unsigned short` mismatches.
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "wchar_t": ["builtin", "typedef"],
    }
    default_options = {
        "shared": True,
        "fPIC": False,
        "wchar_t": "builtin",
    }

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")
        if not is_msvc(self):
            self.options.rm_safe("wchar_t")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def compatibility(self):
        # laslib crashes at runtime if the consumer's compiler.cppstd doesn't
        # match exactly (both higher and lower versions crash). Disable Conan's
        # default cppstd_compat plugin, which would otherwise let a consumer
        # with a higher cppstd silently link against a laslib built with a
        # lower one.
        return None

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["LASZIP_BUILD_STATIC"] = not self.options.shared
        tc.variables["USE_TYPEDEF_WCHAR_T"] = (
            self.options.get_safe("wchar_t") == "typedef"
        )
        # LAStools' CMakeLists.txt declares an old `cmake_minimum_required`,
        # which CMake 4+ rejects (it dropped compatibility with <3.5). This lets
        # newer CMake configure the project as if the minimum were 3.5.
        tc.cache_variables["CMAKE_POLICY_VERSION_MINIMUM"] = "3.5"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        # cmake.install() gets us all the headers we want, but also adds a
        # bunch of executables we don't. This is a consequence of building all
        # of LAStools rather than just the two things we really want. It could
        # probably be fixed, but the exes don't add much build time and this
        # was easy to make work.
        rmdir(self, os.path.join(self.package_folder, "bin"))
        copy(self, "COPYING.txt", src=self.source_folder,
             dst=os.path.join(self.package_folder, "licenses"))
        lib_dir = os.path.join(
            self.source_folder, "LASlib", "lib", str(self.settings.build_type))
        copy(self, "*.dll", src=lib_dir,
             dst=os.path.join(self.package_folder, "bin"))
        copy(self, "*.pdb", src=lib_dir,
             dst=os.path.join(self.package_folder, "bin"))
        copy(self, "*.lib", src=lib_dir,
             dst=os.path.join(self.package_folder, "lib"))

    def package_info(self):
        # Mind the case, this also needs to build on Linux where file name case
        # matters.
        suffix = "d" if self.settings.build_type == "Debug" else ""
        self.cpp_info.libs = [f"LASlib{suffix}"]

        # On non-MSVC platforms the upstream install rule places the library
        # in `lib/LASlib/`, not the default `lib/`. On Windows, the recipe's
        # package() does an extra flat copy into `lib/`, so the default works.
        if self.settings.os != "Windows":
            self.cpp_info.libdirs = [os.path.join("lib", "LASlib")]

        if self.options.shared:
            self.cpp_info.defines.append("LASZIP_DYN_LINK")
        else:
            if self.settings.os in ["Linux", "FreeBSD"]:
                self.cpp_info.system_libs.append("m")
            libcxx = stdcpp_library(self)
            if libcxx:
                self.cpp_info.system_libs.append(libcxx)
