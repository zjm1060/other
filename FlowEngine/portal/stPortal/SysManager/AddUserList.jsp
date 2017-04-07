<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.CachedRowSet,jbportal.PortalUtil" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
      String addtype=request.getParameter("addtype");
      String modu_id=session.getAttribute("modu_id").toString();
      Vector userlist=(Vector)(session.getAttribute("userlist"));
      String user=request.getParameter("adduser");
      String listhtml="";

      if (user!=null){
        HashMap HMuser=new HashMap();
        String usercode=user.substring(0,user.indexOf("^"));
        String ut=user.substring(user.indexOf("^")+1,user.length());
        String usertype=ut.substring(0,ut.indexOf("^"));
        String username=ut.substring(ut.indexOf("^")+1,ut.length());

        boolean isadd=true;
        for (int j=0;j<userlist.size();j++){
          HashMap temp=(HashMap)(userlist.get(j));
          String usercodej=temp.get("user_code").toString();
          String usertypej=temp.get("user_type").toString();
          if ((usercode.equals(usercodej))&&(usertype.equals(usertypej)))
            isadd=false;
        }
        if (isadd){
          HashMap temp=new HashMap();
          temp.put("user_code",usercode);
          temp.put("user_type",usertype);
          temp.put("user_name",username);
          temp.put("right","yes");
          userlist.add(temp);
          session.setAttribute("userlist",userlist);
        }
      }
      for (int j=0;j<userlist.size();j++){
        HashMap temp=(HashMap)(userlist.get(j));
        String usercodej=temp.get("user_code").toString();
        String usertypej=temp.get("user_type").toString();
        String usernamej=temp.get("user_name").toString();
        String right=temp.get("right").toString();
        listhtml+="<tr><TD align=left><input type=checkbox name=removeuc value=\""+usercodej+"^"+usertypej+"\">"+usernamej+"</TD>";
        if (right.equals("yes"))
          listhtml+="<TD align=center><input type=radio value=yes checked name=privilege"+j+" onclick=\"javascript:return motifyClick();\">允许使用&nbsp;&nbsp;&nbsp;<input type=radio name=privilege"+j+" value=no onclick=\"javascript:return motifyClick();\">禁止使用</TD>";
        else
          listhtml+="<TD align=center><input type=radio value=yes name=privilege"+j+" onclick=\"javascript:return motifyClick();\">允许使用&nbsp;&nbsp;&nbsp;<input type=radio name=privilege"+j+" checked value=no onclick=\"javascript:return motifyClick();\">禁止使用</TD>";
        listhtml+="</tr>";

      }

%>
<html>
<link rel="STYLESHEET" type="text/css" href="../include/tree/css/WebFX-ContextMenu.css">
<link rel="STYLESHEET" type="text/css" href="../include/tree/css/Node.css">
<link rel="STYLESHEET" type="text/css" href="../css/WebToolBar/toolbar.css">
<link rel="STYLESHEET" type="text/css" href="../include/Public.css">
<script type="text/javascript">
<!--
function OKClick(){
    if (<%=userlist.size()%>==0) { alert("请先选择要加入的用户");return false;}
    righttree.action="_AddRightOperator.jsp";
    return true;
}
function motifyClick(){
    righttree.action="_RemoveRightOperator.jsp";
    righttree.isdel.value="no";
    righttree.submit();
}
function removeClick(){
    if (<%=userlist.size()%>==0) { alert("请先选择要删除的用户");return false;}
    righttree.action="_RemoveRightOperator.jsp";
    righttree.isdel.value="yes";
    return true;
}

//-->
</script>
        <form action="_AddRightOperator.jsp" id="righttree" name="righttree">
        <Table cellspacing=1 width=100%>
          <Tr>
            <TD align=center class=3DTD  width=40% >加入用户名</TD>
            <TD align=center class=3DTD  width=60% >权限描述</TD>
          </Tr>
          <%=listhtml%>
          <tr>
           <td height=40 width=40%></td>
           <td align=right width=60%>
            <input type="submit" value="添加权限" name="OK" onclick="javascript:return OKClick();">&nbsp;
            <input type="submit" value="删除选中" name="remove" onclick="javascript:return removeClick();">&nbsp;
            <input type="button" value="取消修改" name="cancel" onclick="javascript:window.close();">
            <input type=hidden name="modu_id" value="<%=modu_id%>">
            <input type=hidden name="addtype" value="<%=addtype%>">
            <input type=hidden name="isdel" value="">
         </td>
          </tr>
        </TABLE>
      </form>

