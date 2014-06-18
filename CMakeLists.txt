cmake_minimum_required(VERSION 2.8)
project(avkit)

set(SOURCES source/argb24_to_yuv420p.cpp
            source/yuv420p_to_argb24.cpp
            source/h264mp4_to_annexb.cpp
            source/av_demuxer.cpp
            source/av_muxer.cpp
            source/options.cpp
            source/h264_decoder.cpp
            source/h264_encoder.cpp
            source/h264_transcoder.cpp
            source/jpeg_encoder.cpp
            source/locky.cpp
            source/utils.cpp
            source/av_packet.cpp)

set(COMMON_LIBS cppkit avcodec avformat avutil swscale x264)

if(CMAKE_SYSTEM MATCHES "Linux-")
    add_definitions(-D__STDC_CONSTANT_MACROS)
endif(CMAKE_SYSTEM MATCHES "Linux-")

include(../devel_artifacts/build/base_lib.cmake NO_POLICY_SCOPE)
