/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#include "azieleplot.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <math.h>



AziElePlot::AziElePlot(QWidget *parent) :
    QWidget(parent)
{
    m_plotPixmap.load(":/images/resources/aziElePlot.bmp");
    m_offsetX = 0;
    this->setCursor(Qt::OpenHandCursor);
}
void AziElePlot::resizeEvent(QResizeEvent *ev)
{
    Q_UNUSED(ev)

    screenW = m_plotPixmap.width();
    screenH = m_plotPixmap.height();
    if(&m_plotPixmap) update();
}

void AziElePlot::paintEvent(QPaintEvent *ev)
{    
    Q_UNUSED(ev)
    int mH = m_offsetX*(width()/m_plotPixmap.width());
    QPainter p(this);
    p.drawImage(QRect(0,0,width() - m_offsetX,height()),m_plotPixmap,QRect(mH,0,m_plotPixmap.width()-mH,m_plotPixmap.height()));
    p.drawImage(QRect(width() - m_offsetX ,0,m_offsetX,height()),m_plotPixmap,QRect(0,0,mH,m_plotPixmap.height()));
}

void AziElePlot::setSatTrack(double *azi, double *ele, int n)
{
    QPainter p(&m_plotPixmap);
    p.translate(0.5, 130);
    p.setPen(Qt::green);
    p.setRenderHint(QPainter::Antialiasing);

    qreal w = width();
    int i=0;
    while (ele[i] < 0.0)++i;
    while ( i<n && ele[i] >= 0 ){
        p.drawPoint(azi[i], -1.5*ele[i]);
        i+=10;
    }
}
void AziElePlot::setStepSatAnte(double aziSat,double eleSat,double aziAnt,double eleAnt)
{
    QRgb  valueRed = qRgb(255,0,0);
    QRgb  valueBlue = qRgb(0,0,255);
    m_plotPixmap.setPixel(.5+aziSat, 130.5-1.5*eleSat,valueRed);
    m_plotPixmap.setPixel(.5+aziAnt, 130.5-1.5*eleAnt, valueBlue);
    update();
}

void AziElePlot::reload()
{
    m_plotPixmap.load(":/images/resources/aziElePlot.bmp");
}

void AziElePlot::mousePressEvent(QMouseEvent *ev)
{
    this->setCursor(Qt::ClosedHandCursor);
    m_lastX = ev->x();
}

void AziElePlot::mouseMoveEvent(QMouseEvent *ev)
{
    int w = width();
    m_offsetX += m_lastX - ev->x() + w;
    m_lastX= ev->x();
    m_offsetX%=w;

    update();

}
void AziElePlot::mouseReleaseEvent(QMouseEvent *)
{
    this->setCursor(Qt::OpenHandCursor);
}
