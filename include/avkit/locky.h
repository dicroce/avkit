
#ifndef __avkit_locky_h
#define __avkit_locky_h

#include "cppkit/os/ck_exports.h"

#include <mutex>
#include <list>
#include <memory>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
};

namespace avkit
{

class locky
{
public:
    CK_API static void register_ffmpeg();
    CK_API static void unregister_ffmpeg();
    CK_API static bool is_registered();

private:
    locky();
    locky( const locky& obj );
    locky& operator = ( const locky& obj );

    static std::recursive_mutex* _create_lock();
    static void _destroy_lock( std::recursive_mutex* lock );

    static int _locky_cb( void** mutex, enum AVLockOp op );

    static std::list<std::shared_ptr<std::recursive_mutex> > _locks;
    static bool _registered;
};

}

#endif
