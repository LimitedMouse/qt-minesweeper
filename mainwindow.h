#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<cmine.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Cmine*  cmine;
private:
    Ui::MainWindow *ui;
    int difficulty; // 1\2\3
private slots:
    void slot_lost();
    void slot_count();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};
#endif // MAINWINDOW_H
