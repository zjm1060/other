//取单选记录
function getSingleSelected(fieldName){
    var sRecordID,sFeatures,sHref,stepInco,finiFlag,dealIndex;
    var ob_arr = document.all.item("queryCheckbox");
    if (ob_arr == null){
      	return null;
    }else{
      if (ob_arr.length == null){
        if (ob_arr.checked == true){
            if (fieldName == null){
          		sRecordID = ob_arr.value;
            }else{
         		sRecordID = eval("ob_arr."+fieldName);
            }
	       return  sRecordID;
        }
        else{
          return null;
        }
      }else{
        for (i=0;i<ob_arr.length;i++){
          if (ob_arr.item(i).checked == true){
            if (fieldName == null){
            	sRecordID = ob_arr.item(i).value;
            }else{
         		sRecordID = eval("ob_arr.item(i)."+fieldName);
            }
            break;
          }
        }
      }
    }
    if(sRecordID == null || sRecordID == ""){
      return null;
    }
    return  sRecordID;
  }

//取多选记录，用,分开
function getMultiSelected(fieldName,splitString){
    var sRecordID="",sFeatures,sHref,stepInco,finiFlag,dealIndex;
    var ob_arr = document.all.item("queryCheckbox");
    if (ob_arr == null){
      	return null;
    }else{
      if (ob_arr.length == null){
        if (ob_arr.checked == true){
            if (fieldName == null){
           		sRecordID = ob_arr.value;
           	}else{
             	sRecordID = eval("ob_arr."+fieldName);
            }
	       return  sRecordID;
        }
        else{
          return null;
        }
      }else{
        for (i=0;i<ob_arr.length;i++){
          if (ob_arr.item(i).checked == true){
            if (sRecordID != ""){
            	if (splitString == null){
             		sRecordID+=",";
          		}else{
             		sRecordID+=splitString;
          		}
            }
            if (fieldName == null){
            	sRecordID += ob_arr.item(i).value;
            }else{
             	sRecordID += eval("ob_arr.item(i)."+fieldName);
            }
          }
        }
      }
    }
    if(sRecordID == null || sRecordID == ""){
      return null;
    }
    return  sRecordID;
  }

//取多选记录，用,分开
function getMultiSelectedString(fieldName,splitString){
    var sRecordID="",sFeatures,sHref,stepInco,finiFlag,dealIndex;
    var ob_arr = document.all.item("queryCheckbox");
    if (ob_arr == null){
      	return null;
    }else{
      if (ob_arr.length == null){
        if (ob_arr.checked == true){
            if (fieldName == null){
           		sRecordID = ob_arr.value;
           	}else{
             	sRecordID = eval("ob_arr."+fieldName);
            }
	       return "'" + sRecordID +"'";
        }
        else{
          return null;
        }
      }else{
        for (i=0;i<ob_arr.length;i++){
          if (ob_arr.item(i).checked == true){
            if (sRecordID != ""){
            	if (splitString == null){
             		sRecordID+=",";
          		}else{
             		sRecordID+=splitString;
          		}
            }
            if (fieldName == null){
            	sRecordID += "'" + ob_arr.item(i).value + "'";
            }else{
             	sRecordID += "'" + eval("ob_arr.item(i)."+fieldName)+ "'";
            }
          }
        }
      }
    }
    if(sRecordID == null || sRecordID == ""){
      return null;
    }
    return  sRecordID;
  }

/*
  跳转到某页
*/
function gotoPage(pageNo){
	document.all.item("pageNo").value = pageNo;
    document.pulbicquery_form.submit();
}

/*
  排序
*/
function orderFieldClick(fieldName){
    document.all.item("orderField").value = fieldName;
    pulbicquery_form.submit();
}

/*
  跳转到某页
*/
function gotoInputPage(){
    if (CheckAction()){
        var pageNo=document.all.item('publicquery_gotopage').value;
        gotoPage(pageNo);
    }
}

/*
  跳转到某页
*/
function gotoInputPageKeyPress(){
	if (event.keyCode!=13){
           return;
    }
    if (CheckAction()){
        var pageNo=document.all.item('publicquery_gotopage').value;
        gotoPage(pageNo);
    }
}

//OnClick改变底色，并Check
function clickcheck(trIndex){
 	var obCheckBox = document.all.item("queryCheckbox");
	var currentClickObj = window.event.srcElement;
	if(currentClickObj.tagName.toLowerCase()=="input") //点击在输入构件上
		return;
	if(currentClickObj.tagName.toLowerCase()=="select")//点击在输入构件上
		return;
	if ( obCheckBox == null)
		return;
	if(obCheckBox.length == null){
        //如果只有一条记录
        obCheckBox.checked = true;
        var obTableRow = document.all.item("queryContent");
    	for(i=0;i<obCheckBox.length;i++){
      		parent_td = obCheckBox.item(i).parentElement;
      		parent_tr = parent_td.parentElement;
        	if (parent_tr.index==trIndex){
         		parent_tr.className = "selectData_Td";
                break;
            }
        }
	}else{
    	for(i=0;i<obCheckBox.length;i++){
        	obCheckBox.item(i).checked = false;
      		parent_td = obCheckBox.item(i).parentElement;
      		parent_tr = parent_td.parentElement;
      		parent_tr.className = ( i%2 == 0 ) ? "data_Tr1" : "data_Tr2";
         	if (obCheckBox.item(i).index==trIndex){
	       		parent_tr.className = "selectData_Td";
       			obCheckBox.item(i).checked = true;
        	}
        }
	}
}

//全选记录
function checkAll(checkboxitem){
	var ob_arr = document.all.item("queryCheckbox");
    if (checkboxitem.checked){
        for(i=0;i<ob_arr.length;i++){
        	ob_arr.item(i).checked = true;
      		parent_td = ob_arr.item(i).parentElement;
      		parent_tr = parent_td.parentElement;
           parent_tr.className = "selectData_Td";
        }
    }else{
        for(i=0;i<ob_arr.length;i++){
        	ob_arr.item(i).checked = false;
       		parent_td = ob_arr.item(i).parentElement;
      		parent_tr = parent_td.parentElement;
     		parent_tr.className = ( i%2 == 0 ) ? "data_Tr2" : "data_Tr1";
        }
    }
}

  function inputSingleCheck(item){
 	var obCheckBox = document.all.item("queryCheckbox");
	if ( obCheckBox == null || obCheckBox.length == null)
     	return;
	for(i=0;i<obCheckBox.length;i++){
        if (obCheckBox.item(i)!=item)
    		obCheckBox.item(i).checked = false;
    }
  //  item.checked = true;
  }

//OnMouseOut改变底色
function onMouseOut(currindex){
  ob_arr = document.all.item("queryCheckbox");
  if ( ob_arr == null || ob_arr.length == null  )
    return;
  var item = document.all.item("queryCheckbox",currindex);
  parent_td = item.parentElement;
  parent_tr = parent_td.parentElement;
  if (item.checked  != true){
  	 parent_tr.className = ( (currindex)%2 == 0 ) ? "data_Tr1" : "data_Tr2";
  }
}
//OnMouseOver改变底色
function onMouseOver(currindex){
  ob_arr = document.all.item("queryCheckbox");
  if ( ob_arr == null || ob_arr.length == null )
    return;
  var item = document.all.item("queryCheckbox",currindex);
  parent_td = item.parentElement;
  parent_tr = parent_td.parentElement;
  parent_tr.className = "selectData_Td";
}
