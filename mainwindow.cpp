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
#include<QFileDialog>
#include<QComboBox>

QString dir = QDir::currentPath();

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
    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::CurrentItemInLoop);
    player->setPlaylist(playlist);
    stopMusic();

    ui->comboBox->addItem("By filename");
    ui->comboBox->addItem("By artist");
    ui->comboBox->addItem("By album");

    ui->topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->progressBar->setOrientation(Qt::Orientation::Horizontal);

    ui->prevButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowLeft));
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPlay));
    ui->stopButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaStop));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowRight));

    ui->pauseButton->setShortcut(QKeySequence(Qt::Key_Space));
    ui->prevButton->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_Left));
    ui->nextButton->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_Right));

    connect(ui->comboBox, &QComboBox::currentTextChanged, this,
        [this](const QString& text) {
            loadDirectory(dir);
        });

    connect(ui->searchBox, &QLineEdit::textChanged, this,
        [this] {
            for (auto item : ui->songList->findItems("", Qt::MatchContains)) {
                item->setHidden(true);
            }
            auto match = ui->songList->findItems(ui->searchBox->text(), Qt::MatchContains);
            for (auto item : match) {
                item->setHidden(false);
            }
            statusBar()->showMessage(tr("Matched %1 songs").arg(match.length()));
        });
    connect(ui->songList, &QListWidget::itemClicked, this,
        [this] {
            playlist->setCurrentIndex(ui->songList->currentRow());
        });

    connect(player, &QMediaPlayer::durationChanged, ui->progressBar, &QSlider::setMaximum);
    connect(player, &QMediaPlayer::durationChanged, this,
        [this](qint64 duration) {ui->totalLength->setText(formatTime(duration));});

    connect(player, &QMediaPlayer::positionChanged, ui->progressBar, &QSlider::setValue);
    connect(player, &QMediaPlayer::positionChanged, this,
        [this](qint64 position) {ui->currentPos->setText(formatTime(position));});

    connect(ui->progressBar, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    connect(ui->prevButton, &QPushButton::clicked, playlist, &QMediaPlaylist::previous);
    connect(ui->nextButton, &QPushButton::clicked, playlist, &QMediaPlaylist::next);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopMusic);
    connect(ui->pauseButton, &QPushButton::clicked, this,
        [this] {
            if (player->state() == player->PlayingState) player->pause();
            else player->play();
            ui->pauseButton->setIcon(style()->standardIcon(player->state() == player->PlayingState
                ? QStyle::StandardPixmap::SP_MediaPause
                : QStyle::StandardPixmap::SP_MediaPlay));
        });
    connect(playlist, &QMediaPlaylist::currentMediaChanged, this,
        [this](QMediaContent item) {
            auto filename = item.canonicalUrl().fileName();
            qInfo() << playlist->currentMedia().canonicalUrl().fileName();
            ui->songList->setCurrentItem(nullptr);
            ui->songList->findItems(filename, Qt::MatchContains)[0]->setSelected(true);
            player->play();
            ui->pauseButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPause));
            qDebug() << "Playing " << filename;
            statusBar()->showMessage("Playing: " + filename);
        });

    fileMenu = menuBar()->addMenu(tr("&File"));

    auto openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a folder"));
    connect(openAct, &QAction::triggered, this,
        [this] {
            dir = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly
            );
            loadDirectory(dir);
        });
    fileMenu->addAction(openAct);

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit"));
    connect(exitAct, &QAction::triggered, this, []() {exit(0);});
    fileMenu->addAction(exitAct);

    statusBar()->showMessage(tr("Select a song to play"));
    setWindowTitle(tr("Music Player"));
    setMinimumSize(160, 160);
    resize(480, 320);
    qInfo() << "UI initialized";
    loadDirectory(dir);
}

void MainWindow::stopMusic() {
    ui->currentPos->setText("--:--");
    ui->timerSep->setText("/");
    ui->totalLength->setText("--:--");
    ui->progressBar->setValue(0);
    player->stop();
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPlay));
}

MainWindow::~MainWindow() {
    qInfo() << "Shutting down";
    delete ui;
}
