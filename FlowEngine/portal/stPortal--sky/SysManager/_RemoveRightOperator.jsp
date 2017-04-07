<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page language="java" import="java.util.*" %>
<%
      Vector userlist=(Vector)(session.getAttribute("userlist"));
      int usercount=userlist.size();
      if (request.getParameter("isdel").equals("yes")){
        String[] removeUC=request.getParameterValues("removeuc");
        if (removeUC!=null){
          for (int i=0;i<usercount;i++){
            HashMap temp=(HashMap)(userlist.get(i));
            String m_UserCode=temp.get("user_code").toString();
            String m_UserType=temp.get("user_type").toString();
            for (int j=0;j<removeUC.length;j++){
              String rUserCode=removeUC[j].substring(0,removeUC[j].indexOf("^"));
              String rUserType=removeUC[j].substring(removeUC[j].indexOf("^")+1,removeUC[j].length());
              if ((m_UserCode.equals(rUserCode))&&(m_UserType.equals(rUserType))){
                userlist.remove(temp);
                usercount--;
                i--;
              }
            }
          }
        }
      }
      else{
        for(int i=0;i<usercount;i++){
          String right=request.getParameter("privilege"+i);
          HashMap HMtemp=(HashMap)userlist.get(i);
          HMtemp.put("right",right);
        }
      }
      session.setAttribute("userlist",userlist);
%>
<script LANGUAGE="javascript">
<!--
function window_onload()
{
        window.location="AddUserList.jsp?addtype=<%=request.getParameter("addtype")%>";
}
window.attachEvent("onload",window_onload);
//-->

</script>
