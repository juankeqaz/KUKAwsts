#ifndef SCANNERSETTING_H
#define SCANNERSETTING_H

#include <QDialog>
#include "quiwidget.h"
#include "occview.h"
#include "uitrasonicsettings.h"
#include "myview.h"
#include "occstlreader.h"


namespace Ui {
class scannersetting;
}

class scannersetting : public QDialog
{
    Q_OBJECT

public:
    explicit scannersetting(QWidget *parent = nullptr);
    ~scannersetting();
    static scannersetting *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new scannersetting;
            }
        }

        return self;
    }
signals:
    void getstlmodelformstring(QString str);
public slots:
    void setparamets(QStringList list);
    void itemslot(int row,int col);
    void showmodel();
    void shapemodelready(QString str,TopoDS_Shape sp);
private:
    int getselectedrow();
    bool isstlpath(QString str);



private slots:
    void on_pushButton_r_pressed();

    void on_pushButton_add_pressed();

    void on_checkBox_clicked();

    void on_progressBar_valueChanged(int value);

    void on_toolButton_settings_clicked();

private:
    Ui::scannersetting *ui;
    static scannersetting* self;
    //OccView *myview;
    QIcon *checkedicon;
    QIcon *uncheckedicon;
    myview *occview;
    OccStlReader *reader;
    QMap<QString,TopoDS_Shape> shapemodel;
    bool islocked=false;

};

#endif // SCANNERSETTING_H
