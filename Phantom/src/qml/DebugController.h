#pragma once

class DebugController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(qreal hubAlpha READ hubAlpha WRITE setHubAlpha NOTIFY hubAlphaChanged)
	Q_PROPERTY(qreal mix READ mix WRITE setMix NOTIFY mixChanged)
	QML_ELEMENT
signals:
	void logged(const QString& str);
	void hubAlphaChanged(const qreal& alpha);
	void mixChanged(const qreal& mix);

public slots:
	Q_INVOKABLE void log(const QString& info) { emit logged(info); }

public:
	inline qreal hubAlpha() const { return Debug::getInstance().getHubAlpha(); }
	inline void setHubAlpha(const qreal& alpha) { 
		Debug::getInstance().setHubAlpha(alpha); 
		emit hubAlphaChanged(alpha);
	}

	inline qreal mix() const { return Debug::getInstance().getMix(); }
	inline void setMix(const qreal& mix) { 
		Debug::getInstance().setMix(mix);
		emit mixChanged(mix);
	}

	inline DebugController() {
		connect(&Debug::getInstance(), SIGNAL(logged(QString)), SLOT(log(QString)));
	}
};
