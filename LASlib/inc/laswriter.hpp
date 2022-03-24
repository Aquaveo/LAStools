/*
===============================================================================

  FILE:  laswriter.hpp

  CONTENTS:

    Interface to write LIDAR points to the LAS format versions 1.0 - 1.4 and
    per on-the-fly conversion to simple ASCII files.

  PROGRAMMERS:

    info@rapidlasso.de  -  https://rapidlasso.de

  COPYRIGHT:

    (c) 2007-2015, rapidlasso GmbH - fast tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the LICENSE.txt file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  CHANGE HISTORY:

    7 September 2018 -- replaced calls to _strdup with calls to the LASCopyString macro
    17 August 2017 -- switch on "native LAS 1.4 extension". turns off with '-no_native'.
    29 March 2017 -- enable "native LAS 1.4 extension" for LASzip via '-native'
    13 November 2016 -- return early FALSE when set_directory() will not succeed
    5 September 2011 -- support for writing Terrasolid's BIN format
    11 June 2011 -- billion point support: p_count & npoints are 64 bit counters
    8 May 2011 -- DO NOT USE option for variable chunking via chunk()
    9 April 2011 -- added capability to write on-the-fly conversion to ASCII
    24 January 2011 -- introduced LASwriteOpener
    21 January 2011 -- turned into abstract reader to support multiple files
    3 December 2010 -- updated to (somewhat) support LAS format 1.3
    7 September 2008 -- updated to support LAS format 1.2
    21 February 2007 -- created after eating Sarah's veggies with peanutsauce

===============================================================================
*/
#ifndef LAS_WRITER_HPP
#define LAS_WRITER_HPP

#include "lasdefinitions.hpp"

#include "lasutility.hpp"

class LASLIB_DLL LASwriter
{
public:
  LASquantizer quantizer;
  I64 npoints;
  I64 p_count;
  LASinventory inventory;

  virtual LASBOOL write_point(const LASpoint* point) = 0;
  virtual void update_inventory(const LASpoint* point) { inventory.add(point); };
  virtual LASBOOL chunk() = 0;

  virtual LASBOOL update_header(const LASheader* header, LASBOOL use_inventory=FALSE, LASBOOL update_extra_bytes=FALSE) = 0;
  virtual I64 close(LASBOOL update_npoints=TRUE) = 0;

  void dealloc();

  LASwriter() { npoints = 0; p_count = 0; };
  virtual ~LASwriter() {};
};

#include "laswaveform13writer.hpp"

class LASLIB_DLL LASwriteOpener
{
public:
  void set_io_obuffer_size(I32 io_obuffer_size);
  inline I32 get_io_obuffer_size() const { return io_obuffer_size; };
  LASBOOL set_directory(const CHAR* directory);
  void set_file_name(const CHAR* file_name);
  void set_appendix(const CHAR* appendix);
  void set_cut(U32 cut);
  void set_native(LASBOOL native);
  LASBOOL set_format(I32 format);
  LASBOOL set_format(const CHAR* format);
  void set_force(LASBOOL force);
  void set_chunk_size(U32 chunk_size);
  void make_numbered_file_name(const CHAR* file_name, I32 digits);
  void make_file_name(const CHAR* file_name, I32 file_number=-1);
  const CHAR* get_directory() const;
  const CHAR* get_file_name() const;
  const CHAR* get_appendix() const;
  const CHAR* get_file_name_only() const;
  CHAR* get_file_name_base() const;
  U32 get_cut() const;
  LASBOOL get_native() const;
  LASBOOL format_was_specified() const;
  I32 get_format() const;
  const CHAR* get_format_name() const;
  void set_parse_string(const CHAR* parse_string);
  inline const CHAR* get_parse_string() const { return parse_string; };
  void set_separator(const CHAR* separator);
  inline const CHAR* get_separator() const { return separator; };
  void set_scale_rgb(F32 scale_rgb);
  void usage() const;
  LASBOOL parse(int argc, char* argv[]);
  LASBOOL active() const;
  LASBOOL is_piped() const;
  LASwriter* open(const LASheader* header);
  LASwaveform13writer* open_waveform13(const LASheader* lasheader);
  LASwriteOpener();
  ~LASwriteOpener();
private:
  void add_directory(const CHAR* directory=0);
  void add_appendix(const CHAR* appendix=0);
  void cut_characters(U32 cut=0);
  I32 io_obuffer_size;
  CHAR* directory;
  CHAR* file_name;
  CHAR* appendix;
  U32 cut;
  LASBOOL opts;
  LASBOOL optx;
  CHAR* parse_string;
  CHAR* separator;
  F32 scale_rgb;
  U32 format;
  LASBOOL specified;
  LASBOOL force;
  LASBOOL native;
  U32 chunk_size;
  LASBOOL use_stdout;
  LASBOOL use_nil;
  LASBOOL buffered;
};

#endif
