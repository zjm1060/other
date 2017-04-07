<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="suntek.report.structure.v10.FilterFieldObject,
                 suntek.report.structure.v10.FilterFieldStruct,
                 suntek.report.generator.ParametersGenerator,
                 java.util.Hashtable,
		 baseserverjb.BaseServerClient,
                 com.minstone.report.Base64Encoder,
                 java.sql.*"
%>
<%
  String m_sUser="",rpt_code="";
  Hashtable m_hs=null;
  byte[] btemp;
  int size;

  FilterFieldObject ffo=null;

  try{
    m_sUser = request.getRemoteUser();

  }catch(Exception e){
    out.println("获取参数错误 generateFFO.jsp Error " + e.getMessage());
  }
  ffo=new FilterFieldObject();

  rpt_code=request.getParameter("rpt_code");//报表编号
  size=new Integer(request.getParameter("size")).intValue();//参数个数
  for (int i=0;i<size;i++){
    int id=new Integer(request.getParameter("id"+i)).intValue();//编号
    String list_id=request.getParameter("list_id"+i);//列表编号
    String listName=request.getParameter("listName"+i);//列表名称
    String listValueType=request.getParameter("listValueType");
    btemp=listName.getBytes("iso8859-1");
    listName=new String(btemp);
    String fieldType=request.getParameter("fieldType"+i);//字段类型
    btemp=fieldType.getBytes("iso8859-1");
    fieldType=new String(btemp);
    String fieldName=request.getParameter("fieldName"+i);//字段名称
    btemp=fieldName.getBytes("iso8859-1");
    fieldName=new String(btemp);
    String operator=request.getParameter("operator"+i);//操作符
    btemp=operator.getBytes("iso8859-1");
    operator=new String(btemp);
    String desc=request.getParameter("desc"+i);//字段描述
    //String desc=java.net.URLDecoder.decode(request.getParameter("desc"+i));//字段描述
    btemp=desc.getBytes("iso8859-1");
    //desc=new String(btemp);
    String dataSource=request.getParameter("dataSource"+i);//
    String valueCol=request.getParameter("valueCol"+i);//值列
    String displayCol=request.getParameter("displayCol"+i);//显示列
    String andor=request.getParameter("andor"+i);//条件连接符
    btemp=andor.getBytes("iso8859-1");
    andor=new String(btemp);
    String defaultValue=request.getParameter("defaultValue"+i);//缺省值
    //String defaultValue=java.net.URLDecoder.decode(request.getParameter("defaultValue"+i));//缺省值
    //btemp=defaultValue.getBytes("iso8859-1");
    //defaultValue=new String(btemp);
    int iListValueType=listValueType==null?0:Integer.parseInt(listValueType);

    FilterFieldStruct ffs=new FilterFieldStruct();
    ffs.setRpt_code(rpt_code);
    ffs.setId(id);
    ffs.setList_id(list_id);
    ffs.setListName(listName);
    ffs.setListValueType(iListValueType);
    ffs.setFieldType(fieldType);
    ffs.setFieldName(fieldName);
    ffs.setOperator(operator);
    ffs.setDesc(desc);
    ffs.setDataSource(dataSource);
    ffs.setValueCol(valueCol);
    ffs.setDisplayCol(displayCol);
    ffs.setAndor(andor);
    ffs.setDefaultValue(defaultValue);

    ffo.addFilter(ffs);
  }

  String ret="";
  Connection metaConnection = null;
  try{
    metaConnection=BaseServerClient.getConnection(request.getRemoteUser());
    if (metaConnection!=null){
      ParametersGenerator temp=new ParametersGenerator(rpt_code,metaConnection);
      ret=temp.genrateSQLConditionE(ffo);
    }
    //将SQL语句中的特殊字符用其它字符替换,使进行页面参数传递时不会出现错误
	Base64Encoder base64=new Base64Encoder();
    ret=base64.encode(ret);
    //System.err.println("encodeC="+ret);
  }catch(Exception e){
    out.print("获取用户查询条件失败 generateFFO.jsp Error!");
  }finally{
    if(metaConnection != null)
      metaConnection.close();
  }
%>
<script language="javascript">
  window.parent.getSQL("<%=ret%>");//向父窗口返回获取的用户查询条件
</script>