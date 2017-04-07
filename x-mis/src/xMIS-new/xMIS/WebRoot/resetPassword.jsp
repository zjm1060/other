<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<!DOCTYPE HTML>
<%
	String path = request.getContextPath();
%>

<html>
	<head>
		<title>北京中水科技 监控事业部 综合信息管理系统</title>
		<jsp:include page="jsCss.jsp"></jsp:include>
		<link rel="shortcut icon" href="<%=path%>/favicon.ico" type="image/x-icon"/>
	    <style type="text/css">
	      input.text { width: 125px; height: 20px; border: 1px; border-width: 0px; color: #000; background-color:#fff
	      !important; font-family: Arial, '宋体'; font-weight: bold; padding: 3 4 4 3 !important; font-size: 16px; } body {
	      text-align: center; margin: 0 0 0 0; background-image: url('resources/images/login-back.jpg'); margin: 0 0 0 0; }
	      #login { margin: 200px auto 0 auto; background-image: url('resources/images/login-m.png'); background-repeat:
	      no-repeat; } .label { width: 85px; font-size: 20px; font-family: Tahoma; } #errmsg { color: red; font-size: 14px;
	      font-family: Tahoma; } #bottom { margin: 0 auto 0 auto; background-image:
	      url('resources/images/login-bottom.png'); background-repeat: no-repeat; height: 330px; width: 100%; position:
	      absolute; left:0px; bottom: 0px; z-index: -1 } #sbbtn{ width: 95px; height: 65px; cursor: pointer; }
	    </style>
	</head>

	<body>
	<div id="login" style="width: 623px; height: 140px;text-align:left;">
      <form action="<%=path%>/loginAction.do" method="POST" onsubmit="return check(this)" id = "userLogin">
        <input type=hidden name="uri" value="<%=request.getParameter(WebKeys.RequestKey_Original_URI)%>" />
        <input 
            type=hidden
            name="queryStr"
            value="<%=request.getParameter(WebKeys.RequestKey_Original_QueryString)%>"
        />
        <div style="position: relative; top: 50px; left: 115px; width: 100px;">
        <input id="loginName" type="text" name="loginName" value="" class="text"/>
        </div>
        <div style="position: relative; top: 66px; left: 115px; width: 100px;">
        <input id= "password" type="password" name="password" value="" class="text" />
        </div>
        <div 
            id="btndiv"
            style="position: relative; top: 0px; left: 270px; width: 95px; height: 65px; filter: alpha(Opacity = 0); -moz-opacity: 0; opacity: 0; background-color: #ffffff;">
          <input id="sbbtn" type="submit" value=" " />
        </div>
        <div style="position: relative; top: 0px; left: 115px; width: 100px;">
	        <span id="errmsg">
	          
	        </span>
        </div>
         <div style="padding-left:100px;font-size:12px;" title="今天之内不用再登录">
        	<input type="checkbox" name="remember" />记住我的登录状态
        </div>
      </form>
    </div>
    <div id="bottom">
    </div>
	
	<%
	String uri = (String) request.getAttribute("uri");
	String queryStr = (String) request.getAttribute("queryStr");
	String userid = (String) request.getAttribute("userID");
	String userName = (String) request.getAttribute("userName");
	%>
	<script type="text/javascript">
var win = new Ext.Window( {
	layout : 'fit',
	modal : true,
	title : "请修改登录密码",
	frame : true,
	plain : true,
	resizable : false,
	buttonAlign : "center",
	closable : false,
	bodyStyle : "padding:10px",
	width : 300,
	height : 180,
	layout : "form",
	lableWidth : 30,
	defaults : {
		xtype : "textfield",
		width : 120
	},
	monitorValid : true,
	items : [ 
	//{
	//	fieldLabel : "旧的密码",
	//	inputType : 'password',
	//	name : 'oldPwd',
	//	id : 'oldPwd',
	//	allowBlank : false,
	//	blankText : "必须输入旧密码"
	//}, 
	{
		fieldLabel : "新的密码",
		inputType : 'password',
		name : 'newPwd',
		id : 'newPwd',
		allowBlank : false,
		blankText : "必须输入新密码"
	}, {
		fieldLabel : "重新输入",
		inputType : 'password',
		name : 'renewPwd',
		id : 'renewPwd',
		allowBlank : false,
		blankText : "必须再输入新密码"
	} ],
	buttons : [{text : "确认",
				handler : function() {
					//if ('000000' != Ext.getCmp('oldPwd').getValue().trim()) {
					//	Ext.Msg.alert("错误", "旧密码有误，请重新输入！");
					//	oldPwd.focus();
					//	return false;
					//}

					if ((newPwd.value).trim() == "") {
						Ext.Msg.alert("错误", "请输入新密码！");
						newPwd.focus();
						return false;
					} else if ((renewPwd.value).trim() == "") {
						Ext.Msg.alert("错误", "请再次输入新密码！");
						renewPwd.focus();
						return false;
					} else if ((newPwd.value).trim() != (renewPwd.value).trim()) {
						Ext.Msg.alert("错误", "新密码2次输入不同，请重新输！");
						newPwd.focus();
						return false;
					} else if((newPwd.value).trim() == (renewPwd.value).trim()&&(newPwd.value).trim()=="000000"){
						Ext.Msg.alert("错误", "新密码不能为'000000'，请重新输！");
						newPwd.focus();
						return false;
					}else {
						document.getElementById('password').value = newPwd.value;
						document.getElementById('loginName').value = '<%=userName %>';
						Ext.Ajax.request( {
							url : __cxtPath + '/loginAction.do',
							success : function(response, config) {
								Ext.MessageBox.alert("提示",
										response.responseText,function(){
										document.getElementById('uri').value = '<%=uri%>';
										document.getElementById('queryStr').value = '<%=queryStr%>';
										document.getElementById('userLogin').submit();
										});
							},
							failure : function() {
								Ext.MessageBox.alert("提示",
										response.responseText);
							},
							method : "post",
							params : {
								method : 'updatePwd',
								userid : <%=userid %>,
								pwd : newPwd.value
							}
						});
						win.close();
					}
				}
			}]
});

	
	win.setPosition(400, 210);
	win.show();
	Ext.getCmp('newPwd').focus();
</script>
	</body>
</html>
