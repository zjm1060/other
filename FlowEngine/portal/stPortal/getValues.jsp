<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="java.util.*,java.sql.*"%>
<%@ page import="baseserverjb.BaseServerClient,baseserverjb.IFramework"%>

<html>
<head>
<title></title>
</head>

<%
  request.setCharacterEncoding("GBK");
  //String[] tels=request.getParameterValues("tels");
  String tels=request.getParameter("tels");
  if(tels==null) tels="";
  if(tels.equals("undefined")) tels="";
  String smsMsg=request.getParameter("smsMsg");
  if(smsMsg==null) smsMsg="";
  
  String sendResult="";
  String actionFlag=request.getParameter("actionFlag");
  if(actionFlag==null) actionFlag="";
  
  if(actionFlag.equals("doSend")){
  String currentUser=request.getRemoteUser();
  Connection con = null;
  Statement stm = null;
  PreparedStatement pstm = null;
  ResultSet rs = null;
	String sql = "";
	try{ 
		con = BaseServerClient.getWizardConnection();
		//stm = con.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_UPDATABLE);
		sql = "INSERT into sms_tbl(sms_seq,send_user,mobl_nmbr,sms_msg,"
		    +" send_stat,send_nmbr,sms_type,save_time) values(?,?,?,?,0,0,0,sysdate) ";
		//stm.executeUpdate(sql);
    
	  pstm = con.prepareStatement(sql);
    pstm.setLong(1,System.currentTimeMillis());
    pstm.setString(2, currentUser);
    pstm.setString(3, tels);
    pstm.setString(4, smsMsg);
    pstm.execute();
    pstm.close();
    con.commit();//提交
    out.println("<script language='javascript'>alert('发送成功！');window.close();opener.window.close();</script>");
     sendResult="发送成功！";
	}catch(Exception ex){
		  ex.printStackTrace();
		  out.println("<script language='javascript'>alert('发送失败！');window.close();</script>");
                  sendResult="发送失败！！";
	} finally{
		if (stm != null) {
		    stm.close();
		    stm = null;
		  }
	  if (con != null){
      con.close();
      con = null;
    }
	}
}else{
%>
<script language="javascript" type="">
function doSend(){
	var temp=document.all.item("tels").value;
        var smsMsg=document.all.item("smsMsg").value;

	if(temp.length>4000){
		alert("手机总长度超出4000字符，请分段发送！");
		return;
	}
       if(temp.length==0 || smsMsg.length==0){
		alert("请填写手机和内容！");
		return;
	}

	postForm.actionFlag.value="doSend";
  postForm.submit();
}
</script>

<link rel="stylesheet" type="text/css" href="../Common/Css/Public.css">
  <body>
  <form id="postForm" action="getValues.jsp" method="POST">
    <table width="95%" height="100%" border="0" align=center>
    <tr>
      <td width="100%" height="100%" align=center> <font size="6"><strong>短信发送</strong></font>
      <br>
      <fieldset>
        <legend>短信</legend>
        <table width="100%" border="0" cellspacing="0"  id="whereClause">
          <tr>
            <td width="20%" align="left" bgcolor="#CCCC99">
						  <div align="center"><strong>手机号码:</strong></div></td>
            </td>
            <td width="80%" align="left" bgcolor="">
						  <TEXTAREA cols="50"  rows="3" width="100%" height="100%" name="tels" ><%=tels%></TEXTAREA>
            </td> 
          </tr>
          <tr>
            <td width="20%" align="left" bgcolor="#CCCC99">
						  <div align="center"><strong>短信内容:</strong></div></td>
            </td>
            <td width="80%" align="left" bgcolor="">
						  <TEXTAREA style="width:100%"  class="" id="smsMsg" ROWS="20" name="smsMsg"  cols="" ><%=smsMsg%></TEXTAREA>
            </td>              
          </tr>
          <tr>
            <td width="100%" align="center" bgcolor="" colspan="2">
						 <input type="button" class="Btn_common" name="btnOK" value="发送" id="btnOK" onclick="javacript:doSend();">
						 <input type="button" class="Btn_common" name="btnOK" value="关闭" id="btnOK" onclick="javacript:window.close();">
            </td>  
          </tr>
        </table>
     </fieldset>
     </td>  
   </tr>
   </table>
   <input name="actionFlag" type="hidden" id="actionFlag" value="">
  </form>      
      
  </body>
  </html>
<%}%>