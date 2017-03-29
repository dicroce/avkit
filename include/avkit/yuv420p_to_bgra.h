
#ifndef __avkit_yuv420p_to_bgra_h
#define __avkit_yuv420p_to_bgra_h

#include "avkit/av_packet.h"
#include "avkit/av_packet_factory.h"
#include "cppkit/ck_types.h"

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

namespace avkit
{

class yuv420p_to_bgra
{
public:
    CK_API yuv420p_to_bgra();

    CK_API virtual ~yuv420p_to_bgra() throw();

    CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API void transform( std::shared_ptr<av_packet> input, size_t width, size_t height );

    CK_API std::shared_ptr<av_packet> get();

private:
    yuv420p_to_bgra( const yuv420p_to_bgra& obj );
    yuv420p_to_bgra& operator = ( const yuv420p_to_bgra& );

    void _init_scaler( uint16_t width, uint16_t height );
    void _destroy_scaler();

    std::shared_ptr<av_packet> _rgb24;
    std::shared_ptr<av_packet_factory> _pf;

    SwsContext* _scaler;
    uint16_t _currentWidth;
    uint16_t _currentHeight;
};

}

#endif
