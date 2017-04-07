<%@ page contentType="text/html; charset=GBK" %>
<html>
<head>
<title>警告</title>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<link type="text/css" rel="stylesheet" href="/NewTree/Common/Css/Public.css">
<script LANGUAGE="javascript">
window.onload = initWindow;

function initWindow()
{
    var sMsg = window.dialogArguments;
    document.all.item("MsgUtil").value = sMsg;
}
function Ok()
{
    window.close();
}
</script>
</head>
<body class="innerFrame">
<table width="430" border="0" cellspacing="0" cellpadding="0" style="position:relative;top:15px;left:15px;">
  <tr>
    <td width="85%">
      <img src="/NewTree/Images/Alert.gif" border="0" align="absmiddle">
      <input id="MsgUtil" name="MsgUtil" type="text" readonly style="width:320;height:25">
    </td>
    <td width="15%" style="padding-top:8px;">
      <input type="button" name="CancleBtn" class="standardBtn" value="关闭" onclick="Ok()">
    </td>
  </tr>
</table>
</body>
</html>
