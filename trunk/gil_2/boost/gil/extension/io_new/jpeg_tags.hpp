/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_TAGS_HPP_INCLUDED 
#define BOOST_GIL_EXTENSION_IO_JPEG_TAGS_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief All supported jpeg tags by the gil io extension.
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include <jpeglib.h>
}

#include "detail/base.hpp"

namespace boost { namespace gil {

struct jpeg_tag : format_tag {};

struct jpeg_image_width
{
   typedef JDIMENSION type;
};

struct jpeg_image_height
{
   typedef JDIMENSION type;
};

struct jpeg_num_components
{
   typedef int type;
};

struct jpeg_color_space
{
   typedef J_COLOR_SPACE type;
};

struct jpeg_quality
{
   typedef int type;
};

struct jpeg_data_precision
{
   typedef int type;
};

struct jpeg_dct_method
{
    typedef J_DCT_METHOD type;

    static const unsigned int slow        = JDCT_ISLOW;
    static const unsigned int fast        = JDCT_IFAST;
    static const unsigned int floating_pt = JDCT_FLOAT;
}

template<>
struct image_read_info<jpeg_tag> 
{
   jpeg_image_width::type  _width;
   jpeg_image_height::type _height;

   jpeg_num_components::type _num_components;
   jpeg_color_space::type    _color_space;

   jpeg_data_precision::type _data_precision;
};


template<>
struct image_read_settings<jpeg_tag> 
{
    image_read_settings<jpeg_tag>()
    : _top_left  ( 0, 0 )
    , _dim       ( 0, 0 )
    , _dct_method( slow )
    {}

    point_t _top_left;
    point_t _dim;

    jpeg_dct_method::type _dct_method;
};


template<>
struct image_write_info<jpeg_tag>
{
   jpeg_quality::type _quality;
};


} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_JPEG_TAGS_HPP_INCLUDED
