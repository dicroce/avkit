
#include "jpeg_encoder_test.h"
#include "avkit/jpeg_encoder.h"
#include "avkit/locky.h"
#include "avkit/options.h"
#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace cppkit;
using namespace avkit;

REGISTER_TEST_FIXTURE(jpeg_encoder_test);

void jpeg_encoder_test::setup()
{
    locky::register_ffmpeg();

    // pic_0 comes from the above included file pic.c
    _pic = make_shared<ck_memory>();
    memcpy( _pic->extend_data( pic_0_len ).get_ptr(), pic_0, pic_0_len );
}

void jpeg_encoder_test::teardown()
{
    locky::unregister_ffmpeg();
}

void jpeg_encoder_test::test_constructor()
{
    UT_ASSERT_NO_THROW( shared_ptr<jpeg_encoder> e = make_shared<jpeg_encoder>( get_jpeg_options( 1280, 720 ) ) );
}

void jpeg_encoder_test::test_encode()
{
    shared_ptr<jpeg_encoder> e;
    UT_ASSERT_NO_THROW( e = make_shared<jpeg_encoder>( get_jpeg_options( 1280, 720 ) ) );

    shared_ptr<ck_memory> jpeg = e->encode_yuv420p( _pic );

    UT_ASSERT( jpeg->size_data() > 0 );

#if 0 // To verify that jpeg contains a valid jpeg, enable this block of code.
    jpeg_encoder::write_jpeg_file( "out.jpg", jpeg );
#endif
}
