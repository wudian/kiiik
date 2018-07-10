/********************************************************************************
** Form generated from reading UI file 'LoginDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDLG_H
#define UI_LOGINDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDlg
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_title;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lab_Ico;
    QLabel *lab_Title;
    QWidget *widget_menu;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnMenu;
    QPushButton *cancel_btn_;
    QGroupBox *groupBox_login;
    QGridLayout *gridLayout;
    QPushButton *login_btn_;
    QLineEdit *pwd_edit_;
    QLabel *label;
    QLabel *label_2;
    QLabel *log_label_;
    QLineEdit *usr_name_edit_;

    void setupUi(QDialog *LoginDlg)
    {
        if (LoginDlg->objectName().isEmpty())
            LoginDlg->setObjectName(QStringLiteral("LoginDlg"));
        LoginDlg->resize(243, 183);
        QIcon icon;
        icon.addFile(QStringLiteral(":/WrUiClient/Title.png"), QSize(), QIcon::Normal, QIcon::Off);
        LoginDlg->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(LoginDlg);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_title = new QWidget(LoginDlg);
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
        lab_Ico = new QLabel(widget_title);
        lab_Ico->setObjectName(QStringLiteral("lab_Ico"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lab_Ico->sizePolicy().hasHeightForWidth());
        lab_Ico->setSizePolicy(sizePolicy1);
        lab_Ico->setMinimumSize(QSize(31, 0));
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
        lab_Title->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lab_Title->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(lab_Title);

        widget_menu = new QWidget(widget_title);
        widget_menu->setObjectName(QStringLiteral("widget_menu"));
        sizePolicy1.setHeightForWidth(widget_menu->sizePolicy().hasHeightForWidth());
        widget_menu->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(widget_menu);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnMenu = new QPushButton(widget_menu);
        btnMenu->setObjectName(QStringLiteral("btnMenu"));
        btnMenu->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnMenu->sizePolicy().hasHeightForWidth());
        btnMenu->setSizePolicy(sizePolicy3);
        btnMenu->setMinimumSize(QSize(31, 0));
        btnMenu->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu->setFocusPolicy(Qt::NoFocus);
        btnMenu->setFlat(true);

        horizontalLayout->addWidget(btnMenu);

        cancel_btn_ = new QPushButton(widget_menu);
        cancel_btn_->setObjectName(QStringLiteral("cancel_btn_"));
        sizePolicy3.setHeightForWidth(cancel_btn_->sizePolicy().hasHeightForWidth());
        cancel_btn_->setSizePolicy(sizePolicy3);
        cancel_btn_->setMinimumSize(QSize(40, 0));
        cancel_btn_->setCursor(QCursor(Qt::ArrowCursor));
        cancel_btn_->setFocusPolicy(Qt::NoFocus);
        cancel_btn_->setFlat(true);

        horizontalLayout->addWidget(cancel_btn_);


        horizontalLayout_4->addWidget(widget_menu);


        verticalLayout->addWidget(widget_title);

        groupBox_login = new QGroupBox(LoginDlg);
        groupBox_login->setObjectName(QStringLiteral("groupBox_login"));
        gridLayout = new QGridLayout(groupBox_login);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        login_btn_ = new QPushButton(groupBox_login);
        login_btn_->setObjectName(QStringLiteral("login_btn_"));
        login_btn_->setCursor(QCursor(Qt::PointingHandCursor));
        login_btn_->setFocusPolicy(Qt::StrongFocus);
        login_btn_->setStyleSheet(QStringLiteral(""));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/update.png"), QSize(), QIcon::Normal, QIcon::Off);
        login_btn_->setIcon(icon1);
        login_btn_->setIconSize(QSize(20, 20));

        gridLayout->addWidget(login_btn_, 2, 4, 1, 1);

        pwd_edit_ = new QLineEdit(groupBox_login);
        pwd_edit_->setObjectName(QStringLiteral("pwd_edit_"));

        gridLayout->addWidget(pwd_edit_, 1, 2, 1, 3);

        label = new QLabel(groupBox_login);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_login);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        log_label_ = new QLabel(groupBox_login);
        log_label_->setObjectName(QStringLiteral("log_label_"));

        gridLayout->addWidget(log_label_, 2, 0, 1, 4);

        usr_name_edit_ = new QLineEdit(groupBox_login);
        usr_name_edit_->setObjectName(QStringLiteral("usr_name_edit_"));

        gridLayout->addWidget(usr_name_edit_, 0, 2, 1, 3);


        verticalLayout->addWidget(groupBox_login);

        QWidget::setTabOrder(usr_name_edit_, pwd_edit_);
        QWidget::setTabOrder(pwd_edit_, login_btn_);

        retranslateUi(LoginDlg);

        QMetaObject::connectSlotsByName(LoginDlg);
    } // setupUi

    void retranslateUi(QDialog *LoginDlg)
    {
        LoginDlg->setWindowTitle(QApplication::translate("LoginDlg", "\347\231\273\351\231\206", 0));
        lab_Ico->setText(QString());
        lab_Title->setText(QApplication::translate("LoginDlg", "\347\231\273\351\231\206\346\234\215\345\212\241\345\231\250", 0));
#ifndef QT_NO_TOOLTIP
        btnMenu->setToolTip(QApplication::translate("LoginDlg", "\350\217\234\345\215\225", 0));
#endif // QT_NO_TOOLTIP
        btnMenu->setText(QString());
#ifndef QT_NO_TOOLTIP
        cancel_btn_->setToolTip(QApplication::translate("LoginDlg", "\345\205\263\351\227\255", 0));
#endif // QT_NO_TOOLTIP
        cancel_btn_->setText(QString());
        groupBox_login->setTitle(QString());
        login_btn_->setText(QApplication::translate("LoginDlg", "\347\241\256\345\256\232(&O)", 0));
        label->setText(QApplication::translate("LoginDlg", "\347\224\250\346\210\267\345\220\215", 0));
        label_2->setText(QApplication::translate("LoginDlg", "\345\257\206\347\240\201", 0));
        log_label_->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginDlg: public Ui_LoginDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDLG_H
