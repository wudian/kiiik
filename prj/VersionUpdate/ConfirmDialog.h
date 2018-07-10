#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>
#include "ui_confirm.h"

class ConfirmDialog : public QDialog, public Ui::ConfirmDialog
{
	Q_OBJECT

public:
	ConfirmDialog(QWidget *parent=0);
	~ConfirmDialog();

private slots:
	void on_pushButton_yes_clicked();
	void on_pushButton_no_clicked();
	
};

#endif // CONFIRMDIALOG_H
