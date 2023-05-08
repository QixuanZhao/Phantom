#include "stdafx.h"

ParallelLightModel * ParallelLightModel::instance = nullptr;

ParallelLightModel * ParallelLightModel::getInstance(QQmlEngine * qml, QJSEngine* js)
{
	if (instance) return instance;
	else {
		instance = new ParallelLightModel(qml);
		qml->setObjectOwnership(instance, QQmlEngine::CppOwnership);
		return instance;
	}
}

ParallelLightModel::ParallelLightModel(QObject *parent)
	: QAbstractListModel(parent)
{}

ParallelLightModel::~ParallelLightModel()
{}

qint8 ParallelLightModel::count() const noexcept
{
	return Lighting::lighting.parallelLights.size();
}

int ParallelLightModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid()) return 0;
	else return Lighting::lighting.parallelLights.size();
}

QVariant ParallelLightModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() ||
		index.row() >= Lighting::lighting.parallelLights.size()
	) return QVariant();

	list<ParallelLight>::iterator it;
	if (index.row() >= Lighting::lighting.parallelLights.size() / 2) {
		it = Lighting::lighting.parallelLights.end();
		for (int i = 0; i < Lighting::lighting.parallelLights.size() - index.row(); i++, --it);
	}
	else {
		it = Lighting::lighting.parallelLights.begin();
		for (int i = 0; i < index.row(); i++, ++it);
	}

	switch (role) {
	case Qt::UserRole + 1: return it->getName();
	case Qt::UserRole + 2:
		vec3 diffuse = it->diffuse();
		return QVector3D(diffuse.r, diffuse.g, diffuse.b);
	case Qt::UserRole + 3:
		vec3 specular = it->specular();
		return QVector3D(specular.r, specular.g, specular.b);
	case Qt::UserRole + 4:
		vec3 colour = it->colour() * 255.0f;
		return QColor(colour.r, colour.g, colour.b);
	case Qt::UserRole + 5:
		return it->power();
	case Qt::UserRole + 6:
		vec3 flux = it->flux();
		return QVector3D(flux.r, flux.g, flux.b);
	case Qt::UserRole + 7:
		return QVector2D(it->getAzimuth(), it->getAltitude());
	default:
		return QVariant();
	}
}

QHash<int, QByteArray> ParallelLightModel::roleNames() const
{
	QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
	roles[Qt::UserRole + 1] = "name";
	roles[Qt::UserRole + 2] = "diffuse";
	roles[Qt::UserRole + 3] = "specular";
	roles[Qt::UserRole + 4] = "colour";
	roles[Qt::UserRole + 5] = "power";
	roles[Qt::UserRole + 6] = "radiant_flux";
	roles[Qt::UserRole + 7] = "direction";
	return roles;
}

int ParallelLightModel::role(const QByteArray& roleName) const
{
	QHash<QByteArray, int> roles; 
	roles["name"] = Qt::UserRole + 1;
	roles["diffuse"] = Qt::UserRole + 2;
	roles["specular"] = Qt::UserRole + 3;
	roles["colour"] = Qt::UserRole + 4;
	roles["power"] = Qt::UserRole + 5;
	roles["radiant_flux"] = Qt::UserRole + 6;
	roles["direction"] = Qt::UserRole + 7;
	if (roles.contains(roleName)) return roles[roleName];
	else return Qt::UserRole;
}

bool ParallelLightModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid() || 
		!value.isValid() || 
		index.row() >= Lighting::lighting.parallelLights.size()
	) return false;

	list<ParallelLight>::iterator it;
	if (index.row() >= Lighting::lighting.parallelLights.size() / 2) {
		it = Lighting::lighting.parallelLights.end();
		for (int i = 0; i < Lighting::lighting.parallelLights.size() - index.row(); i++, --it);
	}
	else {
		it = Lighting::lighting.parallelLights.begin();
		for (int i = 0; i < index.row(); i++, ++it);
	}

	switch (role) {
	case Qt::UserRole + 1:
		it->setName(value.toString());
		break;
	case Qt::UserRole + 2: {
		QVector3D diffuse = value.value<QVector3D>();
		it->setDiffuse(vec3(diffuse.x(), diffuse.y(), diffuse.z()));
	} break;
	case Qt::UserRole + 3: {
		QVector3D specular = value.value<QVector3D>();
		it->setSpecular(vec3(specular.x(), specular.y(), specular.z()));
	} break;
	case Qt::UserRole + 4: {
		QColor colour = value.value<QColor>();
		it->setColour(vec3(colour.redF(), colour.greenF(), colour.blueF()));
	} break;
	case Qt::UserRole + 5: {
		qreal power = value.value<qreal>();
		it->setPower(power);
	} break;
	case Qt::UserRole + 7: {
		QVector2D horizontal = value.value<QVector2D>();
		it->setAzimuth(horizontal.x());
		it->setAltitude(horizontal.y());
	} break;
	default:
		return false;
	}

	switch (role) {
	case Qt::UserRole + 4:
	case Qt::UserRole + 5:
		emit dataChanged(index, index, { role, Qt::UserRole + 6 });
		break;
	default:
		emit dataChanged(index, index, { role });
		break;
	}

	return true;
}

Qt::ItemFlags ParallelLightModel::flags(const QModelIndex& index) const
{
	if (!index.isValid()) return Qt::ItemFlags();

	return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

bool ParallelLightModel::insertRows(int row, int count, const QModelIndex& parent)
{
	if (parent.isValid()) return false; // no tree structure

	beginInsertRows(parent, row, row + count - 1);
	ParallelLight light;
	light.setName("Light");
	light.setRadiometric(true);
	light.setDiffuse(vec3(1.0f));
	light.setSpecular(vec3(1.0f));
	light.setColour(vec3(1.0f));
	light.setPower(1.0f);
	OrthographicShadow& shadow = light.getShadow();
	shadow.setPosition(
		Data::instance().getZoomTarget()
		- shadow.getDirection() * Data::instance().getZoomDistance() / 1.7f
	);
	shadow.setWidthAndHeight(
		Data::instance().getZoomDistance(),
		Data::instance().getZoomDistance()
	);
	shadow.requireUpdate();
	list<ParallelLight>::const_iterator it = Lighting::lighting.parallelLights.cbegin();
	for (int i = 0; i < row; i++, ++it);
	Lighting::lighting.parallelLights.insert(it, count, light);
	emit sizeChanged(Lighting::lighting.parallelLights.size());
	endInsertRows();
	return true;
}

bool ParallelLightModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (parent.isValid()) return false; // no tree structure

	beginRemoveRows(parent, row, row + count - 1);
	list<ParallelLight>::const_iterator begin = Lighting::lighting.parallelLights.cbegin();
	for (int i = 0; i < row; i++, ++begin);
	list<ParallelLight>::const_iterator end = begin;
	for (int i = 0; i < count; i++, ++end);

	Lighting::lighting.parallelLights.erase(begin, end);
	emit sizeChanged(Lighting::lighting.parallelLights.size());
	endRemoveRows();
	return true;
}
