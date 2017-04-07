#include "qlinkbutton.h"

#include <QStylePainter>
#include <QDialogButtonBox>
#include <QApplication>

QLinkButton::QLinkButton(QWidget *parent)
	: QAbstractButton(parent)
{
	init();
}

QLinkButton::QLinkButton(const QString& text, QWidget* parent/* =0 */)
:QAbstractButton(parent)
{
	init();

	setText(text);
}

QLinkButton::QLinkButton(const QIcon& icon, const QString& text, QWidget* parent/* =0 */)
:QAbstractButton(parent)
{
	init();

	setIcon(icon);
	setText(text);
}

QLinkButton::~QLinkButton()
{

}

void QLinkButton::init()
{
	m_color[EnterColorRole]=QColor(7, 64, 229);
	m_color[LeaveColorRole]=QColor(64, 128, 212);
	setIconSize(QSize(16, 16));

	QFont ft=font();
	ft.setWeight(QFont::DemiBold);
	ft.setPointSize(10);
	setFont(ft);

	setReadOnly(false);
	setAlignment(Qt::AlignVCenter);
}

void QLinkButton::setReadOnly(bool enable)
{
	m_bReadOnly=enable;

	setCursor(m_bReadOnly? QCursor(Qt::ArrowCursor):QCursor(Qt::PointingHandCursor));
}

void QLinkButton::setAlignment(Qt::Alignment alignment)
{
	m_alignment|=alignment;
}

void QLinkButton::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QStylePainter p(this);
	QStyleOptionButton option;
	initStyleOption(&option);

	QSize pixmapSize = icon().actualSize(iconSize());
	QRect rect=option.rect;

	//Draw icon
	p.drawControl(QStyle::CE_PushButton, option);
	if (!icon().isNull())
		p.drawPixmap(rect.left(), rect.top()+(rect.height()-pixmapSize.height())/2, 
		icon().pixmap(pixmapSize, isEnabled() ? QIcon::Normal : QIcon::Disabled, isChecked() ? QIcon::On : QIcon::Off));

	//Draw title
	QColor textColor = palette().buttonText().color();
	QFont textFont=font();
	if (!m_bReadOnly && isEnabled())
	{
		textColor = m_color[LeaveColorRole];
		if (underMouse())
		{
			textColor = m_color[EnterColorRole];
			textFont.setUnderline(true);
		}

		option.palette.setColor(QPalette::ButtonText, textColor);
	}

	int textflags = Qt::TextShowMnemonic|m_alignment;
	if (!style()->styleHint(QStyle::SH_UnderlineShortcut, &option, this))
		textflags |= Qt::TextHideMnemonic|Qt::TextWordWrap;

	p.setFont(textFont);
	p.drawItemText(option.rect.adjusted(pixmapSize.width()+2, 0, 0, 0), textflags, option.palette, isEnabled(), text(), QPalette::ButtonText);
}

void QLinkButton::initStyleOption(QStyleOptionButton *option) const
{
	if (!option)
		return;

	option->initFrom(this);
	option->features = QStyleOptionButton::Flat;

	option->text = QString();
	option->icon = QIcon();
	option->iconSize = iconSize();
}

QSize QLinkButton::sizeHint() const
{
	ensurePolished();

	int w = 0, h = 0;

	QStyleOptionButton opt;
	initStyleOption(&opt);

	// calculate contents size...
#ifndef QT_NO_ICON

	bool showButtonBoxIcons = qobject_cast<QDialogButtonBox*>(parentWidget())
		&& style()->styleHint(QStyle::SH_DialogButtonBox_ButtonsHaveIcons);

	if (!icon().isNull() || showButtonBoxIcons) 
	{
		int ih = opt.iconSize.height();
		int iw = opt.iconSize.width() + 4;
		w += iw;
		h = qMax(h, ih);
	}
#endif
	QString s(text());
	bool empty = s.isEmpty();
	if (empty)
		s = QString::fromLatin1("XXXX");
	QFontMetrics fm = fontMetrics();
	QSize sz = fm.size(Qt::TextShowMnemonic, s);
	if(!empty || !w)
		w += sz.width();
	if(!empty || !h)
		h = qMax(h, sz.height());
	opt.rect.setSize(QSize(w, h)); // PM_MenuButtonIndicator depends on the height

	QSize size = (style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(w, h), this).expandedTo(QApplication::globalStrut()));
	return size;
}

QSize QLinkButton::minimumSizeHint() const
{
	return sizeHint();
}
