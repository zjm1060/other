#ifndef EXPORTEXCELOBJECT_H
#define EXPORTEXCELOBJECT_H

#include <QtCore/qstring.h>
#include <QObject>
#include <QTableView>
#include <QStringList>
#include <QSqlDatabase>
#include <ActiveQt/QAxFactory>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QAxBase>
#include <QAxObject>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlError>

#include <QTableWidget>
#include <QTableView>
#include <QTableWidgetItem>
#include <QDebug>

typedef unsigned int UINT;
#define EXCEL_MODE_READ		0
#define EXCEL_MODE_WRITE	1

class EEO_Field
{
public:
    EEO_Field(const int ic, const QString &sf, const QString &st):
            iCol(ic),sFieldName(sf),sFieldType(st){};

    int     iCol;
    QString sFieldName;
    QString sFieldType;
};

class ExcelObjectODBC : public QObject
{
	Q_OBJECT
public:
	ExcelObjectODBC(const QString &filePath,const QString &sheetName = QString("sheet"));
	~ExcelObjectODBC();
	inline void SetFilePath(const QString &filePath){m_sFilePath = filePath;};
	inline void SetSheetName(const QString &sheetName){m_sSheetName = sheetName;};
	inline const QString &GetLastError(){return m_sLastError;}
	bool Open();
	void AddHeader(const int columun, const QString &fieldName, const QString &fieldType);//filedType char(n) datetime int float ...
	void RemoveAllFields(){while (!fieldList.isEmpty()) delete fieldList.takeFirst();};
	int ExportTableViewToExcel(QTableView *tableView);
	bool ReadRow(const QVariantList &rowValues, long row=0);
	bool ReadColumn(const QVariantList &columnValues, const QString &column, bool auto = true);
	bool ReadColumn(const QVariantList &columnValues, int column);
	bool WriteRow(const QVariantList &rowValues);
	QSqlQueryModel *GetExcelSheetModel();
Q_SIGNALS:
	void exportedRowCount(int row);
public Q_SLOTS:
	void cancel() {m_bIsCancel = true;};
private:
	void GetExcelDriver();// Get the name of the Excel-ODBC driver
	QString m_sFilePath;
	QString m_sSheetName;
	QString m_sExcelDriver;
	QString m_sDsn;
	QString m_sLastError;
	bool m_bIsCancel;
	QSqlDatabase m_Database;
	QSqlQueryModel *excelModel;
	QList<EEO_Field *> fieldList;
};

class ExportExcelObject : public QObject
{
    Q_OBJECT
public:
	ExportExcelObject(){};
    ExportExcelObject(const QString &filepath, const QString &sheettitle,
                      QTableView *tableview):excelFilePath(filepath),
					  sheetName(sheettitle), tableView(tableview){};

    ~ExportExcelObject() {QSqlDatabase::removeDatabase("excelexport");excelObject->dynamicCall("Quit()");};

public:
    void setOutputFilePath(const QString &spath) {excelFilePath = spath;};
    void setOutputSheetTitle(const QString &ssheet) {sheetName = ssheet;};
    void setTableView(QTableView *tableview) {tableView = tableview;};

    void addField(const int iCol, const QString &fieldname, const QString &fieldtype)
         {fieldList << new EEO_Field(iCol, fieldname, fieldtype);};

    void removeAllFields()
         {while (!fieldList.isEmpty()) delete fieldList.takeFirst();};

    int export2Excel();
	int readExcelData();
	int initExcelObject();//如果要设置excel格式，先执行此函数;
	void setColumnWidth(int column,int width);
	void setColumnFormat(int column,const QString &displayFormat = QString::fromLocal8Bit("G/通用格式"));
	void setAlignment(int row = 0,int column = 0,Qt::Alignment flag = Qt::AlignCenter);//row为0时设置整列(column) column为0时设置整行(row) 全为0时设置所有
	void save();
signals:
    void exportedRowCount(int row);

private:
	bool isCancel;
    QString excelFilePath;
    QString sheetName;
    QTableView *tableView;
    QList<EEO_Field *> fieldList;
	QAxObject *excelObject;
	QAxObject *workbook;
	QAxObject *worksheet;
public slots:
	void cancel() {isCancel = TRUE;};
	void resizeRowToContents(int row);
	void resizeRowsToContents();
	void resizeColumnToContents(int column);
	void resizeColumnsToContents();
};

class ExcelObjectCOM : protected QObject
{
public:
	ExcelObjectCOM();
	ExcelObjectCOM(QString xlsFile);
	~ExcelObjectCOM();

public:
	bool Open(UINT nSheet = 1, bool visible = false);//打开xls文件
	bool Open(QString xlsFile, UINT nSheet = 1, bool visible = false);
	void Save();                //保存xls报表
	void Close();               //关闭xls报表

	bool SaveDataFrTable(QTableWidget *tableWidget); //保存数据到xls
	bool ReadDataToTable(QTableWidget *tableWidget); //从xls读取数据到ui

	QVariant GetCellData(UINT row, UINT column);                //获取指定单元数据
	bool     SetCellData(UINT row, UINT column, QVariant data); //修改指定单元数据
	void setColumnWidth(int column,int width);
	void setColumnFormat(int column,const QString &displayFormat = QString::fromLocal8Bit("G/通用格式"));
	void setAlignment(int row = 0,int column = 0,Qt::Alignment flag = Qt::AlignCenter);//row为0时设置整列(column) column为0时设置整行(row) 全为0时设置所有
	void resizeRowToContents(int row);
	void resizeRowsToContents();
	void resizeColumnToContents(int column);
	void resizeColumnsToContents();
	inline UINT GetRowCount()const;
	inline UINT GetColumnCount()const;

	inline bool IsOpen();
	inline bool IsValid();

protected:
	bool CreateXlsFile(const QString &xlsFile);
	void Clear();

private:
	QAxObject *pExcel;      //指向整个excel应用程序
	QAxObject *pWorkbooks;  //指向工作簿集,excel有很多工作簿
	QAxObject *pWorkbook;   //指向sXlsFile对应的工作簿
	QAxObject *pWorksheet;  //指向工作簿中的某个sheet表单

	QString   sXlsFile;     //xls文件路径
	UINT      nCurrSheet;   //当前打开的第几个sheet
	bool      bIsVisible;   //excel是否可见
	int       nRowCount;    //行数
	int       nColumnCount; //列数
	int       nStartRow;    //开始有数据的行下标值
	int       nStartColumn; //开始有数据的列下标值

	bool      bIsOpen;      //是否已打开
	bool      bIsValid;     //是否有效

};

#endif // EXPORTEXCELOBJECT_H
