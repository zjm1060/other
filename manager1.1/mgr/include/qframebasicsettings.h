#ifndef QFRAMEBASICSETTINGS_H
#define QFRAMEBASICSETTINGS_H

#include <QFrame>
#include <QButtonGroup>
#include <QMap>

#include "ui_basicsettings.h"
#include "qreadwritexmlfile.h"

class QFrameBasicSettings : public QFrame
{
	Q_OBJECT

public:
	QFrameBasicSettings(QWidget *parent);
	~QFrameBasicSettings();

public:
	void refreshSetting(const QMap<QString, QString>& configure);

signals:
	void labelSettingChanged(const QMap<QString, QString>& configure);

private slots:
	void buttonClicked(QAbstractButton* button);

	void onClickedBtnSave();
	void onClickedBtnBrowse();

	void configureChanged(const QMap<QString, QString>& configure);
	void hostsConfigureChanged(const QMap<QString, QString>& configure);

	void currentIndexChanged(int index);

private:
	void initDeaSettings();
	void addDeaItemsToComboBox(QComboBox* comboBox, const QString& key, const QString& defaultValue);
	void saveDeaItemsConfigure(QComboBox* comboBox, const QString& key);

	void dataChanged();

private:
	Ui::FrameBasicSettings*	m_ui;

private:
	QButtonGroup*	m_pButtonGroup;

	QMonitorConfigureFile*	m_pMonitorActions;
	bool	m_bFirstLoadedActions;		//菜单配置文件是否是第一次加载

	QMonitorConfigureFile*	m_pMonitorHosts;		//主机列表配置文件
	QMap<QString, QString>	m_configureHosts;
	bool	m_bFirstLoadedHosts;		//主机配置文件是否是第一次加载
};

#endif // QFRAMEBASICSETTINGS_H
