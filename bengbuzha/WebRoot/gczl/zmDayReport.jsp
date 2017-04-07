<%@ page import="com.bitc.jk.xmis.util.WebUtil" %>
<%--
  Created by IntelliJ IDEA.
  User: zsz
  Date: 2017/1/12
  Time: 9:56
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<%
    if ("处领导".equals(WebUtil.getCurrentEmployee().getDepartmentName()) || "工程科".equals(WebUtil.getCurrentEmployee().getDepartmentName())) {
%>
<head>
    <title></title>
    <script type="text/javascript" src="jquery_ui/jquery-1.10.2.min.js"></script>
    <link rel="stylesheet" type="text/css" href="bootstrap/css/bootstrap.min.css">
    <script type="text/javascript" src="bootstrap/js/bootstrap.min.js"></script>
    <link rel="stylesheet" href="jquery_ui/jquery-ui.min.css">
    <script src="jquery_ui/jquery-ui.min.js"></script>
    <script src="jquery_ui/jquery-ui-timepicker-addon.js" type="text/javascript"></script>
    <link href="jquery_ui/jquery-ui-timepicker-addon.css" type="text/css"/>
    <script src="jquery_ui/jquery.ui.datepicker-zh-CN.js" type="text/javascript" charset="UTF-8"></script>
    <script src="jquery_ui/jquery-ui-timepicker-zh-CN.js" type="text/javascript" charset="UTF-8"></script>
    <script>
        var __path = "<%=request.getContextPath()%>";
        Date.prototype.Format = function (fmt) { //author: meizz
            var o = {
                "M+": this.getMonth() + 1, //月份
                "d+": this.getDate(), //日
                "h+": this.getHours(), //小时
                "m+": this.getMinutes(), //分
                "s+": this.getSeconds(), //秒
                "q+": Math.floor((this.getMonth() + 3) / 3), //季度
                "S": this.getMilliseconds() //毫秒
            };
            if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
            for (var k in o)
                if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
            return fmt;
        }
        function onQuery(){
            var queryDate = $('#queryDate').val() ? new Date($('#queryDate').val()).Format("yyyy-MM-dd") : new Date().Format("yyyy-MM-dd");
            $.post(__path + '/applyManageMaint.do?method=queryzmDayReportData', {
                currentDate: queryDate
            }, function (data) {
                if (data.success) {
                    var obj = JSON.parse(data.attributes.data);
                    var liuliang = obj['liuliang'];
                    var shuiwei= obj['shuiwei'];
                    var kaidu= obj['kaidu'];

                    $('.table tr:eq(2) input').each(function(){
                        $(this).val(liuliang[$(this).attr('attr')]);
                    })
                    $('.table tr:eq(3) input,.table tr:eq(4) input').each(function(){
                        $(this).val(shuiwei[$(this).attr('attr')]);
                    })
                    $('.table').find('.zhamen').each(function(){
                        $(this).val(kaidu[$(this).attr('attr')]);
                    })
                } else {
                    alert(data.message);
                }
            });
        }
        function onExport(){
            var queryDate = $('#queryDate').val() ? new Date($('#queryDate').val()).Format("yyyy-MM-dd") : new Date().Format("yyyy-MM-dd");
            if($('#queryDate').val()){
                queryDate = new Date($('#queryDate').val()).Format("yyyy-MM-dd");
            }
            window.open(__path + '/applyManageMaint.do?method=exportzmDayReportData&currentDate='+queryDate);
        }
        function loadDiv(url){
            window.open(__path+url,"_self");
        }
        $(function () {
            onQuery();
            $('#queryDate').datepicker({
                dateFormat: "yy-mm-dd",
                maxDate: "+0M +0D",
                changeYear: true
            });
        })
    </script>
    <style>
        td {
            text-align: center;
            vertical-align:middle;
        }
        input {
            width: 50px;
            text-align: center;
            border:0px;
        }
    </style>
</head>
<body>
<div>
    <div class="container">
        <div class="row" style="margin-top: 20px;">
            <div class="col-md-12">
                <nav class="navbar navbar-default" role="navigation">
                    <div class="container-fluid">
                        <div class="navbar-header">
                            <a class="navbar-brand" href="#"><strong>蚌埠闸管理系统</strong></a>
                        </div>
                        <div>
                            <ul class="nav navbar-nav">
                                <li><a href="#"onclick="loadDiv('/gczl/shangxiashuiweiliuliang.jsp')">水位流量折线图</a></li>
                                <li><a href="#" onclick="loadDiv('/gczl/doDayReport.jsp')">闸门控制系统报表</a></li>
                                <li class="active"><a href="#" onclick="loadDiv('/gczl/zmDayReport.jsp')">闸门控制系统日报表</a></li>
                            </ul>
                        </div>
                    </div>
                </nav>
            </div>
        </div>
        <div class="row" style="margin-top: 20px;">
            <form class="form-inline" role="form">
                <div class="form-group form-group-md">
                    <label class="control-label" for="queryDate">查询日期</label>
                    <input type="text" class="form-control datepicker queryDate" id="queryDate" readonly="readonly">
                </div>
                <button type="button" class="query_btn btn btn-primary btn-md" onclick="onQuery()">查询</button>
                <button type="button" class="save_btn btn btn-success btn-md" onclick="onExport()">导出Excel</button>
            </form>
        </div>
    </div>
    <div class="row" style="margin-top: 10px;">
        <div class="col-md-12">
         <table class="table table-striped table-bordered table-hover table-condensed">
            <tr>
                <td colspan="26"><h3><strong>蚌埠闸闸门控制系统日报表</strong></h3></td>
            </tr>
            <tr>
                <td colspan="2"></td>
                <td><strong>0时</strong></td>
                <td><strong>1时</strong></td>
                <td><strong>2时</strong></td>
                <td><strong>3时</strong></td>
                <td><strong>4时</strong></td>
                <td><strong>5时</strong></td>
                <td><strong>6时</strong></td>
                <td><strong>7时</strong></td>
                <td><strong>8时</strong></td>
                <td><strong>9时</strong></td>
                <td><strong>10时</strong></td>
                <td><strong>11时</strong></td>
                <td><strong>12时</strong></td>
                <td><strong>13时</strong></td>
                <td><strong>14时</strong></td>
                <td><strong>15时</strong></td>
                <td><strong>16时</strong></td>
                <td><strong>17时</strong></td>
                <td><strong>18时</strong></td>
                <td><strong>19时</strong></td>
                <td><strong>20时</strong></td>
                <td><strong>21时</strong></td>
                <td><strong>22时</strong></td>
                <td><strong>23时</strong></td>
            </tr>
            <tr>
                <td colspan="2" style="vertical-align:middle;"><strong>流量</strong></td>
                <td><input type="text" attr="ling" readonly="readonly"/></td>
                <td><input type="text" attr="yi" readonly="readonly"/></td>
                <td><input type="text" attr="er" readonly="readonly"/></td>
                <td><input type="text" attr="san" readonly="readonly"/></td>
                <td><input type="text" attr="si" readonly="readonly"/></td>
                <td><input type="text" attr="wu" readonly="readonly"/></td>
                <td><input type="text" attr="liu" readonly="readonly"/></td>
                <td><input type="text" attr="qi" readonly="readonly"/></td>
                <td><input type="text" attr="ba" readonly="readonly"/></td>
                <td><input type="text" attr="jiu" readonly="readonly"/></td>
                <td><input type="text" attr="shi" readonly="readonly"/></td>
                <td><input type="text" attr="shiyi" readonly="readonly"/></td>
                <td><input type="text" attr="shier" readonly="readonly"/></td>
                <td><input type="text" attr="shisan" readonly="readonly"/></td>
                <td><input type="text" attr="shisi" readonly="readonly"/></td>
                <td><input type="text" attr="shiwu" readonly="readonly"/></td>
                <td><input type="text" attr="shiliu" readonly="readonly"/></td>
                <td><input type="text" attr="shiqi" readonly="readonly"/></td>
                <td><input type="text" attr="shiba" readonly="readonly"/></td>
                <td><input type="text" attr="shijiu" readonly="readonly"/></td>
                <td><input type="text" attr="ershi" readonly="readonly"/></td>
                <td><input type="text" attr="eryi" readonly="readonly"/></td>
                <td><input type="text" attr="erer" readonly="readonly"/></td>
                <td><input type="text" attr="ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td colspan="2" style="vertical-align:middle;"><strong>上游水位</strong></td>
                <td><input type="text" attr="shangyou_ling" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_yi" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_er" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_san" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_si" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_wu" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_liu" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_qi" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_ba" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_jiu" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shi" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shiyi" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shier" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shisan" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shisi" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shiwu" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shiliu" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shiqi" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shiba" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_shijiu" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_ershi" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_eryi" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_erer" readonly="readonly"/></td>
                <td><input type="text" attr="shangyou_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td colspan="2" style="vertical-align:middle;"><strong>下游水位</strong></td>
                <td><input type="text" attr="xiayou_ling" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_yi" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_er" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_san" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_si" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_wu" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_liu" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_qi" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_ba" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_jiu" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shi" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shiyi" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shier" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shisan" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shisi" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shiwu" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shiliu" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shiqi" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shiba" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_shijiu" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_ershi" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_eryi" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_erer" readonly="readonly"/></td>
                <td><input type="text" attr="xiayou_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td rowspan="12" style="vertical-align:middle;"><strong>新闸闸门开度</strong></td>
                <td><strong>1#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_1_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_1_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>2#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_2_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_2_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>3#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_3_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_3_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>4#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_4_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_4_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>5#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_5_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_5_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>6#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_6_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_6_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>7#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_7_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_7_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>8#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_8_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_8_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>9#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_9_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_9_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>10#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_10_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_10_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>11#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_11_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_11_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>12#孔</strong></td>
                <td><input type="text" class="zhamen" attr="new_12_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="new_12_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td rowspan="28" style="vertical-align:middle;"><strong>老闸闸门开度</strong></td>
                <td><strong>1#孔</strong></td>
                <td><input type="text" class="zhamen" attr="old_1_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_1_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>2#孔</strong></td>
                <td><input type="text" class="zhamen" attr="old_2_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_2_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>3#孔</strong></td>
                <td><input type="text" class="zhamen" attr="old_3_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_3_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_3_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_3_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen" attr="old_3_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_3_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>4#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_4_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_4_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>5#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_5_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_5_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>6#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_6_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_6_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>7#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_7_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_7_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>8#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_8_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_8_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>9#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_9_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_9_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>10#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_10_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_10_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>11#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_11_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_11_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>12#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_12_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_12_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>13#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_13_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_13_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>14#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_14_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_14_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>15#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_15_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_15_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>16#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_16_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_16_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>17#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_17_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_17_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>18#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_18_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_18_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>19#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_19_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_19_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>20#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_20_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_20_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>21#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_21_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_21_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>22#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_22_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_22_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>23#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_23_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_23_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>24#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_24_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_24_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>25#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_25_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_25_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>26#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_26_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_26_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>27#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_27_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_27_ersan" readonly="readonly"/></td>
            </tr>
            <tr>
                <td><strong>28#孔</strong></td>
                <td><input type="text" class="zhamen"  attr="old_28_ling" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_yi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_er" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_san" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_si" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_wu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_liu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_qi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_ba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_jiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shiyi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shier" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shisan" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shisi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shiwu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shiliu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shiqi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shiba" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_shijiu" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_ershi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_eryi" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_erer" readonly="readonly"/></td>
                <td><input type="text" class="zhamen"  attr="old_28_ersan" readonly="readonly"/></td>
            </tr>
        </table>
        </div>
    </div>
</div>
</body>
<%
    } else {
%>
<body>
<div class="container row">
    <div style="margin-top: 300px;text-align: center" class="col-md-6 col-md-offset-3"><h1>该功能仅对处领导与工程科公开！</h1></div>
</div>
</body>
<%
    }
%>
</html>
