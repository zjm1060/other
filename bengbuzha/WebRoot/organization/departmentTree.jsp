<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page contentType="text/html;charset=GBK"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>


<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3c.org/TR/1999/REC-html401-19991224/loose.dtd">
<HTML lang=en xml:lang="en" xmlns="http://www.w3.org/1999/xhtml">
	<HEAD>
		<TITLE>Add / Delete items</TITLE>
		<META http-equiv=content-type content="text/html; charset=utf-8">
		<META content=Samples name=title>
		<META content="" name=keywords>
		<META content="" name=description>

		<link rel="stylesheet" type="text/css"
			href="/xMIS/resources/css/dhtmlxmenu_dhx_skyblue.css">
		<LINK rel="stylesheet" type="text/css"
			href="/xMIS/resources/css/dhtmlxtree.css">


		<META content="MSHTML 6.00.3790.3959" name=GENERATOR>
	</HEAD>


	<BODY style="background-color: transparent">



		<SCRIPT language="JavaScript" src="/xMIS/resources/js/dhtmlxcommon.js"></SCRIPT>
		<SCRIPT language="JavaScript"
			src="/xMIS/resources/js/dhtmlxcommon_ext.js"></SCRIPT>

		<SCRIPT language="JavaScript" src="/xMIS/resources/js/dhtmlxtree.js"></SCRIPT>
		<SCRIPT language="JavaScript"
			src="/xMIS/resources/js/dhtmlxtree_ext.js"></SCRIPT>
		<SCRIPT language="JavaScript"
			src="/xMIS/resources/js/dhtmlxtree_kn.js"></SCRIPT>
		<script language="JavaScript"
			src="/xMIS/resources/js/dhtmlxtree_ed.js"></script>
		<script language="JavaScript"
			src="/xMIS/resources/js/dhtmlxtree_ed_ext.js"></script>

		<script language="JavaScript" src="/xMIS/resources/js/dhtmlxmenu.js"></script>
		<script language="JavaScript"
			src="/xMIS/resources/js/dhtmlxmenu_ext.js"></script>

		<script language="JavaScript" src="/xMIS/resources/js/prototype.js"></script>

		<!--DIV class=content-->

		<DIV style="DISPLAY: block">


			<TABLE width="100%" height="100%">
				<TBODY>
					<TR>
						<TD vAlign=top width="200px" height="100%">
							<DIV id=treeboxbox_tree
								style="OVERFLOW: auto; WIDTH: 200px;	HEIGHT: 800px">
							</DIV>
						</TD>

						<td height="100%" align="left">
							<iframe id="RightFrame" name="RightFrame" scrolling="no"
								frameborder="0" align="left"
								style="OVERFLOW: auto; WIDTH: 100%;	HEIGHT: 100%; background-color: #E7F4FF;">
							</iframe>
						</td>
					</TR>
				</TBODY>
			</TABLE>

		</DIV>

		<!--/DIV-->
	</BODY>
</HTML>

<form name="createNewFrm">
	<input type=hidden name="newDept" value="">
	<input type=hidden name="newEmp" value="">
	<input type=hidden name="empDeptMapping" value="">
</form>

<form name="deleteFrm">
</form>

<script language="JavaScript" type="text/JavaScript"> 
	
var XMLHttpReq;

//创建XMLHttpRequest对象       
function createXMLHttpRequest() {
	if(window.XMLHttpRequest) { //Mozilla 浏览器
		XMLHttpReq = new XMLHttpRequest();
	}
	else if (window.ActiveXObject) { // IE浏览器
		try {
			XMLHttpReq = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			try {
				XMLHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e) {}
		}
	}
}

function fixImage(id){
	switch(tree.getLevel(id)){
	case 1:
	tree.setItemImage2(id,'folderClosed.gif','folderOpen.gif','folderClosed.gif');
		break;
	case 2:
	tree.setItemImage2(id,'folderClosed.gif','folderOpen.gif','folderClosed.gif');			
		break;
	case 3:
	tree.setItemImage2(id,'folderClosed.gif','folderOpen.gif','folderClosed.gif');			
		break;			
	default:
	tree.setItemImage2(id,'leaf.gif','folderClosed.gif','folderOpen.gif');			
		break;
	}
}
		



menu = new dhtmlXMenuObject();
menu.setIconsPath("/xMIS/resources/images/");
menu.renderAsContextMenu();
menu.setOpenMode("web");
menu.attachEvent("onClick", onButtonClick);
menu.loadXML("/xMIS/resources/xml/treeMenu.xml");

//tree=new dhtmlXTreeObject("treeboxbox_tree","100%","100%",0);

//dhtmlxtree_ext.js
tree=new C("treeboxbox_tree","100%","100%",0); 

tree.setSkin('dhx_skyblue');
tree.setImagePath("/xMIS/resources/images/");
tree.loadXML("/xMIS/resources/xml/departmentTree.xml");
//tree.enableDragAndDrop(true);
tree.enableContextMenu(menu);
tree.enableKeyboardNavigation(true);
//tree.enableItemEditor(true);
tree.enableHighlighting(1);




tree.attachEvent("onBeforeContextMenu", function(itemId) {
	/*
	alert(itemId);
	if(tree.MD(itemId)>0){ 
		menu.hideItem('ouather');
	}else{
		menu.showItem('outher');
	}
	if(itemId.indexOf("emp-")<0)
		return true;
	else
		return false;
	*/
	
	var operator;
	<%if(((EmployeeObj) request.getSession()
				.getAttribute(WebKeys.Session_Attribute_Name_UserDNA)).getUserID() < 0){%>
				operator = "<%=WebKeys.User_Role_Name_Admin %>";
	<%}%>
	
	if(operator != "<%=WebKeys.User_Role_Name_Admin %>" ) 
		return false;
	
	if(tree.aiv(itemId)== "<%=WebKeys.NodeTipOnDeptTree %>" ) 	//tree.getItemTooltip()
		return false;
	else		
		return true;
});

tree.attachEvent("onDblClick", function(){

	var operator;
	<%if(((EmployeeObj) request.getSession()
				.getAttribute(WebKeys.Session_Attribute_Name_UserDNA)).getUserID() < 0){%>
				operator = "<%=WebKeys.User_Role_Name_Admin %>";
	<%}%>
	
	if(operator != "<%=WebKeys.User_Role_Name_Admin %>" ) 
		return false;

	tree.enableItemEditor(true);
});

tree.attachEvent("onMouseOut", function(){
	tree.enableItemEditor(false);
});

tree.attachEvent("onClick", function(itemId) {
	
	var url = tree.getUserData(itemId,"file")+"&user="+tree.getItemText(itemId);
	//alert(itemId + "//"+tree.getItemText(itemId));
	
	if(url.indexOf("orgMaint")<0) {
		
		return false;
	}

	parent.RightFrame.location=url;
	return true
});



/* [new dhtmlXTreeObject] model
tree.setOnClickHandler(doOnClick);
function doOnClick(id){ 
	var url = tree.getUserData(id,"file");
		parent.RightFrame.location=url;
} 
*/


function onButtonClick(menuitemId, type) {
    var id = tree.contextID;
    var txt = tree.getItemText(id);
    
    tree.enableItemEditor(false);
    
    if(menuitemId == "new"){
    	
    	var newID, newTitle, newGIf, newPath;
    	
    	if(id.indexOf("div")==0){
    		newID = "dept-new" ;
    		newTitle = "新部门";
    		newGif = "folderClosed.gif" ;
    		newPath = "<%=request.getScheme() + "://"
					+ request.getServerName() + ":" + request.getServerPort()
					+ request.getContextPath()
					+ "/organization/newDeptAlert.html"%>"
    	}else if(id.indexOf("dept")==0){
    		newID = "emp-new" ;
    		newTitle = "新员工";
    		newGif = "01.gif";
    		newPath = "<%=request.getScheme() + "://"
					+ request.getServerName() + ":" + request.getServerPort()
					+ request.getContextPath()
					+ "/orgMaint.do?method=createNewUser&dept="%>"+id+"&deptName="+txt;
    	}else{
    		return false;
    	}

		newPath = "";   
		newID += (new Date()).valueOf();
    	tree.insertNewItem(id,newID,newTitle,0,0,0,0,"SELECT");
    	tree.setItemImage(newID,newGif);
    	tree.setUserData(newID,"file",newPath);
    	
    }else if(menuitemId == "delete"){
    	
    	if(id.indexOf("div")==0) return false;
    	var subItems = tree.Fx(id);	//getAllSubItems()
    	
    	if(tree.Fx(id) == "" ) {
    		if(confirm("请确认是否禁用" +tree.getItemText(id)+"?")){
//	    		tree.deleteItem(id);
				createXMLHttpRequest();
				var url = "orgMaint.do?method=disableTreeNode&item="+id;
				XMLHttpReq.open("GET", url, true);
				XMLHttpReq.onreadystatechange = processResponse;//指定响应函数
				XMLHttpReq.send(null);  // 发送请求	    		
	    	}
    	}else{
    		alert (tree.getItemText(id)+"还有员工，不能禁用!");
    		return false;
    	}
//		alert("select id = " + id);
//    tree.setItemColor(id, menuitemId.split("_")[1]);

	}else if(menuitemId == "save"){
	
		var qp =  tree.Qp(); //tree.getAllChildless()
		var ik = tree.IK();	//tree.getAllItemsWithKids()
		
		var qpA = new Array();
		var ikA = new Array();
		
		qpA = splitIds(qp, ",");
		ikA = splitIds(ik, ",");

		var newDeptName = "";
		var newEmpName="";

		var newDept = "";
		var newEmp="";
		
		var empDeptMapping = "";
		
		for(var j=0; j<qpA.length; j++){
			
			//if((tree.getItemText(qpA[j])).indexOf("value=")>0)
			//alert(tree.getItemText(qpA[j]).substring(tree.getItemText(qpA[j]).indexOf("value=")+6, tree.getItemText(qpA[j]).length-1) );
			
			
			newDept = (qpA[j].indexOf("dept-new")==0) 
				? newDept+","+qpA[j]+"^" + getValue(tree.getItemText(qpA[j]))
				: newDept;
			newDeptName = (qpA[j].indexOf("dept-new")==0) 
				? newDeptName+","+ getValue(tree.getItemText(qpA[j]))
				: newDeptName;
			
			newEmp = (qpA[j].indexOf("emp-new")==0) 
				? newEmp+","+qpA[j]+"^"+ getValue(tree.getItemText(qpA[j]))
				: newEmp;
				
			empDeptMapping = (qpA[j].indexOf("emp-new")==0)  
				? (empDeptMapping+"," + qpA[j]+"^" + "M-"+tree.getParentId(qpA[j]))
				: empDeptMapping;
				
			newEmpName = (qpA[j].indexOf("emp-new")==0) 
				? newEmpName+","+ getValue(tree.getItemText(qpA[j]))
				: newEmpName;	

		}

		for(var j=0; j<ikA.length; j++){
			
			//alert(ikA[j] +"="+ tree.getItemText(ikA[j]));			
			
			newDept = (ikA[j].indexOf("dept-new")==0) 
				? newDept+","+ikA[j]+"^" + getValue(tree.getItemText(ikA[j]))
				: newDept;
			newDeptName = (ikA[j].indexOf("dept-new")==0) 
				? newDeptName+","+ getValue(tree.getItemText(ikA[j]))
				: newDeptName;
			
			
//			newEmpName = (ikA[j].indexOf("emp-new")==0) 
	//			? (newEmpName+","+tree.getItemText(ikA[j])+ "^"+ tree.getParentId(ikA[j])) 
		//		: newEmpName;
		}		
		
		if(newDeptName+newEmpName == ""){
			alert("没有创建新部门或新员工！"); 
			return false;
		}else{
			if(confirm("请确认是否要创建新部门或新员工：\n\n"+ newDeptName.substring(1) + "\n\n" + newEmpName.substring(1) )){
				//alert("参数："+newDept + "\n" + newEmp);
				document.all.newDept.value = newDept;
				document.all.newEmp.value = newEmp;
				document.all.empDeptMapping.value = empDeptMapping;
				
			    var form = document.createNewFrm;
			    var pars = '';
				var url = 'orgMaint.do?method=createNewUser';
				var req = new Ajax.Request(url,{
					method:'post',
					parameters:$(form).serialize(),
					onSuccess : dispatchRequest,
					onFailure : tipMessage
				});				
			}
		}
	}
	
}

function getValue(value){

	//alert(value);
	
	if(value.indexOf("value=")>0)
		return value.substring(value.indexOf("value=")+6, value.length-1) ;
	else
		return value;

}

function dispatchRequest(req){
	alert(req.responseText || '0');
	location.reload(true);
}

function tipMessage(req){
	alert(req.responseText || '');
}

function splitIds(str, splitSep){
	var t=str.split(splitSep); 
	var a=new Array(); 
	for(var i=0;i <t.length;i++) { 
		var temp=t[i].split( '   '); 
		var len=a.length; //当前长度 
		for(var j=0;j <temp.length;j++) { 
			a[len+j]=temp[j]; 
		}   
	} 
	
//	for(var k=0;k <a.length;k++) 
//	alert(a[k]); 
	
	return a;
	
}

function jq(menuitemId,type){
	var id=tree.contextID;
		alert(id);
	
	tree.lF(id,menuitemId.split("_")[1]);
};

function processResponse() {
	if (XMLHttpReq.readyState == 4) { // 判断对象状态
		var res = XMLHttpReq.responseText;
    	if (XMLHttpReq.status == 200) { // 信息已经成功返回
			//DisplayHot();
			alert(res);
			location.reload(true);
        } else { //页面不正常
            alert("删除失败，请重新操作，谢谢！");

        }
    }
}
</SCRIPT>

