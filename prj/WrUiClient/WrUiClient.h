// 界面。用WRClient与服务器通信，并展示WRClient收到的反馈

#ifndef WRUICLIENT_H
#define WRUICLIENT_H

#include <QMainWindow>
#include <QDialog>
#include <QProcess>
#include "ui_wruiclient.h"
#include "ui_LoginDlg.h"
#include "ui_UpdateDlg.h"
#include "WRClient.h"
#include "substblmodel.h"
#include "SymListModel.h"

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

class WrLoginDlg : public BaseDlg, public Ui::LoginDlg
{
	Q_OBJECT

public:
	WrLoginDlg(WRClient *wr_client, QWidget *parent = 0);

private slots:
	// 响应button等控件的事件
	void onLogin();
	// 联系作者
	void onAuthor();
	// 响应服务器反馈
	void SlotLogin(const QString &);
	
private:
	WRClient *wr_client_;
};

class UpdateDlg : public BaseDlg, public Ui::UpdateDlg
{
	Q_OBJECT

public:
	UpdateDlg(const QString &, QWidget *parent = 0);

private slots:
	void on_ok_btn_clicked();
	void on_cancel_btn_clicked();
};

class WrUiClient : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:
	WrUiClient(WRClient *wr_client, QWidget *parent = 0);
	virtual ~WrUiClient();

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *);

private:
	void createSymLstVew();
	void createSubTblVew();
	void connectSignalSlot();

	QPoint mousePoint;
	bool mousePressed;
	bool max;
	QRect location;

	SymListModel *sym_mdl_;
	SubsTblModel *sub_mdl_;

	WRClient *wr_client_;
	CSymbol *syms_;//合约列表
	int syms_num_;//合约个数

	QProcess pro_;

private slots: 
	// 响应button等控件的事件
	void onTestWrite();
	void onSub();
	void onUnsub();
	void onQuote(int = 0);
	void on_btnMenu_Close_clicked();
	void on_btnMenu_Max_clicked();
	void on_btnMenu_Min_clicked();
	void on_btnMenu_clicked();

	// 响应服务器反馈
	void SlotSymbols(CSymbol *, int);
	void SlotQuote(const CQuotation *);
	void SlotNetErr(const QString &);
};

#endif // WRUICLIENT_H
