#pragma once
#include "types.cpp"
#include "taglib/mp4file.h"
#include "taglib/mp4coverart.h"
#include "taglib/flacfile.h"
#include "taglib/flacpicture.h"
#include "taglib/mpegfile.h"
// #include "taglib/attachedpictureframe.h"
// #include "taglib/id3v2tag.h"
// #include "taglib/id3v1tag.h"
// #include "taglib/ape/apefile.h"
// #include "taglib/wavpack/wavfile.h"
#include "taglib/mpcfile.h"
// #include "taglib/opusfile.h"
// #include "taglib/wavpack/wavpackfile.h"
// #include "taglib/vorbisfile.h"
#include "taglib/trueaudiofile.h"
// #include "taglib/aifffile.h"
// #include "taglib/asffile.h"
// #include "taglib/tpropertymap.h"
// #include "taglib/apetag.h"
#include "taglib/fileref.h"
// #include "taglib/speexfile.h"
// #include "taglib/unsynchronizedlyricsframe.h"
// #include "taglib/id3v2frame.h"
// #include "taglib/popularimeterframe.h"
#include<cstring>
#include<map>
#include<QDebug>

using namespace TagLib;

SongInfo GetMpegTagInfo(SongInfo file) {
    MPEG::File f(file.filepath.toStdString().c_str());
    auto tag = f.tag();
    qInfo() << "GetTagInfo" << file.filepath;
    if (tag == nullptr) return file;
    file.title = QString::fromStdWString(tag->title().toWString());
    qInfo() << "artist = " << tag->artist().toCString();
    file.artist = QString::fromStdWString(tag->artist().toWString());
    qInfo() << file.artist;
    file.album = QString::fromStdWString(tag->album().toWString());
    file.year = tag->year();
    file.track = tag->track();
    file.comment = QString::fromStdWString(tag->comment().toWString());
    file.inited = 1;
    return file;
}

SongInfo GetTagInfo(SongInfo file) {
    return GetMpegTagInfo(file);
}
