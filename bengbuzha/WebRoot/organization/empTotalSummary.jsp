<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>

<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";
%>




<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3c.org/TR/1999/REC-html401-19991224/loose.dtd">
<HTML lang=en xml:lang="en" xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<base href="<%=basePath%>">

		<meta http-equiv="pragma" content="no-cache">
		<meta http-equiv="cache-control" content="no-cache">
		<meta http-equiv="expires" content="0">
		<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
		<meta http-equiv="description" content="This is my page">

		<link rel="stylesheet" type="text/css"
			href="/xMIS/resources/css/ext-all.css" />

		<script type="text/javascript" src="/xMIS/resources/js/ext-base.js"></script>
		<script type="text/javascript" src="/xMIS/resources/js/ext-all.js"></script>
	  	<script type="text/javascript" src="/xMIS/resources/js/ext-lang-zh_CN.js"></script>      


	</head>

	<body>
		<div id="grid_EmpTotalSummary" width="100%" ></div>
	</body>
</html>


<script> 
var gridTitle = "<%=request
									.getAttribute(WebKeys.Request_Attribute_Name_Grid_Title_Prefix)%>" + " 项目汇总";
Ext.onReady(function(){
	//Ext.state.Manager.setProvider(new Ext.state.CookieProvider()); 
	
	var store=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({url:'orgMaint.do?method=getDeptTotalSummaryStore&<%=WebKeys.Type_Department%>=<%=request.getAttribute(WebKeys.Type_Department)%>'}),
		remoteSort:true,
		reader:new Ext.data.XmlReader({
			totalProperty: 'TotalResults',
			record:"Item"},
		[
			"user_name",
			"proj_code",
			"proj_name",
			"start_date",
			"finished_task",
			"unfinished_task",
			"not_start_task"
		])
	});

	var colM=new Ext.grid.ColumnModel(
		[
		{header: "员工姓名",dataIndex:"user_name",sortable:true, renderer: biuldLinker},
		{header: "项目编码",dataIndex:"proj_code",sortable:true, renderer: biuldLinker},
		{header: "项目名称",dataIndex:"proj_name",sortable:true, renderer: biuldLinker},
		{header: "项目启动日期",dataIndex:"start_date",sortable:true, renderer: Ext.util.Format.dateRenderer('Y-m-d'),align: 'center'},
		{header: "已完成任务数",dataIndex:"finished_task",sortable:true, renderer: biuldLinker,align: 'right'},
		{header: "进行中任务数",dataIndex:"unfinished_task",sortable:true, renderer: biuldLinker,align: 'right'},
		{header: "未开始任务数",dataIndex:"not_start_task",sortable:true, renderer: biuldLinker,align: 'right'}
		]
	);
		
	var grid = new Ext.grid.GridPanel({
		renderTo:"grid_EmpTotalSummary",
		title:gridTitle,

		//height:Ext.get("grid_EmpTotalSummary").getHeight(),
		width:Ext.get("grid_EmpTotalSummary").getWidth(),
		cm:colM,
		store:store,
		autoExpandColumn:2,
		stripeRows: true,

      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: true,   
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
       				
		// paging bar on the bottom
       	bbar: new Ext.PagingToolbar({
			pageSize: 20,
			store: store,
			displayInfo: true,
			displayMsg: '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg: "无显示数据"
        })
	});
    
    store.load({params:{start:0, limit:20}});
    

	function biuldLinker(val) {
		if (val.indexOf("^") > 0) {
			var text = val.substring(0, val.indexOf("^"));
			var url = val.substring(val.indexOf("^") + 1, val.length);
			if (text != "0") {
				return "<a href=\"" + url + "\">" + text + "</a>";
			} else {
				return text;
			}
		} else {
			return val;
		}
	}

  
});


</script>

