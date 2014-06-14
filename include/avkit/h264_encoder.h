
#ifndef __avkit_h264_encoder_h
#define __avkit_h264_encoder_h

#include "avkit/options.h"

#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace avkit
{

const int H264_ENCODE_ATTEMPTS = 16;

class h264_encoder
{
public:

    enum h264_encoder_frame_type
    {
        FRAME_TYPE_KEY,
        FRAME_TYPE_PARTIAL,
        FRAME_TYPE_AUTO_GOP
    };

    h264_encoder( const struct codec_options& options,
                  bool annexB = true,
                  int encodeAttempts = H264_ENCODE_ATTEMPTS );

    virtual ~h264_encoder() throw();

    /// Encode the YUV420P image pointed to by pic into an H.264 frame, writing the output frame
    /// to the memory pointed to by output.
    size_t encode_yuv420p( uint8_t* pic, uint8_t* output, size_t outputSize,
                           h264_encoder_frame_type type = FRAME_TYPE_AUTO_GOP );

    /// A convenience method that wraps the functionaliy provided above but takes and returns
    /// XMemory objects. (note: because this method allocates memory, it is not quite as efficient
    /// as the above method).
    std::shared_ptr<cppkit::ck_memory> encode_yuv420p( std::shared_ptr<cppkit::ck_memory> pic,
                                                       h264_encoder_frame_type type = FRAME_TYPE_AUTO_GOP );

    bool last_was_key() const { return _lastWasKey; }

    struct codec_options get_options() const;

    std::shared_ptr<cppkit::ck_memory> get_extra_data() const;

private:
    h264_encoder( const h264_encoder& obj );
    h264_encoder& operator = ( const h264_encoder& );

    AVCodec* _codec;
    AVCodecContext* _context;
    struct codec_options _options;
    uint8_t _gopSize;
    uint8_t _numTillKey;
    int _encodeAttempts;
    cppkit::ck_memory _extraData;
    bool _lastWasKey;
    bool _annexB;
};

}

#endif
