<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="baseserverjb.BaseServerClient,baseserverjb.DatabaseInfo,baseserverjb.IFramework,
java.util.Hashtable,
java.sql.*,java.lang.*,java.util.*,java.text.*"
%>

<%@ page import="java.util.*,sun.jdbc.rowset.CachedRowSet"%>
	
<%
 //flowInstId=&stepInstId=&readOnly=&primaryKey=
 String flowInstId=request.getParameter("flowInstId");
 String stepInstId=request.getParameter("stepInstId");
 String sSortId=request.getParameter("sort");
 if(sSortId==null) sSortId="1";
 
 String currentUser=request.getRemoteUser();
 if(currentUser==null) currentUser="admin";
 
 String userName="",userCode="";
 String deptName="";
 String deptSeq="";
 String finiDate="",dealComm="";
 String orderStr="";
 if(sSortId.equals("1")) orderStr=" order by deal_man ";
 if(sSortId.equals("2")) orderStr=" order by deal_man ";
 if(sSortId.equals("3")) orderStr=" order by fini_date ";
 if(sSortId.equals("4")) orderStr=" order by to_char(deal_comm) ";
 
%>
<link rel="stylesheet" type="text/css" href="/FlowEngineWeb/workflow/Common/Css/FlowForm.css">
<link rel="stylesheet" type="text/css" href="/FlowEngineWeb/workflow/Common/Css/Public.css">
<script language="javascript" src="/FlowEngineWeb/workflow/Common/Include/Common.js"></script>
<style type="text/css">
<!--
body {
	margin-left: 0px;
	margin-top: 0px;
	margin-right: 0px;
	margin-bottom: 0px;
}
.subWebTab {
	BORDER-RIGHT: #f1f1f1 1px outset; 
	BORDER-TOP: #f1f1f1 1px outset; 
	BORDER-LEFT: #f1f1f1 1px outset; 
	PADDING-TOP: 2px; 
	PADDING-BOTTOM: 2px; 
	BORDER-BOTTOM: #f1f1f1 1px outset; 
	BACKGROUND-COLOR: #ffffff
}
.select {
	FONT-SIZE: 11px; COLOR: #555555; FONT-FAMILY: Arial, Helvetica, sans-serif; TEXT-DECORATION: none
}
-->
</style>
<script>	
	function initWindow(){
  }
	function canSend(){     
    //if  (!CheckAction()){
	  	//message = "没有填写发送必填字段";
	 	 	//return false;
    //}    	
  	return true;
	}
  
  function trimStr(a_strVal){
   	return(a_strVal.replace(/^\s*|\s*$/g,""));
	}
	
	function canSave(){   
  	//if  (!CheckAction()){
	    	//message = "没有填写发送必填字段";	
  	 		//return false;
    //}      	    
		return true;
	}

	function getMessage(){
  		return message;
	}

	function send(){
		return true;
	}

	function save(){
		return true;
	}
	
 function doSort(ID){
  var cID = ID.value;
  document.frmPost.action="sign.jsp?sort="+cID;
  document.frmPost.submit();
}
</script>
<body scroll="no">
<form action="sign.jsp"  method="post" name="frmPost" id="frmPost" >
	<table class="table_frame">
		<tr>
    <td colspan="3"  class="subWebTab"> 
    			<input name="sortID" type="radio" value="1" <%=(sSortId.equals("1")?" checked":"")%> onclick="doSort(this);">按部门
          <input name="sortID" type="radio" value="2" <%=(sSortId.equals("2")?" checked":"")%> onclick="doSort(this);">按姓名
          <input name="sortID" type="radio" value="3" <%=(sSortId.equals("3")?" checked":"")%> onclick="doSort(this);">按时间
          <input name="sortID" type="radio" value="4" <%=(sSortId.equals("4")?" checked":"")%> onclick="doSort(this);">按内容
    </td>
  </tr>
<%
 Connection Conn = null;
 CachedRowSet crs = null;
 Statement stm= null;
 BaseServerClient bc=new BaseServerClient(currentUser);
 IFramework fw=bc.getFramework();
	try{
	Conn = bc.getConnection(currentUser);
	String sql = "select  deal_man, to_char(fini_date,'yyyy-mm-dd hh24:mi:ss') as fini_date, deal_comm as deal_comm   from deal_info t  "
			+" where   t.flow_inid='"+flowInstId+"'"
			+" and  step_inco<'"+stepInstId+"'"
			+" and  to_char(deal_comm) is not null "
			+ orderStr;
  //System.out.println(sql);
	//Statement stm = Conn.createStatement();
	//ResultSet rs = stm.executeQuery(sql);
	
	//用RowSet.jar包来处理clob型号的内容，如deal_comm
	crs = new CachedRowSet();
	crs.setCommand(sql);
	crs.setType(ResultSet.TYPE_SCROLL_INSENSITIVE);
	crs.setConcurrency(ResultSet.CONCUR_READ_ONLY);
	crs.execute(Conn);
	
	java.util.Random randomTemp=new java.util.Random();
  String colorRandom= new String();
  String colorDept=colorRandom.valueOf(randomTemp.nextInt(999999));
  Hashtable has=new Hashtable();
  Hashtable color=new Hashtable();
	String tempDeptName="";
	
	while(crs.next()){
		userCode=crs.getString("deal_man");
	  userName = fw.getUserName(userCode);
	  deptName=fw.getDeptNameOfUser("",userCode);
	  tempDeptName=(String)has.get(deptName);
	  if(!deptName.equals(tempDeptName)){
	    colorDept=colorRandom.valueOf(randomTemp.nextInt(999999));
		  color.put(deptName,colorDept);
		  has.put(deptName,deptName);
		}
	  dealComm=crs.getString("deal_man");
	  finiDate=crs.getString("fini_date");
	  dealComm=crs.getString("deal_comm");
	  if(dealComm==null) dealComm="";
	  //局领导  test  2007-05-23 14:00：叮叮咚咚叮叮咚咚 
	%>
	
	<tr>
    <td><b><font color="<%=color.get(deptName)%>"><%=deptName%></font></b></td>
    <td><b><font color="<%=color.get(deptName)%>">&nbsp;&nbsp;<%=userName%></font></b></td>
    <td><b><font color="<%=color.get(deptName)%>">&nbsp;&nbsp;<%=finiDate%></font></b></td>
  </tr>
  <tr >
    <td colspan="3" class="subWebTab"><font color="<%=color.get(deptName)%>">&nbsp;&nbsp;&nbsp;&nbsp;<%=dealComm%></font></td>
  </tr>
  
  <%	  
	}
	crs.close();
	crs = null;
	stm.close();
	stm = null;
	}catch(Exception ex){
	  ex.printStackTrace();
	} finally{
	  if (Conn != null)
       Conn.close();
    Conn = null;
	}
	
%>

</table>
<!--以下两行需要保留才能与表单的desc1、desc2控件信息传递-->
<input name="flowInstId" type="hidden" value="<%=flowInstId%>">
<input name="stepInstId" type="hidden" value="<%=stepInstId%>">
</form>
</body>
