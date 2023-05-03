#include "stdafx.h"

TextFile::TextFile(const QString& filename) : filename(filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    content = QString(file.readAll());
    file.close();
}
