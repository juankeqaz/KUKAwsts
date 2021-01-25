#include "myview.h"
#include <OpenGl_GraphicDriver.hxx>

#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>
#include <QDebug>

#ifdef WNT
#include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
#include <Cocoa_Window.hxx>
#else
#undef Bool
#undef CursorShape
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose
#include <Xw_Window.hxx>
#endif


static Handle(Graphic3d_GraphicDriver)& GetGraphicDriver()
{
  static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
  return aGraphicDriver;

}


myview::myview(QWidget *parent):QGLWidget(parent),myRectBand(NULL)
{
  setBackgroundRole( QPalette::NoRole );
  setMouseTracking( true );
}

myview::~myview()
{
  if(m_view)
    m_view->Remove();

}

void myview::showgrounds(bool isshow)
{
  if(isshow)
    {
      m_viewer->SetRectangularGridValues(0,0,20,20,0);
      m_viewer->SetRectangularGridGraphicValues(1000.01,1000.01,0);
      m_viewer->ActivateGrid(Aspect_GT_Rectangular,Aspect_GDM_Lines);
    }
  else
    {
      m_viewer->DeactivateGrid();
    }
  m_view->Redraw();
}

void myview::addshapetocontent(const TopoDS_Shape &shape)
{
  //Handle(AIS_InteractiveContext) m_context2= new AIS_InteractiveContext(m_viewer);
  Handle(AIS_Shape) anAisRevolVertex = new AIS_Shape(shape);
  _currentshape=anAisRevolVertex;
  m_context->Display(anAisRevolVertex, Standard_True);
  m_view->FitAll();

}

void myview::addshapetocontent(const TopoDS_Shape &shape, int index)
{
    Handle(AIS_Shape) anAisRevolVertex = new AIS_Shape(shape);
    shapevec.push_back(anAisRevolVertex);
    _currentshape=anAisRevolVertex;
    switch (index) {
        case 0:
          m_context->SetColor(anAisRevolVertex,Quantity_NOC_RED1,Standard_False);
        break;
    case 1:
        m_context->SetColor(anAisRevolVertex,Quantity_NOC_GREEN1,Standard_False);
        break;
    case 2:
        m_context->SetColor(anAisRevolVertex,Quantity_NOC_BLUE1,Standard_False);
        break;
    default:
        m_context->SetColor(anAisRevolVertex,Quantity_NOC_GRAY,Standard_False);
        break;
    }
    m_context->Display(anAisRevolVertex, 1,0,Standard_True,Standard_True,AIS_DS_Displayed);
    m_view->FitAll();
}

void myview::reset()
{
    //m_context->EraseAll(true);
    m_context->RemoveAll(true);
    shapevec.clear();
}

void myview::init()
{
  if (m_context.IsNull())
    {
      //此对象提供与X server的连接，在Windows和Mac OS中不起作用
      Handle(Aspect_DisplayConnection) m_display_donnection = new Aspect_DisplayConnection();
      //创建OpenGl图形驱动
      if (m_graphic_driver.IsNull())
        {
          m_graphic_driver = new OpenGl_GraphicDriver(m_display_donnection);
        }
      //获取QWidget的窗口系统标识符
      WId window_handle = (WId) winId();
#ifdef _WIN32
      // 创建Windows NT 窗口
      Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
#else
      // 创建XLib window 窗口
      Handle(Xw_Window) wind = new Xw_Window(m_display_donnection, (Window) window_handle);
#endif
      //创建3D查看器
      m_viewer = new V3d_Viewer(m_graphic_driver,Standard_ExtString("viewer3d"));
      //创建视图
      m_view = m_viewer->CreateView();
      m_view->SetWindow(wind);
      //打开窗口
      if (!wind->IsMapped())
        {
          wind->Map();
        }
      m_context = new AIS_InteractiveContext(m_viewer);  //创建交互式上下文
      //配置查看器的光照
      m_viewer->SetDefaultLights();
      m_viewer->SetLightOn();
      //设置视图的背景颜色为灰色
      m_view->SetBackgroundColor(Quantity_NOC_GRAY60);
      m_view->MustBeResized();
      //显示直角坐标系，可以配置在窗口显示位置、文字颜色、大小、样式
      m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
      //设置显示模式
      m_context->SetDisplayMode(AIS_Shaded, Standard_True);

      // 设置模型高亮的风格
      Handle(Prs3d_Drawer) t_hilight_style = m_context->HighlightStyle(); // 获取高亮风格
      t_hilight_style->SetMethod(Aspect_TOHM_COLOR);  // 颜色显示方式
      t_hilight_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);    // 设置高亮颜色
      t_hilight_style->SetDisplayMode(1); // 整体高亮
      t_hilight_style->SetTransparency(0.2f); // 设置透明度

      // 设置选择模型的风格
      Handle(Prs3d_Drawer) t_select_style = m_context->SelectionStyle();  // 获取选择风格
      t_select_style->SetMethod(Aspect_TOHM_COLOR);  // 颜色显示方式
      t_select_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);   // 设置选择后颜色
      t_select_style->SetDisplayMode(1); // 整体高亮
      t_select_style->SetTransparency(0.4f); // 设置透明度
      t_select_style->SetAutoTriangulation(true);

      m_view->SetZoom(100);   // 放大

      fitAll();
    }
}

void myview::paintEvent(QPaintEvent * /*theEvent*/)
{
  if (m_context.IsNull()) // 若未定义交互环境
    {
      init(); // 初始化交互环境
    }
  m_view->Redraw();
}

void myview::resizeEvent(QResizeEvent *)
{
  if( !m_view.IsNull() )
    {
      m_view->MustBeResized();
    }
}

void myview::mousePressEvent(QMouseEvent *event)
{
  if(event->buttons()&Qt::RightButton)
    {
      // 鼠标左右键齐按：初始化平移
      m_x_max=event->x();
      m_y_max=event->y();
    }
  else if(event->buttons()&Qt::LeftButton)
    {
      m_x_min=event->x();
      m_y_min=event->y();
      m_view->StartRotation(event->x(),event->y());

      AIS_StatusOfPick t_pick_status=AIS_SOP_NothingSelected;
//      if(qApp->keyboardModifiers()==Qt::ControlModifier)
//      {
//          t_pick_status= m_context->ShiftSelect(true);
//      }
//      else {
          t_pick_status=m_context->Select(true);
//      }
    }
  else if(event->buttons()&Qt::MidButton)
    {
      // 鼠标滚轮键：初始化平移
      m_x_max=event->x();
      m_y_max=event->y();
      // 鼠标滚轮键：初始化旋转
      m_view->StartRotation(event->x(),event->y());
    }
}

void myview::mouseReleaseEvent(QMouseEvent *event)
{
  if(myRectBand)
    myRectBand->hide();
  if(event->pos().x()==m_x_min &&event->pos().y()==m_y_min)
    {
      if(event->buttons()& Qt::ControlModifier)
        {
          m_context->ShiftSelect(Standard_True);
          emit selectionChanged();
        }
      else
        {
          m_context->Select(Standard_True);
          emit selectionChanged();
        }
    }

    m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
}

void myview::mouseMoveEvent(QMouseEvent *event)
{
  if(event->buttons()&Qt::RightButton)
    {
      // 鼠标左右键齐按：执行平移
      m_view->Pan(event->pos().x()-m_x_max,m_y_max-event->pos().y());
      m_x_max=event->x();
      m_y_max=event->y();
    }
  else if(event->buttons()&Qt::MidButton)
    {
      // 鼠标滚轮键
      if(qApp->keyboardModifiers()==Qt::ShiftModifier)    // 且按下Shift键
        {
          // 鼠标滚轮键：执行平移
          m_view->Pan(event->pos().x()-m_x_max,m_y_max-event->pos().y());
          m_x_max=event->x();
          m_y_max=event->y();
        }
      else
        {
          // 鼠标滚轮键：执行旋转
          m_view->Rotation(event->x(),event->y());
        }
    }
  else if(event->buttons() & Qt::LeftButton)
    {
      //AIS_StatusOfPick t_pick_status=AIS_SOP_NothingSelected;
      // 将鼠标位置传递到交互环境
      //m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
      //if(qApp->keyboardModifiers()==Qt::ShiftModifier)    // 且按下Shift键
      //  {
      //    drawRubberBand(m_x_min,m_y_min,event->x(),event->y());
     //   }else if(qApp->keyboardModifiers()==Qt::ControlModifier)
     //   {
          //          if(qApp->keyboardModifiers()==Qt::ControlModifier)
          //            {
          //              t_pick_status= m_context->ShiftSelect(true);
          //            }
          //          else {
          //t_pick_status=m_context->Select(true);
          //            }

     //   }
     // else
        m_view->Rotation(event->x(),event->y());
    }
}

void myview::wheelEvent(QWheelEvent *event)
{
  m_view->StartZoomAtPoint(event->pos().x(),event->pos().y());
  m_view->ZoomAtPoint(0, 0, event->angleDelta().y(), 0); //执行缩放
}
void myview::drawRubberBand( const int minX, const int minY, const int maxX, const int maxY )
{
  QRect aRect;

  // Set the rectangle correctly.
  (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
  (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

  aRect.setWidth(abs(maxX - minX));
  aRect.setHeight(abs(maxY - minY));

  if (!myRectBand)
    {
      myRectBand = new QRubberBand(QRubberBand::Rectangle, this);

      // setStyle is important, set to windows style will just draw
      // rectangle frame, otherwise will draw a solid rectangle.
      myRectBand->setStyle(QStyleFactory::create("windows"));
    }

  myRectBand->setGeometry(aRect);
  myRectBand->show();
}

void myview::fitAll()
{
  m_view->FitAll();
  m_view->ZFitAll();
  m_view->Redraw();
}

void myview::removeshape()
{
    if(!_currentshape)
        return;
    else {
        m_context->Remove(_currentshape,true);
    }
}

int myview::selectshapeposition()
{
    gp_Pnt pt;
    for(int i=0;i<shapevec.size();++i)
    {
        m_context->InitCurrent();
        Handle(AIS_InteractiveObject) sp=m_context->SelectedInteractive();
        if(sp==shapevec.at(i))
            return i;
    }
    return  -1;
}

void myview::setstlmodelformurl(QString url)
{
    if(currentpath==url)
        return;
    else
        currentpath=url;


    for(int i=0;i<url.length();i++)
    {
        if(url.at(i)=="/")
        {
            url.replace(i,1,"\\");
        }
    }
    StlAPI_Reader *reader=new StlAPI_Reader();
    TopoDS_Shape _shape;
    reader->Read(_shape,url.toStdString().c_str());
    reset();
    addshapetocontent(_shape);
}
