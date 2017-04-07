<%@ page language="java" contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="fireflow" uri="/fireflow-tags"%>

<!DOCTYPE HTML>
<html>
<head>
<title>流程跟踪测试</title>
<%@include file="../jsCss.jsp"%>
<script type="text/javascript" src="../resources/js/kinetic-v4.0.0.min.js"></script>
<script type="text/javascript" src="../resources/js/Workflow.js"></script>
<style type="text/css">
body {
	width: 100%;
	height: 100%;
}
</style>
</head>
<body>
<SCRIPT LANGUAGE="JavaScript">
	Ext.onReady(function() {
		new Ext.Viewport({layout:'border',
			items:[new com.bitc.xmis.workflow.TracePanel({
			region:'center',
			processInstanceId : '402981d838ffc2bf0138ffc88a7c0005'
		})]});
		/*
		var win=new Ext.Window({
			width:600,height:300,closeAction:'hide',
			layout:'border',
			items:new com.bitc.xmis.workflow.Html5TracePanel({region:'center',processId : 'StartProject'})
		});
win.show();
*/
	});
</SCRIPT>

<!-- table width='98%' height='600' cellspacing='0' cellpadding='0'>
	<tr>
		<td valign='top'><object id='fireflowInstanceTrace'
			classid='clsid:D27CDB6E-AE6D-11cf-96B8-444553540000'
			codebase='http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab'
			height="600" width="700">
			<param name='src' value='FireflowInstanceTrace.swf' />
			<param name='flashVars'
				value='endpoint=messagebroker/amf&processInstanceId=40288fd8389df2f001389df67b480014&taskDoubleClick=true&activityDoubleClick=true' />
			<embed name='fireflowInstanceTrace' src='FireflowInstanceTrace.swf'
				pluginspage='http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash'
				flashVars='endpoint=messagebroker/amf&processInstanceId=40288fd8389df2f001389df67b480014&taskDoubleClick=true&activityDoubleClick=true' />
		</object></td>
	</tr>

</table-->

</body>
</html>