#include "qframebasicsettings.h"

#include <QMessageBox>
#include <QFileDialog>

#include "common.h"
#include "qdeafactory.h"

QFrameBasicSettings::QFrameBasicSettings(QWidget *parent)
	: QFrame(parent)
	,m_bFirstLoadedActions(true)
	,m_bFirstLoadedHosts(true)
{
	m_ui=new Ui::FrameBasicSettings;
	m_ui->setupUi(this);

	m_ui->pushButtonSave->setEnabled(false);

	m_pButtonGroup=new QButtonGroup(this);
	m_pButtonGroup->setExclusive(false);

	m_pButtonGroup->addButton(m_ui->checkBoxFileTransfer);
	m_pButtonGroup->addButton(m_ui->checkBoxCommitUpdate);
	m_pButtonGroup->addButton(m_ui->checkBoxDownloadUpdate);
	m_pButtonGroup->addButton(m_ui->checkBoxSynchronization);
	m_pButtonGroup->addButton(m_ui->checkBoxInspection);

	connect(m_pButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

	connect(m_ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(onClickedBtnSave()));

	m_pMonitorActions=new QMonitorConfigureFile(appendPath(getAppParentDir(), CONFIGURE_SETTING_FILE), this);

	connect(m_pMonitorActions, SIGNAL(changed(const QMap<QString, QString>&)), this, SLOT(configureChanged(const QMap<QString, QString>&)));
	m_pMonitorActions->load();

	connect(m_ui->pushButtonBrowse, SIGNAL(clicked()), this, SLOT(onClickedBtnBrowse()));

	m_pMonitorHosts=new QMonitorConfigureFile(appendPath(getAppParentDir(), TRANSFER_SETTING_FILE), this);

	connect(m_pMonitorHosts, SIGNAL(changed(const QMap<QString, QString>&)), this, SLOT(hostsConfigureChanged(const QMap<QString, QString>&)));
	m_pMonitorHosts->load();

	initDeaSettings();
}

QFrameBasicSettings::~QFrameBasicSettings()
{
	onClickedBtnSave();
}

void QFrameBasicSettings::addDeaItemsToComboBox(QComboBox* comboBox, const QString& key, const QString& defaultValue)
{
	if (comboBox == NULL)
	{
		return;
	}

	comboBox->addItem(tr("Plain Text"), PlainText);
	comboBox->addItem(tr("DES"), DES);
	comboBox->addItem(tr("3DES"), TripleDES);

	QString strDea=parseConfig(getAppParentDir()+INI_FILE, key, defaultValue).toString();
	QIDEAPtr ptr=QDEAFactory::instance()->dea(strDea);
	if (ptr == NULL)
	{
		ptr=QDEAFactory::instance()->dea(defaultValue);
	}

	int nDeaType=PlainText;
	if (ptr != NULL)
	{
		nDeaType=ptr->type();
	}

	comboBox->setCurrentIndex(nDeaType+1);

	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
}

void QFrameBasicSettings::initDeaSettings()
{
	addDeaItemsToComboBox(m_ui->comboBoxMessages, "dea/messages", "PlainText");
	addDeaItemsToComboBox(m_ui->comboBoxHeartBeats, "dea/heartBeats", "DES");
	addDeaItemsToComboBox(m_ui->comboBoxAccounts, "dea/accounts", "TripleDES");
}

void QFrameBasicSettings::buttonClicked(QAbstractButton* button)
{
	Q_UNUSED(button);

	dataChanged();
}

void QFrameBasicSettings::currentIndexChanged(int index)
{
	Q_UNUSED(index);

	dataChanged();
}

void QFrameBasicSettings::dataChanged()
{
	m_ui->pushButtonSave->setEnabled(true);
}

void QFrameBasicSettings::configureChanged(const QMap<QString, QString>& configure)
{
	if (!m_bFirstLoadedActions)
	{
		QMessageBox::StandardButton button=QMessageBox::information(this, tr("Information"),
			tr("Configure file has been changed! Are you sure to reload?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

		if (button == QMessageBox::No)
		{
			return;
		}
	}

	m_bFirstLoadedActions=false;

	refreshSetting(configure);
}

void QFrameBasicSettings::hostsConfigureChanged(const QMap<QString, QString>& configure)
{
	if (configure.contains("ServerFile") && configure["ServerFile"] == m_ui->lineEditHostFile->text())
	{
		return;
	}

	if (!m_bFirstLoadedHosts)
	{
		QMessageBox::StandardButton button=QMessageBox::information(this, tr("Information"),
			tr("Configure file has been changed! Are you sure to reload?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

		if (button == QMessageBox::No)
		{
			return;
		}
	}

	m_bFirstLoadedHosts=false;

	m_configureHosts=configure;

	QString strFile;
	if (configure.contains("ServerFile"))
	{
		strFile=configure["ServerFile"];
		convertToStdDir(strFile);
	}
	else
	{
		strFile=appendPath(getAppParentDir(), "def/ems_config.def");
		m_configureHosts["ServerFile"]=strFile;
	}

	m_ui->lineEditHostFile->setText(strFile);
}

void QFrameBasicSettings::saveDeaItemsConfigure(QComboBox* comboBox, const QString& key)
{
	if (comboBox == NULL)
	{
		return;
	}

	int index=comboBox->currentIndex();
	if (index == -1)
	{
		return;
	}

	index--;
	Q_ASSERT(index >= PlainText && index<=TripleDES);
	QIDEAPtr ptr=QDEAFactory::instance()->dea(index);
	if (ptr != NULL)
	{
		codeConfig(getAppParentDir()+INI_FILE, key, ptr->desc());
	}
}

void QFrameBasicSettings::onClickedBtnSave()
{
	if (m_ui->pushButtonSave->isEnabled())
	{
		if (m_pMonitorActions != NULL)
		{
			QMap<QString, QString> configure;

			configure["FileTransfer"]=(m_ui->checkBoxFileTransfer->checkState() == Qt::Checked? "true":"false");
			configure["CommitSync"]=(m_ui->checkBoxCommitUpdate->checkState() == Qt::Checked? "true":"false");
			configure["UpdateSync"]=(m_ui->checkBoxDownloadUpdate->checkState() == Qt::Checked? "true":"false");
			configure["FileRelease"]=(m_ui->checkBoxSynchronization->checkState() == Qt::Checked? "true":"false");
			configure["Inspect"]=(m_ui->checkBoxInspection->checkState() == Qt::Checked? "true":"false");

			m_pMonitorActions->save(configure);

			m_ui->pushButtonSave->setEnabled(false);

			emit labelSettingChanged(configure);
		}

		if (m_pMonitorHosts != NULL)
		{
			m_pMonitorHosts->save(m_configureHosts);
		}

		saveDeaItemsConfigure(m_ui->comboBoxMessages, "dea/messages");
		saveDeaItemsConfigure(m_ui->comboBoxHeartBeats, "dea/heartBeats");
		saveDeaItemsConfigure(m_ui->comboBoxAccounts, "dea/accounts");
	}
}

void QFrameBasicSettings::refreshSetting(const QMap<QString, QString>& configure)
{
	if (!configure.contains("FileTransfer") || configure["FileTransfer"] == "true")
	{
		m_ui->checkBoxFileTransfer->setCheckState(Qt::Checked);
	}
	else
	{
		m_ui->checkBoxFileTransfer->setCheckState(Qt::Unchecked);
	}

	if (!configure.contains("CommitSync") || configure["CommitSync"] == "true")
	{
		m_ui->checkBoxCommitUpdate->setCheckState(Qt::Checked);
	}
	else
	{
		m_ui->checkBoxCommitUpdate->setCheckState(Qt::Unchecked);
	}

	if (!configure.contains("UpdateSync") || configure["UpdateSync"] == "true")
	{
		m_ui->checkBoxDownloadUpdate->setCheckState(Qt::Checked);
	}
	else
	{
		m_ui->checkBoxDownloadUpdate->setCheckState(Qt::Unchecked);
	}

	if (!configure.contains("FileRelease") || configure["FileRelease"] == "true")
	{
		m_ui->checkBoxSynchronization->setCheckState(Qt::Checked);
	}
	else
	{
		m_ui->checkBoxSynchronization->setCheckState(Qt::Unchecked);
	}

	if (!configure.contains("Inspect") || configure["Inspect"] == "true")
	{
		m_ui->checkBoxInspection->setCheckState(Qt::Checked);
	}
	else
	{
		m_ui->checkBoxInspection->setCheckState(Qt::Unchecked);
	}
}

void QFrameBasicSettings::onClickedBtnBrowse()
{
	QString strFileName=m_ui->lineEditHostFile->text();
	QString strFile = QFileDialog::getOpenFileName(this, tr("Switch Workspace"), getParentDir(strFileName));
	if (strFile.isEmpty() || strFileName == strFile)
	{
		return;
	}

	m_ui->lineEditHostFile->setText(strFile);

	m_configureHosts["ServerFile"]=strFile;

	dataChanged();
}
