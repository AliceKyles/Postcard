#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rows.h"
#include <QFile>
#include <QWidget>
#include <QtGui>
#include <cmath>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void read();
    Row<double>* ri;
    Knot<double>* ki;
    int r, k, c, ro1, s;
    bool a, b, m;
    double n, x1;
    double* del;
    double* u1;
    double* N1;
    double* sig1;
    double ux(int i, double x);
    double Nx(int i, double x);
    double sigx(int i, double x);
    ~MainWindow();
private slots:

    void Table();
    void Graph();
    void doSlot();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
