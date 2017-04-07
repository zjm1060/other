#include "qframeauthority.h"

QFrameAuthority::QFrameAuthority(QWidget *parent)
	: QFrame(parent)
	,m_bEditable(true)
	,m_bUserRoleEditable(true)
{
	m_ui=new Ui::FrameAuthorities;
	m_ui->setupUi(this);
}

QFrameAuthority::~QFrameAuthority()
{
	SAFE_DELETE_CLASS(m_ui);
}

void QFrameAuthority::setEditable(bool enable)
{
	m_bEditable=enable;

	if (m_bEditable)
	{
		setUserRoleEditable(m_bUserRoleEditable);

		m_ui->checkBoxCreateProcess->setEnabled(true);
		m_ui->checkBoxTerminateProcess->setEnabled(true);
		m_ui->checkBoxUploadFile->setEnabled(true);
		m_ui->checkBoxDownloadFile->setEnabled(true);
		m_ui->checkBoxCommitFile->setEnabled(true);
		m_ui->checkBoxUpdateFile->setEnabled(true);
	}
	else
	{
	 	m_ui->radioButtonGeneralUser->setEnabled(false);
	 	m_ui->radioButtonAdministrator->setEnabled(false);
	 	m_ui->checkBoxCreateProcess->setEnabled(false);
	 	m_ui->checkBoxTerminateProcess->setEnabled(false);
	 	m_ui->checkBoxUploadFile->setEnabled(false);
	 	m_ui->checkBoxDownloadFile->setEnabled(false);
	 	m_ui->checkBoxCommitFile->setEnabled(false);
	 	m_ui->checkBoxUpdateFile->setEnabled(false);
	}
}

void QFrameAuthority::setUserRoleEditable(bool enable)
{
	m_bUserRoleEditable=enable;

	if (m_bEditable && m_bUserRoleEditable)
	{
		m_ui->radioButtonGeneralUser->setEnabled(true);
		m_ui->radioButtonAdministrator->setEnabled(true);
	}
	else
	{
		m_ui->radioButtonGeneralUser->setEnabled(false);
		m_ui->radioButtonAdministrator->setEnabled(false);
	}
}

void QFrameAuthority::setPrivileges(const QMap<int, quint32>& privileges)
{
	Q_ASSERT_NO_NULL(m_ui);

	disconnect(m_ui->radioButtonGeneralUser, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	disconnect(m_ui->radioButtonAdministrator, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	disconnect(m_ui->checkBoxCreateProcess, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	disconnect(m_ui->checkBoxTerminateProcess, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	disconnect(m_ui->checkBoxUploadFile, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	disconnect(m_ui->checkBoxDownloadFile, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	disconnect(m_ui->checkBoxCommitFile, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	disconnect(m_ui->checkBoxUpdateFile, SIGNAL(toggled(bool)), this, SLOT(checkChange()));

	m_privileges=privileges;

	if (checkPrivileges(privileges, PrivilegeRole, AdministratorRole))
	{
		m_ui->radioButtonAdministrator->toggle();
	}
	else
	{
		m_ui->radioButtonGeneralUser->toggle();
	}

	m_ui->checkBoxCreateProcess->setCheckState(
		checkPrivileges(privileges, ProcessPrivilege, CreateProcessPrivilege)? Qt::Checked:Qt::Unchecked);
	m_ui->checkBoxTerminateProcess->setCheckState(
		checkPrivileges(privileges, ProcessPrivilege, TerminateProcessPrivilege)? Qt::Checked:Qt::Unchecked);

	m_ui->checkBoxUploadFile->setCheckState(
		checkPrivileges(privileges, FileTransferPrivilege, UploadFileAndFolderPrivilege)? Qt::Checked:Qt::Unchecked);
	m_ui->checkBoxDownloadFile->setCheckState(
		checkPrivileges(privileges, FileTransferPrivilege, DownloadFileAndFolderPrivilege)? Qt::Checked:Qt::Unchecked);

	m_ui->checkBoxCommitFile->setCheckState(
		checkPrivileges(privileges, FileSynchronizationPrivilege, CommitFilePrivilege)? Qt::Checked:Qt::Unchecked);
	m_ui->checkBoxUpdateFile->setCheckState(
		checkPrivileges(privileges, FileSynchronizationPrivilege, UpdateFilePrivilege)? Qt::Checked:Qt::Unchecked);

	connect(m_ui->radioButtonGeneralUser, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	connect(m_ui->radioButtonAdministrator, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	connect(m_ui->checkBoxCreateProcess, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	connect(m_ui->checkBoxTerminateProcess, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	connect(m_ui->checkBoxUploadFile, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	connect(m_ui->checkBoxDownloadFile, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	connect(m_ui->checkBoxCommitFile, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
	connect(m_ui->checkBoxUpdateFile, SIGNAL(toggled(bool)), this, SLOT(checkChange()));
}

QMap<int, quint32> QFrameAuthority::privileges() const
{
	QMap<int, quint32> priv;

	priv.insert(PrivilegeRole, m_ui->radioButtonGeneralUser->isChecked()? GuestRole:AdministratorRole);
	priv.insert(ProcessPrivilege, 
		(m_ui->checkBoxCreateProcess->isChecked()? CreateProcessPrivilege:0)|(m_ui->checkBoxTerminateProcess->isChecked()? TerminateProcessPrivilege:0));
	priv.insert(FileTransferPrivilege, 
		(m_ui->checkBoxUploadFile->isChecked()? UploadFileAndFolderPrivilege:0)|(m_ui->checkBoxDownloadFile->isChecked()? DownloadFileAndFolderPrivilege:0));
	priv.insert(FileSynchronizationPrivilege, 
		(m_ui->checkBoxCommitFile->isChecked()? CommitFilePrivilege:0)|(m_ui->checkBoxUpdateFile->isChecked()? UpdateFilePrivilege:0));

	return priv;
}

void QFrameAuthority::checkChange()
{
	if (m_ui->radioButtonAdministrator->isChecked() != checkPrivileges(m_privileges, PrivilegeRole, AdministratorRole))
	{
		emit privilegesChanged(true);
		return;
	}

	if (m_ui->checkBoxCreateProcess->isChecked() != checkPrivileges(m_privileges, ProcessPrivilege, CreateProcessPrivilege))
	{
		emit privilegesChanged(true);
		return;
	}

	if (m_ui->checkBoxTerminateProcess->isChecked() != checkPrivileges(m_privileges, ProcessPrivilege, TerminateProcessPrivilege))
	{
		emit privilegesChanged(true);
		return;
	}

	if (m_ui->checkBoxUploadFile->isChecked() != checkPrivileges(m_privileges, FileTransferPrivilege, UploadFileAndFolderPrivilege))
	{
		emit privilegesChanged(true);
		return;
	}

	if (m_ui->checkBoxDownloadFile->isChecked() != checkPrivileges(m_privileges, FileTransferPrivilege, DownloadFileAndFolderPrivilege))
	{
		emit privilegesChanged(true);
		return;
	}

	if (m_ui->checkBoxCommitFile->isChecked() != checkPrivileges(m_privileges, FileSynchronizationPrivilege, CommitFilePrivilege))
	{
		emit privilegesChanged(true);
		return;
	}

	if (m_ui->checkBoxUpdateFile->isChecked() != checkPrivileges(m_privileges, FileSynchronizationPrivilege, UpdateFilePrivilege))
	{
		emit privilegesChanged(true);
		return;
	}

	emit privilegesChanged(false);
}
