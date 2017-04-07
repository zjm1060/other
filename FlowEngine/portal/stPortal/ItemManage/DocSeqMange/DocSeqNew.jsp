<%@ page contentType="text/html; charset=GBK" %>
<%@ page errorPage="../errorPage.jsp" %>
<%@ page import="minstone.ItemManage.BaseDbManage"%>
<%@ page import="minstone.ItemManage.WorkItem"%>
<%@ page import="java.util.Vector,java.util.Enumeration"%>
<%@ page import="oamodule.publiclib.util.RecordSetObject"%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>新增字号</title>
<link rel="stylesheet" type="text/css" href="../Common/Public.css">
<script language="javascript" src="../Common/Common.js"></script>
<script language="javascript" src="../Common/i_CheckInput.js"></script>
</head>
<script language="javascript">
window.name="frmNewDocSeq";
function doSave(){
    frmNewDocSeq.actionFlag.value="saveIt";
    frmNewDocSeq.submit();
}

function selRight(){

	//通过工作流通用的部门选择方式
  var sFeatures = "dialogWidth:670px;dialogHeight:580px;center:yes;status:no";
  var sPath = "/FlowEngineWeb/workflow/Public/FrameList.jsp?selFlag=3";
  //selFlag=3 单选；4多选;1,2 单选且只能用户，不列部门
  //格式为 222:3;333:3;test1=用户3、用户2、用户1;
  var sReturn = showModalDialog(sPath,"",sFeatures);
  if(sReturn==null)
    return;
  var aTmp = sReturn.split("=");
  sReturn=aTmp[0];
  //alert(sReturn);
  document.all.item("vaildRightCode").value= sReturn;
  document.all.item("vaildRightName").value= aTmp[1];
}

</script>
<%	request.setCharacterEncoding("GBK");
	String actionFlag=request.getParameter("actionFlag");
	if(actionFlag==null) actionFlag="";
	if(!actionFlag.equals("saveIt")){
%>

<body bgcolor="#ffffff">
<div align="center"><strong><font size="+3">新增字号</font></strong> </div>
<form name="frmNewDocSeq" method="post" action="DocSeqNew.jsp" target="frmNewDocSeq">
  <table width="100%" cellpadding="3" cellspacing="1" class="boarder">
    <tr class="content-bg">
      <td width="25%" bgcolor="#FFFFFF"><div align="center">字号名称</div></td>
      <td colspan="2" width="75%" bgcolor="#FFFFFF">
        <input name="itemCode" type="text" class="TextUtil_input" style="width:100%;" >
		</td>
    </tr>
    <tr class="content-bg">
      <td width="25%" bgcolor="#FFFFFF"><div align="center">字号地区</div></td>
      <td colspan="2" width="75%" bgcolor="#FFFFFF">
        <input name="itemName" type="text" class="TextUtil_input" style="width:100%;" >
		</td>
    </tr>
    <tr class="content-bg">
      <td width="25%" bgcolor="#FFFFFF"><div align="center">字号编码</div></td>
      <td colspan="2" width="75%" bgcolor="#FFFFFF">
        <input name="itemNo" type="text" class="TextUtil_input" style="width:100%;" >
		</td>
    </tr>
    <tr class="content-bg">
      <td width="25%" bgcolor="#FFFFFF"><div align="center">字号描述</div></td>
      <td colspan="2" width="75%" bgcolor="#FFFFFF">
        <input name="itemDesc" type="text" class="TextUtil_input" style="width:100%;" >
		</td>
    </tr>
    <tr class="content-bg">
      <td width="25%" bgcolor="#FFFFFF"><div align="center">使用权限</div></td>
      <td width="75%" bgcolor="#FFFFFF">
        <input name="vaildRightName" type="text" class="TextUtil_input" style="width:100%;" ></td>
      <td><div align="center"><img onclick="javascript:selRight()" style="cursor:hand;" src="../Common/Images/icon_urge.gif" width="16" height="16" alt="选择使用权限"></div></td>
		</td>
    </tr>
    <tr class="content-bg">
      <td bgcolor="#FFFFFF"><div align="center">是否生效</div></td>
      <td colspan="2" bgcolor="#FFFFFF">
        <input name="itemValid" type="checkbox" value="1">
      </td>
  </tr>
</table>
  <p align="center">
		<input name="actionFlag" type="hidden" id="actionFlag" >
		<input name="vaildRightCode" type="hidden" id="vaildRightCode" >
    <input name="btnSave" type="button" class="Btn_common" value="保存" onclick="javascript:doSave();">
    <input name="btnCancel" type="button" class="Btn_common" value="取消" onclick="javascript:document.location='DocSeqList.jsp';">
  </p>
</form>
</body>
<%}else{
	String itemCode=request.getParameter("itemCode");
	String itemName=request.getParameter("itemName");
	String itemNo=request.getParameter("itemNo");
	String itemDesc=request.getParameter("itemDesc");
	String vaildRightCode=request.getParameter("vaildRightCode");
	String vaildRightName=request.getParameter("vaildRightName");
	String sValid=request.getParameter("itemValid");
    	String itemValid ="0";
	if(sValid==null){
       		itemValid ="0";
	}else{
      	itemValid ="1";
	}

	BaseDbManage db=new BaseDbManage(request.getRemoteUser());
	WorkItem workItem=new WorkItem();
	workItem.setITEM_CODE(itemCode);
	workItem.setITEM_NAME(itemName);
	workItem.setITEM_NO(itemNo);
	workItem.setITEM_DESC(itemDesc);
	workItem.setITEM_VALID(itemValid);
	workItem.setITEM_RIGHTCODE(vaildRightCode);
	workItem.setITEM_RIGHTNAME(vaildRightName);

	boolean bSuccess= db.addDocSeq(workItem);
%>
<body>
<table width="100%" height="100%" border="0">
  <tr>
    <td>
        <%if(bSuccess){%>
          <script> document.location ="DocSeqList.jsp";</script>
		<%}else{%>
            <p align="center">新建字号失败!序列号可能重复。</p>
      		<p align="center"><a href="DocSeqList.jsp">返回</a></p></td>
		<%}%>
  </tr>
</table>
</body>
<%}%>
</html>

