#include "excelobject.h"
#include "qt_windows.h"
#include <odbcinst.h>

int ExportExcelObject::export2Excel()
{
	isCancel = FALSE;
    if(fieldList.size() <= 0)
    {
        qDebug() << "ExportExcelObject::export2Excel failed: No fields defined.";
        return -1;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "excelexport");
    if(!db.isValid())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: QODBC not supported.";
        return -2;
    }
    // set the dsn string
    QString dsn = QString("DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%1\";DBQ=%2").
                  arg(excelFilePath).arg(excelFilePath);
    db.setDatabaseName(dsn);
    if(!db.open())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: Create Excel file failed by DRIVER={Microsoft Excel Driver (*.xls)}.";
        //QSqlDatabase::removeDatabase("excelexport");
        return -3;
    }
	
    QSqlQuery query(db);

    //drop the table if it's already exists
    QString s, sSql = QString("DROP TABLE [%1] (").arg(sheetName);
    query.exec(sSql);

    //create the table (sheet in Excel file)
    sSql = QString("CREATE TABLE [%1] (").arg(sheetName);
    for (int i = 0; i < fieldList.size(); i++)
    {
        s = QString("[%1] %2").arg(fieldList.at(i)->sFieldName).arg(fieldList.at(i)->sFieldType);
        sSql += s;
        if(i < fieldList.size() - 1)
            sSql += " , ";
    }

    sSql += ")";
    query.prepare(sSql);

    if(!query.exec())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: Create Excel sheet failed.";
        //db.close();
        //QSqlDatabase::removeDatabase("excelexport");
        return -4;
    }

    //add all rows
    sSql = QString("INSERT INTO [%1] (").arg(sheetName);
    for (int i = 0; i < fieldList.size(); i++)
    {
        sSql += fieldList.at(i)->sFieldName;
        if(i < fieldList.size() - 1)
            sSql += " , ";
    }
    sSql += ") VALUES (";
    for (int i = 0; i < fieldList.size(); i++)
    {
        sSql += QString(":data%1").arg(i);
        if(i < fieldList.size() - 1)
            sSql += " , ";
    }
    sSql += ")";

    qDebug() << sSql;

    int r, iRet = 0;
    for(r = 0 ; r < tableView->model()->rowCount() ; r++)
    {
        query.prepare(sSql);
        for (int c = 0; c < fieldList.size(); c++)
        {
			query.bindValue(QString(":data%1").arg(c),tableView->model()->data(tableView->model()->index(r, fieldList.at(c)->iCol)));
        }

        if(query.exec())
            iRet++;

        if(r % 10 == 0)
            emit exportedRowCount(r);

		if(isCancel)
			break;
    }

    emit exportedRowCount(r);
    return iRet;
}
int ExportExcelObject::readExcelData()
{
// 	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "excel");
// 	if(!db.isValid())
// 	{
// 		qDebug() << "ExportExcelObject::export2Excel failed: QODBC not supported.";
// 		return -2;
// 	}
// 	// set the dsn string
// 	QString dsn = QString("DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=TRUE;DBQ=%2").
// 		arg(excelFilePath).arg(excelFilePath);
// 	db.setDatabaseName(dsn);
// 	if(!db.open())
// 	{
// 		qDebug() << "ExportExcelObject::export2Excel failed: Create Excel file failed by DRIVER={Microsoft Excel Driver (*.xls)}.";
// 		//QSqlDatabase::removeDatabase("excelexport");
// 		return -3;
// 	}
// 
// 	QSqlQuery query(db);
// 
// 	QString sql_str = QString::fromLocal8Bit("select * from %1").arg(QString::fromLocal8Bit(sheetName));
// 
// 	if(!query.exec(sql_str))
// 	{
// 		qDebug() << "ExportExcelObject::export2Excel failed: Create Excel sheet failed.";
// 		//db.close();
// 		//QSqlDatabase::removeDatabase("excelexport");
// 		return -4;
// 	}
// 	int col = query.record().count();
	return TRUE;
}
int ExportExcelObject::initExcelObject()
{
	QSqlDatabase::removeDatabase("excelexport");
	excelObject = new QAxObject( "Excel.Application", this );
	if (!excelObject)
	{
		qDebug() << "初始化Excel错误，可能没有安装office组件!";
		return -1;
	}
	QAxObject *workbooks = excelObject->querySubObject( "Workbooks" ); //得到Workbooks集合的指针 
	if (!workbooks)
	{
		qDebug() << "初始化Excel错误，可能没有安装office组件!";
		return -2;
	}
	workbook = workbooks->querySubObject( "Open(const QString&)", excelFilePath); //打开硬盘上的一个Excel文档 
	if (!workbook)
	{
		qDebug() << "Excel文件打开错误!";
		return -3;
	}
	QAxObject *worksheets = workbook->querySubObject("worksheets");
	worksheet = workbook->querySubObject("worksheets(const QString&)",sheetName);
	return TRUE;
}
void ExportExcelObject::setColumnWidth(int column,int width)
{
	QAxObject *columnObject = worksheet->querySubObject("columns(int)",column);
	columnObject->setProperty("ColumnWidth",QVariant(width));//设置第一列列宽为5
}
void ExportExcelObject::setColumnFormat(int column,const QString &displayFormat)
{
	QAxObject *columnObject = worksheet->querySubObject("columns(int)",column);
	columnObject->setProperty("NumberFormatLocal",QVariant(displayFormat));
}
void ExportExcelObject::setAlignment(int row,int column,Qt::Alignment flag)
{
	if (row < 0 || column < 0)
		return;
	QAxObject* rangeObject;
	if (row == 0 && column != 0)
	{
		rangeObject = worksheet->querySubObject("columns(int)",column);
	}
	else if (row != 0 && column == 0)
	{
		rangeObject = worksheet->querySubObject("rows(int)",row);
	}
	else if (row == 0 && column == 0)
	{
		rangeObject = worksheet->querySubObject("Cells()");
	}
	else
	{
		rangeObject = worksheet->querySubObject("Cells( int, int )",row,column);
	}
	switch(flag)
	{
		case Qt::AlignLeft:
			rangeObject->setProperty("HorizontalAlignment",QVariant(-4131));
			break;
		case Qt::AlignRight:
			rangeObject->setProperty("HorizontalAlignment",QVariant(-4152));
			break;
		case Qt::AlignHCenter:
			rangeObject->setProperty("HorizontalAlignment",QVariant(-4108));
			break;
		case Qt::AlignTop:
			rangeObject->setProperty("VerticalAlignment",QVariant(-4160));
			break;
		case Qt::AlignBottom:
			rangeObject->setProperty("VerticalAlignment",QVariant(-4107));
			break;
		case Qt::AlignVCenter:
			rangeObject->setProperty("VerticalAlignment",QVariant(-4108));
			break;
		case Qt::AlignCenter:
			rangeObject->setProperty("HorizontalAlignment",QVariant(-4108));
			rangeObject->setProperty("VerticalAlignment",QVariant(-4108));
			break;
		default:
			break;
	}
}
void ExportExcelObject::resizeRowToContents(int row)
{
	QAxObject *rowObject = worksheet->querySubObject("rows(int)",row);
	rowObject->dynamicCall("AutoFit()");
}
void ExportExcelObject::resizeRowsToContents()
{
	QAxObject *rowsObject = worksheet->querySubObject("rows()");
	rowsObject->dynamicCall("AutoFit()");
}
void ExportExcelObject::resizeColumnToContents(int column)
{
	QAxObject *columnObject = worksheet->querySubObject("columns(int)",column);
	columnObject->dynamicCall("AutoFit()");
}
void ExportExcelObject::resizeColumnsToContents()
{
	QAxObject *columnsObject = worksheet->querySubObject("columns()");
	columnsObject->dynamicCall("AutoFit()");
}
void ExportExcelObject::save()
{
	workbook->querySubObject("Save()");
	excelObject->dynamicCall("Quit()");
}

/**************************************************************************************************************************************************/
ExcelObjectCOM::ExcelObjectCOM()
{
    pExcel     = NULL;
    pWorkbooks = NULL;
    pWorkbook  = NULL;
    pWorksheet = NULL;

    sXlsFile     = "";
    nRowCount    = 0;
    nColumnCount = 0;
    nStartRow    = 0;
    nStartColumn = 0;

    bIsOpen  = false;
    bIsValid = false;

    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
    }
}

ExcelObjectCOM::ExcelObjectCOM(QString xlsFile)
{
    pExcel     = NULL;
    pWorkbooks = NULL;
    pWorkbook  = NULL;
    pWorksheet = NULL;

    sXlsFile     = xlsFile;
    nRowCount    = 0;
    nColumnCount = 0;
    nStartRow    = 0;
    nStartColumn = 0;

    bIsOpen  = false;
    bIsValid = false;

    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
    }
}

ExcelObjectCOM::~ExcelObjectCOM()
{
    if ( bIsOpen )
    {
        //析构前，先保存数据，然后关闭workbook
        Close();
    }
    OleUninitialize();
}

/**
  *@brief 打开sXlsFile指定的excel报表
  *@return true : 打开成功
  *        false: 打开失败
  */
bool ExcelObjectCOM::Open(UINT nSheet, bool visible)
{

    if ( bIsOpen )
    {
        //return bIsOpen;
        Close();
    }

    if ( NULL == pExcel )
    {
        pExcel = new QAxObject("Excel.Application");
        if ( pExcel )
        {
            bIsValid = true;
        }
        else
        {
            bIsValid = false;
            bIsOpen  = false;
            return bIsOpen;
        }
		bIsVisible = visible;
        pExcel->dynamicCall("SetVisible(bool)", bIsVisible);
    }

    if ( !bIsValid )
    {
        bIsOpen  = false;
        return bIsOpen;
    }

    if ( sXlsFile.isEmpty() )
    {
        bIsOpen  = false;
        return bIsOpen;
    }


    bool ok = CreateXlsFile(sXlsFile);
    if ( !ok )
    {
        bIsOpen  = false;
        return bIsOpen;
    }

    nCurrSheet = nSheet;

    pWorkbooks = pExcel->querySubObject("WorkBooks"); //获取工作簿
    pWorkbook = pWorkbooks->querySubObject("Open(QString, QVariant)",sXlsFile,QVariant(0)); //打开xls对应的工作簿
    pWorksheet = pWorkbook->querySubObject("WorkSheets(int)", nCurrSheet);//打开第一个sheet

    //至此已打开，开始获取相应属性
    QAxObject *usedrange = pWorksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject *rows = usedrange->querySubObject("Rows");
    QAxObject *columns = usedrange->querySubObject("Columns");

    //因为excel可以从任意行列填数据而不一定是从0,0开始，因此要获取首行列下标
    nStartRow    = usedrange->property("Row").toInt();    //第一行的起始位置
    nStartColumn = usedrange->property("Column").toInt(); //第一列的起始位置

    nRowCount    = rows->property("Count").toInt();       //获取行数
    nColumnCount = columns->property("Count").toInt();    //获取列数

    bIsOpen  = true;
    return bIsOpen;
}

/**
  *@brief Open()的重载函数
  */
bool ExcelObjectCOM::Open(QString xlsFile, UINT nSheet, bool visible)
{
    sXlsFile = xlsFile;
    nCurrSheet = nSheet;
    bIsVisible = visible;

    return Open(nCurrSheet,bIsVisible);
}

/**
  *@brief 保存表格数据，把数据写入文件
  */
void ExcelObjectCOM::Save()
{
    if ( pWorkbook )
    {
        pWorkbook->dynamicCall("Save()");
    }
}

/**
  *@brief 关闭前先保存数据，然后关闭当前Excel COM对象，并释放内存
  */
void ExcelObjectCOM::Close()
{
    //关闭前先保存数据
    Save();
    if ( pExcel && pWorkbook )
    {
        pWorkbook->dynamicCall("Close(bool)", true);
        pExcel->dynamicCall("Quit()");

        delete pExcel;
        pExcel = NULL;

        bIsOpen = false;
        bIsValid = false;
    }
}

/**
  *@brief 把tableWidget中的数据保存到excel中
  *@param tableWidget : 指向GUI中的tablewidget指针
  *@return 保存成功与否 true : 成功
  *                  false: 失败
  */
bool ExcelObjectCOM::SaveDataFrTable(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }
    if ( !bIsOpen )
    {
        return false;
    }

    int tableR = tableWidget->rowCount();
    int tableC = tableWidget->columnCount();

    //获取表头写做第一行
    for (int i=0; i<tableC; i++)
    {
        if ( tableWidget->horizontalHeaderItem(i) != NULL )
        {
            this->SetCellData(1,i+1,tableWidget->horizontalHeaderItem(i)->text());
        }
    }

    //写数据
    for (int i=0; i<tableR; i++)
    {
        for (int j=0; j<tableC; j++)
        {
            if ( tableWidget->item(i,j) != NULL )
            {
                this->SetCellData(i+2,j+1,tableWidget->item(i,j)->text());
            }
        }
    }

    //保存
    Save();

    return true;
}

/**
  *@brief 从指定的xls文件中把数据导入到tableWidget中
  *@param tableWidget : 执行要导入到的tablewidget指针
  *@return 导入成功与否 true : 成功
  *                   false: 失败
  */
bool ExcelObjectCOM::ReadDataToTable(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }

    //先把table的内容清空
    int tableColumn = tableWidget->columnCount();
    tableWidget->clear();
    for (int n=0; n<tableColumn; n++)
    {
        tableWidget->removeColumn(0);
    }

    int rowcnt    = nStartRow + nRowCount;
    int columncnt = nStartColumn + nColumnCount;

    //获取excel中的第一行数据作为表头
    QStringList headerList;
    for (int n = nStartColumn; n<columncnt; n++ )
    {
        QAxObject * cell = pWorksheet->querySubObject("Cells(int,int)",nStartRow, n);
        if ( cell )
        {
            headerList<<cell->dynamicCall("Value2()").toString();
        }
    }

    //重新创建表头
    tableWidget->setColumnCount(nColumnCount);
    tableWidget->setHorizontalHeaderLabels(headerList);


    //插入新数据
    for (int i = nStartRow+1, r = 0; i < rowcnt; i++, r++ )  //行
    {
        tableWidget->insertRow(r); //插入新行
        for (int j = nStartColumn, c = 0; j < columncnt; j++, c++ )  //列
        {
            QAxObject * cell = pWorksheet->querySubObject("Cells(int,int)", i, j );//获取单元格

            //在r新行中添加子项数据
            if ( cell )
            {
                tableWidget->setItem(r,c,new QTableWidgetItem(cell->dynamicCall("Value2()").toString()));
            }
        }
    }

    return true;
}

/**
  *@brief 获取指定单元格的数据
  *@param row : 单元格的行号
  *@param column : 单元格的列号
  *@return [row,column]单元格对应的数据
  */
QVariant ExcelObjectCOM::GetCellData(UINT row, UINT column)
{
    QVariant data;

    QAxObject *cell = pWorksheet->querySubObject("Cells(int,int)",row,column);//获取单元格对象
    if ( cell )
    {
        data = cell->dynamicCall("Value2()");
    }

    return data;
}

/**
  *@brief 修改指定单元格的数据
  *@param row : 单元格的行号
  *@param column : 单元格指定的列号
  *@param data : 单元格要修改为的新数据
  *@return 修改是否成功 true : 成功
  *                   false: 失败
  */
bool ExcelObjectCOM::SetCellData(UINT row, UINT column, QVariant data)
{
    bool op = false;

    QAxObject *cell = pWorksheet->querySubObject("Cells(int,int)",row,column);//获取单元格对象
    if ( cell )
    {
        QString strData = data.toString(); //excel 居然只能插入字符串和整型，浮点型无法插入
        cell->dynamicCall("SetValue(const QVariant&)",strData); //修改单元格的数据
        op = true;
    }
    else
    {
        op = false;
    }

    return op;
}

/**
  *@brief 创建一个xls报表文件
  *@param xlsFile : 报表文件名
  *@return true : 创建成功
  *        false: 创建失败
  */
bool ExcelObjectCOM::CreateXlsFile(const QString &xlsFile)
{

    bool cr = false;
    QFile f(xlsFile);
    if ( !f.exists())
    {
        /**
          *@note 由于无法找到workbook增加一个工作簿的接口，只能用这种拷贝文件的方法
          *或者，直接创建一个.xls为后缀的空白文档
          */
        QFile copyFile(tr("./template/template.xls"));
        if ( copyFile.exists() )
        {
            //直接拷贝一个模块
            QFile::copy(tr("./template/template.xls"),xlsFile);
        }
        else
        {
            //创建已xls为后缀的空白文档
            f.open(QIODevice::WriteOnly);
            f.close();
        }

        //判断是否创建成功
        cr = f.open(QIODevice::ReadWrite);
        f.close();

    }
    else
    {
        cr = true;
    }

    return cr;
}
void ExcelObjectCOM::setColumnWidth(int column,int width)
{
	QAxObject *columnObject = pWorksheet->querySubObject("columns(int)",column);
	columnObject->setProperty("ColumnWidth",QVariant(width));//设置第一列列宽为5
}
void ExcelObjectCOM::setColumnFormat(int column,const QString &displayFormat)
{
	QAxObject *columnObject = pWorksheet->querySubObject("columns(int)",column);
	columnObject->setProperty("NumberFormatLocal",QVariant(displayFormat));
}
void ExcelObjectCOM::setAlignment(int row,int column,Qt::Alignment flag)
{
	if (row < 0 || column < 0)
		return;
	QAxObject* rangeObject;
	if (row == 0 && column != 0)
	{
		rangeObject = pWorksheet->querySubObject("columns(int)",column);
	}
	else if (row != 0 && column == 0)
	{
		rangeObject = pWorksheet->querySubObject("rows(int)",row);
	}
	else if (row == 0 && column == 0)
	{
		rangeObject = pWorksheet->querySubObject("Cells()");
	}
	else
	{
		rangeObject = pWorksheet->querySubObject("Cells( int, int )",row,column);
	}
	switch(flag)
	{
		case Qt::AlignLeft:
			rangeObject->setProperty("HorizontalAlignment",QVariant(-4131));
			break;
		case Qt::AlignRight:
			rangeObject->setProperty("HorizontalAlignment",QVariant(-4152));
			break;
		case Qt::AlignHCenter:
			rangeObject->setProperty("HorizontalAlignment",QVariant(-4108));
			break;
		case Qt::AlignTop:
			rangeObject->setProperty("VerticalAlignment",QVariant(-4160));
			break;
		case Qt::AlignBottom:
			rangeObject->setProperty("VerticalAlignment",QVariant(-4107));
			break;
		case Qt::AlignVCenter:
			rangeObject->setProperty("VerticalAlignment",QVariant(-4108));
			break;
		case Qt::AlignCenter:
			rangeObject->setProperty("HorizontalAlignment",QVariant(-4108));
			rangeObject->setProperty("VerticalAlignment",QVariant(-4108));
			break;
		default:
			break;
	}
}
void ExcelObjectCOM::resizeRowToContents(int row)
{
	QAxObject *rowObject = pWorksheet->querySubObject("rows(int)",row);
	rowObject->dynamicCall("AutoFit()");
}
void ExcelObjectCOM::resizeRowsToContents()
{
	QAxObject *rowsObject = pWorksheet->querySubObject("rows()");
	rowsObject->dynamicCall("AutoFit()");
}
void ExcelObjectCOM::resizeColumnToContents(int column)
{
	QAxObject *columnObject = pWorksheet->querySubObject("columns(int)",column);
	columnObject->dynamicCall("AutoFit()");
}
void ExcelObjectCOM::resizeColumnsToContents()
{
	QAxObject *columnsObject = pWorksheet->querySubObject("columns()");
	columnsObject->dynamicCall("AutoFit()");
}
/**
  *@brief 清空除报表之外的数据
  */
void ExcelObjectCOM::Clear()
{
    sXlsFile     = "";
    nRowCount    = 0;
    nColumnCount = 0;
    nStartRow    = 0;
    nStartColumn = 0;
}

/**
  *@brief 判断excel是否已被打开
  *@return true : 已打开
  *        false: 未打开
  */
bool ExcelObjectCOM::IsOpen()
{
    return bIsOpen;
}

/**
  *@brief 判断excel COM对象是否调用成功，excel是否可用
  *@return true : 可用
  *        false: 不可用
  */
bool ExcelObjectCOM::IsValid()
{
    return bIsValid;
}

/**
  *@brief 获取excel的行数
  */
UINT ExcelObjectCOM::GetRowCount()const
{
    return nRowCount;
}

/**
  *@brief 获取excel的列数
  */
UINT ExcelObjectCOM::GetColumnCount()const
{
    return nColumnCount;
}
/*******************************************************************************************************************************************************/
ExcelObjectODBC::ExcelObjectODBC(const QString &filePath,const QString &sheetName)
{
	m_sFilePath = filePath;
	m_sSheetName = sheetName;
	excelModel = new QSqlQueryModel;
	m_Database = QSqlDatabase::addDatabase("QODBC", "excel");
	GetExcelDriver();
	//set the dsn string
 	m_sDsn = QString("DRIVER={%1};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%2\";DBQ=%3").
 		arg(m_sExcelDriver).arg(m_sFilePath).arg(m_sFilePath);
}
ExcelObjectODBC::~ExcelObjectODBC()
{
	QSqlDatabase::removeDatabase("excel");
}
void ExcelObjectODBC::AddHeader(const int columun, const QString &fieldName, const QString &fieldType)
{
	fieldList << new EEO_Field(columun, fieldName, fieldType);
}
int ExcelObjectODBC::ExportTableViewToExcel(QTableView *tableView)
{
	m_bIsCancel = false;
	if(fieldList.size() <= 0)
	{
		m_sLastError = QString("ExcelObjectODBC::ExportTableViewToExcel() No fields defined.");
		return FALSE;
	}
	QSqlQuery query(m_Database);

	//drop the table if it's already exists
	QString s, sSql = QString("DROP TABLE [%1] (").arg(m_sSheetName);
	query.exec(sSql);

	//create the table (sheet in Excel file)
	sSql = QString("CREATE TABLE [%1] (").arg(m_sSheetName);
	for (int i = 0; i < fieldList.size(); i++)
	{
		s = QString("[%1] %2").arg(fieldList.at(i)->sFieldName).arg(fieldList.at(i)->sFieldType);
		sSql += s;
		if(i < fieldList.size() - 1)
			sSql += " , ";
	}

	sSql += ")";
	query.prepare(sSql);

	if(!query.exec())
	{
		m_sLastError = QString("ExcelObjectODBC::ExportTableViewToExcel() Create Excel sheet failed.");
		return FALSE;
	}

	//add all rows
	sSql = QString("INSERT INTO [%1] (").arg(m_sSheetName);
	for (int i = 0; i < fieldList.size(); i++)
	{
		sSql += fieldList.at(i)->sFieldName;
		if(i < fieldList.size() - 1)
			sSql += " , ";
	}
	sSql += ") VALUES (";
	for (int i = 0; i < fieldList.size(); i++)
	{
		sSql += QString(":data%1").arg(i);
		if(i < fieldList.size() - 1)
			sSql += " , ";
	}
	sSql += ")";

	qDebug() << sSql;

	int r, iRet = 0;
	for(r = 0 ; r < tableView->model()->rowCount() ; r++)
	{
		query.prepare(sSql);
		for (int c = 0; c < fieldList.size(); c++)
		{
			query.bindValue(QString(":data%1").arg(c),tableView->model()->data(tableView->model()->index(r, fieldList.at(c)->iCol)));
		}

		if(query.exec())
			iRet++;

		if(r % 10 == 0)
			emit exportedRowCount(r);

		if(m_bIsCancel)
			break;
	}

	emit exportedRowCount(r);
	return iRet;
}
bool ExcelObjectODBC::ReadRow(const QVariantList &rowValues, long row)
{
	return true;
}
bool ExcelObjectODBC::ReadColumn(const QVariantList &columnValues, const QString &column, bool auto)
{
	return true;
}
bool ExcelObjectODBC::ReadColumn(const QVariantList &columnValues, int column)
{
	return true;
}
bool ExcelObjectODBC::WriteRow(const QVariantList &rowValues)
{
	if (rowValues.size() != fieldList.size())
	{
		m_sLastError = QString("ExcelObjectODBC::input field num error!");
		return false;
	}
	QSqlQuery query(m_Database);
	QString sSql;
	sSql = QString("INSERT INTO [%1] (").arg(m_sSheetName);
	for (int i = 0; i < fieldList.size(); i++)
	{
		sSql += fieldList.at(i)->sFieldName;
		if(i < fieldList.size() - 1)
			sSql += " , ";
	}
	sSql += ") VALUES (";
	for (int i = 0; i < fieldList.size(); i++)
	{
		sSql += QString("\'%1\'").arg(rowValues.at(fieldList.at(i)->iCol).toString());
		if(i < fieldList.size() - 1)
			sSql += " , ";
	}
	sSql += ")";

	qDebug() << sSql;
	if (!query.exec(sSql))
	{
		m_sLastError = query.lastError().text();
		return false;
	}
	return true;
}
void ExcelObjectODBC::GetExcelDriver()
{
	wchar_t szBuf[2001];
	wchar_t excl[] = L"Excel";
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	wchar_t *pszBuf = szBuf;
	// 获取已安装驱动的名称(函数在odbcinst.h里)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
	{
		m_sExcelDriver = "";
		m_sLastError = QString("ExcelObjectODBC::Get the names of the installed drivers error");

		return;
	}
	// 检索已安装的驱动是否有Excel...
	do
	{
		if(wcsstr(pszBuf, excl) != 0)
		{
			//发现 !
			m_sExcelDriver = QString::fromWCharArray(pszBuf);
			break;
		}
		wchar_t ze = {'\0'};
		pszBuf = wcschr(pszBuf, ze) + 1;
	}
	while (pszBuf[1] != '\0');
}
bool ExcelObjectODBC::Open()
{
	if(!m_Database.isValid())
	{
		qDebug() << "ExcelObjectODBC::open failed: QODBC not supported.";
		m_sLastError = QString("ExcelObjectODBC::open failed: QODBC not supported.");
		return false;
	}
	m_Database.setDatabaseName(m_sDsn);
	if(!m_Database.open())
	{
		qDebug() << "ExcelObjectODBC::open failed: Create Excel file failed by DRIVER={Microsoft Excel Driver (*.xls)}.";
		m_sLastError = QString("ExcelObjectODBC::open failed: Create Excel file failed by DRIVER={Microsoft Excel Driver (*.xls)}.");
		return false;
	}
	if (fieldList.size()!=0)
	{
		QSqlQuery query(m_Database);

		//drop the table if it's already exists
		QString s, sSql = QString("DROP TABLE [%1] (").arg(m_sSheetName);
		query.exec(sSql);

		//create the table (sheet in Excel file)
		sSql = QString("CREATE TABLE [%1] (").arg(m_sSheetName);
		for (int i = 0; i < fieldList.size(); i++)
		{
			s = QString("[%1] %2").arg(fieldList.at(i)->sFieldName).arg(fieldList.at(i)->sFieldType);
			sSql += s;
			if(i < fieldList.size() - 1)
				sSql += " , ";
		}

		sSql += ")";
		query.prepare(sSql);

		if(!query.exec())
		{
			qDebug() << "ExcelObjectODBC::Create Excel sheet failed.";
			m_sLastError = QString("ExcelObjectODBC::Create Excel sheet failed.");
			return false;
		}
	}
	return true;
}
QSqlQueryModel *ExcelObjectODBC::GetExcelSheetModel()
{
	excelModel->clear();
	excelModel->setQuery(QString("SELECT * from [%1$A1:IV65536]").arg(m_sSheetName),m_Database);
	return excelModel;
}