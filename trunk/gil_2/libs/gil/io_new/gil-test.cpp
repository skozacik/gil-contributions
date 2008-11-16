#include "stdafx.h"

#include <iostream>
#include <fstream>

#include <boost/gil/extension/io_new/pnm_all.hpp>
#include <boost/gil/gil_all.hpp>

using namespace std;
using namespace boost;
using namespace gil;

int main(int argc, char *argv[])
{
    const std::string filename( ".\\test_images\\pnm\\p1.pnm" );

    typedef gray8_image_t image_t;

    image_t img;
    read_image( filename, img, pnm_tag() );

    return 0;
}
