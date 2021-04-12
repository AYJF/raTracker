/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QList>
#include "trackerthread.h"
#include "satscene.h"

class SatItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *);
    void userKeyInput(QKeyEvent *);
    void createConnections();
    void bubbleSort();
    void swap(double *ele1,double *ele2);

signals:
    void satShow();
    void showPass(QList<QString> nameList,QList<QString>);

private slots:
    void on_actionLoad_TLEs_triggered();
    void updateInfo();
    void on_listView_clicked(const QModelIndex &index);
    void on_actionSatellites_triggered();
    void on_actionPass_Order_triggered();
    void writeSettings();
    void readSettings();

    void initTracking(SatItem *);
    void on_actionTrack_triggered();
    void on_actionLoad_Map_triggered();
    void on_pb_cancelThread_clicked();
    void restartTrack();
    void messageCard();
    void offsetAntSat(double aziSat, double eleSat,double aziAnt, double eleAnt);
    void showElevMax(double elevMaxima);




private:
    Ui::MainWindow *ui;
    QStandardItemModel m_satList;
    SatItem *m_curSat;
    SatScene myScene;
    TrackerThread m_trackThread;
    QString m_infoPase;
    QList<QString> m_List;
    QList<QString> m_nameL;
    int m_aziC, m_eleC;
    QString m_path;

};

#endif // MAINWINDOW_H
