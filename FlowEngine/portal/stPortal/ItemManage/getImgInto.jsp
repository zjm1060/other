<%@ page contentType="text/html;charset=GBK" %>
<%@ page import="java.util.*"%>
<%@ page import="java.sql.*"%>
<%@ page import="java.io.*"%>
<%@ page import="minstone.ItemManage.BaseDbManage"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head></head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>从一个数据库批量导出印章jpg图片到另外一个数据库</title>
<body>
从一个数据库批量导出印章jpg图片到另外一个数据库
<%

//注意：要修改FRAM_SEQ字段要有一个真正存在的值，流程管理工具才可以看到该章
BaseDbManage db=new BaseDbManage(request.getRemoteUser());
request.setCharacterEncoding("GBK");
  String result="";
  String fileName="";
  String user = request.getRemoteUser();
  String actionFlag=request.getParameter("actionFlag");
  if(actionFlag==null) actionFlag="";
  if(actionFlag.equals("submit")){

  	Connection cn = null;
  	Statement st = null;
  	ResultSet rs= null;

  try {
    String dbUrl =db.getDBUrl();// "jdbc:oracle:thin:@10.1.0.108:1521:oanet";
    String dbUser =db.getDBUrl();// "company";
    String dbPassWord =db.getDBPassWord();// "1qaz2wsx";
    //要修改1
    String sql = "select V_MARK_CODE ,FRAM_SEQ ,V_MARK_NAME,V_MARK_DESC,V_MARK_CONT from zcbcompany.WF_WATERMARK";
    System.out.println("sql语句:"+sql);
    Class.forName("oracle.jdbc.driver.OracleDriver");
    cn = DriverManager.getConnection(dbUrl, dbUser,dbPassWord);
    System.out.println(dbUrl+" "+dbUser+" "+dbPassWord);
    st = cn.createStatement();
    rs = st.executeQuery(sql);
    int i=0,size=0;  //要修改2 i
    String strI="";
    while(rs.next()){
    			i++;
    			//要修改3
    			fileName="c:\\"+rs.getString("V_MARK_NAME")+".jpg";
   				System.out.println("文件名："+fileName);
          File file = new File(fileName);//文件名
					//字节流
          InputStream in =  rs.getBinaryStream("V_MARK_CONT");//文件内容
          FileOutputStream fout = new FileOutputStream(file);
          int c;
					while ((c = in.read()) != -1){
						fout.write(c);  //用字节流写入文件比较安全，但速度较慢
						size++;
					}
					System.out.println("写入文件size:"+size);

				  //文件流
          FileInputStream fin = new FileInputStream(file);
          size= fin.available();
          System.out.println("导入数据库size:"+size);
					PreparedStatement ps = cn.prepareStatement("insert into wf_watermark(V_MARK_CODE ,FRAM_SEQ ,V_MARK_NAME,V_MARK_DESC,V_MARK_CONT) values(?,?,?,?,?)");
					ps.setString(1,strI.valueOf(i));
					ps.setString(2,"3930");//rs.getString("FRAM_SEQ"));//权限为空，否则工具看不到
					ps.setString(3,rs.getString("V_MARK_NAME"));
					ps.setString(4,"总承包"+rs.getString("V_MARK_DESC"));//要修改4
					ps.setBinaryStream(5,fin,size); //写入数据库需要文件流才可以.
					ps.executeUpdate();
          cn.commit();
          fout.close();

    }
    rs.close();
    cn.close();
    result="导出成功";
  }
  catch (Exception e) {
    System.out.println("获取数据库信息错误,详细信息为:" + e.getMessage());
    result="导出失败！";
  }
  finally {
    try{
      if (cn != null) rs.close();
    }catch(Exception e){}
  }


  //显示结果
  //out.print(result);
}
%>
</body>
<form name="getImgInto" action="getImgInto.jsp" method="POST">
	<input name="actionFlag" type="hidden" id="actionFlag" value="submit">
	<input type="Submit" name="other_funtion" value="导出公章" align="MIDDLE">
</form>
导出结果:<%=result%><br>
</html>