#include "stdafx.h"

Control * Control::control = nullptr;

void Control::retranslate(const QLocale& locale)
{
    if (translator.load(locale, "Translation", "_", ":/i18n"));
    engine.retranslate();
    AnalyticalLightTreeModel::getInstance(&engine, &javascript)->retranslate();
}
