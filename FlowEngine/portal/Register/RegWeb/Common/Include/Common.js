/*********************************/
/*功能：弹出装载操作确认页的无态窗口*/
/*参数：TipsPagePath  指定操作确认页的路径*/
/*      Msg           指定要确认的语句*/
/*      ActionPage    指定操作将要提交到的页面（包括详细路径，参数等；如：/OaBorderModule/CarManage/Manage/CarInfo/_Del.jsp?sRecords=25）*/
/*返回值：布尔值*/
/*********************************/
function ActionWindow(TipsPagePath,Msg,ActionPage)
{
  strFeatures = "dialogWidth:380px;dialogHeight:180px;center:yes;status:no";
  sPath = TipsPagePath + "TipsWindow.jsp?ActionPage=" + escape(ActionPage);
  var bflag=showModalDialog(sPath,Msg,strFeatures);
  return bflag;
}
/*********************************/
/*功能：弹出装载操作确认页的无态窗口*/
/*参数：TipsPagePath  指定操作确认页的路径*/
/*      Msg           指定要确认的语句*/
/*返回值：布尔值*/
/*********************************/
function ConfirmWindow(TipsPagePath,Msg)
{
  strFeatures = "dialogWidth:380px;dialogHeight:180px;center:yes;status:no";
  sPath = TipsPagePath + "ConfirmWindow.jsp";
  var bflag=showModalDialog(sPath,Msg,strFeatures);
  return bflag;
}
function ConfirmWindow_Ex(TipsPagePath,Msg)
{
  strFeatures = "dialogWidth:380px;dialogHeight:180px;center:yes;status:no";
  sPath = TipsPagePath + "ConfirmWindow_Ex.jsp";
  var bflag=showModalDialog(sPath,Msg,strFeatures);
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
/*功能：弹出装载操作成功提示页的无态窗口*/
/*参数：TipsPagePath  指定操作成功提示页的路径*/
/*      Msg           指定成功提示语句*/
/*********************************/
function SucceedWindow(TipsPagePath,Msg)
{
  strFeatures = "dialogWidth:380px;dialogHeight:180px;center:yes;status:no";
  sPath = TipsPagePath + "SucceedWindow.jsp";
  showModalDialog(sPath,Msg,strFeatures);
}
/*********************************/
/*功能：弹出正在处理提示页的无态窗口*/
/*参数：TipsPagePath  指定正在处理提示页的路径*/
/*      FuncName      指定函数名称*/
/*      FuncPara      指定函数参数*/
/*********************************/
function openHaddleingWindow(TipsPagePath,FuncName,FuncPara,DialogType){
  var obj;
  strFeatures = "dialogWidth:380px;dialogHeight:50px;center:yes;status:no";
  sPath = TipsPagePath + "haddleingPage.jsp?funcName="+FuncName+"&funcPara="+FuncPara;
  if(arguments.length==3)
    obj=showModalDialog(sPath,window,strFeatures);
  if(arguments.length==4){
    if(DialogType==1)
      obj=showModalDialog(sPath,window,strFeatures);
    if(DialogType==2)
      obj=showModelessDialog(sPath,window,strFeatures);
  }
  return obj;
}
//下面为工作流所用
function checkPage(TipsPagePath,pageCount)
{
    var pageNo = document.all.item("c_page").value;
    if(pageNo == "") {
        AlertWindow(TipsPagePath,"请您填上页码！");
        document.all.item("c_page").focus();
        return false;
    }else if(isNaN(pageNo)) {
        AlertWindow(TipsPagePath,"请您注意，您填写的页码不是一个数字！");
        document.all.item("c_page").value="";
        document.all.item("c_page").focus();
        return false;
    }else if(parseInt(pageNo)<1 || parseInt(pageNo)>pageCount){
        AlertWindow(TipsPagePath,"请您注意，页码要在（1－" + pageCount + "）范围内！");
        document.all.item("c_page").value="";
        document.all.item("c_page").focus();
        return false;
    }
    return true;
}

function isSubmit(pageCount)
{
    if(checkPage(pageCount) == false)
       return false;

    gotoPage(document.all.item("c_page").value);
    return true;
}

function selectMan(aShowName,aHiddenName,aSelFlag,aFrameFlag)
{
    var show = document.all.item(aShowName).value;
    var value = document.all.item(aHiddenName).value;
    var feature = "dialogWidth:563px;dialogHeight:480px;center:yes;status:no";
    var url = "Public/FrameList.jsp?selFlag=" + aSelFlag + "&frameFlag=" + aFrameFlag;
    var tmp = showModalDialog(url,value + "=" + show,feature);
    if(tmp != null){
        var users = tmp.split("=");
        document.all.item(aShowName).value = users[1];
        document.all.item(aHiddenName).value = users[0];
    }
}

function selectList(aUrlPara,aControlName,aHiddenName)
{
    var feature = "dialogWidth:477px;dialogHeight:473px;center:yes;status:no";
    var url = 'SelectListFrame.jsp?' + aUrlPara;
    var struct = window.showModalDialog(url,this,feature);
    if (struct != null){
        document.all.item(aControlName).value = struct[0];
        document.all.item(aHiddenName).value = struct[1];
    }
    return;
}

function selectButton(aUrlPara,aControlList)
{
    var feature = "dialogWidth:477px;dialogHeight:293px;center:yes;status:no";
    var url = 'SelectButtonFrame.jsp?' + aUrlPara;
    var struct = window.showModalDialog(url,this,feature);
    if (struct != null){
        var controlStruct = aControlList.split(";");
        var i = 0;
        for(i=0;i<controlStruct.length;i++){
          var control = document.all.item(controlStruct[i]);
          if(control != null){
            var value = "";
            for(j=0;j<struct.length;j++){
              if(struct[j][i] == "&nbsp;")
                 continue;
              if(value != "")
                value += ";";
              value += struct[j][i];
            }
            control.value = value;
          }
        }
    }
    return;
}

function my_unescape(aValue)
{
  return unescape(aValue);
  //return aValue;
}

//发件人等字段的超链
function UserInfo(sUserCode){
  var feature = "dialogWidth:605px;dialogHeight:393px;center:yes;status:no";
  var path = "UserInfo.jsp?sUserCode="+sUserCode;
  showModalDialog(path,"",feature);
}