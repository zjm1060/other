//关于生成导航页面
var sHtml="",Mainstring="",Substring="",sMain="",sSub="";
var arrSub;
var arrSubRows;
var irowWidth=100,irowHeight=20,iTotalWidth=0,iTotalHeight=20,iNum=0;
var iImgTdWidth=20;
var initLeft=10,initTop=70;

function getStrAllocateLen(str){
	var iLen=0;
	var iUnicode;

	for(i=0;i<str.length;i++){
		iUnicode=str.charCodeAt(i);
		if (iUnicode<=128){				//英文字符
			iLen+=1;
		}else{
			if (iUnicode>128 && iUnicode<=255){
				iLen+=2;
			}else{
				if (iUnicode>=4112){	//中文字符
					iLen+=2;
				}else{
					iLen+=1;			//其他字符
				}
			}
		}
	}
	return(iLen);
}
//asParent：父节点ID，没有父节点为null；asCode：节点ID；asName:节点描述；asLinkPage：连接页
function addTd(asParent,asCode,asName,asLinkPage)
{
  if (asParent == null)
  {
    if (Mainstring == "")
      Mainstring = asCode;
    else
      Mainstring = Mainstring + ";" + asCode;
    var iNameLen=getStrAllocateLen(asName);
    if(iNameLen>=16){
		iTotalHeight=35;
    }
    sMain = sMain + "<td align=\"center\"><div id=\"menubar_"+iNum+"\" style=\"vertical-align:middle;cursor:hand;color:white;width="+irowWidth+";height:menubar_____height;border-width:1px;border-style:outset;background:#003399;padding-top:2px;\" onclick=\"javascript:Navigation('" + asLinkPage + "');\" onmouseover=\"menuControl(1)\" onmouseout=\"menuControl(0)\">" + asName + "</div></td>";
    iNum = iNum + 1;
  }
  if (asParent != null)
  {
    var temp = "<span style=\"cursor:hand;height:"+irowHeight+"px;text-align:left;padding-left:15px;padding-top:4px;\" onmouseover=\"javascript:this.style.backgroundColor='#5ba7fd';this.style.color='white';\" onmouseout=\"javascript:this.style.backgroundColor='#97c2f3';this.style.color='#0033cc';\" onclick=\"javascript:Navigation('"+asLinkPage+"');\">"+asName+"</span><br>";
    if (Substring == "")
    {
      Substring = asParent;
      arrSub = new Array(1);
      arrSub[0] = temp;
      arrSubRows = new Array(1);
      arrSubRows[0] = 1;
    }
    else
    {
      var re = Substring.indexOf(asParent);
      if (re == -1)
      {
        Substring = Substring + ";" + asParent;
        var arrTemp = arrSub;
        arrSub = new Array(arrTemp.length+1);
        for (i=0;i<arrTemp.length;i++)
        {
          arrSub[i] = arrTemp[i];
        }
        arrSub[i] = temp;
        var arrTempSubRows = arrSubRows;
        arrSubRows = new Array(arrTempSubRows.length+1);
        for (j=0;j<arrTempSubRows.length;j++)
        {
          arrSubRows[j] = arrTempSubRows[j];
        }
        arrSubRows[j] = 1;
      }
      else
      {
        var arr = Substring.split(";");
        for (i=0;i<arr.length;i++)
        {
          if (asParent == arr[i])
            break;
        }
        arrSub[i] = arrSub[i] + temp;
        arrSubRows[i] = arrSubRows[i] + 1;
      }
    }
  }
}

function initTable()
{
  if(Mainstring==null || Mainstring=="") return;
  var arr1 = Mainstring.split(";");
  if (arr1.length > 0)
    iTotalWidth = irowWidth*(arr1.length)+2*iImgTdWidth;
  var temp1="",temp2="",temp3="",temp4="",temp5="";
  //构造js函数
  temp1 = "<html><head><scr"+"ipt language=\"javascript\" src=\"include/MoveMenu.js\"></scr"+"ipt><scri"+"pt language=\"javascr"+"ipt\">var SubInitOffsetLeft=1,SubInitOffsetTop=1;SubInitOffsetLeft=parent.getInitLeft();SubInitOffsetTop=parent.getInitTop();</scr"+"ipt>";
  //构造style
  temp1 = temp1 + "<style>table{font-size:9pt;}</style></head>";
  //构造div
  temp1 = temp1 + "<body style=\"background-color:#F0F3FC;\" scroll=\"no\">";
  temp1 = temp1 + "<table width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\" style=\"background-color:white;border: #a0a0a0 1px solid;\">";
  temp3 = "</table></body></html>";
  if(irowHeight!=iTotalHeight){
  	var re;
  	re=re = /menubar_____height/g;
  	sMain=sMain.replace(re,""+iTotalHeight);
  }
  temp2 = "<tr><td align=\"center\" style=\"cursor:move;width:"+(iImgTdWidth-4)+"px;border-top:#eeeeee 2px solid;border-left:#eeeeee 2px solid;border-right:#333366 2px solid;border-bottom:#333366 2px solid;background:#d4d0c8;background-image: url(/stPortal/images/movemenu.gif);background-repeat: no-repeat;background-position: center center;\" title=\"移动菜单\" onmousedown=\"javascript:Drag();\" onmouseUp=\"javascript:undoDrag();\">&nbsp;</td>"+sMain+"<td align=\"center\" style=\"cursor:hand;width:"+(iImgTdWidth-4)+"px;border-top:#eeeeee 2px solid;border-left:#eeeeee 2px solid;border-right:#333366 2px solid;border-bottom:#333366 2px solid;background:#d4d0c8;\" onclick=\"javascript:closeIt();\"><img src=\"images/SelectNot.gif\" border=\"0\" alt=\"关闭菜单\"></td></tr>";
  var arr2 = Substring.split(";");
  var oSubDiv = document.all.item("SubDiv");
  var ay = new Array(arr1.length);
  for (i=0;i<arr1.length;i++)
  {
    var str = arr1[i];
    var TempSubstring = ";" + Substring + ";";
    var re = TempSubstring.indexOf(";"+str+";");
    var iLeft = (i*irowWidth)+iImgTdWidth;
    if (re == -1)
    {
      oSubDiv.innerHTML = oSubDiv.innerHTML + "<iframe id=\"menu_"+i+"\" marginWidth=\"0\" marginHeight=\"0\" frameBorder=\"0\" iTopValue=\"0\" iLeftValue=\"0\" style=\"position:absolute;top:0px;left:0px;width:0px;height:0px;display:none;\"></iframe>";
      temp4 = "";
    }
    else
    {
      for (j=0;j<arr2.length;j++)
      {
        if (arr2[j] == str)
          break;
      }
      var iHeight = irowHeight*arrSubRows[j];
      oSubDiv.innerHTML = oSubDiv.innerHTML + "<iframe id=\"menu_"+i+"\" marginWidth=\"0\" marginHeight=\"0\" frameBorder=\"0\" iTopValue=\""+(iTotalHeight-1)+"\" iLeftValue=\""+iLeft+"\" style=\"position:absolute;left:0px;top:0px;width:"+(irowWidth+35)+"px;height:"+(iHeight+5)+"px;display:none;\"></iframe>";
      temp4 = "<html><head><style>body{font-size:9pt;color:#0033cc;}</style><scr"+"ipt language=\"javasc"+"ript\">var id='menu_"+i+"';var obj=parent.document.all.item(id);function keepMenu(){obj.style.display='block';}function hideMenu(){obj.style.display='none';}function Navigation(asPage){parent.ifrmDashBoardContent.window.location=asPage;}</scr"+"ipt></head><body leftmargin=\"0\" rightmargin=\"0\" topmargin=\"0\" style=\"background-color:#F0F3FC;\" scroll=\"no\"><div align=\"left\" style=\"position:absolute;left:0px;top:0px;width:100%;height:"+iHeight+"px;border-width:2px;border-style:outset;border-color:white sliver sliver white;background-color:#97c2f3;\" onmouseover=\"javascript:keepMenu();\" onmouseout=\"javascript:hideMenu();\">"+arrSub[j]+"</div></body></html>";
    }
    ay[i] = temp4;
  }
  sHtml = temp1 + temp2 + temp3;
  window.frames.MoveiFrame.document.writeln(sHtml);

  for (o=0;o<ay.length;o++)
  {
    window.frames("menu_"+o).document.writeln(ay[o]);
  }
}
//结束

function displayNavi()
{
  document.all.item("MoveiFrame").style.display = "";
}

function getInitLeft()
{
  return initLeft;
}

function getInitTop()
{
  return initTop;
}

//zxc
function addNacTd(asParent,asCode,asName,asType){
	addTd(asParent,asCode,asName,'dashboardEx.jsp?DashboardID='+asCode+'&DashboardType='+asType);
}