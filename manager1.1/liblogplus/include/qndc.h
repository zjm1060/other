#ifndef QNDC_H
#define QNDC_H

#include <QStack>
#include <QThreadStorage>

#include "qsharedobject.h"
#include "qloglog.h"

struct LOGPLUS_EXPORT DiagnosticContext 
{
	DiagnosticContext(){};
	DiagnosticContext(const QString& message, DiagnosticContext *parent);
	DiagnosticContext(const QString& message);

	QString message;		//The message at this context level.
	QString fullMessage;	//The entire message stack.
};

typedef QStack<DiagnosticContext> DiagnosticContextStack;

class QNdc;
typedef QSharedObjectPtr<QNdc> QNdcPtr;
class LOGPLUS_EXPORT QNdc : public QSharedObject, protected QLogLogUser
{
public:
	virtual ~QNdc();

private:
	QNdc();
	QNdc(const QNdc&);
	QNdc& operator=(const QNdc&);

public:
	static QNdcPtr instance();

public:
	
	void clear();

	DiagnosticContextStack cloneStack();
	void inherit(const DiagnosticContextStack& stack);

	QString get();


	int depth();

	QString pop();
	QString peek();
	void push(const QString& message);
	void remove();

	void setMaxDepth(int maxDepth);

private:
	DiagnosticContextStack* stack();

public:
	static QThreadStorage<DiagnosticContextStack*>	g_threadStorage;

private:
	static QMutex	m_mutex;
	static QNdcPtr	m_pInstance;
};

class LOGPLUS_EXPORT QNdcContextCreator
{
public:
	QNdcContextCreator(const QString& msg);

	~QNdcContextCreator();
};

#endif // QNDC_H
