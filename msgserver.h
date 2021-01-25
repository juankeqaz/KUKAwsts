#ifndef MSGSERVER_H
#define MSGSERVER_H

#include <QObject>
#include <QVariant>
#include <QTimer>
//#include "kinematics_interface.h"
#include <QReadWriteLock>
//#include "status.h"
#define POS_NUM 131072
#define POS_NUM1 466033.78
#define POS_NUM2 466033.78
#define POS_NUM3 291271.0

static QVector<float> pos_offset={1,1,1,1,1,1};
static QVector<float> stand_pos={0,0,0,0,0,0};

#define STAND_OFF 0.01
typedef unsigned char FRAMEHEADTYPE;
typedef unsigned char DATALENGTHTYPE;
typedef unsigned char FRAMETAILTYPE;
typedef unsigned char DATATYPE;

struct FRAME{
    quint8 head;
    bool headvalid;
    quint8 datalength;
    bool datalengthvalid;
    QVector<quint8> data;
    bool datavalid;
    quint8 tail;
    bool tailvalid;
};
Q_DECLARE_METATYPE(FRAME)

#define FRAMEHEAD (FRAMEHEADTYPE)0xfe
#define FRAMETAIL (FRAMETAILTYPE)0xff
//#define RAD2DEG (180.0/M_PI)
//#define DEG2RAD (M_PI/180.0)

//static QStringList aglist;
//static QVector<int> maxspeed={20,20,20,20,20,20};

typedef union
{
    struct
    {
        unsigned char low_byte;
        unsigned char mlow_byte;
        unsigned char mhigh_byte;
        unsigned char high_byte;
    }float_byte;
    qint32 value;
}JOINT_ANGLE;
typedef union
{
    struct
    {
        unsigned char low_byte;
        unsigned char high_byte;
    }short_byte;
    short value;
}CURRENT_VALUE;

//Q_DECLARE_METATYPE(FRAME)
class msgserver:public QObject
{
    Q_OBJECT
public:
    msgserver(QObject *parent=0);
    ~msgserver();
signals:
    void lowerrespond(int flag);
    void setiosignal(quint16);
    void changecurspeed(int speed);
    void setmaxcount(int count);
    void motionfinishedsignal();
    void robotstatussignal(QVector<float> robangle,QVector<float> robcurrent);
    void recordposinfor(QVector<float> newvalue);
    void recordcurinfor(QVector<float> newvalue);
private slots:
    void analysismessage();
    void recvtcpmessage(QByteArray bytes);
    void resetreadflag();
    void recordstart();
    void recordstop();
private:
    FRAME frame;
    QVector<float> robcurrent;
    QVector<int> jointflag;
    QVector<int> curflag;
    QVector<QVector<float> > currwaypoint;
    JOINT_ANGLE jointangle;
    CURRENT_VALUE currentvalue;
    int curindex;
    QByteArray framebyte;
    QReadWriteLock lock;
    int ro_spd;//回转平台转速
    int fp_pos;//翻转平台位置
public:
    QVector<float> robangle;
    int rottimelength;
    bool ispro;
    bool isneedset;
    int framenum;
    QVector<QPair<float,float> > poslimit;
    bool warnsend;
};
//}
#endif // MSGSERVER_H
