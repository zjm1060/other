<%@page import="com.bitc.jk.xmis.util.WebUtil"%>
<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.model.CompanyObj"%>
<!DOCTYPE HTML>
<%
CompanyObj company=(CompanyObj)request.getAttribute("company");
%>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <%
			boolean isAdmin = false;
			boolean isDirectDeptAdmin = false;
			int currentUserID = ((EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA))
					.getUserID();
			if (currentUserID == -1 || currentUserID == 605 || currentUserID == 614 ) {
				isAdmin = true;
			}
			//isDirectDeptAdmin 事业部级权限
			if (currentUserID == 153 || currentUserID == 875) {
				isDirectDeptAdmin = true;
			}
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
			var isAdmin= <%=isAdmin%>;
			var isDirectDeptAdmin= <%=isDirectDeptAdmin%>;
			var currentUserID = <%=currentUserID%>;
			var ht = Ext.getBody().getHeight();
			var wd = Ext.getBody().getWidth();
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
    
    <script type="text/javascript" src="<%=path%>/authorize/authorize.js"></script>
    <script type="text/javascript">
      <%
      if(company!=null){
      	out.print("var company = " +company.toJsonString()+";");
      } else {
    	out.print("var company = null;");
      }
      %>
    </script>
    
        <style type=text/css>
        /* style for the "buy" ActionColumn icon */
        .buy-col {
            height: 16px;
            width: 16px;
            background-image: url(../resources/images/split-false.gif);
        }

        /* style for the "alert" ActionColumn icon */
        .alert-col {
            height: 16px;
            width: 16px;
            background-image: url(../resources/images/close.gif);
        }

    </style>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
  </body>
</html>

