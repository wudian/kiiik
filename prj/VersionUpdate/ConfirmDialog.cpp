#include "ConfirmDialog.h"
#include <QProcess>

ConfirmDialog::ConfirmDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
}

ConfirmDialog::~ConfirmDialog()
{

}

extern QProcess *pro_;
void ConfirmDialog::on_pushButton_yes_clicked(){
	pro_->start("taskkill /im WrUiClient.exe /f");
	pro_->waitForFinished();
	this->accept();
}

void ConfirmDialog::on_pushButton_no_clicked(){
	this->reject();
}
