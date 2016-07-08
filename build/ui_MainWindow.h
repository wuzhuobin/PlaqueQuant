/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpenImage;
    QAction *actionExit;
    QAction *actionSave;
    QAction *actionAbout;
    QAction *actionHelp;
    QAction *actionRuler;
    QAction *actionWindowLevel;
    QAction *actionNavigation;
    QAction *actionFourViews;
    QAction *actionInformation;
    QAction *actionAddOverlay;
    QAction *actionRemoveOverlay;
    QAction *actionOverlayVisible;
    QAction *actionIncreaseOverlayOpacity;
    QAction *actionDecreaseOverlayOpacity;
    QAction *actionContour;
    QAction *actionCloseImage;
    QAction *actionImage1;
    QAction *actionImage2;
    QAction *actionImage3;
    QAction *actionImage4;
    QAction *actionSagittal;
    QAction *actionCoronal;
    QAction *actionAllAxialView;
    QAction *actionRemoveContour;
    QAction *actionROI;
    QAction *actionStatistics;
    QAction *actionBrush;
    QAction *actionMultiPlanarView;
    QAction *actionOpenSegmentation;
    QAction *actionCenterlineAlgorithm;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *viewGridLayout;
    QFrame *image3frame;
    QHBoxLayout *horizontalLayout_4;
    QVTKWidget *image3View;
    QVBoxLayout *verticalLayout_3;
    QPushButton *LLBtn;
    QPushButton *LLBtn2;
    QSpacerItem *verticalSpacer_2;
    QPushButton *LLSelectImgBtn;
    QFrame *image4frame;
    QHBoxLayout *horizontalLayout_6;
    QVTKWidget *image4View;
    QVBoxLayout *verticalLayout_9;
    QPushButton *LRBtn;
    QPushButton *LRBtn2;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QWidget *xyzwidget;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label;
    QSpinBox *xSpinBox;
    QLabel *label_2;
    QSpinBox *ySpinBox;
    QLabel *label_3;
    QSpinBox *zSpinBox;
    QWidget *windowlevelwidget;
    QGridLayout *gridLayout;
    QDoubleSpinBox *windowDoubleSpinBoxUR;
    QDoubleSpinBox *windowDoubleSpinBoxLR;
    QDoubleSpinBox *levelDoubleSpinBoxLL;
    QDoubleSpinBox *levelDoubleSpinBoxUR;
    QDoubleSpinBox *windowDoubleSpinBoxUL;
    QDoubleSpinBox *levelDoubleSpinBoxUL;
    QDoubleSpinBox *windowDoubleSpinBoxLL;
    QDoubleSpinBox *levelDoubleSpinBoxLR;
    QDoubleSpinBox *doubleSpinBox;
    QDoubleSpinBox *doubleSpinBox_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *updateBtn;
    QPushButton *pushButton;
    QFrame *image1frame;
    QHBoxLayout *horizontalLayout;
    QVTKWidget *image1View;
    QVBoxLayout *verticalLayout_2;
    QPushButton *ULBtn;
    QPushButton *ULBtn2;
    QSpacerItem *verticalSpacer;
    QPushButton *ULSelectImgBtn;
    QFrame *image2frame;
    QHBoxLayout *horizontalLayout_5;
    QVTKWidget *image2View;
    QVBoxLayout *verticalLayout_8;
    QPushButton *URBtn;
    QPushButton *URBtn2;
    QSpacerItem *verticalSpacer_3;
    QPushButton *URSelectImgBtn;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuRecentImage;
    QMenu *menuHelp;
    QMenu *menuInteraction;
    QMenu *menuView;
    QMenu *menuDisplay;
    QMenu *menuOrientation;
    QMenu *menuSegmentation;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QToolBar *ActionToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::WindowModal);
        MainWindow->resize(1160, 966);
        MainWindow->setAcceptDrops(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/plaqueQuant.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::VerticalTabs);
        actionOpenImage = new QAction(MainWindow);
        actionOpenImage->setObjectName(QStringLiteral("actionOpenImage"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/OpenImage.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenImage->setIcon(icon1);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/Save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon2);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionRuler = new QAction(MainWindow);
        actionRuler->setObjectName(QStringLiteral("actionRuler"));
        actionRuler->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/Ruler.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRuler->setIcon(icon3);
        actionWindowLevel = new QAction(MainWindow);
        actionWindowLevel->setObjectName(QStringLiteral("actionWindowLevel"));
        actionWindowLevel->setCheckable(true);
        actionWindowLevel->setChecked(false);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/WindowLevel.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWindowLevel->setIcon(icon4);
        actionNavigation = new QAction(MainWindow);
        actionNavigation->setObjectName(QStringLiteral("actionNavigation"));
        actionNavigation->setCheckable(true);
        actionNavigation->setChecked(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/Navigation.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNavigation->setIcon(icon5);
        actionFourViews = new QAction(MainWindow);
        actionFourViews->setObjectName(QStringLiteral("actionFourViews"));
        actionFourViews->setCheckable(true);
        actionFourViews->setChecked(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/LayoutFourViews.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFourViews->setIcon(icon6);
        actionInformation = new QAction(MainWindow);
        actionInformation->setObjectName(QStringLiteral("actionInformation"));
        actionAddOverlay = new QAction(MainWindow);
        actionAddOverlay->setObjectName(QStringLiteral("actionAddOverlay"));
        actionRemoveOverlay = new QAction(MainWindow);
        actionRemoveOverlay->setObjectName(QStringLiteral("actionRemoveOverlay"));
        actionOverlayVisible = new QAction(MainWindow);
        actionOverlayVisible->setObjectName(QStringLiteral("actionOverlayVisible"));
        actionOverlayVisible->setCheckable(true);
        actionIncreaseOverlayOpacity = new QAction(MainWindow);
        actionIncreaseOverlayOpacity->setObjectName(QStringLiteral("actionIncreaseOverlayOpacity"));
        actionDecreaseOverlayOpacity = new QAction(MainWindow);
        actionDecreaseOverlayOpacity->setObjectName(QStringLiteral("actionDecreaseOverlayOpacity"));
        actionContour = new QAction(MainWindow);
        actionContour->setObjectName(QStringLiteral("actionContour"));
        actionContour->setCheckable(true);
        actionContour->setChecked(false);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/contour.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionContour->setIcon(icon7);
        actionCloseImage = new QAction(MainWindow);
        actionCloseImage->setObjectName(QStringLiteral("actionCloseImage"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCloseImage->setIcon(icon8);
        QFont font;
        actionCloseImage->setFont(font);
        actionImage1 = new QAction(MainWindow);
        actionImage1->setObjectName(QStringLiteral("actionImage1"));
        actionImage1->setCheckable(true);
        actionImage1->setChecked(false);
        actionImage1->setEnabled(true);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/UpperLeftView.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImage1->setIcon(icon9);
        actionImage1->setVisible(true);
        actionImage1->setIconVisibleInMenu(true);
        actionImage2 = new QAction(MainWindow);
        actionImage2->setObjectName(QStringLiteral("actionImage2"));
        actionImage2->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/UpperRightView.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImage2->setIcon(icon10);
        actionImage3 = new QAction(MainWindow);
        actionImage3->setObjectName(QStringLiteral("actionImage3"));
        actionImage3->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/LowerLeftView.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImage3->setIcon(icon11);
        actionImage4 = new QAction(MainWindow);
        actionImage4->setObjectName(QStringLiteral("actionImage4"));
        actionImage4->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icons/LowerRightView.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImage4->setIcon(icon12);
        actionSagittal = new QAction(MainWindow);
        actionSagittal->setObjectName(QStringLiteral("actionSagittal"));
        actionSagittal->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/icons/dl_sagittal.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSagittal->setIcon(icon13);
        actionSagittal->setFont(font);
        actionSagittal->setVisible(true);
        actionSagittal->setIconVisibleInMenu(true);
        actionCoronal = new QAction(MainWindow);
        actionCoronal->setObjectName(QStringLiteral("actionCoronal"));
        actionCoronal->setCheckable(true);
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/icons/dl_coronal.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCoronal->setIcon(icon14);
        actionAllAxialView = new QAction(MainWindow);
        actionAllAxialView->setObjectName(QStringLiteral("actionAllAxialView"));
        actionAllAxialView->setCheckable(true);
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/icons/dl_axial copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAllAxialView->setIcon(icon15);
        actionRemoveContour = new QAction(MainWindow);
        actionRemoveContour->setObjectName(QStringLiteral("actionRemoveContour"));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/icons/magicdrawdelete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemoveContour->setIcon(icon16);
        actionROI = new QAction(MainWindow);
        actionROI->setObjectName(QStringLiteral("actionROI"));
        actionROI->setCheckable(true);
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/icons/roi.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionROI->setIcon(icon17);
        actionStatistics = new QAction(MainWindow);
        actionStatistics->setObjectName(QStringLiteral("actionStatistics"));
        actionBrush = new QAction(MainWindow);
        actionBrush->setObjectName(QStringLiteral("actionBrush"));
        actionBrush->setCheckable(true);
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/icons/Draw.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBrush->setIcon(icon18);
        actionMultiPlanarView = new QAction(MainWindow);
        actionMultiPlanarView->setObjectName(QStringLiteral("actionMultiPlanarView"));
        actionMultiPlanarView->setCheckable(true);
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/icons/LayoutMultiPlanar.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMultiPlanarView->setIcon(icon19);
        actionOpenSegmentation = new QAction(MainWindow);
        actionOpenSegmentation->setObjectName(QStringLiteral("actionOpenSegmentation"));
        actionCenterlineAlgorithm = new QAction(MainWindow);
        actionCenterlineAlgorithm->setObjectName(QStringLiteral("actionCenterlineAlgorithm"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout_3 = new QHBoxLayout(centralwidget);
        horizontalLayout_3->setSpacing(7);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, 0, 4, 0);
        viewGridLayout = new QGridLayout();
        viewGridLayout->setSpacing(1);
        viewGridLayout->setObjectName(QStringLiteral("viewGridLayout"));
        image3frame = new QFrame(centralwidget);
        image3frame->setObjectName(QStringLiteral("image3frame"));
        image3frame->setFrameShape(QFrame::StyledPanel);
        image3frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(image3frame);
        horizontalLayout_4->setSpacing(1);
        horizontalLayout_4->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        image3View = new QVTKWidget(image3frame);
        image3View->setObjectName(QStringLiteral("image3View"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(image3View->sizePolicy().hasHeightForWidth());
        image3View->setSizePolicy(sizePolicy);
        image3View->setMinimumSize(QSize(350, 350));

        horizontalLayout_4->addWidget(image3View);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(4);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        LLBtn = new QPushButton(image3frame);
        LLBtn->setObjectName(QStringLiteral("LLBtn"));
        LLBtn->setMaximumSize(QSize(20, 20));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/icons/expandwin.png"), QSize(), QIcon::Normal, QIcon::Off);
        LLBtn->setIcon(icon20);
        LLBtn->setIconSize(QSize(15, 15));
        LLBtn->setFlat(false);

        verticalLayout_3->addWidget(LLBtn);

        LLBtn2 = new QPushButton(image3frame);
        LLBtn2->setObjectName(QStringLiteral("LLBtn2"));
        LLBtn2->setMaximumSize(QSize(20, 20));
        LLBtn2->setIcon(icon6);
        LLBtn2->setIconSize(QSize(15, 15));

        verticalLayout_3->addWidget(LLBtn2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        LLSelectImgBtn = new QPushButton(image3frame);
        LLSelectImgBtn->setObjectName(QStringLiteral("LLSelectImgBtn"));
        LLSelectImgBtn->setMaximumSize(QSize(20, 20));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/icons/downarrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        LLSelectImgBtn->setIcon(icon21);
        LLSelectImgBtn->setIconSize(QSize(15, 15));

        verticalLayout_3->addWidget(LLSelectImgBtn);


        horizontalLayout_4->addLayout(verticalLayout_3);


        viewGridLayout->addWidget(image3frame, 6, 0, 1, 1);

        image4frame = new QFrame(centralwidget);
        image4frame->setObjectName(QStringLiteral("image4frame"));
        image4frame->setFrameShape(QFrame::StyledPanel);
        image4frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(image4frame);
        horizontalLayout_6->setSpacing(1);
        horizontalLayout_6->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        image4View = new QVTKWidget(image4frame);
        image4View->setObjectName(QStringLiteral("image4View"));
        sizePolicy.setHeightForWidth(image4View->sizePolicy().hasHeightForWidth());
        image4View->setSizePolicy(sizePolicy);
        image4View->setMinimumSize(QSize(350, 350));

        horizontalLayout_6->addWidget(image4View);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(4);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        LRBtn = new QPushButton(image4frame);
        LRBtn->setObjectName(QStringLiteral("LRBtn"));
        LRBtn->setMaximumSize(QSize(20, 20));
        LRBtn->setIcon(icon20);
        LRBtn->setIconSize(QSize(15, 15));
        LRBtn->setFlat(false);

        verticalLayout_9->addWidget(LRBtn);

        LRBtn2 = new QPushButton(image4frame);
        LRBtn2->setObjectName(QStringLiteral("LRBtn2"));
        LRBtn2->setMaximumSize(QSize(20, 20));
        LRBtn2->setIcon(icon6);
        LRBtn2->setIconSize(QSize(15, 15));

        verticalLayout_9->addWidget(LRBtn2);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_4);


        horizontalLayout_6->addLayout(verticalLayout_9);


        viewGridLayout->addWidget(image4frame, 6, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        xyzwidget = new QWidget(centralwidget);
        xyzwidget->setObjectName(QStringLiteral("xyzwidget"));
        horizontalLayout_7 = new QHBoxLayout(xyzwidget);
        horizontalLayout_7->setSpacing(7);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(6, 6, 3, 3);
        label = new QLabel(xyzwidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_7->addWidget(label);

        xSpinBox = new QSpinBox(xyzwidget);
        xSpinBox->setObjectName(QStringLiteral("xSpinBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(xSpinBox->sizePolicy().hasHeightForWidth());
        xSpinBox->setSizePolicy(sizePolicy1);
        xSpinBox->setMaximum(9999);

        horizontalLayout_7->addWidget(xSpinBox);

        label_2 = new QLabel(xyzwidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_7->addWidget(label_2);

        ySpinBox = new QSpinBox(xyzwidget);
        ySpinBox->setObjectName(QStringLiteral("ySpinBox"));
        sizePolicy1.setHeightForWidth(ySpinBox->sizePolicy().hasHeightForWidth());
        ySpinBox->setSizePolicy(sizePolicy1);
        ySpinBox->setMaximum(999);

        horizontalLayout_7->addWidget(ySpinBox);

        label_3 = new QLabel(xyzwidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_7->addWidget(label_3);

        zSpinBox = new QSpinBox(xyzwidget);
        zSpinBox->setObjectName(QStringLiteral("zSpinBox"));
        sizePolicy1.setHeightForWidth(zSpinBox->sizePolicy().hasHeightForWidth());
        zSpinBox->setSizePolicy(sizePolicy1);
        zSpinBox->setMaximum(999);

        horizontalLayout_7->addWidget(zSpinBox);

        windowlevelwidget = new QWidget(xyzwidget);
        windowlevelwidget->setObjectName(QStringLiteral("windowlevelwidget"));
        windowlevelwidget->setEnabled(true);
        windowlevelwidget->setMinimumSize(QSize(0, 0));
        windowlevelwidget->setMaximumSize(QSize(16777215, 20));
        gridLayout = new QGridLayout(windowlevelwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        windowDoubleSpinBoxUR = new QDoubleSpinBox(windowlevelwidget);
        windowDoubleSpinBoxUR->setObjectName(QStringLiteral("windowDoubleSpinBoxUR"));
        windowDoubleSpinBoxUR->setMaximumSize(QSize(51, 100));
        windowDoubleSpinBoxUR->setMaximum(9999);

        gridLayout->addWidget(windowDoubleSpinBoxUR, 0, 2, 1, 1);

        windowDoubleSpinBoxLR = new QDoubleSpinBox(windowlevelwidget);
        windowDoubleSpinBoxLR->setObjectName(QStringLiteral("windowDoubleSpinBoxLR"));
        windowDoubleSpinBoxLR->setMaximumSize(QSize(51, 100));
        windowDoubleSpinBoxLR->setMaximum(9999);

        gridLayout->addWidget(windowDoubleSpinBoxLR, 1, 2, 1, 1);

        levelDoubleSpinBoxLL = new QDoubleSpinBox(windowlevelwidget);
        levelDoubleSpinBoxLL->setObjectName(QStringLiteral("levelDoubleSpinBoxLL"));
        levelDoubleSpinBoxLL->setMaximumSize(QSize(51, 100));
        levelDoubleSpinBoxLL->setMaximum(9999);

        gridLayout->addWidget(levelDoubleSpinBoxLL, 1, 1, 1, 1);

        levelDoubleSpinBoxUR = new QDoubleSpinBox(windowlevelwidget);
        levelDoubleSpinBoxUR->setObjectName(QStringLiteral("levelDoubleSpinBoxUR"));
        levelDoubleSpinBoxUR->setMaximumSize(QSize(51, 100));
        levelDoubleSpinBoxUR->setMaximum(9999);

        gridLayout->addWidget(levelDoubleSpinBoxUR, 0, 3, 1, 1);

        windowDoubleSpinBoxUL = new QDoubleSpinBox(windowlevelwidget);
        windowDoubleSpinBoxUL->setObjectName(QStringLiteral("windowDoubleSpinBoxUL"));
        windowDoubleSpinBoxUL->setMaximumSize(QSize(51, 100));
        windowDoubleSpinBoxUL->setMaximum(9999);

        gridLayout->addWidget(windowDoubleSpinBoxUL, 0, 0, 1, 1);

        levelDoubleSpinBoxUL = new QDoubleSpinBox(windowlevelwidget);
        levelDoubleSpinBoxUL->setObjectName(QStringLiteral("levelDoubleSpinBoxUL"));
        levelDoubleSpinBoxUL->setMaximumSize(QSize(51, 100));
        levelDoubleSpinBoxUL->setMaximum(9999);

        gridLayout->addWidget(levelDoubleSpinBoxUL, 0, 1, 1, 1);

        windowDoubleSpinBoxLL = new QDoubleSpinBox(windowlevelwidget);
        windowDoubleSpinBoxLL->setObjectName(QStringLiteral("windowDoubleSpinBoxLL"));
        windowDoubleSpinBoxLL->setMaximumSize(QSize(51, 100));
        windowDoubleSpinBoxLL->setMaximum(9999);

        gridLayout->addWidget(windowDoubleSpinBoxLL, 1, 0, 1, 1);

        levelDoubleSpinBoxLR = new QDoubleSpinBox(windowlevelwidget);
        levelDoubleSpinBoxLR->setObjectName(QStringLiteral("levelDoubleSpinBoxLR"));
        levelDoubleSpinBoxLR->setMaximumSize(QSize(51, 100));
        levelDoubleSpinBoxLR->setMaximum(9999);

        gridLayout->addWidget(levelDoubleSpinBoxLR, 1, 3, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(windowlevelwidget);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setMaximumSize(QSize(51, 100));

        gridLayout->addWidget(doubleSpinBox, 0, 4, 1, 1);

        doubleSpinBox_2 = new QDoubleSpinBox(windowlevelwidget);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
        doubleSpinBox_2->setMaximumSize(QSize(51, 100));

        gridLayout->addWidget(doubleSpinBox_2, 1, 4, 1, 1);


        horizontalLayout_7->addWidget(windowlevelwidget);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        updateBtn = new QPushButton(xyzwidget);
        updateBtn->setObjectName(QStringLiteral("updateBtn"));

        horizontalLayout_7->addWidget(updateBtn);

        pushButton = new QPushButton(xyzwidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_7->addWidget(pushButton);


        horizontalLayout_2->addWidget(xyzwidget);


        viewGridLayout->addLayout(horizontalLayout_2, 8, 0, 1, 4);

        image1frame = new QFrame(centralwidget);
        image1frame->setObjectName(QStringLiteral("image1frame"));
        image1frame->setFrameShape(QFrame::StyledPanel);
        image1frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(image1frame);
        horizontalLayout->setSpacing(1);
        horizontalLayout->setContentsMargins(1, 1, 1, 1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        image1View = new QVTKWidget(image1frame);
        image1View->setObjectName(QStringLiteral("image1View"));
        sizePolicy.setHeightForWidth(image1View->sizePolicy().hasHeightForWidth());
        image1View->setSizePolicy(sizePolicy);
        image1View->setMinimumSize(QSize(350, 350));

        horizontalLayout->addWidget(image1View);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(4);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        ULBtn = new QPushButton(image1frame);
        ULBtn->setObjectName(QStringLiteral("ULBtn"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ULBtn->sizePolicy().hasHeightForWidth());
        ULBtn->setSizePolicy(sizePolicy2);
        ULBtn->setMaximumSize(QSize(20, 20));
        QFont font1;
        font1.setPointSize(4);
        ULBtn->setFont(font1);
        ULBtn->setLayoutDirection(Qt::LeftToRight);
        ULBtn->setIcon(icon20);
        ULBtn->setIconSize(QSize(15, 15));
        ULBtn->setCheckable(false);
        ULBtn->setAutoRepeat(false);
        ULBtn->setAutoExclusive(false);
        ULBtn->setAutoDefault(false);
        ULBtn->setFlat(false);

        verticalLayout_2->addWidget(ULBtn);

        ULBtn2 = new QPushButton(image1frame);
        ULBtn2->setObjectName(QStringLiteral("ULBtn2"));
        sizePolicy2.setHeightForWidth(ULBtn2->sizePolicy().hasHeightForWidth());
        ULBtn2->setSizePolicy(sizePolicy2);
        ULBtn2->setMaximumSize(QSize(20, 20));
        ULBtn2->setIcon(icon6);
        ULBtn2->setIconSize(QSize(15, 15));

        verticalLayout_2->addWidget(ULBtn2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        ULSelectImgBtn = new QPushButton(image1frame);
        ULSelectImgBtn->setObjectName(QStringLiteral("ULSelectImgBtn"));
        ULSelectImgBtn->setMaximumSize(QSize(20, 20));
        ULSelectImgBtn->setIcon(icon21);
        ULSelectImgBtn->setIconSize(QSize(15, 15));

        verticalLayout_2->addWidget(ULSelectImgBtn);


        horizontalLayout->addLayout(verticalLayout_2);


        viewGridLayout->addWidget(image1frame, 3, 0, 1, 1);

        image2frame = new QFrame(centralwidget);
        image2frame->setObjectName(QStringLiteral("image2frame"));
        image2frame->setFrameShape(QFrame::StyledPanel);
        image2frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(image2frame);
        horizontalLayout_5->setSpacing(1);
        horizontalLayout_5->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        image2View = new QVTKWidget(image2frame);
        image2View->setObjectName(QStringLiteral("image2View"));
        sizePolicy.setHeightForWidth(image2View->sizePolicy().hasHeightForWidth());
        image2View->setSizePolicy(sizePolicy);
        image2View->setMinimumSize(QSize(350, 350));

        horizontalLayout_5->addWidget(image2View);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(4);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        URBtn = new QPushButton(image2frame);
        URBtn->setObjectName(QStringLiteral("URBtn"));
        URBtn->setMaximumSize(QSize(20, 20));
        URBtn->setIcon(icon20);
        URBtn->setIconSize(QSize(15, 15));
        URBtn->setFlat(false);

        verticalLayout_8->addWidget(URBtn);

        URBtn2 = new QPushButton(image2frame);
        URBtn2->setObjectName(QStringLiteral("URBtn2"));
        URBtn2->setMaximumSize(QSize(20, 20));
        URBtn2->setIcon(icon6);
        URBtn2->setIconSize(QSize(15, 15));

        verticalLayout_8->addWidget(URBtn2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_3);

        URSelectImgBtn = new QPushButton(image2frame);
        URSelectImgBtn->setObjectName(QStringLiteral("URSelectImgBtn"));
        URSelectImgBtn->setMaximumSize(QSize(20, 20));
        URSelectImgBtn->setIcon(icon21);
        URSelectImgBtn->setIconSize(QSize(15, 15));

        verticalLayout_8->addWidget(URSelectImgBtn);


        horizontalLayout_5->addLayout(verticalLayout_8);


        viewGridLayout->addWidget(image2frame, 3, 1, 1, 1);


        horizontalLayout_3->addLayout(viewGridLayout);

        MainWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1160, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRecentImage = new QMenu(menuFile);
        menuRecentImage->setObjectName(QStringLiteral("menuRecentImage"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuInteraction = new QMenu(menuBar);
        menuInteraction->setObjectName(QStringLiteral("menuInteraction"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuDisplay = new QMenu(menuView);
        menuDisplay->setObjectName(QStringLiteral("menuDisplay"));
        menuOrientation = new QMenu(menuView);
        menuOrientation->setObjectName(QStringLiteral("menuOrientation"));
        menuSegmentation = new QMenu(menuBar);
        menuSegmentation->setObjectName(QStringLiteral("menuSegmentation"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(320, 46));
        dockWidget->setMaximumSize(QSize(320, 524287));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);
        ActionToolBar = new QToolBar(MainWindow);
        ActionToolBar->setObjectName(QStringLiteral("ActionToolBar"));
        ActionToolBar->setIconSize(QSize(30, 30));
        MainWindow->addToolBar(Qt::TopToolBarArea, ActionToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuInteraction->menuAction());
        menuBar->addAction(menuSegmentation->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpenImage);
        menuFile->addAction(menuRecentImage->menuAction());
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionInformation);
        menuHelp->addAction(actionHelp);
        menuInteraction->addAction(actionNavigation);
        menuInteraction->addAction(actionWindowLevel);
        menuView->addAction(menuDisplay->menuAction());
        menuView->addAction(menuOrientation->menuAction());
        menuDisplay->addAction(actionImage1);
        menuDisplay->addAction(actionImage2);
        menuDisplay->addAction(actionImage3);
        menuDisplay->addAction(actionImage4);
        menuDisplay->addAction(actionFourViews);
        menuOrientation->addAction(actionAllAxialView);
        menuOrientation->addAction(actionMultiPlanarView);
        menuSegmentation->addAction(actionOpenSegmentation);
        menuSegmentation->addAction(actionROI);
        menuSegmentation->addAction(actionContour);
        menuSegmentation->addAction(actionRuler);
        menuSegmentation->addAction(actionCenterlineAlgorithm);
        ActionToolBar->addAction(actionOpenImage);
        ActionToolBar->addAction(actionSave);
        ActionToolBar->addSeparator();
        ActionToolBar->addAction(actionNavigation);
        ActionToolBar->addAction(actionWindowLevel);
        ActionToolBar->addAction(actionRuler);
        ActionToolBar->addSeparator();
        ActionToolBar->addAction(actionROI);
        ActionToolBar->addAction(actionContour);
        ActionToolBar->addAction(actionBrush);
        ActionToolBar->addSeparator();
        ActionToolBar->addAction(actionMultiPlanarView);
        ActionToolBar->addAction(actionAllAxialView);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PlaqueQuant", 0));
        actionOpenImage->setText(QApplication::translate("MainWindow", "&Open Image", 0));
#ifndef QT_NO_TOOLTIP
        actionOpenImage->setToolTip(QApplication::translate("MainWindow", "Open Image", 0));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("MainWindow", "&Exit", 0));
        actionSave->setText(QApplication::translate("MainWindow", "&Save", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "&About", 0));
        actionHelp->setText(QApplication::translate("MainWindow", "&Help", 0));
#ifndef QT_NO_TOOLTIP
        actionHelp->setToolTip(QApplication::translate("MainWindow", "Help", 0));
#endif // QT_NO_TOOLTIP
        actionRuler->setText(QApplication::translate("MainWindow", "&Ruler", 0));
        actionWindowLevel->setText(QApplication::translate("MainWindow", "&WindowLevel", 0));
        actionNavigation->setText(QApplication::translate("MainWindow", "&Navigation", 0));
        actionFourViews->setText(QApplication::translate("MainWindow", "Four Views", 0));
        actionInformation->setText(QApplication::translate("MainWindow", "Information", 0));
        actionAddOverlay->setText(QApplication::translate("MainWindow", "Add Overlay", 0));
        actionRemoveOverlay->setText(QApplication::translate("MainWindow", "Remove Overlay", 0));
        actionOverlayVisible->setText(QApplication::translate("MainWindow", "Visible", 0));
        actionIncreaseOverlayOpacity->setText(QApplication::translate("MainWindow", "Increase Opacity", 0));
        actionDecreaseOverlayOpacity->setText(QApplication::translate("MainWindow", "Decrease Opacity", 0));
        actionContour->setText(QApplication::translate("MainWindow", "&Contour", 0));
#ifndef QT_NO_TOOLTIP
        actionContour->setToolTip(QApplication::translate("MainWindow", "ContourDrawing", 0));
#endif // QT_NO_TOOLTIP
        actionCloseImage->setText(QApplication::translate("MainWindow", "Close Image", 0));
        actionImage1->setText(QApplication::translate("MainWindow", "Upper Left", 0));
        actionImage2->setText(QApplication::translate("MainWindow", "Upper Right", 0));
        actionImage3->setText(QApplication::translate("MainWindow", "Lower Left", 0));
        actionImage4->setText(QApplication::translate("MainWindow", "Lower Right", 0));
        actionSagittal->setText(QApplication::translate("MainWindow", "All Sagittal View", 0));
        actionCoronal->setText(QApplication::translate("MainWindow", "All Coronal View", 0));
        actionAllAxialView->setText(QApplication::translate("MainWindow", "All Axial View", 0));
        actionRemoveContour->setText(QApplication::translate("MainWindow", "Remove", 0));
        actionROI->setText(QApplication::translate("MainWindow", "Select ROI", 0));
        actionStatistics->setText(QApplication::translate("MainWindow", "Statistics", 0));
        actionBrush->setText(QApplication::translate("MainWindow", "Brush", 0));
        actionMultiPlanarView->setText(QApplication::translate("MainWindow", "Multi-Planar View", 0));
        actionOpenSegmentation->setText(QApplication::translate("MainWindow", "Open Segmentation", 0));
        actionCenterlineAlgorithm->setText(QApplication::translate("MainWindow", "Centerline Algorithm", 0));
#ifndef QT_NO_TOOLTIP
        LLBtn->setToolTip(QApplication::translate("MainWindow", "Expand this view to the entire window", 0));
#endif // QT_NO_TOOLTIP
        LLBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        LLBtn2->setToolTip(QApplication::translate("MainWindow", "Restore the four-panel display configuration", 0));
#endif // QT_NO_TOOLTIP
        LLBtn2->setText(QString());
        LLSelectImgBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        LRBtn->setToolTip(QApplication::translate("MainWindow", "Expand this view to the entire window", 0));
#endif // QT_NO_TOOLTIP
        LRBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        LRBtn2->setToolTip(QApplication::translate("MainWindow", "Restore the four-panel display configuration", 0));
#endif // QT_NO_TOOLTIP
        LRBtn2->setText(QString());
        label->setText(QApplication::translate("MainWindow", "x:", 0));
        label_2->setText(QApplication::translate("MainWindow", "y:", 0));
        label_3->setText(QApplication::translate("MainWindow", "z:", 0));
        updateBtn->setText(QApplication::translate("MainWindow", "update", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Play", 0));
#ifndef QT_NO_TOOLTIP
        ULBtn->setToolTip(QApplication::translate("MainWindow", "Expand this view to the entire window", 0));
#endif // QT_NO_TOOLTIP
        ULBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        ULBtn2->setToolTip(QApplication::translate("MainWindow", "Restore the four-panel display configuration", 0));
#endif // QT_NO_TOOLTIP
        ULBtn2->setText(QString());
        ULSelectImgBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        URBtn->setToolTip(QApplication::translate("MainWindow", "Expand this view to the entire window", 0));
#endif // QT_NO_TOOLTIP
        URBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        URBtn2->setToolTip(QApplication::translate("MainWindow", "Restore the four-panel display configuration", 0));
#endif // QT_NO_TOOLTIP
        URBtn2->setText(QString());
        URSelectImgBtn->setText(QString());
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0));
        menuRecentImage->setTitle(QApplication::translate("MainWindow", "Recent Image", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "&Help", 0));
        menuInteraction->setTitle(QApplication::translate("MainWindow", "Interaction", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "&View", 0));
        menuDisplay->setTitle(QApplication::translate("MainWindow", "Display", 0));
        menuOrientation->setTitle(QApplication::translate("MainWindow", "View Mode", 0));
        menuSegmentation->setTitle(QApplication::translate("MainWindow", "Segmentation", 0));
        ActionToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
