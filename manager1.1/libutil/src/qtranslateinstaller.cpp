#include "qtranslateinstaller.h"

#include <QCoreApplication>

#include "common.h"

#define LANG_FILE_PATH	"/ini/lang"

QTranslateInstaller::QTranslateInstaller(QObject *parent)
	: QObject(parent)
{

}

QTranslateInstaller::~QTranslateInstaller()
{

}

QTranslateInstaller* QTranslateInstaller::instance()
{
	static QTranslateInstaller* pInstance=new QTranslateInstaller;

	return pInstance;
}

void QTranslateInstaller::setTranslators(const QStringList& names)
{
	m_translatorNames=names;
}

void QTranslateInstaller::install(const QString& lang)
{
	if (lang.isEmpty())
	{
		return;
	}

	codeConfig(getAppParentDir()+INI_FILE, "main/lang", lang);
	foreach (QTranslator* translator, m_translators)
	{
		QCoreApplication::removeTranslator(translator);
	}

	m_translators.clear();

	foreach (QString name, m_translatorNames)
	{
		QTranslator* translator=new QTranslator;
		translator->load(QString("%1%2/%3/%4_%5.qm").arg(getAppParentDir()).arg(LANG_FILE_PATH).arg(lang).arg(name).arg(lang));
		QCoreApplication::installTranslator(translator);

		m_translators.append(translator);
	}

	QString strCodecFilePath=QString("%1%2/%3/codec.ini").arg(getAppParentDir()).arg(LANG_FILE_PATH).arg(lang);
	QString strCodec=parseConfig(strCodecFilePath, "codec/cstrings", "UTF-8").toString();

	QTextCodec *codec = QTextCodec::codecForName(strCodec.toLatin1());
	QTextCodec::setCodecForCStrings(codec);

#ifndef Q_OS_WIN
	strCodec=parseConfig(strCodecFilePath, "codec/locale", QTextCodec::codecForLocale()->name().data()).toString();
	codec = QTextCodec::codecForName(strCodec.toLatin1());
	QTextCodec::setCodecForLocale(codec);
#endif
}

void QTranslateInstaller::install()
{
	QString strLang=parseConfig(getAppParentDir()+INI_FILE, "main/lang", "zh").toString();
	if (strLang.isEmpty())
	{
		strLang="zh";
	}

	install(strLang);
}
