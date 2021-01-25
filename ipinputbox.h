#ifndef IPINPUTBOX_H
#define IPINPUTBOX_H
#include <QDialog>
#include <QMutex>
#include <QVBoxLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QLineEdit>
#include <QComboBox>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QTextCodec>
#include <quiwidget.h>
#include <QRegExp>
#include <QSettings>



class IPInputbox:public QDialog
{
    Q_OBJECT
public:
    IPInputbox(QWidget *parent=nullptr);

    ~IPInputbox();

    static IPInputbox *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new IPInputbox;
            }
        }

        return self;
    }


signals:
    void hidesignal();
protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *obj, QEvent *evt);

private:
    static IPInputbox *self;

    QVBoxLayout *verticalLayout1;
    QWidget *widgetTitle;
    QHBoxLayout *horizontalLayout1;
    QLabel *labIco;
    QLabel *labTitle;
    QLabel *labTime;
    QWidget *widgetMenu;
    QHBoxLayout *horizontalLayout2;
    QPushButton *btnMenu_Close;
    QWidget *widgetMain;
    QVBoxLayout *verticalLayout2;
    QFrame *frame;
    QVBoxLayout *verticalLayout3;
    QLabel *labInfo;
    QLineEdit *txtValue;
    QLineEdit *txtValue2;
    QLineEdit *portValue;
    QLineEdit *portValue2;
    QComboBox *cboxValue;
    QHBoxLayout *lay;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;

private:
    int closeSec;                   //总显示时间
    int currentSec;                 //当前已显示时间
    QString value;                  //当前值

private slots:
    void initControl();             //初始化控件
    void initForm();                //初始化窗体
    void checkSec();                //校验倒计时



private slots:
    void on_btnOk_clicked();
    void on_btnMenu_Close_clicked();

public:
    QString getValue()const;

public Q_SLOTS:
    void setIconMain(QChar str, quint32 size = 9);
    void setParameter(const QString &title, int type = 0, int closeSec = 0,
                      QString defaultValue = QString(), bool pwd = false);
    void saveipinfor();
    QString getip1();
    QString getip2();
    QString getport1();
    QString getport2();
};

#endif // IPINPUTBOX_H
