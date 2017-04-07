<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="java.util.*,java.sql.*"%>
<%@ page import="baseserverjb.BaseServerClient,baseserverjb.IFramework"%>

<%!
  public Hashtable getSheetNameHt(){
    Hashtable ht = new Hashtable();
    ht.put("合生集团","/STPublish/view/ViewFolder.jsp?folder_code=71_72_75");//合生集团
    ht.put("京津新城投资决策管理委员会","/STPublish/view/ViewFolder.jsp?folder_code=71_72_75");//京津新城投资决策管理委员会
    ht.put("董事局","/STPublish/view/ViewFolder.jsp?folder_code=71_72_75");//董事局
    
    ht.put("珠江集团","/STPublish/view/ViewFolder.jsp?folder_code=71_73_212");//珠江投资集团
    ht.put("招标中心","/STPublish/view/ViewFolder.jsp?folder_code=71_73_212");//招标中心
    ht.put("支付中心","/STPublish/view/ViewFolder.jsp?folder_code=71_73_212");//支付中心
    ht.put("信息化办公室","/STPublish/view/ViewFolder.jsp?folder_code=71_73_212");//信息化办公室
    ht.put("监事会","/STPublish/view/ViewFolder.jsp?folder_code=71_73_212");//监事会   
    
    ht.put("珠江总承包集团","/STPublish/view/ViewFolder.jsp?folder_code=71_74_261");//珠江总承包集团
  
    return ht;
  }
%>
<%
  //System.out.println("test:aaaaaaaaaaaaaaaaaa");
  
  String user = request.getRemoteUser();
  if(user==null)
  	user="admin_hs";
  //System.out.println("当前用户："+user);
  
  String framName = null;
  String bulletinUrl = null;
  String dept =null;

  Connection cn = null;
  Statement st = null;
  ResultSet rs= null;

  BaseServerClient bsc=new BaseServerClient(user);
  IFramework ifw= bsc.getFramework();
  String framSeq2=ifw.getDeptSeqFromRoot(user,1);
  
  try {
    String dbUrl = "jdbc:oracle:thin:@10.1.0.108:1521:oanet";
    String dbUser = "company";
    String dbPassWord = "1qaz2wsx";
    
    //String sql = "select framName2 from V_RPT_FRAMEDEPARTMENT where user_code = '" + user + "'";
    String sql = "select FRAM_NAME from FRAMEWORK where FRAM_SEQ = '" + framSeq2 + "'";
    //out.println("sql语句:"+sql);
    Class.forName("oracle.jdbc.driver.OracleDriver");
    cn = DriverManager.getConnection(dbUrl, dbUser,dbPassWord);
    st = cn.createStatement();
    rs = st.executeQuery(sql);
    if (rs.next()){
      framName = rs.getString(1);
    }
  }
  catch (Exception e) {
    out.println("获取用户地区库代码错误,详细信息为:" + e.getMessage());
  }
  finally {
    try{
      if (cn != null) rs.close();
    }catch(Exception e){}
  }

  System.out.println("当前部门："+framName);
 
  Hashtable bulletinUrlHt = getSheetNameHt();
  bulletinUrl = (String)bulletinUrlHt.get(framName);
  if (framName == null || framName.equals("")){
      //如果该用户是没有分配有组织架构的,就显示"人才招聘"
      bulletinUrl = "/STPublish/view/ViewFolder.jsp?folder_code=71_263";
  }
  //或者不是上述的几个地区公司的,就显示"人才招聘"
  if (bulletinUrl == null || bulletinUrl.equals("")){
    bulletinUrl = "/STPublish/view/ViewFolder.jsp?folder_code=71_263";
  }
  //System.out.println("当前url为："+bulletinUrl);
  response.sendRedirect(bulletinUrl);
%>
