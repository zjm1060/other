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
		<!-- link rel="stylesheet" type="text/css"
			href="/xMIS/resources/css/grid-examples.css" />
		<link rel="stylesheet" type="text/css"
			href="/xMIS/resources/css/examples.css" /-->

		<script type="text/javascript" src="/xMIS/resources/js/ext-base.js"></script>
		<script type="text/javascript" src="/xMIS/resources/js/ext-all.js"></script>
	  	<script type="text/javascript" src="/xMIS/resources/js/ext-lang-zh_CN.js"></script>      
		<script type="text/javascript"
			src="/xMIS/resources/js/SlidingPager.js"></script>
		<script type="text/javascript"
			src="/xMIS/resources/js/PanelResizer.js"></script>
		<script type="text/javascript"
			src="/xMIS/resources/js/PagingMemoryProxy.js"></script>

	</head>

	<body >
		<div id="grid_DeptTotalSummary"></div>
	</body>
</html>


<script> 
var gridTitle = "<%=request
									.getAttribute(WebKeys.Request_Attribute_Name_Grid_Title_Prefix)%>" + " 项目汇总";
	
Ext.onReady(function(){

    //Ext.state.Manager.setProvider(new Ext.state.CookieProvider());

    var myData = <%=request
									.getAttribute(WebKeys.Request_Attribute_Name_ProjListByDiv)%>;


	function nameLinker(val){    
		var text = val.substring(0, val.indexOf("^"));
		var url = val.substring(val.indexOf("^")+1, val.length);
		if(text != '0'){
			return '<a href="'+url+'">'+text+'</a>'   
		}
	　　else return text;    
　　}    
	

	function numberLinker(val){    
	　　　　if(typeof val=='object'){
				if(val.text != '0')    
					return val.text;
				else
		　　　　　　return '<a style="display:table;width:100%;" title="'+val.url+'" target="_blank" href="'+val.url+'">'+val.text+'</a>'   
	　　　　}    
	　　　　else return val;    
　　}    


    // create the data store
    var store = new Ext.data.SimpleStore({
    		proxy: new Ext.data.PagingMemoryProxy(myData),
        remoteSort:true,
        sortInfo: {field:'dept_name', direction:'ASC'},
        fields: [
           {name: 'dept_name'},
           {name: 'finished_count',type: 'char'},
           {name: 'unfinished_count',type: 'char'},
           {name: 'total_count',type: 'char'}
        ]
    });
    // manually load local data
    //store.loadData(myData);

    // create the Grid
    var grid = new Ext.grid.GridPanel({
       	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
        enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
        store: store,
        columns: [
             {id:'dept_name',header: '部门名称', width: 120, sortable: true, renderer: nameLinker, dataIndex: 'dept_name'},
            {header: '已完成项目数', width: 120, sortable: true, renderer: nameLinker, align: 'right', dataIndex: 'finished_count'},
            {header: '进行中项目数', width: 120, sortable: true, renderer: nameLinker, align: 'right', dataIndex: 'unfinished_count'},
            {header: '总项目数', width: 120, sortable: true, renderer: nameLinker, align: 'right', dataIndex: 'total_count'}
        ],
        stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
        autoExpandColumn: 'dept_name',
        height: 450,
        width: 600,
        title: gridTitle,
        // config options for stateful behavior
        stateful: true,
        stateId: 'grid',        
        
        //autoWidth: true,
        autoHeight: true,   
        loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   

        forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
        				
        // paging bar on the bottom
				loadMask: {msg:'正在加载数据，请稍侯……'},
        bbar: new Ext.PagingToolbar({
                pageSize: 20,
                store: store,
                displayInfo: true,
                displayMsg: '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
                emptyMsg: "无显示数据"
         })


    });
    
    // render the grid to the specified div in the page
    grid.render('grid_DeptTotalSummary');
    store.load({params:{start:0, limit:20}});

    
    
});
</script>

