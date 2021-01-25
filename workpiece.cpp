#include "workpiece.h"
#include "ui_workpiece.h"


WorkPiece* WorkPiece::self=0;

WorkPiece::WorkPiece(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkPiece)
{
    ui->setupUi(this);
    initform();
    init3dview();
    ispress=false;
    reader=new OccStlReader();

    qRegisterMetaType<TopoDS_Shape> ("TopoDS_Shape");
    connect(this,SIGNAL(getstlmodelformstring(QString)),reader,SLOT(readashape(QString)));
    connect(reader,SIGNAL(updatamape(QString,TopoDS_Shape)),this,SLOT(shapemodelready(QString,TopoDS_Shape)));
}

WorkPiece::~WorkPiece()
{
    delete ui;
}

void WorkPiece::initform()
{
    this->max=false;
    this->location=this->geometry();
    this->setProperty("form",true);
    this->setProperty("canmove",true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    IconHelper::Instance()->setIcon(ui->label_icon,QChar(0xF099),30);
    IconHelper::Instance()->setIcon(ui->pushButton_min,QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->pushButton_max,QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->pushButton_close,QChar(0xf00d));
    ui->widgetTitle->setProperty("form","title");
    ui->widgettop->setProperty("nav","top");
    ui->label_text->setText("工件设置");
    ui->label_text->setFont(QFont("Microsoft YaHei",20));
    this->setWindowTitle(ui->label_text->text());
    QSize iconsize(32,32);
    int icowidth=85;
    QList<QToolButton*> tbtns=ui->widgettop->findChildren<QToolButton* >();
    foreach(QToolButton *btn,tbtns)
    {
        btn->setIconSize(iconsize);
        btn->setMinimumWidth(icowidth);
    }
    QList<int> mainwidth;
    mainwidth<<400<<300;
    ui->splitter_mian->setSizes(mainwidth);
    QList<int> widths;
    widths<<400<<100;
    ui->splitter_infor->setSizes(widths);
    QList<int> heights;
    heights<<500<<100;
    ui->splitter_3d->setSizes(heights);
    ui->textEdit->setEnabled(false);
    items<<"长方体"<<"楔形体"<<"柱体"<<"圆管体"<<"扫掠面"<<"直线"<<"圆弧"<<"导入模型"<<"自定义";
    ui->listWidget->addItems(items);
    ui->widgetTitle->hide();
    ui->stackedWidget_parame->setCurrentIndex(0);

}

void WorkPiece::init3dview()
{
    m_view=new myview(this);
    QVBoxLayout *layout=new QVBoxLayout();
    layout->addWidget(m_view);
    ui->frame->setLayout(layout);
}

void WorkPiece::on_pushButton_min_clicked()
{
    showMinimized();
}

void WorkPiece::on_pushButton_max_clicked()
{
    if(max){
        this->setGeometry(location);
        this->setProperty("canmove",true);
    }else{
        location=this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        this->setProperty("canmove",false);
    }
    max=!max;
}

void WorkPiece::on_pushButton_close_clicked()
{
    close();
}

void WorkPiece::showparamerpage(QString text)
{

    ui->stackedWidget_parame->setCurrentIndex(items.indexOf(text));
    qDebug()<<text<<items.indexOf(text)<<ui->stackedWidget_parame->currentIndex();

}

void WorkPiece::on_listWidget_currentTextChanged(const QString &currentText)
{
    showparamerpage(currentText);
}

void WorkPiece::on_pushButton_box_add_clicked()
{
    QString dx=ui->lineEdit_box_dx->text();
    QString dy=ui->lineEdit_box_dy->text();
    QString dz=ui->lineEdit_box_dz->text();
    if(dx.isEmpty()||dy.isEmpty()||dz.isEmpty()) return;
    m_view->reset();
    ToolCoord *toolcd=new ToolCoord(m_view->m_get_context());
    if(toolcd->ToolCoordParame(0,0,0,1,0,0,0,1,0))
    {
        toolcd->maketoolxyz(200);
    }
    box bx(0,0,0,dx.toFloat(),dy.toFloat(),dz.toFloat());
    currentshape=bx.getshape();
    toolcd->addshapes(currentshape);
    m_view->fitAll();

    ui->textEdit->setText(QString("长方体：\n长度：%1\n宽度：%2\n高度：%3").arg(dx).arg(dy).arg(dz));
    currentshapeparame.clear();
    currentshapeparame<<dx.toFloat()<<dy.toFloat()<<dz.toFloat();
    QStringList list;
    list<<"box"<<dx<<dy<<dz;
    emit definedshape(currentshape);
}
void WorkPiece::on_pushButton_line_ok_clicked()
{
    QString startx=ui->lineEdit_line_start_x->text();
    QString starty=ui->lineEdit_line_start_y->text();
    QString startz=ui->lineEdit_line_start_x->text();
    QString endx=ui->lineEdit_line_end_x->text();
    QString endy=ui->lineEdit_line_end_y->text();
    QString endz=ui->lineEdit_line_end_z->text();
    QString normx=ui->lineEdit_line_norm_x->text();
    QString normy=ui->lineEdit_line_norm_y->text();
    QString normz=ui->lineEdit_line_norm_z->text();
    QString h=ui->lineEdit_line_length->text();
    if(startx.isEmpty()||starty.isEmpty()||startz.isEmpty()||endx.isEmpty()||endy.isEmpty()||endz.isEmpty()||normx.isEmpty()||normy.isEmpty()||normz.isEmpty()||h.isEmpty()) return;

    m_view->reset();
    ToolCoord *toolcd=new ToolCoord(m_view->m_get_context());
    if(toolcd->ToolCoordParame(0,0,0,1,0,0,0,1,0))
    {
        toolcd->maketoolxyz(200);
    }

    Line line(gp_Pnt(startx.toFloat(),starty.toFloat(),startz.toFloat()),gp_Vec(endx.toFloat(),endy.toFloat(),endz.toFloat()),h.toFloat());//,normx.toFloat(),normy.toFloat(),normz.toFloat(),h.toFloat());
    line.setdirection(gp_Vec(normx.toFloat(),normy.toFloat(),normz.toFloat()));
    currentshape=line.getshape();
    toolcd->addshapes(currentshape);
    m_view->fitAll();

    ui->textEdit->setText(QString("直线：\n长度：%1\n").arg(h));
    QStringList list;
    list<<"line"<<startx<<starty<<startz<<endx<<endy<<endz<<normx<<normy<<normz<<h;
    emit definedshape(currentshape);
}

void WorkPiece::on_pushButton_arc_ok_clicked()
{
    QString r=ui->lineEdit_arc_radius->text();
    QString angle=ui->lineEdit_arc_angle->text();
    if(r.isEmpty()||angle.isEmpty()) return;
    m_view->reset();
    ToolCoord *toolcd=new ToolCoord(m_view->m_get_context());
    if(toolcd->ToolCoordParame(0,0,0,1,0,0,0,1,0))
    {
        toolcd->maketoolxyz(200);
    }
    circular car(0,0,0,r.toFloat(),angle.toFloat()*M_PI/180);
    currentshape=car.getshape();
    toolcd->addshapes(currentshape);
    m_view->fitAll();

    ui->textEdit->setText(QString("圆弧：\n半径：%1\n圆心角：%2").arg(r).arg(angle));
    QStringList list;
    list<<"arc"<<r<<angle;
    emit definedshape(currentshape);
}

void WorkPiece::on_pushButton_wedge_ok_clicked()
{
    QString dx,dy,dz,xmin,zmin,xmax,zmax;
    dx=ui->lineEdit_wedge_dx->text();
    dy=ui->lineEdit_wedge_dy->text();
    dz=ui->lineEdit_wedge_dz->text();
    xmin=ui->lineEdit_wedge_xmin->text();
    zmin=ui->lineEdit_wedge_zmin->text();
    xmax=ui->lineEdit_wedge_xmax->text();
    zmax=ui->lineEdit_wedge_zmax->text();
    if(dx.isEmpty()||dy.isEmpty()||dz.isEmpty()||xmin.isEmpty()||xmax.isEmpty()||zmin.isEmpty()||zmax.isEmpty()) return;
    m_view->reset();
    ToolCoord *toolcd=new ToolCoord(m_view->m_get_context());
    if(toolcd->ToolCoordParame(0,0,0,1,0,0,0,1,0))
    {
        toolcd->maketoolxyz(200);
    }

    Wedge wd(dx.toFloat(),dy.toFloat(),dz.toFloat(),xmax.toFloat(),xmin.toFloat(),zmax.toFloat(),zmin.toFloat());
    currentshape=wd.getshape();
    toolcd->addshapes(currentshape);
    m_view->fitAll();

    ui->textEdit->setText(QString("楔形体：\n下底面：%1X%2\n楔形高度：%3\n上底面：%4X%5\n偏移：%6X%7").arg(dx).arg(dz).arg(dy).arg(xmax.toFloat()-xmin.toFloat()).arg(zmax.toFloat()-zmin.toFloat()).arg(zmin).arg(xmin));
    QStringList list;
    list<<"wedge"<<dx<<dy<<dz<<xmax<<xmin<<zmax<<zmin;
    emit definedshape(currentshape);
}

void WorkPiece::on_pushButton_cycler_ok_2_clicked()
{
    QString radius=ui->lineEdit_cycle_r_2->text();
    QString height=ui->lineEdit_cycle_h_2->text();
    QString angle=ui->lineEdit_cycle_angle->text();
    if(radius.isEmpty()||height.isEmpty()||angle.isEmpty()) return;
    m_view->reset();
    ToolCoord *toolcd=new ToolCoord(m_view->m_get_context());
    if(toolcd->ToolCoordParame(0,0,0,1,0,0,0,1,0))
    {
        toolcd->maketoolxyz(200);
    }
    cylinder cld(gp_Ax2(gp_Pnt(0,0,0),gp_Dir(0,0,1)),radius.toFloat(),height.toFloat(),angle.toFloat()*M_PI/180);
    currentshape=cld.getshape();
    toolcd->addshapes(currentshape);
    m_view->fitAll();
    ui->textEdit->clear();
    ui->textEdit->setText(QString("圆柱体：\n半径：%1\n高度：%2\n圆心角：%3").arg(radius).arg(height).arg(angle));
    QStringList list;
    list<<"cyclerfan"<<radius<<height<<angle;
    emit definedshape(currentshape);
}

void WorkPiece::on_pushButton_pipe_ok_2_clicked()
{
    QString r1=ui->lineEdit_pipe_r1_2->text();
    QString r2=ui->lineEdit_pipe_r2_2->text();
    QString h=ui->lineEdit_pip_h_2->text();
    QString angle=ui->lineEdit_pip_angle->text();
    if(r1.isEmpty()||r2.isEmpty()||h.isEmpty()||angle.isEmpty()) return;
    m_view->reset();
    ToolCoord *toolcd=new ToolCoord(m_view->m_get_context());
    if(toolcd->ToolCoordParame(0,0,0,1,0,0,0,1,0))
    {
        toolcd->maketoolxyz(200);
    }
    Pipes pipe(gp_Ax2(gp_Pnt(0,0,0),gp_Dir(0,0,1)),r1.toFloat(),r2.toFloat(),h.toFloat(),angle.toFloat()*M_PI/180);
    currentshape=pipe.getshape();
    toolcd->addshapes(currentshape);
    m_view->fitAll();

    ui->textEdit->clear();
    ui->textEdit->setText(QString("圆管体：\n外径：%1\n内径：%2\n高度：%3\n圆心角：%4").arg(r1).arg(r2).arg(h).arg(angle));
    QStringList list;
    list<<"pipefan"<<r1<<r2<<h<<angle;
    emit definedshape(currentshape);
}

void WorkPiece::on_pushButton_point_add_clicked()
{
    QString type,x,y,z;
    bool isline=ui->radioButton_line->isChecked();
    if(isline)
        type="line";
    else
        type="arc";
    x=ui->lineEdit_point_x->text();
    y=ui->lineEdit_point_y->text();
    z=ui->lineEdit_point_z->text();
    if(x.isEmpty()||y.isEmpty()||z.isEmpty()) return;
    subwire wire;
    if(sweepparame.size()==0)
    {
        wire.type=type;
        wire.vertex.push_back(gp_Pnt(x.toFloat(),y.toFloat(),z.toFloat()));
        sweepparame.push_back(wire);
    }else
    {
        if(!sweepparame.last().getvertex(gp_Pnt(x.toFloat(),y.toFloat(),z.toFloat())))
        {
            wire=sweepparame.last();
            wire.type=type;
            wire.vertex.clear();
            wire.vertex.push_back(sweepparame.last().vertex.last());
            wire.vertex.push_back(gp_Pnt(x.toFloat(),y.toFloat(),z.toFloat()));
            sweepparame.push_back(wire);
        }
    }


    QString str="扫掠体边线：\n";
    for(int i=0;i<sweepparame.size();++i)
    {
        str.append(sweepparame.at(i).type).append(":\n");
        QVector<gp_Pnt> points=sweepparame.at(i).vertex;
        for(int p=0;p<points.size();p++)
        {
            str.append(QString::number(points.at(p).X())).append(",");
            str.append(QString::number(points.at(p).Y())).append(",");
            str.append(QString::number(points.at(p).Z())).append(" ");
        }
    }
    ui->textEdit->clear();
    ui->textEdit->setText(str);
}

void WorkPiece::on_pushButton_sweeps_ok_clicked()
{
    QString wx,wy,wz,length;
    wx=ui->lineEdit_width_x->text();
    wy=ui->lineEdit_width_y->text();
    wz=ui->lineEdit_width_z->text();
    length=ui->lineEdit_width_length->text();
    if(wx.isEmpty()||wy.isEmpty()||wz.isEmpty()||length.isEmpty()) return;
    m_view->reset();
    ToolCoord *toolcd=new ToolCoord(m_view->m_get_context());
    if(toolcd->ToolCoordParame(0,0,0,1,0,0,0,1,0))
    {
        toolcd->maketoolxyz(200);
    }
    Sweeps sweep(sweepparame);
    sweep.make_sweeps(gp_Dir(wx.toFloat(),wy.toFloat(),wz.toFloat()),length.toFloat());
    currentshape=sweep.getshape();
    toolcd->addshapes(currentshape);
    m_view->fitAll();


    QString str="扫掠面：\n边线：";
    for(int i=0;i<sweepparame.size();++i)
    {
        str.append(sweepparame.at(i).type).append(":\n");
        QVector<gp_Pnt> points=sweepparame.at(i).vertex;
        for(int p=0;p<points.size();p++)
        {
            str.append(QString::number(points.at(p).X())).append(",");
            str.append(QString::number(points.at(p).Y())).append(",");
            str.append(QString::number(points.at(p).Z())).append(" ");
        }
        //str.append("\n");
    }
    str.append(QString("宽度：\n")+wx+","+wy+","+wz+"\n");
    ui->textEdit->clear();
    ui->textEdit->setText(str);
    emit definedshape(currentshape);
}

void WorkPiece::on_pushButton_point_clear_clicked()
{
    sweepparame.clear();
}

void WorkPiece::on_pushButton_clicked()
{
    m_view->reset();
    ToolCoord *toolcd=new ToolCoord(m_view->m_get_context());
    if(toolcd->ToolCoordParame(0,0,0,1,0,0,0,1,0))
    {
        toolcd->maketoolxyz(200);
    }
    m_view->showgrounds(true);

    ToolCoord *subtoolcd=new ToolCoord(m_view->m_get_context());
    if(subtoolcd->ToolCoordParame(0,0,0,1,0,0,0,1,0))
    {
        toolcd->maketoolxyz(50);
    }
}

void WorkPiece::mousePressEvent(QMouseEvent *event)
{
    if(max)
    {
        return;
    }else
    {
        ispress=true;
        startpos=event->globalPos();
    }
    return QWidget::mousePressEvent(event);

}

void WorkPiece::mouseMoveEvent(QMouseEvent *event)
{
    if(ispress)
    {
        QPoint movepoint=event->globalPos()-startpos;
        QPoint wdpos=this->pos();
        startpos=event->globalPos();
        //this->move(wdpos.x()+movepoint.x(),wdpos.y()+movepoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void WorkPiece::mouseReleaseEvent(QMouseEvent *event)
{
    ispress=false;
    return QWidget::mouseReleaseEvent(event);
}
void WorkPiece::on_stackedWidget_parame_currentChanged(int arg1)
{
    qDebug()<<"stackedWidget_currentChanged"<<arg1;
    currentshapetype=arg1;
}

void WorkPiece::on_pushButton_input_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", qApp->applicationDirPath(), "模型文件(*.stl)");
    if (!fileName.isEmpty()) {
        ui->textEdit_path->setText(fileName);
    }
    emit getstlmodelformstring(fileName);
}



void WorkPiece::on_pushButton_newdir_clicked()
{

}

void WorkPiece::shapemodelready(QString filename, TopoDS_Shape shape)
{
    qDebug()<<ui->textEdit_path->document()->toPlainText()<<filename<<(ui->textEdit_path->document()->toPlainText()==filename);
    if(ui->textEdit_path->document()->toPlainText()==filename)
    {
        currentshape=shape;
        m_view->reset();
        m_view->addshapetocontent(shape);
        emit definedshape(currentshape);
    }else
        return;
}
