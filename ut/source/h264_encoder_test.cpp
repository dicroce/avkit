
#include "h264_encoder_test.h"
#include "avkit/h264_encoder.h"
#include "avkit/h264_decoder.h"
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

REGISTER_TEST_FIXTURE(h264_encoder_test);

void h264_encoder_test::setup()
{
    //av_register_all();
    locky::register_ffmpeg();

    // pic_0 comes from the above included file pic.c
    _pic = make_shared<ck_memory>();
    memcpy( _pic->extend_data( pic_0_len ).get_ptr(), pic_0, pic_0_len );
}

void h264_encoder_test::teardown()
{
    locky::unregister_ffmpeg();
}

void h264_encoder_test::test_constructor()
{
    UT_ASSERT_NO_THROW( shared_ptr<h264_encoder> e = make_shared<h264_encoder>( get_fast_h264_encoder_options( 500000, 1280, 720, 15, 1, 15 ) ) );
}

void h264_encoder_test::test_encode_key()
{
    shared_ptr<h264_encoder> e;
    UT_ASSERT_NO_THROW( e = make_shared<h264_encoder>( get_fast_h264_encoder_options( 500000, 1280, 720, 15, 1, 15 ) ) );

    shared_ptr<ck_memory> output;
    UT_ASSERT_NO_THROW( output = e->encode_yuv420p( _pic, h264_encoder::FRAME_TYPE_KEY ) );

    UT_ASSERT( output->size_data() > 0 );
}

void h264_encoder_test::test_encode_gop()
{
    shared_ptr<h264_decoder> d;
    UT_ASSERT_NO_THROW( d = make_shared<h264_decoder>( get_fast_h264_decoder_options() ) );

    d->set_output_width( 640 );
    d->set_output_height( 360 );

    shared_ptr<h264_encoder> e;
    UT_ASSERT_NO_THROW( e = make_shared<h264_encoder>( get_fast_h264_encoder_options( 500000, 640, 360, 15, 1, 5 ) ) );

    list<shared_ptr<ck_memory> > outputFrames;

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;
        d->decode( gop[index].frame, gop[index].frameSize );

        outputFrames.push_back( e->encode_yuv420p( d->make_yuv420p() ) );
    }

    UT_ASSERT_NO_THROW( d = make_shared<h264_decoder>( get_fast_h264_decoder_options() ) );

    d->set_output_width( 640 );
    d->set_output_height( 360 );

    list<shared_ptr<ck_memory> >::iterator i;
    for( i = outputFrames.begin(); i != outputFrames.end(); i++ )
    {
        shared_ptr<ck_memory> frame = *i;
        d->decode( frame->map().get_ptr(), frame->size_data() );

        shared_ptr<ck_memory> pic = make_shared<ck_memory>();
        UT_ASSERT_NO_THROW( pic = d->make_yuv420p() );

#if 0
        shared_ptr<jpeg_encoder> e = make_shared<jpeg_encoder>( get_jpeg_options( 640, 360 ) );

        jpeg_encoder::write_jpeg_file( "out2.jpg",
                                       e->encode_yuv420p( pic ) );
#endif
    }
}
