#include "substblmodel.h"
#include <QMutexLocker>
#include <QFont>
#include "BaseLibDefine.h"

#define FIRST_COLUMN 0
#define LAST_COLUMN 18

SubsTblModel::SubsTblModel(QObject *parent)
	: QAbstractTableModel(parent)
{

}

SubsTblModel::~SubsTblModel()
{

}

static QMutex wr_quote_datas_mutex_;

int SubsTblModel::rowCount(const QModelIndex&) const{
	
	return wr_quote_datas_.size();
}
int SubsTblModel::columnCount(const QModelIndex &parent /* = QModelIndex() */)const{
	return LAST_COLUMN + 1;
}

//值为0的界面显示为"--"
#define SHOW(field) \
if (field > 0.001) \
	return QString::number(field); \
else \
	return "--";

//贴现率、正回购率、到期逆回购率、灵活逆回购率，这些率按小数6位末尾加0对齐
#define SHOW_RATIO(ratio) \
if (ratio > 0.001) \
{ \
	char out[20] = { 0 }; \
	sprintf(out, "%.6lf", ratio); \
	return out; \
} \
else \
	return "--";

//帖现价、正回购价、灵活逆回购价，这些价按小数1位对齐
#define SHOW_PRICE(price) \
if (price > 0.001) \
{ \
	char out[20] = { 0 }; \
	sprintf(out, "%.1lf", price); \
	return out; \
} \
else \
	return "--";

				 
QVariant SubsTblModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */)const
try
{
	QMutexLocker locker(&wr_quote_datas_mutex_);
	if (!index.isValid())
		return QVariant();

	if (role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	}
	else if (role == Qt::DisplayRole) {
		switch (index.column()){
		case 0:return wr_quote_datas_.at(index.row()).symbol.m_ExchangeId;
		case 1:return wr_quote_datas_.at(index.row()).symbol.m_ProductId;
		case 2:{
				   int days = wr_quote_datas_.at(index.row()).symbol.m_ContractDays;
				   if (0==days)return QStringLiteral("贴现"); else return days;
		}
		case 3:return formatDate(wr_quote_datas_.at(index.row()).m_ContractDate);
		case 4:{ SHOW_RATIO(wr_quote_datas_.at(index.row()).m_DiscountRatio) }
		case 5:{ SHOW_RATIO(wr_quote_datas_.at(index.row()).m_PosiRepoRatio) }
		case 6:{ SHOW_RATIO(wr_quote_datas_.at(index.row()).m_ReveRepoRatio) }
		case 7:{ SHOW_PRICE(wr_quote_datas_.at(index.row()).m_DiscountPrice) }
		case 8:{ SHOW_PRICE(wr_quote_datas_.at(index.row()).m_PosiRepoPrice) }
		case 9:return formatDate(wr_quote_datas_.at(index.row()).m_DeliveryDate);
		case 10:return wr_quote_datas_.at(index.row()).m_AlphaInstrumentId;
		case 11:{ SHOW(wr_quote_datas_.at(index.row()).m_AlphaLastPrice) }
		case 12:return wr_quote_datas_.at(index.row()).m_BravoInstrumentId;
		case 13:{ SHOW(wr_quote_datas_.at(index.row()).m_BravoLastPrice) }
		case 14:return formatDate(wr_quote_datas_.at(index.row()).m_DeadlineDate);
		case 15:{ SHOW_RATIO(wr_quote_datas_.at(index.row()).m_FlexReveRepoRatio) }
		case 16:{ SHOW_PRICE(wr_quote_datas_.at(index.row()).m_FlexReveRepoPrice) }
		case 17: return formatDate(wr_quote_datas_.at(index.row()).m_SystemDate);
		case 18: return formatTime(wr_quote_datas_.at(index.row()).m_SystemTime);
		default:return QVariant();
		}
	}

	return QVariant();
}
catch (...){
	return QVariant();
}

QVariant SubsTblModel::headerData(int section, Qt::Orientation orientation, int role)const{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal)return QVariant();
	switch (section){
	case 0:return QStringLiteral("交易所");
	case 1:return QStringLiteral("品种");
	case 2:return QStringLiteral("贴现/回购天数");
	case 3:return QStringLiteral("回购日");
	case 4:return QStringLiteral("贴现率");
	case 5:return QStringLiteral("正回购率");
	case 6:return QStringLiteral("到期逆回购率");
	case 7:return QStringLiteral("贴现价");
	case 8:return QStringLiteral("正回购价");
	case 9:return QStringLiteral("有效交割日");
	case 10:return QStringLiteral("第一期货");
	case 11:return QStringLiteral("第一期货价");
	case 12:return QStringLiteral("第二期货");
	case 13:return QStringLiteral("第二期货价");
	case 14:return QStringLiteral("灵活逆回购截止日");
	case 15:return QStringLiteral("灵活逆回购率");
	case 16:return QStringLiteral("灵活逆回购价");
	case 17:return QStringLiteral("更新日期");
	case 18:return QStringLiteral("更新时间");
	default:return QVariant();
	}
}

void SubsTblModel::GetSymVec(std::vector<CSymbol> &inst_vec){
	QMutexLocker locker(&wr_quote_datas_mutex_);
	for (QList<CQuotation>::iterator iter = wr_quote_datas_.begin(); iter != wr_quote_datas_.end(); ++iter){
		inst_vec.push_back(iter->symbol);
	}
}

void SubsTblModel::SlotTickRsp(const CQuotation *tick){
	QMutexLocker locker(&wr_quote_datas_mutex_);
	size_t key = SymbolToInt(tick->symbol);
	
	QMap<size_t, CQuotationLine>::iterator iter = wr_data_line_.find(key);
	if (iter == wr_data_line_.end()) {
		wr_quote_datas_.push_back(*tick);
		wr_data_line_[key].data = &wr_quote_datas_.back();
		wr_data_line_[key].line = wr_quote_datas_.size()-1;
		//emit dataChanged(createIndex(wr_quote_datas_.size(), FIRST_COLUMN), createIndex(wr_quote_datas_.size(), LAST_COLUMN));
		endResetModel();
	}
	else {
		// 刷新数据
		*(wr_data_line_[key].data) = *tick;
		emit dataChanged(createIndex(wr_data_line_[key].line, FIRST_COLUMN), createIndex(wr_data_line_[key].line, LAST_COLUMN));
	}

	delete tick;
	


}

void SubsTblModel::SlotRmSym(const CSymbol &sym){
	QMutexLocker locker(&wr_quote_datas_mutex_);

	size_t key = SymbolToInt(sym);
	int line = wr_data_line_[key].line;
	wr_quote_datas_.removeAt(line);
	// 重新设置下wr_data_line_
	wr_data_line_.clear();
	for (int i = 0; i < wr_quote_datas_.size(); ++i){
		size_t k = SymbolToInt(wr_quote_datas_[i].symbol);
		wr_data_line_[k].data = &wr_quote_datas_[i];
		wr_data_line_[k].line = i;
	}
	endResetModel(); // 刷新整个表
}

template<int column> bool Compare(const CQuotation &lhs, const CQuotation &rhs);
FIELD_CMP_IMP(CQuotation, 0, symbol.m_ExchangeId)
FIELD_CMP_IMP(CQuotation, 1, symbol.m_ProductId)
FIELD_CMP_IMP(CQuotation, 2, symbol.m_ContractDays)

template<int column> bool DescCompare(const CQuotation &lhs, const CQuotation &rhs);
FIELD_DESCMP_IMP(CQuotation, 0, symbol.m_ExchangeId)
FIELD_DESCMP_IMP(CQuotation, 1, symbol.m_ProductId)
FIELD_DESCMP_IMP(CQuotation, 2, symbol.m_ContractDays)

void SubsTblModel::sort(int column, Qt::SortOrder order){
	QMutexLocker locker(&wr_quote_datas_mutex_);
	if (order == Qt::AscendingOrder){		
		switch (column)
		{
			SORT_CASE(wr_quote_datas_, 0)
			SORT_CASE(wr_quote_datas_, 1)
			SORT_CASE(wr_quote_datas_, 2)
			default: break;
		}
	}
	else {
		switch (column)
		{
			DESC_SORT_CASE(wr_quote_datas_, 0)
			DESC_SORT_CASE(wr_quote_datas_, 1)
			DESC_SORT_CASE(wr_quote_datas_, 2)
			default: break;
		}
	}

	// 重新设置下wr_data_line_
	wr_data_line_.clear();
	for (int i = 0; i < wr_quote_datas_.size(); ++i){
		size_t k = SymbolToInt(wr_quote_datas_[i].symbol);
		wr_data_line_[k].data = &wr_quote_datas_[i];
		wr_data_line_[k].line = i;
	}
	endResetModel();
	
}

/*
void SubsTblModel::SetSymVec(CSymbol *syms, int n){
	wr_quote_datas_.resize(n);
	for (int i = 0; i < n; ++i){
		wr_quote_datas_[i].symbol = syms[i];
		size_t key = SymbolToInt(syms[i]);
		//wr_data_line_[key].value = &wr_quote_datas_[i];
		wr_data_line_[key] = i;
	}
	endResetModel();
}
*/