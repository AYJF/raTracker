/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */
#ifndef TRACKERWIDGET_H
#define TRACKERWIDGET_H

#include <QDockWidget>

class MainWindow;

class TrackerWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit TrackerWidget(QMainWindow *parent = 0);
    void keyPressEvent(QKeyEvent *);
signals:
    
public slots:
private:
    MainWindow *m_p;
    
};

#endif // TRACKERWIDGET_H
