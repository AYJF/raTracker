/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */
#ifndef SATSCENE_H
#define SATSCENE_H

#include <QGraphicsScene>

class SatItem;

class SatScene : public QGraphicsScene
{
    friend class SatView;
    friend class SatItem;
    Q_OBJECT
public:
    explicit SatScene(QObject *parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    QPointF mapFromNadir(const qreal lon, const qreal lat);

    void beginTrack(SatItem *item);
    void setNotofication(bool isYes);
    bool isNotification();
    
signals:
    
public slots:
    void showAllSat();

private:
    void updateMaper();
    void updateSatItemPositions();
    qreal m_aw,m_bw;
    qreal m_ah,m_bh;
    qreal m_lastX;
    int m_offset;
    qreal m_scf;
    QPointF *m_track;
    QPixmap m_map;
    SatItem *m_curSatItem;
    bool m_notification;

};

#endif // SATSCENE_H
