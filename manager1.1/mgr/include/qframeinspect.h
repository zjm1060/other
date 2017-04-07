#ifndef QFRAMEINSPECT_H
#define QFRAMEINSPECT_H

#include <QFrame>
#include <QPixmap>

#include "ui_inspect.h"
#include "qlinkbutton.h"
#include "qtreeviewinspect.h"
#include "qtreeviewinspectsetting.h"

class QFrameInspect : public QFrame, public Ui::FrameInspect
{
	Q_OBJECT

public:
	QFrameInspect(QWidget* parent=0);
	~QFrameInspect();

public:
	void setInspectItems(const ServerInfo& info, const QTreeViewInspectSetting::InspectItem& items);

signals:
	void goBack();

private slots:
	void message(QString tip);
	void setProgress(int value, int min, int max);
	void finished(int normal, int skip, int total);	//Ñ²¼ìÍê³É
	void addInspectItemsFinished();

	void onClickedBtnPause();
	void onClickedBtnCancel();
	void onClichedBtnBack();
	void onClickedBtnReinspect();

private:
	void setInspectLayout();
	void inspect(bool init=false);

private:
	bool	m_bPause;

	QLinkButton*		m_pLinkBtnReinspect;
	QTreeViewInspect*	m_pTreeViewInspect;
};

#endif // QFRAMEINSPECT_H
