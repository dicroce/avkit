
#include "avkit/jpeg_encoder.h"

#include "cppkit/ck_exception.h"

using namespace avkit;
using namespace cppkit;
using namespace std;

jpeg_encoder::jpeg_encoder( const struct codec_options& options, int encodeAttempts ) :
    _codec( avcodec_find_encoder( CODEC_ID_MJPEG ) ),
    _context( avcodec_alloc_context3( _codec ) ),
    _options( options ),
    _encodeAttempts( encodeAttempts )
{
    if( !_codec )
        CK_THROW(("Unable to locate MJPEG codec."));

    if( !_context )
        CK_THROW(("Unable to allocate MJPEG codec context."));

    _context->codec_id = CODEC_ID_MJPEG;
    _context->codec_type = AVMEDIA_TYPE_VIDEO;

    _context->time_base.num = 1;
    _context->time_base.den = 15;

    _context->pix_fmt = PIX_FMT_YUVJ420P;
    _context->color_range = AVCOL_RANGE_JPEG;

    _context->gop_size = 1;
    _context->bit_rate_tolerance = (int)((double)10000000 *
                                         ((double)_context->time_base.num / (double)_context->time_base.den));
    _context->bit_rate_tolerance += 1; // Make sure we pass the comparison check

    if( !_options.width.is_null() )
        _context->width = _options.width.value();
    else CK_THROW(("Required option missing: width"));

    if( !_options.height.is_null() )
        _context->height = _options.height.value();
    else CK_THROW(("Required option missing: height"));

    if( !_options.bit_rate.is_null() )
        _context->bit_rate = _options.bit_rate.value();
    else CK_THROW(("Required option missing: bit_rate"));

    if( !_options.qmin.is_null() )
        _context->qmin = _options.qmin.value();

    if( !_options.qmax.is_null() )
        _context->qmax = _options.qmax.value();

    if( !_options.max_qdiff.is_null() )
        _context->max_qdiff = _options.max_qdiff.value();

    if( avcodec_open2( _context, _codec, NULL ) < 0 )
        CK_THROW(("Failed to open encoding context."));
}

jpeg_encoder::~jpeg_encoder() throw()
{
    avcodec_close( _context );

    av_free( _context );
}

size_t jpeg_encoder::encode_yuv420p( uint8_t* pic, uint8_t* output, size_t outputSize )
{
    AVFrame frame;
    avcodec_get_frame_defaults( &frame );

    frame.data[0] = pic;
    pic += (_context->width * _context->height);
    frame.data[1] = pic;
    pic += ((_context->width/4) * _context->height);
    frame.data[2] = pic;

    frame.linesize[0] = _context->width;
    frame.linesize[1] = (_context->width/2);
    frame.linesize[2] = (_context->width/2);

    int attempt = 0;
    int gotPacket = 0;
    AVPacket pkt;

    do
    {
        av_init_packet( &pkt );
        pkt.data = output;
        pkt.size = outputSize;

        if( avcodec_encode_video2( _context,
                                   &pkt,
                                   &frame,
                                   &gotPacket ) < 0 )
            CK_THROW(("Error while encoding."));

        attempt++;

    } while( gotPacket == 0 && (attempt < _encodeAttempts) );

    return pkt.size;
}

shared_ptr<ck_memory> jpeg_encoder::encode_yuv420p( shared_ptr<ck_memory> pic )
{
    shared_ptr<ck_memory> frame = make_shared<ck_memory>();
    uint8_t* p = frame->extend_data( (1024*1024) ).get_ptr();
    size_t outputSize = encode_yuv420p( pic->map().get_ptr(), p, frame->size_data() );
    frame->set_data_size( outputSize );
    return frame;
}

void jpeg_encoder::write_jpeg_file( const cppkit::ck_string& fileName, shared_ptr<cppkit::ck_memory> jpeg )
{
    FILE* outFile = fopen( fileName.c_str(), "wb" );
    if( !outFile )
        CK_THROW(("Unable to open output file."));

    fwrite( jpeg->map().get_ptr(), 1, jpeg->size_data(), outFile );

    fclose( outFile );
}
