#pragma once

class DataController : public QQuickItem
{
	Q_OBJECT
	QML_ELEMENT
public:
	enum class PolygonMode {
		FILL,
		LINE,
		POINT
	};

	Q_ENUM(PolygonMode)

	Q_INVOKABLE QString load(const QUrl& url);
	Q_INVOKABLE void remove(const QString& objectName, const QString& surfaceName = QString());
	Q_INVOKABLE void setVisible(const QString& objectName, const QString& surfaceName, bool visible);
	Q_INVOKABLE void setVisible(const QString& objectName, bool visible);
	Q_INVOKABLE void setColour(const QString& objectName, const QString& surfaceName, const QColor& color);
	Q_INVOKABLE void setColour(const QString& objectName, const QColor& color);
	Q_INVOKABLE void setPolygonMode(const QString& objectName, const QString& surfaceName, PolygonMode polygonMode);
	Q_INVOKABLE void setPolygonMode(const QString& objectName, PolygonMode polygonMode);
	Q_INVOKABLE void setShininess(const QString& objectName, const QString& surfaceName, qreal shininess);
	Q_INVOKABLE void setShininess(const QString& objectName, qreal shininess);
	Q_INVOKABLE void setMetallic(const QString& objectName, const QString& surfaceName, qreal metallic);
	Q_INVOKABLE void setMetallic(const QString& objectName, qreal metallic);
	Q_INVOKABLE void setRoughness(const QString& objectName, const QString& surfaceName, qreal roughness);
	Q_INVOKABLE void setRoughness(const QString& objectName, qreal roughness);
	Q_INVOKABLE void setK(const QString& objectName, const QString& surfaceName, qreal K);
	Q_INVOKABLE void setK(const QString& objectName, qreal K);
protected:
	Data& data = Data::getInstance();
};
