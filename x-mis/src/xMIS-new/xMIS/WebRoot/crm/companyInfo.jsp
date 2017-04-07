<%@page import="com.bitc.jk.xmis.util.JSONUtils"%>
<%@page import="com.bitc.jk.xmis.util.WebUtil"%>
<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.model.CompanyObj"%>
<%@page import="com.bitc.jk.xmis.model.SupplierInformation" %>
<%@page import="com.bitc.jk.xmis.model.QualificationObj" %>
<%@page import="com.bitc.jk.xmis.model.PatentInformation" %>
<!DOCTYPE HTML>
<%
CompanyObj company=(CompanyObj)request.getAttribute("company");

%>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <%
			int currentUserID = ((EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA))
					.getUserID();
		%>
		<style type="text/css">
			.x-tree-node-italic-red span {color:#CC0000!important;}
			.x-tree-node-italic-green span {color:green !important;}
			.x-grid3-row-over .x-grid3-cell-inner {
				font-weight: bold;
				color:'#545BED'
			}
		</style>
		<script type="text/javascript">
			var currentUserID = <%=currentUserID%>;
			var ht = Ext.getBody().getHeight();
			var wd = Ext.getBody().getWidth();
			
			
			var isAdmin = com.bitc.xmis.existRole(__emp.roles, 20);//公司级组织树编辑权限
			var isDirectDeptAdmin = com.bitc.xmis.existRole(__emp.roles, 21);//事业部级编辑权限
			var isKZB = com.bitc.xmis.existRole(__emp.roles, 22);//科技质量部
			var isAdminUser = com.bitc.xmis.existRole(__emp.roles, 2);//系统管理员
		</script>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    <script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/workflow/js/TodoInfoPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/device/DeviceGridPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/ItemSelector.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Attention.js"></script>
    
    <script type="text/javascript" src="<%=path%>/resources/js/MultiSelect.js"></script>
    <script type="text/javascript" src="<%=path %>/crm/AddressChooseField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/conInfoObjCom.js"></script>
    
    <script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/companyTree.js"></script>
    
    
    <script type="text/javascript" src="<%=path%>/resources/js/ChooseField.js"></script>
    <script type="text/javascript" src="<%=path%>/crm/companyInfo.js"></script>
    <script type="text/javascript">
      <%
      if(company!=null){
      	out.print("var company = " +company.toJsonString()+";");
      } else {
    	out.print("var company = null;");
      }
      Map map=(Map)request.getAttribute("sition");
      if(map != null){
    	  out.print("var map = " +JSONUtils.toJSON(map, true).toString()+";");
      } else {
    	  out.print("var map = null;");
      }
      Map qualifica=(Map)request.getAttribute("qualificamap");
      if(qualifica != null){
    	  out.print("var qualifica = " +JSONUtils.toJSON(qualifica, true).toString()+";");
      } else {
    	  out.print("var qualifica = null;");
      }
      Map patent =(Map)request.getAttribute("patentmap");
      if(patent != null){
    	  out.print("var patent = " +JSONUtils.toJSON(patent, true).toString()+";");
      } else {
    	  out.print("var patent = null;");
      }
      %>
    </script>
    <%WebUtil.outputRequestParameterAsScript(request,out,"request"); %>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
  </body>
</html>

