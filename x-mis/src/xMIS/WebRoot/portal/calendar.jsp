<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>

<%
	String holidays = "01.01,01.02,01.03,"
		+ "02.13,02.14,02.15,02.16,02.17,02.18,02.19,"
		+ "04.03,04.04,04.05," 
		+ "05.01,05.02,05.03,"
		+ "06.14,06.15,06.16," 
		+ "09.22,09.23,09.24,"
		+ "10.01,10.02,10.03,10.04,10.05,10.06,10.07";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=gb2312">

		<LINK href="resources/css/styleCIS.css" type=text/css rel=stylesheet>

		<meta http-equiv="pragma" content="no-cache">
		<meta http-equiv="cache-control" content="no-cache">
		<meta http-equiv="expires" content="0">
		<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
		<meta http-equiv="description" content="This is my page">
		<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

		<link rel="STYLESHEET" type="text/css"
			href="resources/css/dhtmlxcalendar.css" />

		<script src="resources/js/dhtmlxcommon_calendar.js"></script>
		<script src="resources/js/dhtmlxcalendar.js"></script>

	</head>

	<BODY >

		<div id="dhtmlxcalendar" align=center></div>
		
		<!-- p><input type=button class=blue_button value="天气预报" /></p-->
		<!--iframe src="http://m.weather.com.cn/n/pn9/weather.htm" width="275"
			height="110" marginwidth="0" marginheight="0" hspace="0" vspace="0"
			frameborder="0" scrolling="no" style="font-size:10px"></iframe-->




		<script type="text/javascript">
			var mCal;
			mCal = new dhtmlxCalendarObject('dhtmlxcalendar', true, {isWinHeader: true, isWinDrag: false});
			mCal.setHolidays('<%=holidays%>');
		</script>

	</body>
</html>
