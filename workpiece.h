#ifndef WORKPIECE_H
#define WORKPIECE_H

#include <QDialog>
#include "quiwidget.h"
//#include <occview.h>
#include "myview.h"
#include <QRect>
#include <QMutex>
#include <QMouseEvent>
#include "toolcoord.h"
#include "box.h"
#include "cylinder.h"
#include "pipes.h"
#include "line.h"
#include "circular.h"
#include "wedge.h"
#include "sweeps.h"
#include "globalvar.h"
#include "occstlreader.h"


namespace Ui {
class WorkPiece;
}

class WorkPiece : public QDialog
{
    Q_OBJECT

public:
    explicit WorkPiece(QWidget *parent = nullptr);
    ~WorkPiece();
    static WorkPiece *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new WorkPiece;
            }
        }

        return self;
    }
signals:
    void boxshape();
    void cyclershape();
    void pipeshape();
    void lineshape();
    void arcshape();
    void cyclerfanshape();
    void pipefanshape();
    void sweepshape();
    //void definedshape(QStringList &paramelist);
    void definedshape(TopoDS_Shape &shapes);
    void getstlmodelformstring(QString str);
private:
    void initform();
    void init3dview();


public slots:
    void on_pushButton_min_clicked();

    void on_pushButton_max_clicked();

    void on_pushButton_close_clicked();
    void showparamerpage(QString text);
    TopoDS_Shape getcurrentshape()
    {
        return  currentshape;
    }

private slots:
    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_pushButton_box_add_clicked();

    void on_pushButton_line_ok_clicked();

    void on_pushButton_arc_ok_clicked();

    void on_pushButton_wedge_ok_clicked();

    void on_pushButton_cycler_ok_2_clicked();

    void on_pushButton_pipe_ok_2_clicked();

    void on_pushButton_point_add_clicked();

    void on_pushButton_sweeps_ok_clicked();

    void on_pushButton_point_clear_clicked();

    void on_pushButton_clicked();
   void on_stackedWidget_parame_currentChanged(int arg1);

   void on_pushButton_input_clicked();

   void on_pushButton_newdir_clicked();

   void shapemodelready(QString filename,TopoDS_Shape shape);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::WorkPiece *ui;
    bool max;
    QRect location;
    static WorkPiece *self;
    QStringList items;
    //OccView *myview;
    myview *m_view;
    bool ispress;
    QPoint startpos;
    //ToolCoord *toolcd;
    QVector<subwire> sweepparame;
    TopoDS_Shape currentshape;
    OccStlReader *reader;


};

#endif // WORKPIECE_H
