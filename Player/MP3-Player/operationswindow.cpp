#include "operationswindow.h"
#include "ui_operationswindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <math.h>
#include <algorithm>

OperationsWindow::OperationsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OperationsWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList headers_= {"Name","Format","Duration","Audio Codec","Caraoke Text","Sample Rate","Bit Rate","Player"};
    headers = headers_;
    fileName = "output.txt";
}

OperationsWindow::~OperationsWindow()
{
    delete ui;
}


void OperationsWindow::setFiles(const QList<AudioFile*> audioFiles)
{
    files.clear();
    try{
        if(audioFiles.isEmpty())throw std::bad_typeid();
        for(AudioFile*file:audioFiles){
            AudioFile *file_ = file;
            files.push_back(file_);
        }
        showFiles(files);
    }
    catch(std::bad_typeid){
        QMessageBox::critical(nullptr,"Error","No files to load");
        this->close();
    }
    catch(...){
        QMessageBox::critical(nullptr,"Error","Unexpected error");
    }
}

void OperationsWindow::on_restoreBtn_clicked()
{
    showFiles(files);
}

//Finding all the instances of a file with a given bitRate
void OperationsWindow::on_findBitRateBtn_clicked()
{
    try{
        bool ok;
        QString text = QInputDialog::getText(this,"Input Bit Rate","Bit rate:",QLineEdit::Normal,"",&ok);
        if(ok && text.isEmpty())
        {
            throw std::invalid_argument("text field suppose not to be empty");
        }
        if(!ok)return;
        int value = text.toUInt();
        QList<AudioFile*>files_;
        for(AudioFile*file:files){
            if(file->getBitRate()==value){
                AudioFile *file_ = file;
                files_.push_back(file_);

            }

        }
        showFiles(files_);
    }
    catch(std::invalid_argument){
        QMessageBox::warning(this,"Warning","Text field was empty, nothing to find");
    }
    catch(...){
        QMessageBox::warning(this,"Warning","Unexpected Erorr");
    }
}

//Finding all the instances of a file with a given Codec
void OperationsWindow::on_findCodecBtn_clicked()
{
    try{
        bool ok;
        QString text = QInputDialog::getText(this,"Input Codec","Audio Codec:",QLineEdit::Normal,"",&ok);
        if(ok && text.isEmpty())
        {
            throw std::invalid_argument("text field suppose not to be empty");
        }
        if(!ok)return;
        QList<AudioFile*>files_;
        for(AudioFile*file:files){
            if(file->getAudioCodec()==text){
                AudioFile *file_ = file;
                files_.push_back(file_);
            }

        }
        showFiles(files_);
    }
    catch(std::invalid_argument){
        QMessageBox::warning(this,"Warning","Text field was empty, nothing to find");
    }
    catch(...){
        QMessageBox::warning(this,"Warning","Unexpected Erorr");
    }
}

//Group and sort with QuickSort files which have same AudioFormat
void OperationsWindow::on_groupAndSortBtn_clicked()
{
    ui->tableWidget->clear();
    QMap<QString,QList<AudioFile*>> map;
    for(AudioFile*file:files){
        QMap<QString,QList<AudioFile*>>::iterator it = map.find(file->getFormat());
        AudioFile* file_ = file;
        if(it!=map.end()){
            it->push_back(file_);
        }
        else{
            QList<AudioFile*> fileList;
            fileList.push_back(file_);
            map.insert(file->getFormat(),fileList);
        }
    }
    ui->tableWidget->setColumnCount(map.size());
    int max = 0;
    for(QList<AudioFile*>list:map){
        if(list.size()>max){
            max = list.size();
        }
    }
    ui->tableWidget->setRowCount(max);
    int i=0;

    for(QMap<QString,QList<AudioFile*>>::iterator it = map.begin();it!=map.end();it++,i++){
        QTableWidgetItem*item = new QTableWidgetItem(it.key());
        ui->tableWidget->setHorizontalHeaderItem(i,item);
        QuickSort(it.value(),0,it.value().size()-1);
        int j=0;
        for(AudioFile*file_:it.value()){
            QTableWidgetItem*item_ = new QTableWidgetItem(file_->getName()+" -- "+QString::number(file_->getBitRate()));
            ui->tableWidget->setItem(j,i,item_);
            j++;
        }
    }


}

//Get file with least and most durations from files that have caraokeText
void OperationsWindow::on_getDurationBtn_clicked()
{
    QList<AudioFile*>list;
    AudioFile*min = files[0];
    AudioFile*max = files[0];
    for(AudioFile*file:files){
        if(file->getCaraokeText()){
            if(file->getDuration()<min->getDuration()){
                min = file;
            }
            else if(file->getDuration()>max->getDuration()){
                max = file;
            }
        }
    }
    if(min!=max){
        list.push_back(min);
        list.push_back(max);
    }
    else list.push_back(min);

    showFiles(list);
}

//get most used bit rate from files with the same audioCodec
void OperationsWindow::on_getMostUsedBitRateBtn_clicked()
{
    QMap<QString,QList<AudioFile*>> map;
    for(AudioFile*file:files){
        QMap<QString,QList<AudioFile*>>::iterator it = map.find(file->getFormat());
        AudioFile* file_ = file;
        if(it!=map.end()){
            it->push_back(file_);
        }
        else{
            QList<AudioFile*> fileList;
            fileList.push_back(file_);
            map.insert(file->getFormat(),fileList);
        }
    }
    QMap<QString,QMap<int,int>> bitRateMap;
    for(QMap<QString,QList<AudioFile*>>::iterator it = map.begin();it!=map.end();it++){
        QMap<int,int>tempMap;
        bitRateMap.insert(it.key(),tempMap);
        QMap<QString,QMap<int,int>>::iterator it_ = bitRateMap.find(it.key());
            for(AudioFile*file:it.value()){
                QMap<int,int>::iterator iter = it_.value().find(file->getBitRate());
                if(iter!=it_.value().end()){
                    iter.value()++;
                }
                else{
                    it_.value().insert(file->getBitRate(),1);
                }
            }
            it_++;


    }
    QString string;
//    for(QMap<QString,QList<AudioFile*>>::iterator it = map.begin();it!=map.end();it++){
        for(QMap<QString,QMap<int,int>>::iterator iter = bitRateMap.begin();iter!=bitRateMap.end();iter++){

        QMap<int,int>::iterator best = iter.value().begin();
        for(QMap<int,int>::iterator it_ = iter.value().begin();it_!=iter.value().end();it_++){
            if(best.value()<it_.value()){
                best = it_;
            }
//                    QMap<int,int>::iterator best
//                    = std::max_element(iter.value().begin(),iter.value().end(),
//                    [](std::pair<int,int>& a, std::pair<int,int>& b)->bool{ return a.second < b.second; });
//            qDebug()<<it.key()<<"  "<<it_.key()<<"  "<<it_.value();
//        }

    }
        QString temp = "For "+iter.key()+" files, mostly used is "+QString::number(best.key())+" Bit Rate. It was used "+QString::number(best.value())+" times\n\n";
        string.append(temp);


}
        QMessageBox::information(this,"Mostly used Bit Rates",string);
}

void OperationsWindow::showFiles(const QList<AudioFile*>filesToShow)
{
    if(filesToShow.isEmpty()){
        QMessageBox::information(this,"Information","There are no files, corresponding to your request");
        return;
    }
    const QStringList headers_= {"Name","Format","Duration","Audio Codec","Caraoke Text","Sample Rate","Bit Rate","Player"};
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels(headers_);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setRowCount(filesToShow.size());

    int i=0;
    for(const AudioFile*file:filesToShow){
        for(int j=0;j<8;j++){
            QTableWidgetItem *item = nullptr;
            switch(j){
                case 0:
                item = new QTableWidgetItem(file->getName());
                ui->tableWidget->setItem(i,j,item);
                    break;
                case 1:
                item = new QTableWidgetItem(file->getFormat());
                ui->tableWidget->setItem(i,j,item);
                    break;
                case 2:
                item = new QTableWidgetItem(QString::number(floor(file->getDuration()/0.01+0.5)*0.01));
                ui->tableWidget->setItem(i,j,item);
                    break;
                case 3:
                item = new QTableWidgetItem(file->getAudioCodec());
                ui->tableWidget->setItem(i,j,item);
                    break;
                case 4:
                item = new QTableWidgetItem(file->getCaraokeText()?"true":"false");
                ui->tableWidget->setItem(i,j,item);
                    break;
                case 5:
                item = new QTableWidgetItem(QString::number(file->getSampleRate()));
                ui->tableWidget->setItem(i,j,item);
                    break;
                case 6:
                item = new QTableWidgetItem(QString::number(file->getBitRate()));
                ui->tableWidget->setItem(i,j,item);
                    break;
                case 7:
                item = new QTableWidgetItem(file->getPlayer());
                ui->tableWidget->setItem(i,j,item);
                    break;
            }

        }
        i++;
    }
}

void OperationsWindow::QuickSort(QList<AudioFile *> &fileList,int leftInd,int rightInd)
{
    if(leftInd<rightInd){
        int pivot = partition(fileList,leftInd,rightInd);
        QuickSort(fileList,leftInd,pivot-1);
        QuickSort(fileList,pivot+1,rightInd);
    }
}

int OperationsWindow::partition(QList<AudioFile *> &fileList, int left, int right)
{
    AudioFile*pivot = fileList[right];
        int i = (left); // Index of smaller element and indicates
                        // the right position of pivot found so far

           for (int j = left; j < right; j++) {
               // If current element is smaller than the pivot
               if (fileList[j]->getBitRate() < pivot->getBitRate()) {
                   std::swap(fileList[i], fileList[j]);
                   i++; // increment index of smaller element
               }
           }
           std::swap(fileList[i],fileList[right]);
           return i;
}



void OperationsWindow::on_actionWrite_File_Info_triggered()
{
    fileToWrite.setFileName(fileName);
    fileToWrite.open(QIODevice::Append);
    for(AudioFile*file:files){
        fileToWrite<<*file;
    }
    fileToWrite.close();
}

