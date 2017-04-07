<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="com.minstone.portal.util.XMLWriter"%>
<%@ page import="com.minstone.security.Security"%>
<%@ page import="java.io.StringReader"%>
<%@ page import="java.net.*"%>

<jsp:useBean id="dbutil" scope="page" class="jbportal.DBUtilBean" />
<jsp:useBean id="run" scope="page" class="jbportal.runBean"/>

<%
String rootpath = request.getContextPath();

%>
<HTML>
<link rel="stylesheet" type="text/css"	href="<%=rootpath%>/stPortal/Common/Css/IM.css">
<script language="javascript" src="<%=rootpath%>/stPortal/Common/Include/IM.js"></script>
<script language="javascript" src="./include/HttpSubmit.js" ></script>
<script language="vbscript" src="./include/encode.vbs" ></script>
<SCRIPT LANGUAGE="JavaScript" src="./include/newMsgTips.js"></SCRIPT>
<SCRIPT LANGUAGE="JavaScript">
  function lookMsg(sid){
  var wpath;		
		if(sid!=null){
		wpath="/stPortal/portalSvl?service=msg&func=showEditWindow&flg=send&receiver_code="+sid;	
		}else {
		return;
		}		
		window.showModalDialog(wpath,window,"scroll:no;status:no;center:yes;dialogHeight:370px;dialogWidth:650px;help=no;");
		}
		

	function showMessage(msgCounts)
	{
		var newMsg="";
		var i,j;
		for(i=0;i<msgCounts.length-1;i++){
		    if(msgCounts[i]==null||msgCounts[i].length==0) continue;
			var msgs=msgCounts[i].split(";");		
				newMsg+="<a href=\"#\" onclick=\"javascript:parent.lookMsg('"+msgs[2]+"')\"> <font color='red'>"+msgs[0]+"</font> 给你发来 ";
				newMsg+="<font color='red'>"+msgs[1]+"</font> 条消息</a><br>";
		}
		return newMsg;
	}
	
	//重新加载数据,每隔十分钟查询数据
	function reShow()
	{	
	//alert("reshow..");
	var url = 'service=msg&func=ifExist';      
    var bReturn = HttpSubmitString(url,"/stPortal/portalSvl");
    //alert("fanhuizhi :"+bReturn);
	 if(bReturn==null||typeof(bReturn)=="undefined") return;
        var returnArray=bReturn.split("#");
		//如果所有值都为空，则不提示
		var isOk=0;//是否提示
		if(returnArray!=null&&returnArray.length>0)
		{			
		for(i=0;i<returnArray.length;i++)
			{
				if(returnArray[i]!=0)
				{
					isOk=1;	
				}
			}			
		}
		
		if(isOk==1)
		{
	      var newMsg=showMessage(returnArray);
	      var MSG1 = new CLASS_MSN_MESSAGE("messageId",200,130,"短消息提示：",newMsg); 
	      MSG1.rect(null,null,null,screen.height-50); 
		  MSG1.speed = 10; 
		  MSG1.step  = 5;  
		  MSG1.show(); 
		}
		setInterval("reShow()",1000*60*5);//1分钟
	}
	//把滚动条设置为最底端
function getToButton()
{
  window.frames("docRcvMsg").scrollTo(0,window.frames("docRcvMsg").document.body.scrollHeight);
}
function registerToServer() {
    HttpSubmitString("service=onlineusers&func=registeruser","/stPortal/portalSvl");
    setTimeout("registerToServer()",10000); //10秒
}
function sendMsg(sid){	
		var receiverCode;
		//alert(sid);
		if(sid!=null){
		receiverCode=document.getElementsByName("usercode")[sid].value;		
		}else {
		return;
		}		
		var wpath="<%=rootpath%>/stPortal/portalSvl?service=msg&func=showEditWindow&flg=send&receiver_code="+receiverCode;
		//window.open(wpath);
		window.showModalDialog(wpath,window,"scroll:no;status:no;center:yes;dialogHeight:370px;dialogWidth:580px;help=no;");
		}
		
		function msg_history(){
			var url="<%=rootpath%>/stPortal/portalSvl?service=msg&func=oldMsgs";
		  url="/stPortal/portalSvl?service=msg&func=oldMsgs";
		  //url="/stPortal/OnlineUser/MsgHistory.jsp";
		  window.open(url);
		}
		
		function showUsers(){
		//加载用户信息
   	var userDiv=HttpSubmitString("service=onlineusers&func=toIM_Page","/stPortal/portalSvl");
    if(userDiv!=null)
	{
      document.getElementById("contentTb").innerHTML=userDiv;
	}
	window.setInterval("showUsers()",1000*60*3);
		}
</script>
<%
  	String AUTH_TYPE;
  	String LOGON_USER;
  	String AUTH_PASSWORD;
  	String URL;
  	String SERVER_NAME;
  	String xsl_filename;

    StringBuffer   sDashboard;
    String strWebPartCC = "urn:schemas-microsoft-com:webpart";
    String L_ERRORHEADING_TEXT = "<B>Dashboard Error</B>";
    String L_ERRORINTERNAL_TEXT = "An internal error occurred.";
    String L_ERRORDELETEPERMISSIONS_TEXT = "You do not have sufficient permissions to delete this item from the dashboard. Please contact your system administrator for more assistance.";
    String L_ERRORWEBPARTPERMISSIONS_TEXT = "You do not have sufficient permissions to add a web part to this dashboard. Please contact your system adminsistrator for more assistance.";
    String L_ERRORINVALIDDASHBOARD_TEXT = "The dashboard you have selected does not exist.";
    String L_ERRORCATALOGPERMISSIONS1_TEXT = "You do not have sufficient permissions to add the part, ";
    String L_ERRORCATALOGPERMISSIONS2_TEXT = " to the dashboard:";
    String L_ERRORCATALOGFOLDER_TEXT = "Failed to create the catalog folder: ";
    String varDashboardID;
    long   vardashboardType=0;
    String varTemp;
    String stPartCmd;
    String stPartID;
    String nodeTmp;
    String stXMLPartUpdate;
    String xmlPartUpdate;

    //store.jsp的定义
    String stUser;
    String stPwd;

    String xmlhttp;
    //init初始化相关变量Begin...
    AUTH_TYPE = request.getAuthType();
    LOGON_USER = request.getRemoteUser();
    URL = request.getRequestURI();
    SERVER_NAME = request.getServerName();
    xsl_filename = "./Dashboard.xsl";
    //init初始化相关变量End
    try{
        //Retrieve the dashboard identifier from the QueryString.
        varDashboardID =jbportal.PortalUtil.getValue(request.getParameter("DashboardID"));
        /*
        if (varDashboardID.equals(""))
           varDashboardID = "0001";
        */
        varTemp =jbportal.PortalUtil.getValueN(request.getParameter("DashboardType"));
        if(varTemp.equals("")) varTemp="0";
        vardashboardType = Integer.valueOf(varTemp).longValue();
        sDashboard = new StringBuffer(jbportal.PortalUtil.StoreGetRenderInfo(run,varDashboardID , vardashboardType));
        sDashboard = dbutil.AppendOtherXMLDOM(sDashboard);
        String l_sShow = jbportal.PortalUtil.CreateDashboardView(varDashboardID,sDashboard,URL,SERVER_NAME);
        //System.out.println("l_sShow="+l_sShow);

        String xsl =application.getRealPath("//stPortal//Dashboard.xsl");
        StringReader reader=new StringReader(l_sShow);
        jbportal.PortalUtil.convertToHtml(reader,xsl,out);
        reader.close();
        reader=null;
    }catch(Exception e){
        out.write( L_ERRORHEADING_TEXT + "<BR>");
        if (e.getMessage().equalsIgnoreCase(""))
             out.write( e.getMessage() + "<BR>");
        else
             out.write( L_ERRORINTERNAL_TEXT + "<BR>");
      } 
%>

<script>
	registerToServer();
	//showDepts();
	reShow();
</script>
<body onload="hiddleIM(2);">
<div class="IM_BODY" id="IM_DIV" name="IM_DIV" onmouseover="showIM('442');" >
		<table bgcolor="#ffffff" border="1" cellspacing="0" cellpadding="0"
			bordercolordark="#ffffff" bordercolorlight="#2097C4" width="100%">
			<tr>
				<td style='background-color:#C2CEEB;' height='22' valign="center"
					colspan="2">
					<table width="100%" cellpadding="0" cellspacing="0" border="0">
						<tr>
							<th valign="top" align="left">
								<font size='2'>&nbsp;MD-IM2008 Preview</font>
							</th>
							<td align='right' nowrap="">
								<img alt="全部收缩" style="cursor:hand;"
									src="<%=rootpath%>/stPortal/images/IM/minus.gif" border="0"
									onclick="hideAll();">
								<img alt="全部展开" style="cursor:hand;"
									src="<%=rootpath%>/stPortal/images/IM/funcmodify.gif"
									border="0" onclick="showAll();">
								<img style="cursor:hand;" alt="关闭页面"
									src="<%=rootpath%>/stPortal/images/IM/part_close.gif"
									border="0" onclick="hiddleIM(2);">
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td style="background: #E0E7F8;">
					<input type="text" value="" style="width:150px;"
						name="searchwhere">
					&nbsp;
					<img src="<%=rootpath%>/stPortal/images/IM/queryQuestion.gif"
						border="0" onclick="smanPromptList('searchwhere');">
				</td>
			</tr>
			<tr>
				<td height="350x">
					<table width="100%" cellspacing="0" cellpadding="0" border="0">
						<tr>
							<td>
								<div
									style="width:100%;height:370px;top:0px;left:0px;overflow-y:auto;overflow-x:auto;border: black 0px solid;background-color:#FFFFFF;background-image:url(<%=rootpath%>/stPortal/images/IM/frame_bg.gif) ;background-repeat:repeat-y;background-position-x:right;"
									name="contentTb" id="contentTb">
								<!--部门列表-->
								</div>
							</td>
						</tr>							
					</table>
				</td>
			</tr>
			<tr>
				<td onClick="msg_history();">
					<label style="cursor:head">
						&nbsp;&nbsp;查询历史信息
					</label>
				</td>
			</tr>				
		</table>
	</div>
	<SCRIPT LANGUAGE="JavaScript">
<!--
   reShow();
   //加载用户信息
   	showUsers();//一分钟加载
   	hiddleIM(2) ;
//-->
</SCRIPT>
</body>
</html>