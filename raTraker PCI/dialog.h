/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */
/***************************** NOTE TO USERS *********************************
 * Show the list of satellites's track, with the time of visible.
 *
 ****************************************************************************/

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void toLitsView(const QList<QString> &listname,const QList<QString> &hourPase);
    
private:
    Ui::Dialog *ui;
    QStandardItemModel m_satList;

};

#endif // DIALOG_H
