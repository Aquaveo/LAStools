/*
===============================================================================

  FILE:  laswriter_qfit.hpp
  
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
  
    23 December 2011 -- created after by new OGIO laptop bagpack arrived 
  
===============================================================================
*/
#ifndef LAS_WRITER_QFIT_HPP
#define LAS_WRITER_QFIT_HPP

#include "laswriter.hpp"

#include <stdio.h>

class ByteStreamOut;

class LASwriterQFIT : public LASwriter
{
public:

  LASBOOL refile(FILE* file);

  LASBOOL open(const char* file_name, const LASheader* header, I32 version=48, U32 io_buffer_size=65536);
  LASBOOL open(FILE* file, const LASheader* header, I32 version=48);
  LASBOOL open(ByteStreamOut* stream, const LASheader* header, I32 version=48);

  LASBOOL write_point(const LASpoint* point);
  LASBOOL chunk() { return FALSE; };

  LASBOOL update_header(const LASheader* header, LASBOOL use_inventory=FALSE, LASBOOL update_extra_bytes=FALSE);
  I64 close(LASBOOL update_npoints=TRUE);

  LASwriterQFIT();
  ~LASwriterQFIT();

private:
  ByteStreamOut* stream;
  FILE* file;
  I32 version;
  LASBOOL endian_swap;
  LASBOOL rescale_reoffset;
  I32 buffer[14];
  I32 scan_azimuth_array_offset;
  I32 pitch_array_offset;
  I32 roll_array_offset;
  I32 pulse_width_array_offset;
};

#endif
