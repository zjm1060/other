<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@include file="../jsCss.jsp"%>
<!DOCTYPE HTML>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>上下水位和流量折线图</title>

<script type="text/javascript" src="<%=path%>/resources/js/jquery.min.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/highcharts.js"></script>
<script type="text/javascript"
	src="<%=path%>/resources/js/highcharts-more.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/exporting.js"></script>
<script language="javascript" type="text/javascript" src="<%=path%>/resources/js/My97DatePicker/WdatePicker.js"></script>
<script type="text/javascript">
$(
		
	function(){
		Date.prototype.Format = function(fmt)   
		{ //author: meizz   
		  var o = {   
		    "M+" : this.getMonth()+1,                 //月份   
		    "d+" : this.getDate(),                    //日   
		    "h+" : this.getHours(),                   //小时   
		    "m+" : this.getMinutes(),                 //分   
		    "s+" : this.getSeconds(),                 //秒   
		    "q+" : Math.floor((this.getMonth()+3)/3), //季度   
		    "S"  : this.getMilliseconds()             //毫秒   
		  };   
		  if(/(y+)/.test(fmt))   
		    fmt=fmt.replace(RegExp.$1, (this.getFullYear()+"").substr(4 - RegExp.$1.length));   
		  for(var k in o)   
		    if(new RegExp("("+ k +")").test(fmt))   
		  fmt = fmt.replace(RegExp.$1, (RegExp.$1.length==1) ? (o[k]) : (("00"+ o[k]).substr((""+ o[k]).length)));   
		  return fmt;   
		}  
		var today  = new Date();
		document.getElementById("begin").value=today.getFullYear()+"-01-01";
	    document.getElementById("end").value = today.Format("yyyy-MM-dd");
	}
);
	function show(){
		 Highcharts.setOptions({
	         lang: {
	            　 printChart:"打印图表",
	               downloadJPEG: "下载JPEG 图片" , 
	               downloadPDF: "下载PDF文档"  ,
	               downloadPNG: "下载PNG 图片"  ,
	               downloadSVG: "下载SVG 矢量图" , 
	               exportButtonTitle: "导出图片" 
	         }
	     });
		var chart;
		// var total;
		var start = document.getElementById("begin").value;
		var end = document.getElementById("end").value;
		var xData = new Array();
		var y1Data = new Array();
		var y2Data = new Array();
		var y3Data = new Array();

							$
									.ajax({
										type : "POST",
										dataType : "JSON",
										url : __cxtPath
												+ '/gczlAction.do?method=getWaterLevel&start_date='+start+'&end_date='+end,
										success : function(msg) {

											var total = msg;
											

											if (total != null
													&& total.rows.length != 0) {
												xData[0]=total.rows[0].measure_date;
												y1Data[0]=parseFloat(total.rows[0].ap_w_level);
												y2Data[0]=parseFloat(total.rows[0].bp_w_level);
												y3Data[0]=parseFloat(total.rows[0].flow_qty);
												for ( var i = 1; i < total.rows.length; i++) {
													xData
															.push(total.rows[i].measure_date);
													y1Data
															.push(parseFloat(total.rows[i].ap_w_level));
													y2Data
															.push(parseFloat(total.rows[i].bp_w_level));
													y3Data
															.push(parseFloat(total.rows[i].flow_qty));
													}
											}
											chart = new Highcharts.Chart(
													{

														chart : {
															renderTo : 'container', //图表放置的容器，DIV 
															defaultSeriesType : 'line', //图表类型line(折线图), 
															zoomType : 'x' //x轴方向可以缩放 
														},
														credits : {
															enabled : false
														//右下角不显示LOGO 
														},
														title : {
															text : '上下水位和流量折线图'
														},
														tooltip : {
															formatter : function() {

																return ''
																		+ this.x
																		+ ' 的值: '
																		+ this.y;
															}
														},
														plotOptions : {
															column : {
																pointPadding : 0.2,
																borderWidth : 0
															},
															 series: {
																	marker: {
												                        enabled: false
																		//radius: 10
												                    }
																 }
														},
														xAxis : {
															categories : xData,
															 labels:{rotation : -45,y:30}
														},

														yAxis : [
																{
																	
																	title : {
																		text : '上下水位',style : {
																		//	color : '#89A54E',
																			fontSize:25
																		}
																	}
																},
																{
																	title : {
																		text : '流量',
																		style : {
																		//	color : '#89A54E',
																			fontSize:25
																		}
																	},
																	opposite : true
																} ],
														series : [
																{
																	name : '上水位',
																	yAxis : 0,
																	data : y1Data 
																},{
																name : '下水位',
																yAxis : 0,
																data :y2Data 
															},
																{
																	name : '流量',
																	yAxis : 1,
																	data : y3Data
																} ]

													});

										}
									});

	};
</script>

</head>
<body>
<div align="center" style="margin:20px 0 0 0 ">
开始日期：<input name="begin" id="begin" style="width: 120px;"  onFocus="WdatePicker({readOnly:true,dateFmt:'yyyy-MM-dd'})" /> 结束日期：
       <input name="end" id="end" style="width:120px;"  onFocus="WdatePicker({readOnly:true,dateFmt:'yyyy-MM-dd'})" /> 
       <input type="button" value="查询" style="" onclick="show()"/>
</div>
<div id="container"
	style="min-width: 400px; height: 600px; margin: 0 auto"></div>

</body>
</html>