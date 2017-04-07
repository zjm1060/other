<%@ page contentType="text/html; charset=GBK" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.Hashtable,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
  Hashtable dct_Para = new Hashtable();
  Hashtable dct_Key = new Hashtable();
  String m_usercode=(String)request.getRemoteUser();
  String m_username =oManager.getCurrentUserName();
  String m_ModuID = request.getParameter("modu_id");
  if (m_ModuID == null)
      m_ModuID = "";
  String DashBoardImageSmall = request.getParameter("DashBoardImageSmall");
  if (DashBoardImageSmall == null)
      DashBoardImageSmall = "";
  String title =request.getParameter("title");
  if (title == null)
      title = "";
  String Description =request.getParameter("Description");
  if (Description == null)
      Description = "";
  String DashBoardOrder = request.getParameter("DashBoardOrder");
  if (DashBoardOrder == null)
      DashBoardOrder = "";
  String DashBoardImageLarge = request.getParameter("DashBoardImageLarge");
  if (DashBoardImageLarge == null)
      DashBoardImageLarge = "";
  dct_Key.put("modu_id",m_ModuID);
  dct_Key.put("user_code",m_usercode);

  dct_Para.put("title",title);
  dct_Para.put("Description",Description);
  dct_Para.put("DashBoardOrder",PortalUtil.toDouble(DashBoardOrder));
  dct_Para.put("DashBoardImageSmall",DashBoardImageSmall);
  dct_Para.put("DashBoardImageLarge",DashBoardImageLarge);

  int sReturn = oSys.update(dct_Key,dct_Para,"pers_modu",false);
  dct_Para.clear();
  dct_Key.clear();
  if(sReturn >=0)
  {
  	response.sendRedirect("ModuleDetails.jsp?m_ModuID=" + m_ModuID + "&m_Operation=0");
  }
  else
  {
  	String ShowDBError ="updateError";
  }
  response.sendRedirect("ModuleDetails.jsp?m_ModuID=" + m_ModuID + "&m_Operation=0");
%>
