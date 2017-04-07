#include "qproperties.h"

#include <QStringList>

QProperties::QProperties()
{

}

QProperties::QProperties(const QProperties& properties)
{
	m_data=properties.m_data;
}

QProperties::~QProperties()
{

}

bool QProperties::exists(const QString& key) const
{
	return m_data.contains(key);
}

int QProperties::size() const
{
	return m_data.size();
}

QVariant QProperties::property(const QString& key) const
{
	if (!exists(key))
	{
		return QVariant();
	}
	else
	{
		return m_data[key];
	}
}

QVariant QProperties::property(const QString& key, const QVariant& defaultVal) const
{
	if (!exists(key))
	{
		return defaultVal;
	}
	else
	{
		return m_data[key];
	}
}

void QProperties::setProperty(const QString& key, const QVariant& value)
{
	m_data[key]=value;
}

QList<QString> QProperties::propertyNames() const
{
	return m_data.keys();
}

bool QProperties::removeProperty(const QString& key)
{
	return m_data.remove(key)>0? true:false;
}

QProperties QProperties::propertySubset(const QString& prefix) const
{
	QProperties ret;
	
	QStringList const keys=prefix.split(QChar(' '), QString::SkipEmptyParts);

	foreach (QString key, keys)
	{
		ret.setProperty(key, property(key));
	}

	return ret;
}
