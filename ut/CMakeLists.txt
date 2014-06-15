cmake_minimum_required(VERSION 2.8)
project(ut)

set(SOURCES source/framework.cpp
            source/av_demuxer_test.cpp
            source/av_muxer_test.cpp
            source/h264_decoder_test.cpp
            source/h264_encoder_test.cpp
            source/h264_transcoder_test.cpp
            source/jpeg_encoder_test.cpp)

set(COMMON_LIBS cppkit avkit)

include(../../devel_artifacts/build/base_ut.cmake)
