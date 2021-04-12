/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */


#include "eleplot.h"
#include <QPainter>

elePlot::elePlot(QWidget *parent) :
    QWidget(parent)
{
    m_plotPixmap.load(":/images/resources/2.bmp");
}
void elePlot::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0,0,m_plotPixmap);
}
