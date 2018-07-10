#ifndef SYMLISTMODEL_H
#define SYMLISTMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QMap>
#include "WRStruct.h"
#include "WRClient.h"

using namespace kiiik;

class SymListModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	SymListModel(WRClient *wr_client, QObject *parent);
	~SymListModel();

	// 更新合约列表  // 已经订阅的继续推送，刷新不影响其他地方的显示内容 --- 陈挺
	void SetSymVec(CSymbol *, int);

	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex &parent /* = QModelIndex() */)const;
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */)const;
	QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole)const;
	//设置某一列为可选角色，绘画出QCheckBox
	Qt::ItemFlags flags(const QModelIndex &index) const;
	//根据界面选择QCheckbox，修改Model中的数据
	bool setData(const QModelIndex &index, const QVariant &value, int role);

	// 订阅/退订所有需要修改sym_list_的check
	void ChangeState(Qt::CheckState);

	struct SymCheck {
		CSymbol sym;
		Qt::CheckState check;//打钩，表明已经订阅
		SymCheck() :check(Qt::Unchecked){}
		SymCheck(CSymbol &s, Qt::CheckState c) :sym(s), check(c){}
	};
private:
	QVector<SymCheck> sym_list_;

	WRClient *wr_client_;

	//virtual void sort(int, Qt::SortOrder);

private slots:
	void SlotRowClicked(const QModelIndex &);
	

signals:
	void SignalRmSym(const CSymbol &);
};

#endif // SYMLISTMODEL_H
