
#include "avkit/v4l2_muxer.h"
#include "avkit/locky.h"

extern "C"
{
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
}

using namespace avkit;
using namespace cppkit;
using namespace std;

v4l2_muxer::v4l2_muxer( const struct codec_options& options,
                        const ck_string& fileName ) :
    _options( options ),
    _fileName( fileName ),
    _context( NULL ),
    _stream( NULL ),
    _oweTrailer( false ),
    _pf( std::make_shared<av_packet_factory_default>() ),
    _ts(0)
{
    if( !locky::is_registered() )
        CK_THROW(( "Please call locky::register_ffmpeg() before using this class."));

    avformat_alloc_output_context2( &_context,
                                    NULL,
                                    "v4l2",
                                    _fileName.c_str());

    if(!_context)
        CK_THROW(("Unable to allocate output context."));

    _context->oformat->video_codec = AV_CODEC_ID_RAWVIDEO;

    _stream = avformat_new_stream( _context, NULL );
    if( !_stream )
        CK_THROW(("Unable to allocate output stream."));

    avcodec_get_context_defaults3( _stream->codec, NULL );

    _stream->codec->codec_id = AV_CODEC_ID_RAWVIDEO;
    _stream->codec->codec_type = AVMEDIA_TYPE_VIDEO;

    _apply_codec_options( options );

    _stream->codec->pix_fmt = AV_PIX_FMT_YUV420P;
}

v4l2_muxer::~v4l2_muxer() throw()
{
    if( _oweTrailer )
        av_write_trailer( _context );

    _close_io();

    avformat_free_context( _context );
}

void v4l2_muxer::write_video_packet( shared_ptr<av_packet> input, bool keyFrame )
{
    if( _context->pb == NULL )
        _open_io();

    if( !_oweTrailer )
    {
        if( avformat_write_header( _context, NULL ) < 0 )
            CK_THROW(("Unable to write header to container."));

        _oweTrailer = true;
    }

    AVPacket pkt;
    av_init_packet( &pkt );

    pkt.stream_index = _stream->index;
    pkt.data = input->map();
    pkt.size = (int)input->get_data_size();

    pkt.pts = _ts;
    pkt.dts = _ts;

    _ts += av_rescale_q(1, _stream->codec->time_base, _stream->time_base);

    pkt.flags |= AV_PKT_FLAG_KEY;

    if( av_interleaved_write_frame( _context, &pkt ) < 0 )
        CK_THROW(("Unable to write video frame."));
}

void v4l2_muxer::flush()
{
    if( av_interleaved_write_frame( _context, NULL ) < 0 )
        CK_THROW(("Unable to flush v4l2_muxer."));
}

void v4l2_muxer::_apply_codec_options( const struct codec_options& options )
{
    _options = options;

    if( !_options.width.is_null() )
        _stream->codec->width = _options.width.value();
    else CK_THROW(("Required option missing: width"));

    if( !_options.height.is_null() )
        _stream->codec->height = _options.height.value();
    else CK_THROW(("Required option missing: height"));
}

void v4l2_muxer::_open_io()
{
    if( _context->pb == NULL )
    {
        if( avio_open( &_context->pb, _fileName.c_str(), AVIO_FLAG_WRITE ) < 0 )
            CK_THROW(("Unable to open output file."));
    }
}

void v4l2_muxer::_close_io()
{
    if( _context->pb )
    {
        avio_close( _context->pb );
        _context->pb = NULL;
    }
}
