<%@page import="com.bitc.jk.xmis.model.SimpleDeptObj"%>
<%@page import="com.bitc.jk.xmis.util.JSONUtils"%>
<%@page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.web.SpringInit"%>

<%@page import="com.bitc.jk.xmis.dao.MissAttendanceDAO"%>
<%@page import="com.bitc.jk.xmis.dao.OrganizationDAO"%>
<%@page import="com.bitc.jk.xmis.model.SimpleEmployee"%>
<%@page import="com.bitc.jk.xmis.util.Type"%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
 
	<script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
	<script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreeChooseField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Draft.js"></script>
    <script type="text/javascript" src="<%=path%>/workflow/js/TodoInfoPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/kinetic-v4.0.0.min.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/Workflow.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/RowEditor.js"></script>
	    <script type="text/javascript" src="<%=path%>/resources/js/Draft.js"></script>
    <script type="text/javascript" src="<%=path%>/attendance/extraInfo.js"></script>
    <script type="text/javascript" src="<%=path%>/attendance/missAttendanceReturn.js"></script>
    
        <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/ItemSelector.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
    
    <title>
 	考勤单表单
    </title>
    <%
    MissAttendanceDAO missAttendanceDAO = (MissAttendanceDAO)SpringInit.getBean("missAttendanceDAO");
    String P_LASTDAY = missAttendanceDAO.getValueFromParamTableByName(WebKeys.P_LASTDAY);
    
    OrganizationDAO organizationdao=(OrganizationDAO)SpringInit.getBean("organizationDAOImpl");
    int applicantID = Type.getInteger(request.getParameter("applicantID"));
    SimpleEmployee selectedEmployee = null;
    int selectedDeptID  = 0 ;
    SimpleDeptObj selectedDept = null;
    
    if(applicantID!=0){
    	selectedEmployee =  organizationdao.getSimpleEmployee(applicantID);
    	selectedDeptID  = organizationdao.getDeptIDByUserID(applicantID);
    	selectedDept = organizationdao.getSimpleDeptObj(selectedDeptID);
    }
    
    %>
     <script type="text/javascript">
     var MIN_DATE= com.bitc.xmis.attd.getMin(<%=P_LASTDAY%>);
      <%
	out.print("var lastday ="+ P_LASTDAY+";");
      
      
     if(selectedEmployee!=null){
       	out.print("var selectedEmployee = " + selectedEmployee.toJsonString()+";");
       } else {
       	out.print("var selectedEmployee = null;" );
       }
     if(selectedDept!=null){
       	out.print("var selectedDept = " + selectedDept.toJsonString()+";");
       } else {
       	out.print("var selectedDept = null;" );
       }
      %>
      
    </script>
       <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"request");
    %>
  </head>
  <body>
  
  </body>
</html>

