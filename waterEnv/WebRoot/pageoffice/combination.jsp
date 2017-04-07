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
  <%
    PageOfficeCtrl poCtrl1 = new PageOfficeCtrl(request);
    poCtrl1.setServerPage(request.getContextPath() + "/poserver.zz"); //此行必须
    //打开文件
    WordDocument worddoc = new WordDocument();
    //先在要插入word文件的位置手动插入书签,书签必须以“PO_”为前缀
    //给DataRegion赋值,值的形式为："[word]word文件路径[/word]、[excel]excel文件路径[/excel]、[image]图片路径[/image]"
    //DataTag 不需要插入书签，直接使用[xxxx] 就可以
    worddoc.openDataRegion("PO_ZW").setValue("[word]11.docx[/word]");
    worddoc.openDataRegion("PO_CONTENT").setValue("正文内容");
    poCtrl1.setWriter(worddoc);
    poCtrl1.setCaption("预览拟稿");

    //隐藏菜单栏
    poCtrl1.setMenubar(false);
    //隐藏自定义工具栏
    poCtrl1.setCustomToolbar(false);
    poCtrl1.setSaveFilePage("SaveFile.jsp");
    poCtrl1.webOpen("test.docx", OpenModeType.docNormalEdit, "张三");
    poCtrl1.setTagId("PageOfficeCtrl1"); //此行必须
  %>
</head>
<body>
<div style="width: 100%; height: 100%;">
  <po:PageOfficeCtrl id="PageOfficeCtrl1" />
</div>
</body>
</html>