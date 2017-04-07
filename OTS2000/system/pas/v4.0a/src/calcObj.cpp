
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"

#include <libxml/parser.h>

#pragma comment(lib, "xml2.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma comment(lib, "../../../lan/v4.0a/lib/lan.lib")

enum NODE_TYPE {POINT_NODE_TYPE= 1, COMBO_NODE_TYPE= 2};

#ifndef WINDOWS_OS
#define Sleep(x)	usleep(1000*(x))
#endif


BOOL broadcast_obj_priv;
unsigned char send_buf[1440];

int GetNodePropStr(xmlNodePtr node, const xmlChar *propName, xmlChar *propValue)
{
	xmlChar* str = xmlGetProp(node,propName);
	if(!str)
	{
		SendTraceMsg(0,0,"Fail to find Node %s prop %s define", node->name, propName);
		return -1;
	}
	strcpy((char*)propValue, (char*)str);
	xmlFree(str);
	return 0;
}

void PrintNodeName(const xmlChar* nodeName, int level)
{
	int i;
	
	printf("\n");
	for(i=0;i<level;i++)
		printf("\t");
	printf("node: %s ", nodeName);
}

void PrintProp(const xmlChar* szAttrName, xmlChar* szAttrValue, int level)
{
	printf("%s=%s ", szAttrName, szAttrValue);
}
void PrintNodeProp(xmlNodePtr curNode, int level)
{
	xmlNodePtr subNode= curNode->xmlChildrenNode;
	while(subNode != NULL) 
	{
		if(subNode->type==XML_ELEMENT_NODE)
		{
			PrintNodeName(subNode->name, level);
			
			xmlAttr *attr= subNode->properties;
			while(attr != NULL)
			{
				xmlChar szAttr[256];
				GetNodePropStr(subNode, attr->name, szAttr);
				PrintProp(attr->name, szAttr, level);
				attr= attr->next;
			}
			//printf("\n");

			PrintNodeProp(subNode, level+1);
		}
		
		subNode= subNode->next;
	}
}

int GetNodeType(xmlNodePtr node)
{
	enum NODE_TYPE	node_type;
	xmlChar pinTypeStr[256];

	if(-1==GetNodePropStr(node, BAD_CAST "TYPE", pinTypeStr))
		return -1;

	if(!xmlStrcmp(pinTypeStr, BAD_CAST "POINT"))
		node_type= POINT_NODE_TYPE;		
	else if(!xmlStrcmp(pinTypeStr, BAD_CAST "COMBO"))
		node_type= COMBO_NODE_TYPE;		
	else
	{
		SendTraceMsg(0,0,"NODE %s TYPE %s error", node->name, pinTypeStr);
		return -1;
	}
	return node_type;
}

int GetSinglePtNodeState(xmlNodePtr curNode, BOOL *pState)
{
	xmlChar ptNameStr[256];
	xmlChar oprStr[256];
	POINTER	 pt;
	
	if(-1==GetNodePropStr(curNode, BAD_CAST "NAME", ptNameStr))
		return -1;
	if(-1==GetPtIdByNameStr((char*)ptNameStr, &pt))
		return -1;
	if(-1==GetNodePropStr(curNode, BAD_CAST "OPR", oprStr))
		return -1;

	if(!xmlStrcmp(oprStr,  BAD_CAST "?")) //IND
	{
		int	define_state;
		USHORT curState;
		xmlChar stateStr[256];

		if(pt.data_type!=SOE_TYPE && pt.data_type!=POL_TYPE && pt.data_type!=SYSMSG_TYPE
			&& pt.data_type!=OUTS_TYPE)
		{
			SendTraceMsg(0,0,"NODE %s ? opr must use SOE/POL/SYSMSG/OUTS pt: %s", curNode->name, oprStr);
			return -1;			
		}
		
		if(-1==GetNodePropStr(curNode, BAD_CAST "STATE", stateStr))
			return -1;
		if(1!=sscanf((char*)stateStr, "%d", &define_state))
		{
			SendTraceMsg(0,0,"NODE %s STATE define error: %s", curNode->name, stateStr);
			return -1;			
		}
		
		GetIndPtStateById(&pt, &curState);
		if(curState==(USHORT)define_state)
			*pState= TRUE;
		else
			*pState= FALSE;
		return 0;
	}	
	else //ANA
	{
		xmlChar valueStr[256];
		float	define_value, curValue;

		if(pt.data_type!=ANA_TYPE && pt.data_type!=CALC_TYPE && pt.data_type!=OUTA_TYPE
			&& pt.data_type!=PARAM_TYPE)
		{
			SendTraceMsg(0,0,"NODE %s ? opr must use ANA/PARAM/CALC/OUTA pt: %s", curNode->name, oprStr);
			return -1;			
		}
		
		if(-1==GetNodePropStr(curNode, BAD_CAST "VALUE", valueStr))
			return -1;
		if(1!=sscanf((char*)valueStr, "%f", &define_value))
		{
			SendTraceMsg(0,0,"NODE %s VALUE define error: %s", curNode->name, valueStr);
			return -1;			
		}
		
		GetAnaPtfValueById(&pt, &curValue);
		if(!xmlStrcmp(oprStr, BAD_CAST "<"))
		{
			if(curValue<define_value)
				*pState= TRUE;
			else
				*pState= FALSE;
		}
		else if(!xmlStrcmp(oprStr, BAD_CAST "<="))
		{
			if(curValue<=define_value)
				*pState= TRUE;
			else
				*pState= FALSE;
		}
		else if(!xmlStrcmp(oprStr, BAD_CAST ">"))
		{
			if(curValue>define_value)
				*pState= TRUE;
			else
				*pState= FALSE;
		}
		else if(!xmlStrcmp(oprStr, BAD_CAST ">="))
		{
			if(curValue>=define_value)
				*pState= TRUE;
			else
				*pState= FALSE;
		}
		else
		{
			SendTraceMsg(0,0,"NODE %s unsupported opr define: %s", curNode->name, oprStr);
			return -1;			
		}
		return 0;
	}
}
int GetComboPtNodeState(xmlNodePtr curNode, BOOL *pState)
{
	int	comboLogicFlag;
	xmlChar comboLogicStr[256];
	
	if(-1==GetNodePropStr(curNode, BAD_CAST "OPR", comboLogicStr))
		return -1;

	if(!xmlStrcmp(comboLogicStr, BAD_CAST "AND"))
		comboLogicFlag= 1;
	else if(!xmlStrcmp(comboLogicStr, BAD_CAST "OR"))
		comboLogicFlag= 2;
	else
	{
		SendTraceMsg(0,0,"unsupported operate define: %s", comboLogicStr);
		return -1;
	}

	xmlNodePtr subNode= curNode->xmlChildrenNode;
	while(subNode != NULL) 
	{
		if(subNode->type==XML_ELEMENT_NODE)
		{
			BOOL subNodeState;
			int		ret;

			int node_type= GetNodeType(subNode);
			if(-1==node_type)
			{
				SendTraceMsg(0,0,"NODE %s TYPE error", subNode->name);
				return -1;
			}
			
			if(node_type==POINT_NODE_TYPE)
				ret= GetSinglePtNodeState(subNode, &subNodeState);
			else if(node_type==COMBO_NODE_TYPE)
				ret= GetComboPtNodeState(subNode, &subNodeState);
			else
			{
				SendTraceMsg(0,0,"NODE %s TYPE %d error", subNode->name, node_type);
				return -1;
			}

			if(-1==ret)
			{
				SendTraceMsg(0,0,"NODE %s calc error", subNode->name);
				return -1;
			}
			
			if(comboLogicFlag==1) //AND
			{
				if(subNodeState==FALSE)
				{
					*pState= FALSE;
					return 0;
				}
			}
			else if(comboLogicFlag==2) //OR
			{
				if(subNodeState==TRUE)
				{
					*pState= TRUE;
					return 0;
				}
			}
		}
		
		subNode= subNode->next;
	}

	if(comboLogicFlag==1) //AND
		*pState= TRUE;
	else if(comboLogicFlag==2) //OR
		*pState= FALSE;

	return 0;
}
int GetObjPinState(xmlNodePtr curObj, xmlChar* pinName, BOOL *pState)
{
	int findFlag= 0;

	xmlNodePtr pinNode= curObj->xmlChildrenNode;
	while(pinNode != NULL) 
	{
		if(pinNode->type==XML_ELEMENT_NODE)
		{
			if(!xmlStrcmp(pinNode->name, pinName))
			{
				findFlag= 1;
				break;
			}
		}
		pinNode= pinNode->next;
	}

	if(!findFlag)
	{
		SendTraceMsg(0,0,"Fail to find %s pin %s define", curObj->name, pinName);
		return -1;
	}
	
	int node_type= GetNodeType(pinNode);
	if(-1==node_type)
	{
		SendTraceMsg(0,0,"OBJ %s pin %s TYPE error", curObj->name, pinName);
		return -1;
	}

	if(node_type==POINT_NODE_TYPE)
		return GetSinglePtNodeState(pinNode, pState);
	else if(node_type==COMBO_NODE_TYPE)
		return GetComboPtNodeState(pinNode, pState);
	else
	{
		SendTraceMsg(0,0,"OBJ %s pin %s TYPE %d error", curObj->name, pinName, node_type);
		return -1;
	}
}

void AddObjtoSendBuf(unsigned char *buf_ptr, POINTER pt, ENTRY_DEVOBJ_VAL *obj_var)
{
	buf_ptr[0]= DPS_DEV_FB;
	buf_ptr[1]= pt.stn_id;
	buf_ptr[2]= pt.dev_id;
	buf_ptr[3]= pt.data_type;
	buf_ptr[4]= 1; //number lo byte
	buf_ptr[5]= 0; //number hi byte
	memcpy(buf_ptr+6, &pt.pt_id, 2);
	Swap2Byte(buf_ptr+6);
	memcpy(buf_ptr+8, &obj_var->fb_status1, 2);
	Swap2Byte(buf_ptr+8);
	memcpy(buf_ptr+10, &obj_var->fb_status2, 2);
	Swap2Byte(buf_ptr+10);
	memcpy(buf_ptr+12, &obj_var->param, 2);
	Swap2Byte(buf_ptr+12);

	DATE_TIME	time;
	GetSysTime(&time);
	memcpy(buf_ptr+14, &time.ms, 2);
	Swap2Byte(buf_ptr+14);
	GetLocalHostId(buf_ptr+16);
	buf_ptr[17]= time.second;
	buf_ptr[18]= time.minute;
	buf_ptr[19]= time.hour;
	buf_ptr[20]= time.day;
	buf_ptr[21]= time.month;
	memcpy(&buf_ptr[22], &time.year, 2);
	Swap2Byte(&buf_ptr[22]);
}
void UpdateObjLib(xmlNodePtr curNode, int level)
{
	int send_obj_num= 0;

	xmlNodePtr objNode= curNode->xmlChildrenNode;
	while(objNode != NULL) 
	{
		if(objNode->type==XML_ELEMENT_NODE)
		{
			xmlChar objName[256];
			
			if(-1!=GetNodePropStr(objNode, BAD_CAST "NAME", objName))
			{
				POINTER	point;

				if(-1!=GetPtIdByNameStr((char*)objName, &point))
				{
					BOOL pinState;
					ENTRY_DEVOBJ_VAL devobj_var;
					DMS_COMMON	common_all;
					
					ReadEntryById(&point, &common_all);
					devobj_var= common_all.var.devobj;
					if(-1!=GetObjPinState(objNode, BAD_CAST "CONDC0", &pinState))
						devobj_var.fb_status1.condc0= pinState;
					if(-1!=GetObjPinState(objNode, BAD_CAST "CONDC1", &pinState))
						devobj_var.fb_status1.condc1= pinState;
					if(-1!=GetObjPinState(objNode, BAD_CAST "SWGF_N", &pinState))
						devobj_var.fb_status1.swgf_n= pinState;
					if(-1!=GetObjPinState(objNode, BAD_CAST "P0", &pinState))
						devobj_var.fb_status1.p0= pinState;
					if(-1!=GetObjPinState(objNode, BAD_CAST "P1", &pinState))
						devobj_var.fb_status1.p1= pinState;
					if(-1!=GetObjPinState(objNode, BAD_CAST "LOCALM", &pinState))
						devobj_var.fb_status1.localm= pinState;
					
					GetSysTime(&devobj_var.update_time);
					SetDevObjPtVarById(&point, devobj_var);
					if(broadcast_obj_priv)
					{
						AddObjtoSendBuf(send_buf+send_obj_num*24, point, &devobj_var);
						send_obj_num++;
						if(send_obj_num>=50)
						{
							LAN_HEAD	lanhead;
							lanhead.length = send_obj_num*24;
							Swap2Byte(&lanhead.length);
							lanhead.dp_type=BJ_DB_TIME;
							GetLocalHostId(&lanhead.src_id);
							lanhead.dest_id =0;
							lanhead.dest_stn =send_buf[1];
							lanhead.packet_num =0;
							lan_out ((char *)&send_buf,lanhead,(unsigned char)0);
							send_obj_num = 0;
						}
					}
				}
			}
			else
			{
				SendTraceMsg(0,0,"Fail to get OBJ %s name", objNode->name);
			}

		}
		
		objNode= objNode->next;
	}

	if(send_obj_num>0)
	{
		LAN_HEAD	lanhead;
		lanhead.length = send_obj_num*24;
		Swap2Byte(&lanhead.length);
		lanhead.dp_type=BJ_DB_TIME;
		GetLocalHostId(&lanhead.src_id);
		lanhead.dest_id =0;
		lanhead.dest_stn =send_buf[1];
		lanhead.packet_num =0;
		lan_out ((char *)&send_buf,lanhead,(unsigned char)0);
		send_obj_num = 0;
	}
}

int main(int argc, char* argv[])
{

    xmlDocPtr doc;           //定义解析文档指针
    xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动) 

#ifdef	WINDOWS_OS
#endif
	
	CreateLockFile(".obj_calc.lock");
	if(-1==InitSysLog(H9000_MODULE_OBJCALC, "ObjCalc"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	
	
#ifdef	WINDOWS_OS
	WSADATA		WSAData;
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		Err_log("Fail to initialize net!\n");
		exit(-1);
	}
#endif
	
	if(-1==MountDB(NULL))
	{
		Err_log("Fail to Mount RDB");
		exit(-1);
	}
	
	if(!lan_out_init())
	{
		Err_log("Fail to init lan_out");
		exit(-1);
	}

    char filename[256];
	sprintf(filename, "%s%s", SYS_DEFINE_PATH, OBJ_CALC_DEFINE_FILE);
    doc = xmlReadFile(filename,"uft-8",XML_PARSE_RECOVER); //解析文件

    //检查解析文档是否成功，如果不成功，libxml将指一个注册的错误并停止。
    //一个常见错误是不适当的编码。XML标准文档除了用UTF-8或UTF-16外还可用其它编码保存。
    //如果文档是这样，libxml将自动地为你转换到UTF-8。更多关于XML编码信息包含在XML标准中.

    if (NULL == doc) 
    {  
       Err_log("Fail to parse xml document");     
       exit(0);
    } 

    curNode = xmlDocGetRootElement(doc); //确定文档根元素

    /*检查确认当前文档中包含内容*/ 
    if (NULL == curNode)
    { 
       Err_log("empty document"); 
       xmlFreeDoc(doc); 
       exit(0);
    } 

    /*在这个例子中，我们需要确认文档是正确的类型。“root”是在这个示例中使用文档的根类型。*/

    if (xmlStrcmp(curNode->name, BAD_CAST "root")) 
    {
       Err_log("document of the wrong type, root node != root"); 
       xmlFreeDoc(doc); 
       exit(0);
    } 

	PrintNodeProp(curNode, 0);

	for(;;)
	{
		int host_priv= 0;
		broadcast_obj_priv= FALSE;
		if(GetLocalHostPrvg("APP_SERVER", &host_priv) == 0)
		{
			if(host_priv == TRUE)
			{
				USHORT hostStat; 
				if(-1!=GetLocalHostState(&hostStat))
				{
					if( hostStat==1 ) //MAIN
						broadcast_obj_priv= TRUE;
				}
			}
		}

		UpdateObjLib(curNode, 0);
		Sleep(1000);
	}

    xmlFreeDoc(doc);

    return 0;

}

