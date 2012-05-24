/*
    Copyright 2007-2012 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_READ_AND_CONVERT_VIEW_HPP
#define BOOST_GIL_EXTENSION_IO_READ_AND_CONVERT_VIEW_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>

#include "base.hpp"
#include "io_device.hpp"
#include "path_spec.hpp"
#include "conversion_policies.hpp"

namespace boost{ namespace gil {

/// \ingroup IO

/// \brief Reads and color-converts an image view. No memory is allocated.
/// \param reader    An image reader.
/// \param img       The image in which the data is read into.
/// \param settings  Specifies read settings depending on the image format.
/// \param cc        Color converter function object.
/// \throw std::ios_base::failure
template< typename Reader
        , typename View
        >
inline
void read_and_convert_image( Reader&     reader
                           , const View& view
                           , typename enable_if< mpl::and_< detail::is_reader< Reader >
                                                          , is_format_tag< typename Reader::format_tag_t >
                                                          >
                           >::type* /* ptr */ = 0
                           )
{
    reader.init_view( view
                    , reader._settings
                    );

    reader.apply( view );
}

/// \brief Reads and color-converts an image view. No memory is allocated.
/// \param file      It's a device. Must satisfy is_input_device metafunction.
/// \param view      The image view in which the data is read into.
/// \param settings  Specifies read settings depending on the image format.
/// \param cc        Color converter function object.
/// \throw std::ios_base::failure
template< typename Device
        , typename View
        , typename ColorConverter
        , typename FormatTag
        >
inline
void read_and_convert_view( Device&                                 file
                          , const View&                             view
                          , const image_read_settings< FormatTag >& settings
                          , const ColorConverter&                   cc
                          , typename enable_if< mpl::and_< detail::is_read_device< FormatTag
                                                                                 , Device
                                                                                 >
                                                         , is_format_tag< FormatTag >
                                                         >
                                              >::type* /* ptr */ = 0

                          )
{
    read_and_convert_image( make_reader( file
                                       , settings
                                       , detail::read_and_convert( cc )
                                       )
                          , view
                          );
}

/// \brief Reads and color-converts an image view. No memory is allocated.
/// \param file_name File name. Must satisfy is_supported_path_spec metafunction.
/// \param view      The image view in which the data is read into.
/// \param settings  Specifies read settings depending on the image format.
/// \param cc        Color converter function object.
/// \throw std::ios_base::failure
template < typename String
         , typename View
         , typename ColorConverter
         , typename FormatTag
         >
inline
void read_and_convert_view( const String&                           file_name
                          , const View&                             view
                          , const image_read_settings< FormatTag >& settings
                          , const ColorConverter&                   cc
                          , typename enable_if< mpl::and_< is_format_tag< FormatTag >
                                                         , detail::is_supported_path_spec< String >
                                                         >
                                            >::type* /* ptr */ = 0
                          )
{
    read_and_convert_image( make_reader( file_name
                                       , settings
                                       , detail::read_and_convert( cc )
                                       )
                          , view
                          );
}

/// \brief Reads and color-converts an image view. No memory is allocated.
/// \param file_name File name. Must satisfy is_supported_path_spec metafunction.
/// \param view      The image view in which the data is read into.
/// \param cc        Color converter function object.
/// \param tag       Defines the image format. Must satisfy is_format_tag metafunction. 
/// \throw std::ios_base::failure
template < typename String
         , typename View
         , typename ColorConverter
         , typename FormatTag
         >
inline
void read_and_convert_view( const String&         file_name
                          , const View&           view
                          , const ColorConverter& cc
                          , const FormatTag&      tag
                          , typename enable_if< mpl::and_< is_format_tag< FormatTag >
                                                         , detail::is_supported_path_spec< String >
                                                         >
                                            >::type* /* ptr */ = 0
                          )
{
    read_and_convert_image( make_reader( file_name
                                       , tag
                                       , detail::read_and_convert( cc )
                                       )
                          , view
                          );
}

/// \brief Reads and color-converts an image view. No memory is allocated.
/// \param file It's a device. Must satisfy is_input_device metafunction or is_adaptable_input_device.
/// \param view The image view in which the data is read into.
/// \param cc   Color converter function object.
/// \param tag  Defines the image format. Must satisfy is_format_tag metafunction. 
/// \throw std::ios_base::failure
template < typename Device
         , typename View
         , typename ColorConverter
         , typename FormatTag
         >
inline
void read_and_convert_view( Device&               device
                          , const View&           view
                          , const ColorConverter& cc
                          , const FormatTag&      tag
                          , typename enable_if< mpl::and_< detail::is_read_device< FormatTag
                                                                                 , Device
                                                                                 >
                                                         , is_format_tag< FormatTag >
                                                         >
                                               >::type* /* ptr */ = 0
                          )
{
    read_and_convert_image( make_reader( device
                                       , tag
                                       , detail::read_and_convert( cc )
                                       )
                          , view
                          );
}

/// \brief Reads and color-converts an image view. No memory is allocated.
/// \param file_name File name. Must satisfy is_supported_path_spec metafunction.
/// \param view      The image view in which the data is read into.
/// \param settings  Specifies read settings depending on the image format.
/// \throw std::ios_base::failure
template < typename String
         , typename View
         , typename FormatTag
         >
inline
void read_and_convert_view( const String&                           file_name
                          , const View&                             view
                          , const image_read_settings< FormatTag >& settings
                          , typename enable_if< mpl::and_< is_format_tag< FormatTag >
                                                         , detail::is_supported_path_spec< String >
                                                         >
                                            >::type* /* ptr */ = 0
                          )
{
    read_and_convert_image( make_reader( file_name
                                       , settings
                                       , detail::read_and_convert< default_color_converter >()
                                       )
                          , view
                          );
}

/// \brief Reads and color-converts an image view. No memory is allocated.
/// \param file      It's a device. Must satisfy is_input_device metafunction or is_adaptable_input_device.
/// \param view      The image view in which the data is read into.
/// \param settings  Specifies read settings depending on the image format.
/// \throw std::ios_base::failure
template < typename Device
         , typename View
         , typename FormatTag
         >
inline
void read_and_convert_view( Device&                                 device
                          , const View&                             view
                          , const image_read_settings< FormatTag >& settings
                          , typename enable_if< mpl::and_< detail::is_read_device< FormatTag
                                                                                 , Device
                                                                                 >
                                                         , is_format_tag< FormatTag >
                                                         >
                                               >::type* /* ptr */ = 0
                          )
{
    read_and_convert_image( make_reader( device
                                       , settings
                                       , detail::read_and_convert< default_color_converter >()
                                       )
                          , view
                          );
}

/// \brief Reads and color-converts an image view. No memory is allocated.
/// \param file_name File name. Must satisfy is_supported_path_spec metafunction.
/// \param view      The image view in which the data is read into.
/// \param tag       Defines the image format. Must satisfy is_format_tag metafunction. 
/// \throw std::ios_base::failure
template < typename String
         , typename View
         , typename FormatTag
         >
inline
void read_and_convert_view( const String&    file_name
                          , const View&      view
                          , const FormatTag& tag
                          , typename enable_if< mpl::and_< is_format_tag< FormatTag >
                                                         , detail::is_supported_path_spec< String >
                                                         >
                                              >::type* /* ptr */ = 0
                          )
{
    read_and_convert_image( make_reader( file_name
                                       , tag
                                       , detail::read_and_convert< default_color_converter >()
                                       )
                          , view
                          );
}

/// \brief Reads and color-converts an image view. No memory is allocated.
/// \param file It's a device. Must satisfy is_input_device metafunction or is_adaptable_input_device.
/// \param view The image view in which the data is read into.
/// \param tag  Defines the image format. Must satisfy is_format_tag metafunction. 
/// \throw std::ios_base::failure
template < typename Device
         , typename View
         , typename FormatTag
         >
inline
void read_and_convert_view( Device&          device
                          , const View&      view
                          , const FormatTag& tag
                          , typename enable_if< mpl::and_< detail::is_read_device< FormatTag
                                                                                 , Device
                                                                                 >
                                                         , is_format_tag< FormatTag >
                                                         >
                                               >::type* /* ptr */ = 0
                          )
{
    read_and_convert_image( make_reader( device
                                       , tag
                                       , detail::read_and_convert< default_color_converter >()
                                       )
                          , view
                          );
}

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_READ_AND_CONVERT_VIEW_HPP
