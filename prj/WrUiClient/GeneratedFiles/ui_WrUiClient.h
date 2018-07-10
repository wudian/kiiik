/********************************************************************************
** Form generated from reading UI file 'WrUiClient.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WRUICLIENT_H
#define UI_WRUICLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widget_title;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lab_Ico;
    QLabel *lab_Title;
    QWidget *widget_menu;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnMenu;
    QPushButton *btnMenu_Min;
    QPushButton *btnMenu_Max;
    QPushButton *btnMenu_Close;
    QWidget *MainWindow_2;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_syms;
    QSpacerItem *horizontalSpacer;
    QPushButton *update_symlst_btn_;
    QPushButton *sub_btn_;
    QPushButton *unsub_btn_;
    QTableView *syms_lst_view_;
    QFrame *line1;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QLabel *m_DiscountPrice;
    QLabel *label10;
    QLabel *label16;
    QLabel *label2;
    QLabel *label4;
    QLabel *m_DiscountRatio;
    QLabel *label9;
    QLabel *m_PosiRepoRatio;
    QLabel *label11;
    QLabel *m_ReveRepoRatio;
    QLabel *m_ExchangeId;
    QLabel *label3;
    QLabel *label8;
    QLabel *label1;
    QLabel *label17;
    QLabel *label12;
    QLabel *m_PosiRepoPrice;
    QLabel *m_ContractDate;
    QLabel *m_AlphaLastPrice;
    QLabel *m_ContractDays;
    QLabel *label15;
    QLabel *m_DeliveryDate;
    QLabel *m_ProductId;
    QLabel *m_AlphaInstrumentId;
    QLabel *label_quote;
    QLabel *label14;
    QLabel *label6;
    QLabel *label7;
    QLabel *m_DeadlineDate;
    QLabel *label13;
    QLabel *m_BravoInstrumentId;
    QLabel *m_FlexReveRepoPrice;
    QLabel *m_FlexReveRepoRatio;
    QLabel *label5;
    QLabel *m_BravoLastPrice;
    QLabel *label0;
    QLabel *m_SystemDate;
    QLabel *label18;
    QLabel *m_SystemTime;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QFrame *line2;
    QLabel *label_sub;
    QTableView *sub_tbl_view_;
    QStatusBar *statusBar2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1205, 705);
        QIcon icon;
        icon.addFile(QStringLiteral("C:/Users/Administrator/.designer/backup/Title.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_title = new QWidget(centralwidget);
        widget_title->setObjectName(QStringLiteral("widget_title"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_title->sizePolicy().hasHeightForWidth());
        widget_title->setSizePolicy(sizePolicy);
        widget_title->setMinimumSize(QSize(100, 33));
        horizontalLayout_4 = new QHBoxLayout(widget_title);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(9, 9, 9, 9);
        lab_Ico = new QLabel(widget_title);
        lab_Ico->setObjectName(QStringLiteral("lab_Ico"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lab_Ico->sizePolicy().hasHeightForWidth());
        lab_Ico->setSizePolicy(sizePolicy1);
        lab_Ico->setMinimumSize(QSize(30, 0));
        lab_Ico->setPixmap(QPixmap(QString::fromUtf8(":/image/logo.png")));
        lab_Ico->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(lab_Ico);

        lab_Title = new QLabel(widget_title);
        lab_Title->setObjectName(QStringLiteral("lab_Title"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lab_Title->sizePolicy().hasHeightForWidth());
        lab_Title->setSizePolicy(sizePolicy2);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(10);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        lab_Title->setFont(font);
        lab_Title->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lab_Title->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(lab_Title);

        widget_menu = new QWidget(widget_title);
        widget_menu->setObjectName(QStringLiteral("widget_menu"));
        sizePolicy1.setHeightForWidth(widget_menu->sizePolicy().hasHeightForWidth());
        widget_menu->setSizePolicy(sizePolicy1);
        horizontalLayout_5 = new QHBoxLayout(widget_menu);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        btnMenu = new QPushButton(widget_menu);
        btnMenu->setObjectName(QStringLiteral("btnMenu"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnMenu->sizePolicy().hasHeightForWidth());
        btnMenu->setSizePolicy(sizePolicy3);
        btnMenu->setMinimumSize(QSize(31, 0));
        btnMenu->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu->setFocusPolicy(Qt::NoFocus);
        btnMenu->setFlat(true);

        horizontalLayout_5->addWidget(btnMenu);

        btnMenu_Min = new QPushButton(widget_menu);
        btnMenu_Min->setObjectName(QStringLiteral("btnMenu_Min"));
        sizePolicy3.setHeightForWidth(btnMenu_Min->sizePolicy().hasHeightForWidth());
        btnMenu_Min->setSizePolicy(sizePolicy3);
        btnMenu_Min->setMinimumSize(QSize(31, 0));
        btnMenu_Min->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Min->setFocusPolicy(Qt::NoFocus);
        btnMenu_Min->setFlat(true);

        horizontalLayout_5->addWidget(btnMenu_Min);

        btnMenu_Max = new QPushButton(widget_menu);
        btnMenu_Max->setObjectName(QStringLiteral("btnMenu_Max"));
        sizePolicy3.setHeightForWidth(btnMenu_Max->sizePolicy().hasHeightForWidth());
        btnMenu_Max->setSizePolicy(sizePolicy3);
        btnMenu_Max->setMinimumSize(QSize(31, 0));
        btnMenu_Max->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Max->setFocusPolicy(Qt::NoFocus);
        btnMenu_Max->setFlat(true);

        horizontalLayout_5->addWidget(btnMenu_Max);

        btnMenu_Close = new QPushButton(widget_menu);
        btnMenu_Close->setObjectName(QStringLiteral("btnMenu_Close"));
        sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Close->setSizePolicy(sizePolicy3);
        btnMenu_Close->setMinimumSize(QSize(40, 0));
        btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Close->setFocusPolicy(Qt::NoFocus);
        btnMenu_Close->setFlat(true);

        horizontalLayout_5->addWidget(btnMenu_Close);


        horizontalLayout_4->addWidget(widget_menu);


        verticalLayout->addWidget(widget_title);

        MainWindow_2 = new QWidget(centralwidget);
        MainWindow_2->setObjectName(QStringLiteral("MainWindow_2"));
        horizontalLayout = new QHBoxLayout(MainWindow_2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(9, 9, 9, 0);
        groupBox = new QGroupBox(MainWindow_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget = new QWidget(groupBox);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_syms = new QLabel(widget);
        label_syms->setObjectName(QStringLiteral("label_syms"));
        label_syms->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_2->addWidget(label_syms);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        update_symlst_btn_ = new QPushButton(widget);
        update_symlst_btn_->setObjectName(QStringLiteral("update_symlst_btn_"));

        horizontalLayout_2->addWidget(update_symlst_btn_);

        sub_btn_ = new QPushButton(widget);
        sub_btn_->setObjectName(QStringLiteral("sub_btn_"));

        horizontalLayout_2->addWidget(sub_btn_);

        unsub_btn_ = new QPushButton(widget);
        unsub_btn_->setObjectName(QStringLiteral("unsub_btn_"));

        horizontalLayout_2->addWidget(unsub_btn_);


        verticalLayout_2->addWidget(widget);

        syms_lst_view_ = new QTableView(groupBox);
        syms_lst_view_->setObjectName(QStringLiteral("syms_lst_view_"));

        verticalLayout_2->addWidget(syms_lst_view_);


        horizontalLayout->addWidget(groupBox);

        line1 = new QFrame(MainWindow_2);
        line1->setObjectName(QStringLiteral("line1"));
        line1->setFrameShape(QFrame::VLine);
        line1->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line1);

        groupBox_3 = new QGroupBox(MainWindow_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        m_DiscountPrice = new QLabel(groupBox_3);
        m_DiscountPrice->setObjectName(QStringLiteral("m_DiscountPrice"));

        gridLayout->addWidget(m_DiscountPrice, 8, 3, 1, 1);

        label10 = new QLabel(groupBox_3);
        label10->setObjectName(QStringLiteral("label10"));

        gridLayout->addWidget(label10, 7, 2, 1, 1);

        label16 = new QLabel(groupBox_3);
        label16->setObjectName(QStringLiteral("label16"));

        gridLayout->addWidget(label16, 5, 4, 1, 1);

        label2 = new QLabel(groupBox_3);
        label2->setObjectName(QStringLiteral("label2"));

        gridLayout->addWidget(label2, 5, 0, 1, 1);

        label4 = new QLabel(groupBox_3);
        label4->setObjectName(QStringLiteral("label4"));

        gridLayout->addWidget(label4, 8, 0, 1, 1);

        m_DiscountRatio = new QLabel(groupBox_3);
        m_DiscountRatio->setObjectName(QStringLiteral("m_DiscountRatio"));

        gridLayout->addWidget(m_DiscountRatio, 2, 3, 1, 1);

        label9 = new QLabel(groupBox_3);
        label9->setObjectName(QStringLiteral("label9"));

        gridLayout->addWidget(label9, 5, 2, 1, 1);

        m_PosiRepoRatio = new QLabel(groupBox_3);
        m_PosiRepoRatio->setObjectName(QStringLiteral("m_PosiRepoRatio"));

        gridLayout->addWidget(m_PosiRepoRatio, 5, 3, 1, 1);

        label11 = new QLabel(groupBox_3);
        label11->setObjectName(QStringLiteral("label11"));

        gridLayout->addWidget(label11, 8, 2, 1, 1);

        m_ReveRepoRatio = new QLabel(groupBox_3);
        m_ReveRepoRatio->setObjectName(QStringLiteral("m_ReveRepoRatio"));

        gridLayout->addWidget(m_ReveRepoRatio, 7, 3, 1, 1);

        m_ExchangeId = new QLabel(groupBox_3);
        m_ExchangeId->setObjectName(QStringLiteral("m_ExchangeId"));

        gridLayout->addWidget(m_ExchangeId, 2, 1, 1, 1);

        label3 = new QLabel(groupBox_3);
        label3->setObjectName(QStringLiteral("label3"));

        gridLayout->addWidget(label3, 7, 0, 1, 1);

        label8 = new QLabel(groupBox_3);
        label8->setObjectName(QStringLiteral("label8"));

        gridLayout->addWidget(label8, 2, 2, 1, 1);

        label1 = new QLabel(groupBox_3);
        label1->setObjectName(QStringLiteral("label1"));

        gridLayout->addWidget(label1, 2, 0, 1, 1);

        label17 = new QLabel(groupBox_3);
        label17->setObjectName(QStringLiteral("label17"));

        gridLayout->addWidget(label17, 7, 4, 1, 1);

        label12 = new QLabel(groupBox_3);
        label12->setObjectName(QStringLiteral("label12"));

        gridLayout->addWidget(label12, 9, 2, 1, 1);

        m_PosiRepoPrice = new QLabel(groupBox_3);
        m_PosiRepoPrice->setObjectName(QStringLiteral("m_PosiRepoPrice"));

        gridLayout->addWidget(m_PosiRepoPrice, 9, 3, 1, 1);

        m_ContractDate = new QLabel(groupBox_3);
        m_ContractDate->setObjectName(QStringLiteral("m_ContractDate"));

        gridLayout->addWidget(m_ContractDate, 8, 1, 1, 1);

        m_AlphaLastPrice = new QLabel(groupBox_3);
        m_AlphaLastPrice->setObjectName(QStringLiteral("m_AlphaLastPrice"));

        gridLayout->addWidget(m_AlphaLastPrice, 7, 5, 1, 1);

        m_ContractDays = new QLabel(groupBox_3);
        m_ContractDays->setObjectName(QStringLiteral("m_ContractDays"));

        gridLayout->addWidget(m_ContractDays, 7, 1, 1, 1);

        label15 = new QLabel(groupBox_3);
        label15->setObjectName(QStringLiteral("label15"));

        gridLayout->addWidget(label15, 2, 4, 1, 1);

        m_DeliveryDate = new QLabel(groupBox_3);
        m_DeliveryDate->setObjectName(QStringLiteral("m_DeliveryDate"));

        gridLayout->addWidget(m_DeliveryDate, 2, 5, 1, 1);

        m_ProductId = new QLabel(groupBox_3);
        m_ProductId->setObjectName(QStringLiteral("m_ProductId"));

        gridLayout->addWidget(m_ProductId, 5, 1, 1, 1);

        m_AlphaInstrumentId = new QLabel(groupBox_3);
        m_AlphaInstrumentId->setObjectName(QStringLiteral("m_AlphaInstrumentId"));

        gridLayout->addWidget(m_AlphaInstrumentId, 5, 5, 1, 1);

        label_quote = new QLabel(groupBox_3);
        label_quote->setObjectName(QStringLiteral("label_quote"));

        gridLayout->addWidget(label_quote, 0, 0, 1, 3);

        label14 = new QLabel(groupBox_3);
        label14->setObjectName(QStringLiteral("label14"));

        gridLayout->addWidget(label14, 2, 6, 1, 1);

        label6 = new QLabel(groupBox_3);
        label6->setObjectName(QStringLiteral("label6"));

        gridLayout->addWidget(label6, 5, 6, 1, 1);

        label7 = new QLabel(groupBox_3);
        label7->setObjectName(QStringLiteral("label7"));

        gridLayout->addWidget(label7, 7, 6, 1, 1);

        m_DeadlineDate = new QLabel(groupBox_3);
        m_DeadlineDate->setObjectName(QStringLiteral("m_DeadlineDate"));

        gridLayout->addWidget(m_DeadlineDate, 2, 7, 1, 1);

        label13 = new QLabel(groupBox_3);
        label13->setObjectName(QStringLiteral("label13"));

        gridLayout->addWidget(label13, 8, 4, 1, 1);

        m_BravoInstrumentId = new QLabel(groupBox_3);
        m_BravoInstrumentId->setObjectName(QStringLiteral("m_BravoInstrumentId"));

        gridLayout->addWidget(m_BravoInstrumentId, 8, 5, 1, 1);

        m_FlexReveRepoPrice = new QLabel(groupBox_3);
        m_FlexReveRepoPrice->setObjectName(QStringLiteral("m_FlexReveRepoPrice"));

        gridLayout->addWidget(m_FlexReveRepoPrice, 7, 7, 1, 1);

        m_FlexReveRepoRatio = new QLabel(groupBox_3);
        m_FlexReveRepoRatio->setObjectName(QStringLiteral("m_FlexReveRepoRatio"));

        gridLayout->addWidget(m_FlexReveRepoRatio, 5, 7, 1, 1);

        label5 = new QLabel(groupBox_3);
        label5->setObjectName(QStringLiteral("label5"));

        gridLayout->addWidget(label5, 9, 4, 1, 1);

        m_BravoLastPrice = new QLabel(groupBox_3);
        m_BravoLastPrice->setObjectName(QStringLiteral("m_BravoLastPrice"));

        gridLayout->addWidget(m_BravoLastPrice, 9, 5, 1, 1);

        label0 = new QLabel(groupBox_3);
        label0->setObjectName(QStringLiteral("label0"));

        gridLayout->addWidget(label0, 8, 6, 1, 1);

        m_SystemDate = new QLabel(groupBox_3);
        m_SystemDate->setObjectName(QStringLiteral("m_SystemDate"));

        gridLayout->addWidget(m_SystemDate, 8, 7, 1, 1);

        label18 = new QLabel(groupBox_3);
        label18->setObjectName(QStringLiteral("label18"));

        gridLayout->addWidget(label18, 9, 6, 1, 1);

        m_SystemTime = new QLabel(groupBox_3);
        m_SystemTime->setObjectName(QStringLiteral("m_SystemTime"));

        gridLayout->addWidget(m_SystemTime, 9, 7, 1, 1);


        horizontalLayout->addWidget(groupBox_3);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(2, 2);

        verticalLayout->addWidget(MainWindow_2);

        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 9, -1, -1);
        groupBox_2 = new QGroupBox(widget_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 0, -1, 6);
        line2 = new QFrame(groupBox_2);
        line2->setObjectName(QStringLiteral("line2"));
        line2->setFrameShape(QFrame::HLine);
        line2->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line2);

        label_sub = new QLabel(groupBox_2);
        label_sub->setObjectName(QStringLiteral("label_sub"));

        verticalLayout_3->addWidget(label_sub);

        sub_tbl_view_ = new QTableView(groupBox_2);
        sub_tbl_view_->setObjectName(QStringLiteral("sub_tbl_view_"));

        verticalLayout_3->addWidget(sub_tbl_view_);


        horizontalLayout_3->addWidget(groupBox_2);


        verticalLayout->addWidget(widget_2);

        verticalLayout->setStretch(2, 1);
        MainWindow->setCentralWidget(centralwidget);
        statusBar2 = new QStatusBar(MainWindow);
        statusBar2->setObjectName(QStringLiteral("statusBar2"));
        MainWindow->setStatusBar(statusBar2);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\344\270\234\350\210\252\351\207\221\350\236\215\344\273\223\345\215\225\345\233\236\350\264\255\347\263\273\347\273\237\346\212\245\344\273\267\347\273\210\347\253\257", 0));
        lab_Ico->setText(QString());
        lab_Title->setText(QApplication::translate("MainWindow", "\344\270\234\350\210\252\351\207\221\350\236\215\344\273\223\345\215\225\347\273\274\345\220\210\344\272\244\346\230\223\347\263\273\347\273\237\346\212\245\344\273\267\347\273\210\347\253\257", 0));
#ifndef QT_NO_TOOLTIP
        btnMenu->setToolTip(QApplication::translate("MainWindow", "\350\217\234\345\215\225", 0));
#endif // QT_NO_TOOLTIP
        btnMenu->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Min->setToolTip(QApplication::translate("MainWindow", "\346\234\200\345\260\217\345\214\226", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Min->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Max->setToolTip(QApplication::translate("MainWindow", "\346\234\200\345\244\247\345\214\226", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Max->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Close->setToolTip(QApplication::translate("MainWindow", "\345\205\263\351\227\255", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Close->setText(QString());
        groupBox->setTitle(QString());
        label_syms->setText(QApplication::translate("MainWindow", "\345\220\210\347\272\246\345\210\227\350\241\250", 0));
        update_symlst_btn_->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260", 0));
        sub_btn_->setText(QApplication::translate("MainWindow", "\350\256\242\351\230\205\346\211\200\346\234\211", 0));
        unsub_btn_->setText(QApplication::translate("MainWindow", "\351\200\200\350\256\242\346\211\200\346\234\211", 0));
        groupBox_3->setTitle(QString());
        m_DiscountPrice->setText(QString());
        label10->setText(QApplication::translate("MainWindow", "\345\210\260\346\234\237\351\200\206\345\233\236\350\264\255\347\216\207", 0));
        label16->setText(QApplication::translate("MainWindow", "\347\254\254\344\270\200\346\234\237\350\264\247", 0));
        label2->setText(QApplication::translate("MainWindow", "\345\223\201\347\247\215", 0));
        label4->setText(QApplication::translate("MainWindow", "\345\233\236\350\264\255\346\227\245", 0));
        m_DiscountRatio->setText(QString());
        label9->setText(QApplication::translate("MainWindow", "\346\255\243\345\233\236\350\264\255\347\216\207", 0));
        m_PosiRepoRatio->setText(QString());
        label11->setText(QApplication::translate("MainWindow", "\350\264\264\347\216\260\344\273\267", 0));
        m_ReveRepoRatio->setText(QString());
        m_ExchangeId->setText(QString());
        label3->setText(QApplication::translate("MainWindow", "\350\264\264\347\216\260/\345\233\236\350\264\255\345\244\251\346\225\260", 0));
        label8->setText(QApplication::translate("MainWindow", "\350\264\264\347\216\260\347\216\207", 0));
        label1->setText(QApplication::translate("MainWindow", "\344\272\244\346\230\223\346\211\200", 0));
        label17->setText(QApplication::translate("MainWindow", "\347\254\254\344\270\200\346\234\237\350\264\247\344\273\267", 0));
        label12->setText(QApplication::translate("MainWindow", "\346\255\243\345\233\236\350\264\255\344\273\267", 0));
        m_PosiRepoPrice->setText(QString());
        m_ContractDate->setText(QString());
        m_AlphaLastPrice->setText(QString());
        m_ContractDays->setText(QString());
        label15->setText(QApplication::translate("MainWindow", "\346\234\211\346\225\210\344\272\244\345\211\262\346\227\245", 0));
        m_DeliveryDate->setText(QString());
        m_ProductId->setText(QString());
        m_AlphaInstrumentId->setText(QString());
        label_quote->setText(QApplication::translate("MainWindow", "\346\237\245\350\257\242\346\212\245\344\273\267", 0));
        label14->setText(QApplication::translate("MainWindow", "\347\201\265\346\264\273\351\200\206\345\233\236\350\264\255\346\210\252\346\255\242\346\227\245", 0));
        label6->setText(QApplication::translate("MainWindow", "\347\201\265\346\264\273\351\200\206\345\233\236\350\264\255\347\216\207", 0));
        label7->setText(QApplication::translate("MainWindow", "\347\201\265\346\264\273\351\200\206\345\233\236\350\264\255\344\273\267", 0));
        m_DeadlineDate->setText(QString());
        label13->setText(QApplication::translate("MainWindow", "\347\254\254\344\272\214\346\234\237\350\264\247", 0));
        m_BravoInstrumentId->setText(QString());
        m_FlexReveRepoPrice->setText(QString());
        m_FlexReveRepoRatio->setText(QString());
        label5->setText(QApplication::translate("MainWindow", "\347\254\254\344\272\214\346\234\237\350\264\247\344\273\267", 0));
        m_BravoLastPrice->setText(QString());
        label0->setText(QApplication::translate("MainWindow", "\346\233\264\346\226\260\346\227\245\346\234\237", 0));
        m_SystemDate->setText(QString());
        label18->setText(QApplication::translate("MainWindow", "\346\233\264\346\226\260\346\227\266\351\227\264", 0));
        m_SystemTime->setText(QString());
        groupBox_2->setTitle(QString());
        label_sub->setText(QApplication::translate("MainWindow", "\350\256\242\351\230\205\346\212\245\344\273\267", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WRUICLIENT_H
