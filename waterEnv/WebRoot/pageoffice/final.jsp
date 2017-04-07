<%--
  Created by IntelliJ IDEA.
  User: zsz
  Date: 2017/2/27
  Time: 15:08
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="com.zhuozhengsoft.pageoffice.wordwriter.WordDocument" %>
<%@ page import="com.zhuozhengsoft.pageoffice.PageOfficeCtrl" %>
<%@ page import="com.zhuozhengsoft.pageoffice.wordwriter.DataTag" %>
<%@ page import="com.zhuozhengsoft.pageoffice.OpenModeType" %>
<%@ page import="com.zhuozhengsoft.pageoffice.wordwriter.DataRegion" %>
<%@ taglib uri="http://java.pageoffice.cn" prefix="po" %>
<html>
<head>
  <title></title>
  <%--<%--%>
    <%--PageOfficeCtrl poCtrl1 = new PageOfficeCtrl(request);--%>
    <%--poCtrl1.setServerPage(request.getContextPath() + "/poserver.zz"); //此行必须--%>
    <%--//打开文件--%>
    <%--WordDocument worddoc = new WordDocument();--%>
    <%--//先在要插入word文件的位置手动插入书签,书签必须以“PO_”为前缀--%>
    <%--//给DataRegion赋值,值的形式为："[word]word文件路径[/word]、[excel]excel文件路径[/excel]、[image]图片路径[/image]"--%>
    <%--//DataTag 不需要插入书签，直接使用[xxxx] 就可以--%>
    <%--DataTag year = worddoc.openDataTag("[YYYY]");--%>
    <%--year.setValue("2017");--%>
    <%--DataTag month = worddoc.openDataTag("[MM]");--%>
    <%--month.setValue("01");--%>
    <%--DataTag day = worddoc.openDataTag("[DD]");--%>
    <%--day.setValue("28");--%>

    <%--//正文--%>
    <%--DataRegion data1 = worddoc.openDataRegion("PO_ZW");--%>
    <%--data1.setValue("[word]11.docx[/word]");--%>
    <%--DataRegion PO_CONTENT = worddoc.openDataRegion("PO_CONTENT");--%>
    <%--PO_CONTENT.setValue("正文内容");--%>
    <%--poCtrl1.setWriter(worddoc);--%>
    <%--poCtrl1.setCaption("测试页");--%>

    <%--//隐藏菜单栏--%>
    <%--poCtrl1.setMenubar(false);--%>
    <%--//隐藏自定义工具栏--%>
    <%--poCtrl1.setCustomToolbar(false);--%>
    <%--poCtrl1.setSaveFilePage("SaveFile.jsp?fileName=2123");--%>
    <%--poCtrl1.webOpen("test.docx", OpenModeType.docNormalEdit, "张三");--%>
    <%--poCtrl1.setTagId("PageOfficeCtrl1"); //此行必须--%>
  <%--%>--%>
</head>
<script>
  var fileName = ['222.doc','333.doc'];
  var index = 0;
  function openNewView(){
    if(index<fileName.length){
      document.getElementById('docCom').src="/pageoffice/test.jsp?fileName="+fileName[index];
      index++;
    }else{
      document.getElementById('docCom').src="/pageoffice/combination.jsp?fileName=fianlDoc.doc";
    }
  }
</script>
<body onload="openNewView()">
<div style="width: 100%; height: 100%;">
  <%--<po:PageOfficeCtrl id="PageOfficeCtrl1" />--%>
  aaa
</div>
<iframe id="docCom" style="width: 0px;height: 0px"></iframe>
</body>
</html>
