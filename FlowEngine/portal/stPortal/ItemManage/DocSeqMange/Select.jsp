<%@ page contentType="text/html; charset=GBK" %>
<%
	//request.setCharacterEncoding("GBK");
%>
<link rel="stylesheet" type="text/css" href="../Common/Public.css">
<html>
<script>

 function initWindow(){
  }
 //能否发送响应
 function canSend(){
	return true;
 }

 function trimStr(a_strVal){
	return(a_strVal.replace(/^\s*|\s*$/g,""));
 }
 //能否保存响应
 function canSave(){
	return true;
 }
 //返回工作流可以显示的信息
 function getMessage(){
	return message;
 }
 //发送响应
 function send(){
	return true;
 }
 //保存响应
 function save(){
	return true;
 }

 function selectButton(){
	var sFeatures;
	sFeatures = "dialogHeight:500px;dialogWidth:400px;";
	sFeatures = sFeatures + "center:yes;edge:raised;help:no;scroll:yes;status:no;unadorned:yes;resizable:yes"
	var sReturn = window.showModalDialog("showSelect.jsp",null,sFeatures);
	if (sReturn != null)
	{
    document.all.item("Main1_Text8").value=sReturn; //测试所用，显示在本页面
		window.parent.document.all.item("Main1_Text88").value=sReturn; //表单上的控件名称没有"Main1_"才对应起来
	}
}
</script>
<body scroll="no">
<form action="" target="_self" method="post" name="" id="">
<table>
  <tr>
  	<td>
   		<input type="button" class="" value="选择" onclick="javascript:selectButton();" >
  	</td>
  </tr>
</table>
<input type="hidden" name="Main1_Text8" value=""> <!---测试所用------>
</form>
</body>
</html>
