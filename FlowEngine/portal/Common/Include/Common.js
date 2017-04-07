/*********************************/
/*功能：弹出装载操作确认页的无态窗口*/
/*参数：TipsPagePath  指定操作确认页的路径*/
/*      Msg           指定要确认的语句*/
/*      ActionPage    指定操作将要提交到的页面（包括详细路径，参数等；如：/OaBorderModule/CarManage/Manage/CarInfo/_Del.jsp?sRecords=25）*/
/*返回值：布尔值*/
/*********************************/
function ActionWindow(TipsPagePath,Msg,ActionPage)
{
  strFeatures = "dialogWidth:400px;dialogHeight:250px;center:yes;status:no";
  sPath = TipsPagePath + "TipsWindow.jsp?Msg=" + Msg + "&ActionPage=" + escape(ActionPage);
  var bflag=showModalDialog(sPath,window,strFeatures);
  return bflag;
}
/*********************************/
/*功能：弹出装载操作警告页的无态窗口*/
/*参数：TipsPagePath  指定操作警告页的路径*/
/*      Msg           指定要警告的语句*/
/*********************************/
function AlertWindow(TipsPagePath,Msg)
{
    strFeatures = "dialogWidth:460px;dialogHeight:100px;center:yes;status:no";
    sPath = TipsPagePath + "AlertWindow.jsp?Msg=" + Msg;
    showModalDialog(sPath,window,strFeatures);
}
/*********************************/
/*功能：弹出装载操作失败提示页的无态窗口*/
/*参数：TipsPagePath  指定操作失败提示页的路径*/
/*      Msg           指定失败提示语句*/
/*      ErrorInfo     指定导致操作失败的出错信息*/
/*********************************/
function ErrorWindow(TipsPagePath,Msg,ErrorInfo)
{
    strFeatures = "dialogWidth:460px;dialogHeight:100px;center:yes;status:no";
    sPath = TipsPagePath + "ErrorWindow.jsp?Msg=" + Msg + "&ErrorInfo=" + ErrorInfo;
    showModalDialog(sPath,window,strFeatures);
}
/*********************************/
/*功能：弹出装载操作成功提示页的无态窗口*/
/*参数：TipsPagePath  指定操作成功提示页的路径*/
/*      Msg           指定成功提示语句*/
/*********************************/
function SucceedWindow(TipsPagePath,Msg)
{
    strFeatures = "dialogWidth:380px;dialogHeight:230px;center:yes;status:no";
    sPath = TipsPagePath + "SucceedWindow.jsp?Msg=" + Msg;
    return showModalDialog(sPath,window,strFeatures);
}
/*********************************/
/*功能：弹出装载选择用户页的无态窗口*/
/*参数：PageURL        指定用户选择页（如：/OaBorderModule/Public/FrameList.jsp*/
/*      sOldUserCode   指定原有用户的代码*/
/*      sOldUserName   指定原有用户的名称*/
/*      sNewUserCode   得到新选定用户的代码*/
/*      sNewUserName   得到新选定用户的名称*/
/*********************************/
function ChooseUserWindow(PageURL,sOldUserCode,sOldUserName,sNewUserCode,sNewUserName)
{
    strFeatures = "dialogWidth:550px;dialogHeight:480px;center:yes;status:no";
    if (sOldUserCode != "" && sOldUserName != "")
        sOldUserInfo = sOldUserCode + "=" + sOldUserName;
    else
        sOldUserInfo = "";
    var bflag=showModalDialog(PageURL,sOldUserInfo,strFeatures);
    if (bflag != "" && bflag != "undefined" && bflag != undefined)
    {
        sArr1 = bflag.split("=");
        if (sArr1.length == 2)
        {
            sNewUserCode = sArr1[0];
            sNewUserName = sArr1[1];
        }
    }
}
