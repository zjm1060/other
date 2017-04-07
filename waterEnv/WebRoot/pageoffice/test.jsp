<%--
  Created by IntelliJ IDEA.
  User: zsz
  Date: 2017/2/27
  Time: 15:08
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="com.zhuozhengsoft.pageoffice.DocumentOpenType" %>
<%@ page import="com.zhuozhengsoft.pageoffice.FileMakerCtrl" %>
<%@ page import="com.zhuozhengsoft.pageoffice.wordwriter.WordDocument" %>
<%@ taglib uri="http://java.pageoffice.cn" prefix="po" %>
<html>
<head>
    <title></title>
  <%
    FileMakerCtrl fmCtrl = new FileMakerCtrl(request);
    fmCtrl.setServerPage(request.getContextPath() + "/poserver.zz"); //此行必须
    //打开文件
    WordDocument worddoc = new WordDocument();
    worddoc.setDisableWindowRightClick(true);
    //先在要插入word文件的位置手动插入书签,书签必须以“PO_”为前缀
    //给DataRegion赋值,值的形式为："[word]word文件路径[/word]、[excel]excel文件路径[/excel]、[image]图片路径[/image]"
    //DataTag 不需要插入书签，直接使用[xxxx] 就可以
    worddoc.openDataTag("[YYYY]").setValue("2017");
    worddoc.openDataTag("[MM]").setValue("01");
    worddoc.openDataTag("[DD]").setValue("28");

    //正文
//    DataRegion data1 = worddoc.openDataRegion("PO_ZW");
//    data1.setValue("[word]11.docx[/word]");
//    DataRegion PO_CONTENT = worddoc.openDataRegion("PO_CONTENT");
//    PO_CONTENT.setValue("正文内容");
    String fileName = request.getParameter("fileName");
    fmCtrl.setWriter(worddoc);
    fmCtrl.setSaveFilePage("SaveFile.jsp");
    fmCtrl.setFileTitle("test11.doc");
    fmCtrl.fillDocumentAs("test.docx", DocumentOpenType.Word, fileName);
    fmCtrl.setJsFunction_OnProgressComplete("OnProgressComplete()");
    fmCtrl.setTagId("FileMakerCtrl1"); //此行必须
  %>
</head>
<script>
  function OnProgressComplete(){
      window.parent.openNewView();
  }
</script>
<body>
<div style="width: 100%; height: 100%;">
  <po:FileMakerCtrl id="FileMakerCtrl1" />
</div>
</body>
</html>
