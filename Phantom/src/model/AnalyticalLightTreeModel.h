#pragma once

class AnalyticalLightTreeModel  : public QAbstractItemModel
{
	Q_OBJECT
	Q_PROPERTY(qint8 count READ count NOTIFY sizeChanged)

public:
	static AnalyticalLightTreeModel* instance;
	static AnalyticalLightTreeModel* getInstance(QQmlEngine* qml, QJSEngine* js);

protected:
	AnalyticalLightTreeModel(QObject *parent);
	~AnalyticalLightTreeModel();

signals:
	void sizeChanged(qint8);

protected slots:
	void notifySizeChange();

public slots:
	void retranslate();

public:
	qint8 count() const noexcept;

	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;

	QHash<int, QByteArray> roleNames() const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool insertRows(int row, int count, const QModelIndex& parent) override;
	bool removeRows(int row, int count, const QModelIndex& parent) override;

protected:
	ParallelLightModel* parallelLightModel = nullptr;
	PointLightModel* pointLightModel = nullptr;
	SpotlightModel* spotlightModel = nullptr;

	const char* PARALLEL_LIGHT = "\nD";
	const char* POINT_LIGHT = "\nP";
	const char* SPOTLIGHT = "\nS";
	const char INDICATOR = '\n';
};
