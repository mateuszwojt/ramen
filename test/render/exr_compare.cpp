// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

// Compares two EXR images: exr_compare reference.exr image.exr [tolerance]
// Fails if the windows or channels differ, or if any pixel value differs
// from the reference by more than tolerance.

#include<cmath>
#include<cstdlib>
#include<exception>
#include<iostream>
#include<string>
#include<vector>

#include<OpenEXR/ImfChannelList.h>
#include<OpenEXR/ImfFrameBuffer.h>
#include<OpenEXR/ImfHeader.h>
#include<OpenEXR/ImfInputFile.h>

namespace
{

struct image_t
{
    Imath::Box2i display_window;
    Imath::Box2i data_window;
    std::vector<std::string> channels;
    std::vector<std::vector<float> > pixels;
};

image_t read_image( const std::string& path)
{
    Imf::InputFile file( path.c_str());

    image_t img;
    img.display_window = file.header().displayWindow();
    img.data_window = file.header().dataWindow();

    const Imath::Box2i& dw( img.data_window);
    const int width = dw.max.x - dw.min.x + 1;
    const int height = dw.max.y - dw.min.y + 1;

    for( Imf::ChannelList::ConstIterator it( file.header().channels().begin()); it != file.header().channels().end(); ++it)
        img.channels.push_back( it.name());

    img.pixels.resize( img.channels.size(), std::vector<float>( std::size_t( width) * height));

    Imf::FrameBuffer frame_buffer;

    for( std::size_t i = 0; i < img.channels.size(); ++i)
    {
        char *base = reinterpret_cast<char*>( img.pixels[i].data() - dw.min.x - std::ptrdiff_t( dw.min.y) * width);
        frame_buffer.insert( img.channels[i], Imf::Slice( Imf::FLOAT, base, sizeof( float), sizeof( float) * width));
    }

    file.setFrameBuffer( frame_buffer);
    file.readPixels( dw.min.y, dw.max.y);
    return img;
}

} // unnamed

int main( int argc, char **argv)
{
    if( argc < 3)
    {
        std::cerr << "usage: exr_compare reference.exr image.exr [tolerance]\n";
        return EXIT_FAILURE;
    }

    const float tolerance = argc > 3 ? std::stof( argv[3]) : 0.0f;

    try
    {
        image_t ref( read_image( argv[1]));
        image_t img( read_image( argv[2]));

        if( ref.display_window != img.display_window || ref.data_window != img.data_window)
        {
            std::cerr << "exr_compare: image windows differ\n";
            return EXIT_FAILURE;
        }

        if( ref.channels != img.channels)
        {
            std::cerr << "exr_compare: image channels differ\n";
            return EXIT_FAILURE;
        }

        float max_error = 0.0f;
        std::size_t num_failed = 0;

        for( std::size_t c = 0; c < ref.channels.size(); ++c)
        {
            for( std::size_t i = 0; i < ref.pixels[c].size(); ++i)
            {
                float a = ref.pixels[c][i];
                float b = img.pixels[c][i];

                // NaNs only match NaNs.
                float error = ( std::isnan( a) || std::isnan( b)) ? ( std::isnan( a) == std::isnan( b) ? 0.0f : INFINITY)
                                                                  : std::fabs( a - b);

                if( error > max_error)
                    max_error = error;

                if( error > tolerance)
                    ++num_failed;
            }
        }

        std::cout << argv[2] << ": max error = " << max_error << ", values over tolerance = " << num_failed << "\n";
        return num_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch( std::exception& e)
    {
        std::cerr << "exr_compare: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
