#include "qcmddeffilecompare.h"

QCmdDefFileCompare::QCmdDefFileCompare(QObject *parent)
	: QMsgDefFileCompare(parent)
{
	
}

QCmdDefFileCompare::~QCmdDefFileCompare()
{

}

bool QCmdDefFileCompare::match(const QString& filename) const
{
	if (filename.endsWith(CMD_MSG_FILE))
	{
		return true;
	}

	return false;
}

void QCmdDefFileCompare::parseFile(QFile& file, FileCompareResult& result)
{
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		result.errcode=FileOpenError;
		return;
	}

	QTextStream stream(&file);
	QVector<CMD_DEF_ENTRY> msgs=parseCmdDefFile(stream);
	result.data=qVariantFromValue(msgs);

	result.titles<<tr("Command Type")<<tr("Group Name")<<tr("Status")<<tr("Meaning");
}

QVector<QCmdDefFileCompare::CMD_DEF_ENTRY> QCmdDefFileCompare::parseCmdDefFile(QTextStream& stream)
{
	QVector<CMD_DEF_ENTRY> cmds;

	int nNumType=-1;
	int nTypeValid=0;
	int nNumGroup=-1;
	int nGroupValid=0;
	while (!stream.atEnd())
	{
		QString strText=stream.readLine();

		QVector<QString> labels=parseString(strText);
		if (labels.isEmpty())
		{
			continue;
		}

		if (labels[0] == "END")
		{
			break;
		}

		if (labels.size() == 3 && labels[0] == "CMDTYPE" && labels[1] == "NAME")
		{
			nTypeValid=1;

			for(int i=0; i<cmds.size(); i++)
			{
				if(cmds[i].typeName == labels[2])
				{
					nTypeValid=0;
					break;
				}
			}

			if (nTypeValid == 1)
			{
				nNumType++;

				nGroupValid=0;
				nNumGroup=-1;

				cmds.append(CMD_DEF_ENTRY());
				cmds[nNumType].typeName=labels[2];
			}

			continue;
		}

		if (nTypeValid == 0)
		{
			continue;
		}

		if (labels.size() == 3 && labels[0] == "GROUP" && labels[1] == "NAME")
		{
			nGroupValid=1;

			for(int i=0; i<cmds[nNumType].groups.size(); i++)
			{
				if(cmds[nNumType].groups[i].grpName == labels[2])
				{
					nGroupValid=0;
					break;
				}
			}

			if (nGroupValid == 1)
			{
				nNumGroup++;

				cmds[nNumType].groups.append(CMD_GROUP_DEF_ENTRY());
				cmds[nNumType].groups[nNumGroup].grpName=labels[2];
			}

			continue;
		}

		if (nGroupValid == 0)
		{
			continue;
		}

		if (labels.size() >= 2)
		{
			if (labels[0] ==  "STATE")
			{
				CMD_STATE_DEF_ENTRY state;
				bool bOk;
				state.index=labels[1].toInt(&bOk);
				if (bOk)
				{
					if (labels.size() == 3)
					{
						state.def=labels[2];
					}

					cmds[nNumType].groups[nNumGroup].states<<state;
				}

				continue;
			}
		}
	}

	return cmds;
}

REGISTER_FC_INSTANCE(QCmdDefFileCompare) 
