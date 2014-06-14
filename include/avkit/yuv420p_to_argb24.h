
#ifndef __avkit_yuv420p_to_argb24_h
#define __avkit_yuv420p_to_argb24_h

#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"

namespace avkit
{

class yuv420p_to_argb24
{
public:
    yuv420p_to_argb24();

    virtual ~yuv420p_to_argb24() throw();

    void transform( uint8_t* src, size_t width, size_t height );
    void transform( std::shared_ptr<cppkit::ck_memory> src, size_t width, size_t height );

    size_t get_argb24_size() const;
    void get_argb24( uint8_t* dest ) const;
    std::shared_ptr<cppkit::ck_memory> get_argb24() const;

private:
    yuv420p_to_argb24( const yuv420p_to_argb24& obj );
    yuv420p_to_argb24& operator = ( const yuv420p_to_argb24& );

    std::shared_ptr<cppkit::ck_memory> _rgb24;
};

}

#endif
