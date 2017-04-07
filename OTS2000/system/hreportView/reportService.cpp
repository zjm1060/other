#include <QtGui>

#include "ServiceSoap.nsmap"
#include "reportService.h"

ReportService::ReportService()
{
	soap= new ServiceSoap;

	//	接收的字节流为UTF8格式，必须设置SOAP的字符模式，才能正确接收
	soap_set_mode(soap->soap, SOAP_C_UTFSTRING);
}


bool ReportService::GetTemplate(int reportId, QBuffer *content)
{
	window->statusBar()->showMessage(window->tr("Start to get report template..."));

	_ns1__GetTemplate ns1__GetTemplate;
	_ns1__GetTemplateResponse ns1__GetTemplateResponse;

	ns1__GetTemplate.templateId= reportId;

	if (soap->__ns1__GetTemplate(&ns1__GetTemplate,  &ns1__GetTemplateResponse) == SOAP_OK
		&& ns1__GetTemplateResponse.GetTemplateResult != NULL
		&& ns1__GetTemplateResponse.GetTemplateResult->__any != NULL)
	{
		window->statusBar()->showMessage(window->tr("Success to get report template"));

		const char *recv= ns1__GetTemplateResponse.GetTemplateResult->__any;
		
//		SVG内容必须采用UTF8格式保存，而接收的字节流本身即为UTF8格式，故直接将字节流交给显示函数
		content->setData(recv, strlen(recv));
		
/*
		QFile file("Template.svg");
		file.open(QIODevice::ReadWrite|QIODevice::Truncate);
		file.resize(0);
		
		char file_header[3]= {0xEF, 0xBB, 0xBF};		
		file.write(file_header, 3);
		while(*recv)
		{
			file.write(recv, 1);
			if(*recv=='>')
				file.write("\r\n", 2);
			recv++;
		}
*/

//		QTextStream stream( &file );
//		此函数设置stream输入输出的编码格式
//		stream.setCodec("ISO 8859-1");
//		stream << recv;
		
//		此函数设置QString使用的CString源字符串的编码格式
//		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
//		QString content(recv);
//		stream << content;
		
//		file.close();
		return true;
	}
	else
	{
		window->statusBar()->showMessage(window->tr("Fail to get report template"));
		return false;
	}
}



bool ReportService::GetReportTree(QBuffer *content)
{
	window->statusBar()->showMessage(window->tr("Start to get report tree..."));

	_ns1__GetTemplateTree ns1__GetTemplateTree;
	_ns1__GetTemplateTreeResponse ns1__GetTemplateTreeResponse;

	if (soap->__ns1__GetTemplateTree(&ns1__GetTemplateTree,  &ns1__GetTemplateTreeResponse) == SOAP_OK
		&& ns1__GetTemplateTreeResponse.GetTemplateTreeResult != NULL
		&& ns1__GetTemplateTreeResponse.GetTemplateTreeResult->__any != NULL)
	{
		window->statusBar()->showMessage(window->tr("Success to get report tree"));

		const char *recv= ns1__GetTemplateTreeResponse.GetTemplateTreeResult->__any;
		content->setData(recv, strlen(recv));
		return true;
		 
	}
	else
	{
		window->statusBar()->showMessage(window->tr("Fail to get report tree"));
		return false;
	}
	
}


bool ReportService::GetTemplateInformation(QBuffer *content)
{
	window->statusBar()->showMessage(window->tr("Start to get template info..."));
	
	_ns1__GetTemplateInformation ns1__GetTemplateInformation;
	_ns1__GetTemplateInformationResponse ns1__GetTemplateInformationResponse;
	
	if (soap->__ns1__GetTemplateInformation(&ns1__GetTemplateInformation,  &ns1__GetTemplateInformationResponse) == SOAP_OK
		&& ns1__GetTemplateInformationResponse.GetTemplateInformationResult != NULL
		&& ns1__GetTemplateInformationResponse.GetTemplateInformationResult->__any != NULL)
	{
		window->statusBar()->showMessage(window->tr("Success to get template info"));
		
		const char *recv= ns1__GetTemplateInformationResponse.GetTemplateInformationResult->__any;
		content->setData(recv, strlen(recv));
		return true;
		
	}
	else
	{
		window->statusBar()->showMessage(window->tr("Fail to get template info"));
		return false;
	}
	
}



bool ReportService::GetTemplateConfig(int reportId, QBuffer *content)
{
	window->statusBar()->showMessage(window->tr("Start to get template config..."));

	_ns1__GetTemplateConfig ns1__GetTemplateConfig;
	_ns1__GetTemplateConfigResponse ns1__GetTemplateConfigResponse;

	ns1__GetTemplateConfig.templateId= reportId;
	
	if (soap->__ns1__GetTemplateConfig(&ns1__GetTemplateConfig,  &ns1__GetTemplateConfigResponse) == SOAP_OK
		&& ns1__GetTemplateConfigResponse.GetTemplateConfigResult != NULL
		&& ns1__GetTemplateConfigResponse.GetTemplateConfigResult->__any != NULL)
	{
		window->statusBar()->showMessage(window->tr("Success to get template config"));

		const char *recv= ns1__GetTemplateConfigResponse.GetTemplateConfigResult->__any;
		
		content->setData(recv, strlen(recv));
		return true;
	}
	else
	{
		window->statusBar()->showMessage(window->tr("Fail to get template config"));
		return false;
	}
	
}

bool ReportService::queryReport(int reportId, bool useSavedReport, 
								QString startTime, QString endTime, QString inputCells, QBuffer *content)
{
	window->statusBar()->showMessage(window->tr("Start to get report data..."));

	_ns1__Query ns1__Query;
	_ns1__QueryResponse ns1__QueryResponse;

	ns1__Query.templateId= reportId;
	ns1__Query.useSavedReport= useSavedReport;
	QByteArray start= startTime.toAscii();
	ns1__Query.dateStartString= start.data();
	QByteArray end= endTime.toAscii();
	ns1__Query.dateEndString= end.data();
	QByteArray input= inputCells.toAscii();
	ns1__Query.inputCells = input.data();
	
	if (soap->__ns1__Query(&ns1__Query,  &ns1__QueryResponse) == SOAP_OK
		&& ns1__QueryResponse.QueryResult != NULL
		&& ns1__QueryResponse.QueryResult->__any != NULL)
	{
		window->statusBar()->showMessage(window->tr("Success to get report data"));

		const char *recv= ns1__QueryResponse.QueryResult->__any;
		
//		SVG内容必须采用UTF8格式保存，而接收的字节流本身即为UTF8格式，故直接将字节流交给显示函数
		content->setData(recv, strlen(recv));
			
		return true;
	}
	else
	{
		window->statusBar()->showMessage(window->tr("Fail to get report data"));
		return false;
	}
	
}


