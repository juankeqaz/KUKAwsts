#ifndef UITRASONICSETTINGS_H
#define UITRASONICSETTINGS_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class UItrasonicsettings;
}

class UItrasonicsettings : public QDialog
{
    Q_OBJECT
signals:
    void sendparamets(QStringList list);
public:
    explicit UItrasonicsettings(QWidget *parent = nullptr);
    ~UItrasonicsettings();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_model_clicked();

private:
    Ui::UItrasonicsettings *ui;
};

#endif // UITRASONICSETTINGS_H
