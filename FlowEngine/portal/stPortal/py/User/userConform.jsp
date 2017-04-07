<%@ page contentType="text/html; charset=GBK" %>

<%
  //强制页面刷新
  response.setHeader("Pragma","No-cache");
  response.setHeader("Cache-Control","no-cache");
  response.setDateHeader("Expires", 0);
%>

<html>
<head>
<link href="/stPortal/images/new.css" rel="stylesheet" type="text/css">
<jsp:useBean id="UserConform" scope="page" class="net.kingsoft.user.UserConform"/>
</head>
<body bgcolor="#FFFFFF" text="#000000">
<%
/**参数说明
<分页>
存在的问题已修正,其实只需做简单的改动,但是开始逻辑比较乱,晕了半天才看懂, 参数的命名和算法没有变, 为防止别人跟我一样晕, 特老老实实写好注释....
思路: 每进入新页面前, 赋予三个参数:
pPage				当前页数
mMode				决定升序或降序, 因为上页和下页的检索顺序是不同的
mUserName		下一个页面开始的那个用户.
其余参数
MAX					每页最大显示条目
mAllRows			总记录数
mAllPage			总页数
resultRow			剩余条数, 在最后一页不足MAX的条数时, 取实际剩余的值
这样就清楚了...*/
int MAX=10;
UserConform.setMAXROW(MAX);
int resultRow=0,mPage,mAllRows,mAllPage,mPageRow;
String mUserName,mMode,mPageStr;
mPage=1;
if(request.getParameter("pPage")!=null)
	mPage=Integer.parseInt(request.getParameter("pPage"));
mAllRows=UserConform.countRow();
mAllPage=(mAllRows+MAX-1)/MAX;//取得总页数
mMode=request.getParameter("pMode");
if(mMode == null)
	mMode="0";
mUserName=request.getParameter("pUserName");
if(mUserName == null)
	mUserName="";
if(mMode.equals("0"))
{
	resultRow=UserConform.queryUser(mUserName);
}
if(mMode.equals("1"))
{
	resultRow=UserConform.upQuery(mUserName);
//	if(mUserName.equals(""))
//		mPage=mAllPage-1;
}
if(resultRow>MAX)
	mPageRow=MAX;
else
	mPageRow=resultRow;
if(resultRow<0)
{
%>
系统错误
<%
}
else
{
%>
<br>
<br><div align="center">
<table width="90%"  border="0" cellpadding="3" cellspacing="1" bgcolor="#999999" style="BORDER-COLLAPSE: collapse">
  <tr align="left">
    <td height="25" colspan="5" class="topback">
      <div class="fontw"><strong><font color="#FFFFFF"><strong><img src="/stPortal/images/folder_inbox.gif" width="16" height="16" hspace="5" align="absmiddle"></strong>用户审核</font></strong></div>
      <div align="center" class="fontw"></div>
      <div align="center" class="fontw"></div>
      <div align="center" class="fontw"></div>
      <div align="center" class="fontw"></div></td>
  </tr>
  <tr>
    <td height="23" align="center" bgcolor="#F6f6f3"><strong><font color="#666666">用户帐号</font></strong></td>
    <td width='14%' height="23" align="center" bgcolor="f6f6f3"><font color="#666666"><strong>用户名</strong></font></td>
    <td width='8%' height="23" align="center" bgcolor="f6f6f3"><font color="#666666"><strong>是否批准</strong></font></td>
    <td width='8%' height="23" align="center" bgcolor="f6f6f3"><font color="#666666"><strong>查看</strong></font></td>
    <td width='8%' height="23" align="center" bgcolor="f6f6f3"><font color="#666666"><strong>删除</strong></font></td>
  </tr>
  <%      int i;
        for(i=0;i<mPageRow;i++)
		{
			String user_code = UserConform.getUSER_CODE(i);
			String user_name = UserConform.getFULL_NAME(i);
			String user_pwd = UserConform.getUSER_PWD(i);
%>
  <tr>
    <td width='8%' height="23" align='center' bgcolor="#FFFFFF" class="Tabcell1"><a href="javascript:void(0)" onclick="javascript:ShowUserInfo('<%=user_code%>')"><%=user_code%></a></td>
    <td width='14%' height="23" align='center' bgcolor="#FFFFFF" class="Tabcell1"><%=user_name%></td>
    <td width='8%' height="23" align='center' bgcolor="#FFFFFF" class="Tabcell1"><a href="addUser.jsp?pUserName=<%=user_code%>&pPWD=<%=user_pwd%>">批准注册</a></td>
    <td width='8%' height="23" align='center' bgcolor="#FFFFFF" class="Tabcell1"><a href="javascript:void(0)" onclick="javascript:ShowUserInfo('<%=user_code%>')">查看</a></td>
    <td width='8%' height="23" align='center' bgcolor="#FFFFFF" class="Tabcell1"><a href="javascript:void(0)" onclick="javascript:DeleteUser('<%=user_code%>')"  >删除</a></td>
  </tr>
  <%
		}
%>
  <%
}
%>
  <tr>
    <td height="23" colspan="5" align="left" bgcolor="#FFFFFF" class="Tabcell1" style="padding-right:15px;">
      <div align="left" valign="bottom">共<%=mAllRows%> 条记录</div>
      <div align="right">
        <%	if(mPage != 1)
        {
%>
        <a href="userConform.jsp">首页</a>
        <%
	}
	if(mPage>1)
	{
%>
        <a href="userConform.jsp?pPage=<%=(mPage-1)%>&pMode=1&pUserName=<%=UserConform.getUSER_CODE(0)%>">上一页</a>
        <%
	}
	if(mPage < mAllPage)
	{
%>
        <a href="userConform.jsp?pPage=<%=(mPage+1)%>&pMode=0&pUserName=<%=UserConform.getUSER_CODE(9)%>">下一页</a>
        <%
	}
	if(mPage < mAllPage)
	{
%>
        <a href="userConform.jsp?pMode=1&pPage=<%=(mAllPage)%>">尾页</a>
        <%
	}
%>
        第<%=mPage%>页 共<%=mAllPage%>页 </div></td>
  </tr>
</table>
<script language="javascript">
//-----------------------------------------------------------
//功能：察看用户信息
//-----------------------------------------------------------
function ShowUserInfo(usercode)
{
  var sFeatures;
  sFeatures = "dialogHeight:650px;dialogWidth:820px;";
  sFeatures = sFeatures + "center:yes;edge:raised;scroll:no;status:no;unadorned:yes;"
  var URL="show_user.jsp?pUserName=" + usercode;
  var sReturn = window.showModalDialog(URL,"",sFeatures);
  if (sReturn != "" && sReturn != undefined)
  {
    oFrm = parent.document.frames("LeftFrm");
    if (oFrm.ModifyNode != null)
      oFrm.ModifyNode("220_222_225",sReturn);
  }
}

//-----------------------------------------------------------
//功能：删除用户信息
//-----------------------------------------------------------
function DeleteUser(usercode)
{
		if(window.confirm("是否删除用户"))
		{
				var sFeatures;
				var returnValue;
			  sFeatures = "dialogHeight:650px;dialogWidth:820px;";
			  sFeatures = sFeatures + "center:yes;edge:raised;scroll:no;status:no;unadorned:yes;"
			  
			  var URL="delUser.jsp?pUserName="+usercode;
			  returnValue = window.showModalDialog(URL,"",sFeatures);
			  if(returnValue=="ok" ){
			  	refurbish();
			  	}
		}
			
}
function refurbish(){
	
		window.location.reload();
	
	}
</script>
</body>
</html>