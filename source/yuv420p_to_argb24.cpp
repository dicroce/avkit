
#include "avkit/yuv420p_to_argb24.h"
#include "avkit/locky.h"

#include "cppkit/ck_exception.h"
#include "cppkit/ck_socket.h"
#include "cppkit/ck_path.h"

using namespace avkit;
using namespace cppkit;
using namespace std;

yuv420p_to_argb24::yuv420p_to_argb24() :
    _rgb24(),
    _pf( std::make_shared<av_packet_factory_default>() ),
    _scaler( NULL ),
    _currentWidth( 0 ),
    _currentHeight( 0 )
{
    if( !locky::is_registered() )
        CK_THROW(( "Please call locky::register_ffmpeg() before using this class."));
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
