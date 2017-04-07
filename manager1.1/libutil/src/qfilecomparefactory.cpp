#include "qfilecomparefactory.h"

#include "define.h"
#include "qlogger.h"

#include <QLibrary>

QFileCompareFactory::QFileCompareFactory()
{
	qRegisterMetaType<DiffFile>("DiffFile");
	qRegisterMetaType<QFileCompareFactory::FileCompareResult>("QFileCompareFactory::FileCompareResult");
}

QFileCompareFactory::~QFileCompareFactory()
{
	
}

QFileCompareFactory* QFileCompareFactory::instance()
{
	static QFileCompareFactory* pInstance=new QFileCompareFactory;

	return pInstance;
}

void QFileCompareFactory::registerProduct(const QAbstractFileComparePtr& ptr)
{
	Q_ASSERT_NO_NULL(ptr);

	if (m_pChains != NULL)
	{
		QAbstractFileComparePtr temp;
		do 
		{
			if (temp == NULL)
			{
				temp=m_pChains;
			}
			else
			{
				temp=temp->nextChain();
			}
		} while (temp->nextChain() != NULL);

		temp->setNextChain(ptr);
	}
	else
	{
		m_pChains=ptr;
	}
}

bool QFileCompareFactory::loadLibrary(const QString& filename, const char* symbol)
{
	CreateFileCompareInstanceFunc func;

	QLibrary library(filename);
	func=(CreateFileCompareInstanceFunc)library.resolve(symbol);

	if (func != 0)
	{
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" succeed.").arg(symbol).arg(filename));

		QVector<QAbstractFileComparePtr> instances;
		func(instances);

		for (int i=0; i<instances.size()-1; i++)
		{
			for (int j=i+1; j<instances.size(); j++)
			{
				if (instances[i]->type() > instances[j]->type())
				{
					QAbstractFileComparePtr temp=instances[i];
					instances[i]=instances[j];
					instances[j]=temp;
				}
			}
		}

		foreach(QAbstractFileComparePtr pTool, instances)
		{
			if (pTool != 0)
			{
				instance()->registerProduct(pTool);
			}
		}
	}
	else
	{
		logplusError(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" failed! %3").arg(symbol).arg(filename)
			.arg(library.errorString()));
		return false;
	}

	return true;
}

QFileCompareFactory::FileCompareResult QFileCompareFactory::compare(const QString& source, const QString& target)
{
	QFileCompareFactory::FileCompareResult result;

	if (m_pChains != NULL)
	{
		result.source=m_pChains->parse(source);
		result.target=m_pChains->parse(target);

		m_pChains->compare(result.source, result.target);
	}

	return result;
}

bool QFileCompareFactory::isModified(const QString& source, const QString& target)
{
	QFileCompareFactory::FileCompareResult result;

	if (m_pChains != NULL)
	{
		result.source=m_pChains->parse(source);
		result.target=m_pChains->parse(target);

		bool bModified=m_pChains->isModified(result.source, result.target);

		SAFE_DELETE_CLASS(result.source.model);
		SAFE_DELETE_CLASS(result.target.model);

		return bModified;
	}

	return false;
}

bool QFileCompareFactory::isHeaderHidden(const FileCompareResult& result) const
{
	if (m_pChains != NULL)
	{
		return m_pChains->isHeaderHidden(result.source, result.target);
	}

	return true;
}

QAbstractFileCompare::ErrorCode QFileCompareFactory::write(QAbstractFileCompare::FileCompareResult& source, 
														   QAbstractFileCompare::FileCompareResult& target, QPair<QModelIndex, QModelIndex>& error)
{
	if (m_pChains != NULL)
	{
		return (QAbstractFileCompare::ErrorCode)m_pChains->write(source, target, error);
	}
	
	return QAbstractFileCompare::UnknowError;
}
