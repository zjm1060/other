<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="com.minstone.fileLogin"%>
<%@ page import="oamodule.publiclib.DBService"%>
<%@ page import java.util.*%>
<%
  request.setCharacterEncoding("GBK");

  if(DBService.isAdmin(request.getRemoteUser())){

  String userCode="admin_oa";
  String userName="姓名";
  String userPwdBase="",userCodeBase="";
  String dirverName = request.getParameter("dirverSave");
  if(dirverName==null ) dirverName="A:";

  String actionFlag = request.getParameter("actionFlag");
  if(actionFlag==null) actionFlag="";
  if(actionFlag.equals("make")){
    String strTemp = request.getParameter("actionValue");
    StringTokenizer temp=new StringTokenizer(strTemp,";"); //分开多条记录
    while(temp.hasMoreTokens()){
      String temp1 = temp.nextToken();
      fileLogin fl=new fileLogin();
    	userCodeBase=fl.getBASE64(temp1);	//加密用户
    	userPwdBase=fl.getBASE64(fl.getUserPwd(temp1)); //加密密码
    	if(dirverName.equals("A:")) temp1="";  //如果是A盘，则不要目录
    	System.out.println(temp1+"用户的密钥盘："+userCodeBase+","+userPwdBase);

%>
    <META NAME="GENERATOR" Content="Microsoft Visual Studio 6.0">
    <script Language="VBScript" src="getUserFile_VB.js"></script>
    <script language="javascript">
    	  var allResult=writeIntoFile("<%=dirverName%>","<%=userPwdBase%>","WriteKey","<%=temp1%>"); //vbscript写key文件
        if(allResult==0) {
	        var returnU=writeIntoFile("<%=dirverName%>","<%=userCodeBase%>","WriteUser","<%=temp1%>");  //vbscript写key文件
	        if(returnU==0) {
	         alert("<%=temp1%>制作密钥盘成功!");
	        }else
	         alert("<%=temp1%>制作密钥盘失败!");
        }else
	         alert("<%=temp1%>制作密钥盘失败!");

    </script>
<%  }
  }%>
<script>

function funMakeFileKey(){

	var temps=document.all.item("selUserCodes").value;
  if(temps=="" || temps==null){
  	temps=document.all.item("selUserNames").value;
  	if(temps=="" || temps==null){
  		temps= funcgetSelID();
  	  if(temps=="" || temps==null){
  			alert("请选择相关记录");return;
  	  }
  	}
  }
  document.all.item("dirverSave").value = window.prompt("保存的盘符","A:");

  document.all.item("actionValue").value=temps;  //记录选择的用户
  document.frmMakeFileKey.action="makeFileKey.jsp";
  document.frmMakeFileKey.actionFlag.value="make";
  document.frmMakeFileKey.target="";
  document.frmMakeFileKey.submit();
}
//收取记录
function funcgetSelID(){
  var mystr="";
  var selCount=0;
  var str=document.all.item("userID");
  if(str==null) str="";
  if(str.length!=null){
	//alert("目前暂不支持批量操作。") return;
        for(i=0;i<str.length;i++){
          if(str[i].checked){//检查每条记录是否有选择
            mystr=mystr+str[i].value+";";
          }
        }
  }else  {
  	if(str.checked) mystr=str.value;
  }

  if(str.length!=null)	mystr=mystr.substring(0,mystr.length-1);
  return mystr;
}

//是否全选
function funIfSelectAll(selectId,usedID){
   //usedID 为每条记录可选的控件;selectId 为全选的控件
  if(selectId.checked)
    //alert("全选");
    if(usedID.length!=null){
    	for(i=0;i<usedID.length;i++){
      usedID[i].checked=true;
    	}
    }else
    	usedID.checked=true;
  else
    //alert("不全选");
    if(usedID.length!=null){
     for(i=0;i<usedID.length;i++){
      usedID[i].checked=false;
     }
    }else
    	usedID.checked=false;
}

function funSelUserCodes(){
  var sFeatures = "dialogWidth:670px;dialogHeight:580px;center:yes;status:no";
  var sPath = "/FlowEngineWeb/workflow/Public/FrameList.jsp?selFlag=4&frameFlag=2&userCode=";
  //格式为 222:3;333:3;test1=用户3、用户2、用户1;
  var sReturn = showModalDialog(sPath,"",sFeatures);
  if(sReturn==null)
    return;
  var aTmp = sReturn.split("=");
  document.all.item("selUserCodes").value=aTmp[0];
  document.all.item("selUserNames").value=aTmp[1];
}
</script>
<html>
<head>
<title>制作密钥盘文件
</title>
</head>
<body bgcolor="#ffffff" >
	<form name="frmMakeFileKey" method="post" action="" target="">
	  <input name="buttons" type="button" value="选择用户" onclick="funSelUserCodes();"><br>
    <textArea id="selUserNames" name="selUserNames"  cols="100" rows="3" value="" size="100"></TEXTAREA>
		<input name="buttonm" type="button" value="制作密钥盘" onclick="funMakeFileKey();">
		<table width="100%">
		  <tr class="">
		    <td width="2%" align="left"><input type="checkbox" id="selID" onClick="javascript:funIfSelectAll(selID,userID);"></td>
		    <td width="4%" align="center"><strong>用户帐户</strong></td>
		    <td width="8%" align="center"><strong>用户姓名</strong></td>
		  </tr>
		  <tr bgcolor="#FFFFFF" style="cursor:hand;">
		      <td align="left" ><input type="checkbox" id="userID"  value='<%=userCode%>'></td>
		      <td align="center" ><%=userCode%></td>
		      <td align="center" ><%=userName%></td>
		  </tr>
		</table>
	<input type="hidden" name="actionFlag"  value="">
	<input type="hidden" name="actionValue" value="">
	<input type="hidden" name="selUserCodes" value="">
	<input type="hidden" name="dirverSave" value="">
	</form>
</body>
</html>
<%}else
%>非系统管理员。
