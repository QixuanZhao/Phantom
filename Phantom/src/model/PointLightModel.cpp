#include "stdafx.h"

PointLightModel* PointLightModel::instance = nullptr;

PointLightModel* PointLightModel::getInstance(QQmlEngine* qml, QJSEngine* js)
{
	if (instance) return instance;
	else {
		instance = new PointLightModel(qml);
		qml->setObjectOwnership(instance, QQmlEngine::CppOwnership);
		return instance;
	}
}

PointLightModel::PointLightModel(QObject *parent)
	: QAbstractListModel(parent)
{}

PointLightModel::~PointLightModel()
{}

qint8 PointLightModel::count() const noexcept
{
	return Lighting::lighting.pointLights.size();
}

int PointLightModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid()) return 0;
	else return Lighting::lighting.pointLights.size();
}

QVariant PointLightModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() ||
		index.row() >= Lighting::lighting.pointLights.size()
	) return QVariant();

	list<ShadowCastingPointLight>::iterator it;
	if (index.row() >= Lighting::lighting.pointLights.size() / 2) {
		it = Lighting::lighting.pointLights.end();
		for (int i = 0; i < Lighting::lighting.pointLights.size() - index.row(); i++, --it);
	}
	else {
		it = Lighting::lighting.pointLights.begin();
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
		return QVector3D(it->getPosition().x, it->getPosition().y, it->getPosition().z);
	default:
		return QVariant();
	}
}

QHash<int, QByteArray> PointLightModel::roleNames() const
{
	QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
	roles[Qt::UserRole + 1] = "name";
	roles[Qt::UserRole + 2] = "diffuse";
	roles[Qt::UserRole + 3] = "specular";
	roles[Qt::UserRole + 4] = "colour";
	roles[Qt::UserRole + 5] = "power";
	roles[Qt::UserRole + 6] = "radiant_flux";
	roles[Qt::UserRole + 7] = "position";
	return roles;
}

int PointLightModel::role(const QByteArray& roleName) const
{
	QHash<QByteArray, int> roles;
	roles["name"] = Qt::UserRole + 1;
	roles["diffuse"] = Qt::UserRole + 2;
	roles["specular"] = Qt::UserRole + 3;
	roles["colour"] = Qt::UserRole + 4;
	roles["power"] = Qt::UserRole + 5;
	roles["radiant_flux"] = Qt::UserRole + 6;
	roles["position"] = Qt::UserRole + 7;

	if (roles.contains(roleName)) return roles[roleName];
	else return Qt::UserRole;
}

bool PointLightModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid() ||
		!value.isValid() ||
		index.row() >= Lighting::lighting.pointLights.size()
		) return false;

	list<ShadowCastingPointLight>::iterator it;
	if (index.row() >= Lighting::lighting.pointLights.size() / 2) {
		it = Lighting::lighting.pointLights.end();
		for (int i = 0; i < Lighting::lighting.pointLights.size() - index.row(); i++, --it);
	}
	else {
		it = Lighting::lighting.pointLights.begin();
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
		QVector3D position = value.value<QVector3D>();
		it->setPosition(vec3(position.x(), position.y(), position.z()));
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

Qt::ItemFlags PointLightModel::flags(const QModelIndex& index) const
{
	if (!index.isValid()) return Qt::ItemFlags();

	return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

bool PointLightModel::insertRows(int row, int count, const QModelIndex& parent)
{
	if (parent.isValid()) return false; // no tree structure

	beginInsertRows(parent, row, row + count - 1);
	ShadowCastingPointLight light;
	light.setName("Light");
	light.setRadiometric(true);
	light.setDiffuse(vec3(1.0f));
	light.setSpecular(vec3(1.0f));
	light.setColour(vec3(1.0f));
	light.setPower(1.0f);
	light.setPosition(Display::display.observerCamera.getTarget());
	list<ShadowCastingPointLight>::const_iterator it = Lighting::lighting.pointLights.cbegin();
	for (int i = 0; i < row; i++, ++it);
	Lighting::lighting.pointLights.insert(it, count, light);
	emit sizeChanged(Lighting::lighting.pointLights.size());
	endInsertRows();
	return true;
}

bool PointLightModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (parent.isValid()) return false; // no tree structure

	beginRemoveRows(parent, row, row + count - 1);
	list<ShadowCastingPointLight>::const_iterator begin = Lighting::lighting.pointLights.cbegin();
	for (int i = 0; i < row; i++, ++begin);
	list<ShadowCastingPointLight>::const_iterator end = begin;
	for (int i = 0; i < count; i++, ++end);

	Lighting::lighting.pointLights.erase(begin, end);
	emit sizeChanged(Lighting::lighting.pointLights.size());
	endRemoveRows();
	return true;
}
