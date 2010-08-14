/********************************************************
 *
 * This test file will test palette tiled tiff writing
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>
#include "tiff_tiled_write_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_PP_REPEAT_FROM_TO(9, 17, GENERATE_WRITE_TILE_BIT_ALIGNED_PALETTE, palette )

BOOST_AUTO_TEST_SUITE_END()
