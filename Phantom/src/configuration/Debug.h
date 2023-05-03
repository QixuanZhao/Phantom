#pragma once

#ifndef DEBUG_H
#define DEBUG_H

class Debug : public QObject
{
	Q_OBJECT
signals:
	void logged(const QString& info) const;
private:
	static Debug instance;

protected:
	Debug () { }

	float hubAlpha = 0.7f;
	float mix = 0.5f;

public:
	enum class Format {
		FAIL,
		SUCCESS,
		VALUE,
		PARAMETER,
		IDENTIFIER,
		NUMERIC
	};

	static const char* fail;
	static const char* success;
	static const char* value;
	static const char* parameter;
	static const char* identifier;
	static const char* numeric;

	static Debug& getInstance();

	static void putLog(const QString& info);
	static void putLog(const Format& format, const QString& info);

	void log(const QString& info) const;

	inline void setHubAlpha(const float& alpha) { hubAlpha = alpha; }
	inline float getHubAlpha() const { return hubAlpha; }

	inline void setMix(const float& mix) { this->mix = mix; }
	inline float getMix() const { return mix; }
};

#endif