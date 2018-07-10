
#include <QtWidgets/QApplication>
#include <QFile>
#include <QTranslator>
#include "wruiclient.h"
#include "Baselib/Global.h"

using namespace kiiik;

//设置皮肤样式
static void SetStyle()
{
	QFile file(QString(":/image/black.css"));
	file.open(QFile::ReadOnly);
	QString qss = QLatin1String(file.readAll());
	qApp->setStyleSheet(qss);
	qApp->setPalette(QPalette(QColor("#F0F0F0")));
}

//加载中文字符
static void SetChinese()
{
	QTranslator *translator = new QTranslator(qApp);
	translator->load(":/image/qt_zh_TW.qm");
	//qApp->installTranslator(translator);

}

int main(int argc, char *argv[])
{
	baselib::InitKiiik(argv);

	QApplication a(argc, argv);

	
	
	SetStyle();
	SetChinese();
	
	WRClient wr_client;

	WrLoginDlg login_dlg(&wr_client);
	if (!login_dlg.exec()) return -1;

	WrUiClient w(&wr_client);
	w.show();
	return a.exec();
}
