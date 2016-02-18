
#include "avkit/locky.h"
#include "cppkit/ck_logger.h"
#include "cppkit/os/ck_platform.h"

using namespace avkit;
using namespace std;
using namespace cppkit;

list<shared_ptr<recursive_mutex> > locky::_locks;
bool locky::_registered = false;

void locky::register_ffmpeg()
{
    av_register_all();

    avformat_network_init();

    av_lockmgr_register( locky::_locky_cb );

    _registered = true;

    FULL_MEM_BARRIER();
}

void locky::unregister_ffmpeg()
{
    avformat_network_deinit();

    av_lockmgr_register( NULL );

    locky::_locks.clear();

    _registered = false;

    FULL_MEM_BARRIER();
}

bool locky::is_registered()
{
    return locky::_registered;
}

recursive_mutex* locky::_create_lock()
{
    shared_ptr<recursive_mutex> newLock = make_shared<recursive_mutex>();
    recursive_mutex* newLockPtr = newLock.get();

    locky::_locks.push_back( newLock );

    return newLockPtr;
}

void locky::_destroy_lock( recursive_mutex* lock )
{
    for( auto i = _locks.begin(), end = _locks.end(); i != end; i++ )
    {
        auto lockRef = *i;
        if( lockRef.get() == lock )
        {
            _locks.erase( i );
            return;
        }
    }

    CK_LOG_NOTICE( "Locky asked to clean up a lock that was not managed!" );
}

int locky::_locky_cb( void** mutex, enum AVLockOp op )
{
    switch( op )
    {
    case AV_LOCK_CREATE:
        *mutex = locky::_create_lock();
        break;
    case AV_LOCK_DESTROY:
        locky::_destroy_lock( (recursive_mutex*)*mutex );
        break;
    case AV_LOCK_OBTAIN:
        ((recursive_mutex*)(*mutex))->lock();
        break;
    case AV_LOCK_RELEASE:
        ((recursive_mutex*)(*mutex))->unlock();
        break;
    default:
        break;
    };

    return 0;
}
