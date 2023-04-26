#ifndef PROPERTIESWINDOW_H
#define PROPERTIESWINDOW_H

#include <QDialog>
#include "audiofile.h"

namespace Ui {
class PropertiesWindow;
}

class PropertiesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesWindow(QWidget *parent = nullptr,const AudioFile*file = nullptr);
    ~PropertiesWindow();
    void setFile(const AudioFile*file);

private:
    void showProperties();

    QFileInfo fileInfo;
    const AudioFile *file = nullptr;
    Ui::PropertiesWindow *ui;
};

#endif // PROPERTIESWINDOW_H
