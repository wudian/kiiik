/********************************************************************************
** Form generated from reading UI file 'UpdateDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATEDLG_H
#define UI_UPDATEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UpdateDlg
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_title;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lab_Title;
    QWidget *widget_menu;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *cancel_btn_;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;

    void setupUi(QDialog *UpdateDlg)
    {
        if (UpdateDlg->objectName().isEmpty())
            UpdateDlg->setObjectName(QStringLiteral("UpdateDlg"));
        UpdateDlg->resize(251, 161);
        verticalLayout = new QVBoxLayout(UpdateDlg);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_title = new QWidget(UpdateDlg);
        widget_title->setObjectName(QStringLiteral("widget_title"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_title->sizePolicy().hasHeightForWidth());
        widget_title->setSizePolicy(sizePolicy);
        widget_title->setMinimumSize(QSize(100, 28));
        horizontalLayout_4 = new QHBoxLayout(widget_title);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        lab_Title = new QLabel(widget_title);
        lab_Title->setObjectName(QStringLiteral("lab_Title"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lab_Title->sizePolicy().hasHeightForWidth());
        lab_Title->setSizePolicy(sizePolicy1);
        lab_Title->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lab_Title->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(lab_Title);

        widget_menu = new QWidget(widget_title);
        widget_menu->setObjectName(QStringLiteral("widget_menu"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_menu->sizePolicy().hasHeightForWidth());
        widget_menu->setSizePolicy(sizePolicy2);
        horizontalLayout_2 = new QHBoxLayout(widget_menu);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        cancel_btn_ = new QPushButton(widget_menu);
        cancel_btn_->setObjectName(QStringLiteral("cancel_btn_"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(cancel_btn_->sizePolicy().hasHeightForWidth());
        cancel_btn_->setSizePolicy(sizePolicy3);
        cancel_btn_->setMinimumSize(QSize(40, 0));
        cancel_btn_->setCursor(QCursor(Qt::ArrowCursor));
        cancel_btn_->setFocusPolicy(Qt::NoFocus);
        cancel_btn_->setFlat(true);

        horizontalLayout_2->addWidget(cancel_btn_);


        horizontalLayout_4->addWidget(widget_menu);


        verticalLayout->addWidget(widget_title);

        widget_2 = new QWidget(UpdateDlg);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_ = new QLabel(widget_2);
        label_->setObjectName(QStringLiteral("label_"));

        horizontalLayout_3->addWidget(label_);


        verticalLayout->addWidget(widget_2);

        widget = new QWidget(UpdateDlg);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(55, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ok_btn = new QPushButton(widget);
        ok_btn->setObjectName(QStringLiteral("ok_btn"));

        horizontalLayout->addWidget(ok_btn);

        cancel_btn = new QPushButton(widget);
        cancel_btn->setObjectName(QStringLiteral("cancel_btn"));

        horizontalLayout->addWidget(cancel_btn);


        verticalLayout->addWidget(widget);


        retranslateUi(UpdateDlg);

        QMetaObject::connectSlotsByName(UpdateDlg);
    } // setupUi

    void retranslateUi(QDialog *UpdateDlg)
    {
        UpdateDlg->setWindowTitle(QApplication::translate("UpdateDlg", "Dialog", 0));
        lab_Title->setText(QApplication::translate("UpdateDlg", " \350\275\257\344\273\266\346\233\264\346\226\260", 0));
#ifndef QT_NO_TOOLTIP
        cancel_btn_->setToolTip(QApplication::translate("UpdateDlg", "\345\205\263\351\227\255", 0));
#endif // QT_NO_TOOLTIP
        cancel_btn_->setText(QString());
        label_->setText(QString());
        ok_btn->setText(QApplication::translate("UpdateDlg", "\347\241\256\345\256\232\357\274\210Y\357\274\211", 0));
        cancel_btn->setText(QApplication::translate("UpdateDlg", "\345\217\226\346\266\210\357\274\210N\357\274\211", 0));
    } // retranslateUi

};

namespace Ui {
    class UpdateDlg: public Ui_UpdateDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATEDLG_H
