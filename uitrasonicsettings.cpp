#include "uitrasonicsettings.h"
#include "ui_uitrasonicsettings.h"

UItrasonicsettings::UItrasonicsettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UItrasonicsettings)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("探头参数设置"));
}

UItrasonicsettings::~UItrasonicsettings()
{
    delete ui;
}

void UItrasonicsettings::on_pushButton_ok_clicked()
{
    QStringList list;
    list.append(ui->lineEdit_name->text());
    list.append(ui->lineEdit_scannerlength->text());
    list.append(ui->lineEdit_scanwidth->text());
    list.append(ui->lineEdit_distans->text());
    list.append(ui->lineEdit_url->text());
    emit sendparamets(list);
    close();
}

void UItrasonicsettings::on_pushButton_model_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", qApp->applicationDirPath(), "模型文件(*.stl)");
    //qDebug()<<"filename is"<<fileName;
    if (!fileName.isEmpty()) {
        ui->lineEdit_url->setText(fileName);
    }
//        QFile file(fileName);
//        if (file.open(QFile::ReadOnly)) {
//            QString str = file.readAll();
}
