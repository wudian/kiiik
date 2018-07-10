
#include <QtWidgets/QApplication>
#include <QDir>
#include <QFile>
#include <QTranslator>
#include <string>
#include <fstream>
#include <QProcess>
#include "versionupdate.h"

using namespace std;

string g_KiiikHome;
//ofstream ofs("VersionUpdate.log");
QProcess *pro_;

void InitKiiik(char *argv[]) {
	string _KiiikHome = argv[0];
	size_t _bin = _KiiikHome.find("bin");
	g_KiiikHome = _KiiikHome.substr(0, _bin);
}

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

string KiiikHome() { return g_KiiikHome == "" ? "E:\\Kiiik\\" : g_KiiikHome; }

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	InitKiiik(argv);

	SetStyle();
	SetChinese();

	pro_ = new QProcess;

	/*ConfirmDialog confirm_dialog;
	if (!confirm_dialog.exec()) return -1;*/
	pro_->start("taskkill /im WrUiClient.exe /f");
	pro_->waitForFinished();

	
#ifdef DEBUG
	QString update_folder = (KiiikHome() + "update\\").c_str();
	QString bin_folder = (KiiikHome() + "bin\\Debug\\").c_str();
#else
	QString update_folder = (KiiikHome() + "update\\").c_str();
	QString bin_folder = (KiiikHome() + "bin\\").c_str();
#endif // DEBUG

	QString config_folder = (KiiikHome() + "config\\").c_str();

	QDir update_dir(update_folder);
	foreach(QFileInfo info, update_dir.entryInfoList(QDir::Files)) {
		QString path = info.absoluteFilePath();
		QString name = info.fileName();
		if (path.contains(".xml")) {
			QDir config_dir(config_folder);
			if (config_dir.exists(name))
				config_dir.remove(name);
			if (!QFile::copy(path, config_folder + name))
				;//ofs << "copy " << path.toStdString() << " fail\n";
			else
				;//ofs << "copy " << path.toStdString() << " success\n";
		}
		else {
			QDir bin_dir(bin_folder);
			if (bin_dir.exists(name))
				bin_dir.remove(name);
			if (!QFile::copy(path, bin_folder + name))
				;//ofs << "copy " << path.toStdString() << " fail\n";
			else
				;//ofs << "copy " << path.toStdString() << " success\n";
		}
	}


	VersionUpdate w;
	w.show();
	return a.exec();
}
