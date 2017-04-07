<%@ page contentType="text/html; charset=GBK" %>
<html>
<head>
<title>
系统设置
</title>
</head>
<link rel="STYLESHEET" type="text/css" href="../include/tree/css/WebFX-ContextMenu.css">
<link rel="STYLESHEET" type="text/css" href="../include/tree/css/Node.css">
<link rel="STYLESHEET" type="text/css" href="../css/WebToolBar/toolbar.css">
<script type="text/javascript" src="../include/webtoolbar.js"></script>
<script type="text/javascript" src="../include/tree/ContextMenu.js"></script>
<script language='javascript'>
function toLookAndFeel(){
	window.main.location="LookAndFeelMain.jsp";
}
function toSystemPara(){
	window.main.location="SystemParameterMain.jsp";
}
</script>
<body leftmargin="0" topmargin="0">
<table border="0" cellspacing="0" cellpadding="0" height="100%" width="100%">
<tr>
	<td width="100%">
		<table border="1" width="100%" height="100%">
			<tr>
				<td>
					<div id="toolbar"></div>
					<script type="text/javascript">
						SetToolBarHeight(40);
						AddToolBarItemEx("lookAndFeel","../images/refresh.gif","","","系统外观","60","0","toLookAndFeel()");
                        AddToolBarItemEx("systemPara","../images/go.gif","","","系统参数","60","0","toSystemPara()");
						window.document.getElementById("toolbar").innerHTML=GenToolBarV();
					</script>
				</td>
			</tr>
			<tr height="100%">
				<td>
					<iframe name="main" FRAMEBORDER=0 width=100% height=100% src="">
					</iframe>
					<iframe name="update" width=0 height=0 src="">
					</iframe>
				</td>
			</tr>
		</table>
	</td>

</tr>
</table>
</body>
</html>
