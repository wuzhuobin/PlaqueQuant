/********************************************************************************
** Form generated from reading UI file 'InfoDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFODIALOG_H
#define UI_INFODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *Infotag;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QFrame *line;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_8;
    QLabel *label_3;
    QLabel *label;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer;
    QLabel *label_19;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *SkipBtn;
    QPushButton *Nextp0Btn;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QFrame *line_2;
    QLabel *label_7;
    QLabel *label_6;
    QLabel *label_18;
    QLabel *label_15;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *Backp1Btn;
    QPushButton *Nextp1Btn;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_5;
    QFrame *line_3;
    QLabel *label_8;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_14;
    QLabel *label_22;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *Backp2Btn;
    QPushButton *Nextp2Btn;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_6;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_3;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_9;
    QFrame *line_4;
    QLabel *label_10;
    QLabel *label_16;
    QLabel *label_13;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *Backp3Btn;
    QPushButton *Nextp3Btn;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_8;
    QScrollArea *scrollArea_4;
    QWidget *scrollAreaWidgetContents_4;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_11;
    QFrame *line_5;
    QLabel *label_12;
    QLabel *label_17;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *FinishBtn;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(1064, 791);
        Dialog->setContextMenuPolicy(Qt::DefaultContextMenu);
        Dialog->setLayoutDirection(Qt::LeftToRight);
        Dialog->setAutoFillBackground(false);
        Dialog->setSizeGripEnabled(false);
        horizontalLayout = new QHBoxLayout(Dialog);
        horizontalLayout->setSpacing(8);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Infotag = new QTabWidget(Dialog);
        Infotag->setObjectName(QStringLiteral("Infotag"));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        Infotag->setFont(font);
        Infotag->setTabPosition(QTabWidget::North);
        Infotag->setTabShape(QTabWidget::Rounded);
        Infotag->setIconSize(QSize(20, 20));
        Infotag->setElideMode(Qt::ElideLeft);
        Infotag->setDocumentMode(false);
        Infotag->setTabsClosable(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setSpacing(30);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(26);
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_2);

        line = new QFrame(tab);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(30);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(-1, 6, -1, -1);
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_8);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(0, 0));
        label_3->setMaximumSize(QSize(850, 400));
        label_3->setTextFormat(Qt::AutoText);
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/icons/Infop0.png")));
        label_3->setScaledContents(true);
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(label_3);

        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(400, 0));
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        font2.setPointSize(11);
        label->setFont(font2);
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(true);

        horizontalLayout_10->addWidget(label);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_7);


        verticalLayout_3->addLayout(horizontalLayout_10);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        label_19 = new QLabel(tab);
        label_19->setObjectName(QStringLiteral("label_19"));

        verticalLayout_3->addWidget(label_19);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        SkipBtn = new QPushButton(tab);
        SkipBtn->setObjectName(QStringLiteral("SkipBtn"));
        SkipBtn->setFont(font);

        horizontalLayout_2->addWidget(SkipBtn);

        Nextp0Btn = new QPushButton(tab);
        Nextp0Btn->setObjectName(QStringLiteral("Nextp0Btn"));
        Nextp0Btn->setFont(font);

        horizontalLayout_2->addWidget(Nextp0Btn);


        verticalLayout_3->addLayout(horizontalLayout_2);

        Infotag->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout = new QVBoxLayout(tab_2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        scrollArea = new QScrollArea(tab_2);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(false);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1592, 1432));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setSpacing(30);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QStringLiteral("label_4"));
        QFont font3;
        font3.setFamily(QStringLiteral("Arial"));
        font3.setPointSize(19);
        label_4->setFont(font3);
        label_4->setStyleSheet(QStringLiteral(" QLabel { color: rgb(8, 0, 140) }"));

        verticalLayout_2->addWidget(label_4);

        line_2 = new QFrame(scrollAreaWidgetContents);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_2);

        label_7 = new QLabel(scrollAreaWidgetContents);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMinimumSize(QSize(0, 0));
        QFont font4;
        font4.setFamily(QStringLiteral("Arial"));
        font4.setPointSize(11);
        font4.setKerning(true);
        label_7->setFont(font4);
        label_7->setMouseTracking(true);
        label_7->setScaledContents(false);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_7->setWordWrap(true);

        verticalLayout_2->addWidget(label_7);

        label_6 = new QLabel(scrollAreaWidgetContents);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMaximumSize(QSize(830, 500));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/icons/Infop1.png")));
        label_6->setScaledContents(true);

        verticalLayout_2->addWidget(label_6);

        label_18 = new QLabel(scrollAreaWidgetContents);
        label_18->setObjectName(QStringLiteral("label_18"));
        QFont font5;
        font5.setPointSize(11);
        label_18->setFont(font5);

        verticalLayout_2->addWidget(label_18);

        label_15 = new QLabel(scrollAreaWidgetContents);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setMaximumSize(QSize(700, 500));
        label_15->setPixmap(QPixmap(QString::fromUtf8(":/icons/Infop2.png")));
        label_15->setScaledContents(true);

        verticalLayout_2->addWidget(label_15);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        Backp1Btn = new QPushButton(tab_2);
        Backp1Btn->setObjectName(QStringLiteral("Backp1Btn"));

        horizontalLayout_3->addWidget(Backp1Btn);

        Nextp1Btn = new QPushButton(tab_2);
        Nextp1Btn->setObjectName(QStringLiteral("Nextp1Btn"));

        horizontalLayout_3->addWidget(Nextp1Btn);


        verticalLayout->addLayout(horizontalLayout_3);

        Infotag->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        scrollArea_2 = new QScrollArea(tab_3);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_2->setWidgetResizable(false);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 1266, 821));
        verticalLayout_5 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_5->setSpacing(30);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_5 = new QLabel(scrollAreaWidgetContents_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font3);
        label_5->setStyleSheet(QStringLiteral(" QLabel { color: rgb(8, 0, 140) }"));

        verticalLayout_5->addWidget(label_5);

        line_3 = new QFrame(scrollAreaWidgetContents_2);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(line_3);

        label_8 = new QLabel(scrollAreaWidgetContents_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font5);
        label_8->setScaledContents(false);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_8->setWordWrap(true);

        verticalLayout_5->addWidget(label_8);

        label_20 = new QLabel(scrollAreaWidgetContents_2);
        label_20->setObjectName(QStringLiteral("label_20"));
        QFont font6;
        font6.setFamily(QStringLiteral("Arial"));
        font6.setPointSize(13);
        label_20->setFont(font6);
        label_20->setStyleSheet(QStringLiteral(" QLabel { color: rgb(8, 0, 140) }"));

        verticalLayout_5->addWidget(label_20);

        label_21 = new QLabel(scrollAreaWidgetContents_2);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setMaximumSize(QSize(16777215, 16777215));
        label_21->setFont(font5);
        label_21->setTextFormat(Qt::AutoText);
        label_21->setScaledContents(false);

        verticalLayout_5->addWidget(label_21);

        label_14 = new QLabel(scrollAreaWidgetContents_2);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setMaximumSize(QSize(830, 500));
        label_14->setPixmap(QPixmap(QString::fromUtf8(":/icons/Infop3.png")));
        label_14->setScaledContents(true);

        verticalLayout_5->addWidget(label_14);

        label_22 = new QLabel(scrollAreaWidgetContents_2);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setFont(font5);

        verticalLayout_5->addWidget(label_22);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_4->addWidget(scrollArea_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        Backp2Btn = new QPushButton(tab_3);
        Backp2Btn->setObjectName(QStringLiteral("Backp2Btn"));

        horizontalLayout_4->addWidget(Backp2Btn);

        Nextp2Btn = new QPushButton(tab_3);
        Nextp2Btn->setObjectName(QStringLiteral("Nextp2Btn"));

        horizontalLayout_4->addWidget(Nextp2Btn);


        verticalLayout_4->addLayout(horizontalLayout_4);

        Infotag->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        verticalLayout_6 = new QVBoxLayout(tab_4);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        scrollArea_3 = new QScrollArea(tab_4);
        scrollArea_3->setObjectName(QStringLiteral("scrollArea_3"));
        scrollArea_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_3->setWidgetResizable(false);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QStringLiteral("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 991, 745));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents_3);
        verticalLayout_7->setSpacing(30);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_9 = new QLabel(scrollAreaWidgetContents_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font3);
        label_9->setStyleSheet(QStringLiteral(" QLabel { color: rgb(8, 0, 140) }"));

        verticalLayout_7->addWidget(label_9);

        line_4 = new QFrame(scrollAreaWidgetContents_3);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_7->addWidget(line_4);

        label_10 = new QLabel(scrollAreaWidgetContents_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font5);
        label_10->setScaledContents(false);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_10->setWordWrap(true);

        verticalLayout_7->addWidget(label_10);

        label_16 = new QLabel(scrollAreaWidgetContents_3);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setMaximumSize(QSize(830, 500));
        label_16->setPixmap(QPixmap(QString::fromUtf8(":/icons/Infop4.png")));
        label_16->setScaledContents(true);

        verticalLayout_7->addWidget(label_16);

        label_13 = new QLabel(scrollAreaWidgetContents_3);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setFont(font5);
        label_13->setScaledContents(false);
        label_13->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_13->setWordWrap(true);

        verticalLayout_7->addWidget(label_13);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_6->addWidget(scrollArea_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        Backp3Btn = new QPushButton(tab_4);
        Backp3Btn->setObjectName(QStringLiteral("Backp3Btn"));

        horizontalLayout_5->addWidget(Backp3Btn);

        Nextp3Btn = new QPushButton(tab_4);
        Nextp3Btn->setObjectName(QStringLiteral("Nextp3Btn"));

        horizontalLayout_5->addWidget(Nextp3Btn);


        verticalLayout_6->addLayout(horizontalLayout_5);

        Infotag->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        verticalLayout_8 = new QVBoxLayout(tab_5);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        scrollArea_4 = new QScrollArea(tab_5);
        scrollArea_4->setObjectName(QStringLiteral("scrollArea_4"));
        scrollArea_4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_4->setWidgetResizable(false);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QStringLiteral("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 1012, 674));
        verticalLayout_9 = new QVBoxLayout(scrollAreaWidgetContents_4);
        verticalLayout_9->setSpacing(30);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        label_11 = new QLabel(scrollAreaWidgetContents_4);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setFont(font3);
        label_11->setStyleSheet(QStringLiteral(" QLabel { color: rgb(8, 0, 140) }"));

        verticalLayout_9->addWidget(label_11);

        line_5 = new QFrame(scrollAreaWidgetContents_4);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_9->addWidget(line_5);

        label_12 = new QLabel(scrollAreaWidgetContents_4);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setFont(font5);
        label_12->setScaledContents(false);
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_12->setWordWrap(true);

        verticalLayout_9->addWidget(label_12);

        label_17 = new QLabel(scrollAreaWidgetContents_4);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setMaximumSize(QSize(830, 500));
        label_17->setPixmap(QPixmap(QString::fromUtf8(":/icons/Infop5.png")));
        label_17->setScaledContents(true);

        verticalLayout_9->addWidget(label_17);

        scrollArea_4->setWidget(scrollAreaWidgetContents_4);

        verticalLayout_8->addWidget(scrollArea_4);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);

        FinishBtn = new QPushButton(tab_5);
        FinishBtn->setObjectName(QStringLiteral("FinishBtn"));
        QFont font7;
        font7.setPointSize(12);
        FinishBtn->setFont(font7);
        FinishBtn->setIconSize(QSize(20, 20));

        horizontalLayout_8->addWidget(FinishBtn);


        verticalLayout_8->addLayout(horizontalLayout_8);

        Infotag->addTab(tab_5, QString());

        horizontalLayout->addWidget(Infotag);


        retranslateUi(Dialog);

        Infotag->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Welcome", 0));
        label_2->setText(QApplication::translate("Dialog", "Welcome", 0));
        label_3->setText(QString());
        label->setText(QApplication::translate("Dialog", "<html><head/><body><p>This research program is a cross platform software package that enables you to visualize MRI data. It is used to facilitate user to identify the lesions inside the brain. Users can use the Contour Widget to select the Region of Interest (ROI) from multiple MRI data. Other parts which are similair to the ROI can also be highlighted given different weighting parameters in different modalities. Click on <span style=\" font-weight:600;\">Next</span> to see the procedures or <span style=\" font-weight:600;\">Getting Started</span> to try the software.</p></body></html>", 0));
        label_19->setText(QApplication::translate("Dialog", "Noted: This software is for research only.", 0));
        SkipBtn->setText(QApplication::translate("Dialog", "Getting Started", 0));
        Nextp0Btn->setText(QApplication::translate("Dialog", "Next", 0));
        Infotag->setTabText(Infotag->indexOf(tab), QApplication::translate("Dialog", "Welcoming Page", 0));
        label_4->setText(QApplication::translate("Dialog", "Step 1", 0));
        label_7->setText(QApplication::translate("Dialog", "<html><head/><body><p>Select <span style=\" font-weight:600;\">open image</span> or <span style=\" font-weight:600;\">open recent image </span>to visualize multiple dicom images. </p><p>You can select up to <span style=\" font-weight:600;\">four </span>images from different modalities</p></body></html>", 0));
        label_6->setText(QString());
        label_18->setText(QApplication::translate("Dialog", "<html><head/><body><p>Follow the instructions on the window\357\274\214 The registration wizard will ask you to select input dicom images series</p><p> to be displayed in the window. </p><p>Check the header and information to see if it is correct.</p></body></html>", 0));
        label_15->setText(QString());
        Backp1Btn->setText(QApplication::translate("Dialog", "Back", 0));
        Nextp1Btn->setText(QApplication::translate("Dialog", "Next", 0));
        Infotag->setTabText(Infotag->indexOf(tab_2), QApplication::translate("Dialog", "Step 1", 0));
        label_5->setText(QApplication::translate("Dialog", "Step 2", 0));
        label_8->setText(QApplication::translate("Dialog", "After visualize the images, you can use different tools such as cursor, ruler, portractor to play with the images", 0));
        label_20->setText(QApplication::translate("Dialog", "Graphical User Interface", 0));
        label_21->setText(QApplication::translate("Dialog", "<html><head/><body><p>The figure below shows the <span style=\" font-weight:600;\">graphical user interface (GUI)</span> to the user. </p><p>The central part contains a workspace area for multiple dicom images</p></body></html>", 0));
        label_14->setText(QString());
        label_22->setText(QApplication::translate("Dialog", "<html><head/><body><p>On the left is the <span style=\" font-weight:600;\">display panel</span> which shows the cursor position, intensity under the cursor and window and level setting. </p><p>Also you can change the orientation by clicking the buttons in orientation. </p><p>The main toolbar provide access to basic functionality such as </p></body></html>", 0));
        Backp2Btn->setText(QApplication::translate("Dialog", "Back", 0));
        Nextp2Btn->setText(QApplication::translate("Dialog", "Next", 0));
        Infotag->setTabText(Infotag->indexOf(tab_3), QApplication::translate("Dialog", "Step 2", 0));
        label_9->setText(QApplication::translate("Dialog", "Step 3", 0));
        label_10->setText(QApplication::translate("Dialog", "You can use Contour tool to select ROI in one of the window.  You can at anytime zoom in to one of the window by clicking the top left hand display window icon.", 0));
        label_16->setText(QString());
        label_13->setText(QApplication::translate("Dialog", "<html><head/><body><p>The right panel includes all the <span style=\" font-weight:600;\">parameters </span>you need to set before finding similiar region inside the brain.</p><p>The threshold means the similarity setting to the ROI. The larger the threshold, the more likely you will include unrelated target. However, if the parameters are getting too small, it is likely that you won't get anyting that similiar to the selected area. </p><p>The parameters can be saved which can be used later. You have to click load everytime you select a parameter set. </p></body></html>", 0));
        Backp3Btn->setText(QApplication::translate("Dialog", "Back", 0));
        Nextp3Btn->setText(QApplication::translate("Dialog", "Next", 0));
        Infotag->setTabText(Infotag->indexOf(tab_4), QApplication::translate("Dialog", "Step 3", 0));
        label_11->setText(QApplication::translate("Dialog", "Step 4", 0));
        label_12->setText(QApplication::translate("Dialog", "<html><head/><body><p>After you clicked the button <span style=\" font-weight:600;\">Creat Overlay,</span> overlay will be drawn on the window which highlight some areas which is very closed to the ROI you select in terms of intensity</p></body></html>", 0));
        label_17->setText(QString());
        FinishBtn->setText(QApplication::translate("Dialog", "Getting Started!", 0));
        Infotag->setTabText(Infotag->indexOf(tab_5), QApplication::translate("Dialog", "Step 4", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFODIALOG_H
