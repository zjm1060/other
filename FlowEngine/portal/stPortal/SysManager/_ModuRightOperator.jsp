<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page language="java" import="java.util.Hashtable,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
      int usercount=new Integer(request.getParameter("usercount")).intValue();
      String m_ModuID = request.getParameter("modu_id");
      if (request.getParameter("operator").equals("ok")){
        for (int i=0;i<usercount;i++){
          Hashtable dct_Para = new Hashtable();
          Hashtable dct_Key = new Hashtable();

          String m_UserCode=request.getParameter("user_code"+i);
          dct_Key.put("modu_id",m_ModuID);
          dct_Key.put("user_code",m_UserCode);
          dct_Key.put("user_type",request.getParameter("user_type"+i).toString());
          String m_privilege=request.getParameter("privilege"+i);
          if (m_privilege.equals("yes")){
            dct_Para.put("allow_use",PortalUtil.toDouble("1"));
            dct_Para.put("deny_use",PortalUtil.toDouble("0"));
          }
          else{
            dct_Para.put("allow_use",PortalUtil.toDouble("0"));
            dct_Para.put("deny_use",PortalUtil.toDouble("1"));
          }
          int sReturn = oSys.update(dct_Key,dct_Para,"MODU_RIGHT",false);
        }
      }
      else{
        String[] removeUC=request.getParameterValues("removeuc");
        if (removeUC!=null)
          for (int i=0;i<removeUC.length;i++){
            Hashtable dct_Key = new Hashtable();
            String user_code=removeUC[i].substring(0,removeUC[i].indexOf("^"));
            String user_type=removeUC[i].substring(removeUC[i].indexOf("^")+1,removeUC[i].length());
            dct_Key.put("MODU_ID",m_ModuID);
            dct_Key.put("USER_CODE",user_code);
            dct_Key.put("USER_TYPE",user_type);
            int sReturn = oSys.Delete(dct_Key,"MODU_RIGHT");
          }
      }
      response.sendRedirect("moduRightList.jsp?modu_id=" + m_ModuID );
%>
