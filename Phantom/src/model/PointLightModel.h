#pragma once

class PointLightModel  : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(qint8 count READ count NOTIFY sizeChanged)

public:
	static PointLightModel* instance;
	static PointLightModel* getInstance(QQmlEngine* qml, QJSEngine* js);

protected:
	PointLightModel(QObject *parent);
	~PointLightModel();

signals:
	void sizeChanged(qint8);

public:
	qint8 count() const noexcept;

	int rowCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;
	int role(const QByteArray& roleName) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool insertRows(int row, int count, const QModelIndex& parent) override;
	bool removeRows(int row, int count, const QModelIndex& parent) override;
};
