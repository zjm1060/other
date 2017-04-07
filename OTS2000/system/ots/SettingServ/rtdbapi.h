#ifndef _RTDBAPI_H
#define _RTDBAPI_H
#undef AFX_DATA
#define AFX_DATA AFX_EXT_DATA
// <body of your header file>



/********************************************************
//    读取表属性
///
*********************************************************/
// 读取当前有效记录数
//szDbName:数据库名称；szTabName：表名；
//返回有效记录数
extern "C"
{
	int GetRecNum(LPCSTR szDbName,LPCSTR szTabName);
}
// 读取表的最大维数
//szDbName:数据库名称；szTabName：表名；
//返回表维数
extern "C"
{
	int GetTabDimension(LPCSTR szDbName,LPCSTR szTabName);
}
//读取表的字段数
//szDbName:数据库名称；szTabName：表名；
//返回表字段数
extern "C"
{
	int GetTabFldNum(LPCSTR szDbName,LPCSTR szTabName);
}
//读取表的字段名称
//nFldNum:字段号，从1开始; szDbName:数据库名称；szTabName：表名；
//szFldName:返回的字段名称
//返回函数是否成功执行
extern "C"
{
	BOOL GetTabFldName(int nFldNum,LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName);
}
//读取表的字段数据类型
//szDbName:数据库名称；szTabName：表名；
//szFldName:字段名称,cDataType:返回的字段类型
//char:c,short:s,int:i,long:l,float:f,double:d,
//unsigned char:C,unsigned short:S,unsigned int:I,unsigned long:L
//返回字段类型的字节长度
extern "C"
{
	int GetTabFldType(LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char& cDataType);
}

/********************************************************
//    读取记录号
///
*********************************************************/
//按关键字读取记录号(要求关键字在输入量类中）
//szDbName:数据库名称；szTabName：表名；
//scKeyFldName：关键字域名；pBuf：存放关键字域值
extern "C"
{
	int GetIndex(LPCSTR szDbName,LPCSTR szTabName,LPCSTR scKeyFldName,char* pBuf);
}

/********************************************************
//    读取域值
///
*********************************************************/
//按关键字读取域值
//szDbName:数据库名称；szTabName：表名；
//scKeyFldName：关键字域名；szKeyValue：关键字域值
//szFldName：要读取域名；pBuf：域值存放区
//返回值：TRUE为正确执行；FALSE为有错误
AFX_EXT_CLASS BOOL GetFldValue(LPCSTR szDbName,LPCSTR szTabName,LPCSTR scKeyFldName,LPCSTR szKeyValue,LPCSTR szFldName,char* pBuf);

//读取域值(不属于任何表）
//szDbName:数据库名称；
//szFldName：要读取域名；pBuf：域值存放区
//返回值：TRUE为正确执行；FALSE为有错误
AFX_EXT_CLASS BOOL GetFldValue(LPCSTR szDbName,LPCSTR szFldName,char* pBuf);

//按记录号读取域值
//index:记录号（从1开始）
//szDbName:数据库名称；szTabName：表名；
//szFldName：要读取域名；pBuf：域值存放区
//返回值：TRUE为正确执行；FALSE为有错误
AFX_EXT_CLASS BOOL GetFldValue(int index,LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf);


/********************************************************
//    修改域值
///
*********************************************************/
//按关键字修改域值
//szDbName:数据库名称；szTabName：表名；
//scKeyFldName：关键字域名；szKeyValue：关键字域值
//szFldName：要修改域名；pBuf：域值存放区
//返回值：TRUE为正确执行；FALSE为有错误
AFX_EXT_CLASS BOOL UpdateFldValue(LPCSTR szDbName,LPCSTR szTabName,LPCSTR scKeyFldName,LPCSTR szKeyValue,LPCSTR szFldName,char* pBuf);


//修改域值(不属于任何表）
//szDbName:数据库名称；
//szFldName：要读取域名；pBuf：域值存放区
//返回值：TRUE为正确执行；FALSE为有错误
AFX_EXT_CLASS BOOL UpdateFldValue(LPCSTR szDbName,LPCSTR szFldName,char* pBuf);


//按记录号修改域值
//index:记录号（从1开始）
//szDbName:数据库名称；szTabName：表名；
//szFldName：要修改域名；pBuf：域值存放区
//返回值：TRUE为正确执行；FALSE为有错误
AFX_EXT_CLASS BOOL UpdateFldValue(int index,LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf);

/********************************************************
//    增加记录
///
*********************************************************/
//增加一条记录必须首先调用该函数，而后按记录号修改各域值
//szDbName:数据库名称；szTabName：表名；
//返回值：正确则返回记录号，否则为-1
extern "C"
{
	int AddOneRecord(LPCSTR szDbName,LPCSTR szTabName);
}

/********************************************************
//    删除记录
///
*********************************************************/
//删除最后一条记录
AFX_EXT_CLASS BOOL DeleteOneRecord(LPCSTR szDbName,LPCSTR szTabName);

//删除其中一条记录
//index 为记录号
AFX_EXT_CLASS BOOL DeleteOneRecord(int index,LPCSTR szDbName,LPCSTR szTabName);

//删除所有记录
AFX_EXT_CLASS BOOL DeleteAllRecord(LPCSTR szDbName,LPCSTR szTabName);


/***************************************
// 获得配置文件中的值
//
**************************************************/
AFX_EXT_CLASS char* GetCfgValue(const char *VarString);

/***********************************
//连接服务器
//
*******************************************/
extern "C"
{
	BOOL ConnectHost();
}
extern "C"
{
	void DisConnectHost();
}
/******************************************
//用户登录名及权限读写
//
************************************************/
//从内存中读取用户名

AFX_EXT_CLASS	void GetUserName(LPCSTR szUserName);

//写用户名到内存中

AFX_EXT_CLASS	void UpdateUserName(LPCSTR szUserName);

//读用户权限

AFX_EXT_CLASS	int GetUserPrior();

//写用户权限

AFX_EXT_CLASS	void UpdateUserPrior(int nPrior);

//读取用户密码

AFX_EXT_CLASS	void GetUserPsw(LPCSTR szUserPsw);

//写用户密码

AFX_EXT_CLASS	void UpdateUserPsw(LPCSTR szUserPsw);


/************************************************
//直接访问数据库地址
//
*************************************************/
AFX_EXT_CLASS LPVOID GetRTDBInfoPnt( LPSTR szDbInfoStr);
AFX_EXT_CLASS LPVOID GetRTDBPnt( LPSTR szDbStr);


//2005-6-14
/*************************************
//数据库拷贝
//
//
******************************************/
//从服务器数据库中拷贝数据到本地数据库
//szDbName 为数据库名称
AFX_EXT_CLASS BOOL CopyRTDBFromServer(LPCSTR szDbName);
//将本地数据库数据拷贝到服务器数据库中
//szDbName 为数据库名称
AFX_EXT_CLASS BOOL CopyRTDBToServer(LPCSTR szDbName);

//读取某字段的所有纪录值
//
AFX_EXT_CLASS BOOL GetFldAllRec(LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf);
//写某字段的所有纪录值
//
AFX_EXT_CLASS BOOL UpdateFldAllRec(LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf,DWORD dwSize);

/*************************************************
//进入以及退出操作票状态
//
**************************************************/
//进入操作票状态
AFX_EXT_CLASS BOOL IntoOprStatus(BOOL bInit=FALSE);
//退出操作票状态
AFX_EXT_CLASS BOOL OutOprStatus();


/*************************************************
//图形系统变量操作接口函数
//
**************************************************/
//读取变量序号，返回序号，若该变量不存在则返回值<=0
//szVarName为变量名称
AFX_EXT_CLASS int GetPsgVarIndex(LPCSTR szVarName);
//依据序号读取变量值
//index变量序号，ivalue、fvalue、 cvalue为变量值
AFX_EXT_CLASS BOOL GetPsgVarValue(int index,int& ivalue);
AFX_EXT_CLASS BOOL GetPsgVarValue(int index,float& fvalue);
AFX_EXT_CLASS BOOL GetPsgVarValue(int index,char* cvalue);
//依据序号修改变量值
//index变量序号，ivalue、fvalue、 cvalue为变量值
AFX_EXT_CLASS BOOL SetPsgVarValue(int index,int ivalue);
AFX_EXT_CLASS BOOL SetPsgVarValue(int index,float fvalue);
AFX_EXT_CLASS BOOL SetPsgVarValue(int index,char* cvalue);
//依据变量名称读取变量值
//szVarName变量名称，ivalue、fvalue、 cvalue为变量值
AFX_EXT_CLASS BOOL GetPsgVarValue(LPCSTR szVarName,int& ivalue);
AFX_EXT_CLASS BOOL GetPsgVarValue(LPCSTR szVarName,float& fvalue);
AFX_EXT_CLASS BOOL GetPsgVarValue(LPCSTR szVarName,char* cvalue);
//依据变量名称修改变量值
//szVarName变量名称，ivalue、fvalue、 cvalue为变量值
AFX_EXT_CLASS BOOL SetPsgVarValue(LPCSTR szVarName,int ivalue);
AFX_EXT_CLASS BOOL SetPsgVarValue(LPCSTR szVarName,float fvalue);
AFX_EXT_CLASS BOOL SetPsgVarValue(LPCSTR szVarName,char* cvalue);

/********************************************
//PSS服务器操作
//
*********************************************/
//连接指定服务器
AFX_EXT_CLASS BOOL ConnectHostByName(LPCSTR strHostName);

//获得所配置的PSS服务器个数
AFX_EXT_CLASS int GetPssServerNum();

//获得PSS服务器的名称
//index从1开始，szServerName返回PSS服务器的名称，函数返回值为FALSE则没有正确获得PSS服务器的名称
AFX_EXT_CLASS BOOL GetPssServerName(int index,LPCSTR szServerName);
//获得PSS服务器的当前状态，返回值1表示该PSS服务器系统已启动状态，0表示没有启动系统
//index从1开始
AFX_EXT_CLASS int GetPssServerStatus(int index);


////////////////////////////////////////////////////////
//前版本函数
AFX_EXT_CLASS BOOL GetInFldValue(LPCSTR szDbName,LPCSTR szTabName,LPCSTR scKeyFldName,LPCSTR szKeyValue,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL GetOutFldValue(LPCSTR szDbName,LPCSTR szTabName,LPCSTR scKeyFldName,LPCSTR szKeyValue,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL GetInFldValue(LPCSTR szDbName,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL GetOutFldValue(LPCSTR szDbName,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL GetInFldValue(int index,LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL GetOutFldValue(int index,LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL UpdateInFldValue(LPCSTR szDbName,LPCSTR szTabName,LPCSTR scKeyFldName,LPCSTR szKeyValue,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL UpdateOutFldValue(LPCSTR szDbName,LPCSTR szTabName,LPCSTR scKeyFldName,LPCSTR szKeyValue,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL UpdateInFldValue(LPCSTR szDbName,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL UpdateOutFldValue(LPCSTR szDbName,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL UpdateInFldValue(int index,LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf);
AFX_EXT_CLASS BOOL UpdateOutFldValue(int index,LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf);
////////////
extern "C"
{
 BOOL GetFldValue1(LPCSTR szDbName,LPCSTR szTabName,LPCSTR scKeyFldName,LPCSTR szKeyValue,LPCSTR szFldName,char* pBuf);
}
extern "C"
{
	BOOL GetFldValue2(LPCSTR szDbName,LPCSTR szFldName,char* pBuf);
}
extern "C"
{
	BOOL GetFldValue3(int index,LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf);
}
extern "C"
{
	BOOL UpdateFldValue1(LPCSTR szDbName,LPCSTR szTabName,LPCSTR scKeyFldName,LPCSTR szKeyValue,LPCSTR szFldName,char* pBuf);
}
extern "C"
{
	BOOL UpdateFldValue2(LPCSTR szDbName,LPCSTR szFldName,char* pBuf);
}
extern "C"
{
	BOOL UpdateFldValue3(int index,LPCSTR szDbName,LPCSTR szTabName,LPCSTR szFldName,char* pBuf);
}
extern "C"
{
	BOOL DeleteOneRecord1(LPCSTR szDbName,LPCSTR szTabName);
}
extern "C"
{
	BOOL DeleteOneRecord2(int index,LPCSTR szDbName,LPCSTR szTabName);
}

extern "C"
{
	BOOL GetCfgValueByName(LPCSTR   szVarString,char* szValue);
}

#undef AFX_DATA
#define AFX_DATA

#endif