
#include "av_muxer_test.h"
#include "avkit/options.h"
#include "avkit/av_muxer.h"
#include "avkit/h264_decoder.h"
#include "avkit/h264_encoder.h"
#include "avkit/jpeg_encoder.h"
#include "avkit/locky.h"
#include "avkit/options.h"
#include "cppkit/ck_memory.h"

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace cppkit;
using namespace avkit;

REGISTER_TEST_FIXTURE(av_muxer_test);

void av_muxer_test::setup()
{
    locky::register_ffmpeg();

    // pic_0 comes from the above included file pic.c
    _pic = make_shared<ck_memory>();
    memcpy( _pic->extend_data( pic_0_len ).get_ptr(), pic_0, pic_0_len );
}

void av_muxer_test::teardown()
{
    locky::unregister_ffmpeg();
}

void av_muxer_test::test_constructor()
{
    UT_ASSERT_NO_THROW( shared_ptr<av_muxer> c = make_shared<av_muxer>( get_fast_h264_encoder_options( 500000, 1280, 720, 15, 1, 15 ), "foo.mp4", av_muxer::OUTPUT_LOCATION_BUFFER ) );
}

void av_muxer_test::test_mp4()
{
    shared_ptr<h264_decoder> d = make_shared<h264_decoder>( get_fast_h264_decoder_options() );
    d->set_output_width( 640 );
    d->set_output_height( 360 );
    shared_ptr<h264_encoder> e = make_shared<h264_encoder>( get_fast_h264_encoder_options( 250000, 640, 360, 15, 1, 15 ) );
    shared_ptr<av_muxer> c = make_shared<av_muxer>( e->get_options(),
                                                    "foo.mp4",
                                                    av_muxer::OUTPUT_LOCATION_FILE );

    c->set_extra_data( e->get_extra_data() );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;

        shared_ptr<av_packet> pkt = make_shared<av_packet>( gop[index].frame, gop[index].frameSize, false );
        d->decode( pkt );
        e->encode_yuv420p( d->get() );
        c->write_video_packet( e->get(), ((i % 15) == 0) ? true : false );
    }

    c->finalize_file();
}

void av_muxer_test::test_recontainerize()
{
    struct codec_options options;

    options.video_codec = "h264";
    options.gop_size = 15;
    options.bit_rate = 1424400; // size of gop * 8 == bit_rate
    options.width = 1280;
    options.height = 720;
    options.time_base_num = 1;
    options.time_base_den = 30;

    shared_ptr<av_muxer> c = make_shared<av_muxer>( options,
                                                    "bar.mp4",
                                                    av_muxer::OUTPUT_LOCATION_FILE );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;
        shared_ptr<av_packet> pkt = make_shared<av_packet>( gop[index].frame, gop[index].frameSize, false );
        c->write_video_packet( pkt, ((i % 15) == 0) ? true : false );
    }

    c->finalize_file();
}
