/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#include <QPoint>
#include "global.h"



QPixmap *satd, *satg;
double TimeZone;
cSite myStation;
QPointF *trackArray;
int nPoints;
QList<QString> ltdName;
QList<double> ltdDovi;

void initGlobals()
{
    trackArray = new QPointF [2024];
    satd = new QPixmap(":/Icons/resources/satd.PNG");
    satg = new QPixmap(":/Icons/resources/satg.PNG");

    myStation = cSite(22.947190, -82.364090, 188.0/1000); /* lat,lon->degrees , AltKm */
    TimeZone = 4/24.0; /*diference of time zone with respect the Greenwich Mean*/
}


