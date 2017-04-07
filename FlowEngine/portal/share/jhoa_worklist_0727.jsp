<%@ page contentType="text/html; charset=GB2312" %>
<%@ page import="baseserverjb.BaseServerClient,com.minstone.util.Base64Encoder" %>
<%@ page import="java.sql.*" %>
<%
  //禁止使用cached
  //此页面一定在工作流的统一待办功能才能使用（不用getRemoteUser），不能单独使用，否则会报用户不存在
  response.setHeader("Pragma", "No-cache");
  response.setHeader("Cache-Control", "no-cache");
  response.setDateHeader("Expires", 0);
  String userCode = request.getParameter("user_code"); //系统即可得到当前登录用户
  if (userCode == null) userCode = "";
  int pageNo  = Integer.parseInt(request.getParameter("page"));
  //System.out.println("第"+pageNo +"页");
%>
<%
  String currentUser = userCode;//request.getRemoteUser();
  if (currentUser == null) currentUser = "";
  String userName = "Web";
  //System.out.println("当前用户：" + currentUser);

  int pageSize = 5;
  //pageNo = 1;//第一页
  String recordCount = "0";
  int isLastPage = 1;  //决定是否出现下页/上页的按钮
  long readFlag = -1;

  String sTYPE = "";
  String sTITLE = "";
  String sSENDER = "";
  String sSENDTIME = "";
  String sSTATUS = "";
  String sURL = "";
  String sSYSTAG = "";
  String sMODAL = "";

  String procid = "";
  String entrid = "";
  String dataid = "";

  Connection cn = null;
  Statement st = null;
  ResultSet rs = null;


  try {//取用户中文名称
    cn = BaseServerClient.getConnection(currentUser);
    String sql3 = " SELECT FULL_NAME FROM BASE_USER WHERE USER_CODE ='";
    sql3 = sql3 + currentUser + "'";
    //out.println("sql3："+sql3);
    st = cn.createStatement();
    rs = st.executeQuery(sql3);
    if (rs.next()) {
      userName = rs.getString(1);
    }
    rs.close();
    rs = null;
    st.close();
    st = null;
  } catch (SQLException ex) {
    ex.printStackTrace();
    throw new Exception("取用户中文名称失败！错误为：" + ex.getMessage());
  } finally {
    if (cn != null)
      cn.close();
    cn = null;
  }

  try {
    //String dbUrl = "jdbc:oracle:thin:@EXOA:1521:oanet"; //oracle
    String dbUrl = "jdbc:odbc:EXOA";

    //String dbDriver="oracle.jdbc.driver.OracleDriver"; //oracle
    String dbDriver = "sun.jdbc.odbc.JdbcOdbcDriver";

    String dbUser = "sa";
    String dbPassWord = "excellence";
    
    String countSql = "SELECT count(*) as myCount"
            + " FROM RECI_ENTR_ALL "
            + " WHERE PIIS_TERM = 0 "
            + " 	AND ( ENTR_STAT = 1 or ENTR_STAT = 2 )"
            + " 	AND ENTR_ALCO = 0 "
            + " 	AND 1=1 "
            + " 	AND ( replace(ENTR_RECI,' ' ,'') ='" + userName + "' "
            + " 	  OR EXISTS ( SELECT 1 FROM USER_DATA , SYS_USER "
            + " 	   WHERE RECI_ENTR_ALL.DATA_REFC = USER_DATA.DATA_ID "
            + " 	   AND PROX_USER IN ('" + currentUser + "') "
            + " 	   AND USER_DATA.USER_CODE = SYS_USER.USER_CODE "
            + " 	   AND RECI_ENTR_ALL.ENTR_RECI = SYS_USER.USER_NAME) ) ";
            
    String sql = "SELECT PROC_NAME AS TITLE, ENTR_SEND AS SENDER, CONVERT(varchar(10),ENTR_STAR,120)  AS SENDTIME, "
            + "PROC_ID AS proc_id, ENTR_ID AS entr_id, DATA_REFC AS data_id,ACTI_NAME AS STATUS "
            + " FROM RECI_ENTR_ALL "
            + " WHERE PIIS_TERM = 0 "
            + " 	AND ( ENTR_STAT = 1 or ENTR_STAT = 2 ) "
            + " 	AND ENTR_ALCO = 0 "
            + " 	AND 1=1 "
            + " 	AND ( replace(ENTR_RECI,' ' ,'') ='" + userName + "' "
            + " 	  OR EXISTS ( SELECT 1 FROM USER_DATA , SYS_USER "
            + " 	   WHERE RECI_ENTR_ALL.DATA_REFC = USER_DATA.DATA_ID "
            + " 	   AND PROX_USER IN ('" + currentUser + "') "
            + " 	   AND USER_DATA.USER_CODE = SYS_USER.USER_CODE "
            + " 	   AND RECI_ENTR_ALL.ENTR_RECI = SYS_USER.USER_NAME) ) "
            + " 	ORDER BY SENDTIME DESC ,PROC_ID DESC , ENTR_ID DESC ";

    //System.out.println(userName+"sql语句:"+sql);

    Class.forName(dbDriver);
    cn = DriverManager.getConnection(dbUrl, dbUser, dbPassWord);
    st = cn.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_READ_ONLY);
    //查询条数
    rs = st.executeQuery(countSql);    
    while (rs.next()) {
    	recordCount = rs.getString("myCount");
      if (recordCount == null) recordCount = "0";
    }
    //System.out.println("京华总数"+recordCount);
    
    //查询待办列表
    st=null;
		st = cn.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_READ_ONLY);
		rs=null;
    rs = st.executeQuery(sql);
    
    //System.out.println("每页数"+pageSize);
    int bookMark=(pageNo-1) * pageSize+1 ; 
    
    
     int intPageCount =(int) Math.ceil(Integer.parseInt(recordCount)/ pageSize);
     if(pageNo>intPageCount)
      pageNo = intPageCount;
     else
    	isLastPage = 0;
%>
    
    <?xml version="1.0" encoding="GB2312"?>
		<WORKLIST>
		  <PAGE><%=pageNo%></PAGE>
		  <TOTALRECORD><%=recordCount%></TOTALRECORD>
		  <ISLASTPAGE><%=isLastPage%></ISLASTPAGE>
  <%

    if(bookMark <= Integer.parseInt(recordCount)){
      isLastPage = 0;
      if(pageNo >1) rs.absolute(bookMark-1); 
      //System.out.println("定位"+bookMark);
      int i=0;
      while (rs.next() && i<pageSize) {
      
      i++;
      //System.out.println("next："+i);
    	//通过odbc取数据，一定要与sql的顺序一样，否则出错
      sTITLE = rs.getString("TITLE");
      if (sTITLE == null) sTITLE = "";
      sTITLE = Base64Encoder.encode(sTITLE.getBytes());
      
      sSENDER =rs.getString("SENDER");
      if (sSENDER == null) sSENDER = "";
      sSENDER = Base64Encoder.encode(sSENDER.getBytes());
      
      sSENDTIME=rs.getString("SENDTIME");
      if (sSENDTIME == null) sSENDTIME = "";
      
      procid = rs.getString("proc_id");
      if (procid == null) procid = "";
      
      entrid = rs.getString("entr_id");
      if (entrid == null) entrid = "";
      
      dataid = rs.getString("data_id");
      if (dataid == null) dataid = "";
      
      sSTATUS=rs.getString("STATUS");
      if (sSTATUS == null) sSTATUS = "";
      
      sURL = "/exoa2000/exflow/flowitem/default2.asp?userid=" + currentUser + "&amp;procid=" + procid + "&amp;entrid=" + entrid + "&amp;dataid=" + dataid;

  %>
  <LISTITEM>
    <TYPE>一般</TYPE>
    <TITLE><%=sTITLE%></TITLE>
    <SENDER><%=sSENDER%></SENDER>
    <SENDTIME><%=sSENDTIME%></SENDTIME>
    <STATUS><%=sSTATUS%></STATUS>
    <URL><%=sURL%></URL>
    <SYSTAG>扩展OA</SYSTAG>
    <MODAL>0</MODAL>
    <ISNOTICE>0</ISNOTICE>
  </LISTITEM>
  <%
    }
    rs.close();
    st.close();
    cn.close();
   }
  %>
</WORKLIST>
<%
  }
  catch (Exception e) {
    e.printStackTrace();
    System.out.println("获取ODBC数据错误,详细信息为:" + e.getMessage());
  }
  finally {
    try {
      if (rs != null) rs.close();
      if (st != null) st.close();
      if (cn != null) cn.close();
    } catch (Exception e) {
    }
  }
%>
