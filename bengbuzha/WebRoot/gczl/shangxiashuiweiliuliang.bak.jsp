<%--
  Created by IntelliJ IDEA.
  User: zsz
  Date: 2017/1/17
  Time: 15:36
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title></title>
  <script type="text/javascript" src="jquery_ui/jquery-1.10.2.min.js"></script>
  <script type="text/javascript" src="echarts3/echarts.js"></script>
  <link rel="stylesheet" type="text/css" href="bootstrap/css/bootstrap.min.css">
  <script type="text/javascript" src="bootstrap/js/bootstrap.min.js"></script>
  <link rel="stylesheet" href="jquery_ui/jquery-ui.min.css">
  <script src="jquery_ui/jquery-ui.min.js"></script>
  <script src="jquery_ui/jquery-ui-timepicker-addon.js" type="text/javascript"></script>
  <link href="jquery_ui/jquery-ui-timepicker-addon.css" type="text/css"/>
  <script src="jquery_ui/jquery.ui.datepicker-zh-CN.js" type="text/javascript" charset="UTF-8"></script>
  <script src="jquery_ui/jquery-ui-timepicker-zh-CN.js" type="text/javascript" charset="UTF-8"></script>
  <script>
    var jobId;
    var __path = "<%=request.getContextPath()%>";
    Date.prototype.Format = function (fmt) {
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
    function flashChar(){
      $.post(__path + '/applyManageMaint.do?method=getTopReportData&datetime='+new Date().getTime(),null,
              function (data) {
                if (data.success) {
                  var obj = JSON.parse(data.attributes.data);
                  var liuliang = obj['liuliang'];
                  var shangyou = obj['shangyou'];
                  var xiayou = obj['xiayou'];
                  var new_open_konghao=obj['new_open_konghao'];
                  var old_open_konghao=obj['old_open_konghao'];
                  var newData,oldData;
                  if(new_open_konghao){
                    newData = new_open_konghao.split(',');
                    $('.new_count').text(newData.length);
                    var kh='';
                    for(var i=0;i<newData.length;i++){
                      if(i == newData.length-1){
                        kh = kh+newData[i]+"号";
                      }else{
                        kh = kh+newData[i]+"号，";
                      }
                    }
                    $('.new_konghao').text(kh);
                  }else{
                    $('.new_count').text(0);
                  }
                  if(old_open_konghao){
                    oldData = old_open_konghao.split(',');
                    $('.old_count').text(oldData.length);
                    var kh='';
                    for(var i=0;i<oldData.length;i++){
                      if(i == oldData.length-1){
                        kh = kh+oldData[i]+"号";
                      }else{
                        kh = kh+oldData[i]+"号，";
                      }
                    }
                    $('.old_konghao').text(kh);
                  }else{
                    $('.old_count').text(0);
                  }
                  $('.new_value').text(obj['new_kaidu']);
                  $('.old_value').text(obj['old_kaidu']);
                  var llTime = [];
                  var llData=[];
                  var shuiweiTime=[];
                  var shangData=[];
                  var xiaData=[];
                  for(var key in liuliang){
                    llTime.push(key);
                    llData.push(liuliang[key]);
                  }
                  for(var key in shangyou){
                    shuiweiTime.push(key);
                    shangData.push(shangyou[key]);
                    xiaData.push(xiayou[key]);
                  }
                  var option = {
//                    toolbox: {
//                      show : false,
//                      feature : {
//                        mark : {show: true},
//                        dataView : {show: true, readOnly: false},
//                        magicType : {show: true, type: ['line', 'bar', 'stack', 'tiled']},
//                        restore : {show: true},
//                        saveAsImage : {show: true}
//                      }
//                    },
                    title: {
                      text: '上下游水位及流量',
                      x: 'center'
                    },
                    tooltip: {
                      trigger: 'axis',
                      formatter: function (params) {
                        if(params.length>1){
                          return params[0].name + '<br/>'
                                  + params[0].seriesName + ' : ' + params[0].value + ' (m)<br/>'
                                  + params[1].seriesName + ' : ' + params[1].value + ' (m)<br/>';
                        }
                        return params[0].name + '<br/>'
                                + params[0].seriesName + ' : ' + params[0].value + ' (m^3/s)<br/>';
                      },
                      axisPointer: {
                        animation: false
                      }
                    },
                    legend: {
                      data:['上游水位','下游水位','流量'],
                      x: 'right'
                    },
                    grid: [{
                      left: 70,
                      right: 50,
                      height: '35%'
                    }, {
                      left: 70,
                      right: 50,
                      top: '55%',
                      height: '35%'
                    }],
                    xAxis : [
                      {
                        type : 'category',
                        boundaryGap : false,
                        axisLine: {onZero: true},
                        data: shuiweiTime
                      },
                      {
                        type : 'category',
                        gridIndex: 1,
                        boundaryGap : false,
                        axisLine: {onZero: true},
                        data: llTime,
                        position: 'top'
                      }
                    ],
                    yAxis : [
                      {
                        name : '水位',
                        type : 'value'
                      },
                      {
                        gridIndex: 1,
                        name : '流量',
                        type : 'value',
                        inverse: true
                      }
                    ],
                    series : [
                      {
                        name:'上游水位',
                        type:'line',
                        symbolSize: 8,
                        xAxisIndex:0,
                        yAxisIndex: 0,
                        animation:false,
                        hoverAnimation: false,
                        data:shangData,
                        areaStyle: {
                          normal: {
                            color: new echarts.graphic.LinearGradient(0,0,0,1,[{
                              offset: 0, color: 'rgba(216, 244, 247,1)'
                            },{
                              offset: 1, color: 'rgba(216, 244, 247,1)'
                            }],false)
                          }
                        },
                      },{
                        name:'下游水位',
                        type:'line',
                        symbolSize: 8,
                        xAxisIndex:0,
                        yAxisIndex: 0,
                        animation:false,
                        hoverAnimation: false,
                        data:xiaData,
                        areaStyle: {
                          normal: {
                            color: new echarts.graphic.LinearGradient(0,0,0,1,[{
                              offset: 0, color: 'rgba(199, 237, 250,0.5)'
                            },{
                              offset: 1, color: 'rgba(199, 237, 250,0.2)'
                            }],false)
                          }
                        }
                      },
                      {
                        name:'流量',
                        type:'line',
                        xAxisIndex: 1,
                        yAxisIndex: 1,
                        symbolSize: 8,
                        animation:false,
                        hoverAnimation: false,
                        data:llData,
                        areaStyle: {
                          normal: {
                            color: new echarts.graphic.LinearGradient(0,0,0,1,[{
                              offset: 0, color: 'rgba(199, 237, 250,0.5)'
                            },{
                              offset: 1, color: 'rgba(199, 237, 250,0.2)'
                            }],false)
                          }
                        }
                      }
                    ]
                  };
                  var myChart = echarts.init($('.mychart')[0]);
                  myChart.setOption(option);
                } else {
                  alert(data.message);
                }
              }, "json");
    }
    $(function(){
      flashChar();
      jobId = setInterval(function(){
        flashChar();
      },20000);
      $(window).bind('beforeunload',function(){
        if(jobId){
          clearInterval(jobId);
        }
      });
    });
    function loadDiv(url){
      window.open(__path+url,"_self");
    }
  </script>
  <style>
    td {
      text-align: center;
      vertical-align:middle;
    }
  </style>
</head>
<body>
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
              <li class="active"><a href="#"onclick="loadDiv('/gczl/shangxiashuiweiliuliang.jsp')">水位流量折线图</a></li>
              <li><a href="#" onclick="loadDiv('/gczl/doDayReport.jsp')">闸门控制系统报表</a></li>
              <li><a href="#" onclick="loadDiv('/gczl/zmDayReport.jsp')">闸门控制系统日报表</a></li>
            </ul>
          </div>
        </div>
      </nav>
    </div>
  </div>
  <div class="row">
      <table class="table table-striped table-bordered table-hover table-condensed">
        <tr><td class="col-md-1"></td><td class="col-md-1"><strong>开启数量</strong></td><td class="col-md-1"><strong>开度</strong></td><td class="col-md-8"><strong>开启孔号</strong></td></tr>
        <tr><td class="col-md-1"><strong>新闸</strong></td><td class="col-md-1 new_count"></td><td class="col-md-1 new_value"></td><td class="col-md-8 new_konghao"></td></tr>
        <tr><td class="col-md-1"><strong>老闸</strong></td><td class="col-md-1 old_count"></td><td class="col-md-1 old_value"></td><td class="col-md-8 old_konghao"></td></tr>
      </table>
  </div>
    <div class="mychart row" style="margin-top: 20px;height: 800px;"></div>
</div>
</body>
</html>
