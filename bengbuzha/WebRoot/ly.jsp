<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>

<%
 
	String swfFilePath="swf/Paper.swf";
 
	System.out.print(swfFilePath);
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<script type="text/javascript" src="resources/js/jquery.js"></script>
<script type="text/javascript" src="resources/js/flexpaper_flash.js"></script>
<script type="text/javascript" src="resources/js/flexpaper_flash_debug.js"></script>
<style type="text/css" media="screen"> 
			html, body	{ height:100%; }
			body { margin:0; padding:0; overflow:auto; }   
			#flashContent { display:none; }
        </style> 


<title>文档在线预览系统</title>
</head>
<body> 
 
<div style='padding-top:5px;padding-bottom: 5px;font-size:14px; '>提示：PDF格式文件的预览支持前20页，想看20页以上的文件的全部内容，请下载后查看。</div>
        <div id='chart' style="position:absolute;left:50px;top:30px;">
	        <a id="viewerPlaceHolder" style="width:820px;height:650px;display:block"></a>
	        
	        <script type="text/javascript"> 

				var fp = new FlexPaperViewer(	
						 'FlexPaperViewer',
						 'viewerPlaceHolder', { config : {
						 SwfFile : decodeURI('<%=swfFilePath%>'),
						 Scale : 0.6, 
						 ZoomTransition : 'easeOut',
						 ZoomTime : 0.5,
						 ZoomInterval : 0.2,
						 FitPageOnLoad : true,
						 FitWidthOnLoad : false,
						 FullScreenAsMaxWindow : false,
						 ProgressiveLoading : false,
						 MinZoomSize : 0.2,
						 MaxZoomSize : 5,
						 SearchMatchAll : false,
						 InitViewMode : 'SinglePage',
						 
						 ViewModeToolsVisible : true,
						 ZoomToolsVisible : true,
						 NavToolsVisible : true,
						 CursorToolsVisible : true,
						 SearchToolsVisible : true,
  						 localeChain: 'zh_CN'
						 }});
			 
			 
	        </script>            
        </div>
</body>
</html>

