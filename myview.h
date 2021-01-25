#ifndef MYVIEW_H
#define MYVIEW_H
#include <QGLWidget>
#include "occ.h"
#include <QDesktopWidget>
#include <QApplication>

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QRubberBand>
#include <QUrl>


class myview:public QGLWidget
{
    Q_OBJECT
public:
    myview(QWidget *parent=0);
    ~myview();
    Handle(AIS_InteractiveContext) m_get_context(){return m_context;}
public slots:
    void showgrounds(bool isshow);
    void addshapetocontent(const TopoDS_Shape &shape);
    void addshapetocontent(const TopoDS_Shape &shape,int index);
    void reset();
    void fitAll();
    void removeshape();
    TopoDS_Shape getshape()
    {
        return _currentshape->Shape();
    }
    int selectshapeposition();
    void setstlmodelformurl(QString url);
protected:
    void init();
    void paintEvent(QPaintEvent * theEvent);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    //!覆写鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //!覆写鼠标滚轮事件
    void wheelEvent(QWheelEvent *event);
    void drawRubberBand( const int minX, const int minY, const int maxX, const int maxY );

signals:
    void selectionChanged();


protected:
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicPanning, //平移
        CurAction3d_DynamicZooming, //缩放
        CurAction3d_DynamicRotation //旋转
    };
private:
    //!交互式上下文能够管理一个或多个查看器(viewer)中的图形行为和交互式对象的选择
    Handle(AIS_InteractiveContext) m_context=NULL;
    //!定义查看器(viewer)类型对象上的服务
    Handle(V3d_Viewer) m_viewer=NULL;
    //!创建一个视图
    Handle(V3d_View) m_view=NULL;
    //!创建3d接口定义图形驱动程序
    Handle(Graphic3d_GraphicDriver) m_graphic_driver=NULL;


    Standard_Integer m_x_max;    //!记录鼠标平移坐标X
    Standard_Integer m_y_max;    //!记录鼠标平移坐标Y
    Standard_Integer m_x_min;    //!记录鼠标平移坐标X
    Standard_Integer m_y_min;    //!记录鼠标平移坐标Y
    CurrentAction3d m_current_mode; //!三维场景转换模式
    QRubberBand *myRectBand;
    Handle(AIS_Shape) _currentshape=NULL;
    QVector<Handle(AIS_Shape)> shapevec;
    QString currentpath;

};

#endif // MYVIEW_H
