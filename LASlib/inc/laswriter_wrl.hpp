/*
===============================================================================

  FILE:  laswriter_wrl.hpp
  
  CONTENTS:
  
    Writes LIDAR points to VRML through on-the-fly conversion from LAS.

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
  
    26 August 2012 -- after a slice of cake at the Volksfest Sommerhausen
  
===============================================================================
*/
#ifndef LAS_WRITER_WRL_HPP
#define LAS_WRITER_WRL_HPP

#include "laswriter.hpp"

#include <stdio.h>

class LASwriterWRL : public LASwriter
{
public:

  LASBOOL open(const CHAR* file_name, const LASheader* header, const CHAR* parse_string=0);
  LASBOOL open(FILE* file, const LASheader* header, const CHAR* parse_string=0);

  LASBOOL write_point(const LASpoint* point);
  LASBOOL chunk() { return FALSE; };

  LASBOOL update_header(const LASheader* header, LASBOOL use_inventory=FALSE, LASBOOL update_extra_bytes=FALSE);
  I64 close(LASBOOL update_npoints=TRUE);

  LASwriterWRL();
  ~LASwriterWRL();

private:
  LASBOOL close_file;
  FILE* file;
  const LASheader* header;
  CHAR printstring[512];
  U8* rgb;
  I32 rgb_alloc;
};

#endif
