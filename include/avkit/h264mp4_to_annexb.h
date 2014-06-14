
#ifndef __avkit_h264mp4_to_annexb_h
#define __avkit_h264mp4_to_annexb_h

#include "avkit/options.h"
#include "avkit/av_demuxer.h"

#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace avkit
{

class h264mp4_to_annexb
{
public:
    h264mp4_to_annexb( av_demuxer& deMuxer );

    virtual ~h264mp4_to_annexb() throw();

    void transform( uint8_t* src, size_t srcSize, bool keyFrame );
    void transform( std::shared_ptr<cppkit::ck_memory> src, bool keyFrame );

    size_t get_annexb_size() const;
    void get_annexb( uint8_t* dest ) const;
    std::shared_ptr<cppkit::ck_memory> get_annexb() const;

private:
    h264mp4_to_annexb( const h264mp4_to_annexb& obj );
    h264mp4_to_annexb& operator = ( const h264mp4_to_annexb& );

    void _free_filtered_packet();

    AVBitStreamFilterContext* _bsfc;
    AVCodecContext* _codec;
    AVPacket _filteredPacket;
};

}

#endif
