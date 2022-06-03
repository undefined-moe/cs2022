#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLabel>
#include<QFrame>
#include<QVBoxLayout>
#include<QListWidget>
#include<QLineEdit>
#include<QDir>
#include<QPushButton>
#include<QDebug>

const QString dir = "/root/大作业";

QString formatTime(qint64 t) {
    auto sec = t / 1000;
    auto minute = sec / 60;
    sec %= 60;
    QString q = QString::number(minute) + ":";
    if (sec < 10) q += "0";
    q += QString::number(sec);
    return q;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    player = new QMediaPlayer();
    stopMusic();

    ui->topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->progressBar->setOrientation(Qt::Orientation::Horizontal);

    ui->prevButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowLeft));
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPlay));
    ui->stopButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaStop));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowRight));

    ui->pauseButton->setShortcut(QKeySequence(Qt::Key_Space));
    ui->prevButton->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_Left));
    ui->nextButton->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_Right));

    connect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchBoxChanged);
    connect(ui->songList, &QListWidget::itemClicked, this, &MainWindow::playMusic);

    connect(player, &QMediaPlayer::durationChanged, ui->progressBar, &QSlider::setMaximum);
    connect(player, &QMediaPlayer::durationChanged, this,
        [this](qint64 duration) {ui->totalLength->setText(formatTime(duration));});

    connect(player, &QMediaPlayer::positionChanged, ui->progressBar, &QSlider::setValue);
    connect(player, &QMediaPlayer::positionChanged, this,
        [this](qint64 position) {ui->currentPos->setText(formatTime(position));});

    connect(ui->progressBar, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopMusic);
    connect(ui->pauseButton, &QPushButton::clicked, this,
        [this] {
            if (player->state() == player->PlayingState) player->pause();
            else player->play();
            ui->pauseButton->setIcon(style()->standardIcon(state == player->PlayingState
                ? QStyle::StandardPixmap::SP_MediaPause
                : QStyle::StandardPixmap::SP_MediaPlay));
        });

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit"));
    connect(exitAct, &QAction::triggered, this, []() {exit(0);});
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);
    statusBar()->showMessage(tr("Select a song to play"));
    setWindowTitle(tr("Music Player"));
    setMinimumSize(160, 160);
    resize(480, 320);
    qInfo() << "UI initialized";
    loadDirectory(dir);
}

void MainWindow::playMusic(QListWidgetItem* item) {
    qDebug() << item;
    QString filename = item->whatsThis();
    qDebug() << "Playing " << filename;
    statusBar()->showMessage("Loading file: " + filename);
    player->setMedia(QUrl::fromLocalFile(filename));
    player->play();
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPause));
    statusBar()->showMessage("Playing: " + filename);
}

void MainWindow::stopMusic() {
    ui->currentPos->setText("--:--");
    ui->timerSep->setText("/");
    ui->totalLength->setText("--:--");
    ui->progressBar->setValue(0);
    player->stop();
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPlay));
}

// void MainWindow::contextMenuEvent(QContextMenuEvent* event) {
//     QMenu menu(this);
// }

MainWindow::~MainWindow() {
    qInfo() << "Shutting down";
    delete ui;
}
