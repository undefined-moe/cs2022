#pragma once
#include <string>
#include<QString>

struct SongInfo {
    QString filepath;
    QString filename;
    QString title;
    QString artist;
    QString album;
    QString comment;
    unsigned int year;
    unsigned int track;
    unsigned int inited = 0;
};
