#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/png_io_old.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

BOOST_AUTO_TEST_CASE( png_old_read_dimensions_test )
{
    {
        point2< ptrdiff_t > dim = png_read_dimensions( png_filename );

        BOOST_CHECK_EQUAL( dim.x, 320 );
        BOOST_CHECK_EQUAL( dim.y, 240 );
    }
}

BOOST_AUTO_TEST_CASE( png_old_read_image_test )
{
    {
        rgba8_image_t img;
        png_read_image( png_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }
}

BOOST_AUTO_TEST_CASE( png_old_read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        png_read_and_convert_image( png_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }
}

BOOST_AUTO_TEST_CASE( png_old_read_view_test )
{
    {
        rgba8_image_t img( 320, 240 );
        png_read_view( png_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( png_old_read_and_convert_view_test )
{
    {
        rgb8_image_t img( 320, 240 );
        png_read_and_convert_view( png_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( png_old_write_view_test )
{
    {
        string filename( png_out + "test5.png" );

        gray8_image_t img( 320, 240 );
        png_write_view( filename, view( img ) );
    }
}
