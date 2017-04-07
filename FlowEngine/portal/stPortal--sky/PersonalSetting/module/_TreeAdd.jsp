<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.Hashtable,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    response.setHeader("Cache-Control","no-cache");
    String m_Msg = "";
    Hashtable dct_Para = new Hashtable();
    Hashtable dct_Key = new Hashtable();
	String m_Operation = request.getParameter("m_Operation");
	String m_NodeValue = request.getParameter("HNodeValue");
    if (m_NodeValue == null)
          m_NodeValue = "";
    String HNodeName =request.getParameter("HNodeName");// new String(request.getParameter("HNodeName").getBytes("iso8859-1"),"GB2312");
    if (HNodeName == null)
        HNodeName = "";
    String HNodeDesc =request.getParameter("HNodeDesc");// new String(request.getParameter("HNodeDesc").getBytes("iso8859-1"),"GB2312");
    if (HNodeDesc == null)
        HNodeDesc = "";

    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();

    String m_OrderTemp = oManager.sGetPersonModuID(m_usercode,m_NodeValue);
    String m_NodeID = m_OrderTemp.substring(0,4);
    String m_Order =m_OrderTemp.substring(5,m_OrderTemp.length());
    //执行数据库操作
    if (m_Operation.equals("del")){
      //删除个人模块及修改相关表
      int sReturn = oSys.DeletePersonModu(m_NodeValue,m_usercode);
      if (sReturn >=0)
        m_Msg = "DeleteOk";
      else
        m_Msg = "DeleteErr";
    }
    else
    {
      //新增模块
      dct_Key.put("modu_id",m_NodeValue + m_NodeID);
      dct_Key.put("user_code",m_usercode);

      dct_Para.put("modu_id",m_NodeValue + m_NodeID);
      dct_Para.put("user_code",m_usercode);
      dct_Para.put("title",HNodeName);
      if(m_NodeValue!=null && !m_NodeValue.trim().equals(""))
      	dct_Para.put("para_id",m_NodeValue);
      dct_Para.put("Description",HNodeDesc);
      dct_Para.put("DashBoardOrder",PortalUtil.toDouble(m_Order));
      dct_Para.put("DashBoardImageLarge","");
      dct_Para.put("DashBoardImageSmall","");

      int sReturn = oSys.update(dct_Key,dct_Para,"pers_modu",true);
      if (sReturn >=0)
        m_Msg = "AddOk";
      else
        m_Msg = "AddErr";
    }
%>
<SCRIPT LANGUAGE=javascript>
<!--
   var m_Msg = '<%=m_Msg%>';
   var m_Operation = '<%=m_Operation%>';
   //modify by hhy 2002-6-10
   var obj=parent;
   if (obj!=null)
	{
      if(obj.name=='main')
        obj=obj.parent;
      if (m_Msg!=''){
        if (m_Operation=='add'){
          obj.AddNewNode('<%=m_NodeValue + m_NodeID%>');
        }
        else{
          if (m_Operation=='del'){
            obj.OnDeleteNode('<%=m_NodeValue%>');
          }
        }
      }
      else{
        obj.MsgError('<%=m_Msg%>');
      }
	}
  //end modify by hhy
//-->
</SCRIPT>
