/*
===============================================================================

  FILE:  bytestreamout_ostream.hpp
  
    Class for ostream-based output streams with endian handling.

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
  
     1 October 2011 -- added 64 bit file support in MSVC 6.0 at McCafe at Hbf Linz
    10 January 2011 -- licensing change for LGPL release and liblas integration
    12 December 2010 -- created from ByteStreamOutFile after Howard got pushy (-;
  
===============================================================================
*/
#ifndef BYTE_STREAM_OUT_OSTREAM_H
#define BYTE_STREAM_OUT_OSTREAM_H

#include "bytestreamout.hpp"

#ifdef LZ_WIN32_VC6
#include <fstream.h>
#else
#include <istream>
#include <fstream>
#endif

class ByteStreamOutOstream : public ByteStreamOut
{
public:
  ByteStreamOutOstream(std::ostream& stream);
/* write a single byte                                       */
  LASBOOL putByte(U8 byte);
/* write an array of bytes                                   */
  LASBOOL putBytes(const U8* bytes, U32 num_bytes);
/* is the stream seekable (e.g. standard out is not)         */
  LASBOOL isSeekable() const;
/* get current position of stream                            */
  I64 tell() const;
/* seek to this position in the stream                       */
  LASBOOL seek(const I64 position);
/* seek to the end of the file                               */
  LASBOOL seekEnd();
/* destructor                                                */
  ~ByteStreamOutOstream(){};
protected:
  std::ostream& stream;
};

class ByteStreamOutOstreamLE : public ByteStreamOutOstream
{
public:
  ByteStreamOutOstreamLE(std::ostream& stream);
/* write 16 bit low-endian field                             */
  LASBOOL put16bitsLE(const U8* bytes);
/* write 32 bit low-endian field                             */
  LASBOOL put32bitsLE(const U8* bytes);
/* write 64 bit low-endian field                             */
  LASBOOL put64bitsLE(const U8* bytes);
/* write 16 bit big-endian field                             */
  LASBOOL put16bitsBE(const U8* bytes);
/* write 32 bit big-endian field                             */
  LASBOOL put32bitsBE(const U8* bytes);
/* write 64 bit big-endian field                             */
  LASBOOL put64bitsBE(const U8* bytes);
private:
  U8 swapped[8];
};

class ByteStreamOutOstreamBE : public ByteStreamOutOstream
{
public:
  ByteStreamOutOstreamBE(std::ostream& stream);
/* write 16 bit low-endian field                             */
  LASBOOL put16bitsLE(const U8* bytes);
/* write 32 bit low-endian field                             */
  LASBOOL put32bitsLE(const U8* bytes);
/* write 64 bit low-endian field                             */
  LASBOOL put64bitsLE(const U8* bytes);
/* write 16 bit big-endian field                             */
  LASBOOL put16bitsBE(const U8* bytes);
/* write 32 bit big-endian field                             */
  LASBOOL put32bitsBE(const U8* bytes);
/* write 64 bit big-endian field                             */
  LASBOOL put64bitsBE(const U8* bytes);
private:
  U8 swapped[8];
};

inline ByteStreamOutOstream::ByteStreamOutOstream(std::ostream& stream_param) :
    stream(stream_param)
{
}

inline LASBOOL ByteStreamOutOstream::putByte(U8 byte)
{
  stream.put(byte);
  return stream.good();
}

inline LASBOOL ByteStreamOutOstream::putBytes(const U8* bytes, U32 num_bytes)
{
  stream.write((const char*)bytes, num_bytes);
  return stream.good();
}

inline LASBOOL ByteStreamOutOstream::isSeekable() const
{
  return !!(static_cast<std::ofstream&>(stream));
}

inline I64 ByteStreamOutOstream::tell() const
{
  return (I64)stream.tellp();
}

inline LASBOOL ByteStreamOutOstream::seek(I64 position)
{
  stream.seekp(static_cast<std::streamoff>(position));
  return stream.good();
}

inline LASBOOL ByteStreamOutOstream::seekEnd()
{
  stream.seekp(0, std::ios::end);
  return stream.good();
}

inline ByteStreamOutOstreamLE::ByteStreamOutOstreamLE(std::ostream& stream) : ByteStreamOutOstream(stream)
{
}

inline LASBOOL ByteStreamOutOstreamLE::put16bitsLE(const U8* bytes)
{
  return putBytes(bytes, 2);
}

inline LASBOOL ByteStreamOutOstreamLE::put32bitsLE(const U8* bytes)
{
  return putBytes(bytes, 4);
}

inline LASBOOL ByteStreamOutOstreamLE::put64bitsLE(const U8* bytes)
{
  return putBytes(bytes, 8);
}

inline LASBOOL ByteStreamOutOstreamLE::put16bitsBE(const U8* bytes)
{
  swapped[0] = bytes[1];
  swapped[1] = bytes[0];
  return putBytes(swapped, 2);
}

inline LASBOOL ByteStreamOutOstreamLE::put32bitsBE(const U8* bytes)
{
  swapped[0] = bytes[3];
  swapped[1] = bytes[2];
  swapped[2] = bytes[1];
  swapped[3] = bytes[0];
  return putBytes(swapped, 4);
}

inline LASBOOL ByteStreamOutOstreamLE::put64bitsBE(const U8* bytes)
{
  swapped[0] = bytes[7];
  swapped[1] = bytes[6];
  swapped[2] = bytes[5];
  swapped[3] = bytes[4];
  swapped[4] = bytes[3];
  swapped[5] = bytes[2];
  swapped[6] = bytes[1];
  swapped[7] = bytes[0];
  return putBytes(swapped, 8);
}

inline ByteStreamOutOstreamBE::ByteStreamOutOstreamBE(std::ostream& stream) : ByteStreamOutOstream(stream)
{
}

inline LASBOOL ByteStreamOutOstreamBE::put16bitsLE(const U8* bytes)
{
  swapped[0] = bytes[1];
  swapped[1] = bytes[0];
  return putBytes(swapped, 2);
}

inline LASBOOL ByteStreamOutOstreamBE::put32bitsLE(const U8* bytes)
{
  swapped[0] = bytes[3];
  swapped[1] = bytes[2];
  swapped[2] = bytes[1];
  swapped[3] = bytes[0];
  return putBytes(swapped, 4);
}

inline LASBOOL ByteStreamOutOstreamBE::put64bitsLE(const U8* bytes)
{
  swapped[0] = bytes[7];
  swapped[1] = bytes[6];
  swapped[2] = bytes[5];
  swapped[3] = bytes[4];
  swapped[4] = bytes[3];
  swapped[5] = bytes[2];
  swapped[6] = bytes[1];
  swapped[7] = bytes[0];
  return putBytes(swapped, 8);
}

inline LASBOOL ByteStreamOutOstreamBE::put16bitsBE(const U8* bytes)
{
  return putBytes(bytes, 2);
}

inline LASBOOL ByteStreamOutOstreamBE::put32bitsBE(const U8* bytes)
{
  return putBytes(bytes, 4);
}

inline LASBOOL ByteStreamOutOstreamBE::put64bitsBE(const U8* bytes)
{
  return putBytes(bytes, 8);
}

#endif
