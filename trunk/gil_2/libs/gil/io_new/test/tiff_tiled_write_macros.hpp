#ifndef __TIFF_TILED_WRITE_MACROS_HPP__
#define __TIFF_TILED_WRITE_MACROS_HPP__

#include <boost/mpl/vector.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>

#include "paths.hpp"

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/comparison/less.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

typedef boost::gil::tiff_tag tag_t;

#define GENERATE_WRITE_TILE_BIT_ALIGNED_RGB(z, n, data)\
    BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(write_rgb_tile_and_compare_with_,data),_strip_),n), bit_bit_aligned) )\
    { \
      using namespace std; \
      using namespace boost; \
      using namespace gil; \
      string filename_strip( tiff_in_GM + "tiger-" + BOOST_PP_STRINGIZE(data) + "-strip-contig-" ); \
      string padding(""); \
      if(BOOST_PP_LESS(n, 10)==1) \
        padding = "0"; \
      filename_strip = filename_strip + padding + BOOST_PP_STRINGIZE(n) + ".tif"; \
      bit_aligned_image3_type< n, n, n, rgb_layout_t >::type img_strip, img_saved; \
      read_image( filename_strip, img_strip, tag_t() ); \
      image_write_info<tag_t> info; \
      info._is_tiled = true; \
      info._tile_width = info._tile_length = 16; \
      write_view( "./temp.tif", view(img_strip), info ); \
      read_image( "./temp.tif", img_saved, tag_t() ); \
      BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_saved) ), true); \
    } \

// Special case for minisblack images
#define GENERATE_WRITE_TILE_BIT_ALIGNED_MINISBLACK(z, n, data)\
    BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(write_minisblack_tile_and_compare_with_,data),_strip_),n), bit_bit_aligned) )\
    { \
      using namespace std; \
      using namespace boost; \
      using namespace gil; \
      string filename_strip( tiff_in_GM + "tiger-" + BOOST_PP_STRINGIZE(data) + "-strip-" ); \
      string padding(""); \
      if(BOOST_PP_LESS(n, 10)==1) \
        padding = "0"; \
      filename_strip = filename_strip + padding + BOOST_PP_STRINGIZE(n) + ".tif"; \
      bit_aligned_image1_type< n, gray_layout_t >::type img_strip, img_saved; \
      read_image( filename_strip, img_strip, tag_t() ); \
      image_write_info<tag_t> info; \
      info._is_tiled = true; \
      info._tile_width = info._tile_length = 16; \
      write_view( "./temp.tif", view(img_strip), info ); \
      read_image( "./temp.tif", img_saved, tag_t() ); \
      BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_saved) ), true); \
    } \

// Special case for palette images
#define GENERATE_WRITE_TILE_BIT_ALIGNED_PALETTE(z, n, data)\
    BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(write_palette_tile_and_compare_with_,data),_strip_),n), bit) )\
    { \
      using namespace std; \
      using namespace boost; \
      using namespace gil; \
      string filename_strip( tiff_in_GM + "tiger-" + BOOST_PP_STRINGIZE(data) + "-strip-" ); \
      string padding(""); \
      if(BOOST_PP_LESS(n, 10)==1) \
        padding = "0"; \
      filename_strip = filename_strip + padding + BOOST_PP_STRINGIZE(n) + ".tif"; \
      rgb16_image_t img_strip, img_saved; \
      read_image( filename_strip, img_strip, tag_t() ); \
      image_write_info<tag_t> info; \
      info._is_tiled = true; \
      info._tile_width = info._tile_length = 16; \
      write_view( "./temp.tif", view(img_strip), info ); \
      read_image( "./temp.tif", img_saved, tag_t() ); \
      BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_saved) ), true); \
    } \



#endif // __TIFF_TILED_READ_MACROS_HPP__
