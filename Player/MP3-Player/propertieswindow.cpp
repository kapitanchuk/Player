#include "propertieswindow.h"
#include "ui_propertieswindow.h"
#include <math.h>
#include <QMessageBox>

PropertiesWindow::PropertiesWindow(QWidget *parent,const AudioFile*file) :
    QDialog(parent),file(file),
    ui(new Ui::PropertiesWindow)
{
    ui->setupUi(this);

    try{
        showProperties();
    }
    catch(std::bad_typeid){
        QMessageBox::critical(this,"Error","File was nullptr");
        qDebug()<<"file was nullptr";
    }
    catch(...){
        QMessageBox::critical(this,"Error","Unexpected error");
        qDebug()<<"unexpected error";

    }
}

PropertiesWindow::~PropertiesWindow()
{
    delete ui;
}

void PropertiesWindow::setFile(const AudioFile *file)
{
    this->file = file;
}

void PropertiesWindow::showProperties()
{

    if(file==nullptr){
        throw std::bad_typeid();
    }
    fileInfo.setFile(file->getFilePath());
    this->setWindowTitle("Properties: "+fileInfo.fileName());
    ui->lineEdit->setText(file->getName());
    ui->lineEdit_2->setText(file->getFormat());
    ui->lineEdit_3->setText(QString::number(floor(file->getDuration()/0.01+0.5)*0.01)+"s");
    ui->lineEdit_4->setText(file->getAudioCodec());
    ui->lineEdit_5->setText(file->getCaraokeText()?"true":"false");
    ui->lineEdit_6->setText(QString::number(file->getSampleRate())+"Гц");
    ui->lineEdit_7->setText(QString::number(file->getBitRate()/1000)+"Кбіт/сек");
    ui->lineEdit_8->setText(file->getPlayer());
}
