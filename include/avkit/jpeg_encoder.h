
#ifndef __avkit_jpeg_encoder_h
#define __avkit_jpeg_encoder_h

#include "avkit/options.h"

#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace avkit
{

const int JPEG_ENCODE_ATTEMPTS = 16;

class jpeg_encoder
{
public:
    jpeg_encoder( const struct codec_options& options, int encodeAttempts = JPEG_ENCODE_ATTEMPTS );

    virtual ~jpeg_encoder() throw();

    size_t encode_yuv420p( uint8_t* pic, uint8_t* output, size_t outputSize );

    std::shared_ptr<cppkit::ck_memory> encode_yuv420p( std::shared_ptr<cppkit::ck_memory> pic );

    static void write_jpeg_file( const cppkit::ck_string& fileName, std::shared_ptr<cppkit::ck_memory> jpeg );

private:
    jpeg_encoder( const jpeg_encoder& obj );
    jpeg_encoder& operator = ( const jpeg_encoder& );

    AVCodec* _codec;
    AVCodecContext* _context;
    struct codec_options _options;
    int _encodeAttempts;
};

}

#endif
