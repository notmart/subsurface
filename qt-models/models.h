/*
 * models.h
 *
 * header file for the equipment models of Subsurface
 *
 */
#ifndef MODELS_H
#define MODELS_H

#include <QAbstractTableModel>
#include <QCoreApplication>
#include <QStringList>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QPixmap>

#include "metrics.h"

#include "../dive.h"
#include "../divelist.h"
#include "../divecomputer.h"
#include "cleanertablemodel.h"
#include "treemodel.h"
/*! An AbstractItemModel for recording dive trip information such as a list of dives.
*
*/

struct DiveItem : public TreeItem {
	enum Column {
		NR,
		DATE,
		RATING,
		DEPTH,
		DURATION,
		TEMPERATURE,
		TOTALWEIGHT,
		SUIT,
		CYLINDER,
		GAS,
		SAC,
		OTU,
		MAXCNS,
		LOCATION,
		COLUMNS
	};

	virtual QVariant data(int column, int role) const;
	int diveId;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	QString displayDate() const;
	QString displayDuration() const;
	QString displayDepth() const;
	QString displayDepthWithUnit() const;
	QString displayTemperature() const;
	QString displayWeight() const;
	QString displaySac() const;
	int weight() const;
};

struct TripItem;

class DiveTripModel : public TreeModel {
	Q_OBJECT
public:
	enum Column {
		NR,
		DATE,
		RATING,
		DEPTH,
		DURATION,
		TEMPERATURE,
		TOTALWEIGHT,
		SUIT,
		CYLINDER,
		GAS,
		SAC,
		OTU,
		MAXCNS,
		LOCATION,
		COLUMNS
	};

	enum ExtraRoles {
		STAR_ROLE = Qt::UserRole + 1,
		DIVE_ROLE,
		TRIP_ROLE,
		SORT_ROLE,
		DIVE_IDX
	};
	enum Layout {
		TREE,
		LIST,
		CURRENT
	};

	Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	DiveTripModel(QObject *parent = 0);
	Layout layout() const;
	void setLayout(Layout layout);

private:
	void setupModelData();
	QMap<dive_trip_t *, TripItem *> trips;
	Layout currentLayout;
};

/* TablePrintModel:
 * for now we use a blank table model with row items TablePrintItem.
 * these are pretty much the same as DiveItem, but have color
 * properties, as well. perhaps later one a more unified model has to be
 * considered, but the current TablePrintModel idea has to be extended
 * to support variadic column lists and column list orders that can
 * be controlled by the user.
 */
struct TablePrintItem {
	QString number;
	QString date;
	QString depth;
	QString duration;
	QString divemaster;
	QString buddy;
	QString location;
	unsigned int colorBackground;
};

class TablePrintModel : public QAbstractTableModel {
	Q_OBJECT

private:
	QList<struct TablePrintItem *> list;

public:
	~TablePrintModel();
	TablePrintModel();

	int rows, columns;
	void insertRow(int index = -1);
	void callReset();

	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
};

/* ProfilePrintModel:
 * this model is used when printing a data table under a profile. it requires
 * some exact usage of setSpan(..) on the target QTableView widget.
 */
class ProfilePrintModel : public QAbstractTableModel {
	Q_OBJECT

private:
	int diveId;
	double fontSize;

public:
	ProfilePrintModel(QObject *parent = 0);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	void setDive(struct dive *divePtr);
	void setFontsize(double size);
};

class GasSelectionModel : public QStringListModel {
	Q_OBJECT
public:
	static GasSelectionModel *instance();
	Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
public
slots:
	void repopulate();
};


class LanguageModel : public QAbstractListModel {
	Q_OBJECT
public:
	static LanguageModel *instance();
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
	LanguageModel(QObject *parent = 0);

	QStringList languages;
};

#endif // MODELS_H