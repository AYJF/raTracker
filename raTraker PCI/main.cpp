/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */


#include <QtGui/QApplication>
#include <QPlastiqueStyle>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new QPlastiqueStyle);
    MainWindow w;
    w.show();

    return a.exec();
}
