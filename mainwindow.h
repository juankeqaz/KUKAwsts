#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include "maintitle.h"
#include <runpage.h>
#include "ipinputbox.h"
#include "dataanysis.h"
#include "workpiece.h"
#include "scannersetting.h"
#include "detecsting.h"
#include <QSettings>
#include <QCloseEvent>
#include <globalvar.h>
#include <tcpserver.h>
#include "homeseting.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void initform();
    void initmainwidget();
    void backstartpage();
    void savecurrentuserlist();
protected:
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent* event);
private slots:
    void on_pushButton_clicked();
    void updatatime();
    void getcurrentindex(int index);
    void hideipsettingwidget();
    void on_pushButton_showset_clicked();
    void on_pushButton_zc_clicked();

    void on_lineEdit_password_textChanged(const QString &arg1);

    void on_checkBox_clicked();

    void on_pushButton_saveip_clicked();
    void checkstatus();

    void on_pushButton_robotip_clicked();
    void robotconnected();
    void robotdisconnected();
    void plantconnected();
    void plantdisconnected();

    void on_pushButton_toolplantip_clicked();

    void on_pushButton_2_clicked();


    void on_pushButton_Home_clicked();

    void on_pushButton_New_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *mlbg;
    QTimer *timer;
    MainTitle *title;
    QList<QString> ttext;
    Runpage *runpage;
    //OccView *myview;
    IPInputbox *ipinput;
    DataAnysis *anysis;
    WorkPiece *workpiece;
    scannersetting *scanner;
    DetecSting *Detec;
    QVector<QStringList> userlist;
    QString currentuser;
    TcpServer *robserver=nullptr;
    TcpServer *UIrserver=nullptr;
    Homeseting *homesetingpage;

};
#endif // MAINWINDOW_H
