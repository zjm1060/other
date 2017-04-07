<%@page import="java.util.*" pageEncoding="UTF-8"%>
<%
	String path = request.getContextPath();
	String basePath = path + "/";
	String processKey="workflow";
%>
<html>
<head>		
<title>设计流程</title>
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-all.css" />
<link rel="stylesheet" type="text/css" href="<%=basePath%>workflow/css/grapheditor.css" />
<script type="text/javascript">
    mxBasePath = '<%=basePath%>workflow';
	mxImageBasePath='<%=basePath%>workflow/images';
	mxCssBasePath='<%=basePath%>workflow/css';
	mxResBasePath='<%=basePath%>workflow';
	basePath = '<%=basePath%>';
	processKey='<%=processKey%>';
	processName='测试';
	_graphXml='<mxGraphModel>  <root>    <mxCell id="0"/>    <mxCell id="1" value="workflow" parent="0" name="测试"/>    <mxCell id="9" value="项目启动" style="start;image;image=images/start.png" vertex="1" parent="1">      <mxGeometry x="20" y="60" width="38" height="38" as="geometry"/>    </mxCell>    <mxCell id="11" value="" edge="1" parent="1" source="9" target="10">      <mxGeometry relative="1" as="geometry"/>    </mxCell>    <mxCell id="16" value="任务分配子流程" style="swimlane" vertex="1" parent="1">      <mxGeometry x="140" y="30" width="270" height="210" as="geometry">        <mxRectangle x="120" y="20" width="110" height="23" as="alternateBounds"/>      </mxGeometry>    </mxCell>    <mxCell id="10" value="选择接收部门/人" style="step;image;image=images/task.png" vertex="1" parent="16">      <mxGeometry x="30" y="30" width="58" height="38" as="geometry"/>    </mxCell>    <mxCell id="17" value="任务接收" style="step;image;image=images/task.png" vertex="1" parent="16">      <mxGeometry x="170" y="70" width="58" height="38" as="geometry"/>    </mxCell>    <mxCell id="18" value="" edge="1" parent="16" source="10" target="17">      <mxGeometry relative="1" as="geometry"/>    </mxCell>    <mxCell id="19" value="不接收" edge="1" parent="16" source="17" target="10">      <mxGeometry relative="1" as="geometry">        <Array as="points">          <mxPoint x="55.1824165948907" y="166.996262403102"/>        </Array>      </mxGeometry>    </mxCell>    <mxCell id="20" value="接收" edge="1" parent="1" source="17" target="21">      <mxGeometry relative="1" as="geometry">        <mxPoint x="540" y="120" as="targetPoint"/>      </mxGeometry>    </mxCell>    <mxCell id="21" value="结束" style="end;image;image=images/end.png" vertex="1" parent="1">      <mxGeometry x="500" y="100" width="38" height="38" as="geometry"/>    </mxCell>  </root></mxGraphModel>';
</script>
<script type="text/javascript" src="<%=path%>/dynamic.jsp"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-base.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-all-debug.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-lang-zh_CN.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-extra.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
<script type="text/javascript" src='<%=basePath%>workflow/js/graph-ie.js'></script>
<script type="text/javascript" src='<%=basePath%>workflow/js/ysGraph.js'></script>
<script type="text/javascript" src="<%=basePath%>workflow/js/GraphEditor.js"></script>
<script type="text/javascript" src="<%=basePath%>workflow/js/MainPanel.js"></script>
<script type="text/javascript" src="<%=basePath%>workflow/js/LibraryPanel.js"></script>
<script type="text/javascript" src="<%=basePath%>workflow/js/PropPanel.js"></script>
<script type="text/javascript" src="<%=basePath%>workflow/js/DiagramStore.js"></script>
<script type="text/javascript" src="<%=basePath%>workflow/js/DiagramPanel.js"></script>
	<script type="text/javascript">
        mxUtils.alert = function(message) {
            Ext.Msg.alert('', message);
        };
        
</script>
	</head>

	<body onload="main();GraphEditor.initData(); /* see js/GraphEditor.js */ ">
		<div id="header">
			<div style="float: right; margin: 5px;" class="x-small-editor"></div>
		</div>
	</body>
</html>
