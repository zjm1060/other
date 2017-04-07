function update(sValue)
{
  if ((sValue!="") && (sValue != null)){

			var aVal = sValue.split("=");

			document.all.item("i_app_dept").value = 1;
			document.all.item("i_app_dept_desc").value = aVal[2];

			document.all.item("c_scop_code").value = aVal[0];
			document.all.item("c_scop_type").value = aVal[1];
			document.all.item("c_scop_name").value = aVal[2];

			//alert(document.all.item("c_scop_code").value);
			//alert(document.all.item("c_scop_type").value);
			//alert(document.all.item("c_scop_name").value);
	}
}
function ErrorWindow(defaultErrorMsg,rederect,ErrorMsg) {

	strFeatures = "dialogWidth:460px;dialogHeight:100px;center:yes;status:no";
	sPath = "/Public/ErrorWindow.jsp?ErrorMsg=" + ErrorMsg + "&rederect=" + rederect + "&defaultErrorMsg=" + defaultErrorMsg;
	showModalDialog(sPath,window,strFeatures);

}
function ErrorWindow(defaultErrorMsg,ErrorMsg) {

	strFeatures = "dialogWidth:460px;dialogHeight:100px;center:yes;status:no";
	sPath = "/Public/ErrorWindow.jsp?ErrorMsg=" + ErrorMsg + "&defaultErrorMsg=" + defaultErrorMsg;
	showModalDialog(sPath,window,strFeatures);
}
function RightWindow(sPath,Msg) {

	strFeatures = "dialogWidth:330px;dialogHeight:180px;center:yes;status:no";
	sPath = sPath + "?Msg=" + Msg;
	showModalDialog(sPath,'',strFeatures);

}
function CheckRight(bModifyRight)
{

	if (bModifyRight == 0)
	{
		BlankInfoWindow('你没权限修改')

		window.close();
	}
}
function BlankInfoWindow(Msg)
{
		strFeatures = "dialogWidth:380px;dialogHeight:230px;center:yes;status:no";
		sPath = "/Public/ActionTipsWindow.jsp?Msg=" + Msg ;
		var bflag=showModalDialog(sPath,'',strFeatures);
}

function DelWindow(Msg)
{
		strFeatures = "dialogWidth:380px;dialogHeight:230px;center:yes;status:no";
		sPath = "/Public/TipsWindow.jsp?Msg=" + Msg ;
		var bflag=showModalDialog(sPath,'',strFeatures);
		return bflag;
}
