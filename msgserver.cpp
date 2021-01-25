#include "msgserver.h"
#include <QDebug>

msgserver::msgserver(QObject *parent)
    :QObject(parent)
{
    jointflag={0,0,0};
    curflag={0,0,0};
    robangle={0,0,0};
    robcurrent=QVector<float> (3,0.0);
    curindex=0;
    framenum=0;
    framebyte.clear();
    ispro=false;
    qRegisterMetaType<QVariant>("QVariant");
    isneedset=true;

    frame.headvalid=false;
    frame.datalengthvalid=false;
    frame.datavalid=false;
    frame.datalength=0;
    frame.tailvalid=false;
    ro_spd=0;
}
msgserver::~msgserver()
{
}
void msgserver::analysismessage()
{
    //frame=framevar.value<FRAME>();
    int actioncode,mark,flag=-1;
    qDebug()<<"current frame length is"<<frame.datalength;
    switch (frame.datalength) {
    case 2:
    {
        mark=frame.data.at(0);
        actioncode=frame.data.at(1);
        //qDebug()<<"mark is "<<mark<<actioncode;
        switch (mark) {
        case 0:
            flag=actioncode==0? 0:1;
            break;
        case 1:
            flag=actioncode==0? 10:11;
            if(flag==10)
            {
                isneedset=true;
                Q_EMIT setmaxcount(1);
                //guisendvaluetimer->start(100);
            }
            break;
        case 2:
            flag=actioncode==0? 20:21;
            if(flag==20)
                Q_EMIT setmaxcount(10);
            break;
        case 3:
            flag=actioncode==0? 30:31;
            break;
        case 4:
        {
            qDebug()<<actioncode<<ispro;

            if(actioncode==0)
            {
                //if(ispro)
                    //sysfunction->rottimerstart();
                //else
                 //   flag=40;
            }
            if(actioncode==1)
            {
                ispro=false;
                flag=41;
            }
            if(actioncode==2)
            {
                ispro=false;
                flag=42;
            }
            if(actioncode==3)
            {
                ispro=false;
                flag=43;
            }
        }
            break;
        case 5:
            flag=actioncode==0? 50:51;
            break;
        case 6:
            flag=actioncode==0? 60:61;
            break;
        case 7:
            flag=actioncode==0? 70:71;
            break;
        case 8:
        {
            flag=actioncode==0? 80:(actioncode==1? 81:83);
//            if(flag==83 && actuastatus==data_collect_model)
//            {
//                Q_EMIT motionfinishedsignal();
//            }
        }
            break;
        case 9:
            flag=actioncode==0? 90:91;
            break;
        case 10:
        {
            if(actioncode==0)
            {
                flag=100;
                //curspeed=tagetspeed;
            }
            else
                flag=101;
        }
            break;
        case 11:
            flag=actioncode==0? 110:111;
            //if(flag==110)
            //sysfunction->messagetolower(37);
            break;
        case 12:
            flag=actioncode==0? 120:121;
            break;
        case 13:
            flag=actioncode==0? 130:131;
            break;
        case 14:
            flag=actioncode==0? 140:141;
            break;
        case 15:
            flag=actioncode==0? 150:151;
            break;
        case 16:
            flag=actioncode==0? 160:161;
            break;
        case 17:
            flag=actioncode==0? 170:171;
            if(flag==170)
            {
                isneedset=false;
                //guisendvaluetimer->stop();
            }
            break;
        case 18:
            if(actioncode==0) flag=180;
            if(actioncode==1) flag=181;
            if(actioncode==2) flag=182;
            if(actioncode==3) flag=183;
            if(actioncode==4) flag=184;
            break;
        case 19:
            if(actioncode==0) flag=190;
            if(actioncode==1) flag=191;
            if(actioncode==2)
            {
                flag=192;
//                if(actuastatus==data_collect_model)
//                Q_EMIT motionfinishedsignal();
            }
            break;
        case 20:
            flag=actioncode==0? 200:201;
            break;
        case 21:
            flag=actioncode==0? 210:211;
            break;
        case 22:
            flag=actioncode==0? 220:(actioncode==1? 221:(actioncode==2? 222:223));
            break;
        default:
            break;
        }
        qDebug()<<"recvd form robot"<<QString::number(frame.head,16)<<QString::number(frame.datalength,16)\
               <<QString::number(frame.data.at(0),16)<<QString::number(frame.data.at(1),16)<<QString::number(frame.tail,16);
        if(flag>=0)
        {
            Q_EMIT lowerrespond(flag);
        }
    }
        break;
    case 5:
    {
        int t=frame.data.at(2);
        currentvalue.short_byte.low_byte=frame.data.at(3);
        currentvalue.short_byte.high_byte=frame.data.at(4);
        float vl=(float)currentvalue.value;
        robcurrent[t]=(float)((vl/1000.00));
        //qDebug()<<robcurrent[t]<<"current value"<<currentvalue.value<<vl/1000.0;
        curflag[t]=1;
        if(curflag.at(0)*curflag.at(1)*curflag.at(2)*curflag.at(3)*curflag.at(4)*curflag.at(5)==1)
        {
            //sendjointcurrentvalue();
            curflag={0,0,0,0,0,0};
        }
    }
        break;
    case 7:
    {
        int t=frame.data.at(2);
        jointangle.float_byte.low_byte=frame.data.at(3);
        jointangle.float_byte.mlow_byte=frame.data.at(4);
        jointangle.float_byte.mhigh_byte=frame.data.at(5);
        jointangle.float_byte.high_byte=frame.data.at(6);
        if(isneedset)
        {
            switch (t) {
            case 0:
                pos_offset[t] = jointangle.value>=POS_NUM? 0:-360;
                break;
            case 1:
                pos_offset[t] = jointangle.value>=POS_NUM? 90:-270;
                break;
            case 2:
                pos_offset[t] = jointangle.value>=POS_NUM? 0:-360;
                break;
            case 3:
                pos_offset[t] = jointangle.value>=POS_NUM? 90:-270;
                break;
            case 4:
                pos_offset[t] = jointangle.value>=POS_NUM? 0:-360;
                break;
            case 5:
                pos_offset[t] = jointangle.value>=POS_NUM? 0:-360;
                break;
            default:
                break;
            }
        }
        //qDebug()<<"jointangle.value="<<jointangle.value;
        if(t<3)
            robangle[t]=jointangle.value/POS_NUM2+pos_offset.at(t);
        else
            robangle[t]=jointangle.value/POS_NUM3+pos_offset.at(t);
        jointflag[t]=1;
        if(jointflag.at(0)*jointflag.at(1)*jointflag.at(2)*jointflag.at(3)*jointflag.at(4)*jointflag.at(5)==1)
        {
            //sendjointanglevalue();
            //Q_EMIT jointnaglevaluesignal(robangle);
            jointflag={0,0,0,0,0,0};
            isneedset=false;
        }
    }
    case 25:
    {
        frame.data.removeFirst();
        int p=frame.data.size();
        int t=0;

        if(p==24)
        {
            for(int i=0;i<p;)
            {
                t=t%4;
                if(i<p-8)
                {
                    jointangle.float_byte.low_byte=frame.data.at(i);
                    jointangle.float_byte.mlow_byte=frame.data.at(i+1);
                    jointangle.float_byte.mhigh_byte=frame.data.at(i+2);
                    jointangle.float_byte.high_byte=frame.data.at(i+3);
                    if(isneedset)
                    {
                        switch (t) {
                        case 0:
                            pos_offset[t] =2221.4981; //jointangle.value<POS_NUM? 0:-360;
                            break;
                        case 1:
                            pos_offset[t] =-2181.440982668;// jointangle.value<POS_NUM? 90:-270;
                            break;
                        case 2:
                            pos_offset[t] =-89.99596; //jointangle.value<POS_NUM? 0:-360;
                            break;
                        case 3:
                            break;
                        default:
                            break;
                        }
                    }
                    qDebug()<<"jointangle.value="<<jointangle.value;
                    if(t==0)
                        robangle[t]=(float)jointangle.value/POS_NUM1-pos_offset.at(t);
                    else if(t==1)
                        robangle[t]=(float)jointangle.value/POS_NUM2-pos_offset.at(t);
                    else if(t==2)
                        robangle[t]=(float)jointangle.value/POS_NUM3-pos_offset.at(t);
                    else
                        fp_pos=(float)jointangle.value/POS_NUM2+2181.440982668;//翻转平台角度
                    //qDebug()<<"curpos is"<<t<<robangle[t];
                    if(t==2)
                    {
                        //qDebug()<<"start robstartus";
                        lock.lockForWrite();
                        //                        robstatus->position.clear();
                        //                        robstatus->spaceposition.clear();
                        for(int i=0;i<robangle.size();++i)
                        {
                            //robstatus->position.push_back(robangle.at(i));
                            //position[i]=robangle.at(i);
                        }
                        //platformangle=fp_pos;
                        //calculater->fKCal(robangle,robstatus->spaceposition);
                        lock.unlock();
                        //if(isrecordstart)
                        Q_EMIT recordposinfor(robangle);
                    }
                    i+=4;
                }else
                {
                    currentvalue.short_byte.low_byte=frame.data.at(i);
                    currentvalue.short_byte.high_byte=frame.data.at(i+1);
                    float vl=(float)currentvalue.value;
                    if(t<3)
                    {
                        robcurrent[t]=(float)((vl/1000.00));
                        qDebug()<<"curcurrent is"<<t<<robcurrent[t];
                    }
                    else
                    {
                        ro_spd=vl*60;//回转平台转速
                        qDebug()<<"ro_spd is"<<ro_spd;
                    }
                    if(t>2)
                    {
                        //sendjointcurrentvalue();
//                        if(actuastatus==data_collect_model || iscollitionon)
//                        {
//                            Q_EMIT robotstatussignal(robangle,robcurrent);
//                        }
                        lock.lockForWrite();
                        //                        robstatus->current=robcurrent;
                        for(int i=0;i<robcurrent.size();++i)
                        {
                            //robstatus->current.replace(i,robcurrent.at(i));
                            //current[i]=robcurrent.at(i);
                        }
                        //platformspeed=ro_spd;
                        lock.unlock();
//                        if(isrecordstart)
//                        {
//                            //qDebug()<<"send current value";
//                            Q_EMIT recordcurinfor(robcurrent);
//                        }
                    }
                    i+=2;
                }
                t++;
            }

        }
        isneedset=false;
    }
        break;
    default:
        break;
    }
}
void msgserver::recvtcpmessage(QByteArray bytes)
{
    framebyte=bytes;
    int unit=sizeof(quint8);
    int num=framebyte.size();
    if(num <= 0)
        return;
    if(curindex>=num)
        curindex=0;
    while(num)
    {
        if(!frame.headvalid)
        {
            //qDebug()<<"head"<<curindex<<num<<framebyte.size();
            if(num<unit) break;
            else
            {
                frame.head=(qint8)framebyte.at(curindex);//真实使用，调试时需注释掉
                num-=unit;
                curindex+=unit;
                if(frame.head!=FRAMEHEAD)
                {
                    continue;
                }
                else
                {
                    frame.headvalid=true;
                }
            }
        }
        else if(!frame.datalengthvalid && frame.headvalid)
        {
            //qDebug()<<"length"<<curindex<<num<<framebyte.size();
            if(num<unit) break;
            else
            {
                frame.datalength=(quint8)framebyte.at(curindex);
                num-=unit;
                curindex+=unit;
                frame.datalengthvalid=true;
            }
        }
        else if(!frame.datavalid && frame.datalengthvalid)
        {
            //qDebug()<<"data"<<curindex<<num<<framebyte.size();
            if(num < unit*frame.datalength) break;
            else
            {
                frame.data.clear();
                for(int i=0;i<frame.datalength;++i)
                {
                    frame.data.push_back((quint8)framebyte.at(curindex));
                    curindex+=unit;
                    num-=unit;
                }
                //num-=sizeof(quint8)*frame.datalength;
                frame.datavalid=true;
            }
        }
        else if(!frame.tailvalid && frame.datavalid)
        {
            //qDebug()<<"tail"<<curindex<<num<<framebyte.size();
            if(num<unit) break;
            else
            {
                frame.tail=(qint8)framebyte.at(curindex);
                num-=unit;
                curindex+=unit;
                if(frame.tail==FRAMETAIL)
                {
                    frame.tailvalid=true;
                    //framenum++;
                    //qDebug()<<"has received frame num: "<<framenum;
                    analysismessage();
                    resetreadflag();
                    //break;
                }
                else
                {
                    resetreadflag();
                    continue;
                }

            }
        }
    }
    if(framebyte.size() == curindex*unit)
    {
        framebyte.clear();
        curindex=0;
    }else
    {
        framebyte.remove(0,curindex*unit);
        curindex=0;
    }
}

void msgserver::resetreadflag()
{
    frame.headvalid=false;
    frame.datalengthvalid=false;
    frame.datavalid=false;
    frame.datalength=0;
    frame.tailvalid=false;
}

void msgserver::recordstart()
{
    //isrecordstart=true;
}

void msgserver::recordstop()
{
    //isrecordstart=false;
}


