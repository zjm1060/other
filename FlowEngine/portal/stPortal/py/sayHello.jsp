<%@ page contentType="text/html; charset=gb2312" errorPage="/stPortal/include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.*,suntek.portal.publicLib.RecordSetObject" %>
<%@ page import="java.io.*" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
String m_usercode=(String)session.getAttribute(suntek.security.Security.m_sUser);
String m_username =oManager.getCurrentUserName();
%>
<html>
<head>
<title>问候</title>

<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<STYLE type=text/css>
<!--
.12pt {
	FONT-SIZE: 12pt
}
.10pt {
	FONT-SIZE: 10.5pt;
	COLOR: black
}
.10ptred {
	FONT-SIZE: 9pt;
	COLOR: #BE0405;
	padding-right:30px
}
TD {
	FONT: 9pt  Verdana,Tahoma,宋体
}

a:link {
	color: #660000;
	text-decoration: none
}
a:visited {
	color: #660000;
	text-decoration: none
}
a:active {
	color: #BE0405;
	text-decoration: none
}
a:hover {
	color: #BE0405;
	text-decoration: none
}
INPUT {
	FONT-SIZE: 9pt
}
SELECT {
	FONT-SIZE: 9pt
}
TABLE {
	FONT-SIZE: 9pt; CURSOR: default
}
.normal {
	BORDER-RIGHT: 0px solid; BORDER-TOP: 0px solid; BORDER-LEFT: 0px solid; BORDER-BOTTOM: 0px solid
}
.over {
	BORDER-RIGHT: #808080 1px solid; BORDER-TOP: #ffffff 1px solid; BORDER-LEFT: #ffffff 1px solid; BORDER-BOTTOM: #808080 1px solid
}
.out {
	BORDER-RIGHT: 0px solid; BORDER-TOP: 0px solid; BORDER-LEFT: 0px solid; BORDER-BOTTOM: 0px solid
}
.down {
	BORDER-RIGHT: #ffffff 1px solid; BORDER-TOP: #808080 1px solid; PADDING-LEFT: 2px; BORDER-LEFT: #808080 1px solid; PADDING-TOP: 2px; BORDER-BOTTOM: #ffffff 1px solid
}
.up {
	BORDER-RIGHT: 0px solid; BORDER-TOP: 0px solid; BORDER-LEFT: 0px solid; BORDER-BOTTOM: 0px solid
}
A {
	FONT-SIZE: 9pt; CURSOR: default
}
FORM {
	MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px
}
body {
	background-color: #fefefe;
	margin-left: 0px;
	margin-top: 0px;
	margin-right: 0px;
	margin-bottom: 0px;
}

#InfoBar
{
    FONT-SIZE: 100%;
    BACKGROUND: #fefefe;
    BORDER-BOTTOM: #999 1px solid
}

-->
</STYLE>

</head>

<body>
<div align=center style="width:100%;heigth:100%">
<% 
//计数器
String strDirPath = new File(application.getRealPath(request.getRequestURI())).getParent();

BufferedReader inf = new BufferedReader(new FileReader(strDirPath + "\\counter.txt"));
int tmp = Integer.parseInt(inf.readLine());
int i=0;

try {

i = Integer.parseInt(request.getSession().getValue("tal").toString());
} catch (NullPointerException t) {i=0; }

if (i==0) {
tmp++;

PrintWriter outf = new PrintWriter(new BufferedWriter(new FileWriter(strDirPath + "\\counter.txt")));
outf.println(tmp);
outf.close();
inf.close();
request.getSession().putValue("tal", "1");
}

BufferedReader inf2 = new BufferedReader(new FileReader(strDirPath + "\\counter.txt"));
%>
<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0 id="InfoBar">
        <TBODY>
        <TR>
          <TD align="center" noWrap>
            <P style="MARGIN-TOP: 3px; MARGIN-BOTTOM: 3px; MARGIN-LEFT: 10px">
            <%if(m_username.equals("管理员")){%>
            	<span onclick="javascript:window.open('/stPortal/SysManager/default.jsp');">界面设置&nbsp;&nbsp;&nbsp;&nbsp;</span> 
            <%}%>
            网站访问总人数&nbsp;&nbsp;
            <%
            //计数器
						String zeroes="";
						String hits = inf2.readLine();
						inf2.close();
						for (int t=0; t < 8-hits.length(); t++) {
						zeroes=zeroes+" "; }
						out.println(zeroes + hits);%>人&nbsp;&nbsp;&nbsp;&nbsp;<SCRIPT language=javascript type=text/javascript>                            var runTime = new Date();
              var hours = runTime.getHours();
              var dn = "";
              if(hours>6 && hours <12)
                dn="上午";
              else if(hours>= 12 && hours<=13){
                dn="中午";
              }
              else if (hours > 13 && hours<18) {
                dn = "下午";
                //hours = hours - 12;
              }else{
                dn = "晚上";
              }

              document.write(dn);
        </SCRIPT>好！<SPAN class="10ptred "><%=m_username%></SPAN>  
            <SCRIPT language=javascript type=text/javascript>                				var cur_Date = new Date();
                var cur_Year = cur_Date.getFullYear();
                var cur_Month = cur_Date.getMonth()+1;
                var cur_Day = cur_Date.getDate();

                var week = new Array("日", "一", "二");
                week = week.concat("三","四", "五");
                week = week.concat("六");
                day = cur_Date.getDay();

                document.write(cur_Year+"年"+cur_Month+"月"+cur_Day+"日 &nbsp;星期"+week[day]);
            </SCRIPT>
			<FONT id=Clock
            name="clock">9：00
            <SCRIPT language=javascript type=text/javascript>
            	function funClock() {
                  if (!document.layers && !document.all)
                  return;
                  var runTime = new Date();
                  var hours = runTime.getHours();
                  var minutes = runTime.getMinutes();
                  var seconds = runTime.getSeconds();
                  var dn = "上午";
                  if (hours >= 12) {
                  dn = "下午";
                  //hours = hours - 12;
                  }
                  if (hours == 0) {
                   hours = 12;
                  }
                  if (minutes <= 9) {
                   minutes = "0" + minutes;
                  }
                  if (seconds <= 9) {
                  seconds = "0" + seconds;
                  }
                  movingtime = "<font color='black'> "+ hours + ":" + minutes + ":" + seconds+"<\/font>" ;
                  if (document.layers) {
                    document.layers.clock.document.write(movingtime);
                    document.layers.clock.document.close();
                  }
                  else if (document.all) {
                    var obj = eval("Clock");
                    //alert(eval("Name").nodeName);
                    obj.innerHTML = movingtime;
                    //Clock.innerHTML = movingtime;
                  }
                  setTimeout("funClock()", 1000);
                }
            </SCRIPT>
			<SCRIPT language=javascript type=text/javascript>funClock()</SCRIPT>          </TD>
          </TR>
</TBODY>
</TABLE>
</div>
</body>
</html>



