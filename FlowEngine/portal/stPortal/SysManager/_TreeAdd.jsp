<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page language="java" import="java.util.Hashtable,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
      String m_Msg = "";
      Hashtable dct_Para = new Hashtable();
      Hashtable dct_Key = new Hashtable();
      String m_Operation = request.getParameter("m_Operation");
      String m_NodeValue = request.getParameter("HNodeValue");

      String m_OrderTemp = oManager.sGetModuID(m_NodeValue);
      String m_NodeID = m_OrderTemp.substring(0,4);
      String m_Order =m_OrderTemp.substring(5,m_OrderTemp.length());

	  if (m_Operation.equals("del")){
        //删除公用模块信息
        int sReturn=oSys.DeleteModu(m_NodeValue);
		if (sReturn >=0){m_Msg="OK";} else {m_Msg="Err";}
      }
	  else{
        String title =request.getParameter("HNodeName");// new String(request.getParameter("HNodeName").getBytes("iso8859-1"),"GB2312");
        String HNodeDesc =request.getParameter("HNodeDesc");//new String(request.getParameter("HNodeDesc").getBytes("iso8859-1"),"GB2312");
        String canPersonalize=new String(request.getParameter("HNodePersonalize"));
        String canUse=new String(request.getParameter("HNodeAllowUse"));
		dct_Key.put("modu_id",m_NodeValue + m_NodeID);
		dct_Para.put("modu_id",m_NodeValue + m_NodeID);
		dct_Para.put("title",title);
        if(m_NodeValue!=null && !m_NodeValue.trim().equals(""))
      		dct_Para.put("para_id",m_NodeValue);
		dct_Para.put("Description",HNodeDesc);
		dct_Para.put("DashBoardOrder",PortalUtil.toDouble(m_Order));
		dct_Para.put("DashBoardImageLarge","");
		dct_Para.put("DashBoardImageSmall","");
        dct_Para.put("ALLOWPERSONIZE",PortalUtil.toDouble(canPersonalize));
        dct_Para.put("ALLOWUSE",PortalUtil.toDouble(canUse));

        boolean isOK=false;
		int sReturn = oSys.update(dct_Key,dct_Para,"modu_def",true);
        isOK=sReturn>=0;
        //清空hash表中的数据。
        dct_Key.clear();
        dct_Para.clear();
        //设置权限表中权限的主键。
        dct_Key.put("modu_id",m_NodeValue + m_NodeID);
        dct_Key.put("user_code","AllPortalUser");
        dct_Key.put("user_type","1");

        dct_Para.put("modu_id",m_NodeValue + m_NodeID);
        dct_Para.put("user_code","AllPortalUser");
        dct_Para.put("user_type",PortalUtil.toDouble("1"));
        dct_Para.put("allow_use",PortalUtil.toDouble("1"));
        dct_Para.put("deny_use",PortalUtil.toDouble("0"));

        sReturn=oSys.update(dct_Key,dct_Para,"modu_right",true);
        isOK= isOK && (sReturn>=0);
		if (sReturn >=0){m_Msg = "OK";} else {m_Msg = "Err";}
      }
      dct_Para=null;
      dct_Key=null;
      m_Msg = "OK";
%>
<SCRIPT type="text/javascript">
  var m_Msg = "<%=m_Msg%>";
  var m_Operation = "<%=m_Operation%>";
  var obj=parent
  if (obj!=null)
  {
    //modify by hhy 2002-6-10
    //不知道怎么搞的，parent的值竟然存在二意性，有时是main,有时是main的parent
    if(obj.name=='main')
      obj=obj.parent
    if (m_Msg!=""){
      if (m_Operation=='add'){
        obj.AddNewNode('<%=m_NodeValue + m_NodeID%>');
      }
      else{
        if (m_Operation=='del')
        {
          obj.OnDeleteNode('<%=m_NodeValue%>');
        }
      }
    }
    else{
      obj.MsgError('<%=m_Msg%>');
    }
  }
</SCRIPT>
