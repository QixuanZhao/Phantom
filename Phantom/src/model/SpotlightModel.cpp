#include "stdafx.h"

SpotlightModel* SpotlightModel::instance = nullptr;

SpotlightModel* SpotlightModel::getInstance(QQmlEngine* qml, QJSEngine* js)
{
	if (instance) return instance;
	else {
		instance = new SpotlightModel(qml);
		qml->setObjectOwnership(instance, QQmlEngine::CppOwnership);
		return instance;
	}
}

SpotlightModel::SpotlightModel(QObject* parent)
	: QAbstractListModel(parent)
{
}

SpotlightModel::~SpotlightModel()
{
}

qint8 SpotlightModel::count() const noexcept
{
	return Lighting::lighting.spotlights.size();
}

int SpotlightModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid()) return 0;
	else return Lighting::lighting.spotlights.size();
}

QVariant SpotlightModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() ||
		index.row() >= Lighting::lighting.spotlights.size()
		) return QVariant();

	list<ShadowCastingSpotlight>::iterator it;
	if (index.row() >= Lighting::lighting.spotlights.size() / 2) {
		it = Lighting::lighting.spotlights.end();
		for (int i = 0; i < Lighting::lighting.spotlights.size() - index.row(); i++, --it);
	}
	else {
		it = Lighting::lighting.spotlights.begin();
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
	case Qt::UserRole + 8:
		return QVector2D(it->getAzimuth(), it->getAltitude());
	case Qt::UserRole + 9:
		return QVector2D(it->getInnerCutOff(), it->getOuterCutOff());
	default:
		return QVariant();
	}
}

QHash<int, QByteArray> SpotlightModel::roleNames() const
{
	QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
	roles[Qt::UserRole + 1] = "name";
	roles[Qt::UserRole + 2] = "diffuse";
	roles[Qt::UserRole + 3] = "specular";
	roles[Qt::UserRole + 4] = "colour";
	roles[Qt::UserRole + 5] = "power";
	roles[Qt::UserRole + 6] = "radiant_flux";
	roles[Qt::UserRole + 7] = "position";
	roles[Qt::UserRole + 8] = "direction";
	roles[Qt::UserRole + 9] = "cutoff";
	return roles;
}

int SpotlightModel::role(const QByteArray& roleName) const
{
	QHash<QByteArray, int> roles;
	roles["name"] = Qt::UserRole + 1;
	roles["diffuse"] = Qt::UserRole + 2;
	roles["specular"] = Qt::UserRole + 3;
	roles["colour"] = Qt::UserRole + 4;
	roles["power"] = Qt::UserRole + 5;
	roles["radiant_flux"] = Qt::UserRole + 6;
	roles["position"] = Qt::UserRole + 7;
	roles["direction"] = Qt::UserRole + 8;
	roles["cutoff"] = Qt::UserRole + 9;

	if (roles.contains(roleName)) return roles[roleName];
	else return Qt::UserRole;
}

bool SpotlightModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid() ||
		!value.isValid() ||
		index.row() >= Lighting::lighting.spotlights.size()
		) return false;

	list<ShadowCastingSpotlight>::iterator it;
	if (index.row() >= Lighting::lighting.spotlights.size() / 2) {
		it = Lighting::lighting.spotlights.end();
		for (int i = 0; i < Lighting::lighting.spotlights.size() - index.row(); i++, --it);
	}
	else {
		it = Lighting::lighting.spotlights.begin();
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
	case Qt::UserRole + 8: {
		QVector2D horizontal = value.value<QVector2D>();
		it->setHorizontal(horizontal.x(), horizontal.y());
	} break;
	case Qt::UserRole + 9: {
		QVector2D cutoff = value.value<QVector2D>();
		it->setInnerCutOff(cutoff.x());
		it->setOuterCutOff(cutoff.y());
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

Qt::ItemFlags SpotlightModel::flags(const QModelIndex& index) const
{
	if (!index.isValid()) return Qt::ItemFlags();

	return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

bool SpotlightModel::insertRows(int row, int count, const QModelIndex& parent)
{
	if (parent.isValid()) return false; // no tree structure

	beginInsertRows(parent, row, row + count - 1);
	ShadowCastingSpotlight light;
	light.setName("Light");
	light.setRadiometric(true);
	light.setColour(vec3(1.0f));
	light.setPower(1.0f);
	light.setPosition(Display::display.observerCamera.getTarget());
	light.setDirection(Display::display.observerCamera.getFront());
	list<ShadowCastingSpotlight>::const_iterator it = Lighting::lighting.spotlights.cbegin();
	for (int i = 0; i < row; i++, ++it);
	Lighting::lighting.spotlights.insert(it, count, light);
	emit sizeChanged(Lighting::lighting.spotlights.size());
	endInsertRows();
	return true;
}

bool SpotlightModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (parent.isValid()) return false; // no tree structure

	beginRemoveRows(parent, row, row + count - 1);
	list<ShadowCastingSpotlight>::const_iterator begin = Lighting::lighting.spotlights.cbegin();
	for (int i = 0; i < row; i++, ++begin);
	list<ShadowCastingSpotlight>::const_iterator end = begin;
	for (int i = 0; i < count; i++, ++end);

	Lighting::lighting.spotlights.erase(begin, end);
	emit sizeChanged(Lighting::lighting.spotlights.size());
	endRemoveRows();
	return true;
}
