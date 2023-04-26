#ifndef OPERATIONSWINDOW_H
#define OPERATIONSWINDOW_H

#include <QMainWindow>
#include "audiofile.h"
namespace Ui {
class OperationsWindow;
}

class OperationsWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit OperationsWindow(QWidget *parent = nullptr);
    ~OperationsWindow();
    void setFiles(const QList<AudioFile*>audioFiles);
private slots:
    void on_findBitRateBtn_clicked();

    void on_findCodecBtn_clicked();

    void on_groupAndSortBtn_clicked();

    void on_getDurationBtn_clicked();

    void on_getMostUsedBitRateBtn_clicked();

    void on_restoreBtn_clicked();

    void on_actionWrite_File_Info_triggered();

private:
    QFile fileToWrite;
    QString fileName;
    QList<AudioFile*> files;
    Ui::OperationsWindow *ui;
    QStringList headers;
    void showFiles(const QList<AudioFile*>filesToShow);
    void QuickSort(QList<AudioFile*>&fileList,int leftInd,int rightInd);
    int partition(QList<AudioFile*>&fileList,int left,int right);
};

#endif // OPERATIONSWINDOW_H
