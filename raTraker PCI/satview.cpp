/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#include <QDebug>
#include <QResizeEvent>
#include <cstdio>
#include "satview.h"
#include "satscene.h"
#include "satitem.h"
#include "global.h"


extern double utcTime;

SatView::SatView(QWidget *parent) :
    QGraphicsView( parent )
{
    m_tID = 0;
    m_scene = new SatScene(this);
    setScene(m_scene);
    this->setCursor(Qt::OpenHandCursor);
}

void SatView::resizeEvent(QResizeEvent *event)
{
    forceResize(event->size().width(), event->size().height() );
}

void SatView::forceResize(int w, int h)
{
    QRectF scnR = scene()->sceneRect();
    scnR.setHeight(h);

    int mapW = ((SatScene*)scene())->m_map.width();

    if (w>mapW)
        scnR.setWidth(w);
    else
        scnR.setWidth(mapW);

    m_scene->setSceneRect(scnR);
    m_scene->updateMaper();
    m_scene->updateSatItemPositions();
    if (nPoints)
        ((SatScene *)scene())->m_curSatItem->calcTrack();
}

void SatView::setMap(const QPixmap &map)
{
    m_scene->setSceneRect(map.rect());
    m_scene->m_map = map;
    m_scene->updateMaper();
    forceResize(width(), height());
    m_scene->update();
}

void SatView::startTrack(int mseconds)
{
    if(m_tID) killTimer(m_tID);
    m_tID = startTimer(mseconds);
    m_scene->setNotofication(true);
}

void SatView::timerEvent(QTimerEvent *event)
{
    time_t ddtt = time(0);
    struct tm date = *(localtime(&ddtt));
    date.tm_mon++;
    date.tm_year+=1900;
    utcTime = cJulian(date.tm_year, date.tm_mon, date.tm_mday,
                      date.tm_hour, date.tm_min, date.tm_sec).m_Date + TimeZone;



    m_scene->advance();
    emit toInfo();
}

void SatView::addSat(SatItem *sat)
{
    m_scene->addItem(sat);
}

void SatView::beginSatTrack(SatItem *sat)
{
    killTimer(m_tID);
    centerOn(sat);
    emit readyToTracking(sat);
}
void SatView::startOneSatTrack(int mscds)
{
    if(m_tID) killTimer(m_tID);
    m_scene->setNotofication(false);
    m_tID = startTimer(mscds);


}
