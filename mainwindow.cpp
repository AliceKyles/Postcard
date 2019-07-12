#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    a=b=0;
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(Table()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(Graph()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(doSlot()));
}

double MainWindow::ux(int i, double x){
    double d1=ri[i].L;
    double d2=ri[i].q;
    double d3=ri[i].E*ri[i].A*2;
    double u=del[i]+x/d1*(del[i+1]-del[i])+d2*pow(d1,2)/d3*x/d1*(1-x/d1);
    return u;
}

double MainWindow::Nx(int i, double x){
    double d1=ri[i].E*ri[i].A;
    double d2=ri[i].q;
    double d3=ri[i].L;
    double N=d1/d3*(del[i+1]-del[i])+d2*d3/2*(1-2*x/d3);
    return N;
}

double MainWindow::sigx(int i, double x){
    double d1=ri[i].E*ri[i].A;
    double d2=ri[i].q;
    double d3=ri[i].L;
    double d4=ri[i].A;
    double sig=(d1/d3*(del[i+1]-del[i]))/d4+d2*d3/2*(1-2*x/d3)/d4;
    return sig;
}

void MainWindow::doSlot(){
    if (ui->pushButton_3->text()=="Which component?"){
        s=0;
        if (ui->lineEdit->text()=="u") s=1;
        if (ui->lineEdit->text()=="N") s=2;
        if (ui->lineEdit->text()=="Sig") s=3;
        if (s==1||s==2||s==3){
            b=1;
            update();
        }
    }
    if (ui->pushButton_3->text()=="Which row?"){
        int vall = ui->lineEdit->text().toInt();
        if (vall>0&&vall<=r){
            ro1=vall;
            ui->pushButton_3->setText("Which component?");
        }
    }
    if (ui->pushButton_3->text()=="Enter x"){
        double vall = ui->lineEdit->text().toDouble();
        if ((vall>0&&vall<=ri[ro1-1].L)||ui->lineEdit->text()=="0"){
            x1=vall;
            m=1;
            ui->pushButton_3->setText("Enter the row");
            update();
        }
    }
    if (ui->pushButton_3->text()=="Enter the row"){
        int vall = ui->lineEdit->text().toInt();
        if (vall>0&&vall<=r){
            ro1=vall;
            ui->pushButton_3->setText("Enter x");
        }
    }
    if (ui->pushButton_3->text()=="How many points?"){
        int vall = ui->lineEdit->text().toInt();
        if (vall>0){
            a=1;
            b=0;
            m=0;
            n=vall;
            update();
            ui->pushButton_3->setText("Enter the row");
        }
    }
}

void MainWindow::Table(){
    read();
    ui->pushButton_3->setText("How many points?");
}

void MainWindow::Graph(){
    read();
    ui->pushButton_3->setText("Which row?");
}

void MainWindow::read(){
    a=b=0;
    QFile file("D:/KM");
    if (!file.open(QIODevice::ReadOnly)){
        ui->pushButton->setText("Fail");
        ui->pushButton_2->setText("Fail");
        ui->pushButton_3->setText("Fail");
    }
    QDataStream out(&file);
    out>>r>>k;
    ri=new Row<double> [r];
    ki=new Knot<double> [k];
    for (int j=0; j<r; j++){
        out>>ri[j].L;
        out>>ri[j].A;
        out>>ri[j].E;
        out>>ri[j].q;
        out>>ri[j].Sig;
    }
    for (int j=0; j<k; j++){
        out>>ki[j].F;
        out>>ki[j].sup;
    }
    if (out.status()!=QDataStream::Ok){
        ui->pushButton->setText("Fail");
        ui->pushButton_2->setText("Fail");
        ui->pushButton_3->setText("Fail");
    }
    QFile file1("D:/KM1");
    if (!file1.open(QIODevice::ReadOnly)){
        ui->pushButton->setText("Fail");
        ui->pushButton_2->setText("Fail");
        ui->pushButton_3->setText("Fail");
    }
    QDataStream out1(&file1);
    out1>>r>>k;
    del=new double [k];
    u1=new double [r];
    N1=new double [r];
    sig1=new double [r];
    for (int j=0; j<r; j++){
        out1>>u1[j];
        out1>>N1[j];
        out1>>sig1[j];
    }
    for (int j=0; j<k; j++){
        out1>>del[j];
    }
    if (out.status()!=QDataStream::Ok){
        ui->pushButton->setText("Fail");
        ui->pushButton_2->setText("Fail");
        ui->pushButton_3->setText("Fail");
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    if (a==1){
        QRect rx1(QPoint(20, 20), QSize(70, 20));
        painter.drawRect(rx1);
        painter.drawText(25,35,"Row, x");
        QRect rx2(QPoint(90, 20), QSize(70, 20));
        painter.drawRect(rx2);
        painter.drawText(95,35,"ux");
        QRect rx3(QPoint(160, 20), QSize(70, 20));
        painter.drawRect(rx3);
        painter.drawText(165,35,"Nx");
        QRect rx4(QPoint(230, 20), QSize(70, 20));
        painter.drawRect(rx4);
        painter.drawText(235,35,"Sigx");
        QRect rx5(QPoint(300, 20), QSize(70, 20));
        painter.drawRect(rx5);
        painter.drawText(305,35,"|Sig|");
        c=0;
        for (int i=1; i<r+1; i++){
            double d1=ri[i-1].L;
            double d=d1/(n-1);
            double j=0;
            while ( j<=d1){
            if (sigx(i-1,j)>ri[i-1].Sig||sigx(i-1,j)<(ri[i-1].Sig*(-1)))
                painter.setBrush(Qt::red);
            else painter.setBrush(Qt::green);
            QRect rq(QPoint(20, 20+20*i+c*20), QSize(350, 20));
            painter.drawRect(rq);
            painter.setBrush(Qt::NoBrush);
            QRect r1(QPoint(20, 20+20*i+c*20), QSize(70, 20));
            painter.drawRect(r1);
            QString a1=QString::number(i);
            QString a2=", ";
            QString a3=QString::number(j);
            QString stra=a1+a2+a3;
            painter.drawText(25,35+20*i+20*c,stra);
            QRect r2(QPoint(90, 20+20*i+20*c), QSize(70, 20));
            painter.drawRect(r2);
            QString stra1=QString::number(ux(i-1,j));
            painter.drawText(95,35+20*i+20*c,stra1);
            QRect r3(QPoint(160, 20+20*i+20*c), QSize(70, 20));
            painter.drawRect(r3);
            QString stra2=QString::number(Nx(i-1,j));
            painter.drawText(165,35+20*i+20*c,stra2);
            QRect r4(QPoint(230, 20+20*i+20*c), QSize(70, 20));
            painter.drawRect(r4);
            QString stra3=QString::number(sigx(i-1,j));
            painter.drawText(235,35+20*i+20*c,stra3);
            QRect r5(QPoint(300, 20+20*i+20*c), QSize(70, 20));
            painter.drawRect(r5);
            QString stra4=QString::number(ri[i-1].Sig);
            painter.drawText(305,35+20*i+20*c,stra4);
            c++;
            if ((j<d1)&&((j+d)>d1))
                j=d1;
            else j+=d;
        }
        }
        if (m==1){
            if (sigx(ro1-1,x1)>ri[ro1-1].Sig||sigx(ro1-1,x1)<(ri[ro1-1].Sig*(-1)))
                painter.setBrush(Qt::red);
            else painter.setBrush(Qt::green);
            QRect rq(QPoint(20, 20+20*(r+1)+c*20), QSize(350, 20));
            painter.drawRect(rq);
            painter.setBrush(Qt::NoBrush);
            QRect r1(QPoint(20, 20+20*(r+1)+c*20), QSize(70, 20));
            painter.drawRect(r1);
            QString a1=QString::number(ro1);
            QString a2=", ";
            QString a3=QString::number(x1);
            QString stra=a1+a2+a3;
            painter.drawText(25,35+20*(r+1)+20*c,stra);
            QRect r2(QPoint(90, 20+20*(r+1)+20*c), QSize(70, 20));
            painter.drawRect(r2);
            QString stra1=QString::number(ux(ro1-1,x1));
            painter.drawText(95,35+20*(r+1)+20*c,stra1);
            QRect r3(QPoint(160, 20+20*(r+1)+20*c), QSize(70, 20));
            painter.drawRect(r3);
            QString stra2=QString::number(Nx(ro1-1,x1));
            painter.drawText(165,35+20*(r+1)+20*c,stra2);
            QRect r4(QPoint(230, 20+20*(r+1)+20*c), QSize(70, 20));
            painter.drawRect(r4);
            QString stra3=QString::number(sigx(ro1-1,x1));
            painter.drawText(235,35+20*(r+1)+20*c,stra3);
            QRect r5(QPoint(300, 20+20*(r+1)+20*c), QSize(70, 20));
            painter.drawRect(r5);
            QString stra4=QString::number(ri[ro1-1].Sig);
            painter.drawText(305,35+20*(r+1)+20*c,stra4);
        }
    }
    if (b==1){
        QLine y(30,30,30,360);
        painter.drawLine(y);
        QLine x(360,180,30,180);
        painter.drawLine(x);
        painter.setBrush(Qt::black);
        QPointF pointsx[3] = {
            QPointF(360, 175),
            QPointF(370, 180),
            QPointF(360, 185),
        };
        painter.drawPolygon(pointsx, 3);
        QPointF pointsy[3] = {
            QPointF(25, 30),
            QPointF(30, 20),
            QPointF(35, 30),
        };
        painter.drawPolygon(pointsy, 3);
        QString st=QString::number(0);
        painter.drawText(20,185,st);
        painter.drawText(365,195,"x");
        if (s==1){
            painter.drawText(5,30,"u(x)");
            double i=330/ri[ro1-1].L;
            double i1=ri[ro1-1].L/100;
            double j1=0;
            double i2=0;
            while (j1<ri[ro1-1].L){
                if (sqrt(pow(ux(ro1-1,j1),2))>i2) i2=sqrt(pow(ux(ro1-1,j1),2));
                j1+=i1;
            }
            if (i2!=0){
            i1=i2/150;
            c=0;
            for (int j=30; j<361; j++){
                QPoint p(j,180-ux(ro1-1,c/i)/i1);
                painter.drawPoint(p);
                c++;
            }
            int j=50;
            while (j<360){
                QString st=QString::number((j-30)/i);
                painter.drawText(j-5,195,st);
                painter.drawLine(j,175,j,185);
                j+=70;
            }
            j=50;
            while (j<180){
                QString st=QString::number((180-j)*i1);
                painter.drawText(10,j-5,st);
                painter.drawLine(25,j,35,j);
                j+=50;
            }
        }}
        if (s==2){
            painter.drawText(5,30,"N(x)");
            double i=330/ri[ro1-1].L;
            double i1=ri[ro1-1].L/100;
            double j1=0;
            double i2=0;
            while (j1<ri[ro1-1].L){
                if (sqrt(pow(Nx(ro1-1,j1),2))>i2) i2=sqrt(pow(Nx(ro1-1,j1),2));
                j1+=i1;
            }
            if (i2!=0){
            i1=i2/150;
            c=0;
            for (int j=30; j<361; j++){
                QPoint p(j,180-Nx(ro1-1,c/i)/i1);
                painter.drawPoint(p);
                c++;
            }
            int j=50;
            while (j<360){
                QString st=QString::number((j-30)/i);
                painter.drawText(j-5,195,st);
                painter.drawLine(j,175,j,185);
                j+=70;
            }
            j=50;
            while (j<180){
                QString st=QString::number((180-j)*i1);
                painter.drawText(10,j-5,st);
                painter.drawLine(25,j,35,j);
                j+=50;
            }
        }}
        if (s==3){
            painter.drawText(0,30,"Sig(x)");
            double i=330/ri[ro1-1].L;
            double i1=ri[ro1-1].L/100;
            double j1=0;
            double i2=0;
            while (j1<ri[ro1-1].L){
                if (sqrt(pow(sigx(ro1-1,j1),2))>i2) i2=sqrt(pow(sigx(ro1-1,j1),2));
                j1+=i1;
            }
            if (i2!=0){
            i1=i2/150;
            c=0;
            for (int j=30; j<361; j++){
                QPoint p(j,180-sigx(ro1-1,c/i)/i1);
                painter.drawPoint(p);
                c++;
            }
            int j=50;
            while (j<360){
                QString st=QString::number((j-30)/i);
                painter.drawText(j-5,195,st);
                painter.drawLine(j,175,j,185);
                j+=70;
            }
            j=50;
            while (j<180){
                QString st=QString::number((180-j)*i1);
                painter.drawText(10,j-5,st);
                painter.drawLine(25,j,35,j);
                j+=50;
            }
        }
    }}
}

MainWindow::~MainWindow()
{
    delete ui;
}
