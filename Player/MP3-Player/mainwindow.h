#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QClipboard>
#include <QTime>
#include <QMediaPlayer>
#include <QUrl>
#include <QMediaPlaylist>
#include "audiofile.h"
#include "propertieswindow.h"
#include "operationswindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnRepeat_clicked();
    void on_btnShuffel_clicked();
    void on_btnPlay_clicked();
    void actionCopy_to_Clipboard_triggered(int song);
    void actionRemove_File_triggered(int song);
    void actionClear_all_triggered();
    void actionAdd_to_Queue_triggered(int song);
    void actionPlay_Selected_triggered(int song);
    void actionOpen_Properties(int song);

    void on_btnNext_clicked();
    void on_btnPrev_clicked();
    void on_btnStop_clicked();
    void on_btnMute_clicked();

    void on_SeekSlider_sliderMoved(int position);
    void on_VolumeSlider_sliderMoved(int position);

    void selectRowAccordingToCurrentlyPlayingSong();
    void ElapsedTime(qint64 x);
    void RemaningTime(qint64 x);

    void on_actionAdd_dir_triggered();

    void on_actionAdd_Files_triggered();

    void on_TablePlayList_cellClicked(int row, int column);

    void on_TablePlayList_customContextMenuRequested(const QPoint &pos);



    void on_actionOpen_operations_window_triggered();

private:
    Ui::MainWindow *ui;

    enum RepeatFlags {Sequential = 0, CurrentItemLoop = 1, AllLoop =2};

    OperationsWindow *operationsWindow = nullptr;
    PropertiesWindow *propertiesWindow = nullptr;
    bool ShuffleFlag;
    bool isPlayingFlag;
    bool paused;
    int RepeatStat;
    QList<AudioFile*> content;
//    QList<QMediaContent>content;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

};

#endif // MAINWINDOW_H
