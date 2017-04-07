<%--<%@ page language="java"--%>
         <%--import="com.bitc.jk.xmis.util.JacksonUtil,com.bitc.jk.xmis.util.WebUtil,com.zhuozhengsoft.pageoffice.OpenModeType,com.zhuozhengsoft.pageoffice.PageOfficeCtrl,com.zhuozhengsoft.pageoffice.wordwriter.DataTag"--%>
         <%--pageEncoding="gb2312"%>--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="com.zhuozhengsoft.pageoffice.*,java.awt.*" %>
<%@ page import="com.zhuozhengsoft.pageoffice.wordwriter.WordDocument" %>
<%@ page import="java.text.SimpleDateFormat" %>
<%@ page import="com.bitc.jk.xmis.util.JacksonUtil" %>
<%@ page import="com.zhuozhengsoft.pageoffice.wordwriter.DataTag" %>
<%@ page import="java.util.*" %>
<%@ page import="java.util.List" %>
<%@ page import="com.bitc.jk.xmis.util.WebUtil" %>
<%@ taglib uri="http://java.pageoffice.cn" prefix="po"%>
<%
    //******************************卓正PageOffice组件的使用*******************************
    PageOfficeCtrl poCtrl1 = new PageOfficeCtrl(request);
    poCtrl1.setServerPage(request.getContextPath()+"/poserver.zz"); //此行必须
    WordDocument worddoc = new WordDocument();
    //表格数据
    String data = request.getParameter("data");
    List list = JacksonUtil.readValue(data, ArrayList.class);
    Map map = (Map) list.get(0);

    DataTag sqYear = worddoc.openDataTag("[yyyy]");
    sqYear.setValue(String.valueOf(Calendar.getInstance().get(Calendar.YEAR)));
    DataTag sqMonth = worddoc.openDataTag("[MM]");
    sqMonth.setValue(String.valueOf(Calendar.getInstance().get(Calendar.MONTH)+1));
    DataTag sqDay = worddoc.openDataTag("[dd]");
    sqDay.setValue(String.valueOf(Calendar.getInstance().get(Calendar.DAY_OF_MONTH)));
    DataTag new_kd = worddoc.openDataTag("[new_kd]");
    if(map.get("new_kongshu") == null || "".equals(map.get("new_kongshu"))||"0".equals(map.get("new_kongshu"))){
        new_kd.setValue("");
    }else{
        new_kd.setValue(String.valueOf(map.get("new_kongshu")+"×"+map.get("new_kaidu")));
    }
    DataTag old_kd = worddoc.openDataTag("[old_kd]");
    if(map.get("old_kongshu") == null || "".equals(map.get("old_kongshu"))||"0".equals(map.get("old_kongshu"))){
        old_kd.setValue("");
    }else{
        old_kd.setValue(String.valueOf(map.get("old_kongshu")+"×"+map.get("old_kaidu")));
    }
    DataTag s_sw = worddoc.openDataTag("[s_sw]");
    s_sw.setValue(String.valueOf(map.get("shang_shuiwei")));
    DataTag x_sw = worddoc.openDataTag("[x_sw]");
    x_sw.setValue(String.valueOf(map.get("xia_shuiwei")));
    DataTag ll = worddoc.openDataTag("[ll]");
    ll.setValue(String.valueOf(map.get("liuliang")));

    DataTag Mi = null;
    DataTag di = null;
    DataTag hi = null;
    DataTag mi = null;
    DataTag s_swi = null;
    DataTag x_swi = null;
    DataTag lli = null;
    DataTag sum_lli = null;

    Calendar calendar = Calendar.getInstance();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    for(int i=1;i<list.size();i++){
        map = (Map) list.get(i);
        calendar.setTime(sdf.parse(String.valueOf(map.get("datetime"))));
        Mi = worddoc.openDataTag("[M"+(i-1)+"]");
        Mi.setValue(String.valueOf(calendar.get(Calendar.MONTH )+1));
        di = worddoc.openDataTag("[d"+(i-1)+"]");
        di.setValue(String.valueOf(calendar.get(Calendar.DAY_OF_MONTH)));
        hi = worddoc.openDataTag("[h"+(i-1)+"]");
        hi.setValue(String.valueOf(calendar.get(Calendar.HOUR_OF_DAY)));
        mi = worddoc.openDataTag("[h"+(i-1)+"]");
        mi.setValue(String.valueOf(calendar.get(Calendar.MINUTE)));
        s_swi = worddoc.openDataTag("[s_sw"+(i-1)+"]");
        s_swi.setValue(String.valueOf(map.get("shang_shuiwei")));
        x_swi = worddoc.openDataTag("[x_sw"+(i-1)+"]");
        x_swi.setValue(String.valueOf(map.get("xia_shuiwei")));
        lli = worddoc.openDataTag("[ll"+(i-1)+"]");
        lli.setValue(String.valueOf(map.get("liuliang")));
        sum_lli = worddoc.openDataTag("[sum_ll"+(i-1)+"]");
        sum_lli.setValue(String.valueOf(map.get("sum_ll")));
    }
    for(int i=list.size();i<=3;i++){
        Mi = worddoc.openDataTag("[M"+(i-1)+"]");
        Mi.setValue("");
        di = worddoc.openDataTag("[d"+(i-1)+"]");
        di.setValue("");
        hi = worddoc.openDataTag("[h"+(i-1)+"]");
        hi.setValue("");
        mi = worddoc.openDataTag("[h"+(i-1)+"]");
        mi.setValue("");
        s_swi = worddoc.openDataTag("[s_sw"+(i-1)+"]");
        s_swi.setValue("");
        x_swi = worddoc.openDataTag("[x_sw"+(i-1)+"]");
        x_swi.setValue("");
        lli = worddoc.openDataTag("[ll"+(i-1)+"]");
        lli.setValue("");
        sum_lli = worddoc.openDataTag("[sum_ll"+(i-1)+"]");
        sum_lli.setValue("");
    }
    poCtrl1.setWriter(worddoc);
    poCtrl1.setCaption("调度单");
    //隐藏菜单栏
    poCtrl1.setMenubar(true);
    //隐藏自定义工具栏
    poCtrl1.setCustomToolbar(true);
//    poCtrl1.setSaveFilePage("SaveShowFile.jsp");

    poCtrl1.webOpen("diaodudan.doc", OpenModeType.docNormalEdit,"");
    poCtrl1.setTagId("PageOfficeCtrl1"); //此行必须
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<%
    if ("处领导".equals(WebUtil.getCurrentEmployee().getDepartmentName()) || "工程科".equals(WebUtil.getCurrentEmployee().getDepartmentName())) {
%>
<head>
    <title>调度单</title>
</head>
<body>
<form id="form1">
    <div style="width: auto; height: 100%;">
        <!--**************   PageOffice 客户端代码开始    ************************-->
        <po:PageOfficeCtrl id="PageOfficeCtrl1">
        </po:PageOfficeCtrl>
        <!--**************   PageOffice 客户端代码结束    ************************-->
    </div>
</form>
</body>
<%
} else {
%>
<body>
<div class="container row">
    <div style="margin-top: 300px;text-align: center" class="col-md-6 col-md-offset-3"><h1>该功能仅对处领导与工程科公开！</h1></div>
</div>
</body>
<%
    }
%>
</html>