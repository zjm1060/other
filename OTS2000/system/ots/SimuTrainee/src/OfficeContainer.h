#ifndef __OFFICECONTAINER_H__
#define __OFFICECONTAINER_H__

#include "dsoframer.h"
using namespace DSOFramer;

class OfficeContainer: public QWidget
{
	Q_OBJECT
public:
	enum DialogType {
		officeDialogNew            = 0,
		officeDialogOpen           = 1,
		officeDialogSave           = 2,
		officeDialogSaveCopy       = 3,
		officeDialogPrint          = 4,
		officeDialogPageSetup      = 5,
		officeDialogProperties     = 6
	};
	OfficeContainer(QWidget *parent = NULL);
	~OfficeContainer();

	void Open(const QVariant& Document);
	void Open(const QVariant& Document, const QVariant& ReadOnly);
	void Close();

	void Save();
	void Save(const QVariant& SaveAsDocument);
	void Save(const QVariant& SaveAsDocument, const QVariant& OverwriteExisting);

	QString DocumentFullName() const;
	QString DocumentName() const;
	bool IsDirty() const;
	bool IsReadOnly() const;
	void PrintPreview();
	void PrintPreviewExit();
	
	void ShowDialog(DialogType DlgType);
	//DZY added
	void showwait();
	void showtips();
	QGridLayout*ply2;
	QFrame	*pFramer;
	QWidget *waitDialog;
	QTimer *ptimer;
private:
	QGridLayout*ply;
	FramerControl	*pFramerControl;
public slots:
	void 	close_wait();

};

#endif