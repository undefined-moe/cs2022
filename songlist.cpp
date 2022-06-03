#include "mainwindow.h"
#include<iostream>
#include "ui_mainwindow.h"
#include<QLabel>
#include<QFrame>
#include<QVBoxLayout>
#include<QListWidget>
#include<QLineEdit>
#include<QMultimedia>
#include<QDir>
#include<QDebug>
#include "taglib.cpp"

int fileNameLength = 1024;
int mp3TagSize = 128;

using namespace std;

void MainWindow::addSong(SongInfo info) {
    auto item = new QListWidgetItem();
    auto widget = new QWidget();
    auto widgetLayout = new QVBoxLayout();
    item->setWhatsThis(info.filepath);

    qInfo() << "addSong" << info.title << ' ' << info.artist;

    widgetLayout->addWidget(new QLabel(info.filename));
    widgetLayout->addWidget(new QLabel(info.title + " - " + info.artist));

    widgetLayout->addStretch();
    widgetLayout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setLayout(widgetLayout);
    item->setSizeHint(widget->sizeHint());
    ui->songList->addItem(item);
    ui->songList->setItemWidget(item, widget);
}

QString dirStorage;

void MainWindow::loadDirectory(const QString dir) {
    QDir directory(dir);
    if (!directory.exists()) {
        statusBar()->showMessage(tr("Folder doesn't exist"));
        return;
    }
    dirStorage = dir;
    auto filenames = directory.entryList(QStringList() << "*.mp3" << "*.wav" << "*.m4a", QDir::Files);
    ui->songList->clear();
    songs.clear();
    foreach(auto filename, filenames) {
        SongInfo s;
        s.filepath = dir + "/" + filename;
        s.filename = filename;
        s = GetTagInfo(s);
        addSong(s);
        songs << s;
    }
    statusBar()->showMessage(tr("Loaded %1 songs").arg(songs.length()));
}

void MainWindow::searchBoxChanged(const QString& text) {
    ui->songList->clear();
    int cnt = 0;
    foreach(auto song, songs) {
        if (
            song.filename.indexOf(text) != -1
            || song.artist.indexOf(text) != -1
            || song.album.indexOf(text) != -1
            || song.title.indexOf(text) != -1
            ) {
            cnt++;
            addSong(song);
        }
    }
    statusBar()->showMessage(tr("Matched %1 songs").arg(cnt));
}