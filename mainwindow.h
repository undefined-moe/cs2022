#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"types.cpp"
#include <QMainWindow>
#include<QActionGroup>
#include<QMediaPlayer>
#include<QLabel>
#include<QListWidget>
#include<QMediaPlaylist>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    void createActions();
    void createMenus();
    void loadDirectory(const QString dir);
    void addSong(SongInfo song);
    void stopMusic();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 duration);
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    SongInfo currentPlaying;
    QMenu* fileMenu;
    QAction* openAct;
    QAction* exitAct;
    QLabel* infoLabel;
    QList<SongInfo> songs;

private slots:
    // void openFolder();

protected:
    // void contextMenuEvent(QContextMenuEvent* event) override;

};
#endif // MAINWINDOW_H
