/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */
#ifndef SATITEM_H
#define SATITEM_H

#include <QGraphicsItem>
#include "core/coreLib.h"

struct Pase{
    double utcBegin;
    double utcEnd;
//    double alt;

};


class SatItem : public QGraphicsItem
{
    friend class SatScene;
    friend class SatView;
public:
    SatItem(cTle tle);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void advance(int phase);
    void calcSatPos();
    bool isSdp();
    double nadirLat();
    double nadirLon();
    double period();
    double alt();
    void showName(bool );
    void calcVisibleTrack();


    QString &infoPase();
    void AziEle(double *Azi,double *Ele );
    QString &satName();
    cOrbit *getOrbit();

    QList<Pase> m_datesOfV;

public slots:
    void setLatLon(qreal *lat,qreal *lon, int nPoints);

private:
    void calcDatesOfV();
    void calcTrack();
    bool m_sgp;
    cOrbit *m_cOrbit;
    cCoordGeo m_geoCoord;
    cEci m_EciPos;
    qreal m_xCoord;
    QString m_infoPase;
    QString m_name;
    QRectF m_bRect;
    bool m_showName;
};

#endif // SATITEM_H
