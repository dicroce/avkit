
#ifndef __avkit_h264_transcoder_h
#define __avkit_h264_transcoder_h

#include "avkit/av_demuxer.h"
#include "avkit/h264_decoder.h"
#include "avkit/h264_encoder.h"
#include "avkit/av_muxer.h"
#include "avkit/options.h"

namespace avkit
{

class h264_transcoder
{
public:
    h264_transcoder( int inputTimeBaseNum, int inputTimeBaseDen, int outputTimeBaseNum, int outputTimeBaseDen, double speed = 1.0, bool decodeSkipping = false );

    // The job of this method is to decode frames until it is time to make an output frame. If the output
    // framerate is greater than the input framerate this means that sometimes calls to this method will not
    // actually decode a frame (because we need to encode multiple outputs from the same input in that case).
    // Likewise, when the output framerate is less than the input framerate, we will sometimes decode multiple
    // times before we return. In either case, after this method returns, we are ready to make an output frame.
    template<class T>
    bool decode( T& avDeMuxer, h264_decoder& decoder )
    {
        int videoStreamIndex = avDeMuxer.get_video_stream_index();

        if( _step < 1.0 )
        {
            while( !avDeMuxer.end_of_file() && (_step < 1.0) )
            {
                int streamIndex = 0;
                if( !avDeMuxer.read_frame( streamIndex ) )
                    return false;

                if( streamIndex == videoStreamIndex )
                {
                    _step += _outputFramesPerInputFrame;

                    if( _step > 1.0 || !_decodeSkipping )
                        decoder.decode( avDeMuxer.get_frame() );
                }
            }
        }

        _step -= 1.0;

        return true;
    }

    void encode_yuv420p_and_mux( h264_encoder& encoder,
                                 av_muxer& muxer,
                                 std::shared_ptr<cppkit::ck_memory> pic,
                                 h264_encoder::h264_encoder_frame_type type = h264_encoder::FRAME_TYPE_AUTO_GOP );

    static int64_t compute_num_output_frames( int64_t numInputFrames,
                                              int inputTimeBaseNum, int inputTimeBaseDen,
                                              int outputTimeBaseNum, int outputTimeBaseDen,
                                              double speed = 1.0 );

private:
    double _outputFramesPerInputFrame;
    double _step;
    double _speed;
    bool _decodeSkipping;
};

};

#endif
