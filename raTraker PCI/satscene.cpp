/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#include "satscene.h"
#include "satitem.h"
#include "satview.h"
#include "trackerthread.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "global.h"


QList<QGraphicsItem*> allItem;

SatScene::SatScene(QObject *parent) :
    QGraphicsScene(parent)
{
    m_offset=0;
    m_track = new QPointF [2024];
    m_scf=1;
    m_notification = true;

}

void SatScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{  
    m_lastX = event->scenePos().x();
    SatItem *item = (SatItem*)itemAt(event->scenePos());

    if ( nPoints && item && item == m_curSatItem){
        nPoints=0;
        event->ignore();
        update();
    } else
        QGraphicsScene::mousePressEvent(event);
}

void SatScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int w = width();
    m_offset+=(m_lastX-event->scenePos().x()+w);
    m_lastX = event->scenePos().x();
    m_offset%=w;
    updateSatItemPositions();
    update();
}

void SatScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    static int last_offset=-1;
    int mH = m_offset*m_scf+.5;
    painter->drawPixmap(QRect(0, 0, width()-m_offset, height()), m_map, QRect(mH, 0, m_map.width()-mH, m_map.height()));
    painter->drawPixmap(QRect(width()-m_offset, 0, m_offset, height()), m_map, QRect(0, 0, mH, m_map.height()));

    painter->setPen(Qt::red);
    if (nPoints){
        last_offset = m_offset;
        qreal w = width();
        for (int i=0; i<nPoints; ++i){
            m_track[i] = trackArray[i];
            m_track[i].setX( fmod( trackArray[i].x()-m_offset+w, w) );
        }
        painter->drawPoints(m_track, nPoints);
    }
}

QPointF SatScene::mapFromNadir(const qreal lon, const qreal lat)
{
    return QPointF( m_aw*lon + m_bw, m_ah*lat + m_bh);
}


void SatScene::updateSatItemPositions()
{
    double tx;
    double w = width();
    foreach (QGraphicsItem *it, items()){
        tx = ((SatItem*)it)->m_xCoord - m_offset + w;
        it->setX(fmod(tx,w));
    }
}

void SatScene::updateMaper()
{
    qreal mH,mW;
    mH = height();
    mW = width();
    m_aw = mW/(2*PI);  //longitud
    m_bw = mW/2;

    m_ah = -mH/PI;     //latitud
    m_bh = mH/2;

    m_scf = qreal(m_map.width())/mW; //scale
}

void SatScene::beginTrack(SatItem *sat)
{
    ((SatView*)views().first())->beginSatTrack(sat);

    foreach (QGraphicsItem *item, items()){
        if (item != sat){
            allItem<<item;
            item->setVisible(false);
//            item->setActive(false);
        }
    }
    sat->showName(true);
    sat->setVisible(true);

}
void SatScene::showAllSat()
{

    for(int i = 0; i < allItem.size();++i){
        allItem[i]->setVisible(true);
    }
}

void SatScene::setNotofication(bool isYes)
{
    m_notification = isYes;
}

bool SatScene::isNotification()
{
    return m_notification;
}
