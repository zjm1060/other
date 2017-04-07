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
    <script src="jquery_ui/jquery.bootstrap.min.js" type="text/javascript" charset="UTF-8"></script>
    <script>
        var __path = "<%=request.getContextPath()%>";
        var trHtml = '<tr>' +
                '<td><input style="width: 170px" class="datepicker" attr="doTime" readonly="readonly"></td>' +
                '<td><input type="text" attr="doName"/></td>' +
                '<td><input type="text" attr="new_kongshu" readonly="readonly"/></td>' +
                '<td><input type="text" attr="new_kaidu" readonly="readonly"/></td>' +
                '<td><input type="text" attr="new_konghao" readonly="readonly"/></td>' +
                '<td><input type="text" attr="old_kongshu" readonly="readonly"/></td>' +
                '<td><input type="text" attr="old_kaidu" readonly="readonly"/></td>' +
                '<td><input type="text" attr="old_konghao" readonly="readonly"/></td>' +
                '<td><input type="text" attr="shang_shuiwei" readonly="readonly"/></td>' +
                '<td><input type="text" attr="xia_shuiwei" readonly="readonly"/></td>' +
                '<td><input type="text" attr="liuliang" readonly="readonly"/></td>' +
                '<td class="rec_hide"><span class="deleteRec glyphicon glyphicon-remove" style="color:red;width:60px;"></span></td>' +
                '<td class="rec_show"><input type="checkbox" class="rec_checkbox"></td>' +
                '</tr>';
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
        function addDatePickEvent() {
            $('.table .datepicker').datetimepicker({
                showSecond: true,
                timeFormat: "HH:mm:ss",
                dateFormat: "yy-mm-dd",
                maxDate: "+0M +0D",
                changeYear: true,
                onClose: queryDataByDatetime
            });
            $('.diaodu_btn').off('click').on('click', function () {
                var tds = $(this).parent().siblings();
                var obj = {};
                tds.each(function () {
                    obj[$(this).find('input').attr('attr')] = $(this).find('input').val();
                });
                $.load(__path + "/gczl/doDayDiaodu.jsp?data=" + JSON.stringify(obj));
            });
            $('.deleteRec').off('click').on('click', function () {
                if (!confirm('确定要删除吗？')) {
                    return false;
                }
                var tr = $(this).parent().parent();
                var queryDate = tr.find('input:first').val();
                var queryPerson = tr.find('input:eq(1)').val();
                $.post(__path + '/applyManageMaint.do?method=deleteDoDayReportData', {
                    queryDate: queryDate,
                    queryPerson: queryPerson
                }, function (data) {
                    if (data.success) {
                        tr.remove();
                    } else {
                        alert(data.message);
                    }
                });
            });
            $('.deleteRec').show();
            $('.table tr:eq(3) .deleteRec').hide();
        }
        function addRow() { //在第四行添加空白行
            if ($('.table tr:eq(3) input:eq(0)').val()) {
                $('.table tr:eq(3)').unbind('change');
                $('.table tr:eq(3)').before(trHtml);
                $('.table tr:eq(3)').on('change', addRow);
                addDatePickEvent();
            }
        }
        function onSave() {
            var data = [];
            $('.table tr').each(function () {
                var obj = {};
                if ($(this).find('input:first').val()) {
                    $(this).find('input').each(function () {
                        obj[$(this).attr('attr')] = $(this).val();
                    })
                    data.push(obj);
                }
            });
            $.post(__path + '/applyManageMaint.do?method=saveDoDayReportData', {
                data: JSON.stringify(data)
            }, function (data) {
                if (data.success) {
                    //alert('保存成功');
                } else {
                    alert(data.message);
                }
            });
        }
        function onQuery() {
            var currentDate = new Date();
            var currentMonth = currentDate.getMonth() + 1;
            if(currentMonth<10){
                currentMonth = "0"+currentMonth;
            }
            var startDate = $('.startDate').val() ? $('.startDate').val() : new Date(currentDate.getFullYear() + '-' + currentMonth + '-01').Format("yyyy-MM-dd hh:mm:ss");
            var endDate = $('.endDate').val() ? $('.endDate').val() : currentDate.Format("yyyy-MM-dd hh:mm:ss");
            $.post(__path + '/applyManageMaint.do?method=queryDoDayReportData', {
                startDate: startDate,
                endDate: endDate
            }, function (data) {
                if (data.success) {
                    $('.table tr').slice(4).remove();
                    var array = JSON.parse(data.attributes.data);
                    for (var i = 0; i < array.length; i++) {
                        $('.table tr:eq(3)').after(trHtml);
                    }
                    for (var i = 0; i < array.length; i++) {
                        $('.table').find('tr:eq(' + (i + 4) + ') td').each(function () {
                            if ($(this).find('input').attr('attr') == 'doTime') {
                                $(this).find('input').val(array[i][$(this).find('input').attr('attr')])
                            } else {
                                $(this).find('input').val(array[i][$(this).find('input').attr('attr')])
                            }
                        });
                    }
                    addDatePickEvent();
                } else {
                    alert(data.message);
                }
            });
        }
        function queryDataByDatetime() {
            if ($(this).val()) {
                var tr = $(this).parent().parent().siblings()[4];
                if(tr){
                    var beforetime = $(tr).find('input:eq(0)').val();
                    if($(this).val()<beforetime){
                        alert('选择时间必须要晚于上次操作的时间！');
                        $(this).parent().parent().remove();
                        return;
                    }
                }
                var datetime = $(this).val();
                var tds = $(this).parent().siblings();
                $.post(__path + '/applyManageMaint.do?method=getDoDayReportData', {datetime: datetime},
                        function (data) {
                            if (data.success) {
                                var obj = JSON.parse(data.attributes.data);
                                tds.each(function () {
                                    if ($(this).find('input').attr('attr') != 'doName') {
                                        $(this).find('input').val(obj[$(this).find('input').attr('attr')])
                                    }
                                });
//                                onSave();
                            } else {
                                alert(data.message);
                            }
                        }, "json");
            }
        }
        function onExport() {
            var currentDate = new Date();
            var currentMonth = currentDate.getMonth() + 1;
            if(currentMonth<10){
                currentMonth = "0"+currentMonth;
            }
            var startDate = $('.startDate').val() ? $('.startDate').val() : new Date(currentDate.getFullYear() + '-' + currentMonth + '-01').Format("yyyy-MM-dd hh:mm:ss");
            var endDate = $('.endDate').val() ? $('.endDate').val() : currentDate.Format("yyyy-MM-dd hh:mm:ss");
            window.open(__path + '/applyManageMaint.do?method=exportdoDayReportData&startDate=' + startDate + "&endDate=" + endDate);
        }
        function doCreateDdWord() {//生成调度单
            var count = 0;
            var queryDate = [];
            $('.rec_checkbox').each(function () {
                if ($(this).is(':checked')) {
                    count++;
                    var val = $(this).parent().parent().find('input:first').val();
                    if (val) {
                        queryDate.push(val);
                    }
                }
            });
            if (count > 3) {
                alert("一张调度单最多选择3条记录");
                return false;
            }
            $.post(__path + '/applyManageMaint.do?method=doCreateDdWord', {
                queryDate: queryDate.join(',')
            }, function (data) {
                if (data.success) {
                    window.open(__path + '/gczl/doDayDiaodu.jsp?data=' + data.attributes.data);
                    $('.rec_show').hide();
                    $('.rec_hide').show();
                    $('.rec_checkbox').attr('checked', false)
                } else {
                    alert(data.message);
                }
            }, 'json');
        }
        function createDdWord() {
            $('.rec_hide').hide();
            $('.rec_show').show();
        }
        function notCreateDdWord() {
            $('.rec_hide').show();
            $('.rec_show').hide();
        }
        $(function () {
            onQuery();
            $('.form-inline .datepicker').datetimepicker({
                showSecond: true,
                timeFormat: "HH:mm:ss",
                dateFormat: "yy-mm-dd",
                maxDate: "+0M +0D",
                changeYear: true
            });
            $('.table tr:eq(3)').on('change', addRow);
        })
        function loadDiv(url){
            window.open(__path+url,"_self");
        }
    </script>
    <style>
        td {
            text-align: center;
            vertical-align: middle;
        }

        input {
            text-align: center;
            width: 90px;
            border: 0px;
        }

        .rec_show {
            display: none;
        }
    </style>
</head>
<body>
<div class="container" id="report">
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
                            <li class="active"><a href="#" onclick="loadDiv('/gczl/doDayReport.jsp')">闸门控制系统报表</a></li>
                            <li><a href="#" onclick="loadDiv('/gczl/zmDayReport.jsp')">闸门控制系统日报表</a></li>
                        </ul>
                    </div>
                </div>
            </nav>
        </div>
    </div>
    <div class="row" style="margin-top: 20px;">
        <form class="form-inline" role="form">
            <div class="rec_hide form-group form-group-md">
                <label class="control-label" for="startDate">开始日期</label>
                <input type="text" class="form-control datepicker startDate" id="startDate" readonly="readonly">
            </div>
            <div class="rec_hide form-group form-group-md">
                <label class="control-label" for="endDate">结束日期</label>
                <input type="text" class="form-control datepicker endDate" id="endDate" readonly="readonly">
            </div>
            <button type="button" class="rec_hide query_btn btn btn-primary btn-md" onclick="onQuery()">查询</button>
            <button type="button" class="rec_hide save_btn btn btn-success btn-md" onclick="onSave()">保存</button>
            <button type="button" class="rec_hide exp_btn btn btn-success btn-md" onclick="onExport()">导出Excel</button>
            <button type="button" class="rec_hide doc_btn btn btn-success btn-md" onclick="createDdWord()">生成调度单
            </button>
            <span class="pull-right">
                <button type="button" class="rec_show btn btn-success btn-md" onclick="doCreateDdWord()">确认生成</button>
                <button type="button" class="rec_show btn btn-primary btn-md" onclick="notCreateDdWord()">取消</button>
            </span>
        </form>
    </div>
    <div class="row" style="margin-top: 10px;">
        <table class="table table-striped table-bordered table-hover table-condensed">
            <tr>
                <td colspan="12"><h3><strong>蚌埠闸闸门控制系统报表</strong></h3></td>
            </tr>
            <tr>
                <td rowspan="2" style="vertical-align:middle;"><strong>调度指令时间</strong></td>
                <td rowspan="2" style="vertical-align:middle;"><strong>调度员</strong></td>
                <td colspan="3"><strong>新闸</strong></td>
                <td colspan="3"><strong>老闸</strong></td>
                <td rowspan="2" style="vertical-align:middle;"><strong>上游水位</strong></td>
                <td rowspan="2" style="vertical-align:middle;"><strong>下游水位</strong></td>
                <td rowspan="2" style="vertical-align:middle;"><strong>流量</strong></td>
                <td rowspan="2" class="rec_show" style="vertical-align:middle;"><strong>选择记录</strong></td>
                <td rowspan="2" class="rec_hide" style="vertical-align:middle;"><strong>操作</strong></td>
            </tr>
            <tr>
                <td><strong>开启孔数</strong></td>
                <td><strong>开度</strong></td>
                <td><strong>开启孔号</strong></td>
                <td><strong>开启孔数</strong></td>
                <td><strong>开度</strong></td>
                <td><strong>开启孔号</strong></td>
            </tr>
            <tr>
                <td><input style="width: 170px" class="datepicker" attr="doTime" readonly="readonly"></td>
                <td><input type="text" attr="doName"/></td>
                <td><input type="text" attr="new_kongshu" readonly="readonly"/></td>
                <td><input type="text" attr="new_kaidu" readonly="readonly"/></td>
                <td><input type="text" attr="new_konghao" readonly="readonly"/></td>
                <td><input type="text" attr="old_kongshu" readonly="readonly"/></td>
                <td><input type="text" attr="old_kaidu" readonly="readonly"/></td>
                <td><input type="text" attr="old_konghao" readonly="readonly"/></td>
                <td><input type="text" attr="shang_shuiwei" readonly="readonly"/></td>
                <td><input type="text" attr="xia_shuiwei" readonly="readonly"/></td>
                <td><input type="text" attr="liuliang" readonly="readonly"/></td>
                <td class="rec_hide"><span class="deleteRec glyphicon glyphicon-remove"
                                           style="color:red;width:60px;"></span></td>
                <td class="rec_show"><input type="checkbox" class="rec_checkbox"></td>
            </tr>
        </table>
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
