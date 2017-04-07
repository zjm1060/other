#ifndef QTRANSLATEINSTALLER_H
#define QTRANSLATEINSTALLER_H

#include <QObject>
#include <QStringList>
#include <QTranslator>
#include <QTextCodec>

#include "global.h"

class UTIL_EXPORT QTranslateInstaller : public QObject
{
	Q_OBJECT

public:
	~QTranslateInstaller();

protected:
	QTranslateInstaller(QObject *parent=0);
	
public:
	static QTranslateInstaller* instance();

public:
	void setTranslators(const QStringList& names);
	void install(const QString& lang);
	void install();

private:
	QStringList			m_translatorNames;
	QList<QTranslator*>	m_translators;
};

#endif // QTRANSLATEINSTALLER_H
