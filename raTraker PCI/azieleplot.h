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
 *    1) Is for paint the track of satellite in the widget AziElePlot.
 *    2) The graphics is move to x-axi in two directions, you only need cliked
 *       in the widget and move whit the mouse pressed.
 *    3) Green line represent the track of the satellite
 *    4) Red line represent the current position of the satellite
 *    5) Blue line represent the current position of the Antena
 *
 ****************************************************************************/


#ifndef AZIELEPLOT_H
#define AZIELEPLOT_H

#include <QWidget>

class AziElePlot : public QWidget
{
    Q_OBJECT
public:
    explicit AziElePlot(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void reload();

signals:
    
public slots:
    void setSatTrack(double *azi, double *ele, int n);
    void setStepSatAnte(double aziSat,double eleSat,double aziAnt,double eleAnt);

private:
    QImage m_plotPixmap;
    int m_offsetX;
    int m_lastX;
    int screenW;
    int screenH;

};

#endif // AZIELEPLOT_H
