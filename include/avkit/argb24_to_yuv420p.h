
#ifndef __avkit_argb24_to_yuv420p_h
#define __avkit_argb24_to_yuv420p_h

#include <memory>
#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"
#include "cppkit/os/ck_exports.h"

namespace avkit
{

class argb24_to_yuv420p
{
public:
    CK_API argb24_to_yuv420p();
    CK_API virtual ~argb24_to_yuv420p() throw();

    CK_API void transform( uint8_t* src, size_t width, size_t height );
    CK_API void transform( std::shared_ptr<cppkit::ck_memory> src, size_t width, size_t height );

    size_t get_yuv420p_size() const;
    void get_yuv420p( uint8_t* dest ) const;
    std::shared_ptr<cppkit::ck_memory> get_yuv420p() const;

private:
    argb24_to_yuv420p( const argb24_to_yuv420p& obj );
    argb24_to_yuv420p& operator = ( const argb24_to_yuv420p& );

    std::shared_ptr<cppkit::ck_memory> _yuv420;
};

}

#endif
