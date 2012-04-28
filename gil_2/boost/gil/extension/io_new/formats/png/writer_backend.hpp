/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNG_IO_WRITER_BACKEND_HPP
#define BOOST_GIL_EXTENSION_IO_PNG_IO_WRITER_BACKEND_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/extension/io_new/png_tags.hpp>

namespace boost { namespace gil {

///
/// PNG Writer Backend
///
template< typename Device >
struct writer_backend< Device
                     , png_tag
                     >
{
    writer_backend( Device& io_dev
                  , const image_write_info< png_tag >& info
                  )
    : _io_dev( io_dev )
    , _info( info )
    , _png_ptr( NULL )
    , _info_ptr( NULL )
    {
        // Create and initialize the png_struct with the desired error handler
        // functions.  If you want to use the default stderr and longjump method,
        // you can supply NULL for the last three parameters.  We also check that
        // the library version is compatible with the one used at compile time,
        // in case we are using dynamically linked libraries.  REQUIRED.
        _png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING
                                          , NULL  // user_error_ptr
                                          , NULL  // user_error_fn
                                          , NULL  // user_warning_fn
                                          );

        io_error_if( _png_ptr == NULL
                   , "png_writer: fail to call png_create_write_struct()"
                   );

        // Allocate/initialize the image information data.  REQUIRED 
        _info_ptr = png_create_info_struct( _png_ptr );

        if( _info_ptr == NULL )
        {
            png_destroy_write_struct( &_png_ptr
                                    , NULL
                                    );

            io_error( "png_writer: fail to call png_create_info_struct()" );
        }

        // Set error handling.  REQUIRED if you aren't supplying your own
        // error handling functions in the png_create_write_struct() call.
        if( setjmp( png_jmpbuf( _png_ptr )))
        {
            //free all of the memory associated with the png_ptr and info_ptr
            png_destroy_write_struct( &_png_ptr
                                    , &_info_ptr
                                    );

            io_error( "png_writer: fail to call setjmp()" );
        }

        init_io( _png_ptr );
    }

    ~writer_backend()
    {
        png_destroy_write_struct( &_png_ptr
                                , &_info_ptr
                                );
    }

protected:

    void write_header()
    {
        // Set the image information here.  Width and height are up to 2^31,
        // bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
        // the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
        // PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
        // or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
        // PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
        // currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
        png_set_IHDR( _png_ptr
                    , _info_ptr
                    , static_cast< png_image_width::type  >( view.width()  )
                    , static_cast< png_image_height::type >( view.height() )
                    , static_cast< png_bitdepth::type     >( png_rw_info_t::_bit_depth )
                    , static_cast< png_color_type::type   >( png_rw_info_t::_color_type )
                    , info._interlace_method
                    , info._compression_method
                    , info._filter_method
                    );

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
        if( _info._valid_cie_colors )
        {
            png_set_cHRM( _png_ptr
                        , _info_ptr
                        , _info._white_x
                        , _info._white_y
                        , _info._red_x
                        , _info._red_y
                        , _info._green_x
                        , _info._green_y
                        , _info._blue_x
                        , _info._blue_y
                        );
        }

        if( _info._valid_file_gamma )
        {
            png_set_gAMA( _png_ptr
                        , _info_ptr
                        , _info._gamma
                        );
        }
#else
        if( _info._valid_cie_colors )
        {
            png_set_cHRM_fixed( _png_ptr
                              , _info_ptr
                              , _info._white_x
                              , _info._white_y
                              , _info._red_x
                              , _info._red_y
                              , _info._green_x
                              , _info._green_y
                              , _info._blue_x
                              , _info._blue_y
                              );
        }

        if( _info._valid_file_gamma )
        {
            png_set_gAMA_fixed( _png_ptr
                              , _info_ptr
                              , _info._file_gamma
                              );
        }
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

        if( _info._valid_icc_profile )
        {
#if PNG_LIBPNG_VER_MINOR >= 5
            png_set_iCCP( _png_ptr
                        , _info_ptr
                        , const_cast< png_charp >( _info._icc_name.c_str() )
                        , _info._iccp_compression_type
                        , reinterpret_cast< png_const_bytep >( _info._profile.c_str() )
                        , _info._profile_length
                        );
#else
            png_set_iCCP( _png_ptr
                        , _info_ptr
                        , const_cast< png_charp >( _info._icc_name.c_str() )
                        , _info._iccp_compression_type
                        , const_cast< png_charp >( _info._profile.c_str() )
                        , _info._profile_length
                        );
#endif
        }

        if( _info._valid_intent )
        {
            png_set_sRGB( _png_ptr
                        , _info_ptr
                        , _info._intent
                        );
        }

        if( _info._valid_palette )
        {
            png_set_PLTE( _png_ptr
                        , _info_ptr
                        , const_cast< png_colorp >( &_info._palette.front() )
                        , _info._num_palette
                        );
        }

        if( _info._valid_background )
        {
            png_set_bKGD( _png_ptr
                        , _info_ptr
                        , const_cast< png_color_16p >( &_info._background )
                        );
        }

        if( _info._valid_histogram )
        {
            png_set_hIST( _png_ptr
                        , _info_ptr
                        , const_cast< png_uint_16p >( &_info._histogram.front() )
                        );
        }

        if( _info._valid_offset )
        {
            png_set_oFFs( _png_ptr
                        , _info_ptr
                        , _info._offset_x
                        , _info._offset_y
                        , _info._off_unit_type
                        );
        }

        if( _info._valid_pixel_calibration )
        {
            std::vector< const char* > params( _info._num_params );
            for( std::size_t i = 0; i < params.size(); ++i )
            {
                params[i] = _info._params[ i ].c_str();
            }

            png_set_pCAL( _png_ptr
                        , _info_ptr
                        , const_cast< png_charp >( _info._purpose.c_str() )
                        , _info._X0
                        , _info._X1
                        , _info._cal_type
                        , _info._num_params
                        , const_cast< png_charp  >( _info._units.c_str() )
                        , const_cast< png_charpp >( &params.front()     )
                        );
        }

        if( _info._valid_resolution )
        {
            png_set_pHYs( _png_ptr
                        , _info_ptr
                        , _info._res_x
                        , _info._res_y
                        , _info._phy_unit_type
                        );
        }

        if( _info._valid_significant_bits )
        {
            png_set_sBIT( _png_ptr
                        , _info_ptr
                        , const_cast< png_color_8p >( &_info._sig_bits )
                        );
        }

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED 
        if( _info._valid_scale_factors )
        {
            png_set_sCAL( _png_ptr
                        , _info_ptr
                        , _info._scale_unit
                        , _info._scale_width
                        , _info._scale_height
                        );
        }
#else
#ifdef BOOST_GIL_IO_PNG_FIXED_POINT_SUPPORTED

        if( _info._valid_scale_factors )
        {
            png_set_sCAL_s( _png_ptr
                          , _info_ptr
                          , _scale_unit
                          , const_cast< png_charp >( _scale_width.c_str()  )
                          , const_cast< png_charp >( _scale_height.c_str() )
                          );
        }
#endif // BOOST_GIL_IO_PNG_FIXED_POINT_SUPPORTED
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED


        if( _info._valid_text )
        {
            std::vector< png_text > texts( _info._num_text );
            for( std::size_t i = 0; i < texts.size(); ++i )
            {
                png_text pt;
                pt.compression = _info._text[i]._compression;
                pt.key         = const_cast< png_charp >( _info._text[i]._key.c_str()  );
                pt.text        = const_cast< png_charp >( _info._text[i]._text.c_str() );
                pt.text_length = _info._text[i]._text.length();

                texts[i] = pt;
            }

            png_set_text( _png_ptr
                        , _info_ptr
                        , &texts.front()
                        , _info._num_text
                        );
        }

        if( _info._valid_modification_time )
        {
            png_set_tIME( _png_ptr
                        , _info_ptr
                        , const_cast< png_timep >( &_info._mod_time )
                        );
        }

        if( _info._valid_transparency_factors )
        {
            int sample_max = ( 1 << _info._bit_depth );

            /* libpng doesn't reject a tRNS chunk with out-of-range samples */
            if( !(  (  _info._color_type == PNG_COLOR_TYPE_GRAY 
                    && (int) _info._trans_values[0].gray > sample_max
                    )
                 || (  _info._color_type == PNG_COLOR_TYPE_RGB
                    &&(  (int) _info._trans_values[0].red   > sample_max 
                      || (int) _info._trans_values[0].green > sample_max
                      || (int) _info._trans_values[0].blue  > sample_max
                      )
                    )
                 )
              )
            {
                //@todo Fix that once reading transparency values works
/*
                png_set_tRNS( _png_ptr
                            , _info_ptr
                            , trans
                            , num_trans
                            , trans_values
                            );
*/
            }
        }

        png_write_info( _png_ptr
                      ,_info_ptr
                      );
    }

private:

    void init_io( png_structp png_ptr )
    {
        png_set_write_fn( png_ptr
                        , static_cast< void* >        ( &this->_io_dev                   )
                        , static_cast< png_rw_ptr >   ( &png_io_base<Device>::write_data )
                        , static_cast< png_flush_ptr >( &png_io_base<Device>::flush      )
                        );
    }

public:

    Device& _io_dev;

    image_write_info< bmp_tag > _info;

    png_structp _png_ptr;
    png_infop _info_ptr;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_PNG_IO_WRITER_BACKEND_HPP