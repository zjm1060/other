<!DOCTYPE HTML>
<%@page contentType="text/html; charset=utf-8"%>
<%
String path = request.getContextPath();
%>
<html>
  <head>
    <link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-all.css" />
    <style type="text/css">
      body {TEXT-ALIGN: center;} body .x-panel { margin-bottom: 0px; } #errormsg { margin: 150px auto;
      width:400px;text-align:left;} #errormsg .title{text-align:left;} #errormsg .msg{}
    </style>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
    <div id="errormsg" class="x-panel" >
      <div class="x-panel-tl">
        <div class="x-panel-tr">
          <div class="x-panel-tc">
            <div 
                style="CURSOR: move; MozUserSelect: none; KhtmlUserSelect: none"
                class="x-panel-header x-unselectable"
                unselectable="on">
              <span class="x-panel-header-text" style="font-size:14px;">
                <%=request.getAttribute("errorTitle").toString()%>
              </span>
            </div>
          </div>
        </div>
      </div>
      <div class="x-panel-bwrap">
        <div class="x-panel-ml">
          <div class="x-panel-mr">
            <div class="x-panel-mc">
              <div class="x-panel-body">
              	<p style="text-indent:2em;font-size:14px;line-height:22px;">
                <%=request.getAttribute("errorMsg").toString()%>
                </p>
              </div>
            </div>
          </div>
        </div>
      </div>
    <div class="x-panel-bl x-panel-nofooter">
      <div class="x-panel-br">
        <div class="x-panel-bc">
        </div>
      </div>
    </div>
  </div>
</div>
</body>
</html>

