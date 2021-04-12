/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */
#ifndef SATVIEW_H
#define SATVIEW_H

#include <QGraphicsView>



class SatScene;
class SatItem;
struct tle_t;

class SatView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SatView(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event);
    void forceResize(int w, int h);
    void setMap(const QPixmap &map);
    void startTrack(int);
    void timerEvent(QTimerEvent *);
    void addSat(SatItem * sat);
    void beginSatTrack(SatItem *);
    void startOneSatTrack(int mscds);

signals:
    void toInfo();
    void readyToTracking(SatItem *);
public slots:


private:
    SatScene *m_scene;
    int m_tID;
};

#endif // SATVIEW_H
