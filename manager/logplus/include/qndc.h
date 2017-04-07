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

class LOGPLUS_EXPORT QNDC : public QSharedObject, protected QLogLogUser
{
public:
	virtual ~QNDC();

private:
	QNDC();
	QNDC(const QNDC&);
	QNDC& operator=(const QNDC&);

public:
	static QSharedObjectPtr<QNDC> instance();

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
	static QMutex	m_gMutex;
	static QSharedObjectPtr<QNDC> m_pInstance;
};

class LOGPLUS_EXPORT QNDCContextCreator
{
public:
	QNDCContextCreator(const QString& msg);

	~QNDCContextCreator();
};

#endif // QNDC_H
