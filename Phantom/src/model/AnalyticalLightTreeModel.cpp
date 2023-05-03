#include "stdafx.h"

AnalyticalLightTreeModel* AnalyticalLightTreeModel::instance = nullptr;

AnalyticalLightTreeModel* AnalyticalLightTreeModel::getInstance(QQmlEngine* qml, QJSEngine* js)
{
	if (instance) return instance;
	else {
		instance = new AnalyticalLightTreeModel(qml);
		instance->parallelLightModel = ParallelLightModel::getInstance(qml, js);
		instance->pointLightModel = PointLightModel::getInstance(qml, js);
		instance->spotlightModel = SpotlightModel::getInstance(qml, js);
		instance->connect(instance->parallelLightModel, SIGNAL(sizeChanged(qint8)), SLOT(notifySizeChange()));
		instance->connect(instance->pointLightModel, SIGNAL(sizeChanged(qint8)), SLOT(notifySizeChange()));
		instance->connect(instance->spotlightModel, SIGNAL(sizeChanged(qint8)), SLOT(notifySizeChange()));
		qml->setObjectOwnership(instance, QQmlEngine::CppOwnership);
		return instance;
	}
}

AnalyticalLightTreeModel::AnalyticalLightTreeModel(QObject *parent)
	: QAbstractItemModel(parent)
{}

AnalyticalLightTreeModel::~AnalyticalLightTreeModel()
{}

void AnalyticalLightTreeModel::notifySizeChange()
{
	emit sizeChanged(count());
}

qint8 AnalyticalLightTreeModel::count() const noexcept
{
	return parallelLightModel->count() + pointLightModel->count() + spotlightModel->count();
}

QModelIndex AnalyticalLightTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid()) {
		const char* type = static_cast<const char*>(parent.internalPointer());
		if (type && type[0] != INDICATOR) return QModelIndex();

		if (type == PARALLEL_LIGHT && row < parallelLightModel->count()) return createIndex(row, column, "Parallel Light");
		if (type == POINT_LIGHT && row < pointLightModel->count()) return createIndex(row, column, "Point Light");
		if (type == SPOTLIGHT && row < spotlightModel->count()) return createIndex(row, column, "Spotlight");
	}
	else {
		switch (row) {
		case 0: return createIndex(row, column, PARALLEL_LIGHT);
		case 1: return createIndex(row, column, POINT_LIGHT);
		case 2: return createIndex(row, column, SPOTLIGHT);
		}
	}
	return QModelIndex();
}

QModelIndex AnalyticalLightTreeModel::parent(const QModelIndex& index) const
{
	if (!index.isValid()) return QModelIndex();

	QString type = static_cast<const char*>(index.internalPointer());
	if (type == "Parallel Light") 
		return createIndex(0, 0, PARALLEL_LIGHT);
	else if (type == "Point Light") 
		return createIndex(1, 0, POINT_LIGHT);
	else if (type == "Spotlight") 
		return createIndex(2, 0, SPOTLIGHT);
	else return QModelIndex();

}

int AnalyticalLightTreeModel::rowCount(const QModelIndex& parent) const
{
	if (!parent.isValid()) return 3;

	const char* type = static_cast<const char*>(parent.internalPointer());
	if (type && type[0] != INDICATOR) return 0;
	
	if (type == PARALLEL_LIGHT) return parallelLightModel->count();
	if (type == POINT_LIGHT) return pointLightModel->count();
	if (type == SPOTLIGHT) return spotlightModel->count();
}

int AnalyticalLightTreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant AnalyticalLightTreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) return QVariant();

	QByteArray roleName = roleNames()[role];
	if (roleName == "index_row") return index.row();

	const char* type = static_cast<const char*>(index.internalPointer());
	if (roleName == "type") return type;
	if (!type || type[0] == INDICATOR) {
		if (roleName != "name") return QVariant();
		// name
		if (type == PARALLEL_LIGHT) return tr("Parallel Light");
		if (type == POINT_LIGHT) return tr("Point Light");
		if (type == SPOTLIGHT) return tr("Spotlight");
	}
	else {
		QString typeStr = type;
		if (typeStr == "Parallel Light") {
			QByteArray roleName = roleNames()[role];
			int plmRole = parallelLightModel->role(roleName);
			if (plmRole == Qt::UserRole) return QVariant();
			return parallelLightModel->data(parallelLightModel->index(index.row()), plmRole);
		}
		else if (typeStr == "Point Light") {
			QByteArray roleName = roleNames()[role];
			int plmRole = pointLightModel->role(roleName);
			if (plmRole == Qt::UserRole) return QVariant();
			return pointLightModel->data(pointLightModel->index(index.row()), plmRole);

		}
		else if (typeStr == "Spotlight") {
			QByteArray roleName = roleNames()[role];
			int smRole = spotlightModel->role(roleName);
			if (smRole == Qt::UserRole) return QVariant();
			return spotlightModel->data(spotlightModel->index(index.row()), smRole);
		}
	}
	
	return QVariant();
}

QHash<int, QByteArray> AnalyticalLightTreeModel::roleNames() const
{
	QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
	int role = Qt::UserRole + 1;
	roles[role++] = "name";
	roles[role++] = "diffuse";
	roles[role++] = "specular";
	roles[role++] = "colour";
	roles[role++] = "power";
	roles[role++] = "radiant_flux";
	roles[role++] = "position";
	roles[role++] = "direction";
	roles[role++] = "cutoff";
	roles[role++] = "type";
	roles[role++] = "index_row";
	return roles;
}

bool AnalyticalLightTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid()) return false;
	const char* type = static_cast<const char*>(index.internalPointer());
	if (!type || type[0] == INDICATOR) return false;
	QString typeStr = type;
	QAbstractListModel* model = nullptr;

	if (typeStr == "Parallel Light") {
		QByteArray roleName = roleNames()[role];
		int plmRole = parallelLightModel->role(roleName);
		if (plmRole == Qt::UserRole) return false;
		if (!parallelLightModel->setData(parallelLightModel->index(index.row()), value, plmRole)) return false;
	}
	else if (typeStr == "Point Light") {
		QByteArray roleName = roleNames()[role];
		int plmRole = pointLightModel->role(roleName);
		if (plmRole == Qt::UserRole) return false;
		if (!pointLightModel->setData(pointLightModel->index(index.row()), value, plmRole)) return false;

	}
	else if (typeStr == "Spotlight") {
		QByteArray roleName = roleNames()[role];
		int smRole = spotlightModel->role(roleName);
		if (smRole == Qt::UserRole) return false;
		if (!spotlightModel->setData(spotlightModel->index(index.row()), value, smRole)) return false;
	}
	else return false;

	emit dataChanged(index, index, { role });
	return true;
}

Qt::ItemFlags AnalyticalLightTreeModel::flags(const QModelIndex& index) const
{
	if (!index.isValid()) return Qt::ItemFlags();

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool AnalyticalLightTreeModel::insertRows(int row, int count, const QModelIndex& parent)
{
	if (!parent.isValid()) return false;
	const char* type = static_cast<const char*>(parent.internalPointer());
	if (type && type[0] != INDICATOR) return false;

	beginInsertRows(parent, row, row + count - 1);

	if (type == PARALLEL_LIGHT) {
		if (!parallelLightModel->insertRows(row, count, QModelIndex())) {
			endInsertRows();
			return false;
		}
	}
	else if (type == POINT_LIGHT) {
		if (!pointLightModel->insertRows(row, count, QModelIndex())) {
			endInsertRows();
			return false;
		}
	}
	else if (type == SPOTLIGHT) {
		if (!spotlightModel->insertRows(row, count, QModelIndex())) {
			endInsertRows();
			return false;
		}
	}
	else {
		endInsertRows();
		return false;
	}

	emit sizeChanged(this->count());
	endInsertRows();
	return true;
}

bool AnalyticalLightTreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (!parent.isValid()) return false;
	const char* type = static_cast<const char*>(parent.internalPointer());
	if (type && type[0] != INDICATOR) return false;
	beginRemoveRows(parent, row, row + count - 1);

	if (type == PARALLEL_LIGHT) {
		if (!parallelLightModel->removeRows(row, count, QModelIndex())) {
			endRemoveRows();
			return false;
		}
	}
	else if (type == POINT_LIGHT) {
		if (!pointLightModel->removeRows(row, count, QModelIndex())) {
			endRemoveRows();
			return false;
		}
	}
	else if (type == SPOTLIGHT) {
		if (!spotlightModel->removeRows(row, count, QModelIndex())) {
			endRemoveRows();
			return false;
		}
	}
	else {
		endRemoveRows();
		return false;
	}

	emit sizeChanged(this->count());
	endRemoveRows();
	return true;
}
