<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="com.minstone.portal.util.XMLWriter"%>
<%@ page import="com.minstone.security.Security"%>
<%@ page import="java.io.StringReader"%>
<%@ page import="java.net.*"%>
<%
String rootpath = request.getContextPath();

%>


<jsp:useBean id="dbutil" scope="page" class="jbportal.DBUtilBean" />
<jsp:useBean id="run" scope="page" class="jbportal.runBean"/>
<HTML>
<body>

<link rel="stylesheet" type="text/css"	href="<%=rootpath%>/stPortal/Common/Css/IM.css">
<script language="javascript" src="<%=rootpath%>/stPortal/Common/Include/IM.js"></script>
<script language="javascript" src="./include/HttpSubmit.js" ></script>
<script language="vbscript" src="./include/encode.vbs" ></script>
<SCRIPT LANGUAGE="JavaScript" src="./include/newMsgTips.js"></SCRIPT>

<%
  	String AUTH_TYPE;
  	String LOGON_USER;
  	String AUTH_PASSWORD;
  	String URL;
  	String SERVER_NAME;
  	String xsl_filename;

    StringBuffer   sDashboard;
    String strWebPartCC = "urn:schemas-microsoft-com:webpart";
    String L_ERRORHEADING_TEXT = "<B>Dashboard Error</B>";
    String L_ERRORINTERNAL_TEXT = "An internal error occurred.";
    String L_ERRORDELETEPERMISSIONS_TEXT = "You do not have sufficient permissions to delete this item from the dashboard. Please contact your system administrator for more assistance.";
    String L_ERRORWEBPARTPERMISSIONS_TEXT = "You do not have sufficient permissions to add a web part to this dashboard. Please contact your system adminsistrator for more assistance.";
    String L_ERRORINVALIDDASHBOARD_TEXT = "The dashboard you have selected does not exist.";
    String L_ERRORCATALOGPERMISSIONS1_TEXT = "You do not have sufficient permissions to add the part, ";
    String L_ERRORCATALOGPERMISSIONS2_TEXT = " to the dashboard:";
    String L_ERRORCATALOGFOLDER_TEXT = "Failed to create the catalog folder: ";
    String varDashboardID;
    long   vardashboardType=0;
    String varTemp;
    String stPartCmd;
    String stPartID;
    String nodeTmp;
    String stXMLPartUpdate;
    String xmlPartUpdate;

    //store.jsp的定义
    String stUser;
    String stPwd;

    String xmlhttp;
    //init初始化相关变量Begin...
    AUTH_TYPE = request.getAuthType();
    LOGON_USER = request.getRemoteUser();
    URL = request.getRequestURI();
    SERVER_NAME = request.getServerName();
    xsl_filename = "./Dashboard.xsl";
    //init初始化相关变量End
    try{
        //Retrieve the dashboard identifier from the QueryString.
        varDashboardID =jbportal.PortalUtil.getValue(request.getParameter("DashboardID"));
        /*
        if (varDashboardID.equals(""))
           varDashboardID = "0001";
        */
        varTemp =jbportal.PortalUtil.getValueN(request.getParameter("DashboardType"));
        if(varTemp.equals("")) varTemp="0";
        vardashboardType = Integer.valueOf(varTemp).longValue();
        sDashboard = new StringBuffer(jbportal.PortalUtil.StoreGetRenderInfo(run,varDashboardID , vardashboardType));
        sDashboard = dbutil.AppendOtherXMLDOM(sDashboard);
        String l_sShow = jbportal.PortalUtil.CreateDashboardView(varDashboardID,sDashboard,URL,SERVER_NAME);
        //System.out.println("l_sShow="+l_sShow);

        String xsl =application.getRealPath("//stPortal//Dashboard.xsl");
        StringReader reader=new StringReader(l_sShow);
        jbportal.PortalUtil.convertToHtml(reader,xsl,out);
        reader.close();
        reader=null;
    }catch(Exception e){
        out.write( L_ERRORHEADING_TEXT + "<BR>");
        if (e.getMessage().equalsIgnoreCase(""))
             out.write( e.getMessage() + "<BR>");
        else
             out.write( L_ERRORINTERNAL_TEXT + "<BR>");
      }
%>
<script>
if(document.URL.substring(0,25)=="http://192.168.8.9:6001/;"||document.URL=="http://192.168.8.9:6001/")
{
window.location="http://192.168.8.9:6001/stPortal/default.jsp";
}
</script>

</body>
</html>
