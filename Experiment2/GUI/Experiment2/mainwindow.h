// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "mcsolver.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_calculateButton_clicked();

private:
    QString format(const state&, const int&);
    QString formatBoat(const state&, const state&, const int&);
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
