
#ifndef __avkit_bgra_to_yuv420p_h
#define __avkit_bgra_to_yuv420p_h

#include "avkit/av_packet.h"
#include "avkit/av_packet_factory.h"
#include <memory>
#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"
#include "cppkit/os/ck_exports.h"

// 1) We might want to preallocate buffers

namespace avkit
{

class bgra_to_yuv420p
{
public:
    CK_API bgra_to_yuv420p();
    CK_API virtual ~bgra_to_yuv420p() throw();

    CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API void transform( std::shared_ptr<av_packet> pkt, size_t width, size_t height );

    CK_API std::shared_ptr<av_packet> get() const;

private:
    bgra_to_yuv420p( const bgra_to_yuv420p& obj );
    bgra_to_yuv420p& operator = ( const bgra_to_yuv420p& );

    std::shared_ptr<av_packet> _yuv420;
    std::shared_ptr<av_packet_factory> _pf;
};

}

#endif
