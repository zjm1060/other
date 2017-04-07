<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@include file="../jsCss.jsp"%>
<!DOCTYPE HTML>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>水文局上下水位折线图</title>

<script type="text/javascript" src="<%=path%>/resources/js/jquery.min.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/highcharts.js"></script>
<script type="text/javascript"
	src="<%=path%>/resources/js/highcharts-more.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/exporting.js"></script>
<script language="javascript" type="text/javascript"
	src="<%=path%>/resources/js/My97DatePicker/WdatePicker.js"></script>
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
		document.getElementById("begin").value=today.Format("yyyy-MM-dd");
	    document.getElementById("end").value = today.Format("yyyy-MM-dd");
	}
)

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
	var start = document.getElementById("begin").value+" "+document.getElementById("start_time").value;
	var end = document.getElementById("end").value+" "+document.getElementById("end_time").value;
	var xData = new Array();
	var y1Data = new Array();
	var y2Data = new Array();

						$
								.ajax({
									type : "POST",
									dataType : "JSON",
									url : __cxtPath
											+ '/gczlAction.do?method=getSWJWaterLevel&start_date='+start+'&end_date='+end,
									success : function(msg) {

										var total = msg;
										

										if (total != null
												&& total.rows.length != 0) {
											xData[0]=total.rows[0].measure_date;
											y1Data[0]=parseFloat(total.rows[0].ap_w_level);
											y2Data[0]=parseFloat(total.rows[0].bp_w_level);
											for ( var i = 1; i < total.rows.length; i++) {
												xData
														.push(total.rows[i].measure_date);
												y1Data
														.push(parseFloat(total.rows[i].ap_w_level));
												y2Data
														.push(parseFloat(total.rows[i].bp_w_level));
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
														text : '水文局上下水位折线图'
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
														 labels:{rotation : -45,y:50}
													},

													yAxis : [
															{
																title : {
																	text : '上下水位',
																	style : {
																		//	color : '#89A54E',
																			fontSize:25
																		}
																}
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
														} ]

												});

									}
								});

};
</script>

</head>
<body>
<div align="center" style="margin: 20px 0 0 0">
开始日期：<input name="begin" id="begin" style="width: 120px;"  onFocus="WdatePicker({readOnly:true,dateFmt:'yyyy-MM-dd'})" /> 
<select name="start_time" id="start_time">
<option value="00:00">00:00</option>
<option value="01:00">01:00</option>
<option value="02:00">02:00</option>
<option value="03:00">03:00</option>
<option value="04:00">04:00</option>
<option value="05:00">05:00</option>
<option value="06:00">06:00</option>
<option value="07:00">07:00</option>
<option value="08:00">08:00</option>
<option value="09:00">09:00</option>
<option value="10:00">10:00</option>
<option value="11:00">11:00</option>
<option value="12:00">12:00</option>
<option value="13:00">13:00</option>
<option value="14:00">14:00</option>
<option value="15:00">15:00</option>
<option value="16:00">16:00</option>
<option value="17:00">17:00</option>
<option value="18:00">18:00</option>
<option value="19:00">19:00</option>
<option value="20:00">20:00</option>
<option value="21:00">21:00</option>
<option value="22:00">22:00</option>
<option value="23:00">23:00</option>
<option  value="23:59">23:59</option>
</select>
结束日期：<input name="end" id="end" style="width:120px;"  onFocus="WdatePicker({readOnly:true,dateFmt:'yyyy-MM-dd'})" /> 
<select name="end_time" id="end_time">
<option value="00:00">00:00</option>
<option value="01:00">01:00</option>
<option value="02:00">02:00</option>
<option value="03:00">03:00</option>
<option value="04:00">04:00</option>
<option value="05:00">05:00</option>
<option value="06:00">06:00</option>
<option value="07:00">07:00</option>
<option value="08:00">08:00</option>
<option value="09:00">09:00</option>
<option value="10:00">10:00</option>
<option value="11:00">11:00</option>
<option value="12:00">12:00</option>
<option value="13:00">13:00</option>
<option value="14:00">14:00</option>
<option value="15:00">15:00</option>
<option value="16:00">16:00</option>
<option value="17:00">17:00</option>
<option value="18:00">18:00</option>
<option value="19:00">19:00</option>
<option value="20:00">20:00</option>
<option value="21:00">21:00</option>
<option value="22:00">22:00</option>
<option value="23:00">23:00</option>
<option  value="23:59" selected>23:59</option>
</select>
       <input type="button" value="查询" style="" onclick="show()"/>
</div>

<div id="container"
	style="min-width: 400px; height: 600px; margin: 0 auto"></div>


</body>
</html>