<%@ page contentType="text/html; charset=gb2312" errorPage="/stPortal/include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.*,suntek.portal.publicLib.RecordSetObject" %>
<%@ page import ="java.util.*"%>
<%@ page import ="java.sql.*"%>
<%@ page import ="oamodule.publiclib.DBManager"%>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%! private DBManager dbManager = null;
%>
<%! public void initDB(String userCode) throws Exception
    {
    if(dbManager==null)
     	dbManager = new DBManager(userCode);

    }
%>

<%

    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();

    Connection cn = null;
    String u_password = null;
            try{
                    initDB(m_usercode);
                    if(dbManager == null)
                            System.out.println("can't get dbManager");
                    cn = dbManager.getConnection();
                    if(cn == null)
                            System.out.println("can't get db connect");
                    Statement stm = cn.createStatement();
                    String sql = "select * from dbwizard.users where U_NAME='" + m_usercode+"'";

                    ResultSet rs=stm.executeQuery(sql);

                    while(rs.next())
                    {
                             u_password = rs.getString("U_PASSWORD");
                    }
  		    rs.close();
                    cn.close();
		    rs=null;
   		    cn=null;
            }catch(Exception e)
            {
                    e.printStackTrace();
                    try{
                    cn.close();
                    }catch(Exception e1){}

            }
    System.out.println(m_usercode+m_username+u_password);

%>


<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<script language="javascript">
function initWindow(){
      
      var m_usercode = "<%=m_usercode%>";
      var m_username = "<%=m_username%>";
      var u_password = "<%=u_password%>";
      alert(m_usercode+m_username+u_password);  //取得用户信息
      var URL="XXX.asp?USERCODE="+m_usercode+"&PASSWORD="+u_password;  //传递给asp
      alert(URL);
      window.location = URL;
}
</script>
</head>
<body onload="javascript:initWindow();">
</body>
</html>



