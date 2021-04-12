/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat 13. Dec 19:13:00 2014
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <eleplot.h>
#include <satview.h>
#include "azieleplot.h"
#include "trackerwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_TLEs;
    QAction *actionSatellites;
    QAction *actionTrack;
    QAction *actionLoad_Map;
    QAction *actionPass_Order;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    SatView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidgetSats;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QLabel *lb_sat;
    QListView *listView;
    QGroupBox *gb_satInfo;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *la_lat;
    QLabel *label_2;
    QLabel *la_lon;
    QLabel *label_7;
    QLabel *la_azi;
    QLabel *label_9;
    QLabel *la_elev;
    QLabel *label_12;
    QLabel *la_alt;
    QLabel *label_11;
    QLabel *la_pase;
    QLabel *label_13;
    QLabel *la_period;
    QLabel *label_18;
    QLabel *lb_elevMaxi;
    TrackerWidget *dockWidgetTrack;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QFormLayout *formLayout_3;
    QLabel *laAziOffset;
    QLabel *label_10;
    QLabel *lbEleOffset;
    QLabel *label_8;
    QFormLayout *formLayout_4;
    QLabel *label_14;
    QLabel *lbAziDiff;
    QLabel *label_16;
    QLabel *lbEleDiff;
    QFormLayout *formLayout_5;
    QLabel *label_3;
    QLabel *lb_aziSat;
    QLabel *label_15;
    QLabel *lb_eleSat;
    QFormLayout *formLayout_6;
    QLabel *label_4;
    QLabel *lb_aziAnte;
    QLabel *label_17;
    QLabel *lb_eleAnte;
    QPushButton *pb_cancelThread;
    elePlot *widget;
    AziElePlot *azielePlot;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(925, 641);
        MainWindow->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/resources/globe.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionLoad_TLEs = new QAction(MainWindow);
        actionLoad_TLEs->setObjectName(QString::fromUtf8("actionLoad_TLEs"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/resources/Documents.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon1.addFile(QString::fromUtf8(":/images/resources/folder.png"), QSize(), QIcon::Active, QIcon::On);
        actionLoad_TLEs->setIcon(icon1);
        actionSatellites = new QAction(MainWindow);
        actionSatellites->setObjectName(QString::fromUtf8("actionSatellites"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/resources/viewSat.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSatellites->setIcon(icon2);
        actionTrack = new QAction(MainWindow);
        actionTrack->setObjectName(QString::fromUtf8("actionTrack"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/resources/Exe.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTrack->setIcon(icon3);
        actionLoad_Map = new QAction(MainWindow);
        actionLoad_Map->setObjectName(QString::fromUtf8("actionLoad_Map"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/resources/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad_Map->setIcon(icon4);
        actionPass_Order = new QAction(MainWindow);
        actionPass_Order->setObjectName(QString::fromUtf8("actionPass_Order"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/resources/notepad.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPass_Order->setIcon(icon5);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(1, 1, 1, 1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new SatView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        graphicsView->setFocusPolicy(Qt::NoFocus);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setInteractive(true);

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 925, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidgetSats = new QDockWidget(MainWindow);
        dockWidgetSats->setObjectName(QString::fromUtf8("dockWidgetSats"));
        dockWidgetSats->setMaximumSize(QSize(524287, 524287));
        dockWidgetSats->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lb_sat = new QLabel(dockWidgetContents);
        lb_sat->setObjectName(QString::fromUtf8("lb_sat"));

        verticalLayout->addWidget(lb_sat);

        listView = new QListView(dockWidgetContents);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setAlternatingRowColors(true);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);
        listView->setUniformItemSizes(true);

        verticalLayout->addWidget(listView);

        gb_satInfo = new QGroupBox(dockWidgetContents);
        gb_satInfo->setObjectName(QString::fromUtf8("gb_satInfo"));
        formLayout = new QFormLayout(gb_satInfo);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(1, 1, 1, 1);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(3);
        formLayout->setVerticalSpacing(1);
        label = new QLabel(gb_satInfo);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        la_lat = new QLabel(gb_satInfo);
        la_lat->setObjectName(QString::fromUtf8("la_lat"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(la_lat->sizePolicy().hasHeightForWidth());
        la_lat->setSizePolicy(sizePolicy1);
        la_lat->setFrameShape(QFrame::Box);
        la_lat->setFrameShadow(QFrame::Sunken);
        la_lat->setLineWidth(0);
        la_lat->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(0, QFormLayout::FieldRole, la_lat);

        label_2 = new QLabel(gb_satInfo);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        la_lon = new QLabel(gb_satInfo);
        la_lon->setObjectName(QString::fromUtf8("la_lon"));
        sizePolicy1.setHeightForWidth(la_lon->sizePolicy().hasHeightForWidth());
        la_lon->setSizePolicy(sizePolicy1);
        la_lon->setFrameShape(QFrame::Box);
        la_lon->setLineWidth(0);
        la_lon->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(1, QFormLayout::FieldRole, la_lon);

        label_7 = new QLabel(gb_satInfo);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_7);

        la_azi = new QLabel(gb_satInfo);
        la_azi->setObjectName(QString::fromUtf8("la_azi"));
        sizePolicy1.setHeightForWidth(la_azi->sizePolicy().hasHeightForWidth());
        la_azi->setSizePolicy(sizePolicy1);
        la_azi->setFrameShape(QFrame::Box);
        la_azi->setLineWidth(0);
        la_azi->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(2, QFormLayout::FieldRole, la_azi);

        label_9 = new QLabel(gb_satInfo);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_9);

        la_elev = new QLabel(gb_satInfo);
        la_elev->setObjectName(QString::fromUtf8("la_elev"));
        sizePolicy1.setHeightForWidth(la_elev->sizePolicy().hasHeightForWidth());
        la_elev->setSizePolicy(sizePolicy1);
        la_elev->setFrameShape(QFrame::Box);
        la_elev->setLineWidth(0);
        la_elev->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(3, QFormLayout::FieldRole, la_elev);

        label_12 = new QLabel(gb_satInfo);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy2);

        formLayout->setWidget(4, QFormLayout::LabelRole, label_12);

        la_alt = new QLabel(gb_satInfo);
        la_alt->setObjectName(QString::fromUtf8("la_alt"));
        sizePolicy1.setHeightForWidth(la_alt->sizePolicy().hasHeightForWidth());
        la_alt->setSizePolicy(sizePolicy1);
        la_alt->setFrameShape(QFrame::Box);
        la_alt->setLineWidth(0);
        la_alt->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(4, QFormLayout::FieldRole, la_alt);

        label_11 = new QLabel(gb_satInfo);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy2.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy2);

        formLayout->setWidget(5, QFormLayout::LabelRole, label_11);

        la_pase = new QLabel(gb_satInfo);
        la_pase->setObjectName(QString::fromUtf8("la_pase"));
        sizePolicy1.setHeightForWidth(la_pase->sizePolicy().hasHeightForWidth());
        la_pase->setSizePolicy(sizePolicy1);
        la_pase->setFrameShape(QFrame::Box);
        la_pase->setLineWidth(0);
        la_pase->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(5, QFormLayout::FieldRole, la_pase);

        label_13 = new QLabel(gb_satInfo);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        sizePolicy2.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy2);

        formLayout->setWidget(6, QFormLayout::LabelRole, label_13);

        la_period = new QLabel(gb_satInfo);
        la_period->setObjectName(QString::fromUtf8("la_period"));
        sizePolicy1.setHeightForWidth(la_period->sizePolicy().hasHeightForWidth());
        la_period->setSizePolicy(sizePolicy1);
        la_period->setFrameShape(QFrame::Box);
        la_period->setLineWidth(0);
        la_period->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(6, QFormLayout::FieldRole, la_period);

        label_18 = new QLabel(gb_satInfo);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_18);

        lb_elevMaxi = new QLabel(gb_satInfo);
        lb_elevMaxi->setObjectName(QString::fromUtf8("lb_elevMaxi"));
        sizePolicy1.setHeightForWidth(lb_elevMaxi->sizePolicy().hasHeightForWidth());
        lb_elevMaxi->setSizePolicy(sizePolicy1);
        lb_elevMaxi->setFrameShape(QFrame::Box);
        lb_elevMaxi->setLineWidth(0);
        lb_elevMaxi->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(7, QFormLayout::FieldRole, lb_elevMaxi);


        verticalLayout->addWidget(gb_satInfo);

        dockWidgetSats->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetSats);
        dockWidgetTrack = new TrackerWidget(MainWindow);
        dockWidgetTrack->setObjectName(QString::fromUtf8("dockWidgetTrack"));
        dockWidgetTrack->setFocusPolicy(Qt::NoFocus);
        dockWidgetTrack->setAllowedAreas(Qt::BottomDockWidgetArea);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        gridLayout_5 = new QGridLayout(dockWidgetContents_2);
        gridLayout_5->setSpacing(3);
        gridLayout_5->setContentsMargins(1, 1, 1, 1);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        groupBox = new QGroupBox(dockWidgetContents_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(9999999, 16777215));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setSpacing(6);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        laAziOffset = new QLabel(groupBox);
        laAziOffset->setObjectName(QString::fromUtf8("laAziOffset"));
        laAziOffset->setMinimumSize(QSize(81, 0));
        laAziOffset->setFrameShape(QFrame::NoFrame);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, laAziOffset);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_10->setFont(font);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_10);

        lbEleOffset = new QLabel(groupBox);
        lbEleOffset->setObjectName(QString::fromUtf8("lbEleOffset"));
        lbEleOffset->setMinimumSize(QSize(81, 0));
        lbEleOffset->setFrameShape(QFrame::NoFrame);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, lbEleOffset);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_8);


        gridLayout_4->addLayout(formLayout_3, 0, 0, 1, 1);

        formLayout_4 = new QFormLayout();
        formLayout_4->setSpacing(6);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_14);

        lbAziDiff = new QLabel(groupBox);
        lbAziDiff->setObjectName(QString::fromUtf8("lbAziDiff"));
        lbAziDiff->setMinimumSize(QSize(81, 0));
        lbAziDiff->setFrameShape(QFrame::NoFrame);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, lbAziDiff);

        label_16 = new QLabel(groupBox);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font);

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_16);

        lbEleDiff = new QLabel(groupBox);
        lbEleDiff->setObjectName(QString::fromUtf8("lbEleDiff"));
        lbEleDiff->setMinimumSize(QSize(81, 0));
        lbEleDiff->setFrameShape(QFrame::NoFrame);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, lbEleDiff);


        gridLayout_4->addLayout(formLayout_4, 0, 1, 1, 1);

        formLayout_5 = new QFormLayout();
        formLayout_5->setSpacing(6);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        formLayout_5->setWidget(0, QFormLayout::LabelRole, label_3);

        lb_aziSat = new QLabel(groupBox);
        lb_aziSat->setObjectName(QString::fromUtf8("lb_aziSat"));

        formLayout_5->setWidget(0, QFormLayout::FieldRole, lb_aziSat);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setFont(font);

        formLayout_5->setWidget(1, QFormLayout::LabelRole, label_15);

        lb_eleSat = new QLabel(groupBox);
        lb_eleSat->setObjectName(QString::fromUtf8("lb_eleSat"));

        formLayout_5->setWidget(1, QFormLayout::FieldRole, lb_eleSat);


        gridLayout_4->addLayout(formLayout_5, 1, 0, 1, 1);

        formLayout_6 = new QFormLayout();
        formLayout_6->setSpacing(6);
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label_4);

        lb_aziAnte = new QLabel(groupBox);
        lb_aziAnte->setObjectName(QString::fromUtf8("lb_aziAnte"));

        formLayout_6->setWidget(0, QFormLayout::FieldRole, lb_aziAnte);

        label_17 = new QLabel(groupBox);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setFont(font);

        formLayout_6->setWidget(1, QFormLayout::LabelRole, label_17);

        lb_eleAnte = new QLabel(groupBox);
        lb_eleAnte->setObjectName(QString::fromUtf8("lb_eleAnte"));

        formLayout_6->setWidget(1, QFormLayout::FieldRole, lb_eleAnte);


        gridLayout_4->addLayout(formLayout_6, 1, 1, 1, 1);


        gridLayout_5->addWidget(groupBox, 0, 0, 1, 1);

        pb_cancelThread = new QPushButton(dockWidgetContents_2);
        pb_cancelThread->setObjectName(QString::fromUtf8("pb_cancelThread"));
        pb_cancelThread->setMaximumSize(QSize(999999, 16777215));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/resources/Stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_cancelThread->setIcon(icon6);

        gridLayout_5->addWidget(pb_cancelThread, 0, 3, 1, 1);

        widget = new elePlot(dockWidgetContents_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(50, 164));
        widget->setMaximumSize(QSize(50, 164));
        widget->setCursor(QCursor(Qt::CrossCursor));

        gridLayout_5->addWidget(widget, 0, 5, 1, 1);

        azielePlot = new AziElePlot(dockWidgetContents_2);
        azielePlot->setObjectName(QString::fromUtf8("azielePlot"));
        azielePlot->setMinimumSize(QSize(361, 164));
        azielePlot->setMaximumSize(QSize(360, 164));
        azielePlot->setCursor(QCursor(Qt::OpenHandCursor));

        gridLayout_5->addWidget(azielePlot, 0, 6, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

        gridLayout_5->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_2, 0, 4, 1, 1);

        dockWidgetTrack->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidgetTrack);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionLoad_TLEs);
        menuFile->addAction(actionLoad_Map);
        menuView->addAction(actionSatellites);
        menuView->addAction(actionTrack);
        menuView->addAction(actionPass_Order);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionLoad_TLEs->setText(QApplication::translate("MainWindow", "Load TLEs", 0, QApplication::UnicodeUTF8));
        actionLoad_TLEs->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSatellites->setText(QApplication::translate("MainWindow", "Satellites", 0, QApplication::UnicodeUTF8));
        actionSatellites->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionTrack->setText(QApplication::translate("MainWindow", "Track", 0, QApplication::UnicodeUTF8));
        actionTrack->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0, QApplication::UnicodeUTF8));
        actionLoad_Map->setText(QApplication::translate("MainWindow", "Load Map ", 0, QApplication::UnicodeUTF8));
        actionLoad_Map->setShortcut(QApplication::translate("MainWindow", "Ctrl+M", 0, QApplication::UnicodeUTF8));
        actionPass_Order->setText(QApplication::translate("MainWindow", "Pass Order", 0, QApplication::UnicodeUTF8));
        actionPass_Order->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        lb_sat->setText(QApplication::translate("MainWindow", "Satelites", 0, QApplication::UnicodeUTF8));
        gb_satInfo->setTitle(QApplication::translate("MainWindow", "Sat Info", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Lat:", 0, QApplication::UnicodeUTF8));
        la_lat->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "Lon:", 0, QApplication::UnicodeUTF8));
        la_lon->setText(QString());
        label_7->setText(QApplication::translate("MainWindow", "Azi:", 0, QApplication::UnicodeUTF8));
        la_azi->setText(QString());
        label_9->setText(QApplication::translate("MainWindow", "Ele:", 0, QApplication::UnicodeUTF8));
        la_elev->setText(QString());
        label_12->setText(QApplication::translate("MainWindow", "Alt (km):", 0, QApplication::UnicodeUTF8));
        la_alt->setText(QString());
        label_11->setText(QApplication::translate("MainWindow", "Visible a:", 0, QApplication::UnicodeUTF8));
        la_pase->setText(QString());
        label_13->setText(QApplication::translate("MainWindow", "Period (hrs):", 0, QApplication::UnicodeUTF8));
        la_period->setText(QString());
        label_18->setText(QApplication::translate("MainWindow", "Elev. Max:", 0, QApplication::UnicodeUTF8));
        lb_elevMaxi->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "Info:", 0, QApplication::UnicodeUTF8));
        laAziOffset->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Ele offset:", 0, QApplication::UnicodeUTF8));
        lbEleOffset->setText(QString());
        label_8->setText(QApplication::translate("MainWindow", "Azi offset:", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "Azi diff:", 0, QApplication::UnicodeUTF8));
        lbAziDiff->setText(QString());
        label_16->setText(QApplication::translate("MainWindow", "Ele diff:", 0, QApplication::UnicodeUTF8));
        lbEleDiff->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Azi Satellite:", 0, QApplication::UnicodeUTF8));
        lb_aziSat->setText(QString());
        label_15->setText(QApplication::translate("MainWindow", "Ele Satellite:", 0, QApplication::UnicodeUTF8));
        lb_eleSat->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "Azi Antena:", 0, QApplication::UnicodeUTF8));
        lb_aziAnte->setText(QString());
        label_17->setText(QApplication::translate("MainWindow", "Ele Antena:", 0, QApplication::UnicodeUTF8));
        lb_eleAnte->setText(QString());
        pb_cancelThread->setText(QApplication::translate("MainWindow", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
