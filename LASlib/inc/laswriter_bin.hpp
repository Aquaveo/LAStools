/*
===============================================================================

  FILE:  laswriter_bin.hpp
  
  CONTENTS:
  
    Writes LIDAR points from to ASCII through on-the-fly conversion from LAS.

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
  
    13 October 2014 -- changed default IO buffer size with setvbuf() to 262144
    5 November 2011 -- changed default IO buffer size with setvbuf() to 65536
    5 September 2011 -- created after sampling grapes in the sommerhausen hills  
  
===============================================================================
*/
#ifndef LAS_WRITER_BIN_HPP
#define LAS_WRITER_BIN_HPP

#include "laswriter.hpp"

#include <stdio.h>

class ByteStreamOut;

class LASwriterBIN : public LASwriter
{
public:

  LASBOOL refile(FILE* file);

  LASBOOL open(const char* file_name, const LASheader* header, const char* version, U32 io_buffer_size=LAS_TOOLS_IO_OBUFFER_SIZE );
  LASBOOL open(FILE* file, const LASheader* header, const char* version);
  LASBOOL open(ByteStreamOut* stream, const LASheader* header, const char* version);

  LASBOOL write_point(const LASpoint* point);
  LASBOOL chunk() { return FALSE; };

  LASBOOL update_header(const LASheader* header, LASBOOL use_inventory=FALSE, LASBOOL update_extra_bytes=FALSE);
  I64 close(LASBOOL update_npoints=TRUE);

  LASwriterBIN();
  ~LASwriterBIN();

private:
  ByteStreamOut* stream;
  FILE* file;
  U32 version;
  I32 units;
  F64 origin_x;
  F64 origin_y;
  F64 origin_z;
};

#endif
