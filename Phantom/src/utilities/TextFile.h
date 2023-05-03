#pragma once

/**
 * @file    TextFile.h
 * @brief   This file declares the TextFile class. <br/>
 *          此文件声明 TextFile 类。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
#ifndef TEXT_FILE_H
#define TEXT_FILE_H

/** 
 * @class TextFile
 * @brief Text file class. <br/> 文本文件类。
 * @details This class is used to read text files. <br/>
 *			这个类用于读取文本文件。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
class TextFile
{
protected:
	QString filename;
	QString content;
public:
	explicit TextFile(const QString& filename);
	explicit TextFile(const QUrl& url) : TextFile(url.toLocalFile()) { }
	inline const QString& getContent() const { return content; }
};

#endif