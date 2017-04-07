var treeHtml="";                                       //构造菜单树的html
var arrTree=new Array();                               //菜单树所有节点组成的数组
var treeNodeIDs="";                                    //菜单树所有节点ID以“；”分隔串结成的字符串
var treeBlankWidth=16;                                 //上一级节点与下一级节点之间向右缩进的宽度
var treeImageWidth=16;                                 //展开与折叠标示图片的宽度
var treeExpandImage="/stPortal/images/tree_arrow_show.gif";      //展开标示图片
var treeCloseImage="/stPortal/images/tree_arrow_hide.gif";       //折叠标示图片
var treeLayerFlag=1;                                   //菜单树当前层标示
var treeSelectedTd="";                                 //标志菜单树当前被选中的节点
var treeIfExpand=1;                                    //当treeIfExpand=1时，菜单树节点展开；当treeIfExpand=0时，菜单树节点折叠


/************************/
/*功能：生成菜单树*/
/*参数：*/
/*   aDivName：指定菜单树所在位置的div名称*/
/*aExpandFlag：当aExpandFlag=1时，菜单树节点展开；当aExpandFlag=0时，菜单树节点折叠*/
/************************/
function initTree(aDivName,aExpandFlag){
  treeIfExpand=aExpandFlag;
  if(treeNodeIDs!=null && treeNodeIDs!=""){
    var arrTemp=treeNodeIDs.split(";");
    for(i=0;i<arrTemp.length-1;i++){
      var strIndex=arrTemp[i];
      if(arrTree[strIndex]!=null && arrTree[strIndex]["pid"]==""){
        createTree(arrTree[strIndex],1);
      }
    }
    treeHtml="<table id=\"treeTable\" name=\"treeTable\" width=\"160\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">"+treeHtml+"</table>";
    document.all.item(aDivName).innerHTML=treeHtml;
  }
}
/************************/
/*功能：构造菜单树数组，暂存在全局变量arrTree中*/
/*参数：*/
/*       aID：节点ID*/
/*      aPID：父节点ID*/
/*     aDesc：节点描述*/
/*aClickFunc：点击节点完成的操作，当取值“”时，默认nodeClick()；可以取值例如：‘Test();’*/
function addNode(aID,aPID,aDesc,aClickFunc){
  if(aID==null || aID=="")
    return;
  if(aPID!=null && aPID!=""){
    if(arrTree[aPID]==null)
      arrTree[aID]=null;
    else{
      var temp=arrTree[aPID]["children"];
      if(temp=="")
        arrTree[aPID]["children"]=aID;
      else
        arrTree[aPID]["children"]=temp+";"+aID;
    }
  }else{
    aPID="";
  }
  var arrNode=new Array();
  arrNode["id"]=aID;
  arrNode["pid"]=aPID;
  arrNode["desc"]=aDesc;
  var clickFunc="nodeClick();";
  if(aClickFunc!=null && aClickFunc!="")
    clickFunc=aClickFunc
  arrNode["clickfunc"]=clickFunc;
  arrNode["children"]="";
  arrTree[aID]=arrNode;
  treeNodeIDs=treeNodeIDs+aID+";";
}
/************************/
/*功能：构造树节点的html，暂存在全局变量treeHtml中*/
/*参数：*/
/* aNode：节点数组*/
/*aIndex：节点层次；根节点为1，依次递增*/
/************************/
function createTree(aNode,aIndex){
  var strStyle=" style=\"display:block;\"";
  if(treeIfExpand==1)
    strStyle=" style=\"display:block;\"";
  else
    strStyle=" style=\"display:none;\"";
  var tableHead="<tr id=\""+aNode["pid"]+"_children\" name=\""+aNode["pid"]+"_children\""+strStyle+"><td><table id=\""+aNode["pid"]+"_table\" name=\""+aNode["pid"]+"_table\" width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">";
  var tableTail="</table></td></tr>";
  if(treeLayerFlag==aIndex)
    treeHtml=treeHtml+getNodeHtml(aNode,aIndex);
  else{
    if(treeLayerFlag>aIndex){
      var tempTail="";
      while(treeLayerFlag-1>=aIndex){
        tempTail=tempTail+tableTail
        treeLayerFlag=treeLayerFlag-1;
      }
      treeHtml=treeHtml+tempTail+getNodeHtml(aNode,aIndex);
      treeLayerFlag=aIndex;
    }else{
      treeHtml=treeHtml+tableHead+getNodeHtml(aNode,aIndex);
      treeLayerFlag=aIndex;
    }
  }
  if(aNode["children"]!=""){
    var arrTemp = aNode["children"].split(";");
    var tempIndex=0;
    while(tempIndex<arrTemp.length){
      var tempNode=arrTree[arrTemp[tempIndex]];
      tempIndex=tempIndex+1;
      createTree(tempNode,aIndex+1);
    }
  }
}
/************************/
/*功能：参照createTree*/
/************************/
function getNodeHtml(aNode,aIndex){
  var tempID=aNode["id"];
  var tempPID=aNode["pid"];
  var breakHtml="<tr id=\""+tempID+"_break\" name=\""+tempID+"_break\" nodeID=\""+tempID+"\"><td width=\"100%\" class=\"breakL1\"></td></tr>";
  var imageHtml="";
  var tempBreak;
  var tempHtml;
  var hasChildren=false;

  if(aNode["children"]!=""){
    hasChildren=true;
    tempBreak=treeBlankWidth*aIndex;
    var tempImage=treeExpandImage;
    var tempExpand=1;
    if(treeIfExpand==1){
      tempImage=treeExpandImage;
      tempExpand=1;
    }else{
      tempImage=treeCloseImage;
      tempExpand=0;
    }
    imageHtml="<img id=\""+tempID+"_img\" name=\""+tempID+"_img\" nodeID=\""+tempID+"\" nodePID=\""+tempPID+"\" level=\""+aIndex+"\" expandFlag=\""+tempExpand+"\" src=\""+tempImage+"\" border=\"0\" style=\"cursor:hand;\">";
  }else{
    tempBreak=treeBlankWidth*aIndex+treeImageWidth;
    imageHtml="";
  }
  var trHtml="";
  if(!hasChildren)
  	trHtml="<tr onclick=\"javascript:selectNodeAndExpandNode(false);\" id=\""+tempID+"_tr\" name=\""+tempID+"_tr\" nodeID=\""+tempID+"\" nodePID=\""+tempPID+"\" level=\""+aIndex+"\"><td id=\""+tempID+"_td\" name=\""+tempID+"_td\" nodeID=\""+tempID+"\" nodePID=\""+tempPID+"\" level=\""+aIndex+"\" nodeClass=\"normalL"+aIndex+"\" width=\"100%\" class=\"normalL"+aIndex+"\" style=\"padding-left:"+tempBreak+"px;\">"+imageHtml+"<span id=\""+tempID+"_word\" name=\""+tempID+"_word\" nodeID=\""+tempID+"\" nodePID=\""+tempPID+"\" level=\""+aIndex+"\" style=\"cursor:hand;\">"+aNode["desc"]+"</span></td></tr>";
  else
  	trHtml="<tr onclick=\"javascript:selectNodeAndExpandNode(true);\" id=\""+tempID+"_tr\" name=\""+tempID+"_tr\" nodeID=\""+tempID+"\" nodePID=\""+tempPID+"\" level=\""+aIndex+"\"><td id=\""+tempID+"_td\" name=\""+tempID+"_td\" nodeID=\""+tempID+"\" nodePID=\""+tempPID+"\" level=\""+aIndex+"\" nodeClass=\"normalL"+aIndex+"\" width=\"100%\" class=\"normalL"+aIndex+"\" style=\"padding-left:"+tempBreak+"px;\">"+imageHtml+"<span id=\""+tempID+"_word\" name=\""+tempID+"_word\" nodeID=\""+tempID+"\" nodePID=\""+tempPID+"\" level=\""+aIndex+"\" style=\"cursor:hand;\">"+aNode["desc"]+"</span></td></tr>";
  if(aIndex==1 && treeHtml!="")
    tempHtml=breakHtml+trHtml;
  else
    tempHtml=trHtml;
  return tempHtml;
}
function selectNodeAndExpandNode(hasChildren){
	var obj=window.event.srcElement;
  	var objNode=arrTree[obj.nodeID];
  	if(objNode!=null){
		if(hasChildren){
			expandOrClose();
			selectNode();
		}
		else{
			selectNode();
		}
		var lClickFunc=objNode["clickfunc"];
		//alert(lClickFunc);
		if(lClickFunc!=null)
			eval(lClickFunc);
	}
}
/************************/
/*功能：展开或折叠树节点*/
/************************/
function expandOrClose(){
  var obj=window.event.srcElement;
  var objImage=document.all.item(obj.nodeID+"_img");
  var objTable=document.all.item(obj.nodeID+"_children");
  if(objImage.expandFlag=="1"){
    objImage.src=treeCloseImage;
    objImage.expandFlag="0";
    objTable.style.display="none";
  }else{
    objImage.src=treeExpandImage;
    objImage.expandFlag="1";
    objTable.style.display="block";
  }
}
/************************/
/*功能：点击选中树节点*/
/************************/
function selectNode(){
  var obj=window.event.srcElement;
  var objTd=document.all.item(obj.nodeID+"_td");
  if(treeSelectedTd!=""){
    var objOld=document.all.item(treeSelectedTd);
    objOld.className=objOld.nodeClass;
  }
  objTd.className="selectL";
  treeSelectedTd=obj.nodeID+"_td";
}
/************************/
/*功能：点击树节点完成操作*/
/************************/
function nodeClick(){
  var objWord=window.event.srcElement;
  alert(objWord.innerText);
}
/************************/
/*功能：添加菜单树节点*/
/*参数：*/
/*       aID：节点ID*/
/*     aDesc：节点描述*/
/*aClickFunc：点击节点完成的操作，当取值“”时，默认nodeClick()；可以取值例如：‘Test();’*/
/************************/
function addTreeNode(aID,aDesc,aClickFunc){
  if(treeSelectedTd==""){
    AlertWindow("Public/","请先选择节点！");
    return;
  }else{
    if(arrTree[aID]!=null){
      AlertWindow("Public/","已经存在相同ID的节点！");
      return;
    }
    var currentTd=document.all.item(treeSelectedTd);
    var newNodeLevel=parseInt(currentTd.level)+1;
    var oNode=arrTree[currentTd.nodeID];
    var strChildren=oNode["children"];
    if(strChildren==""){
      //添加节点数组
      addNode(aID,oNode["id"],aDesc,aClickFunc);
      //添加新节点
      var newNodeHtml=getNodeHtml(arrTree[aID],newNodeLevel);
      var newTr=document.createElement("tr");
      var currentTr=currentTd.parentElement;
      currentTr.insertAdjacentElement("AfterEnd",newTr);
      newTr.id=oNode["id"]+"_children";
      newTr.name=oNode["id"]+"_children";
      var tempHtml="<table id=\""+oNode["id"]+"_table\" name=\""+oNode["id"]+"_table\" width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">"+newNodeHtml+"</table>";
      var newTd = newTr.insertCell();
      newTd.innerHTML=tempHtml;
      //调整原选中节点的展开标示
      var imageHtml="<img id=\""+oNode["id"]+"_img\" name=\""+oNode["id"]+"_img\" nodeID=\""+oNode["id"]+"\" nodePID=\""+oNode["pid"]+"\" level=\""+currentTd.level+"\" expandFlag=\"1\" src=\""+treeExpandImage+"\" border=\"0\" style=\"cursor:hand;\">";
      var oldHtml=currentTd.innerHTML;
      currentTd.innerHTML=imageHtml+oldHtml;
      currentTd.style.paddingLeft=treeBlankWidth*parseInt(currentTd.level);
      //调整当前选中节点
      currentTd.className=currentTd.nodeClass;
      document.all.item(aID+"_td").className="selectL";
      treeSelectedTd=aID+"_td";
    }else{
      //添加节点数组
      addNode(aID,oNode["id"],aDesc,aClickFunc);
      //添加新节点
      oTable=document.all.item(oNode["id"]+"_table");
      var newTr=oTable.insertRow();
      newTr.id=aID+"_tr";
      newTr.name=aID+"_tr";
      newTr.nodeID=aID;
      newTr.nodePID=oNode["id"];
      newTr.level=newNodeLevel;
      var newTd=newTr.insertCell(0);
      newTd.id=aID+"_td";
      newTd.name=aID+"_td";
      newTd.nodeID=aID;
      newTd.nodePID=oNode["id"];
      newTd.level=newNodeLevel;
      newTd.nodeClass="normalL"+newNodeLevel;
      newTd.className="selectL";
      newTd.width="100%";
      var paddingLeft=treeBlankWidth*parseInt(newNodeLevel)+treeImageWidth;
      newTd.style.paddingLeft=paddingLeft+"px";
      //newTd.attachEvent("onclick",selectNode);
      newTd.innerHTML="<span id=\""+aID+"_word\" name=\""+aID+"_word\" nodeID=\""+aID+"\" nodePID=\""+oNode["id"]+"\" level=\""+newNodeLevel+"\" style=\"cursor:hand;\">"+aDesc+"</span></td>";
      //调整当前选中节点
      currentTd.className=currentTd.nodeClass;
      treeSelectedTd=aID+"_td";
    }
  }
}
/************************/
/*功能：添加菜单树根节点*/
/*参数：*/
/*       aID：节点ID*/
/*     aDesc：节点描述*/
/*aClickFunc：点击节点完成的操作，当取值“”时，默认nodeClick()；可以取值例如：‘Test();’*/
/************************/
function addTreeRoot(aID,aDesc,aClickFunc){
  if(arrTree[aID]!=null){
    AlertWindow("Public/","已经存在相同ID的节点！");
    return;
  }
  //添加节点数组
  addNode(aID,"",aDesc,aClickFunc);
  //添加节点
  var oTable=document.all.item("treeTable")
  if(oTable.rows.length>=1){
    var oBreakTr=oTable.insertRow();
    oBreakTr.id=aID+"_break";
    oBreakTr.name=aID+"_break";
    oBreakTr.nodeID=aID;
    var oBreakTd=oBreakTr.insertCell(0);
    oBreakTd.width="100%";
    oBreakTd.className="breakL1";
  }
  var oTr=oTable.insertRow();
  oTr.id=aID+"_tr";
  oTr.name=aID+"_tr";
  oTr.nodeID=aID;
  oTr.nodePID="";
  oTr.level=1;
  var oTd=oTr.insertCell(0);
  oTd.id=aID+"_td";
  oTd.name=aID+"_td";
  oTd.nodeID=aID;
  oTd.nodePID="";
  oTd.level=1;
  oTd.nodeClass="normalL1";
  oTd.className="selectL";
  oTd.width="100%";
  var paddingLeft=treeBlankWidth+treeImageWidth;
  oTd.style.paddingLeft=paddingLeft+"px";
  //oTd.attachEvent("onclick",selectNode);
  oTd.innerHTML="<span id=\""+aID+"_word\" name=\""+aID+"_word\" nodeID=\""+aID+"\" nodePID=\"\" level=\"1\" style=\"cursor:hand;\">"+aDesc+"</span></td>";
  //调整当前选中节点
  if(treeSelectedTd!="")
    document.all.item(treeSelectedTd).className=document.all.item(treeSelectedTd).nodeClass;
  treeSelectedTd=aID+"_td";
}
/************************/
/*功能：修改菜单树节点*/
/*参数：*/
/*     aDesc：节点描述*/
/*aClickFunc：点击节点完成的操作，当取值“”时，默认nodeClick()；可以取值例如：‘Test();’*/
/************************/
function modifyTreeNode(aDesc,aClickFunc){
  if(aDesc == null || aDesc==""){
    AlertWindow("Public/","节点描述不能为空！");
    return;
  }
  if(treeSelectedTd==""){
    AlertWindow("Public/","请先选择节点！");
    return;
  }
  var currentTd=document.all.item(treeSelectedTd);
  var nodeID=currentTd.nodeID;
  var level=currentTd.level;
  var oNode=arrTree[nodeID];
  oNode["desc"]=aDesc;
  if(aClickFunc != null && aClickFunc != "")
    oNode["clickfunc"]=aClickFunc;
  var objWord=document.all.item(nodeID+"_word");
  var sHtml="<span id=\""+nodeID+"_word\" name=\""+nodeID+"_word\" nodeID=\""+nodeID+"\" nodePID=\""+oNode["pid"]+"\" level=\""+level+"\" style=\"cursor:hand;\">"+aDesc+"</span></td>";
  objWord.outerHTML=sHtml;
}
/************************/
/*功能：删除菜单树节点*/
/************************/
function deleteTreeNode(){
  if(treeSelectedTd==""){
    AlertWindow("Public/","请先选择节点！");
    return;
  }
  var currentTd=document.all.item(treeSelectedTd);
  var nodeID=currentTd.nodeID;
  var oNode=arrTree[nodeID];
  var strChildren=oNode["children"];
  if(strChildren==""){
    var sTips="您确定要删除节点\""+oNode["desc"]+"\"吗？"
  }else{
    AlertWindow("Public/","请先删除此节点下的所有子节点！");
    return;
  }
  var sReturn = ConfirmWindow("Public/",sTips);
  if(sReturn=="True"){
    var nodePID=currentTd.nodePID;
    if(nodePID==""){    //要删除的节点为根节点
      //删除节点
      var oTable=document.all.item("treeTable");
      var oTr=document.all.item(nodeID+"_tr");
      oTr.removeNode(true);
      var oBreakTr=document.all.item(nodeID+"_break");
      if(oBreakTr!=null)
        oBreakTr.removeNode(true);
      else{
        if(oTable.rows.length>=1)
          oTable.rows.item(0).removeNode(true);
      }
      //删除节点数组
      arrTree[nodeID]=null;
      //调整当前选中节点
      if(oTable.rows.length>=1){
        var tempID=oTable.rows.item(0).nodeID;
        treeSelectedTd=tempID+"_td";
        document.all.item(treeSelectedTd).className="selectL";
      }else{
        treeSelectedTd="";
      }
    }else{              //要删除的节点为非根节点
      var oPNode=arrTree[nodePID];
      var strPChildren=oPNode["children"];
      var arrTemp=strPChildren.toString().split(";");
      if(arrTemp.length==1){   //要删除的节点没有兄弟节点
        //删除节点
        var oTr=document.all.item(nodePID+"_children");
        oTr.removeNode(true);
        //调整父节点的展开标示
        var oPTd=document.all.item(nodePID+"_td");
        var oPWord=document.all.item(nodePID+"_word");
        var level=oPTd.level;
        var paddingLeft=treeBlankWidth*parseInt(level)+treeImageWidth;
        oPTd.style.paddingLeft=paddingLeft+"px";
        oPTd.innerHTML=oPWord.outerHTML;
        //调整被选中节点
        oPTd.className="selectL";
        treeSelectedTd=nodePID+"_td";
        //调整父节点的children属性
        oPNode["children"]="";
      }
      if(arrTemp.length>1){     //要删除的节点有兄弟节点
        //删除节点
        var oTr=document.all.item(nodeID+"_tr");
        oTr.removeNode(true);
        //调整被选中节点
        document.all.item(nodePID+"_td").className="selectL";
        treeSelectedTd=nodePID+"_td";
        //调整父节点的children属性
        var temp="";
        for(i=0;i<arrTemp.length;i++){
          if(arrTemp[i]!=nodeID){
            if(temp=="")
              temp=arrTemp[i];
            else
              temp=temp+";"+arrTemp[i];
          }
        }
        oPNode["children"]=temp;
      }
      //删除节点数组
      arrTree[nodeID]=null;
    }
  }
}