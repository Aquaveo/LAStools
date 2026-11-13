Aquaveo fork of LAStools
========================

Licensing
---------

Several Aquaveo products use a Conan package called "laslib". This package
contains the LASlib library, which we use to read lidar points out of .las and
.laz files.

LASlib is licensed under the terms of the LGPL, which requires that we make it
reasonably easy for the user to do all the following:
- Swap out LASlib with an equivalent library
- Obtain, examine, modify and build the source code for the library we use

We apply several modifications to the library to make it work for our products,
so we can't just refer users to the maintainer. We do the following to make sure
we use the library the way the authors intended:
- Build it as a .dll/.so for easy swapping
- Publish the source code
- Provide build scripts and documentation
- Add notes to our own product's licensing terms and About pages pointing the
  user to this repo

Making changes to an existing version
-------------------------------------

If you need to change an existing version, find the branch that matches the
version. Make your changes to the branch, one self-contained change per commit,
then tag the branch like `2022.22.3.post1`. The first three numbers are the
version of LASlib the branch started from, and the `postN` suffix indicates the
release number we made. Start from `post1`, then `post2`, `post3`, etc.

Be aware that commits you make will eventually need to be rebased onto a new
version of LASlib which may be very different from the one the commit was
originally written against. The developer responsible for this will likely be
interested in:
- Problems in LASlib we're working around. They may have been fixed upstream,
  making the commit obsolete and letting the developer drop it.
- Constraints imposed by our own products. Similarly, the current versions of
  our products may have had their use improved to make these commits obsolete.
- A brief explanation of how to reproduce the problem, or otherwise verify if
  the fix is still needed. Keep in mind though that most commits will be due to
  outside code that may change, making precise references go stale. Explanations
  should stick to things that are unlikely to change if possible, or refer to
  specific commits or tags otherwise.
  
  Good examples:
  - SMS fails to build at link time due to a missing GeoProjectionConverter
    symbol.
  - When GMS #include's laslib.h after windows.h, it warns about redefinition of
    the MAX_PATH macro. XMS products treat warnings as errors, so the build
	fails.
  - XMS depends on our essentials package, which was built by an old Visual
    Studio that defines `wchar_t` as a typedef. Building XMS with `wchar_t`
	defined as a builtin the way it should have been results in linker errors
	about missing `std::ostream::operator<<` overloads for
	`std::string<const uint16_t*>` that originate from that package. Building
	XMS with the old `wchar_t` and this package with the new one instead results
	in similar linker errors from here. They have to be consistent, and our
	LASlib package is easier to fix than the essentials one, so it's fixed here.
- A general explanation of how the commit was derived, in case it isn't
  obvious. Example: "I ran the included script to find all the uses of `BOOL`
  and replace them", or "I used a small sample program to read and check all
  the fields in a test file's header in random order about a million times each
  across thirty threads and applied the fix to ones that came back wrong at
  least once".

If we're working around a problem in LASlib, consider filing a bug report or
submitting a patch to them with the fix. They might fix the problem upstream
and make it easier for us to update our LASlib next time. For problems on our
end, consider filing a bug report against our own product to try and get the
problem fixed there eventually so we don't have to patch it again next time.

Commits should completely fix one problem, and only one problem. This will make
it easier to update LASlib when necessary, since we won't have to try and
untangle multiple changes from a single commit.


Updating the LASlib we're based on
----------------------------------

Eventually we'll want to update to a new LASlib. Suggested steps:

- Fetch the latest from the original LAStools repo into our `master`.
- Make a new branch off the tip of our `master`, or one of LAStools' tags if
  it has one that looks good. The current branch is named for the date it was
  made, ones made from LAStools tags should be named after the LAStools tag.
- Replay all the commits from the most recent branch in this repo onto the
  new branch, fixing any merge conflicts that arise.
  - Some commits may be obsolete and removable. Feel free to drop them so we
    don't have to maintain them any more going forward.
- Test it out in the products that use it and make sure it works. If not, add
  more commits that fix the issues, one problem per commit, same as making
  changes to an existing version. If a new problem looks like part of an old
  one, feel free to rewrite the history of the new branch to fix it before
  pushing it. It'll be easier to update again later if the problem isn't spread
  out across multiple commits.
- When everything looks good, dependent products build, and their tests pass,
  push your branch and tag it. The tag for this branch uses the date when we
  branched off LASlib. Future ones would probably be best named after a LASlib
  tag if practical, so it's easier to know which one we started from.


Branches
--------

* `master` follows upstream LAStools. Don't commit our changes to it.
* Each of our branches starts at an upstream commit and adds our changes on
  top of it, one commit per change. Each branch is named after the version of
  the Conan package built from it (for example, `2022.22.3`). Other products
  depend on these version numbers, so they don't follow LAStools' own tags.
* Keep old branches. Each shipped package must be traceable to the exact source
  it was built from.

Commit messages should say why we need each change. The git history of a
branch is the list of our changes.


External links
--------------

The following links provide some information that helped develop the 2022.22.3
version:
https://groups.google.com/g/lastools/c/zDfLAcbSR7o/m/XAFdu1Nvie4J
https://groups.google.com/g/lastools/c/Rz6jzfq-DR4
