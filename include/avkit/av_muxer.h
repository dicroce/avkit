
#ifndef __avkit_av_muxer_h
#define __avkit_av_muxer_h

#include "avkit/options.h"

#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavformat/avformat.h"
}

namespace avkit
{

class av_muxer
{
public:

    enum OUTPUT_LOCATION
    {
        OUTPUT_LOCATION_FILE,
        OUTPUT_LOCATION_BUFFER
    };

    av_muxer( const struct codec_options& options,
              const cppkit::ck_string& fileName,
              OUTPUT_LOCATION location );

    virtual ~av_muxer() throw();

    cppkit::ck_string get_file_name() const;

    void set_extra_data( std::shared_ptr<cppkit::ck_memory> extraData );

    void write_video_frame( uint8_t* data, size_t size, bool keyFrame );
    void write_video_frame( std::shared_ptr<cppkit::ck_memory> frame, bool keyFrame );

    void flush();

    void finalize_buffer( std::shared_ptr<cppkit::ck_memory> buffer );
    void finalize_file();

    void apply_codec_options( const struct codec_options& options );

private:
    av_muxer( const av_muxer& obj );
    av_muxer& operator = ( const av_muxer& obj );

    void _finalize_common();

    void _open_io();

    void _close_io();

    struct codec_options _options;
    cppkit::ck_string _fileName;
    AVFormatContext* _context;
    AVStream* _stream;
    OUTPUT_LOCATION _location;
    int64_t _ts;
    bool _oweTrailer;
    int64_t _numVideoFramesWritten;
    bool _isTS;       // true if our container type is mpeg2ts
    int64_t _fileNum; // the number of files made (really only useful for mpeg2ts)o
};

}

#endif
