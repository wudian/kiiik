#include "versionupdate.h"
#include <string>
#include <fstream>
#include <QProcess>
#include "iconhelper.h"

BaseDlg::BaseDlg(QWidget *parent) : QDialog(parent)
{
	this->mousePressed = false;
	//设置窗体标题栏隐藏
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

}

BaseDlg::~BaseDlg(){

}

void BaseDlg::mouseMoveEvent(QMouseEvent *e)
{
	if (mousePressed && (e->buttons() && Qt::LeftButton)) {
		this->move(e->globalPos() - mousePoint);
		e->accept();
	}
}

void BaseDlg::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		mousePressed = true;
		mousePoint = e->globalPos() - this->pos();
		e->accept();
	}
}

void BaseDlg::mouseReleaseEvent(QMouseEvent *)
{
	mousePressed = false;
}

VersionUpdate::VersionUpdate(QWidget *parent)
: BaseDlg(parent)
{
	setupUi(this);
	IconHelper::Instance()->SetIcon(cancel_btn_, QChar(0xf00d), 10);
	QObject::connect(cancel_btn_, SIGNAL(clicked()), this, SLOT(reject()));
}

VersionUpdate::~VersionUpdate()
{

}

extern std::string KiiikHome();
extern QProcess *pro_;

void VersionUpdate::on_pushButton_yes_clicked(){
	//std::ofstream ofs("2.log");
	QString cmd = KiiikHome().c_str();
#ifdef DEBUG
	cmd += "bin\\Debug\\WrUiClient.exe";
#else
	cmd += "bin\\WrUiClient.exe";
#endif // DEBUG

	pro_->start(cmd);
	//pro_->waitForFinished();
	//ofs << cmd.toStdString() << std::endl;
	this->accept();
}

void VersionUpdate::on_pushButton_no_clicked(){
	this->reject();
}
