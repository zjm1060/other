<%@ page contentType="text/html; charset=GBK" %>
<%@ page errorPage="../errorPage.jsp" %>
<%@ page import="minstone.ItemManage.BaseDbManage"%>
<%@ page import="minstone.ItemManage.WorkItem"%>
<%@ page import="java.util.Vector,java.util.Hashtable,java.util.Enumeration"%>
<%@ page import="oamodule.publiclib.util.RecordSetObject"%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>日志列表</title>
<link rel="stylesheet" type="text/css" href="../Common/Public.css">
</head>
<%
	request.setCharacterEncoding("GBK");
	BaseDbManage db=new BaseDbManage(request.getRemoteUser());
	String tableName="V_YW_DSJ";//通用的表名
	String condition=request.getRemoteUser();	//条件
	String selCon="",txtCon="";
	String listName0="";	//显示字段0－5个
	String listName1="";
	String listName2="";
	String listName3="";
	String listName4="";
	String listName5="";

	String actionFlag=request.getParameter("actionFlag");
	/*查询条件组合开始＝＝＝＝＝＝＝＝＝＝*/
	String mode = request.getParameter("mode");
        if (mode ==null)     mode="SEARCH";
        Hashtable hshParam =(Hashtable)session.getAttribute("SEARCH_PARAM");
        //查询字段的变量
	selCon=request.getParameter("selCon");
	txtCon=request.getParameter("txtCon");
	condition=request.getParameter("condition");

        if(hshParam !=null){
            condition=(String)hshParam.get("condition");
            selCon=(String)hshParam.get("selCon");
            txtCon=(String)hshParam.get("txtCon");
            if(condition ==null) condition="";
	    if(selCon ==null) selCon="";
	    if(txtCon ==null) txtCon="";
            else {
            	selCon=txtCon.substring(0,1);//从has取则要去掉前面的selCon
            	txtCon=txtCon.substring(1);//从has取则要去掉前面的selCon

            	}
        }
        if("SEARCH".equals(mode)){
          condition=request.getParameter("condition");
          selCon=request.getParameter("selCon");
          txtCon=request.getParameter("txtCon");
          hshParam = new Hashtable();//要查询的字段名字，字段值
          if(condition!=null && !"".equals(condition))
            hshParam.put("condition",condition);

	  if(txtCon!=null && !"".equals(txtCon) && selCon!=null && !"".equals(selCon))
            hshParam.put("txtCon",selCon+txtCon);

          session.setAttribute("SEARCH_PARAM", hshParam);
          response.sendRedirect("commList_Print.jsp?mode=SHOW");//向客户端发出一个临时重定向信息
          return;
        }
	/*查询条件组合结束＝＝＝＝＝＝＝＝＝＝*/

	//每页行数为10条
	String sPageSize = request.getParameter("pagesize");
	if(sPageSize==null) sPageSize="0";
	int pageSize=0;
	pageSize=Integer.parseInt(sPageSize);

	if(actionFlag==null) actionFlag="";
	if(actionFlag.equals("DeleteFlag")){
        	db.delDocSeq(listName0);
    	}
        //RecordSetObject rsoLiverBed=db.getDocSeq();
	//第几页的页码
        String pageStr=(String)request.getParameter("pageNo");
        if(pageStr==null || pageStr.equals(""))
          pageStr="1";
        int pageNo = java.lang.Integer.parseInt(pageStr);
        if(pageNo==0) pageNo=1;
        //每页行数为10条
        if(pageSize==0)   pageSize = 10;
	//得到结果集，并放在数组变量中
        Vector vResult=new Vector();
        vResult=db.getCommList(pageNo,pageSize,tableName,hshParam);
        //计算翻页参数
        int count=Integer.parseInt((String)vResult.elementAt(0));
        int pageCount=(int)((count-1)/pageSize + 1);
%>

<body bgcolor="#ffffff">
	<br><br>
<table width="100%" cellpadding="3" cellspacing="1" border="0" class="boarder">
  <tr class="content-bg">
    <td width="15%"><div align="center"><strong>部门名称</strong></div></td>
    <td width="15%"><div align="center"><strong>标   题</strong></div></td>
    <td width="10%"><div align="center"><strong>人员名称</strong></div></td>
    <td width="14%"><div align="center"><strong>开始日期</strong></div></td>
    <td width="14%"><div align="center"><strong>结束日期</strong></div></td>
    <td width="*"><div align="center"><strong>主要内容</strong></div></td>
    <td width="5%">&nbsp;</td>
  </tr>
  <%
   //第0个元素是记录条数，所以真正的数据从1开始
    for (int i=1;i<vResult.size();i++) {
      Vector vResultRow=new Vector();
      vResultRow=(Vector)vResult.elementAt(i);	//逐行提取，逐个字段显示
      	listName0 =(String)vResultRow.elementAt(0);
      	listName1   =(String)vResultRow.elementAt(1);
      	listName2   =(String)vResultRow.elementAt(2);
        listName3   =(String)vResultRow.elementAt(3);
      	listName4   =(String)vResultRow.elementAt(4);
      	listName5=(String)vResultRow.elementAt(5);
      	if(listName0==null) listName0="";
	if(listName1==null) listName1="";
	if(listName2==null) listName2="";
	if(listName3==null) listName3="";
	if(listName4==null) listName4="";
	if(listName5==null) listName5="";
  %>
  <tr class="content-bg">
    <td><%=listName0%></td>
    <td><%=listName1%></td>
    <td><%=listName2%></td>
    <td><%=listName3%></td>
    <td><%=listName4%></td>
    <td><%=listName5%></td>
    <td>&nbsp;</td>
    <!--<td><div align="center"><img onclick=" " style="cursor:hand;" src="" width="16" height="16" alt=""></div></td>-->
  </tr>
   <%} %>

</table>

<h1> </h1>
</body>
</html>

