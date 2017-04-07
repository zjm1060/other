<%@ page contentType="text/html;charset=GBK" %>
<%@ page import="java.util.*" %>
<%@ page import="com.minstone.report.CReport" %>

<%
  //变量定义
  String rpt_code="";                      //报表编号
  String m_sOut="";                        //查询条件输入框字串
  String[] m_arrPara={"rpt_code"};
  String[] m_arrUserPwd = new String[2];
  int m_iLength = 0;
  Hashtable m_hs=null;

  CReport m_oRpt = new CReport();

  rpt_code = request.getParameter("rpt_code");
  try{
    if(!rpt_code.trim().equals(""))
      m_sOut = m_oRpt.generateWebForm(rpt_code,request.getRemoteUser());
      //m_sOut = "测试";
    else
      System.out.println("获取条件输入框错误! ShowRptCondition.jsp");
    //获取m_sOut的长度
    m_iLength = m_sOut.length();
  }catch(Exception e){
    e.printStackTrace();
    out.println("获取用户输入条件框错误! ShowRptCondition.jsp Error!" + e.getMessage());
  }
%>
<html>
<head>
<title>输入查询条件</title>
<link rel="stylesheet" type="text/css" href="../Common/Css/Public.css">
<link rel="stylesheet" type="text/css" href="../Common/Css/Query.css">
<style>
body
{
  	background-color:#C2CEEB;
  	font-size:9pt;
  	font-family: 宋体;
 	margin:5px 5px 0px 5px;  /*top right bottom left*/
  	padding:0px 0px 0px 0px;
  	scrollbar-face-color: #DEE3E7;
  	scrollbar-highlight-color: #FFFFFF;
  	scrollbar-shadow-color: #DEE3E7;
  	scrollbar-3dlight-color: #D1D7DC;
  	scrollbar-arrow-color:  #006699;
  	scrollbar-track-color: #EFEFEF;
  	scrollbar-darkshadow-color: #98AAB1;
	overflow-x:auto;
	overflow-y:auto;
}
td
{
	font-size: 12px;
}
.Btn_last_td
{
	height:25px;
	text-align:right;
}
.Out_border
{
	border-top: 1px solid #ffffff;
	border-left: 1px solid #fffffff;
	border-right: 2px ridge #95A8D7;
	border-bottom: 2px ridge #95A8D7;
	background-color:#E0E7F8;
	padding-top:6px;
	padding-left:8px;
	padding-bottom:6px;
	padding-right:8px;
	text-align:center;
	vertical-align:top;
}
.ReprotParameterDesc{
	padding-top:6px;
	padding-bottom:4px;
	padding-right:3px;
	width:200px;
}
.ReprotOperator{
	font-size:9pt;
}
.ReprotAndor{
	font-size:9pt;
}
.ReprotValue{
	background-color:#ffffff;
	font-size:9pt;
	height:18px;
	width:200px;
}
.Btn_common{
	width:55px;
	height:20px;
	padding-top:4px;
	padding-left: 0px;
	border:1px #939EB2 solid;
	text-align:center;
	background-image:url('./images/Btn_united_bg.gif');
	cursor:hand;
	font-size: 9pt;
}
</style>
<script language="javascript">
function VerifyInput(){
	return true;
}
function getSQL(a_sSQL)
{
  var l_oWindow = window.dialogArguments;
  window.returnValue = a_sSQL;
  window.close();
}
function window_onload()
{
  if("<%=m_iLength%>" == "0"){
    window.returnValue = "";
    window.close();
  }
}
</script>
</head>
<body onload="javascript:window_onload()">
<table border="0" height="100%" cellspacing="0" cellpadding="0" width="100%">
    <tr>
	<td class="Out_border" height="100%">
	<fieldset style="height:100%">
	<div class="TaskList_ListIV">
	<%=m_sOut%>
	</div>
	</fieldset>
	</td>
    </tr>
    <tr height="40px">
	<td class="Btn_last_td">
		<input type="submit" class="btn1" value="确 定" name="okbtn" onclick="document.reportform.submit();" size=10>&nbsp; <input type="reset" value="重 填" class="btn1" name="cancelbtn" onclick="document.reportform.reset();" size="10">
	</td>
    </tr>
</table>
<iframe id="frmGetSQL" name="frmGetSQL" src="" height="0" width="0" scrolling="no" frameborder="NO" border="0" framespacing="0" noresize style="position:absolute;top:0px;left:0px;"></iframe>
</body>
</html>