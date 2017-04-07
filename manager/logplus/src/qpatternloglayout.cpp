#include "qpatternloglayout.h"

#include "common.h"

#include <stdexcept>

#define ESCAPE_CHAR '%'

void FormattingInfo::reset()
{
	minLen = -1;
	maxLen = 0x7FFFFFFF;
	leftAlign = false;
}

void FormattingInfo::dump(QLogLog& loglog)
{
	loglog.debug("min=%d, max=%d, leftAlign=%s", minLen, maxLen, (leftAlign ? "true" : "false"));
}

QPatternConverter::QPatternConverter(const FormattingInfo& i)
{
	m_format.minLen = i.minLen;
	m_format.maxLen = i.maxLen;
	m_format.leftAlign = i.leftAlign;
}

QPatternConverter::~QPatternConverter()
{

}

void QPatternConverter::formatAndAppend(QTextStream& output, const QLoggingEvent& event)
{
	QString s=convert(event);
	int len=s.length();

	if(len > m_format.maxLen)
	{
		output<<s.left(m_format.maxLen);
	}
	else if(len < m_format.minLen)
	{
		if(m_format.leftAlign)
		{
			output<<s;
			output<<QString(m_format.minLen - len, QChar(' '));
		}
		else 
		{
			output<<QString(m_format.minLen - len, QChar(' '));
			output<<s;
		}
	}
	else 
	{
		output<<s;
	}
}

QLiteralPatternConverter::QLiteralPatternConverter(const QString& str)
:QPatternConverter(FormattingInfo()),
m_str(str)
{
}

QLiteralPatternConverter::~QLiteralPatternConverter()
{

}

QString QLiteralPatternConverter::convert(const QLoggingEvent&)
{
	return m_str;
}

QBasicPatternConverter::QBasicPatternConverter(const FormattingInfo& info, Type type)
:QPatternConverter(info),
m_levelManager(QLogLevelManager::instance()),
m_type(type)
{
}

QString QBasicPatternConverter::convert(const QLoggingEvent& event)
{
	switch(m_type)
	{
	case LOGLEVEL_CONVERTER: return m_levelManager->toString(event.level());
	case NDC_CONVERTER:      return event.ndc();
	case MESSAGE_CONVERTER:  return event.message();
	case NEWLINE_CONVERTER:  return "\n";
	case BASENAME_CONVERTER: return getParentDir(event.filename());
	case FILE_CONVERTER:     return event.filename();
	case THREAD_CONVERTER:   return event.thread(); 
	case PROCESS_CONVERTER:  return QString::number(getpid()); 
	case LINE_CONVERTER:
		{
			int line = event.line();
			if(line != -1)
			{
				return QString::number(line);
			}
			else
			{
				return QString();
			}
		}
	case FULL_LOCATION_CONVERTER:
		{
			QString const& filename = event.filename();
			if(! filename.isEmpty())
			{
				return   filename + ":" + QString::number(event.line());
			}
			else
			{
				return ":";
			}
		}
	}

	return "INTERNAL LOGPLUS ERROR";
}

QLoggerPatternConverter::QLoggerPatternConverter(const FormattingInfo& info, int precision)
: QPatternConverter(info),
m_nPrecision(precision)
{
}

QString QLoggerPatternConverter::convert(const QLoggingEvent& event)
{
	const QString& name = event.logger();
	if (m_nPrecision <= 0)
	{
		return name;
	}
	else
	{
		int nLen = name.length();

		// We substract 1 from 'len' when assigning to 'end' to avoid out of
		// bounds exception in return r.substring(end+1, len). This can happen
		// if precision is 1 and the logger name ends with a dot. 
		int nEnd= nLen - 1;
		for(int i=m_nPrecision; i>0; --i)
		{
			nEnd = name.lastIndexOf('.', nEnd - 1);
			if(nEnd == -1)
			{
				return name;
			}
		}
		return name.left(nEnd);
	}
}

QDatePatternConverter::QDatePatternConverter(const FormattingInfo& info,const QString& format, bool useGMT)
: QPatternConverter(info),
m_bUseGMT(useGMT),
m_strFormat(format)
{
	if (m_strFormat.isEmpty())
	{
		m_strFormat="yyyy-MM-dd hh:mm:ss.zzz";
	}
}

QString QDatePatternConverter::convert(const QLoggingEvent& event)
{
	if (m_bUseGMT)
	{
		return event.timestamp().toString(m_strFormat);
	}
	else
	{
		return event.timestamp().toUTC().toString(m_strFormat);
	}
}


QHostnamePatternConverter::QHostnamePatternConverter(const FormattingInfo& info)
: QPatternConverter(info)
, m_strHostName(getHostName())
{ }


QString QHostnamePatternConverter::convert(const QLoggingEvent&)
{
	return m_strHostName;
}


QNDCPatternConverter::QNDCPatternConverter(const FormattingInfo& info, int precision)
: QPatternConverter(info)
, m_nPrecision(precision)
{ }


QString	QNDCPatternConverter::convert(const QLoggingEvent& event)
{
	const QString& text = event.ndc();
	if (m_nPrecision <= 0)
		return text;
	else
	{
		int p = text.indexOf(QChar(' '));
		for (int i = 1; i < m_nPrecision && p != -1; ++i)
			p = text.indexOf(QChar(' '), p + 1);

		return text.left(p);
	}
}


QPatternParser::QPatternParser(const QString& pattern, quint32 ndcMaxDepth)
: m_strPattern(pattern)
, m_state(LITERAL_STATE)
, m_nPos(0)
, m_nNDCMaxDepth(ndcMaxDepth)
{
}

QString QPatternParser::extractOption() 
{
	if ((m_nPos < m_strPattern.length()) && (m_strPattern[m_nPos] == '{')) 
	{
		int end = m_strPattern.indexOf(QChar('}'), m_nPos);
		if (end != -1)
		{
			QString r = m_strPattern.mid(m_nPos+1, end-m_nPos-1);
			m_nPos = end + 1;
			return r;
		}
		else
		{
			logLog().error("No matching '}' found in conversion pattern string \""+m_strPattern+"\"");
			m_nPos= m_strPattern.length();
		}
	}

	return "";
}


int QPatternParser::extractPrecisionOption() 
{
	QString opt = extractOption();
	int r = 0;
	if(!opt.isEmpty())
	{
		r = opt.toInt();
	}
	return r;
}


QPatternConverterList QPatternParser::parse() 
{
	QChar c;
	int nPos = 0;
	while(nPos < m_strPattern.length())
	{
		c = m_strPattern[nPos++];
		switch (m_state)
		{
		case LITERAL_STATE :
			// In literal state, the last char is always a literal.
			if(nPos == m_strPattern.length())
			{
				m_strCurrentLiteral += c;
				continue;
			}

			if(c == ESCAPE_CHAR)
			{
				// peek at the next char. 
				switch (m_strPattern[nPos].digitValue())
				{
				case ESCAPE_CHAR:
					m_strCurrentLiteral += c;
					nPos++; // move pointer
					break;
				default:
					if(!m_strCurrentLiteral.isEmpty())
					{
						m_listConverter.append(new QLiteralPatternConverter(m_strCurrentLiteral));
					}

					m_strCurrentLiteral.clear();
					m_strCurrentLiteral += c; // append %
					m_state = CONVERTER_STATE;
					m_formattingInfo.reset();
				}
			}
			else
			{
				m_strCurrentLiteral += c;
			}
			break;
		case CONVERTER_STATE:
			m_strCurrentLiteral += c;
			switch (c.digitValue())
			{
			case '-':
				m_formattingInfo.leftAlign = true;
				break;
			case '.':
				m_state = DOT_STATE;
				break;
			default:
				if(c >= QChar('0') && c <= QChar('9'))
				{
					m_formattingInfo.minLen = c.digitValue() - QChar('0').digitValue();
					m_state = MIN_STATE;
				}
				else
				{
					finalizeConverter(c);
				}
			} // switch
			break;
		case MIN_STATE:
			m_strCurrentLiteral += c;
			if (c >= QChar('0') && c <= QChar('9')) {
				m_formattingInfo.minLen = m_formattingInfo.minLen * 10 + (c.digitValue() - QChar('0').digitValue());
			}
			else if(c == QChar('.'))
			{
				m_state = DOT_STATE;
			}
			else
			{
				finalizeConverter(c);
			}
			break;

		case DOT_STATE:
			m_strCurrentLiteral += c;
			if(c >= QChar('0') && c <= QChar('9'))
			{
				m_formattingInfo.maxLen = c.digitValue() - QChar('0').digitValue();
				m_state = MAX_STATE;
			}
			else
			{
				logLog().error("Error occured in position %d.\n Was expecting digit, instead got char \"%c\".", nPos, c.toAscii());
				m_state = LITERAL_STATE;
			}
			break;
		case MAX_STATE:
			m_strCurrentLiteral += c;
			if (c >= QChar('0') && c <= QChar('9'))
				m_formattingInfo.maxLen = m_formattingInfo.maxLen * 10 + (c.digitValue() - QChar('0').digitValue());
			else
			{
				finalizeConverter(c);
				m_state = LITERAL_STATE;
			}
			break;
		} // end switch
	} // end while

	if(!m_strCurrentLiteral.isEmpty())
	{
		m_listConverter.append(new QLiteralPatternConverter(m_strCurrentLiteral));

		logLog().debug("Parsed LITERAL converter: \""+m_strCurrentLiteral+"\".");
	}

	return m_listConverter;
}

void QPatternParser::finalizeConverter(QChar c) 
{
	QPatternConverter* pc = 0;
	switch (c.digitValue())
	{
	case 'b':
		pc = new QBasicPatternConverter(m_formattingInfo, QBasicPatternConverter::BASENAME_CONVERTER);    
		break;

	case 'c':
		pc = new QLoggerPatternConverter(m_formattingInfo, extractPrecisionOption());
		logLog().debug("LOGGER converter.");
		m_formattingInfo.dump(logLog());      
		break;

	case 'd':
	case 'D':
		{
			QString dOpt = extractOption();
			if(dOpt.isEmpty())
			{
				dOpt ="yyyy-MM-dd hh:mm:ss.zzz";
			}
			bool bUseGMT= (c == QChar('d'));
			pc = new QDatePatternConverter(m_formattingInfo, dOpt, bUseGMT);
		}
		break;

	case 'F':
		pc = new QBasicPatternConverter(m_formattingInfo, QBasicPatternConverter::FILE_CONVERTER);    
		break;

	case 'h':
	case 'H':
		{
			pc = new QHostnamePatternConverter(m_formattingInfo);
		}
		break;

	case 'i':
		pc = new QBasicPatternConverter(m_formattingInfo, QBasicPatternConverter::PROCESS_CONVERTER);     
		break;

	case 'l':
		pc = new QBasicPatternConverter(m_formattingInfo, QBasicPatternConverter::FULL_LOCATION_CONVERTER);    
		break;

	case 'L':
		pc = new QBasicPatternConverter(m_formattingInfo, QBasicPatternConverter::LINE_CONVERTER);    
		break;

	case 'm':
		pc = new QBasicPatternConverter(m_formattingInfo, QBasicPatternConverter::MESSAGE_CONVERTER);     
		break;

	case 'M':
		goto not_implemented;

	case 'n':
		pc = new QBasicPatternConverter(m_formattingInfo, QBasicPatternConverter::NEWLINE_CONVERTER);    
		break;

	case 'p':
		pc = new QBasicPatternConverter(m_formattingInfo, QBasicPatternConverter::LOGLEVEL_CONVERTER);
		break;

	case 'r':
		goto not_implemented;

	case 't':
		pc = new QBasicPatternConverter(m_formattingInfo, QBasicPatternConverter::THREAD_CONVERTER);    
		break;

	case 'x':
		pc = new QNDCPatternConverter(m_formattingInfo, m_nNDCMaxDepth);
		break;

	case 'X':
		goto not_implemented;

not_implemented:;
	default:
		logLog().error("Unexpected char [%c] at position %d in conversion patterrn.", c.toAscii(), m_nPos);
		pc = new QLiteralPatternConverter(m_strCurrentLiteral);
	}

	m_strCurrentLiteral.resize(0);
	m_listConverter.append(pc);
	m_state = LITERAL_STATE;
	m_formattingInfo.reset();
}


QPatternLogLayout::QPatternLogLayout(const QString& pattern)
{
	init(pattern, 0);
}

QPatternLogLayout::QPatternLogLayout(const QProperties& properties)
{
	quint32 nNDCMaxDepth=properties.property("NDCMaxDepth",QVariant("0")).toUInt();

	bool hasConversionPattern = properties.exists("ConversionPattern");

	if(hasConversionPattern)
	{
		init(properties.property("ConversionPattern").toString(), nNDCMaxDepth);
	}
	else
	{
		throw std::runtime_error("ConversionPattern not specified in properties");
	}
}

void QPatternLogLayout::init(const QString& pattern, quint32 ndcMaxDepth)
{
	m_strPattern = pattern;
	m_patterns = QPatternParser(pattern, ndcMaxDepth).parse();

	for(int i=0; i<m_patterns.size(); i++)
	{
		if( m_patterns[i] == 0 )
		{
			logLog().error("Parsed Pattern created a NULL PatternConverter");
			m_patterns[i] = new QLiteralPatternConverter("");
		}
	}

	if(m_patterns.isEmpty())
	{
		logLog().warn("PatternLayout pattern is empty.  Using default...");
		m_patterns.append(new QBasicPatternConverter(FormattingInfo(), QBasicPatternConverter::MESSAGE_CONVERTER));
	}
}

QPatternLogLayout::~QPatternLogLayout()
{
	for(int i=0; i<m_patterns.size(); i++)
	{
		delete m_patterns[i];
	}

	m_patterns.clear();
}

void QPatternLogLayout::formatAndAppend(QTextStream& output, const QLoggingEvent& event)
{
	for(int i=0; i<m_patterns.size(); i++)
	{
		m_patterns[i]->formatAndAppend(output, event);
	}
}
