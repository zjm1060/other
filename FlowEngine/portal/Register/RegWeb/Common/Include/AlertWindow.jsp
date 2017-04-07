<%@ page contentType="text/html; charset=GBK" %>
<%
    String Msg="";
    Msg = request.getParameter("Msg");
%>
<html>
<head>
<title>¾¯¸æ----------------------------------------------------------------------------</title>
<link type="text/css" rel="stylesheet" href="../Css/Public.css">
<script LANGUAGE="javascript">
function Ok()
{
    window.close();
}
</script>
</head>
<body rightmargin="0" bottommargin="0" leftmargin="0" topmargin="0" class="DialogOuter">
<table width="440" border="0" cellspacing="0" cellpadding="0" style="position:relative;top:10px;left:10px;">
  <tr>
    <td width="80%">
      &nbsp;<img src="../Images/Alert.gif" border="0">
      <input type="text" readonly value="<%=Msg%>" style="width:300;height:30">
    </td>
    <td width="20%" align="center">
      <input type="button" name="CancleBtn" class="Btn_common" value="¹Ø±Õ" onclick="Ok()">
    </td>
  </tr>
</table>
</body>
</html>