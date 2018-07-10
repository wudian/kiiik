#ifndef VERSIONUPDATE_H
#define VERSIONUPDATE_H

#include <QDialog>
#include <QMouseEvent>
#include "ui_versionupdate.h"

class BaseDlg : public QDialog {
	Q_OBJECT

protected:
	BaseDlg(QWidget *parent = 0);
	virtual ~BaseDlg();

	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *);


	QPoint mousePoint;              //鼠标拖动自定义标题栏时的坐标
	bool mousePressed;              //鼠标是否按下
};

class VersionUpdate : public BaseDlg, public Ui::VersionUpdateDialog
{
	Q_OBJECT

public:
	VersionUpdate(QWidget *parent = 0);
	virtual ~VersionUpdate();

private slots:
	void on_pushButton_yes_clicked();
	void on_pushButton_no_clicked();

private:
};

#endif // VERSIONUPDATE_H
