#ifndef ITEMBUCKET_H
#define ITEMBUCKET_H
#include <QDialog>
#include "item.h"
#include "ui_itembucket.h"

class ItemModel;
class QModelIndex;

namespace Ui
{
	class ItemBucket;
};

class ItemBucket : public QDialog
{
	Q_OBJECT

	ItemModel *Model_;
	Ui::ItemBucket Ui_;

public:
	ItemBucket (QWidget* = 0);
	virtual ~ItemBucket ();
private slots:
	void on_Items__activated (const QModelIndex&);
	void on_ActionDeleteItem__triggered ();
	void currentItemChanged (const QModelIndex&);
};

#endif
