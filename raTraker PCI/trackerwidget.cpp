/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#include "trackerwidget.h"
#include "mainwindow.h"
#include <QDebug>


TrackerWidget::TrackerWidget(QMainWindow *parent) :
    QDockWidget(parent)
{
    m_p = (MainWindow *) parent;
}

void TrackerWidget::keyPressEvent(QKeyEvent *ev)
{
    QDockWidget::keyPressEvent(ev);
    m_p->userKeyInput(ev);
}
