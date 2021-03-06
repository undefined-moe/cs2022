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

using namespace std;

void MainWindow::addSong(SongInfo info) {
    auto item = new QListWidgetItem();
    auto widget = new QWidget();
    auto widgetLayout = new QVBoxLayout();
    item->setWhatsThis(info.filepath);
    item->setText(QString("                                                                                                   ") + info.filename + ' ' + info.album + ' ' + info.title + ' ' + info.artist);
    item->setTextColor(QColor(255, 255, 255, 0));

    widgetLayout->addWidget(new QLabel(info.filename));
    widgetLayout->addWidget(new QLabel(info.title + " - " + info.artist));
    widgetLayout->addStretch();
    widgetLayout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setLayout(widgetLayout);
    item->setSizeHint(widget->sizeHint());
    ui->songList->addItem(item);
    ui->songList->setItemWidget(item, widget);

    playlist->addMedia(QUrl::fromLocalFile(info.filepath));
}

void MainWindow::loadDirectory(const QString dir) {
    QDir directory(dir);
    if (!directory.exists()) {
        statusBar()->showMessage(tr("Folder doesn't exist"));
        return;
    }
    auto filenames = directory.entryList(QStringList() << "*.mp3" << "*.wav" << "*.m4a", QDir::Files);
    ui->songList->clear();
    songs.clear();
    playlist->clear();
    QList<SongInfo> songs1;
    foreach(auto filename, filenames) {
        SongInfo s;
        s.filepath = dir + "/" + filename;
        s.filename = filename;
        s = GetTagInfo(s);
        songs1 << s;
    }
    auto order = ui->comboBox->currentText();
    std::sort(songs1.begin(), songs1.end(), [order](SongInfo a, SongInfo b) {
        if (order == "By filename") {
            return a.filename < b.filename;
        } else if (order == "By artist") {
            return a.artist < b.artist;
        } else if (order == "By album") {
            return a.album < b.album;
        }
        });
    foreach(auto s, songs1) {
        addSong(s);
        songs << s;
    }
    statusBar()->showMessage(tr("Loaded %1 songs").arg(songs.length()));
}
