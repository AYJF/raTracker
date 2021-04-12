/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */
/***************************** NOTE TO USERS *********************************
 *
 *                 THIS IS A HEADER FILE, NOT A MANUAL
 *
 *    If you want to know how to use Qt, please read any manual or consult
 *    http://qt.nokia.com/.
 *
 *    ABOUT THIS CLASS:
 *    1) A trackerThread represents a separate thread of control within the program;
 *       it shares data with all the other threads within the process but executes
 *       independently in the way that a separate program does on a multitasking
 *       operating system. TrackerThread begin executing in the virtual function run().
 *       this class create your own threads, subclass QThread and reimplement run()
 *       (see trackerThrad::run).  
 *    2) For more information see the class QThread in Qt Help
 *******************************************************************************/

#ifndef TRACKERTHREAD_H
#define TRACKERTHREAD_H

#include <QThread>
#include "non_gui_clases/VARY.H"
#include "core/coreLib.h"


struct ra_Antena{
    double xx;
    double yy;
};

class TrackerThread : public QThread
{
    Q_OBJECT
public:
    explicit TrackerThread(QObject *parent = 0);
    void run();
    void precalcSatTrack(cOrbit *orbit, double endTime);
    void ControlElevation();
    void ControlAzimuth();
    void ReadEncoder();
    void MaxElevAnzi();
    void setStop();


signals:
    void predictedSatTrack(double *azi, double *ele, int n);
    void stepSatAnt(double aziSat, double eleSat,double aziAnt, double eleAnt );
    void setElevMax(double eleM);
    void toMessage();

    
public slots:
    void setAziOffset(int n);
    void setEleOffset(int n);



private:
    double *m_aziArray;
    double *m_eleArray;
    double m_aziOffset;
    double m_eleOffset;
    double m_eleVelAnt,  m_lastEleVelAnt;
    double m_aziVelAnt,  m_lastAziVelAnt;

    int m_aziC, m_eleC, m_index;
    int m_nPoints;
    float an_off,bang_off;
    time_t m_initDate;
    volatile bool m_isRunning, m_userAzi, m_userEle;
    void updateUserInputs();

    CPCI_RW m_ddk;  //read and write to device
    struct ra_Antena m_ant;

};



#endif // TRACKERTHREAD_H
