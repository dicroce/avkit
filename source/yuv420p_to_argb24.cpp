
#include "avkit/yuv420p_to_argb24.h"

#include "cppkit/ck_exception.h"
#include "cppkit/ck_socket.h"
#include "cppkit/ck_path.h"

using namespace avkit;
using namespace cppkit;
using namespace std;

#define GETR(y,u,v) ((1.164 * (y - 16)) + (1.596 * ((v) - 128)))
#define GETG(y,u,v) ((1.164 * (y - 16)) - (0.813 * ((v) - 128)) - (0.391 * ((u) - 128)))
#define GETB(y,u,v) ((1.164 * (y - 16)) + (2.018 * ((u) - 128)))

#define CLIP(val,min,max) (val<min)?min:(val>max)?max:val

yuv420p_to_argb24::yuv420p_to_argb24() :
    _rgb24(),
    _pf( std::make_shared<av_packet_factory_default>() ),
    _scaler( NULL ),
    _currentWidth( 0 ),
    _currentHeight( 0 )
{
}

yuv420p_to_argb24::~yuv420p_to_argb24() throw()
{
    _destroy_scaler();
}

void yuv420p_to_argb24::transform( shared_ptr<av_packet> input, size_t width, size_t height )
{
    uint8_t* src = input->map();

    AVFrame frame;
    frame.data[0] = src;
    src += width * height;
    frame.data[1] = src;
    src += ((width/2) * (height/2));
    frame.data[2] = src;

    frame.linesize[0] = width;
    frame.linesize[1] = (width/2);
    frame.linesize[2] = (width/2);

    size_t dataSize = height * (width*4);
    _rgb24 = _pf->get( dataSize );
    _rgb24->set_data_size( dataSize );

    _rgb24->set_width( width );
    _rgb24->set_height( height );

    AVPicture pict;
    pict.data[0] = _rgb24->map();
    pict.linesize[0] = width * 4;

    if( (width != _currentWidth) || (height != _currentHeight) )
        _destroy_scaler();

    if( !_scaler )
        _init_scaler( width, height );

    int ret = sws_scale( _scaler,
                         frame.data,
                         frame.linesize,
                         0,
                         height,
                         pict.data,
                         pict.linesize );

#if 0
    const size_t halfWidth = width / 2;
    const size_t halfHeight = height / 2;

    const uint8_t* srcY = input->map();
    const uint8_t* srcU = srcY + (width*height);
    const uint8_t* srcV = srcU + (halfWidth*halfHeight);

    for( size_t dstY = 0, uvY = 0; dstY < height; dstY+=2, uvY++ )
    {
        for( size_t dstX = 0, uvX = 0; dstX < width; dstX+=2, uvX++ )
        {
            uint8_t y = *srcY++;
            uint8_t u = *(srcU + (( uvY * halfWidth ) + uvX ));
            uint8_t v = *(srcV + (( uvY * halfWidth ) + uvX ));

            *dst++ = (uint8_t)(CLIP(GETB( y, u, v ),0,255));
            *dst++ = (uint8_t)(CLIP(GETG( y, u, v ),0,255));
            *dst++ = (uint8_t)(CLIP(GETR( y, u, v ),0,255));
            *dst++ = 0xFF;

            y = *srcY++;
            u = *(srcU + (( uvY * halfWidth ) + uvX ));
            v = *(srcV + (( uvY * halfWidth ) + uvX ));

            *dst++ = (uint8_t)(CLIP(GETB( y, u, v ),0,255));
            *dst++ = (uint8_t)(CLIP(GETG( y, u, v ),0,255));
            *dst++ = (uint8_t)(CLIP(GETR( y, u, v ),0,255));
            *dst++ = 0xFF;
        }

        for( size_t dstX = 0, uvX = 0; dstX < width; dstX+=2, uvX++ )
        {
            uint8_t y = *srcY++;
            uint8_t u = *(srcU + (( uvY * halfWidth ) + uvX ));
            uint8_t v = *(srcV + (( uvY * halfWidth ) + uvX ));

            *dst++ = (uint8_t)(CLIP(GETB( y, u, v ),0,255));
            *dst++ = (uint8_t)(CLIP(GETG( y, u, v ),0,255));
            *dst++ = (uint8_t)(CLIP(GETR( y, u, v ),0,255));
            *dst++ = 0xFF;

            y = *srcY++;
            u = *(srcU + (( uvY * halfWidth ) + uvX ));
            v = *(srcV + (( uvY * halfWidth ) + uvX ));

            *dst++ = (uint8_t)(CLIP(GETB( y, u, v ),0,255));
            *dst++ = (uint8_t)(CLIP(GETG( y, u, v ),0,255));
            *dst++ = (uint8_t)(CLIP(GETR( y, u, v ),0,255));
            *dst++ = 0xFF;
        }
    }
#endif

}

shared_ptr<av_packet> yuv420p_to_argb24::get()
{
    return std::move( _rgb24 );
}

void yuv420p_to_argb24::_init_scaler( uint16_t width, uint16_t height )
{
    int scaleFlags = SWS_FAST_BILINEAR;

    _scaler = sws_getContext( width,
                              height,
                              PIX_FMT_YUV420P,
                              width,
                              height,
                              PIX_FMT_BGRA,
                              scaleFlags,
                              NULL,
                              NULL,
                              NULL );

    if( !_scaler )
        CK_THROW(("Unable to allocate scaler context!"));
}

void yuv420p_to_argb24::_destroy_scaler()
{
    if( _scaler )
    {
        sws_freeContext( _scaler );
        _scaler = NULL;
    }
}
