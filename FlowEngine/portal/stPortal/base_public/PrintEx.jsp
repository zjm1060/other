<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="flowengineweb.*" %>
<%@ page import="flowengineforjava.util.BaseToolJB" %>
<%@ page import="java.util.Hashtable" %>
<%@ page import="flowengineforjava.util.RecordSet" %>
<%@ page import="flowengineforjava.structure.FormControlHtmlData" %>

<%
  //取得路径变量
  String sCommonPath="",sPublicPath="";
  sCommonPath = SysPara.getCommonPath(request);
  sPublicPath = SysPara.getPublicPath(request);
%>

<script type="text/javascript" src="<%=sCommonPath%>Include/Common.js"></script>
<%
  //强制页面刷新
  response.setHeader("Pragma","No-cache");
  response.setHeader("Cache-Control","no-cache");
  response.setDateHeader("Expires", 0);
  //myFlowEngine.initialize(pageContext,false);
  long flowInid = 21104;//Integer.parseInt(request.getParameter("flowInid"));
  long stepInid = -1;//Integer.parseInt(request.getParameter("stepInid"));
  String title="",formula="";
  RecordSet rs=null;
  //是否有打印输出模版,缺省为没有.
  boolean hasTemplet=false;
  try{
  	BaseToolJB  baseTool=new BaseToolJB("dx13107");

    FormControlHtmlData fchd = baseTool.getFormLib().getFormControlHtmlData(flowInid,stepInid);
    title =fchd.getTitle();
    formula =fchd.getFormula();
    rs =fchd.getControlHtmlData();
    hasTemplet=true;
    //如果有模版的话就使用模版来生成html,如果没有设置模版,则还是使用旧的方式来显示.
    if(fchd.hasTemplet()){
      out.print(fchd.toHTML());
    }
    else{
      %>
      <html>
      <head>
      <meta http-equiv="Content-Type" content="text/html; charset=GBK">
        <link rel="stylesheet" type="text/css" href="<%=sCommonPath%>Css/AdvancedPrint.css">
      </head>
      <body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" scroll="auto" style="background-color:white;">
      <table width="85%" border="0" cellspacing="0" cellpadding="0" align="center">
        <tr>
          <td align="left" valign="bottom" class="PrintEx_Title1">
            <%=formula%>
          </td>
        </tr>
        <% if (title != null && !title.equals("")){ %>
        <tr>
          <td align="center" valign="middle" class="PrintEx_Title2">
            <%=title%>
          </td>
        </tr>
        <% } %>
        <tr>
          <td width="100%">
            <table width="100%" border="1" cellspacing="0" cellpadding="0" class="outline" bordercolor="#007CA6">
              <tr>
                <td class="content_title" width="12%">名称</td>
                <td class="content_title" width="88%">内容</td>
              </tr>
              <%while(rs.next()){
              String fieldDesc = (String)rs.getObject("fieldDesc");
              String fieldValue = (String)rs.getObject("fieldValue");
              if(fieldDesc == null || fieldDesc.equals(""))
              fieldDesc = "&nbsp;";
              if(fieldValue == null || fieldValue.equals(""))
              fieldValue = "&nbsp;";
              %>
              <tr>
                <td class="content_td_left" width="12%"><%=fieldDesc%></td>
                <td class="content_td_right" width="88%"><%=fieldValue%></td>
              </tr>
              <%}%>
            </table>
</td>
        </tr>
      </table>
      </body>
      </html>
      <%
      }
    }catch(Exception e){
      e.printStackTrace();
    //out.println("<script type='text/javascript'>AlertWindow('"+sPublicPath+"','表单打印失败！');top.close();</script>");
    out.println("<script type='text/javascript'>alert('"+sPublicPath+"-表单打印失败！');</script>");
    return;
  }
%>
