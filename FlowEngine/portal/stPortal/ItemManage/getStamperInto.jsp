<%@ page contentType="text/html;charset=GBK" %>
<%@ page import="java.util.*"%>
<%@ page import="java.sql.*"%>
<%@ page import="java.io.*"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head></head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>从一个数据库批量导出手写签章jpg图片到另外一个数据库</title>
<body>
从一个数据库批量导出印章jpg图片到另外一个数据库
<%

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
    String dbUrl = "jdbc:oracle:thin:@10.1.0.108:1521:oanet";
    String dbUser = "company";
    String dbPassWord = "1qaz2wsx";
    //要修改1
    //找出此库的内容不存在新库中的这些记录
    String sql = "select USER_CODE,USER_SIGN  "
								+" from  zjcompany.usersign where not exists "
								+"( select * from usersign where zjcompany.usersign.USER_CODE=usersign.USER_CODE )";
    System.out.println("sql语句:"+sql);
    Class.forName("oracle.jdbc.driver.OracleDriver");
    cn = DriverManager.getConnection(dbUrl, dbUser,dbPassWord);
    System.out.println(dbUrl+" "+dbUser+" "+dbPassWord);
    st = cn.createStatement();
    rs = st.executeQuery(sql);
    int i=0,size=0; 
    String strI="";
    while(rs.next()){
    			i++;
    			//要修改2
    			fileName="c:\\company_picture\\zj\\"+rs.getString("USER_CODE")+".jpg";
   				System.out.println("文件名："+fileName);
          File file = new File(fileName);//文件名
					//字节流
          InputStream in =  rs.getBinaryStream("USER_SIGN");//文件内容
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
					PreparedStatement ps = cn.prepareStatement("insert into usersign(USER_CODE ,USER_SIGN) values(?,?)");
					ps.setString(1,rs.getString("USER_CODE"));
					ps.setBinaryStream(2,fin,size); //写入数据库需要文件流才可以.
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
<form name="getImgInto" action="getStamperInto.jsp" method="POST">
	<input name="actionFlag" type="hidden" id="actionFlag" value="submit">
	<input type="Submit" name="other_funtion" value="导出手写签章" align="MIDDLE"> 
</form>
导出结果:<%=result%><br>
</html>