<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,suntek.portal.publicLib.RecordSetObject,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    String m_StrCSS = "";
	String m_oSys = "";
    response.setHeader("Pragma","No-cache");
    response.setHeader("Cache-Control","no-cache");
    response.setDateHeader("Expires", 0);
    String cmd = request.getParameter("cmd");
    if (cmd == null)
      cmd = "";
    String user_code=(String)request.getRemoteUser();
    if(user_code==null)
      user_code="";
    if(cmd.equals("update"))
    {
      String ls_cssID = request.getParameter("css_id");
      if (ls_cssID == null)
        ls_cssID = "";
      try{
        Hashtable hKey=new Hashtable();
        Hashtable hPara=new Hashtable();
        hKey.put("USER_CODE",user_code);
        hPara.put("CSS_ID",ls_cssID);
        hPara.put("USER_CODE",user_code);
        oSys.update(hKey,hPara,"USER_CSS",false);
      }
      catch(Exception ex){
        System.err.println("更新USER_CSS失败了。");
      }
    }
    else{
      String ls_CurCSS = "";
      RecordSetObject lo_rsUserCSS = null;
      RecordSetObject lo_rsCSS = null;
      lo_rsCSS = oManager.GetCSSSetting();
      lo_rsUserCSS = oManager.GetUserCSS();
      if (lo_rsUserCSS.getRowCount()>0)
        ls_CurCSS = lo_rsUserCSS.getString("css_id",0)+"";
      m_StrCSS =PortalUtil.RecordsetOptions("css_id","css_name",ls_CurCSS,lo_rsCSS);
    }
%>
<HTML>
<script LANGUAGE="javascript">
<!--
function Save()
{
	var strURL;
	var xhreq;
	xhreq = new ActiveXObject("Microsoft.XMLHTTP");
	strUrl = "CSSSetting.jsp?cmd=update"
	strUrl += "&css_id=" + document.all.item("css").value;

	xhreq.open("POST", strUrl, false);

	xhreq.send();
	// If we got "OK" back, then consider the post successful.
	if("OK" == xhreq.statusText)
		window.close();
	else
		alert( xhreq.statusText );
}

//-->
</script>

<title>界面风格设置</title>
<HEAD>
</HEAD>
<BODY leftmargin="0" topmargin="0" bgcolor="White">
<link rel="STYLESHEET" type="text/css" href="../../include/Public.css">
  <table  width="100%" height="100%"  Class="Table_Normal" align="center" vlign="top" cellspacing="0" cellpadding="0">
    <tr>
      <td  width="15%" align="right">界面风格：&nbsp;</td>
      <td width="85%"><select NAME="css"  STYLE="width:95%" class="DrawSelect">
            <%=m_StrCSS%>
      </select>
      </td>
    </tr>
    <tr>
          <td align="center" colspan="2">
              <input type="button" id="ok" name="ok" value="确    定" class="Input_Button" onclick="javascript:Save();">&nbsp;
              <input type="button" id="cancel" name="cancel" value="取    消" class="Input_Button" onclick="javascript:window.close();">&nbsp;
          </td>
    </tr>
  </table>
</BODY>
</HTML>