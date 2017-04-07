/*********************************/
/*功能：弹出装载操作警告页的无态窗口*/
/*参数：TipsPagePath  指定操作警告页的路径*/
/*      Msg           指定要警告的语句*/
/*********************************/
function AlertWindow(TipsPagePath,Msg)
{
  strFeatures = "dialogWidth:380px;dialogHeight:210px;center:yes;status:no";
  sPath = TipsPagePath + "AlertWindow.jsp";
  showModalDialog(sPath,Msg,strFeatures);
}

/*********************************/
/*功能：弹出装载操作确认页的无态窗口*/
/*参数：TipsPagePath  指定操作确认页的路径*/
/*      Msg           指定要确认的语句*/
/*返回值：布尔值*/
/*********************************/
function ConfirmWindow(TipsPagePath,Msg)
{
  strFeatures = "dialogWidth:380px;dialogHeight:230px;center:yes;status:no";
  sPath = TipsPagePath + "ConfirmWindow.jsp";
  bflag = showModalDialog(sPath,Msg,strFeatures);
  return bflag;
}
/*********************************/
/*功能：弹出装载操作成功提示页的无态窗口*/
/*参数：TipsPagePath  指定操作成功提示页的路径*/
/*      Msg           指定成功提示语句*/
/*********************************/
function SucceedWindow(TipsPagePath,Msg)
{
  strFeatures = "dialogWidth:380px;dialogHeight:210px;center:yes;status:no";
  sPath = TipsPagePath + "SucceedWindow.jsp";
  showModalDialog(sPath,Msg,strFeatures);
}
function openFrameList(aShowName,aHiddenName,aSelFlag,aFrameFlag,aFrameLevel,aFrameLevelTo,aFrameList,
						dispGroupNode,dispPersGroupNode,dispAllUserNode,groupNodeDesc,
						persGroupNodeDesc,groupList,dataSource,modifyMode){
	var show = document.getElementById(aShowName).value;
    var value = document.getElementById(aHiddenName).value;
    var feature = "dialogWidth:720px;dialogHeight:615px;center:yes;status:no;help:no";
    var url = "/FlowEngineWeb/workflow/Public/FrameList.jsp?selFlag=" + aSelFlag + "&frameFlag=" + aFrameFlag+"&frameLevel="+aFrameLevel+"&frameLevelTo="+aFrameLevelTo+"&frameList="+aFrameList;
    url=url+"&dispGroupNode="+dispGroupNode+"&dispPersGroupNode="+dispPersGroupNode+"&dispAllUserNode="+dispAllUserNode;
	url=url+"&groupNodeDesc="+groupNodeDesc+"&persGroupNodeDesc="+persGroupNodeDesc+"&groupList="+groupList+"&dataSource="+dataSource;
    url=url+"&modifyMode="+modifyMode;
    var tmp = showModalDialog(url,value + "=" + show,feature);
    if(tmp != null){
        var users = tmp.split("=");
        document.getElementById(aShowName).value = users[1];
        document.getElementById(aHiddenName).value = users[0];
    }
}