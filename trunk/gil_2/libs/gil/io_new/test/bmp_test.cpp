#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT

#include <fstream>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/bmp_all.hpp>

#include <boost/filesystem/convenience.hpp>

#include <boost/test/unit_test.hpp>

#include "cmp_view.hpp"
#include "mandel_view.hpp"
#include "paths.hpp"
#include "subimage_test.hpp"

using namespace std;
using namespace boost;
using namespace gil;
namespace fs = boost::filesystem;

typedef bmp_tag tag_t;

BOOST_AUTO_TEST_SUITE( bmp_test )

BOOST_AUTO_TEST_CASE( read_image_info_using_string )
{

    {
        image_read_info< tag_t > info = read_image_info( bmp_filename
                                                       , tag_t() );
        BOOST_CHECK_EQUAL( info._width , 127 );
        BOOST_CHECK_EQUAL( info._height, 64 );
    }

    {
        ifstream in( bmp_filename.c_str(), ios::binary );

        image_read_info< tag_t > info = read_image_info( in
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 127 );
        BOOST_CHECK_EQUAL( info._height, 64 );
    }

    {
        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        
        image_read_info< tag_t > info = read_image_info( file
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 127 );
        BOOST_CHECK_EQUAL( info._height, 64 );
    }

    {
        fs::path my_path( bmp_filename );
        image_read_info< tag_t > info = read_image_info( my_path
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 127 );
        BOOST_CHECK_EQUAL( info._height, 64  );
    }
}

BOOST_AUTO_TEST_CASE( read_image_test )
{
    {
        rgb8_image_t img;
        read_image( bmp_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 127 );
        BOOST_CHECK_EQUAL( img.height(), 64 );
    }

    {
        ifstream in( bmp_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 127 );
        BOOST_CHECK_EQUAL( img.height(), 64 );
    }

    {
        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        
        rgb8_image_t img;
        read_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 127 );
        BOOST_CHECK_EQUAL( img.height(), 64 );
    }
}

BOOST_AUTO_TEST_CASE( read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        read_and_convert_image( bmp_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 127 );
        BOOST_CHECK_EQUAL( img.height(), 64 );
    }

    {
        ifstream in( bmp_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_and_convert_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 127 );
        BOOST_CHECK_EQUAL( img.height(), 64 );
    }

    {
        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        
        rgb8_image_t img;
        read_and_convert_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 127 );
        BOOST_CHECK_EQUAL( img.height(), 64 );
    }
}

BOOST_AUTO_TEST_CASE( read_view_test )
{
    {
        rgb8_image_t img( 127, 64 );
        read_view( bmp_filename, view( img ), tag_t() );
    }

    {
        ifstream in( bmp_filename.c_str(), ios::binary );

        rgb8_image_t img( 127, 64 );
        read_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        
        rgb8_image_t img( 127, 64 );
        read_view( file, view( img ), tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( read_and_convert_view_test )
{
    {
        rgb8_image_t img( 127, 64 );
        read_and_convert_view( bmp_filename, view( img ), tag_t() );
    }

    {
        ifstream in( bmp_filename.c_str(), ios::binary );

        rgb8_image_t img( 127, 64 );
        read_and_convert_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        
        rgb8_image_t img( 127, 64 );
        read_and_convert_view( file
                             , view( img )
                             , tag_t()
                             );
    }
}

BOOST_AUTO_TEST_CASE( write_view_test )
{
    {
        string filename( bmp_out + "write_test_ofstream.bmp" );

        ofstream out( filename.c_str(), ios::binary );

        write_view( out
                  , create_mandel_view( 127, 64
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , tag_t()
                  );
    }

    {
        string filename( bmp_out + "write_test_file.bmp" );

        FILE* file = fopen( filename.c_str(), "wb" );
        
        write_view( file
                  , create_mandel_view( 127, 64
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , tag_t()
                  );
    }

    {
        string filename( bmp_out + "write_test_info.bmp" );

        image_write_info< tag_t > info;

        FILE* file = fopen( filename.c_str(), "wb" );

        write_view( file
                  , create_mandel_view( 127, 64
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , info
                  );
    }
}

BOOST_AUTO_TEST_CASE( stream_test )
{
    // 1. Read an image.
    ifstream in( bmp_filename.c_str(), ios::binary );

    rgb8_image_t img;
    read_image( in, img, tag_t() );

    // 2. Write image to in-memory buffer.
    stringstream out_buffer( ios_base::in | ios_base::out | ios_base::binary );
    write_view( out_buffer, view( img ), tag_t() );

    // 3. Copy in-memory buffer to another.
    stringstream in_buffer( ios_base::in | ios_base::out | ios_base::binary );
    in_buffer << out_buffer.rdbuf();

    // 4. Read in-memory buffer to gil image
    rgb8_image_t dst;
    read_image( in_buffer, dst, tag_t() );

    // 5. Write out image.
    string filename( bmp_out + "stream_test.bmp" );
    ofstream out( filename.c_str(), ios_base::binary );
    write_view( out, view( dst ), tag_t() );
}

BOOST_AUTO_TEST_CASE( subimage_test )
{
    run_subimage_test< rgb8_image_t, tag_t >( bmp_filename
                                            , point_t(  0,  0 )
                                            , point_t( 50, 50 )
                                            );

    run_subimage_test< rgb8_image_t, tag_t >( bmp_filename
                                            , point_t( 39,  7 )
                                            , point_t( 50, 50 )
                                            );
}

BOOST_AUTO_TEST_CASE( dynamic_image_test )
{
    typedef mpl::vector< gray8_image_t
                       , gray16_image_t
                       , rgb8_image_t
                       , rgba8_image_t
                       > my_img_types;


    any_image< my_img_types > runtime_image;

    read_image( bmp_filename.c_str()
              , runtime_image
              , tag_t()
              );

    write_view( bmp_out + "dynamic_image_test.bmp"
              , view( runtime_image )
              , tag_t()
              );
}

BOOST_AUTO_TEST_SUITE_END()