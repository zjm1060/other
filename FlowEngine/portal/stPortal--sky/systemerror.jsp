<%@ page contentType="text/html; charset=GBK" %>
<%@ page isErrorPage="true" %>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>系统信息</title>
<link href="../css/gld.css" type="text/css" rel="stylesheet">
</head>
<body class="bgimage01" leftmargin="0" topmargin="0" style="background-color: #ffffff">
<div align="center">
  <center>
  <table width="80%" height="80%" border="1">
    <tr>
      <td width="600" valign="top" height="200">
      <table border="0" cellpadding="0" cellspacing="0" width="600" height="200" class="comment-font-size">
        <tr>
          <td width="150" valign="middle">
            <p align="center"><img border="0" src="images/gb.gif" width="183" height="100"></p>
          </td>
          <td width="1">
               <table border="0" height="100%" cellspacing="1" cellpadding="1" align="center"><tr><td class="table-header-sytle-01"></td></tr></table>
          </td>
          <td width="450">
            <p align="center"><font color="#FF0000"><b><%=exception.getMessage()%></b></font>
          </td>
        </tr>
      </table>
      </td>
    </tr>
  </table>
</div>
</body>
</html>