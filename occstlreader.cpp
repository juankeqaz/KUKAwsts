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

void OccStlReader::readstepshape(QString str)
{
    QVector<TopoDS_Shape> shapevec;
    STEPControl_Reader aReader_step;
    IFSelect_ReturnStatus status = aReader_step.ReadFile(str.toStdString().c_str());
    if(status!=IFSelect_RetDone)
    {
        qDebug()<<"add file failure";
        return;
    }

    aReader_step.WS()->MapReader()->SetTraceLevel(2);
    aReader_step.PrintCheckLoad(Standard_False,IFSelect_ItemsByEntity);
    aReader_step.PrintCheckTransfer(Standard_False,IFSelect_ItemsByEntity);
        //qDebug()<<aReader_step.NbRootsForTransfer()<<aReader_step.NbShapes();
    for(Standard_Integer i=1;i<=aReader_step.NbRootsForTransfer();i++)
        aReader_step.TransferRoot(i);
    for(Standard_Integer i=1;i<=aReader_step.NbShapes();i++)
    {
        TopoDS_Shape shape_step=aReader_step.Shape(i);
        shapevec.push_back(shape_step);
    }
    //qDebug()<<"OccStlReader::readstepshape"<<str<<shapevec.size();
    emit updatemapes(shapevec);

}
