#include "SymListModel.h"
#include <QMutexLocker>
#include <QFont>
#include <QBrush>
#include "BaseLibDefine.h"

#define CHECK_COLUMN 3

SymListModel::SymListModel(WRClient *wr_client, QObject *parent)
	: QAbstractTableModel(parent)
	, wr_client_(wr_client)
{

}

SymListModel::~SymListModel()
{

}

int SymListModel::rowCount(const QModelIndex&) const{
	return sym_list_.size();
}
int SymListModel::columnCount(const QModelIndex &parent /* = QModelIndex() */)const{
	return 4;
}

static QMutex lock_;

QVariant SymListModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */)const
try
{
	QMutexLocker locker(&lock_);
	if (!index.isValid())
		return QVariant();

	if (role == Qt::TextAlignmentRole) {
			return Qt::AlignCenter;
	}
	else if (role == Qt::DisplayRole) {
		switch (index.column()){
		case 0:return sym_list_.at(index.row()).sym.m_ExchangeId;
		case 1:return sym_list_.at(index.row()).sym.m_ProductId;
		case 2:{
				   int days = sym_list_.at(index.row()).sym.m_ContractDays;
				   if (0 == days)return QStringLiteral("贴现"); else return days;
		}
		default:return QVariant();
		}
	}
	else if (role == Qt::CheckStateRole) {
		if (index.column() == CHECK_COLUMN){
			return sym_list_[index.row()].check == Qt::Checked ? Qt::Checked : Qt::Unchecked;
		}
	}

	return QVariant();
}
catch (...){
	return QVariant();
}

QVariant SymListModel::headerData(int section, Qt::Orientation orientation, int role)const{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal)return QVariant();
	switch (section){
	case 0:return QStringLiteral("交易所");
	case 1:return QStringLiteral("品种");
	case 2:return QStringLiteral("贴现/回购天数");
	case 3:return QStringLiteral("是否订阅");
	default:return QVariant();
	}
}

Qt::ItemFlags SymListModel::flags(const QModelIndex &index) const{
	if (!index.isValid())return 0;
	if (index.column() == CHECK_COLUMN)
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
	return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//根据界面选择QCheckbox，修改Model中的数据
bool SymListModel::setData(const QModelIndex &index, const QVariant &value, int role){
	if (!index.isValid())
		return false;
	if (role == Qt::CheckStateRole && index.column() == CHECK_COLUMN)
	{
		QMutexLocker locker(&lock_);
		if (value == Qt::Checked) {
			sym_list_[index.row()].check = Qt::Checked;
			std::vector<CSymbol> inst_vec;
			inst_vec.push_back(sym_list_[index.row()].sym);
			wr_client_->Sub(inst_vec);
		}			
		else {
			sym_list_[index.row()].check = Qt::Unchecked;
			std::vector<CSymbol> inst_vec;
			inst_vec.push_back(sym_list_[index.row()].sym);
			wr_client_->UnSub(inst_vec);
			emit SignalRmSym(sym_list_[index.row()].sym);
		}
	
		emit dataChanged(createIndex(index.row(), CHECK_COLUMN), createIndex(index.row(), CHECK_COLUMN));
	}
	return true;
}

void SymListModel::ChangeState(Qt::CheckState s){
	QMutexLocker locker(&lock_);
	for (int i = 0; i < sym_list_.size(); ++i){
		sym_list_[i].check = s;
		emit dataChanged(createIndex(i, CHECK_COLUMN), createIndex(i, CHECK_COLUMN));
		if (s == Qt::Unchecked)
			emit SignalRmSym(sym_list_[i].sym);
	}
}

void SymListModel::SetSymVec(CSymbol *syms, int n){

	// 如果软件刚启动，默认查询第一个合约
	if (sym_list_.empty()) {
		CSymbol quote_sym = syms[0];
		wr_client_->Quote(quote_sym);
	}

	{
		// 已经订阅的继续推送，刷新不影响其他地方的显示内容 --- 陈挺
		QMutexLocker locker(&lock_);

		QMap<CSymbol, Qt::CheckState> tmp;
		for (int i = 0; i < n; ++i){
			sym_list_.push_back(SymCheck(syms[i], Qt::Unchecked));
			tmp[sym_list_[i].sym] = sym_list_[i].check;
		}

		sym_list_.clear();
		for (int i = 0; i < n; ++i){
			Qt::CheckState check = Qt::Unchecked;
			QMap<CSymbol, Qt::CheckState>::iterator iter = tmp.find(syms[i]);
			if (iter != tmp.end()) check = iter.value();
			sym_list_.push_back(SymCheck(syms[i], check));
		}
	}
	
	endResetModel();
}

void SymListModel::SlotRowClicked(const QModelIndex &index){
	if (sym_list_.size() > index.row()) {
		CSymbol quote_sym = sym_list_[index.row()].sym;
		wr_client_->Quote(quote_sym);
	}
	
}

/*
template<int column> bool Compare(const SymListModel::SymCheck &lhs, const SymListModel::SymCheck &rhs);
FIELD_CMP_IMP(SymListModel::SymCheck, 0, sym)
FIELD_CMP_IMP(SymListModel::SymCheck, 1, sym)
FIELD_CMP_IMP(SymListModel::SymCheck, 2, sym)

void SymListModel::sort(int column, Qt::SortOrder order){
	QMutexLocker locker(&lock_);
	if (order == Qt::AscendingOrder){
		switch (column)
		{
			SORT_CASE(sym_list_, 0)
			SORT_CASE(sym_list_, 1)
			SORT_CASE(sym_list_, 2)
		
			default: break;
		}
	}
	
	
	endResetModel();
}
*/