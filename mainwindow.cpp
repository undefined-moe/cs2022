#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include<QLabel>
#include<QFrame>
#include<QVBoxLayout>
#include<QListWidget>
#include<QLineEdit>
#include<QDir>
#include<QPushButton>

const QString dir = "/root/大作业";

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    player = new QMediaPlayer();

    auto widget = new QWidget;
    setCentralWidget(widget);
    auto layout = new QHBoxLayout();
    auto layoutLeft = new QVBoxLayout();
    auto layoutRight = new QVBoxLayout();
    layout->addLayout(layoutLeft);
    layout->addLayout(layoutRight);

    // QWidget* bottomFiller = new QWidget;
    // bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // layout->setContentsMargins(5, 5, 5, 5);
    // layout->addWidget(bottomFiller);

layoutLeft:
    {
        auto searchBox = new QLineEdit();
        connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::searchBoxChanged);
        layoutLeft->addWidget(searchBox);
        songList = new QListWidget();
        layoutLeft->addWidget(songList);
    }

layoutRight:
    {
        auto title = new QLabel(tr("Select a song to play"));
        layoutRight->addWidget(title);


        auto topFiller = new QWidget;
        topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layoutRight->addWidget(topFiller);



    progressBar:
        {
            auto progressBarLine = new QHBoxLayout();

            auto progressBar = new QSlider(Qt::Orientation::Horizontal);
            progressBarLine->addWidget(progressBar);

            auto timer = new QLabel("--:-- / --:--");
            progressBarLine->addWidget(timer);

            layoutRight->addLayout(progressBarLine);
        }

    actionsList:
        {
            auto actionsList = new QHBoxLayout();

            auto prevButton = new QPushButton();
            prevButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowLeft));
            actionsList->addWidget(prevButton);

            auto pauseButton = new QPushButton();
            pauseButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPause));
            actionsList->addWidget(pauseButton);

            auto stopButton = new QPushButton();
            stopButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaStop));
            actionsList->addWidget(stopButton);

            auto nextButton = new QPushButton();
            nextButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowRight));
            actionsList->addWidget(nextButton);

            layoutRight->addLayout(actionsList);
        }


        infoLabel = new QLabel(tr("<i>Choose a menu option</i>"));
        infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        infoLabel->setAlignment(Qt::AlignCenter);
        layoutRight->addWidget(infoLabel);
    }

    widget->setLayout(layout);
    createActions();
    createMenus();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Music Player"));
    setMinimumSize(160, 160);
    resize(480, 320);
    loadDirectory(dir);
}


void MainWindow::createActions() {
    exitAct = new QAction(tr("&Exit"), this);
    // exitAct->setShortcuts(QKeySequence::);
    exitAct->setStatusTip(tr("Exit"));
    connect(exitAct, &QAction::triggered, this, []() {exit(0);});
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    // fileMenu->addAction(openAct);
    // fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}

// void MainWindow::contextMenuEvent(QContextMenuEvent* event) {
//     QMenu menu(this);
// }

MainWindow::~MainWindow() {
    std::cout << "Shutting down" << std::endl;
    delete ui;
}
