#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ShuffleFlag = false;
    this->RepeatStat= RepeatFlags::Sequential;
    this->paused = true;

    ui->setupUi(this);
    ui->TablePlayList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    player->setPlaylist(playlist);
    player->setVolume(ui->VolumeSlider->value());
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(ElapsedTime(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(RemaningTime(qint64)));
    connect(playlist,&QMediaPlaylist::currentIndexChanged,this,&MainWindow::selectRowAccordingToCurrentlyPlayingSong);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    delete playlist;
}

//////////////Context Menu Buttons//////////////////////
///

void MainWindow::on_actionAdd_dir_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(this,tr("Select dir for files to import"),"C:\\_programmer\\univer\\2_course\\1_sem\\course-project\\music");
    if(directory.isEmpty())
        return;
    QDir dir(directory);
    QStringList list = dir.entryList(QStringList(),QDir::Files);
    if(list.isEmpty())
        return;

    int counter = playlist->mediaCount();
    for(const QString&f:list){
        ui->TablePlayList->insertRow(counter);

        QFileInfo fi(f);

        AudioFile*file = new AudioFile(QUrl::fromLocalFile(dir.path()+"/"+fi.filePath()));
        content.push_back(file);

        QTableWidgetItem *item_title = new QTableWidgetItem(fi.baseName());
        QTableWidgetItem *item_path = new QTableWidgetItem(dir.path()+"/"+f);

        ui->TablePlayList->setItem(counter, 0, item_title);
        ui->TablePlayList->setItem(counter, 1, item_path);
        counter++;
    }
    QList<QMediaContent>mediaContent;
    for(AudioFile*file:content){
        mediaContent.push_back((QMediaContent)*file);
    }
    if(playlist->isEmpty())
        playlist->addMedia(mediaContent);
    else playlist->insertMedia(playlist->mediaCount(),mediaContent);
    ui->TablePlayList->resizeColumnToContents(0);

}

//adding file(s) to playlist table
void MainWindow::on_actionAdd_Files_triggered()
{
    QStringList list  = QFileDialog::getOpenFileNames(this,
                                                     tr("Select Files"), "C:\\_programmer\\univer\\2_course\\1_sem\\course-project\\music");
    if(list.isEmpty())
        return;

    int counter = playlist->mediaCount();
    for(const QString&f:list){
        ui->TablePlayList->insertRow(counter);

        QFileInfo fi(f);
        AudioFile*file = new AudioFile(QUrl::fromLocalFile(fi.filePath()));
        content.push_back(file);

        QTableWidgetItem *item_title = new QTableWidgetItem(fi.baseName());
        QTableWidgetItem *item_path = new QTableWidgetItem(fi.absoluteFilePath());

        ui->TablePlayList->setItem(counter, 0, item_title);
        ui->TablePlayList->setItem(counter, 1, item_path);
        counter++;
    }
    QList<QMediaContent>mediaContent;
    for(AudioFile*file:content){
        mediaContent.push_back((QMediaContent)*file);
    }
    if(playlist->isEmpty())
        playlist->addMedia(mediaContent);
    else playlist->insertMedia(playlist->mediaCount(),mediaContent);
    ui->TablePlayList->resizeColumnToContents(0);
}

void MainWindow::actionPlay_Selected_triggered(int song)
{
    playlist->setCurrentIndex(song);
    player->play();
    QIcon ico;
    ico.addPixmap(QPixmap("../MP3-Player/icons/Play-32.png"));
    ui->btnPlay->setIcon(ico);
}

void MainWindow::actionOpen_Properties(int song)
{
    if(song<0)return;
    propertiesWindow = new PropertiesWindow(this,content[song]);
    propertiesWindow->show();
}

//remove single file from the playlist table
void MainWindow::actionRemove_File_triggered(int song)
{
    playlist->removeMedia(song);
    ui->TablePlayList->removeRow(song);
    if(playlist->isEmpty()){
        QIcon ico;
        ico.addPixmap(QPixmap("../MP3-Player/icons/Play-32.png"));
        ui->btnPlay->setIcon(ico);
    }
    delete content[song];
    content.removeAt(song);
}

//clearing the Playlist table
void MainWindow::actionClear_all_triggered()
{
    playlist->clear();
    for(AudioFile*file:content){
        delete file;
    }
    content.clear();

    QIcon ico;
    ico.addPixmap(QPixmap("../MP3-Player/icons/Play-32.png"));
    ui->btnPlay->setIcon(ico);

    ui->TablePlayList->clearContents();
    ui->TablePlayList->setRowCount(0);
}

//Saving File path to clipboard if needed
void MainWindow::actionCopy_to_Clipboard_triggered(int song)
{
    QClipboard *clipboard = QApplication::clipboard();

    clipboard->setText(ui->TablePlayList->item(song, 1)->text());
    qDebug() << "Clipboard content: " << clipboard->text();

}

//// --------------------- NOT IMPLEMENTED YET --------------------------//
void MainWindow::actionAdd_to_Queue_triggered(int song)
{

}
///--------------------------------------------------------------------//

//////////////End of Context Menu Buttons//////////////////////

//////////////// Buttons /////////////////////////////////
void MainWindow::on_btnRepeat_clicked()
{
    RepeatStat++;

    if(RepeatStat == RepeatFlags::CurrentItemLoop )
    {
        ui->btnRepeat->setChecked(true);
        ui->statusBar->showMessage("Repeat One", 5*1000);
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }else if(RepeatStat == RepeatFlags::AllLoop)
    {
        ui->btnRepeat->setChecked(true);
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        ui->btnRepeat->setStyleSheet("border-color:#1a75ff;""background-color:#66a3ff;");
        ui->statusBar->showMessage("Repeat All", 5*1000);
    }else{
        ui->btnRepeat->setChecked(false);
        ui->btnRepeat->setStyleSheet("");
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        ui->statusBar->showMessage("Repeat Off", 5*1000);
        RepeatStat=0;
    }

}

void MainWindow::on_btnShuffel_clicked()
{
    if(ui->btnShuffel->isChecked())
    {
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        ShuffleFlag = true;
        ui->statusBar->showMessage("Shuffle selection is On", 5*1000);
    }else{
        switch(RepeatStat){
        case RepeatFlags::Sequential:{
            playlist->setPlaybackMode(QMediaPlaylist::Sequential);
            break;
        }
        case RepeatFlags::CurrentItemLoop:{
            playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
            break;
        }
        case RepeatFlags::AllLoop:{
            playlist->setPlaybackMode(QMediaPlaylist::Loop);
            break;
        }
        default:
            return;
        }
        ShuffleFlag = false;
        ui->statusBar->showMessage("Shuffle selection is Off", 5*1000);
    }
}

/////// Play Button

void MainWindow::on_btnPlay_clicked()
{

    if((player->state() == QMediaPlayer::PausedState||player->state() == QMediaPlayer::StoppedState)&&!playlist->isEmpty()){
        player->play();
        QIcon ico;
        ico.addPixmap(QPixmap("../MP3-Player/icons/Pause-32.png"));
        ui->btnPlay->setIcon(ico);
    }
    else{
        player->pause();
        QIcon ico;
        ico.addPixmap(QPixmap("../MP3-Player/icons/Play-32.png"));
        ui->btnPlay->setIcon(ico);
    }


}

/////// Stop Button
void MainWindow::on_btnStop_clicked()
{
    player->stop();
    QIcon ico;
    ico.addPixmap(QPixmap("../MP3-Player/icons/Play-32.png"));
    ui->btnPlay->setIcon(ico);
}

/////// Mute Button
void MainWindow::on_btnMute_clicked()
{
    if(ui->btnMute->isChecked())
    {
        player->setMuted(true);
    }else{
        player->setMuted(false);
    }
}

/////// Next Button
void MainWindow::on_btnNext_clicked()
{
    if(playlist->currentIndex()==playlist->mediaCount()-1){
        playlist->setCurrentIndex(0);
    }
    else{
        playlist->setCurrentIndex(playlist->currentIndex()+1);
    }

}

/////// Previous Button
void MainWindow::on_btnPrev_clicked()
{

    if(playlist->currentIndex()==0){
        playlist->setCurrentIndex(playlist->mediaCount()-1);
    }
    else{
        playlist->setCurrentIndex(playlist->currentIndex()-1);
    }

}

//////////////// End of Buttons /////////////////////////////////

////////////////////////////////////////////


//Seek slider
void MainWindow::on_SeekSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

//Volume Slider
void MainWindow::on_VolumeSlider_sliderMoved(int position)
{
    player->setVolume(position);
}


//Defined slots

void MainWindow::ElapsedTime(qint64 x)
{

    QTime t = QTime(0,0,0);
    t =  t.addMSecs(x);
    ui->lblTimeElsp->setText(t.toString("mm:ss"));
    ui->SeekSlider->setValue(x);


}
/// Slot to update interface
void MainWindow::RemaningTime(qint64 x)
{
    QTime t = QTime(0,0,0);
    t =  t.addMSecs(x);
    ui->lblTimeRemaining->setText(t.toString("mm:ss"));
    //qDebug() << t;
    ui->SeekSlider->setRange(0, x);
}

void MainWindow::on_TablePlayList_cellClicked(int row, int column)
{
    playlist->setCurrentIndex(row);
}

void MainWindow::selectRowAccordingToCurrentlyPlayingSong(){
    ui->TablePlayList->selectRow(playlist->currentIndex());
}

void MainWindow::on_TablePlayList_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->TablePlayList->indexAt(pos);
    int song = index.row();
    qDebug()<<index;
    QMenu *menu = new QMenu(this);
    QIcon ico;
    ico.addPixmap(QPixmap("../MP3-Player/icons/Play-16.png"));
    menu->addAction(ico,"Play selected",this,[this,song](){actionPlay_Selected_triggered(song);});
    menu->addSeparator();
    ico.addPixmap(QPixmap("../MP3-Player/icons/Minus-16.png"));
    menu->addAction(ico,"Remove from tray",this,[this,song](){actionRemove_File_triggered(song);});
    ico.addPixmap(QPixmap("../MP3-Player/icons/Delete-16.png"));
    menu->addAction(ico,"Remove all from tray",this,[this](){actionClear_all_triggered();});
    menu->addSeparator();
    menu->addAction("Properties",this,[this,song](){actionOpen_Properties(song);});

    menu->popup(ui->TablePlayList->viewport()->mapToGlobal(pos));
}

void MainWindow::on_actionOpen_operations_window_triggered()
{
    operationsWindow = new OperationsWindow(this);
    operationsWindow->setFiles(content);
    operationsWindow->show();
}





