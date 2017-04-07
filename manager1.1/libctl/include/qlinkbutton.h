#ifndef QLINKBUTTON_H
#define QLINKBUTTON_H

#include <QAbstractButton>
#include <QColor>
#include <QStyleOptionButton>

#include "libctlglobal.h"

class CTL_EXPORT QLinkButton : public QAbstractButton
{
	Q_OBJECT

public:
	QLinkButton(QWidget *parent=0);
	QLinkButton(const QString& text, QWidget* parent=0);
	QLinkButton(const QIcon& icon, const QString& text, QWidget* parent=0);
	~QLinkButton();

public:
	QSize sizeHint() const;
	QSize minimumSizeHint() const;

public:
	void setReadOnly(bool enable);
	void setAlignment(Qt::Alignment alignment);

protected:
	void initStyleOption(QStyleOptionButton *option) const;

protected:
	virtual void paintEvent(QPaintEvent* event);

private:
	void init();

private:
	enum ColorRoleType
	{
		EnterColorRole,
		LeaveColorRole,
		ColorTypeEnd
	};

	QColor	m_color[ColorTypeEnd];

	bool	m_bReadOnly;
	Qt::Alignment	m_alignment;
};

#endif // QLINKBUTTON_H
