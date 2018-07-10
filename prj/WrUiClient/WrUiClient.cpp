#include "wruiclient.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QGroupBox>
#include <QTableView>
#include <QPixmap>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QFile>
#include <QSettings>
#include <QSizeGrip>
#include <QSystemTrayIcon>
#include "iconhelper.h"
#include "BaseLibDefine.h"
#include "Baselib/Global.h"

BaseDlg::BaseDlg(QWidget *parent) : QDialog(parent)
{
	this->mousePressed = false;
	//设置窗体标题栏隐藏
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

}

BaseDlg::~BaseDlg(){

}

WrLoginDlg::WrLoginDlg(WRClient *wr_client, QWidget *parent) : BaseDlg(parent),wr_client_(wr_client)
{
	setupUi(this);
	// 设置图标
	QIcon *icon = new QIcon(":/image/logo.png");
	QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(*icon);
	setWindowIcon(*icon);

	//设置图形字体
	//IconHelper::Instance()->SetIcon(lab_Ico, QChar(0xf183), 12);
	IconHelper::Instance()->SetIcon(btnMenu, QChar(0xf1d6), 10);
	IconHelper::Instance()->SetIcon(cancel_btn_, QChar(0xf00d), 10);

	//账号密码
	QSettings settings("kiiik", "WrUiClient");
	usr_name_edit_->setText(settings.value("login/usr_name").toString());
	pwd_edit_->setEchoMode(QLineEdit::Password);
	pwd_edit_->setText(settings.value("login/pwd").toString());

	// 连接信号、槽
	bool ret = false;

	ret = QObject::connect(login_btn_, SIGNAL(clicked()), this, SLOT(onLogin()));
	ret = QObject::connect(cancel_btn_, SIGNAL(clicked()), this, SLOT(reject()));
	ret = QObject::connect(btnMenu, SIGNAL(clicked()), this, SLOT(onAuthor()));
	ret = QObject::connect(wr_client_, SIGNAL(SignalNetErr(const QString &)), log_label_, SLOT(setText(const QString &)));
	ret = QObject::connect(wr_client_, SIGNAL(SignalLogin(const QString&)), this, SLOT(SlotLogin(const QString &)));
}

void WrLoginDlg::onAuthor(){
	log_label_->setText(QStringLiteral("联系方式：\n东航金控有限责任公司\nwww.kiiik.com"));
}

void WrLoginDlg::onLogin(){
	QString user_id = usr_name_edit_->text();
	QString pwd = pwd_edit_->text();
#if 1
	wr_client_->Login((char*)user_id.toStdString().c_str(), (char*)pwd.toStdString().c_str());
#else
	wr_client_->Login("admin", "123456");
#endif
	
}

void WrLoginDlg::SlotLogin(const QString &text){
	log_label_->setText(text);
	if (text == QStringLiteral("登陆成功")) {
		// 保存账号 密码
		QSettings settings("kiiik", "WrUiClient");
		settings.setValue("login/usr_name", usr_name_edit_->text());
		settings.setValue("login/pwd", pwd_edit_->text());
		this->accept();
	}
	else {
		if (text == QStringLiteral("账号不存在")){
			usr_name_edit_->setText("");
			pwd_edit_->setText("");
		}
		else{
			pwd_edit_->setText("");
		}
	}
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


WrUiClient::WrUiClient(WRClient *wr_client, QWidget *parent)
	: QMainWindow(parent)
	, syms_(NULL)
	, wr_client_(wr_client)
{
	setupUi(this);

	// 设置图标
	QIcon *icon = new QIcon(":/image/logo.png");
	QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(*icon);
	setWindowIcon(*icon);

	//设置窗体标题栏隐藏
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
	location = this->geometry();
	max = false;
	mousePressed = false;
	//安装事件监听器,让标题栏识别鼠标双击
	lab_Title->installEventFilter(this);
	IconHelper::Instance()->SetIcon(btnMenu_Close, QChar(0xf00d), 10);
	IconHelper::Instance()->SetIcon(btnMenu_Max, QChar(0xf096), 10);
	IconHelper::Instance()->SetIcon(btnMenu_Min, QChar(0xf068), 10);
	IconHelper::Instance()->SetIcon(btnMenu, QChar(0xf0aa), 10);
	btnMenu->setToolTip(QStringLiteral("检测新版本"));
	//IconHelper::Instance()->SetIcon(lab_Ico, QChar(0xf015), 12);


	createSymLstVew();
	createSubTblVew();

	connectSignalSlot();

	wr_client_->GetSymbols();
}

bool WrUiClient::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonDblClick) {
		this->on_btnMenu_Max_clicked();
		return true;
	}
	return QObject::eventFilter(obj, event);
}

void WrUiClient::mouseMoveEvent(QMouseEvent *e)
{
	if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
		this->move(e->globalPos() - mousePoint);
		e->accept();
	}
}

void WrUiClient::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		mousePressed = true;
		mousePoint = e->globalPos() - this->pos();
		e->accept();
	}
}

void WrUiClient::mouseReleaseEvent(QMouseEvent *)
{
	mousePressed = false;
}

void WrUiClient::on_btnMenu_Close_clicked()
{
	qApp->exit();
}

void WrUiClient::on_btnMenu_Max_clicked()
{
	if (max) {
		this->setGeometry(location);
		IconHelper::Instance()->SetIcon(btnMenu_Max, QChar(0xf096), 10);
		btnMenu_Max->setToolTip(QStringLiteral("最大化"));
	}
	else {
		location = this->geometry();
		this->setGeometry(qApp->desktop()->availableGeometry());
		IconHelper::Instance()->SetIcon(btnMenu_Max, QChar(0xf079), 10);
		btnMenu_Max->setToolTip(QStringLiteral("还原"));
	}
	max = !max;
}

void WrUiClient::on_btnMenu_Min_clicked()
{
	this->showMinimized();
}

void WrUiClient::on_btnMenu_clicked(){
	wr_client_->CheckVersion();
}

void WrUiClient::connectSignalSlot(){
	bool ret = false;
	
	ret = QObject::connect(update_symlst_btn_, SIGNAL(clicked()), wr_client_, SLOT(GetSymbols()));
	ret = QObject::connect(sub_btn_, SIGNAL(clicked()), this, SLOT(onSub()));
	ret = QObject::connect(unsub_btn_, SIGNAL(clicked()), this, SLOT(onUnsub()));
	
	ret = QObject::connect(wr_client_, SIGNAL(SignalNetErr(const QString &)), this, SLOT(SlotNetErr(const QString &)));
	ret = QObject::connect(wr_client_, SIGNAL(SignalSymbols(CSymbol *, int)), this, SLOT(SlotSymbols(CSymbol *, int)));
	ret = QObject::connect(wr_client_, SIGNAL(SignalQuote(const CQuotation *)), this, SLOT(SlotQuote(const CQuotation *)));
	ret = QObject::connect(wr_client_, SIGNAL(SignalTickRsp(const CQuotation *)), sub_mdl_, SLOT(SlotTickRsp(const CQuotation *)));

	ret = QObject::connect(sym_mdl_, SIGNAL(SignalRmSym(const CSymbol &)), sub_mdl_, SLOT(SlotRmSym(const CSymbol &)));
}

WrUiClient::~WrUiClient()
{
	if (syms_) delete[]syms_;
	//if (wr_client_) delete wr_client_;
}

void WrUiClient::SlotNetErr(const QString &msg){
	statusBar2->showMessage(msg, 5000);
	// 重连 重新订阅
	if (msg == QStringLiteral("  网络重新连上")){
		std::vector<CSymbol> inst_vec;
		sub_mdl_->GetSymVec(inst_vec);
		wr_client_->Sub(inst_vec);
	}
	else if (msg == QStringLiteral("  有新版本，请更新")){
		UpdateDlg up_dlg(QStringLiteral("有更新版本，是否更新？"));
		if (up_dlg.exec()){
			wr_client_->DownloadVersion();
		}
		else{

		}
	}
	else if (msg == QStringLiteral("  下载完毕，是否安装")){
		UpdateDlg up_dlg(QStringLiteral("更新已下载，是否关闭程序并安装？"));
		if (up_dlg.exec()){
			// 启动update程序，进行新版本文件覆盖
			QString kiiik_home = baselib::KiiikHome().c_str();
#ifdef DEBUG
			pro_.start(kiiik_home + "bin\\Debug\\VersionUpdate.exe");
#else
			pro_.start(kiiik_home + "bin\\VersionUpdate.exe");
#endif // DEBUG		
		}
		else{

		}
	}
	else if (msg == QStringLiteral("  目前是最新版本")){
		btnMenu->setEnabled(false);
		UpdateDlg up_dlg(QStringLiteral("当前已是最新版本"));
		up_dlg.exec();
	}
}

void WrUiClient::SlotSymbols(CSymbol *syms, int n){
	
	if (syms_) delete[]syms_;
	syms_ = syms;
	syms_num_ = n;

	sym_mdl_->SetSymVec(syms, n);
}


#define  LABEL_SHOW(field) \
	tick->field > 0.001 ? field->setText(QString::number(tick->field)) : field->setText("--");

#define  LABEL_SHOW_RATIO(ratio) \
if (tick->ratio > 0.001) \
{ \
	char out[20] = { 0 }; \
	sprintf(out, "%.6lf", tick->ratio); \
	ratio->setText(out); \
} \
else \
	ratio->setText("--");

#define  LABEL_SHOW_PRICE(price) \
if (tick->price > 0.001) \
{ \
	char out[20] = { 0 }; \
	sprintf(out, "%.1lf", tick->price); \
	price->setText(out); \
} \
else \
	price->setText("--");

void WrUiClient::SlotQuote(const CQuotation *tick){
	QString ss = QString::fromStdString(tick->symbol.Str()) + " " + QString::number(tick->m_SystemDate)
		+ " " + QString::number(tick->m_SystemTime) + " " + QString::number(tick->m_AlphaLastPrice, 'g', 3);
	
	m_ExchangeId->setText(tick->symbol.m_ExchangeId);
	m_ProductId->setText(tick->symbol.m_ProductId);
	if (tick->symbol.m_ContractDays == 0)
		m_ContractDays->setText(QStringLiteral("贴现"));
	else
		m_ContractDays->setText(QString::number(tick->symbol.m_ContractDays));
	m_ContractDate->setText(formatDate(tick->m_ContractDate));
	
		LABEL_SHOW_RATIO(m_DiscountRatio)
		LABEL_SHOW_RATIO(m_PosiRepoRatio)
		LABEL_SHOW_RATIO(m_ReveRepoRatio)
		LABEL_SHOW_PRICE(m_DiscountPrice)
		LABEL_SHOW_PRICE(m_PosiRepoPrice)


		m_DeliveryDate->setText(formatDate(tick->m_DeliveryDate));
		m_AlphaInstrumentId->setText(tick->m_AlphaInstrumentId);
		LABEL_SHOW(m_AlphaLastPrice)
		m_BravoInstrumentId->setText(tick->m_BravoInstrumentId);
		LABEL_SHOW(m_BravoLastPrice)

	
		
		m_DeadlineDate->setText(formatDate(tick->m_DeadlineDate));
		LABEL_SHOW_RATIO(m_FlexReveRepoRatio)
		LABEL_SHOW_PRICE(m_FlexReveRepoPrice)
		m_SystemDate->setText(formatDate(tick->m_SystemDate));
		m_SystemTime->setText(formatTime(tick->m_SystemTime));

	delete tick;
}



void WrUiClient::onTestWrite(){
	wr_client_->TestTimerWrite();
}
void WrUiClient::onSub(){
	std::vector<CSymbol> syms;
	if (syms_){
		for (int i = 0; i < syms_num_; ++i)
			syms.push_back(syms_[i]);
	}
	wr_client_->Sub(syms);
	sym_mdl_->ChangeState(Qt::Checked);
}

void WrUiClient::onUnsub(){
	std::vector<CSymbol> syms;
	if (syms_){
		for (int i = 0; i < syms_num_; ++i)
			syms.push_back(syms_[i]);
	}
	wr_client_->UnSub(syms);
	sym_mdl_->ChangeState(Qt::Unchecked);
}

// idx :  QComboBox的第几个
void WrUiClient::onQuote(int idx){	
	wr_client_->Quote(syms_[idx]);
}


void WrUiClient::createSubTblVew(){
	
	sub_mdl_ = new SubsTblModel(this);
	sub_tbl_view_->setModel(sub_mdl_);

	sub_tbl_view_->verticalHeader()->setDefaultSectionSize(25);
	sub_tbl_view_->verticalHeader()->setVisible(false);
	sub_tbl_view_->horizontalHeader()->setFixedHeight(30);
	sub_tbl_view_->horizontalHeader()->setDefaultSectionSize(90);
	sub_tbl_view_->horizontalHeader()->resizeSection(2, 100);
	sub_tbl_view_->horizontalHeader()->resizeSection(14, 110);
	sub_tbl_view_->horizontalHeader()->setStretchLastSection(true);//最后一列填满表格剩余空间
	sub_tbl_view_->setSelectionBehavior(QAbstractItemView::SelectRows);//按行选择
	sub_tbl_view_->setSelectionMode(QAbstractItemView::SingleSelection);//每次只可选择一行

	sub_tbl_view_->setSortingEnabled(true);
}

void WrUiClient::createSymLstVew(){
	sym_mdl_ = new SymListModel(wr_client_, this);
	syms_lst_view_->setModel(sym_mdl_);

	syms_lst_view_->verticalHeader()->setDefaultSectionSize(25);
	syms_lst_view_->verticalHeader()->setVisible(false);
	syms_lst_view_->horizontalHeader()->setFixedHeight(30);
	syms_lst_view_->horizontalHeader()->setDefaultSectionSize(100);

	syms_lst_view_->horizontalHeader()->setStretchLastSection(true);//最后一列填满表格剩余空间
	//syms_lst_view_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//按内容调整列宽
	syms_lst_view_->setSelectionBehavior(QAbstractItemView::SelectRows);//按行选择
	syms_lst_view_->setSelectionMode(QAbstractItemView::SingleSelection);//每次只可选择一行


	//捕捉单击、双击某行事件，用于quote
	bool ret = QObject::connect(syms_lst_view_, SIGNAL(doubleClicked(const QModelIndex &)), sym_mdl_, SLOT(SlotRowClicked(const QModelIndex &)));
	ret = QObject::connect(syms_lst_view_, SIGNAL(clicked(const QModelIndex &)), sym_mdl_, SLOT(SlotRowClicked(const QModelIndex &)));
}

UpdateDlg::UpdateDlg(const QString &content, QWidget *parent) 
	: BaseDlg(parent)
{
	setupUi(this);
	label_->setText(content);
	IconHelper::Instance()->SetIcon(cancel_btn_, QChar(0xf00d), 10);
	QObject::connect(cancel_btn_, SIGNAL(clicked()), this, SLOT(reject()));

	if (content == QStringLiteral("当前已是最新版本"))
		cancel_btn->setVisible(false);
}

void UpdateDlg::on_ok_btn_clicked(){
	this->accept();

}

void UpdateDlg::on_cancel_btn_clicked(){
	this->reject();
}