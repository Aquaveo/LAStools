/*
===============================================================================

  FILE:  lasinterval.hpp
  
  CONTENTS:
  
    Used by lasindex to manage intervals of consecutive LiDAR points that are
    read sequentially.

  PROGRAMMERS:

    info@rapidlasso.de  -  https://rapidlasso.de

  COPYRIGHT:

    (c) 2007-2022, rapidlasso GmbH - fast tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the Apache Public License 2.0 published by the Apache Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    20 October 2018 -- fixed rare bug in merge_intervals() when verbose is TRUE
    29 April 2011 -- created after cable outage during the royal wedding (-:
  
===============================================================================
*/
#ifndef LAS_INTERVAL_HPP
#define LAS_INTERVAL_HPP

#include "mydefs.hpp"

class ByteStreamIn;
class ByteStreamOut;

class LASintervalCell
{
public:
  U32 start;
  U32 end;
  LASintervalCell* next;
  LASintervalCell();
  LASintervalCell(const U32 p_index);
  LASintervalCell(const LASintervalCell* cell);
};

class LASintervalStartCell : public LASintervalCell
{
public:
  U32 full;
  U32 total;
  LASintervalCell* last;
  LASintervalStartCell();
  LASintervalStartCell(const U32 p_index);
  LASBOOL add(const U32 p_index, const U32 threshold=1000);
};

class LASinterval
{
public:
  LASinterval(const U32 threshold=1000);
  ~LASinterval();

  // add points and create cells with intervals
  LASBOOL add(const U32 p_index, const I32 c_index);

  // get total number of cells
  U32 get_number_cells() const;

  // get total number of intervals
  U32 get_number_intervals() const;

  // merge cells (and their intervals) into one cell
  LASBOOL merge_cells(const U32 num_indices, const I32* indices, const I32 new_index);

  // merge adjacent intervals with small gaps in cells to reduce total interval number to maximum
  void merge_intervals(U32 maximum, const LASBOOL verbose=TRUE);

  // read from file or write to file
  LASBOOL read(ByteStreamIn* stream);
  LASBOOL write(ByteStreamOut* stream) const;

  // get one cell after the other
  void get_cells();
  LASBOOL has_cells();

  // get a particular cell
  LASBOOL get_cell(const I32 c_index);

  // add cell's intervals to those that will be merged 
  LASBOOL add_current_cell_to_merge_cell_set();
  LASBOOL add_cell_to_merge_cell_set(const I32 c_index, const LASBOOL erase=FALSE);
  LASBOOL merge(const LASBOOL erase=FALSE);
  void clear_merge_cell_set();
  LASBOOL get_merged_cell();

  // iterate intervals of current cell (or over merged intervals)
  LASBOOL has_intervals();

  I32 index;
  U32 start;
  U32 end;
  U32 full;
  U32 total;

private:
  void* cells;
  void* cells_to_merge;
  U32 threshold;
  U32 number_intervals;
  I32 last_index;
  LASintervalStartCell* last_cell;
  LASintervalCell* current_cell;
  LASintervalStartCell* merged_cells;
  LASBOOL merged_cells_temporary;
};

#endif
