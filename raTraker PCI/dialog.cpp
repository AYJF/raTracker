/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Next");
    setAutoFillBackground(true);
    setPalette(Qt::darkGray);
    m_satList.setColumnCount(1);    
    ui->treeView->setModel(&m_satList);
    m_satList.setHorizontalHeaderLabels(QStringList() << "Sat. Name" << "Hour");
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::toLitsView(const QList<QString> &listname, const  QList<QString> &hourPase)
{
    QStandardItem *itNm, *itH;
    for(int i = 0; i <hourPase.size();i++){
        itNm = new QStandardItem(listname[i]);
        itH = new QStandardItem(hourPase[i]);
        m_satList.appendRow(QList<QStandardItem*>() << itNm << itH);
    }
}
