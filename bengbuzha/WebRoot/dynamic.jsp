<%@page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@page contentType="text/javascript; charset=utf-8"%>

var __version = "<%=application.getAttribute(WebKeys.Session_Application_Cfg_Version) %>";
var __cxtPath="<%=request.getContextPath()%>";
if(__cxtPath=='/'){
	__cxtPath='';
}
var __upload={
	fileSize:20*1024,
	fileTypes:"*.*",
	fileTypesDescription:"所有类型"
};
var __fullPath ="<%=request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+request.getContextPath()+"/"%>";
<%EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA); 
if(emp!=null){
	out.print("var __emp="+com.bitc.jk.xmis.util.JSONUtils.toJSON(emp,true)+";");
}
%>

