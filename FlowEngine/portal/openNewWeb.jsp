<script>
  var w=window.screen.width-10;
	var h=window.screen.height-55;
	var sStyle="width="+w+"px,height="+h+"px,left=0px,top=0px,scrollbars=yes,resizable=yes";
	var sStyle="width="+w+"px,height="+h+"px,left=0px,top=0px,scrollbars=yes,resizable=yes";
	//var sStyle="width="+w+"px,height="+h+"px,left=0px,top=0px";
	window.open("<%=request.getParameter("WebUrl")%>","",sStyle);
</script>