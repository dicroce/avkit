
#include "avkit/utils.h"
#include <algorithm>

extern "C"
{
#include "libavutil/rational.h"
}

using namespace avkit;
using namespace std;

namespace avkit
{

void aspect_correct_dimensions( uint16_t streamWidth, uint16_t streamHeight,
                                uint16_t requestedWidth, uint16_t requestedHeight,
                                uint16_t& destWidth, uint16_t& destHeight )
{
    destWidth = requestedWidth;
    destHeight = requestedHeight;

    // encode size
    if(streamWidth != 0 && streamHeight !=0)
    {
        uint16_t newEncodeWidth;
        uint16_t newEncodeHeight;

        if(requestedHeight != 0 && requestedWidth != 0)
        {
            float streamAspectRatio = streamWidth * 1.0f / streamHeight;
            float maxAspectRatio = requestedWidth * 1.0f / requestedHeight;
            float scaleFactor;

            if(maxAspectRatio < streamAspectRatio)
                scaleFactor = requestedWidth * 1.0f / streamWidth;
            else
                scaleFactor = requestedHeight * 1.0f / streamHeight;

            uint16_t scaledRoundedPixelWidth = (uint16_t)(streamWidth * scaleFactor + 0.5);
            uint16_t scaledRoundedPixelHeight = (uint16_t)(streamHeight * scaleFactor + 0.5);

            uint16_t multipleOfEightWidth = max( scaledRoundedPixelWidth / 8, 1) * 8;
            uint16_t multipleOfEightHeight = max( scaledRoundedPixelHeight / 8, 1) * 8;

            newEncodeWidth = multipleOfEightWidth;
            newEncodeHeight = multipleOfEightHeight;
        }
        else
        {
            newEncodeWidth = streamWidth;
            newEncodeHeight = streamHeight;
        }

        if(requestedWidth != newEncodeWidth)
            destWidth = newEncodeWidth;

        if(requestedHeight != newEncodeHeight)
            destHeight = newEncodeHeight;
    }
}

double q_to_d( int num, int den )
{
    AVRational q = { num, den };
    return av_q2d( q );
}

void d_to_q( double val, int& num, int& den, int max )
{
    AVRational q = av_d2q( val, max );
    num = q.num;
    den = q.den;
}

AVCodecID _codec_name_to_id( const cppkit::ck_string& name )
{
    if( name.to_lower() == "h264" )
        return AV_CODEC_ID_H264;
    else if( name.to_lower() == "s16le" )
        return AV_CODEC_ID_PCM_S16LE;
    else if( name.to_lower() == "s16be" )
        return AV_CODEC_ID_PCM_S16BE;
    else if( name.to_lower() == "u16le" )
        return AV_CODEC_ID_PCM_U16LE;
    else if( name.to_lower() == "u16be" )
        return AV_CODEC_ID_PCM_U16BE;

    CK_THROW(("Unknown/unsupported codec name."));
}

}
