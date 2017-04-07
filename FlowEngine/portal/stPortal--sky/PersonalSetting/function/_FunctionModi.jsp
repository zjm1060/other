<%@ page contentType="text/html; charset=GBK" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.Hashtable,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
	String m_Msg = "" ;
    Hashtable dct_Para = new Hashtable();
    Hashtable dct_Key = new Hashtable();
    String m_Operation = request.getParameter("m_Operation");
    String m_usercode=(String)(request.getRemoteUser());
    String m_username =oManager.getCurrentUserName();

    String m_NodeValue = request.getParameter("HNodeValue");
    if (m_NodeValue == null)
    	m_NodeValue = "";

    String HNodeName =request.getParameter("HNodeName");// new String(request.getParameter("HNodeName").getBytes("iso8859-1"),"GB2312");
    if (HNodeName == null)
    	HNodeName = "";
    String HNodeDesc =request.getParameter("HNodeDesc");//new String(request.getParameter("HNodeDesc").getBytes("iso8859-1"),"GB2312");
    if (HNodeDesc == null)
    	HNodeDesc = "";
          /*String href_desc = new String(request.getParameter("href_desc").getBytes("iso8859-1"),"GB2312");

          if (href_desc == null)
              href_desc = "";--*zxc 6.12*---*/
    String href_desc="";
    String m_Order = oManager.sGetPersonFuncID(m_usercode);
	if (m_Operation.equals("del"))
    {
		dct_Key.put("func_id",m_NodeValue);
		dct_Key.put("user_code",m_usercode);

		int iReturn = oSys.Delete(dct_Key,"pers_func");
		if (iReturn >=0)
        	m_Msg = "OK";
		else
			m_Msg = "DeleteErr";

    }
	else
    {
		dct_Key.put("func_id",m_Order);
		dct_Key.put("user_code",m_usercode);

		dct_Para.put("func_id",m_Order);
		dct_Para.put("user_code",m_usercode);
		if (!(m_NodeValue.equals("0")))
                    {
			dct_Para.put("modu_id",m_NodeValue);
		}
		dct_Para.put("title",HNodeName);
		dct_Para.put("Description",HNodeDesc);
		dct_Para.put("href","http://");
		dct_Para.put("href_desc",href_desc);
		dct_Para.put("AllowMinimize",PortalUtil.toDouble("1"));
		dct_Para.put("Zone",PortalUtil.toDouble("3"));
		dct_Para.put("IsVisible",PortalUtil.toDouble("1"));
		dct_Para.put("AllowRemove",PortalUtil.toDouble("1"));
		dct_Para.put("IsIncluded",PortalUtil.toDouble("1"));
		dct_Para.put("width",PortalUtil.toDouble("100"));
		dct_Para.put("height",PortalUtil.toDouble("100"));
		dct_Para.put("WidthEnabled",PortalUtil.toDouble("1"));
		dct_Para.put("HeightEnabled",PortalUtil.toDouble("1"));
		dct_Para.put("WidthUnitType","%");
        dct_Para.put("HeightUnitType","%");
		dct_Para.put("HasFrame",PortalUtil.toDouble("1"));
		dct_Para.put("FrameState",PortalUtil.toDouble("0"));
		dct_Para.put("RequiresIsolation",PortalUtil.toDouble("1"));

		int sReturn = oSys.AddPersonFunction(dct_Key,dct_Para,"pers_func",true);

		if (sReturn >=0)
        	m_Msg  = "OK";
		else
			m_Msg =  "保存对功能所做的修改失败";
	}
%>

<SCRIPT LANGUAGE=javascript>
<!--
   var m_Msg = '<%=m_Msg%>';
   var m_Operation = '<%=m_Operation%>';
   //alert('m_Msg='+ m_Msg);
   //alert('m_Operation='+ m_Operation);
	if (parent!=null){
    	if (m_Msg=='OK'){
        	if(m_Operation!='del'){
            	parent.AddNewNode('<%=m_Order%>');
            }
            else{
				parent.OnDeleteNode('<%=m_NodeValue%>');

            }
        }
        else{
			parent.MsgError(m_Msg);
        }
    }
//-->
</SCRIPT>

