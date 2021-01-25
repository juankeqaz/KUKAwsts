#include "occstlreader.h"
#include <QDebug>

OccStlReader::OccStlReader(QObject *parent):QObject(parent)
{
    thread = new QThread();
    moveToThread(thread);
    thread->start();

}

void OccStlReader::readashape(QString str)
{
    qDebug()<<"OccStlReader::readashape"<<str;
    QString str2=str;
    for(int i=0;i<str2.length();i++)
    {
        if(str2.at(i)=="/")
        {
            str2.replace(i,1,"\\");
        }
    }
    StlAPI_Reader *reader=new StlAPI_Reader();
    TopoDS_Shape _shape;
    reader->Read(_shape,str2.toStdString().c_str());
    //shapemap[str]=_shape;
    emit updatamape(str,_shape);
}
