var g_Activecheck = 0;
function  Window_Onload()
{
  //ob_arr是通用查询列表的checkbox数组，值为null表示该列表没有记录，ob_arr.length为null表示该列表只有一条记录
  ob_arr = document.all.item("query_checkbox");
  if ( ob_arr == null )
    return;
  if ( ob_arr.length == null )
  {
    ob_arr.checked = true;
    //设置选中时tr和td的css
   ob_arr.parentElement.parentElement.className = "query_list_data_trSelected";
    ob_arr.parentElement.className = "query_list_data_tdSelected";
  }
  else
  {
    for (i=0;i<ob_arr.length;i++)
      //设置checkbox那列td的css为query_list_meta_td
      //ob_arr.item(i).parentElement.className = "query_list_meta_td";
      //设置默认列表的第一条记录为选中
    ob_arr.item(0).checked = true;
    ob_arr.item(0).parentElement.parentElement.className = "query_list_data_trSelected";
    ob_arr.item(0).parentElement.className = "query_list_data_tdSelected";
  }
}
function mouseover(item)
{
  ob_arr = document.all.item("query_checkbox");
  if ( ob_arr == null || ob_arr.length == null || ob_arr.length<=item.index-1 )
    return;
  if (document.all.item("query_checkbox",item.index-1).checked  != true)
    item.className = "query_list_data_trOver";
}
//OnMouseOut改变底色
function mouseout(item)
{
  ob_arr = document.all.item("query_checkbox");
  if ( ob_arr == null || ob_arr.length == null || ob_arr.length<=item.index-1 )
    return;
  item.className = ( (item.index-1)%2 == 0 ) ? "query_list_data_tr1" : "query_list_data_tr2";
  item.style.backgroundColor = item.currentStyle.getAttribute("backgroundColor",2);
  ob_arr = document.all.item("query_checkbox");
  if (document.all.item("query_checkbox",item.index-1).checked  == true)
    item.className = "query_list_data_trSelected";
}
//OnClick改变底色，并Check

 function ClickCheck(it)
    {
      ob_arr = document.all.item("query_checkbox");
      if ( ob_arr == null || ob_arr.length == null || ob_arr[it.index-1] == null )
        return;
      if ( g_Activecheck != null )
      {
        document.all.item("query_checkbox",g_Activecheck).parentElement.className = ( g_Activecheck%2 == 0 ) ? "query_list_data_td1" : "query_list_data_td2";
      }
      if ( window.event.ctrlKey == false )
      {
        for (i=0;i<ob_arr.length;i++)
        {
          parent_td = ob_arr.item(i).parentElement;
          parent_tr = parent_td.parentElement;
          parent_td.className = ( i%2 == 0 ) ? "query_list_data_td1" : "query_list_data_td2";
          ob_arr.item(i).checked = false;
          parent_tr.className = ( i%2 == 0 ) ? "query_list_data_tr1" : "query_list_data_tr2";

          iS = ( g_Activecheck < it.index ) ? g_Activecheck : it.index - 1;
          iB = ( g_Activecheck < it.index ) ? it.index -1 : g_Activecheck;
          if ( window.event.shiftKey && i >= iS && i <= iB )
          {
            ob_arr.item(i).checked = true;
            //parent_tr.className = "query_list_data_trSelected";
          }
        }
      }
      ob_arr[it.index-1].checked = true;
      it.className = "query_list_data_trSelected";
      g_Activecheck = it.index - 1;

     // for (i=0;i<ob_arr.length;i++)
      //  ob_arr.item(i).parentElement.className = "query_list_meta_td";

      parent_td = document.all.item("query_checkbox",it.index-1).parentElement;
      parent_td.className = "query_list_data_tdSelected";
}
//点击列表的某一个checkbox时，令该checkbox所在的tr显示为被选中状态
function listCheckboxClick(){
  var obj = window.event.srcElement;
  var ppObj = obj.parentElement.parentElement;
  var i = ppObj.index-1;
  if(!obj.checked){
    ppObj.className = (i%2==1)?"query_list_data_tr1":"query_list_data_tr2";
  }
  if(obj.checked){
    ppObj.className = "query_list_data_trSelected";
  }
}
//当列表没有选中任何一列时，返回true；
function listIfSelectNone(){
  if(listSelectNum<=0)
    return true;
  else
    return false;
}

//当列表选中超过一列（两列或两列以上）时，返回true；
function listIfSelectMore(){
  if(listSelectNum>1)
    return true;
  else
    return false;
}

//返回列表当前选中值，如果有多个，值和值之间用","分隔；当hiddenCheckboxObj==null或没有选中任何记录或选中了一条记录但记录值确实为""时，返回值都会是""；
function listGetValue(){
  var hiddenCheckboxObj = document.all.item("hiddenCheckbox");
  var returnValue="";
  if(hiddenCheckboxObj.length==null){
    if(hiddenCheckboxObj.checked)
      returnValue=hiddenCheckboxObj.value;
  }else{
    for(i=0;i<hiddenCheckboxObj.length;i++){
      if(hiddenCheckboxObj[i].checked){
        if(returnValue=="")
          returnValue=hiddenCheckboxObj[i].value;
        else
          returnValue=returnValue+","+hiddenCheckboxObj[i].value;
      }
    }
  }
  return returnValue;
}

//完成对hiddenCheckbox的全选和全不选操作
function listAllorNone(){
  var obj = document.all.item("AllorNone");
  if(obj.checked){
    listSelectAll();
    obj.title="全不选";
  }
  if(!obj.checked){
    listSelectNone();
    obj.title="全选";
  }
}

//全选
function listSelectAll(){
  var hiddenCheckboxObj = document.all.item("query_checkbox");
  if(hiddenCheckboxObj==null)
    return;
  if(hiddenCheckboxObj.length==null){
    hiddenCheckboxObj.checked = true;
    hiddenCheckboxObj.parentElement.parentElement.className = "query_list_data_trSelected";
    listSelectNum=1;
  }else{
    for(i=0;i<hiddenCheckboxObj.length;i++){
      hiddenCheckboxObj.item(i).checked = true;
      hiddenCheckboxObj.item(i).parentElement.parentElement.className = "query_list_data_trSelected";
    }
    listSelectNum=hiddenCheckboxObj.length;
  }
}

//全不选
function listSelectNone(){
  var hiddenCheckboxObj = document.all.item("query_checkbox");
  if(hiddenCheckboxObj==null)
    return;
  if(hiddenCheckboxObj.length==null){
    hiddenCheckboxObj.checked = false;
    hiddenCheckboxObj.parentElement.parentElement.className = "query_list_data_tr1";
  }else{
    for(i=0;i<hiddenCheckboxObj.length;i++){
      hiddenCheckboxObj.item(i).checked = false;
      hiddenCheckboxObj.item(i).parentElement.parentElement.className = (i%2==1)?"query_list_data_tr1":"query_list_data_tr2";
    }
  }
  listSelectNum=0;
}
