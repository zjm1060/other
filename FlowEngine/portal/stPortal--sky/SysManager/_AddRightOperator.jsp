<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page language="java" import="java.util.*,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
      String addtype=request.getParameter("addtype");
      String moduid = request.getParameter("modu_id");
      Vector userlist=(Vector)session.getAttribute("userlist");
      for (int i=0;i<userlist.size();i++){
        Hashtable dct_Key=new Hashtable();
        Hashtable dct_Para=new Hashtable();
        HashMap HMuser=(HashMap)userlist.get(i);
        String m_UserCode=HMuser.get("user_code").toString();
        int usert=new Integer(HMuser.get("user_type").toString()).intValue();
        String m_UserType=new Integer(usert-1).toString();
        dct_Key.put("user_code",m_UserCode);
        dct_Key.put("user_type",m_UserType);
        dct_Para.put("user_code",m_UserCode);
        dct_Para.put("user_type",PortalUtil.toDouble(m_UserType));
        String m_privilege=HMuser.get("right").toString();
        if (m_privilege.equals("yes")){
          dct_Para.put("allow_use",PortalUtil.toDouble("1"));
          dct_Para.put("deny_use",PortalUtil.toDouble("0"));
        }
        else{
          dct_Para.put("allow_use",PortalUtil.toDouble("0"));
          dct_Para.put("deny_use",PortalUtil.toDouble("1"));
        }
        if (addtype.equals("modu")){
          dct_Key.put("MODU_ID",moduid);
          dct_Para.put("MODU_ID",moduid);
          int sReturn = oSys.update(dct_Key,dct_Para,"modu_right",true);
        }
        else{
          dct_Key.put("FUNC_ID",moduid);
          dct_Para.put("FUNC_ID",moduid);
          int sReturn = oSys.update(dct_Key,dct_Para,"FUNC_RIGHT",true);
        }
      }
      session.removeAttribute("userlist");
%>
<script LANGUAGE="javascript">
<!--
function window_onload()
{
        window.returnValue=1;
        window.close();
}
window.attachEvent("onload",window_onload);
//-->

</script>
