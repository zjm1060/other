<script>
	var w=window.screen.width-10;
	var h=window.screen.height-55;
	var sStyle="width="+w+"px,height="+h+"px,left=0px,top=0px,scrollbars=yes,resizable=yes";
	window.open("/FlowEngineWeb/workflow/FlowFrame.jsp?action=0&flowInid=<%=request.getParameter("flowid")%>","",sStyle);
</script>