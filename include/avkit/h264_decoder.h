
#ifndef __avkit_h264_decoder_h
#define __avkit_h264_decoder_h

#include "avkit/options.h"
#include "avkit/av_demuxer.h"

#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

namespace avkit
{

const int H264_DECODE_ATTEMPTS = 16;

class h264_decoder
{
public:
    h264_decoder( const struct codec_options& options, int decodeAttempts = H264_DECODE_ATTEMPTS );
    h264_decoder( av_demuxer& deMuxer, const struct codec_options& options, int decodeAttempts = H264_DECODE_ATTEMPTS );
    virtual ~h264_decoder() throw();

    void decode( uint8_t* frame, size_t frameSize );
    void decode( std::shared_ptr<cppkit::ck_memory> frame );

    uint16_t get_input_width() const;
    uint16_t get_input_height() const;

    void set_output_width( uint16_t outputWidth );
    uint16_t get_output_width() const;

    void set_output_height( uint16_t outputHeight );
    uint16_t get_output_height() const;

    size_t get_yuv420p_size() const;
    void make_yuv420p( uint8_t* dest );
    std::shared_ptr<cppkit::ck_memory> make_yuv420p();

private:
    h264_decoder( const h264_decoder& obj );
    h264_decoder& operator = ( const h264_decoder& );

    void _destroy_scaler();

    AVCodec* _codec;
    AVCodecContext* _context;
    struct codec_options _options;
    AVFrame* _frame;
    SwsContext* _scaler;
    uint16_t _outputWidth;
    uint16_t _outputHeight;

    int _decodeAttempts;
};

}

#endif
