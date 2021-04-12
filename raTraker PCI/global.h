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
 *    1) It's global class, given a chance of introduce cSite myStation,
 *       this class represents a location (ground site) on the earth (for more
 *       information see class cSite).
 *    2) TimeZone is the diference of time zone with respect the Greenwich Mean
 *    3)*satd is a pointer to QPixmap (icon for deep perturbation)
 *    4)*satg is a pointer to QPixmap (icon for general perturbation)
 *
 ****************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H
#include <QPixmap>
#include <QList>
#include "core/coreLib.h"

extern QPixmap *satd, *satg;
extern QPointF *trackArray;
extern int nPoints;
extern cSite myStation;
extern double TimeZone;
extern QList<QString> ltdName;
extern QList<double> ltdDovi;

void initGlobals();


#endif // GLOBAL_H
