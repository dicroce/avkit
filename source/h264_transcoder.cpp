
#include "avkit/h264_transcoder.h"

using namespace avkit;
using namespace cppkit;
using namespace std;

h264_transcoder::h264_transcoder( int inputTimeBaseNum, int inputTimeBaseDen,
                                  int outputTimeBaseNum, int outputTimeBaseDen,
                                  double speed,
                                  bool decodeSkipping ) :
    _step( 0.0 ),
    _speed( speed ),
    _decodeSkipping( decodeSkipping )
{
    // When the input and output framerates were specified with single integers, this was how we did it...
    // _outputFramesPerInputFrame( (((double)outputFrameRate) / (((double)inputFrameRate) * speed)) ),

    AVRational inputRational = { inputTimeBaseNum, inputTimeBaseDen };
    AVRational outputRational = { outputTimeBaseNum, outputTimeBaseDen };

    _outputFramesPerInputFrame = av_q2d( inputRational ) / (av_q2d( outputRational ) * _speed);
}

void h264_transcoder::encode_yuv420p_and_mux( h264_encoder& encoder,
                                              av_muxer& muxer,
                                              shared_ptr<av_packet> pic,
                                              h264_encoder::h264_encoder_frame_type type )
{
    encoder.encode_yuv420p( pic, type );

    shared_ptr<av_packet> encodeBuffer = encoder.get();

    muxer.write_video_packet( encodeBuffer, encoder.last_was_key() );
}

int64_t h264_transcoder::compute_num_output_frames( int64_t numInputFrames,
                                                    int inputTimeBaseNum, int inputTimeBaseDen,
                                                    int outputTimeBaseNum, int outputTimeBaseDen,
                                                    double speed )
{
    AVRational inputRational = { inputTimeBaseNum, inputTimeBaseDen };
    AVRational outputRational = { outputTimeBaseNum, outputTimeBaseDen };

    double outputFramesPerInputFrame = av_q2d( inputRational ) / (av_q2d( outputRational ) * speed);

    return (int64_t)(outputFramesPerInputFrame * (double)numInputFrames);
}
