#ifndef OCCSTLREADER_H
#define OCCSTLREADER_H
#include "occ.h"
#include <StlAPI_Reader.hxx>
#include <QObject>
#include <QThread>
#include <QMap>

class OccStlReader : public QObject
{
    Q_OBJECT
public:
    OccStlReader(QObject *parent=nullptr);
    //osg::Node *readSTLFile();
    QMap<QString,TopoDS_Shape> getshapemap()
    {
        return  shapemap;
    }
signals:
    void updatamape(QString str,TopoDS_Shape shape);
private slots:
    void readashape(QString str);
private:
    TopoDS_Shape *_shape=NULL;
    StlAPI_Reader *reader;
    QMap<QString,TopoDS_Shape> shapemap;
    QThread *thread;

};

#endif // OCCSTLREADER_H
