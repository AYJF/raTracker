/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#include <QPainter>
#include "satscene.h"
#include "satitem.h"
#include "global.h"
#include "azieleplot.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>


double utcTime;
cCoordTopo cAziele;

SatItem::SatItem(cTle tle)
{
    m_cOrbit = new cOrbit(tle);
    m_name = QString::fromStdString(m_cOrbit->SatName(false));
    m_sgp = (m_cOrbit->Period() < 255*60);
    calcDatesOfV();
    m_bRect.setRect(-8 - 0.5, -8 - 0.5,
                    16 + 1, 16 + 1);

    m_showName = false;

}

QRectF SatItem::boundingRect() const
{
    return m_bRect;
}

void SatItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    painter->drawPixmap(-6,-6, (m_sgp) ? *satg : *satd);
    if (m_showName){
        QFont serifFont("Times", 8);
        painter->setFont(serifFont);
        painter->setPen(Qt::white);
        painter->drawText(m_bRect.adjusted(17, 0, 0, 0), Qt::AlignCenter, m_name);
    }
}

void SatItem::showName(bool s)
{
    prepareGeometryChange();
    m_showName=s;
    if (!m_showName){
        m_bRect.setSize(QSizeF(17,17));
    } else {
        QFont serifFont("Times", 8);
        QFontMetrics metrics(serifFont);

        double w = metrics.width(m_name);
        m_bRect.setWidth(16+w+1);
        if (m_bRect.height() < metrics.height())
            m_bRect.setHeight(metrics.height()+1);
    }
}

void SatItem::advance(int phase)
{
    if(phase == 0){
        calcSatPos();
        return;
    }

    if ( m_datesOfV.count() && ((SatScene*)scene())->isNotification() ){

        if ( (utcTime >= (m_datesOfV[0].utcBegin - 15.0/SEC_PER_DAY)) && (utcTime < m_datesOfV[0].utcEnd)   ){

            int r = QMessageBox::information(0, "Tracking",
                                     QString("He wishes to give tracking to: %1").arg(m_cOrbit->SatName(false).c_str()),
                                     "Yes", "No", "No returning to notify");



            switch (r)
            {
                case 0:
                    ((SatScene*)scene())->beginTrack(this);
                break;
                case 2:
                    m_datesOfV.removeAt(0);
                break;
            }

        } else if ( m_datesOfV[0].utcEnd < utcTime )
            m_datesOfV.removeAt(0);
    }
    SatScene *scn = (SatScene *) scene();
    QPointF p = scn->mapFromNadir(m_geoCoord.m_Lon, m_geoCoord.m_Lat);
    m_xCoord = p.x();
    p.setX(fmod(m_xCoord-scn->m_offset+scn->width(), scn->width()));
    setPos(p);
}

void SatItem::calcSatPos()
{    
    m_EciPos = m_cOrbit->GetPosition( (utcTime - m_cOrbit->Epoch().m_Date)*MIN_PER_DAY );
    m_geoCoord = m_EciPos.ToGeo();
    if (m_geoCoord.m_Lon > PI)
        m_geoCoord.m_Lon -= TWOPI;

}

QString &SatItem::infoPase()
{
    if (m_datesOfV.count()){
        int year, mont, d, h, m, s;
        double frac;
        cJulian(m_datesOfV.first().utcBegin-TimeZone).GetComponent(&year, &mont, &frac);
        d = frac; frac-=d; frac*=24;
        h = frac; frac-=h; frac*=60;
        m = frac; frac-=m; frac*=60;
        s = frac+.5;
        m_infoPase.sprintf("%d/%02d/%02d_%02d:%02d:%02d", year, mont, d, h, m, s);
    }
    return m_infoPase;
}

QString &SatItem::satName()
{
    return m_name;
}

cOrbit *SatItem::getOrbit()
{
    return m_cOrbit;
}

bool SatItem::isSdp()
{
    return m_sgp;
}
double SatItem::nadirLat()
{
    return rad2deg(m_geoCoord.m_Lat);
}

double SatItem::period()
{
    return m_cOrbit->Period()/(60*60);
}

double SatItem::nadirLon()
{
    return rad2deg(m_geoCoord.m_Lon);
}

double SatItem::alt()
{
    return m_geoCoord.m_Alt;
}

void SatItem::AziEle(double *Azi, double *Ele)
{
    cCoordTopo coordTopo = myStation.GetLookAngle(m_EciPos);
    *Azi = rad2deg(coordTopo.m_Az);
    *Ele = rad2deg(coordTopo.m_El);
}

void SatItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->pos().x() < 8 && event->button() == Qt::LeftButton ){
        calcTrack();

        if (!m_showName){
            showName(true);
        }
    }
    else if(event->pos().x() < 8 && event->button() == Qt::RightButton)
        calcVisibleTrack();
    else
        if (m_showName) showName(false);

}

void SatItem::calcTrack()
{
    bool nvsb = true;
    nPoints=0;
    double dt = ((m_cOrbit->Period()/60.0)/2024);
    double a, tsince = (utcTime-m_cOrbit->Epoch().m_Date)*MIN_PER_DAY;
    double curTime = utcTime;
    a = tsince;
    SatScene *satScene = ((SatScene*)scene());
    satScene->m_curSatItem = this;
    cCoordTopo aziEle;
    while (nvsb && nPoints<2024){
        m_EciPos = m_cOrbit->GetPosition(tsince);
        m_geoCoord = m_EciPos.ToGeo();
        if (m_geoCoord.m_Lon > PI)
            m_geoCoord.m_Lon -= TWOPI;
        aziEle = myStation.GetLookAngle(m_EciPos);

        if (aziEle.m_El > 0) nvsb=false;
        trackArray[nPoints] = satScene->mapFromNadir(m_geoCoord.m_Lon, m_geoCoord.m_Lat);
        ++nPoints;
        curTime = utcTime + (nPoints*dt)/MIN_PER_DAY;
        tsince = a + dt*nPoints;

    }
    satScene->update();

}

void SatItem::calcDatesOfV()
{
    double tsince;

    struct Pase _ps;

    time_t ddtt = time(0);
    struct tm date = *(localtime(&ddtt));
    date.tm_mon++;
    date.tm_year+=1900;

    cJulian Julian_date(date.tm_year, date.tm_mon, date.tm_mday,
                         date.tm_hour, date.tm_min, date.tm_sec);
    Julian_date.AddDay(TimeZone);
    Julian_date.AddHour(-1);


    const double stop = Julian_date.m_Date+1;
    const double tleEpoch = m_cOrbit->Epoch().m_Date;
    double beginTime, lastTime, xTime;
    int signEle,signAnt, s, xs;


    tsince = (Julian_date.m_Date - tleEpoch)*MIN_PER_DAY;
    m_EciPos = m_cOrbit->GetPosition(tsince);
    cAziele = myStation.GetLookAngle(m_EciPos);
    signEle = (cAziele.m_El<0) ? -1 : 1;
    signAnt = signEle;
    beginTime = Julian_date.m_Date;
    Julian_date.AddMin(5);

    while( Julian_date.m_Date < stop){

        tsince = (Julian_date.m_Date - tleEpoch)*MIN_PER_DAY;
        m_EciPos = m_cOrbit->GetPosition(tsince);
        cAziele = myStation.GetLookAngle(m_EciPos);
        s = (cAziele.m_El<0) ? -1 : 1;


        if (s == 1 && signEle == -1){

            lastTime = Julian_date.m_Date;
            while ( (lastTime-beginTime)/2 > 1.0/(24*60*60)){
                xTime = (beginTime+lastTime)/2;
                tsince = (xTime - tleEpoch)*MIN_PER_DAY;
                m_EciPos = m_cOrbit->GetPosition(tsince);
                cAziele = myStation.GetLookAngle(m_EciPos);

                if (cAziele.m_El==0.0)
                    break;
                else
                    xs = (cAziele.m_El<0) ? -1 : 1;

                if (xs == -1)
                    beginTime=xTime;
                else
                    lastTime=xTime;
            }
            _ps.utcBegin = xTime - 30.0/SEC_PER_DAY;

            beginTime = Julian_date.m_Date;
            signEle = 1;
            Julian_date.AddMin(5);


            while (Julian_date.m_Date < stop){

                tsince = (Julian_date.m_Date - tleEpoch)*MIN_PER_DAY;
                m_EciPos = m_cOrbit->GetPosition(tsince);
                cAziele = myStation.GetLookAngle(m_EciPos);

                s = (cAziele.m_El<0) ? -1 : 1;

                if (s == -1){
                    lastTime = Julian_date.m_Date;

                    while ( (lastTime-beginTime)/2 > 1.0/(24*60*60)){
                        xTime = (beginTime+lastTime)/2;
                        tsince = (xTime - tleEpoch)*MIN_PER_DAY;
                        m_EciPos = m_cOrbit->GetPosition(tsince);
                        cAziele = myStation.GetLookAngle(m_EciPos);

                        if (cAziele.m_El==0.0)
                            break;
                        else
                            xs = (cAziele.m_El<0) ? -1 : 1;

                        if (xs == 1)
                            beginTime=xTime;
                        else
                            lastTime=xTime;
                    }
                    _ps.utcEnd = xTime;
                    m_datesOfV.push_back(_ps);
                    break;
                }
                beginTime = Julian_date.m_Date;
                Julian_date.AddMin(5);
            }
        }

        beginTime = Julian_date.m_Date;
        Julian_date.AddMin(5);
        signEle = s;
    }




    if (!m_datesOfV.count()){
        if (s == -1 && signAnt == -1)
            m_infoPase = "Not Visible";
        else if(s == -1 && signAnt == 1)
            m_infoPase = "No visible pannier";
        else if(s == 1 && signAnt == -1)
            m_infoPase = "Visible pannier";
        else
            m_infoPase ="Visible always";

    }


        for(int i = 0; i < m_datesOfV.size();i++){
            ltdName.push_back(m_name);
            ltdDovi.push_back(m_datesOfV[i].utcBegin);

        }



}

void SatItem::calcVisibleTrack()
{
    bool vsb = true;
    nPoints=0;
    double dt = ((m_cOrbit->Period()/60.0)/2024);
    double a, tsince = (utcTime-m_cOrbit->Epoch().m_Date)*MIN_PER_DAY;
    double curTime = utcTime;
    a = tsince;
    SatScene *satScene = ((SatScene*)scene());
    satScene->m_curSatItem = this;
    cCoordTopo aziEle;
    while (vsb && nPoints<2024){
        m_EciPos = m_cOrbit->GetPosition(tsince);
        m_geoCoord = m_EciPos.ToGeo();
        if (m_geoCoord.m_Lon > PI)
            m_geoCoord.m_Lon -= TWOPI;
        aziEle = myStation.GetLookAngle(m_EciPos);

        if (aziEle.m_El < 0) vsb=false;
        trackArray[nPoints] = satScene->mapFromNadir(m_geoCoord.m_Lon, m_geoCoord.m_Lat);
        ++nPoints;
        curTime = utcTime + (nPoints*dt)/MIN_PER_DAY;
        tsince = a + dt*nPoints;

    }
    satScene->update();




}
