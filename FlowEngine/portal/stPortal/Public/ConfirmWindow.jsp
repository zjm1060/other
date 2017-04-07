<%@ page contentType="text/html; charset=GBK" %>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<title>确认！</title>
<link rel="stylesheet" href="/NewTree/Common/Css/Public.css" type="text/css">
<script type="text/javascript">
window.onload = initWindow;

function initWindow(){
  var sMsg = window.dialogArguments;
  document.all.item("MsgUtil").innerText = sMsg;
}
function Ok(){
  window.returnValue = "True";
  window.close();
}
function Quit(){
  window.returnValue = "False";
  window.close();
}
</script>
</head>
<body scroll="no" class="innerFrame">
<table width="375" border="0" cellspacing="0" cellpadding="0" align="center">
  <tr style="background-color:white;">
    <td width="40%">
      <span style="width:100%;font-size:10.8pt;font-weight:bold;text-align:center;padding-top:15px;">系统提示信息</span>
    </td>
    <td width="60%" style="text-align:right;">
      <img src="/NewTree/Images/alert.jpg" border="0">
    </td>
  </tr>
  <tr>
    <td id="MsgUtil" colspan="2" width="100%" height="90" style="text-align:center;padding-top:40px;">
    </td>
  </tr>
  <tr>
    <td colspan="2" width="100%" valign="top" align="center">
      <hr style="width:100%">
    </td>
  </tr>
  <tr>
    <td colspan="2" width="100%" height="35" style="text-align:right;padding-right:10px;">
      <input type="button" class="standardBtn" value="确定" onclick="javascript:Ok();">
      <input type="button" class="standardBtn" value="取消" onclick="javascript:Quit();">
    </td>
  </tr>
</table>
</body>
</html>
