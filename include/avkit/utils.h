
#ifndef __avkit_utils_h
#define __avkit_utils_h

#include "cppkit/ck_types.h"
#include "cppkit/ck_string.h"
#include <climits>

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace avkit
{

// Adjusts requestedWidth X requestedHeight to maintain aspect ratio of streamWidth X streamHeight.

CK_API void aspect_correct_dimensions( uint16_t streamWidth, uint16_t streamHeight,
				       uint16_t requestedWidth, uint16_t requestedHeight,
				       uint16_t& destWidth, uint16_t& destHeight );

CK_API double q_to_d( int num, int den );

CK_API void d_to_q( double val, int& num, int& den, int max = INT_MAX );

CK_API AVCodecID _codec_name_to_id( const cppkit::ck_string& name );

}

#endif
