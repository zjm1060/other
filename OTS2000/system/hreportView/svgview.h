/****************************************************************************
**
** Copyright (C) 2005-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef SVGVIEW_H
#define SVGVIEW_H


#include <QWidget>
#include <QImage>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QSvgRenderer;
class QWheelEvent;
QT_END_NAMESPACE

class SvgRasterView : public QWidget
{
    Q_OBJECT

public:
    SvgRasterView(const QString &file, QWidget *parent=0);
    SvgRasterView(const QByteArray &contents, QWidget *parent=0);

    virtual QSize sizeHint() const;

protected slots:
    void poluteImage();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
	
private:
    QSvgRenderer *doc;
    QImage buffer;
    bool m_dirty;
};

class SvgNativeView : public QWidget
{
    Q_OBJECT

public:
    SvgNativeView(const QString &file, QWidget *parent=0);
    SvgNativeView(const QByteArray &contents, QWidget *parent=0);
	
    virtual QSize sizeHint() const;
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    QSvgRenderer *doc;
};


#endif
