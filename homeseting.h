#ifndef HOMESETING_H
#define HOMESETING_H

#include <QWidget>
#include "kinematics.h"
#include "globalvar.h"

namespace Ui {
class Homeseting;
}

class Homeseting : public QWidget
{
    Q_OBJECT

public:
    explicit Homeseting(QWidget *parent = nullptr);
    ~Homeseting();

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_ok_clicked();

private:
    void getspacepos();
    void getjointpos();

private:
    Ui::Homeseting *ui;
    int currenttype;
};

#endif // HOMESETING_H
