<%@ page language="java"
	pageEncoding="utf-8"%>
<%@page import="com.bitc.jk.xmis.model.security.BizModuleObj"%>
<%@page import="com.bitc.jk.xmis.model.security.BizSubModuleObj"%>
<%@page import="com.bitc.jk.xmis.model.security.BizFunctionObj"%>
<%@page import="com.bitc.jk.xmis.util.Type"%>
<!DOCTYPE HTML>
<html>
	<head>
		<%@include file="../jsCss.jsp"%>
		<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/styleTab.css" />
<!-- 奇偶行颜色
 -->
<script type="text/javascript">
window.onload = function(){
	var tb = document.getElementById("tab");
	var tbody = tb.getElementsByTagName("tbody")[0];
	var trs = tbody.getElementsByTagName("tr");
    for (var i = 0; i < trs.length; i++) {
        if (i % 2 == 0) {
            trs[i].style.backgroundColor = "rgb(249, 249, 249)";
        }
    }
};
</script>		
	</head>
	<body>
		<%
			List<BizModuleObj> list = (List<BizModuleObj>) request
					.getAttribute("resultList");
			Map<Integer,Integer> allModuleMapping = (Map<Integer,Integer>)request.getAttribute("allModuleMapping");
			Map<Integer,Integer> allSubModuleMapping = (Map<Integer,Integer>)request.getAttribute("allSubModuleMapping");
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
			if (list != null && list.size() > 0) {
			int moduleId = -2;  //模块ID
			int subModuleId = -2; //子模块ID
		%>
		<table id="tab" align="center" width="100%" border="0" class="tb"
			cellpadding="0" cellspacing="0">
			<tr>
				<th colspan="3">
					业务模块
				</th>
				<th colspan="4">
					业务子模块
				</th>
				<th colspan="4">
					业务功能
				</th>
			</tr>
			<tr>
				<th>
					名称
				</th>
				<th>
					提交人
				</th>
				<th>
					提交时间
				</th>
				<th>
					名称
				</th>
				<th>
					提交人
				</th>
				<th>
					提交时间
				</th>
				<th>
					状态
				</th>
				<th>
					名称
				</th>
				<th>
					提交人
				</th>
				<th>
					提交时间
				</th>
				<th>
					状态
				</th>
			</tr>
			<%
				for (int i = 0; i < list.size(); i++) {
					Map map = (Map)list.get(i);
					int modelId = Type.GetInt(map.get("业务模块ID"));
					int subModelId = Type.GetInt(map.get("业务子模块ID"));
					%>
					<tr>
					<%
					if(moduleId == -2|| moduleId != modelId){
						String moduleName = Type.getString(map.get("业务模块名称"));
						String moduleCommitPersonName = Type.getString(map.get("业务模块提交人"));
						String moduleCommitDate = "&nbsp;";
						if("".equals(moduleName.trim())){
							moduleName = "&nbsp;";
						}
						if("".equals(moduleCommitPersonName.trim())){
							moduleCommitPersonName = "&nbsp;";
						}
						if(map.get("业务模块提交时间")!=null){
							moduleCommitDate = sdf.format(map.get("业务模块提交时间"));
						}
						%>
						<td rowspan=<%=allModuleMapping.get(modelId) %> align="center"><%=moduleName %></td>
    					<td rowspan=<%=allModuleMapping.get(modelId) %>><%=moduleCommitPersonName %></td>
    					<td rowspan=<%=allModuleMapping.get(modelId) %> align="center"><%=moduleCommitDate %></td>
						<%
						moduleId = modelId;
					}
					if(subModuleId==-2||subModuleId!=subModelId){
						String subModuleName = Type.getString(map.get("业务子模块名称"));
						String subModuleCommitPersonName = Type.getString(map.get("业务子模块提交人"));
						String subModuleCommitDate = "&nbsp;";
						int subModuleStatus = Type.GetInt(map.get("业务子模块提交状态ID"));
						String subModuleStatusType = "";
						switch(subModuleStatus){
							case 1 : subModuleStatusType = "demand";
							break;
							case 2 : subModuleStatusType = "design";
							break;
							case 3 : subModuleStatusType = "develop";
							break;
							case 4 : subModuleStatusType = "finish";
							break;
							case 5 : subModuleStatusType = "online";
							break;
						}
						String subModuleStatusName = Type.getString(map.get("业务子模块提交状态"));
						if("".equals(subModuleName.trim())){
							subModuleName = "&nbsp;";
						}
						if("".equals(subModuleCommitPersonName.trim())){
							subModuleCommitPersonName = "&nbsp;";
						}
						if(map.get("业务子模块提交时间")!=null){
							subModuleCommitDate = sdf.format(map.get("业务子模块提交时间"));
						}
						if("".equals(subModuleStatusName.trim())){
							subModuleStatusName = "&nbsp;";
						}
						%>
						<td rowspan=<%=allSubModuleMapping.get(subModelId) %> class=<%=subModuleStatusType %> ><%=subModuleName %></td>
    					<td rowspan=<%=allSubModuleMapping.get(subModelId) %> class=<%=subModuleStatusType %> align="center"><%=subModuleCommitPersonName %></td>
    					<td rowspan=<%=allSubModuleMapping.get(subModelId) %> class=<%=subModuleStatusType %> align="center"><%=subModuleCommitDate %></td>
    					<td rowspan=<%=allSubModuleMapping.get(subModelId) %> class=<%=subModuleStatusType %> align="center"><%=subModuleStatusName %></td>
						<%
						subModuleId = subModelId;
					}
					//业务功能
					String funcName = Type.getString(map.get("业务功能名称"));
					String funcCommitPersonName = Type.getString(map.get("业务功能提交人"));
					String funcCommitDate = "&nbsp;";
					int funcStatus = Type.GetInt(map.get("业务功能提交状态ID"));
					String funcStatusType = "";
						switch(funcStatus){
							case 1 : funcStatusType = "demand";
							break;
							case 2 : funcStatusType = "design";
							break;
							case 3 : funcStatusType = "develop";
							break;
							case 4 : funcStatusType = "finish";
							break;
							case 5 : funcStatusType = "online";
							break;
						}
					String funcStatusName = Type.getString(map.get("业务功能提交状态"));
					if("".equals(funcName.trim())){
							funcName = "&nbsp;";
					}
					if("".equals(funcCommitPersonName.trim())){
						funcCommitPersonName = "&nbsp;";
					}
					if(map.get("业务功能提交时间")!=null){
						funcCommitDate = sdf.format(map.get("业务功能提交时间"));
					}
					if("".equals(funcStatusName.trim())){
						funcStatusName = "&nbsp;";
					}
					%>
						<td class=<%=funcStatusType %>><%=funcName %></td>
    					<td class=<%=funcStatusType %> align="center"><%=funcCommitPersonName %></td>
    					<td class=<%=funcStatusType %> align="center"><%=funcCommitDate %></td>
    					<td class=<%=funcStatusType %> align="center"><%=funcStatusName %></td>
					<%
					%>
					</tr>
					<%
				}
			}
			%>
		
	</body>
</html>
