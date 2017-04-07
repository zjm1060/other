<%@ page contentType="text/html; charset=GBK" errorPage="../include/error.jsp"%>
<%@ page language="java" import="java.util.Hashtable,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
	String m_Msg = "" ;
    Hashtable dct_Para = new Hashtable();
    Hashtable dct_Key = new Hashtable();
    String m_Order="";
	String m_Operation = request.getParameter("m_Operation");
	String m_NodeValue = request.getParameter("HNodeValue");
	String m_usercode=(String)(request.getRemoteUser());
    //删除公用功能
    if(m_Operation.equals("del")){
        int sReturn = oSys.DeletePublicFunction(m_NodeValue);
        if (sReturn >=0){m_Msg="OK";} else {m_Msg="Err";}
    }
    //新增公用功能
	else{
      //得到最大id号
      m_Order = oManager.sGetFuncID(m_NodeValue);
      dct_Key.put("func_id",m_Order);
      String href_desc="";
      if (!(m_NodeValue.equals("0")))
      {
        dct_Para.put("modu_id",m_NodeValue);
      }
      //request.getCharacterEncoding();
      String title =request.getParameter("HNodeName");//new String(request.getParameter("HNodeName").getBytes("iso8859-1"),"GB2312");
      String HNodeName = request.getParameter("HNodeName");//new String(request.getParameter("HNodeName").getBytes("iso8859-1"),"GB2312");
      String HNodeDesc =request.getParameter("HNodeDesc");// new String(request.getParameter("HNodeDesc").getBytes("iso8859-1"),"GB2312");

      dct_Para.put("func_id",m_Order);
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

      int sReturn = oSys.update(dct_Key,dct_Para,"func_def",true);
      if (sReturn >=0){
        m_Msg = "OK";
        //清空hash表中的数据。
        dct_Key.clear();
        dct_Para.clear();
        //设置权限表中权限的主键。
        dct_Key.put("func_id",m_Order);
        dct_Key.put("user_code","AllPortalUser");
        dct_Key.put("user_type","1");

        dct_Para.put("func_id",m_Order);
        dct_Para.put("user_code","AllPortalUser");
        dct_Para.put("user_type",PortalUtil.toDouble("1"));
        dct_Para.put("allow_use",PortalUtil.toDouble("1"));
        dct_Para.put("deny_use",PortalUtil.toDouble("0"));

        sReturn=oSys.update(dct_Key,dct_Para,"Func_Right",true);
        if (sReturn >=0){m_Msg = "OK";}
        else{m_Msg += "\n警告：公用功能权限设置失败了。";}
      }
      else{m_Msg = "新增公用函数失败了。";}
    }//if(m_Operation.equals("del"))
    //m_Msg = "OK";
%>
<SCRIPT type="text/javascript">
<!--
  var m_Msg = '<%=m_Msg%>';
  var m_Operation = '<%=m_Operation%>';
  if (parent!=null)
  {
    if (m_Msg=='OK')
    {
      if (m_Operation!='del'){
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
