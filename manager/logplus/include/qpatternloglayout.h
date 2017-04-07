#ifndef QPATTERNLOGLAYOUT_H
#define QPATTERNLOGLAYOUT_H

#include "qloglayout.h"

/**
* This is used by QPatternConverter class to inform them how to format
* their output.
*/
struct FormattingInfo
{
	int minLen;
	int maxLen;
	bool leftAlign;
	FormattingInfo() { reset(); }

	void reset();
	void dump(QLogLog&);
};

/**
* This is the base class of all "Converter" classes that format a
* field of QLoggingEvent objects.  In fact, the QPatternLayout
* class simply uses an array of QPatternConverter objects to format
* and append a logging event.
*/
class QPatternConverter : protected QLogLogUser
{
public:
	QPatternConverter(const FormattingInfo& info);
	virtual ~QPatternConverter();

	void formatAndAppend(QTextStream& output, const QLoggingEvent& event);

protected:
	virtual QString convert(const QLoggingEvent& event) = 0;

private:
	FormattingInfo m_format;
};

/**
* This QPatternConverter returns a constant string.
*/
class QLiteralPatternConverter : public QPatternConverter
{
public:
	QLiteralPatternConverter(const QString& str);
	virtual ~QLiteralPatternConverter();

public:
	virtual QString convert(const QLoggingEvent&);

private:
	QString m_str;
};

/**
* This QPatternConverter is used to format most of the "simple" fields
* found in the QLoggingEvent object.
*/
class QBasicPatternConverter : public QPatternConverter
{
public:
	enum Type
	{ 
		THREAD_CONVERTER,
		PROCESS_CONVERTER,
		LOGLEVEL_CONVERTER,
		NDC_CONVERTER,
		MESSAGE_CONVERTER,
		NEWLINE_CONVERTER,
		BASENAME_CONVERTER,
		FILE_CONVERTER,
		LINE_CONVERTER,
		FULL_LOCATION_CONVERTER
	};

public:
	QBasicPatternConverter(const FormattingInfo& info, Type type);

public:
	virtual QString convert(const QLoggingEvent& event);

private:
	QSharedObjectPtr<QLogLevelManager> m_levelManager;
	Type m_type;

private:
	// Disable copy
	QBasicPatternConverter(const QBasicPatternConverter&);
	QBasicPatternConverter& operator=(QBasicPatternConverter&);
};

/**
* This QPatternConverter is used to format the Logger field found in
* the QLoggingEvent object.
*/
class QLoggerPatternConverter : public QPatternConverter
{
public:
	QLoggerPatternConverter(const FormattingInfo& info, int precision);

public:
	virtual QString convert(const QLoggingEvent& event);

private:
	int m_nPrecision;
};

/**
* This QPatternConverter is used to format the timestamp field found in
* the QLoggingEvent object.  It will be formatted according to
* the specified "pattern".
*/
class QDatePatternConverter : public QPatternConverter
{
public:
	QDatePatternConverter(const FormattingInfo& info, const QString& format, bool useGMT);

public:
	virtual QString convert(const QLoggingEvent& event);

private:
	bool m_bUseGMT;
	QString m_strFormat;
};

/**
* This QPatternConverter is used to format the hostname field.
*/
class QHostnamePatternConverter : public QPatternConverter
{
public:
	QHostnamePatternConverter(const FormattingInfo& info);

public:
	virtual QString convert(const QLoggingEvent& event);

private:
	QString m_strHostName;
};

/**
* This QPatternConverter is used to format the NDC field found in
* the QLoggingEvent object, optionally limited to
* \c precision levels (using space to separate levels).
*/
class QNDCPatternConverter : public QPatternConverter
{
public:
	QNDCPatternConverter(const FormattingInfo& info, int precision);

public:
	virtual QString convert(const QLoggingEvent& event);

private:
	int m_nPrecision;
};

/**
* This class parses a "pattern" string into an array of QPatternConverter objects.
* 
* @see QPatternLayout for the formatting of the "pattern" string.
*
* @use [A-Za-z0-9]{1,*}[%(-{0,1}|.[\d]{1,*}|[\d]{1.*})]{0,1}[bcdDFhHilLmMmprtxX]
*/
typedef QVector<QPatternConverter*> QPatternConverterList;
class QPatternParser : protected QLogLogUser
{
public:
	QPatternParser(const QString& pattern, quint32 ndcMaxDepth);

private:
	enum ParserState
	{
		LITERAL_STATE, 
		CONVERTER_STATE,
		DOT_STATE,
		MIN_STATE,
		MAX_STATE
	};

public:
	QPatternConverterList parse();

private:
	QString extractOption();
	int extractPrecisionOption();
	void finalizeConverter(QChar c);

private:
	QString m_strPattern;
	FormattingInfo m_formattingInfo;
	QPatternConverterList m_listConverter;
	ParserState m_state;
	int m_nPos;
	QString m_strCurrentLiteral;
	unsigned m_nNDCMaxDepth;
};


class LOGPLUS_EXPORT QPatternLogLayout : public QLogLayout
{
public:
	QPatternLogLayout(const QString& pattern);
	QPatternLogLayout(const QProperties& properties);
	virtual ~QPatternLogLayout();

public:
	virtual void formatAndAppend(QTextStream& output, const QLoggingEvent& event);

protected:
	void init(const QString& pattern, quint32 ndcMaxDepth = 0);

protected:
	QString m_strPattern;
	QPatternConverterList m_patterns;

private: 
	// Disallow copying of instances of this class
	QPatternLogLayout(const QPatternLogLayout&);
	QPatternLogLayout& operator=(const QPatternLogLayout&);
};

#endif // QPATTERNLOGLAYOUT_H
