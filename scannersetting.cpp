#include "scannersetting.h"
#include "ui_scannersetting.h"

scannersetting *scannersetting::self=0;
scannersetting::scannersetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scannersetting)
{
    ui->setupUi(this);
    occview=new myview(this);
    QVBoxLayout *layout=new QVBoxLayout();
    layout->addWidget(occview);
    ui->frame_3d->setLayout(layout);
    int h=ui->progressBar->height();
    ui->pushButton_r->setFixedSize(h,h);
    ui->pushButton_add->setFixedSize(h,h);


    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setAcceptDrops(false);
    setAcceptDrops(true);

    QStringList header;
    header<<tr("选择")<<tr("探头名称")<<tr("参数")<<tr("模型")<<tr("删除");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setHighlightSections(true);
    ui->tableWidget->setColumnWidth(0,40);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,160);
    ui->tableWidget->setColumnWidth(4,50);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(46);
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setPointSize(16);
    font.setBold(false);
    ui->tableWidget->horizontalHeader()->setFont(font);
    ui->tableWidget->verticalHeader()->setVisible(false);
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(itemslot(int,int)));
    checkedicon=new QIcon(":/image/checked.png");

    uncheckedicon=new QIcon(":/image/unchecked.png");
    uncheckedicon->setThemeName("unchecked");
    reader=new OccStlReader();
    qRegisterMetaType<TopoDS_Shape> ("TopoDS_Shape");
    connect(this,SIGNAL(getstlmodelformstring(QString)),reader,SLOT(readashape(QString)));
    connect(reader,SIGNAL(updatamape(QString,TopoDS_Shape)),this,SLOT(shapemodelready(QString,TopoDS_Shape)));
}

scannersetting::~scannersetting()
{
    delete ui;
    delete reader;
}

void scannersetting::setparamets(QStringList list)
{
    QTableWidgetItem *item0 = new QTableWidgetItem();
    QTableWidgetItem *item1 = new QTableWidgetItem();
    QTableWidgetItem *item2 = new QTableWidgetItem();
    QTableWidgetItem *item3 = new QTableWidgetItem();
    QTableWidgetItem *item4 = new QTableWidgetItem();
    int t=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(t);

    item0->setIcon(*uncheckedicon);
    item1->setText(list.at(0));
    item2->setText(QString("长度：%1,扫描宽度：%2,距离：%3").arg(list.at(1)).arg(list.at(2)).arg(list.at(3)));
    item3->setText(list.at(4));
    item4->setText(QString("删除"));

    ui->tableWidget->setItem(t,0,item0);
    ui->tableWidget->setItem(t,1,item1);
    ui->tableWidget->setItem(t,2,item2);
    ui->tableWidget->setItem(t,3,item3);
    ui->tableWidget->setItem(t,4,item4);
    ui->tableWidget->show();
    QString str=list.at(4);
    if(isstlpath(str))
        emit getstlmodelformstring(str);
    islocked=true;

}

void scannersetting::itemslot(int row, int col)
{
    if(islocked)
        return;
    if(col==0)
    {
        for(int i=0;i<ui->tableWidget->rowCount();++i)
        {
            QTableWidgetItem *item=ui->tableWidget->item(i,col);
            if(i==row)
            {
                item->setIcon(*checkedicon);
                item->setSelected(true);
                showmodel();
            }
            else
                item->setIcon(*uncheckedicon);
            //qDebug()<<item->isSelected();
        }
    }
    if(col==4)
    {
        QString str=ui->tableWidget->item(row,3)->text();
        shapemodel.remove(str);
        ui->tableWidget->removeRow(row);
    }

}

void scannersetting::showmodel()
{
    int row=getselectedrow();
    if(row<0) return;
    QString str=ui->tableWidget->item(row,3)->text();
    if(!str.isEmpty())
    {
        if(isstlpath(str))
        {
            TopoDS_Shape _shape=shapemodel[str];
            occview->reset();
            occview->addshapetocontent(_shape);
        }
    }
    isUItrasonicsetted=true;
}

void scannersetting::shapemodelready(QString str, TopoDS_Shape sp)
{
    shapemodel[str]=sp;
    islocked=false;
}

int scannersetting::getselectedrow()
{
    int row=-1;
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        QTableWidgetItem *item=ui->tableWidget->item(i,0);
        if(item->isSelected())
        {
            row=i;
            break;
        }
    }
    return row;
}

bool scannersetting::isstlpath(QString str)
{
    if(str.isEmpty()) return false;
    QFileInfo url(str);
    if(!url.isFile())
    {
        QMessageBox::warning(this,"warning",QString("路径：%1 错误，找不到文件").arg(url.path()));
        return false;
    }
    return true;
}

void scannersetting::on_pushButton_r_pressed()
{
    if(!ui->checkBox->isChecked()) return;
    int vl=ui->progressBar->value();
    if(vl>0)
        ui->progressBar->setValue(vl-1);

}

void scannersetting::on_pushButton_add_pressed()
{
    if(!ui->checkBox->isChecked()) return;
    int vl=ui->progressBar->value();
    if(vl<100)
        ui->progressBar->setValue(vl+1);
}

void scannersetting::on_checkBox_clicked()
{
    if(!ui->checkBox->isChecked())
    {
        ui->progressBar->setValue(0);
    }
}

void scannersetting::on_progressBar_valueChanged(int value)
{

}

void scannersetting::on_toolButton_settings_clicked()
{
    UItrasonicsettings *st=new UItrasonicsettings();
    connect(st,SIGNAL(sendparamets(QStringList)),this,SLOT(setparamets(QStringList)));
    st->exec();
}
