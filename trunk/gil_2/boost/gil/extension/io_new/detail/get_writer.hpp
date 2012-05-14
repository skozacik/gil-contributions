/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_GET_WRITER_HPP
#define BOOST_GIL_EXTENSION_IO_GET_WRITER_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include "get_write_device.hpp"

namespace boost { namespace gil {

///
/// Helper metafunction to generate image writer type.
///
template< typename T
        , typename FormatTag
        >
struct get_writer
{
    typedef typename get_write_device< T, FormatTag >::type device_t;

    typedef writer< device_t
                  , FormatTag
                  > type;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_GET_WRITER_HPP
