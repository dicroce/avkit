
#include "avkit/argb24_to_yuv420p.h"

#include "cppkit/ck_exception.h"
#include "cppkit/ck_socket.h"

using namespace avkit;
using namespace cppkit;
using namespace std;

argb24_to_yuv420p::argb24_to_yuv420p() :
    _yuv420()
{
}

argb24_to_yuv420p::~argb24_to_yuv420p() throw()
{
}

void argb24_to_yuv420p::transform( uint8_t* src, size_t width, size_t height )
{
    _yuv420 = make_shared<ck_memory>();

    uint8_t* yuv = _yuv420->extend_data( width * height * 1.5 ).get_ptr();

    /// Cairo ARGB24 buffers are stored "native endian" as 8 bit unsigned integer quantities in the order
    /// ARGB.

    size_t imageSize = width * height;
    size_t upos = imageSize;
    size_t vpos = upos + upos / 4;
    size_t i = 0;

    for( size_t line = 0; line < height; ++line )
    {
        if( !(line % 2) )
        {
            for( size_t x = 0; x < width; x += 2 )
            {
                // Here I read a 4 byte quantity and on some architectures, swap it. Why swap? Well, cairo buffers
                // are native endian... Which means they will be different on little endian than on big endian. By
                // using htonl(), I can achieve portable code.
                uint32_t word = *((uint32_t*)&src[4 * i]);
                uint32_t pixel = htonl( word );
                uint8_t* rgba = (uint8_t*)&pixel;

                uint8_t r = *(rgba + 1);
                uint8_t g = *(rgba + 2);
                uint8_t b = *(rgba + 3);

                yuv[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;

                yuv[upos++] = ((-38*r + -74*g + 112*b) >> 8) + 128;
                yuv[vpos++] = ((112*r + -94*g + -18*b) >> 8) + 128;

                word = *((uint32_t*)&src[4 * i]);
                pixel = htonl( word );
                rgba = (uint8_t*)&pixel;

                r = *(rgba + 1);
                g = *(rgba + 2);
                b = *(rgba + 3);

                yuv[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
        else
        {
            for( size_t x = 0; x < width; x += 1 )
            {
                uint32_t word = *((uint32_t*)&src[4 * i]);
                uint32_t pixel = htonl( word );
                uint8_t* rgba = (uint8_t*)&pixel;

                uint8_t r = *(rgba + 1);
                uint8_t g = *(rgba + 2);
                uint8_t b = *(rgba + 3);

                yuv[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
    }
}

void argb24_to_yuv420p::transform( shared_ptr<ck_memory> src, size_t width, size_t height )
{
    transform( src->map().get_ptr(), width, height );
}

size_t argb24_to_yuv420p::get_yuv420p_size() const
{
    return _yuv420->size_data();
}

void argb24_to_yuv420p::get_yuv420p( uint8_t* dest ) const
{
    memcpy( dest, _yuv420->map().get_ptr(), _yuv420->size_data() );
}

shared_ptr<cppkit::ck_memory> argb24_to_yuv420p::get_yuv420p() const
{
    return ck_memory::clone( _yuv420 );
}
