
#ifndef __avkit_av_packet_h
#define __avkit_av_packet_h

#include "cppkit/ck_types.h"
#include "cppkit/os/ck_exports.h"

class av_packet_test;

namespace avkit
{

class av_packet
{
    friend class ::av_packet_test;

public:
    CK_API av_packet( size_t sz );
    CK_API av_packet( uint8_t* src, size_t sz, bool owning = true );
    CK_API av_packet( const av_packet& obj );
    CK_API av_packet( av_packet&& obj ) throw();
    CK_API virtual ~av_packet() throw();

    CK_API av_packet& operator = ( const av_packet& obj );
    CK_API av_packet& operator = ( av_packet&& obj ) throw();

    CK_API uint8_t* map() const;

    CK_API size_t get_buffer_size() const;

    CK_API void set_data_size( size_t sz );
    CK_API size_t get_data_size() const;

    CK_API void migrate_md_from( const av_packet& obj );

    CK_API void set_pts( int64_t pts );
    CK_API int64_t get_pts() const;

    CK_API void set_dts( int64_t dts );
    CK_API int64_t get_dts() const;

    CK_API void set_ts_freq( uint32_t freq );
    CK_API uint32_t get_ts_freq() const;

    CK_API void set_duration( uint32_t duration );
    CK_API uint32_t get_duration() const;

    CK_API void set_key( bool key );
    CK_API bool is_key() const;

    CK_API void set_width( uint16_t width );
    CK_API uint16_t get_width() const;

    CK_API void set_height( uint16_t height );
    CK_API uint16_t get_height() const;

private:
    void _clear() throw();

    // The actual size of the buffer we may or may not have allocated (includes padding).
    size_t _bufferSize;
    // The requested buffer size.
    size_t _requestedSize;
    // true if we own the memory of our buffer (and we therefore need to copy the data in), false otherwise.
    bool _owning;
    // a pointer to our buffer (that we may more may not have allocated).
    uint8_t* _buffer;
    // the amount of valid (set) memory in the buffer.
    size_t _dataSize;
    // the current presentation timestamp
    int64_t _pts;
    // the current decode timestamp
    int64_t _dts;
    // how many _ts ticks are in one second.
    uint32_t _ticksInSecond;
    // whether this packet is a key frame
    bool _key;
    // If known, the width of the frame
    uint16_t _width;
    // If known, the height of the frame
    uint16_t _height;
    // duration of packet
    uint32_t _duration;
};

}

#endif
