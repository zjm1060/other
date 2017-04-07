<%@ page language="java" import="java.util.*,jbportal.PortalUtil" errorPage="../include/error.jsp" %>
<%@ page contentType="text/html; charset=GBK" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<%
	Hashtable dct_Para = new Hashtable();
	Hashtable dct_Key = new Hashtable();
    String m_ModuID = request.getParameter("modu_id");
      //out.print(m_ModuID);
	  dct_Key.put("modu_id",m_ModuID);
      String title =request.getParameter("title");// new String(request.getParameter("title").getBytes("iso8859-1"),"GB2312");
      String Description =request.getParameter("Description");//new String(request.getParameter("Description").getBytes("iso8859-1"),"GB2312");
      String DashBoardOrder =jbportal.PortalUtil.getValue(request.getParameter("DashBoardOrder"));
      String DashBoardImageLarge =jbportal.PortalUtil.getValue(request.getParameter("DashBoardImageLarge"));
      String DashBoardImageSmall =jbportal.PortalUtil.getValue(request.getParameter("DashBoardImageSmall"));
      String AllowPersonize =jbportal.PortalUtil.getValueN(request.getParameter("AllowPersonize"));
      String AllowUse=jbportal.PortalUtil.getValueN(request.getParameter("AllowUse"));

	  dct_Para.put("title",title);
      dct_Para.put("modu_id",m_ModuID);
	  dct_Para.put("Description",Description);
	  dct_Para.put("DashBoardOrder",PortalUtil.toDouble(DashBoardOrder));
	  dct_Para.put("DashBoardImageLarge",DashBoardImageLarge);
	  dct_Para.put("DashBoardImageSmall",DashBoardImageSmall);
	  dct_Para.put("AllowPersonize",PortalUtil.toDouble(AllowPersonize));
      dct_Para.put("AllowUse",PortalUtil.toDouble(AllowUse));
      int sReturn = oSys.update(dct_Key,dct_Para,"modu_def",false);
      dct_Key.clear();
      dct_Para.clear();
      String m_usercode=(String)request.getRemoteUser();
      dct_Key.put("modu_id",m_ModuID);
      dct_Key.put("modu_type","0");
      dct_Key.put("user_code",m_usercode);
      dct_Para.put("use_flag",PortalUtil.toDouble(AllowUse));
	  sReturn = oSys.update(dct_Key,dct_Para,"user_modu",false);

//	if (sReturn >=0)
//          {
		response.sendRedirect("ModuleDetails.jsp?m_ModuID=" + m_ModuID + "&m_Operation=0");
//          }
	//else
		//ShowDBError("update",sReturn,"");

%>
