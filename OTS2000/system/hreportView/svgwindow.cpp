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

#include <QtGui>

#include "svgview.h"
#include "svgwindow.h"

SvgWindow::SvgWindow()
    : QScrollArea()
{
    view = new QWidget(this);
    renderer = SvgWindow::Image;
    setWidget(view);
	view->resize(1024, 768);
}

void SvgWindow::openFile(const QString &file)
{
//    setRenderer(renderer);
}

void SvgWindow::setRenderer(RendererType type)
{
    renderer = type;
}


void SvgWindow::mousePressEvent(QMouseEvent *event)
{
    mousePressPos = event->pos();
    scrollBarValuesOnMousePress.rx() = horizontalScrollBar()->value();
    scrollBarValuesOnMousePress.ry() = verticalScrollBar()->value();
    event->accept();
}

void SvgWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mousePressPos.isNull()) {
        event->ignore();
        return;
    }

    horizontalScrollBar()->setValue(scrollBarValuesOnMousePress.x() - event->pos().x() + mousePressPos.x());
    verticalScrollBar()->setValue(scrollBarValuesOnMousePress.y() - event->pos().y() + mousePressPos.y());
    horizontalScrollBar()->update();
    verticalScrollBar()->update();
    event->accept();
}

void SvgWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressPos = QPoint();
    event->accept();
}


void SvgWindow::displaySvg(const QByteArray &svgContents)
{
	QSize view_size(1024, 768);
    if(view)
	{
		view_size= view->size();
		delete view;
	}
	
    if (renderer == Image) {
        view = new SvgRasterView(svgContents, this);
    } else {
        view = new SvgNativeView(svgContents, this);
    }
	
    setWidget(view);
	view->resize(view_size);
    view->show();
}

void SvgWindow::displaySvg(const QString &file)
{
	QSize view_size(1024, 768);
    if(view)
	{
		view_size= view->size();
		delete view;
	}
	
    if (renderer == Image) {
        view = new SvgRasterView(file, this);
    } else {
        view = new SvgNativeView(file, this);
    }
    setWidget(view);
	view->resize(view_size);
    view->show();
}

