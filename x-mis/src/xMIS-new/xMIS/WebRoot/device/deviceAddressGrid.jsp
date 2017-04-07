<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
		<style type="text/css">
			.x-tree-node-italic-red span {color:#CC0000!important;}
			.x-tree-node-italic-green span {color:green !important;}
			.x-grid3-row-over .x-grid3-cell-inner {
				font-weight: bold;
				color:'#545BED'
			}
		</style>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    <script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/ChooseField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/InklingSearchField.js"></script>
     <script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
    <script type="text/javascript" src="<%=path%>/device/deviceIn.js"></script>
       <script type="text/javascript" src="<%=path%>/resources/js/MultiSelect.js"></script>
    <script type="text/javascript" src="<%=path %>/crm/AddressChooseField.js"></script>
    
    <script type="text/javascript" src="<%=path%>/device/deviceAddressGrid.js"></script>
    <script type="text/javascript">
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
      物资存放地址信息列表
    </title>
  </head>
  <body>
  </body>
</html>

