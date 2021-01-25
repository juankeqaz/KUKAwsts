#ifndef DATAANYSIS_H
#define DATAANYSIS_H

#include <QDialog>
#include <QMutex>
#include "quiwidget.h"
//#include "occview.h"
#include "myview.h"
#include "occ.h"
#include "globalvar.h"
#include "kinematics.h"

namespace Ui {
class DataAnysis;
}

class DataAnysis : public QDialog
{
    Q_OBJECT

public:
    explicit DataAnysis(QWidget *parent = nullptr);
    ~DataAnysis();
    static DataAnysis *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new DataAnysis;
            }
        }

        return self;
    }
public slots:
    void on_pushButton_min_clicked();

    void on_pushButton_max_clicked();

    void on_pushButton_close_clicked();


    void showcurrentpath(QVector<float> path, int index);
    void getballposition();
    void clearpoint();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_toolButton_clear_clicked();
    void on_toolButton_start_clicked();

private:
    void initform();
    void init3D();
    void showpath();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::DataAnysis *ui;
    static DataAnysis* self;
    bool max;
    QRect location;
    //OccView *myview;
    bool ispress=false;
    QPoint startpos;
    myview *occview;
    float toolradius=2;
    QVector<QVector<float> > point;
    unsigned int count=0;
};

#endif // DATAANYSIS_H
