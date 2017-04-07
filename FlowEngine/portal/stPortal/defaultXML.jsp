<%@ page contentType="text/html; charset=gb2312" %>
<%@ page import="com.minstone.portal.util.XMLWriter"%>
<%@ page import="com.minstone.security.Security"%>
<%@ page import="java.io.StringReader"%>
<%@ page import="java.net.*"%>
<jsp:useBean id="dbutil" scope="page" class="jbportal.DBUtilBean" />
<jsp:useBean id="run" scope="page" class="jbportal.runBean"/>
<%
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
    try{
        String l_sShow = run.getAllModuleXML();
        out.println(l_sShow);
    }catch(Exception e){
        out.write( L_ERRORHEADING_TEXT + "<BR>");
        if (e.getMessage().equalsIgnoreCase(""))
             out.write( e.getMessage() + "<BR>");
        else
             out.write( L_ERRORINTERNAL_TEXT + "<BR>");
      }
%>
