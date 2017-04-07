/*********************************/
/*功能：弹出装载操作警告页的无态窗口*/
/*参数：TipsPagePath  指定操作警告页的路径*/
/*      Msg           指定要警告的语句*/
/*********************************/
function AlertWindow(TipsPagePath,Msg)
{
  strFeatures = "dialogWidth:460px;dialogHeight:100px;center:yes;status:no";
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
