<%@ page contentType="text/html; charset=gb2312" errorPage="/stPortal/include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.*,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
String m_usercode=request.getRemoteUser();
String m_username =oManager.getCurrentUserName();
%>
<html>
<head>
<title>问候
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</title>

<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link href="oacss.css" rel="stylesheet" type="text/css">

<STYLE type=text/css>.12pt {
	FONT-SIZE: 12pt
}
.10pt {
	FONT-SIZE: 10.5pt
}
.9pt {
	FONT-SIZE: 9pt
}
A:link {
	COLOR: black; TEXT-DECORATION: none
}
A:visited {
	COLOR: black; TEXT-DECORATION: none
}
A:hover {
	COLOR: red; TEXT-DECORATION: none
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
</STYLE>

</head>

<body rightmargin="0" bottommargin="0" bgColor="#99ccff" text="black" leftmargin="0" topmargin="0">
<div align=center style="width:100%">

<TABLE height=25 cellSpacing=0 cellPadding=0 width="100%" border=0>
        <TBODY>
        <TR>
          <TD noWrap>
            <P
            style="MARGIN-TOP: 3px; MARGIN-BOTTOM: 3px; MARGIN-LEFT: 10px"><FONT
            color=BLACK><FONT size=2 color="black">
            <SCRIPT language=javascript type=text/javascript>                            var runTime = new Date();
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
        </SCRIPT>好！</FONT><FONT color=black><SPAN><%=m_username%></SPAN> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
           现在是北京时间: <FONT id=Clock  name="clock" COLOR="YELLOW">9：00</FONT>
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

            <SCRIPT language=javascript type=text/javascript>funClock()</SCRIPT>
             </FONT></P></TD>
          <TD noWrap>
            <P style="MARGIN-RIGHT: 0px" align=right><FONT COLOR="black" size=2>
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
            </FONT></P></TD>
          <TD noWrap>
            <P align=right></P></TD></TR></TBODY>
</TABLE>
</div>
</body>
</html>
