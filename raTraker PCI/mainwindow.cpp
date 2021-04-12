/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#include <QFileDialog>
#include <fstream>
#include <QDateTime>
#include <QMouseEvent>
#include <QStandardItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "satitem.h"
#include "global.h"
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QtAlgorithms>
#include "dialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initGlobals();
    setWindowTitle("raTracker");


    setAutoFillBackground(true);
    setPalette(Qt::darkGray);

    m_satList.setColumnCount(1);
    ui->listView->setModel(&m_satList);
    ui->listView->setCursor(Qt::PointingHandCursor);
    ui->graphicsView->setMap(QPixmap(":/images/resources/map.bmp"));
    ui->graphicsView->setCursor(Qt::OpenHandCursor);

    createConnections();
    readSettings();

    m_aziC=0;
    m_eleC=0;

    ui->dockWidgetTrack->close();

}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}
void MainWindow::createConnections()
{
    connect(ui->graphicsView,SIGNAL(toInfo()),SLOT(updateInfo()));
    connect(ui->graphicsView, SIGNAL(readyToTracking(SatItem*)), SLOT(initTracking(SatItem*)));
    connect(&m_trackThread, SIGNAL(predictedSatTrack(double*,double*,int)),
            ui->azielePlot, SLOT(setSatTrack(double*,double*,int)));
    connect(this,SIGNAL(satShow()),&myScene,SLOT(showAllSat()));
    connect(&m_trackThread,SIGNAL(stepSatAnt(double,double,double,double)),ui->azielePlot,SLOT(setStepSatAnte(double,double,double,double)));
    connect(&m_trackThread,SIGNAL(finished()),this,SLOT(restartTrack()));
    connect(&m_trackThread,SIGNAL(stepSatAnt(double,double,double,double)),this,SLOT(offsetAntSat(double,double,double,double)));
    connect(&m_trackThread,SIGNAL(toMessage()),this,SLOT(messageCard()));
    connect(&m_trackThread,SIGNAL(setElevMax(double)),this,SLOT(showElevMax(double)));

}

void MainWindow::readSettings()
{    
    QSettings settings("AYJF","raTracker",this);
    m_path = settings.value("currentDir").toString();
}

void MainWindow::writeSettings()
{
    QSettings settings("AYJF","raTracker",this);
    settings.setValue("currentDir",m_path);
}

void MainWindow::on_actionLoad_TLEs_triggered()
{
    QString TLEFilePath = QFileDialog::getOpenFileName(this,"Open Tle...",m_path);
    if (TLEFilePath.isEmpty()) return;

    m_path = TLEFilePath;

    QStandardItem *stdItem;
    std::ifstream tlefile(TLEFilePath.toUtf8());
    string satName, line1, line2;
    char line[128];
    while (tlefile){

        tlefile.getline(line,    127);  satName = line;   /*read TLE  */
        tlefile.getline(line,    127);  line1   = line;   /*in 3 lines*/
        tlefile.getline(line,    127);  line2   = line;

        if (!tlefile) break;

        m_curSat = new SatItem(cTle(satName, line1, line2)); /*create the new satellite for this TLE*/

        stdItem = new QStandardItem(((m_curSat->isSdp()) ? *satg : *satd),m_curSat->satName());
        stdItem->setData(QByteArray((char*)&m_curSat,sizeof(SatItem *)));
        m_satList.appendRow(stdItem);
        ui->graphicsView->addSat(m_curSat);
    }
    ui->lb_sat->setText(QString("%1 satellites").arg(m_satList.rowCount()));
    ui->graphicsView->startTrack(1000); /*Begin the track of satellites, 1000 milliseconds*/
}
void MainWindow::updateInfo()
{
    ui->la_lat->setNum(m_curSat->nadirLat());
    ui->la_lon->setNum(m_curSat->nadirLon());
    double a, e;
    m_curSat->AziEle(&a, &e);
    ui->la_azi->setNum(a);
    ui->la_elev->setNum(e);
    ui->la_alt->setNum(m_curSat->alt());
    ui->la_period->setNum(m_curSat->period());
    ui->la_pase->setText(m_curSat->infoPase());
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QStandardItem *item = m_satList.itemFromIndex(index);
    m_curSat =  *((SatItem **)item->data().toByteArray().data());
    m_curSat->showName(true);
    ui->gb_satInfo->setTitle(item->text());
    ui->graphicsView->centerOn(m_curSat);
}

void MainWindow::on_actionSatellites_triggered()
{
    ui->dockWidgetSats->show();
}

void MainWindow::initTracking(SatItem *sat)
{
    m_aziC=0;
    m_eleC=0;
    ui->dockWidgetTrack->show();
    m_trackThread.precalcSatTrack(sat->getOrbit(), sat->m_datesOfV[0].utcEnd);
    ui->graphicsView->startOneSatTrack(3000);
    m_trackThread.start();

}


void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    QMainWindow::keyPressEvent(ev);
    userKeyInput(ev);
}

void MainWindow::userKeyInput(QKeyEvent *ev)
{
    if (ui->dockWidgetTrack->isVisible()){
        switch (ev->key())
        {
        case Qt::Key_Right:
            m_trackThread.setAziOffset(++m_aziC);
            ui->laAziOffset->setNum(m_aziC*0.05);
        break;
        case Qt::Key_Left:
            m_trackThread.setAziOffset(--m_aziC);
            ui->laAziOffset->setNum(m_aziC*0.05);
        break;

        case Qt::Key_Up:
            m_trackThread.setEleOffset(++m_eleC);
            ui->lbEleOffset->setNum(m_eleC*0.05);
        break;
        case Qt::Key_Down:
            m_trackThread.setEleOffset(--m_eleC);
            ui->lbEleOffset->setNum(m_eleC*0.05);
        break;
        }
    }
}

void MainWindow::on_actionTrack_triggered()
{
    ui->dockWidgetTrack->show();
}

void MainWindow::on_actionLoad_Map_triggered()
{
    QString map = QFileDialog::getOpenFileName(this,"Map...");
    if(map.isEmpty())return;
    ui->graphicsView->setMap(QPixmap(map));
}

void MainWindow::on_pb_cancelThread_clicked()
{
    if(m_trackThread.isRunning())
    {
        m_trackThread.setStop();
        ui->azielePlot->reload();
        restartTrack();
    }

}
void MainWindow::restartTrack()
{
    ui->dockWidgetSats->show();
    ui->dockWidgetTrack->close();
    ui->graphicsView->startTrack(1000);
    ui->lb_elevMaxi->clear();
    emit satShow();
}

void MainWindow::messageCard()
{
    QMessageBox::warning(this, tr("Error of Configuration !!!!"),
                      tr("The Card can not be configurate.\n"
                         "The calculation continue anyway "),
                         QMessageBox::Ok);

}

void MainWindow::offsetAntSat(double aziSat, double eleSat,
                              double aziAnt, double eleAnt)
{
    ui->lbAziDiff->setNum(aziSat - aziAnt);
    ui->lbEleDiff->setNum(eleSat - eleAnt);

    ui->lb_aziSat->setNum(aziSat);
    ui->lb_eleSat->setNum(eleSat);

    ui->lb_aziAnte->setNum(aziAnt);
    ui->lb_eleAnte->setNum(eleAnt);
}
void MainWindow::showElevMax(double elevMaxima)
{
    ui->lb_elevMaxi->setNum(elevMaxima);
}

void MainWindow::bubbleSort()
{
    for(int i = 0;i < ltdDovi.size()-1;i++){
        for(int j = i+1;j<ltdDovi.size();j++){
            if(ltdDovi[i]>ltdDovi[j]){

                const double temp = ltdDovi[i];
                ltdDovi[i] = ltdDovi[j];
                ltdDovi[j] = temp;

                const QString temp1 = ltdName[i];
                ltdName[i] = ltdName[j];
                ltdName[j] = temp1;
            }
        }
    }
}
void MainWindow::on_actionPass_Order_triggered()
{
    m_nameL.clear();
    m_List.clear();
    bubbleSort();

    int year, mont, d, h, m, s;
    double frac;
    int j=0;
    for(int i = 0; i < ltdDovi.size();i++){

        cJulian(ltdDovi[i]-TimeZone).GetComponent(&year, &mont, &frac);
        d = frac; frac-=d; frac*=24;
        h = frac; frac-=h; frac*=60;
        m = frac; frac-=m; frac*=60;
        s = frac+.5;

        if(h > QTime::currentTime().hour()){
            m_infoPase.sprintf("%d/%02d/%02d_%02d:%02d:%02d", year, mont, d, h, m, s);
            m_List.insert(j,m_infoPase);
            m_nameL.insert(j,ltdName[i]);
            j++;
        }
        else if(h == QTime::currentTime().hour() && m > QTime::currentTime().minute() ){
            m_infoPase.sprintf("%d/%02d/%02d_%02d:%02d:%02d", year, mont, d, h, m, s);
            m_List.insert(j,m_infoPase);
            m_nameL.insert(j,ltdName[i]);
            j++;
        }
    }

    Dialog dlg(this);
    dlg.toLitsView(m_nameL,m_List);
    dlg.exec();

}
