/********************************************************************************
** Form generated from reading UI file 'ModuleWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODULEWIDGET_H
#define UI_MODULEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModuleWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QFrame *line_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout;
    QLabel *label_37;
    QFrame *frame_9;
    QVBoxLayout *verticalLayout_23;
    QVBoxLayout *verticalLayout_24;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_38;
    QLabel *label_39;
    QLabel *label_8;
    QFrame *line_10;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_8;
    QSpinBox *spinBox_4;
    QSpinBox *spinBox_5;
    QSpinBox *spinBox_6;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_16;
    QSpinBox *spinBox_7;
    QSpinBox *spinBox_8;
    QSpinBox *spinBox_9;
    QPushButton *pushButton;
    QPushButton *SetROIBtn;
    QSpacerItem *verticalSpacer_8;
    QWidget *page_4;
    QVBoxLayout *verticalLayout_15;
    QLabel *label_21;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_11;
    QVBoxLayout *verticalLayout_12;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_23;
    QLabel *label_18;
    QFrame *line_7;
    QLabel *label_20;
    QFrame *line_3;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QCheckBox *checkBox;
    QSpacerItem *verticalSpacer_3;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_25;
    QFrame *frame_6;
    QVBoxLayout *verticalLayout_13;
    QVBoxLayout *verticalLayout_14;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_22;
    QFrame *line_5;
    QLabel *label_16;
    QHBoxLayout *horizontalLayout;
    QSlider *BrushSizeSlider;
    QSpinBox *BrushSizeSpinBox;
    QLabel *label_17;
    QHBoxLayout *horizontalLayout_9;
    QComboBox *BrushComBox;
    QLabel *label_15;
    QCheckBox *checkBox_2;
    QSpacerItem *verticalSpacer;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_2;
    QTableWidget *tableWidget;
    QLabel *label_3;
    QTableWidget *tableWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer;
    QSpinBox *spinBox_2;
    QSpacerItem *verticalSpacer_4;
    QFrame *line_4;
    QFrame *line;
    QLabel *label_14;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_12;
    QComboBox *labelComboBox;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_13;
    QHBoxLayout *horizontalLayout_6;
    QSpinBox *opacitySpinBox;
    QSlider *opacitySlider;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *BackBtn;
    QPushButton *NextBtn;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *ModuleWidget)
    {
        if (ModuleWidget->objectName().isEmpty())
            ModuleWidget->setObjectName(QStringLiteral("ModuleWidget"));
        ModuleWidget->resize(336, 825);
        ModuleWidget->setMinimumSize(QSize(0, 0));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        ModuleWidget->setFont(font);
        verticalLayout_2 = new QVBoxLayout(ModuleWidget);
        verticalLayout_2->setSpacing(14);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(ModuleWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        line_2 = new QFrame(ModuleWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_2);

        stackedWidget = new QStackedWidget(ModuleWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setFrameShape(QFrame::Box);
        stackedWidget->setFrameShadow(QFrame::Raised);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        verticalLayout = new QVBoxLayout(page);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_37 = new QLabel(page);
        label_37->setObjectName(QStringLiteral("label_37"));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        label_37->setFont(font2);
        label_37->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_37);

        frame_9 = new QFrame(page);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setMinimumSize(QSize(0, 0));
        QFont font3;
        font3.setFamily(QStringLiteral("Arial"));
        font3.setPointSize(10);
        frame_9->setFont(font3);
        frame_9->setStyleSheet(QStringLiteral("background-color: rgb(226, 226, 226)"));
        frame_9->setFrameShape(QFrame::Box);
        frame_9->setFrameShadow(QFrame::Raised);
        frame_9->setLineWidth(1);
        verticalLayout_23 = new QVBoxLayout(frame_9);
        verticalLayout_23->setSpacing(7);
        verticalLayout_23->setObjectName(QStringLiteral("verticalLayout_23"));
        verticalLayout_24 = new QVBoxLayout();
        verticalLayout_24->setSpacing(7);
        verticalLayout_24->setObjectName(QStringLiteral("verticalLayout_24"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_38 = new QLabel(frame_9);
        label_38->setObjectName(QStringLiteral("label_38"));
        label_38->setFont(font);

        horizontalLayout_15->addWidget(label_38);


        verticalLayout_24->addLayout(horizontalLayout_15);


        verticalLayout_23->addLayout(verticalLayout_24);

        label_39 = new QLabel(frame_9);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setFont(font);

        verticalLayout_23->addWidget(label_39);


        verticalLayout->addWidget(frame_9);

        label_8 = new QLabel(page);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setWordWrap(true);

        verticalLayout->addWidget(label_8);

        line_10 = new QFrame(page);
        line_10->setObjectName(QStringLiteral("line_10"));
        line_10->setFrameShape(QFrame::HLine);
        line_10->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_10);

        label_9 = new QLabel(page);
        label_9->setObjectName(QStringLiteral("label_9"));

        verticalLayout->addWidget(label_9);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(-1, -1, -1, 0);
        spinBox_4 = new QSpinBox(page);
        spinBox_4->setObjectName(QStringLiteral("spinBox_4"));
        spinBox_4->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_8->addWidget(spinBox_4);

        spinBox_5 = new QSpinBox(page);
        spinBox_5->setObjectName(QStringLiteral("spinBox_5"));
        spinBox_5->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_8->addWidget(spinBox_5);

        spinBox_6 = new QSpinBox(page);
        spinBox_6->setObjectName(QStringLiteral("spinBox_6"));
        spinBox_6->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_8->addWidget(spinBox_6);


        verticalLayout->addLayout(horizontalLayout_8);

        label_10 = new QLabel(page);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout->addWidget(label_10);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        horizontalLayout_16->setContentsMargins(-1, -1, -1, 0);
        spinBox_7 = new QSpinBox(page);
        spinBox_7->setObjectName(QStringLiteral("spinBox_7"));
        spinBox_7->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_16->addWidget(spinBox_7);

        spinBox_8 = new QSpinBox(page);
        spinBox_8->setObjectName(QStringLiteral("spinBox_8"));
        spinBox_8->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_16->addWidget(spinBox_8);

        spinBox_9 = new QSpinBox(page);
        spinBox_9->setObjectName(QStringLiteral("spinBox_9"));
        spinBox_9->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_16->addWidget(spinBox_9);


        verticalLayout->addLayout(horizontalLayout_16);

        pushButton = new QPushButton(page);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        SetROIBtn = new QPushButton(page);
        SetROIBtn->setObjectName(QStringLiteral("SetROIBtn"));

        verticalLayout->addWidget(SetROIBtn);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_8);

        stackedWidget->addWidget(page);
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        verticalLayout_15 = new QVBoxLayout(page_4);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        label_21 = new QLabel(page_4);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setFont(font2);
        label_21->setAlignment(Qt::AlignCenter);

        verticalLayout_15->addWidget(label_21);

        frame_5 = new QFrame(page_4);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setMinimumSize(QSize(0, 0));
        frame_5->setFont(font3);
        frame_5->setStyleSheet(QStringLiteral("background-color: rgb(226, 226, 226)"));
        frame_5->setFrameShape(QFrame::Box);
        frame_5->setFrameShadow(QFrame::Raised);
        frame_5->setLineWidth(1);
        verticalLayout_11 = new QVBoxLayout(frame_5);
        verticalLayout_11->setSpacing(7);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(7);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_23 = new QLabel(frame_5);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setFont(font);

        horizontalLayout_11->addWidget(label_23);


        verticalLayout_12->addLayout(horizontalLayout_11);


        verticalLayout_11->addLayout(verticalLayout_12);

        label_18 = new QLabel(frame_5);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setFont(font);

        verticalLayout_11->addWidget(label_18);


        verticalLayout_15->addWidget(frame_5);

        line_7 = new QFrame(page_4);
        line_7->setObjectName(QStringLiteral("line_7"));
        line_7->setFrameShape(QFrame::HLine);
        line_7->setFrameShadow(QFrame::Sunken);

        verticalLayout_15->addWidget(line_7);

        label_20 = new QLabel(page_4);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setWordWrap(true);

        verticalLayout_15->addWidget(label_20);

        line_3 = new QFrame(page_4);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_15->addWidget(line_3);

        radioButton = new QRadioButton(page_4);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setChecked(true);

        verticalLayout_15->addWidget(radioButton);

        radioButton_2 = new QRadioButton(page_4);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        verticalLayout_15->addWidget(radioButton_2);

        checkBox = new QCheckBox(page_4);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        verticalLayout_15->addWidget(checkBox);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_15->addItem(verticalSpacer_3);

        stackedWidget->addWidget(page_4);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        verticalLayout_10 = new QVBoxLayout(page_3);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        label_25 = new QLabel(page_3);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setFont(font2);
        label_25->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(label_25);

        frame_6 = new QFrame(page_3);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setMinimumSize(QSize(0, 0));
        frame_6->setFont(font3);
        frame_6->setStyleSheet(QStringLiteral("background-color: rgb(226, 226, 226)"));
        frame_6->setFrameShape(QFrame::Box);
        frame_6->setFrameShadow(QFrame::Raised);
        frame_6->setLineWidth(1);
        verticalLayout_13 = new QVBoxLayout(frame_6);
        verticalLayout_13->setSpacing(7);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(7);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_26 = new QLabel(frame_6);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setFont(font);

        horizontalLayout_12->addWidget(label_26);


        verticalLayout_14->addLayout(horizontalLayout_12);


        verticalLayout_13->addLayout(verticalLayout_14);

        label_27 = new QLabel(frame_6);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setFont(font);

        verticalLayout_13->addWidget(label_27);


        verticalLayout_10->addWidget(frame_6);

        label_22 = new QLabel(page_3);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setWordWrap(true);

        verticalLayout_10->addWidget(label_22);

        line_5 = new QFrame(page_3);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_10->addWidget(line_5);

        label_16 = new QLabel(page_3);
        label_16->setObjectName(QStringLiteral("label_16"));

        verticalLayout_10->addWidget(label_16);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        BrushSizeSlider = new QSlider(page_3);
        BrushSizeSlider->setObjectName(QStringLiteral("BrushSizeSlider"));
        BrushSizeSlider->setMaximum(40);
        BrushSizeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(BrushSizeSlider);

        BrushSizeSpinBox = new QSpinBox(page_3);
        BrushSizeSpinBox->setObjectName(QStringLiteral("BrushSizeSpinBox"));
        BrushSizeSpinBox->setMaximum(40);

        horizontalLayout->addWidget(BrushSizeSpinBox);


        verticalLayout_10->addLayout(horizontalLayout);

        label_17 = new QLabel(page_3);
        label_17->setObjectName(QStringLiteral("label_17"));

        verticalLayout_10->addWidget(label_17);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        BrushComBox = new QComboBox(page_3);
        BrushComBox->setObjectName(QStringLiteral("BrushComBox"));

        horizontalLayout_9->addWidget(BrushComBox);


        verticalLayout_10->addLayout(horizontalLayout_9);

        label_15 = new QLabel(page_3);
        label_15->setObjectName(QStringLiteral("label_15"));

        verticalLayout_10->addWidget(label_15);

        checkBox_2 = new QCheckBox(page_3);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        verticalLayout_10->addWidget(checkBox_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer);

        stackedWidget->addWidget(page_3);
        label_22->raise();
        line_5->raise();
        label_17->raise();
        label_25->raise();
        frame_6->raise();
        checkBox_2->raise();
        label_15->raise();
        label_16->raise();
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        verticalLayout_8 = new QVBoxLayout(page_2);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        label_2 = new QLabel(page_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_8->addWidget(label_2);

        tableWidget = new QTableWidget(page_2);
        if (tableWidget->columnCount() < 1)
            tableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (tableWidget->rowCount() < 4)
            tableWidget->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem4);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setFont(font);
        tableWidget->setAutoScroll(true);
        tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableWidget->setShowGrid(true);
        tableWidget->setWordWrap(true);
        tableWidget->setCornerButtonEnabled(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setDefaultSectionSize(100);
        tableWidget->horizontalHeader()->setHighlightSections(true);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->verticalHeader()->setCascadingSectionResizes(false);

        verticalLayout_8->addWidget(tableWidget);

        label_3 = new QLabel(page_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_8->addWidget(label_3);

        tableWidget_2 = new QTableWidget(page_2);
        if (tableWidget_2->columnCount() < 1)
            tableWidget_2->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        if (tableWidget_2->rowCount() < 6)
            tableWidget_2->setRowCount(6);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(4, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(5, __qtablewidgetitem11);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        tableWidget_2->horizontalHeader()->setDefaultSectionSize(130);

        verticalLayout_8->addWidget(tableWidget_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, -1, 0);
        label_4 = new QLabel(page_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_2->addWidget(label_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        spinBox_2 = new QSpinBox(page_2);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_2->addWidget(spinBox_2);


        verticalLayout_8->addLayout(horizontalLayout_2);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_4);

        line_4 = new QFrame(page_2);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_8->addWidget(line_4);

        stackedWidget->addWidget(page_2);

        verticalLayout_2->addWidget(stackedWidget);

        line = new QFrame(ModuleWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        label_14 = new QLabel(ModuleWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        QFont font4;
        font4.setPointSize(11);
        font4.setBold(true);
        font4.setWeight(75);
        label_14->setFont(font4);
        label_14->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_14);

        frame_4 = new QFrame(ModuleWidget);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setMinimumSize(QSize(0, 0));
        frame_4->setStyleSheet(QStringLiteral("background-color: rgb(226, 226, 226)"));
        frame_4->setFrameShape(QFrame::Box);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_9 = new QVBoxLayout(frame_4);
        verticalLayout_9->setSpacing(7);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(-1, 11, 12, 11);
        label_12 = new QLabel(frame_4);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setFont(font);

        verticalLayout_9->addWidget(label_12);

        labelComboBox = new QComboBox(frame_4);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/redbox.png"), QSize(), QIcon::Normal, QIcon::Off);
        labelComboBox->addItem(icon, QString());
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/bluebox.png"), QSize(), QIcon::Normal, QIcon::Off);
        labelComboBox->addItem(icon1, QString());
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/greenbox.png"), QSize(), QIcon::Normal, QIcon::Off);
        labelComboBox->addItem(icon2, QString());
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/yellowbox.png"), QSize(), QIcon::Normal, QIcon::Off);
        labelComboBox->addItem(icon3, QString());
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/lightbluebox.png"), QSize(), QIcon::Normal, QIcon::Off);
        labelComboBox->addItem(icon4, QString());
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/purplebox.png"), QSize(), QIcon::Normal, QIcon::Off);
        labelComboBox->addItem(icon5, QString());
        labelComboBox->setObjectName(QStringLiteral("labelComboBox"));
        labelComboBox->setFont(font);
        labelComboBox->setIconSize(QSize(13, 13));

        verticalLayout_9->addWidget(labelComboBox);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 0, -1, -1);

        verticalLayout_9->addLayout(horizontalLayout_5);

        label_13 = new QLabel(frame_4);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setFont(font);

        verticalLayout_9->addWidget(label_13);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(-1, 0, -1, 0);
        opacitySpinBox = new QSpinBox(frame_4);
        opacitySpinBox->setObjectName(QStringLiteral("opacitySpinBox"));
        opacitySpinBox->setMaximum(100);
        opacitySpinBox->setValue(100);

        horizontalLayout_6->addWidget(opacitySpinBox);

        opacitySlider = new QSlider(frame_4);
        opacitySlider->setObjectName(QStringLiteral("opacitySlider"));
        opacitySlider->setMaximum(100);
        opacitySlider->setValue(100);
        opacitySlider->setSliderPosition(100);
        opacitySlider->setOrientation(Qt::Horizontal);

        horizontalLayout_6->addWidget(opacitySlider);


        verticalLayout_9->addLayout(horizontalLayout_6);


        verticalLayout_2->addWidget(frame_4);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 0, -1, -1);
        BackBtn = new QPushButton(ModuleWidget);
        BackBtn->setObjectName(QStringLiteral("BackBtn"));

        horizontalLayout_4->addWidget(BackBtn);

        NextBtn = new QPushButton(ModuleWidget);
        NextBtn->setObjectName(QStringLiteral("NextBtn"));

        horizontalLayout_4->addWidget(NextBtn);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        retranslateUi(ModuleWidget);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ModuleWidget);
    } // setupUi

    void retranslateUi(QWidget *ModuleWidget)
    {
        ModuleWidget->setWindowTitle(QApplication::translate("ModuleWidget", "Widget", 0));
        label->setText(QApplication::translate("ModuleWidget", "Segment 3D", 0));
        label_37->setText(QApplication::translate("ModuleWidget", "<html><head/><body><p><span style=\" color:#0000a5;\">Current Stage</span></p></body></html>", 0));
        label_38->setText(QApplication::translate("ModuleWidget", "Step 1/3", 0));
        label_39->setText(QApplication::translate("ModuleWidget", "Select ROI", 0));
        label_8->setText(QApplication::translate("ModuleWidget", "Selects the region of interest (ROI) for segmentation", 0));
        label_9->setText(QApplication::translate("ModuleWidget", "ROI corner position (x, y, z):", 0));
        label_10->setText(QApplication::translate("ModuleWidget", "ROI size (x,y,z)", 0));
        pushButton->setText(QApplication::translate("ModuleWidget", "Reset ROI", 0));
        SetROIBtn->setText(QApplication::translate("ModuleWidget", "Segmentation", 0));
        label_21->setText(QApplication::translate("ModuleWidget", "<html><head/><body><p><span style=\" color:#0000a5;\">Current Stage</span></p></body></html>", 0));
        label_23->setText(QApplication::translate("ModuleWidget", "Step 2/3", 0));
        label_18->setText(QApplication::translate("ModuleWidget", "Segmentation of Vessel and Lumen", 0));
        label_20->setText(QApplication::translate("ModuleWidget", "Use the Contour Widget to highlight the vessel wall and the lumen, select the layer by the window below", 0));
        radioButton->setText(QApplication::translate("ModuleWidget", "Smooth Curve", 0));
        radioButton_2->setText(QApplication::translate("ModuleWidget", "Polygon", 0));
        checkBox->setText(QApplication::translate("ModuleWidget", "Close Polygon as the user select vertics", 0));
        label_25->setText(QApplication::translate("ModuleWidget", "<html><head/><body><p><span style=\" color:#0000a5;\">Current Stage</span></p></body></html>", 0));
        label_26->setText(QApplication::translate("ModuleWidget", "Step 3/3", 0));
        label_27->setText(QApplication::translate("ModuleWidget", "Segmentation of Plaque", 0));
        label_22->setText(QApplication::translate("ModuleWidget", "You can use the brush tool to highlight the region of plaque", 0));
        label_16->setText(QApplication::translate("ModuleWidget", "Brush Size", 0));
        label_17->setText(QApplication::translate("ModuleWidget", "Brush Style", 0));
        BrushComBox->clear();
        BrushComBox->insertItems(0, QStringList()
         << QApplication::translate("ModuleWidget", "Square", 0)
         << QApplication::translate("ModuleWidget", "Circle", 0)
        );
        label_15->setText(QApplication::translate("ModuleWidget", "Brush Option:", 0));
        checkBox_2->setText(QApplication::translate("ModuleWidget", "3D", 0));
        label_2->setText(QApplication::translate("ModuleWidget", "2D Measurement", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ModuleWidget", "Value", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("ModuleWidget", "Lumen Area / mm^2", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem2->setText(QApplication::translate("ModuleWidget", "Vessel Wall Area / mm^2", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem3->setText(QApplication::translate("ModuleWidget", "Wall Thickness / mm", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem4->setText(QApplication::translate("ModuleWidget", "NMI", 0));
        label_3->setText(QApplication::translate("ModuleWidget", "3D Measurement", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QApplication::translate("ModuleWidget", "Volume", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_2->verticalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("ModuleWidget", "Lumen /mm^3", 0));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_2->verticalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("ModuleWidget", "Vessel Wall /mm^3", 0));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_2->verticalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("ModuleWidget", "Total Plaque /mm^3", 0));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_2->verticalHeaderItem(3);
        ___qtablewidgetitem9->setText(QApplication::translate("ModuleWidget", "Calcification /mm^3", 0));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_2->verticalHeaderItem(4);
        ___qtablewidgetitem10->setText(QApplication::translate("ModuleWidget", "Hemorrhage /mm^3", 0));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_2->verticalHeaderItem(5);
        ___qtablewidgetitem11->setText(QApplication::translate("ModuleWidget", "LRNC /mm^3", 0));
        label_4->setText(QApplication::translate("ModuleWidget", "Stenosis %", 0));
        label_14->setText(QApplication::translate("ModuleWidget", "<html><head/><body><p><span style=\" font-size:10pt; color:#0000b3;\">Segmentation Labels</span></p></body></html>", 0));
        label_12->setText(QApplication::translate("ModuleWidget", "Active Label:", 0));
        labelComboBox->setItemText(0, QApplication::translate("ModuleWidget", "Lumen", 0));
        labelComboBox->setItemText(1, QApplication::translate("ModuleWidget", "Vessel Wall", 0));
        labelComboBox->setItemText(2, QApplication::translate("ModuleWidget", "Calcification", 0));
        labelComboBox->setItemText(3, QApplication::translate("ModuleWidget", "Hemorrhage", 0));
        labelComboBox->setItemText(4, QApplication::translate("ModuleWidget", "LRNC", 0));
        labelComboBox->setItemText(5, QApplication::translate("ModuleWidget", "LM", 0));

        label_13->setText(QApplication::translate("ModuleWidget", "Label Opacity:", 0));
        BackBtn->setText(QApplication::translate("ModuleWidget", "Back", 0));
        NextBtn->setText(QApplication::translate("ModuleWidget", "Next", 0));
    } // retranslateUi

};

namespace Ui {
    class ModuleWidget: public Ui_ModuleWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODULEWIDGET_H
