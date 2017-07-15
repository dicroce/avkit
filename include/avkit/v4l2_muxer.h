
#ifndef __avkit_v4l2_muxer_h
#define __avkit_v4l2_muxer_h

#include "avkit/options.h"
#include "avkit/av_packet.h"
#include "avkit/av_packet_factory.h"
#include "cppkit/ck_memory.h"
#include <memory>

extern "C"
{
#include "libavformat/avformat.h"
}

namespace avkit
{

class v4l2_muxer
{
public:
    CK_API v4l2_muxer( const struct codec_options& options,
                       const cppkit::ck_string& fileName );

    CK_API virtual ~v4l2_muxer() throw();

    CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API void write_video_packet( std::shared_ptr<av_packet> input, bool keyFrame );

    CK_API void flush();

private:
    v4l2_muxer( const v4l2_muxer& obj );
    v4l2_muxer& operator = ( const v4l2_muxer& obj );

    void _apply_codec_options( const struct codec_options& options );

    void _open_io();
    void _close_io();

    struct codec_options _options;
    cppkit::ck_string _fileName;
    AVFormatContext* _context;
    AVStream* _stream;
    bool _oweTrailer;
    std::shared_ptr<av_packet_factory> _pf;
    int64_t _ts;
};

}

#endif
