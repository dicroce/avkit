cmake_minimum_required(VERSION 2.8)
project(avkit)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(SOURCES include/avkit/argb24_to_yuv420p.h
            include/avkit/av_demuxer.h
            include/avkit/av_muxer.h
            include/avkit/av_packet.h
            include/avkit/av_packet_factory.h
            include/avkit/frame_types.h
            include/avkit/h264_decoder.h
            include/avkit/h264_encoder.h
            include/avkit/h264_transcoder.h
            include/avkit/h264mp4_to_annexb.h
            include/avkit/jpeg_encoder.h
            include/avkit/locky.h
            include/avkit/options.h
            include/avkit/utils.h
            include/avkit/yuv420p_to_argb24.h
            source/argb24_to_yuv420p.cpp
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
            source/av_packet.cpp
            source/av_packet_factory.cpp)

set(COMMON_LIBS cppkit avcodec avformat swscale avfilter avutil)
#set(THIRD_PARTY_LIBS avcodec avformat swscale avfilter avutil)

if(CMAKE_SYSTEM MATCHES "Linux-")
    include_directories(/usr/include/ffmpeg)
    add_definitions(-D__STDC_CONSTANT_MACROS)
    include_directories(/usr/include/ffmpeg)
endif(CMAKE_SYSTEM MATCHES "Linux-")

include(common.cmake NO_POLICY_SCOPE)
include("${devel_artifacts_path}/build/base_lib.cmake" NO_POLICY_SCOPE)
