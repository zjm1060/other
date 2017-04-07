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

#ifndef SVGWINDOW_H
#define SVGWINDOW_H

#include <QPoint>
#include <QScrollArea>
#include <QString>

QT_BEGIN_NAMESPACE
class QKeyEvent;
class QMouseEvent;
QT_END_NAMESPACE


class SvgWindow : public QScrollArea
{
public:
	enum RendererType { Native, Image };

    SvgWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void openFile(const QString &file);
    void setRenderer(RendererType type);
    void displaySvg(const QByteArray &svgContents);
    void displaySvg(const QString &file);
	
	QWidget *view;

private:
    QPoint mousePressPos;
    QPoint scrollBarValuesOnMousePress;
    RendererType renderer;
	QByteArray reportContents;
	
};

#endif
