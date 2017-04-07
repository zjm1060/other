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
      ob_arr.item(i).parentElement.className = "query_list_meta_td";
      //设置默认列表的第一条记录为选中
    ob_arr.item(0).checked = true;
    ob_arr.item(0).parentElement.parentElement.className = "query_list_data_trSelected";
    ob_arr.item(0).parentElement.className = "query_list_data_tdSelected";
  }
}
//OnMouseOver改变底色
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
        parent_tr.className = "query_list_data_trSelected";
      }
    }
  }
  ob_arr[it.index-1].checked = true;
  it.className = "query_list_data_trSelected";
  g_Activecheck = it.index - 1;

  for (i=0;i<ob_arr.length;i++)
    ob_arr.item(i).parentElement.className = "query_list_meta_td";

  parent_td = document.all.item("query_checkbox",it.index-1).parentElement;
  parent_td.className = "query_list_data_tdSelected";
}