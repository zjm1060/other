#ifndef QPROPERTIES_H
#define QPROPERTIES_H

#include <QMap>
#include <QVariant>
#include <QString>

#include "global.h"

class GLOBAL_EXPORT QProperties
{
public:
	QProperties();
	QProperties(const QProperties& properties);
	virtual ~QProperties();

public:
	bool exists(const QString& key) const;
	int size() const;

	QVariant property(const QString& key) const;
	QVariant property(const QString& key, const QVariant& defaultVal) const;
	void setProperty(const QString& key, const QVariant& value);

	bool removeProperty(const QString& key);

	QList<QString> propertyNames() const;
	QProperties propertySubset(const QString& prefix) const;

private:
	QMap<QString, QVariant>		m_data;
};

#endif // QPROPERTIES_H
