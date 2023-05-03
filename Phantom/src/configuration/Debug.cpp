#include "stdafx.h"

Debug Debug::instance;

const char* Debug::fail = "<span style=\"color:#e4281f\">%1</span>";
const char* Debug::success = "<span style=\"color:#1a8000\">%1</span>";
const char* Debug::value = "<span style=\"color:#5992af\">%1</span>";
const char* Debug::parameter = "<span style=\"color:#a31515\">%1</span>";
const char* Debug::identifier = "<span style=\"color:#a31515\">%1</span>";
const char* Debug::numeric = "<span style=\"color:#8a1bff\">%1</span>";

Debug& Debug::getInstance()
{
	return instance;
}

void Debug::putLog(const QString& info)
{
	instance.log(info);
}

void Debug::putLog(const Format& format, const QString& info)
{
	const char* stringFormat;
	switch (format) {
	case Format::FAIL:
		stringFormat = fail;
		break;
	case Format::SUCCESS:
		stringFormat = success;
		break;
	case Format::VALUE:
		stringFormat = value;
		break;
	case Format::PARAMETER:
		stringFormat = parameter;
		break;
	case Format::IDENTIFIER:
		stringFormat = identifier;
		break;
	case Format::NUMERIC:
		stringFormat = numeric;
		break;
	default:
		instance.log(info);
		return;
	}

	instance.log(QString(stringFormat).arg(info));
}

//void Debug::setDebugPanel(DebugPanel* debugPanel)
//{
//	this->debugPanel = debugPanel;
//}

void Debug::log(const QString& info) const
{
	//if (debugPanel) {
	//	debugPanel->log(info);
	//}
	emit logged(info);
}
