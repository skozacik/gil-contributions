/*
    Copyright 2007-2012 Olivier Tournaire, Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_RAW_IO_DEVICE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_RAW_IO_DEVICE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Olivier Tournaire \n
///
/// \date   2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

// taken from jpegxx - https://bitbucket.org/edd/jpegxx/src/ea2492a1a4a6/src/ijg_headers.hpp
#ifndef BOOST_GIL_EXTENSION_IO_RAW_C_LIB_COMPILED_AS_CPLUSPLUS
    extern "C" {
#else
    // DONT_USE_EXTERN_C introduced in v7 of the IJG library.
    // By default the v7 IJG headers check for __cplusplus being defined and
    // wrap the content in an 'extern "C"' block if it's present.
    // When DONT_USE_EXTERN_C is defined, this wrapping is not performed.
    #ifndef DONT_USE_EXTERN_C
        #define DONT_USE_EXTERN_C 1
    #endif
#endif

#include <libraw/libraw.h>

#ifndef BOOST_GIL_EXTENSION_IO_RAW_C_LIB_COMPILED_AS_CPLUSPLUS
    }
#endif


#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

namespace boost { namespace gil { namespace detail {

class raw_device_base
{
public:

    ///
    /// Constructor
    ///
    raw_device_base() {}

    ///
    /// Constructor
    ///
    raw_device_base( const LibRaw& processor )
    : m_libraw_processor(processor)
    {}

    // iparams getters
    std::string get_camera_manufacturer() { return std::string(m_libraw_processor.imgdata.idata.make);  }
    std::string get_camera_model()        { return std::string(m_libraw_processor.imgdata.idata.model); }
    unsigned get_raw_count()              { return m_libraw_processor.imgdata.idata.raw_count; }
    unsigned get_dng_version()            { return m_libraw_processor.imgdata.idata.dng_version; }
    int get_colors()                      { return m_libraw_processor.imgdata.idata.colors; }
    unsigned get_filters()                { return m_libraw_processor.imgdata.idata.filters; }
    std::string get_cdesc()               { return std::string(m_libraw_processor.imgdata.idata.cdesc); }

    // image_sizes getters
    unsigned short get_raw_width()    { return m_libraw_processor.imgdata.sizes.raw_width;  }
    unsigned short get_raw_height()   { return m_libraw_processor.imgdata.sizes.raw_height; }
    unsigned short get_image_width()  { return m_libraw_processor.imgdata.sizes.width;  }
    unsigned short get_image_height() { return m_libraw_processor.imgdata.sizes.height; }
    unsigned short get_top_margin()   { return m_libraw_processor.imgdata.sizes.top_margin;  }
    unsigned short get_left_margin()  { return m_libraw_processor.imgdata.sizes.left_margin; }
    unsigned short get_iwidth()       { return m_libraw_processor.imgdata.sizes.iwidth;  }
    unsigned short get_iheight()      { return m_libraw_processor.imgdata.sizes.iheight; }
    double get_pixel_aspect()         { return m_libraw_processor.imgdata.sizes.pixel_aspect;  }
    int get_flip()                    { return m_libraw_processor.imgdata.sizes.flip; }

    // colordata getters
    // TODO

    // imgother getters
    float get_iso_speed()     { return m_libraw_processor.imgdata.other.iso_speed; }
    float get_shutter()       { return m_libraw_processor.imgdata.other.shutter; }
    float get_aperture()      { return m_libraw_processor.imgdata.other.aperture; }
    float get_focal_len()     { return m_libraw_processor.imgdata.other.focal_len; }
    time_t get_timestamp()    { return m_libraw_processor.imgdata.other.timestamp; }
    unsigned get_shot_order() { return m_libraw_processor.imgdata.other.shot_order; }
    unsigned* get_gpsdata()   { return m_libraw_processor.imgdata.other.gpsdata; }
    std::string get_desc()    { return std::string(m_libraw_processor.imgdata.other.desc); }
    std::string get_artist()  { return std::string(m_libraw_processor.imgdata.other.artist); }

    std::string get_version()               { return std::string(m_libraw_processor.version()); }
    std::string get_unpack_function_name()  { return std::string(m_libraw_processor.unpack_function_name()); }

    void get_mem_image_format(int *widthp, int *heightp, int *colorsp, int *bpp) { m_libraw_processor.get_mem_image_format(widthp, heightp, colorsp, bpp); }

    int unpack()                                                         { return m_libraw_processor.unpack(); }
    int dcraw_process()                                                  { return m_libraw_processor.dcraw_process(); }
    libraw_processed_image_t* dcraw_make_mem_image(int* error_code=NULL) { return m_libraw_processor.dcraw_make_mem_image(error_code); }

protected:

    LibRaw m_libraw_processor;
};

/*!
 *
 * file_stream_device specialization for raw images
 */
template<>
class file_stream_device< raw_tag > : public raw_device_base
{
public:

    struct read_tag {};

    ///
    /// Constructor
    ///
    file_stream_device( std::string const& file_name
                      , read_tag   = read_tag()
                      )
    {
        io_error_if( m_libraw_processor.open_file( file_name.c_str() ) != LIBRAW_SUCCESS 
                   , "file_stream_device: failed to open file"
                   );
    }

    ///
    /// Constructor
    ///
    file_stream_device( const char* file_name
                      , read_tag   = read_tag()
                      )
    {
        io_error_if( m_libraw_processor.open_file( file_name ) != LIBRAW_SUCCESS 
                   , "file_stream_device: failed to open file"
                   );
    }

    ///
    /// Constructor
    ///
    file_stream_device( const LibRaw& processor ) 
    : raw_device_base( processor )
    {}
};

template< typename FormatTag >
struct is_adaptable_input_device< FormatTag
                                , LibRaw
                                , void
                                >
    : mpl::true_
{
    typedef file_stream_device< FormatTag > device_type;
};


} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_RAW_IO_DEVICE_HPP_INCLUDED
