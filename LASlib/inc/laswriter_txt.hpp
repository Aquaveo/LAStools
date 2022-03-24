/*
===============================================================================

  FILE:  laswriter_txt.hpp

  CONTENTS:

    Writes LIDAR points to ASCII through on-the-fly conversion from LAS.

  PROGRAMMERS:

    info@rapidlasso.de  -  https://rapidlasso.de

  COPYRIGHT:

    (c) 2007-2014, rapidlasso GmbH - fast tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the LICENSE.txt file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  CHANGE HISTORY:

     7 September 2018 -- replaced calls to _strdup with calls to the LASCopyString macro
    10 April 2011 -- created after a sunny weekend of biking to/from Buergel

===============================================================================
*/
#ifndef LAS_WRITER_TXT_HPP
#define LAS_WRITER_TXT_HPP

#include "laswriter.hpp"

#include <stdio.h>

class LASwriterTXT : public LASwriter
{
public:

  void set_pts(LASBOOL pts);
  void set_ptx(LASBOOL ptx);
  void set_scale_rgb(F32 scale_rgb);

  LASBOOL refile(FILE* file);

  LASBOOL open(const CHAR* file_name, const LASheader* header, const CHAR* parse_string=0, const CHAR* separator=0);
  LASBOOL open(FILE* file, const LASheader* header, const CHAR* parse_string=0, const CHAR* separator=0);

  LASBOOL write_point(const LASpoint* point);
  LASBOOL chunk() { return FALSE; };

  LASBOOL update_header(const LASheader* header, LASBOOL use_inventory=FALSE, LASBOOL update_extra_bytes=FALSE);
  I64 close(LASBOOL update_npoints=TRUE);

  LASwriterTXT();
  ~LASwriterTXT();

private:
  LASBOOL close_file;
  FILE* file;
  const LASheader* header;
  CHAR* parse_string;
  LASBOOL opts;
  LASBOOL optx;
  F32 scale_rgb;
  CHAR separator_sign;
  CHAR printstring[512];
  I32 attribute_starts[10];
  LASBOOL check_parse_string(const CHAR* parse_string);
  LASBOOL unparse_attribute(const LASpoint* point, I32 index);
};

#endif
