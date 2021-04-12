/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */
#ifndef ELEPLOT_H
#define ELEPLOT_H

#include <QWidget>
#include <QImage>

class elePlot : public QWidget
{
    Q_OBJECT
public:
    explicit elePlot(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

private:
    QImage m_plotPixmap;
    
};

#endif // ELEPLOT_H
