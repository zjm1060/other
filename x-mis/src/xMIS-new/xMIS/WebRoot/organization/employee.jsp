<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<!DOCTYPE HTML>
<%

EmployeeObj emp = (EmployeeObj)request.getAttribute(WebKeys.Request_Attribute_Name_EmployeeObjert);

String pwdDisabled = "disabled";
String eduDisabled = "disabled";
String cmpDisabled = "disabled";
String depDisabled = "disabled";
String addDisabled = "disabled";
String picDisabled = "disabled";
String savDisabeld = "disabled";
String lunDisabled = "disabled";
String disabledFlag = "disabled=ture";

if(((EmployeeObj) request.getSession()
				.getAttribute(WebKeys.Session_Attribute_Name_UserDNA)).getUserID() < 0){
	
		pwdDisabled = "";		
		eduDisabled	= "";
		cmpDisabled = "";
		depDisabled = "";
		addDisabled = "";
		picDisabled = "";
		savDisabeld = "";
		lunDisabled = "";
		disabledFlag = "";
} else { 
	if(((EmployeeObj) request.getSession()
			.getAttribute(WebKeys.Session_Attribute_Name_UserDNA)).getUserID() == emp.getUserID()){
		pwdDisabled = "";		
		eduDisabled	= "";
		picDisabled = "";
		savDisabeld = "";
		lunDisabled = "";
			
	}

}			

%>


<html>
  <head>
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	
	<link rel="stylesheet" type="text/css" href="/xMIS/resources/css/ext-all.css" />
	<LINK href="/xMIS/resources/css/styleCIS.css" type=text/css	rel=stylesheet>
    <link href="/xMIS/resources/css/calendarStyle.css" type=text/css rel=stylesheet></link>

		<script type="text/javascript" src="/xMIS/resources/js/ext-base.js"></script>
		<script type="text/javascript" src="/xMIS/resources/js/ext-all.js"></script>
	  	<script type="text/javascript" src="/xMIS/resources/js/ext-lang-zh_CN.js"></script>      

	    <link rel="stylesheet" type="text/css" href="/xMIS/resources/css/combos.css" />
     
		<script type="text/javascript" src="/xMIS/resources/js/common.js"></script>
		<script type="text/javascript" src="/xMIS/resources/js/calendarPopup.js"></script>
		<script language="JavaScript" src="/xMIS/resources/js/prototype.js"></script>

<script>
var cal1x = new CalendarPopup("calendarDiv");
var cal2x = new CalendarPopup("calendarDiv");
</script>

  </head>
     <DIV id=calendarDiv style="VISIBILITY: hidden; POSITION: absolute; BACKGROUND-COLOR: white; layer-background-color: white; border:0px; z-index:9999;"></DIV>
  
  <body>
  <form action="orgMaint.do" method="post" name="empFrm">
  	<input type=hidden name=method value=saveEmpInfo />
  	<input type=hidden name=userid value="<%=emp.getUserID() %>" />
    <TABLE class=grid id=employee cellSpacing=1 cellPadding=1 border=0 width=800px>
	  <tr class=even>
	    <TD class=info width=20px noWrap >&nbsp;&#22995;</TD>
	    <td width=40px >&nbsp;<INPUT class=firstname name=firstName value="<%=emp.getFirstName() %>"></td>
	    <TD class=info width=20px noWrap >名</TD>
	    <td>&nbsp;<INPUT class=lastname name=lastName value="<%=emp.getLastName() %>" ></td>
	    <TD class=info width=60px noWrap>登录名</TD>
	    <td>
	    	&nbsp;<INPUT class=text name=loginName size=10 value="<%=emp.getLoginName() %>">
	    	&nbsp;<input type=button class=blue_button name="setPassword" value="设置密码" <%=pwdDisabled %>/>
	    	<input type=hidden name="password" value=<%=emp.getPassword() %>>
	    	<input type=hidden name="newpassword" >
	    </td>
	    <TD width=60px class=info noWrap>性别</TD>
	    <td>
	    	<%String checked = emp.getSexID()!=0?"checked":""; %>
			<input type="radio" name="sex" value="1" <%=checked %>>男
	    	<%checked = emp.getSexID()==0?"checked":""; %>
			<input type="radio" name="sex" value="0" <%=checked %>>女
		</td>
	    <TD width=60px class=info noWrap>入职日期</TD>
	    <td width=110px noWrap>&nbsp;<INPUT class=date name=joinDate size=11 value="<%=emp.getJoinDate()==null?"":emp.getJoinDate().toString().trim() %>"  onChange="checkDateTime(this)" onkeypress=doKey('nubmer')>
			<a id=anchor1x onclick="cal1x.select(MM_findObj('joinDate'),'anchor1x','yyyy-MM-dd'); return false;" href="#">
				<IMG height=13 alt=calendar src="/xMIS/resources/images/calendar_2.gif" width=13 align=middle border=0></IMG> 
			</a>
	    </td>
	    <TD class=info rowSpan=4>
	    &nbsp;<IMG name="userPic" height=90 width=90 alt=photo src="/xMIS/resources/images/userPic/<%=emp.getImgName() %>" align=middle border=0></IMG>&nbsp;
		</TD>
	  </tr>
	  <tr class=even>
	    <TD width=60px class=info colSpan=2 noWrap>出生日期</TD>
	    <td colSpan=2 noWrap>
	    	&nbsp;<INPUT class=date name=birthday size=11 value="<%=emp.getBirthday()==null?"":emp.getBirthday().toString().trim() %>" onChange="checkDateTime(this)" onpropertychange="changaAge()" onkeypress=doKey('nubmer')>
			<a id=anchor2x onclick="cal2x.select(MM_findObj('birthday'),'anchor2x','yyyy-MM-dd'); return false;" href="#">
		    	<IMG height=13 alt=calendar src="/xMIS/resources/images/calendar_2.gif" width=13 align=middle border=0 ></IMG></a>
	    </td>
	    <TD width=60px class=info  noWrap>身份证号</TD>
	    <td>
	    	&nbsp;<INPUT class=IDCard name=IDCard value="<%=emp.getIDCard() %>" onkeypress=doKey('idCard')>
	    </td>
	    <TD width=60px class=info noWrap>年龄</TD>
	    <td>&nbsp;
	    <SPAN id=age><%=emp.getAge()==0?"":emp.getAge()+" 周岁" %></SPAN>
	    </td>
	    <TD width=60px class=info noWrap>
	    <input type=button class=blue_button_40_20 name=educationBtn value=学历 <%=eduDisabled %>>
	    <input type=hidden name=educationID value=<%=emp.getEducationHighestLevel() %>>
	    <input type=hidden name=educationSchool value=<%=emp.getSchoolName() %>>
	    <input type=hidden name=educationGDate value=<%=emp.getGraduationDate() %>>
	    <input type=hidden name=educationMajor value=<%=emp.getMajor() %>>
	    <input type=hidden name=educationRemark value=<%=emp.getEducationRemark() %>>
	    </TD>
	    <td>&nbsp;<INPUT class=text_readonly_short name="education" value="<%=emp.getEducationLevelName() %>" readonly></td>
	  </tr>
	  <tr class=even>
	    <TD width=60px class=info colSpan=2 noWrap>手机号码</TD>
	    <td colSpan=2>
	    	&nbsp;<INPUT class=phone name=mobileNo size=15 value="<%=emp.getMobileNo() %>" onkeypress=doKey('nubmer')>
	    </td>
	    <TD width=60px class=info noWrap>Email</TD>
	    <td>&nbsp;<INPUT class=IDCard name=email value="<%=emp.getEmailAddress() %>"></td>
	    <TD width=60px class=info noWrap>主页</TD>
	    <td colSpan=3>&nbsp;<INPUT class=text_long name=webPage value="<%=emp.getWebPage() %>"></td>
	  </tr>	  
	  <tr class=even>
	    <TD width=60px class=info colSpan=2 noWrap><input type=button class=blue_button_40_20 value=所属单位 <%=cmpDisabled %>></TD>
	    <td colSpan=4 noWrap>
		    &nbsp;<INPUT class=text_readonly_longest name=company value="<%=emp.getCompanyName() %>" readonly>&nbsp;
	    </td>
	    <TD width=60px class=info noWrap><input type=button class=blue_button_40_20 name=departmentBtn value=部门 <%=depDisabled %>></TD>
	    <td>&nbsp;<INPUT class=text_readonly_short name=deptName value="<%=emp.getDepartmentName() %>" readonly></TD>
	    <TD width=60px class=info noWrap>职务</TD>
	    <td>&nbsp;<INPUT class=chinese name=position value="<%=emp.getPosition()%>" size=10></TD>
	  </tr>	  
	  <tr class=even>
	    <TD width=60px class=info colSpan=2 rowSpan=2><input type=button class=blue_button_40_20 value=办公地址 <%=addDisabled %>></TD>
	    <td colSpan=4 rowSpan=2>
	    	&nbsp;<TEXTAREA name="officeAddress" class="textarea_long" cols="" rows="2" readonly="readonly"><%=emp.getOfficeAddress() %></textarea>
	    </td>
	    <TD width=60px class=info noWrap>单位电话</TD>
	    <td>&nbsp;<INPUT class=text name=officeTelNo size=10 value="<%=emp.getSwitchboardNo() %>" onkeypress=doKey('nubmer')></TD>
	    <TD class=info noWrap>分机号码</TD>
	    <td>&nbsp;<INPUT class=text name=extNo size=10 value="<%=emp.getExtNo() %>" onkeypress=doKey('nubmer')></TD>
	    <td rowSpan=7 valign=bottom>
	    	<table>
	    		<tr><td style="line-height:300px;">&nbsp;&nbsp;&nbsp;<input type=button class=blue_button name="uploadPhoto" value="上传照片" <%=picDisabled %> /></td></tr>
	    		<!-- tr><td style="line-height:300px;">&nbsp;&nbsp;&nbsp;<input type=button class=blue_button value="权限设置" /></td></tr-->
	    		<tr><td style="line-height:300px;">&nbsp;&nbsp;&nbsp;<input type=button class=blue_button value="信息保存" onclick="saveEmpInfo()" <%=savDisabeld %>/></td></tr>
	    	</table>
	    </TD>
	  </tr>	  
	  <tr class=even>
	    <TD width=60px class=info noWrap>传真号码</TD>
	    <td>&nbsp;<INPUT class=text name=faxNo size=10 value="<%=emp.getFaxNo() %>" onkeypress=doKey('nubmer')></TD>
	    <TD width=60px class=info noWrap>
	    <input type=button class=blue_button_40_20 name=lunchAddBtn value=午餐地点 <%=lunDisabled %>>
	    <input type=hidden name=lunchAddressID value=<%=emp.getLunchAddressID() %>>
	    </TD>
	    <td>&nbsp;<INPUT class=text_readonly_short name="lunchAddress" value="<%=emp.getLunchAddress() %>" readonly></td>
	  </tr>	  	  
	  <tr class=even>
	    <TD width=60px class=info colSpan=2 noWrap rowSpan=2>家庭地址</TD>
	    <td colSpan=4 rowSpan=2>
	    	&nbsp;<TEXTAREA name="homeAddress" class="textarea_long" cols="" rows="2" ><%=emp.getHomeAddress() %></textarea>
	    </td>
	    <TD width=60px class=info noWrap>邮政编码</TD>
	    <td>&nbsp;<INPUT class=text name=zipCode size=10 value="<%=emp.getZipCode() %>" onkeypress=doKey('nubmer')></TD>
	    <TD width=60px class=info noWrap>住宅电话</TD>
	    <td>&nbsp;<INPUT class=text name=homeTelNo size=10 value="<%=emp.getHomeTelNo() %>" onkeypress=doKey('nubmer')></TD>
	  </tr>
	  <tr class=even>
	    <TD width=60px class=info noWrap>政治面貌</TD>
	    <TD >
	    &nbsp;<select style="BACKGROUND-COLOR: #ffffcc" name="politicalStatus">
	    	<option value="0">---</option>
	    <%
				int mapsize = emp.getPoliticalStatusMap().size();
				Iterator it = emp.getPoliticalStatusMap().entrySet().iterator();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					String selected = ((Integer)entry.getKey()).intValue() == emp.getPoliticalStatus()?"selected":"";
	     %>
	     	<option value="<%=String.valueOf(entry.getKey())%>" <%=selected %>><%=String.valueOf(entry.getValue())%></option>
	     <%} %>
	    	</select>
	    </TD>
	    <TD width=60px class=info noWrap>籍贯</TD>
	    <TD>&nbsp;<INPUT class=chinese name=nativePlace size=10 value="<%=emp.getNativePlace() %>"></TD>
	  </tr>	  	  
	  <tr class=even>
	    <TD width=60px class=info colSpan=2 noWrap>婚姻状况</TD>
	    <td colSpan=2>&nbsp;
	    	<%checked = emp.getMaritalStatus()!=1?"checked":""; %>
			<input type="radio" name="MaritalStatus" value="0" <%=checked %>>未婚
	    	<%checked = emp.getMaritalStatus()==1?"checked":""; %>
			<input type="radio" name="MaritalStatus" value="1" <%=checked %>>已婚
		</td>
	    <TD width=60px class=info noWrap>配偶姓名</TD>
	    <td>&nbsp;<INPUT class=chinese name=spouseName value="<%=emp.getSpouseName() %>"></TD>
	    <TD width=60px class=info noWrap>子女姓名</TD>
	    <td>&nbsp;<INPUT class=chinese name=issueName size=10 value="<%=emp.getIssueName() %>"></TD>
	    <TD width=60px class=info noWrap>子女性别</TD>
	    <td>&nbsp;
	    	<%checked = emp.getIssueSex()!=0?"checked":""; %>
			<input type="radio" name="issueSex" value="1" <%=checked%>>男
	    	<%checked = emp.getIssueSex()==0?"checked":""; %>
			<input type="radio" name="issueSex" value="0" <%=checked%>>女
	    </td>
	  </tr>	  
	  <tr class=even>
	    <TD width=60px class=info colSpan=2 noWrap>兴趣爱好</TD>
	    <td colSpan=4>&nbsp;<INPUT class=text_longest name=hobby value="<%=emp.getHobby() %>"></TD>
	    <TD width=60px class=info noWrap>特长</TD>
	    <td colSpan=3>&nbsp;<INPUT class=text_long name=specialty value="<%=emp.getSpecialty() %>"></TD>
	  </tr>
	  <tr class=even>
	    <TD width=60px class=info colSpan=2 noWrap>禁用</TD>
	    <td colSpan=2>&nbsp;
	    	<%if(disabledFlag.equals("")){ %>
	    	<%checked = emp.getDisabledFlag()!=1?"checked":""; %>
			<input name="disabledFlag" type="radio" value="0" <%=checked%> <%=disabledFlag %>>否
	    	<%checked = emp.getDisabledFlag()==1?"checked":""; %>
			<input name="disabledFlag" type="radio" value="1" <%=checked%> <%=disabledFlag %>>是
			<%checked = ""; %>
	    	<%}else{ %>
			<input type=hidden name=disabledFlag value=<%=emp.getDisabledFlag() %>>	    
			<%=emp.getDisabledFlag()==1?"是":"否" %>	
	    	<%} %>
	    </td>
	    <TD width=60px class=info noWrap>控制文件</TD>
	    <TD class=info>&nbsp;</TD>
	    <TD width=60px class=info noWrap>附注</TD>
	    <td colSpan=3>&nbsp;<INPUT class=text_long name=remark value="<%=emp.getRemark() %>"></TD>
	  </tr>	  
	 </table>
	 <%if(emp.getDisabledFlag() == 1 ) {%>
		 <h3>该用户已被禁用；如需重新启用，请联系管理员。</h3>
	 <%}%>
	</form>
  </body>
</html>



<script language="javascript">

function doKey(type){
	//alert(event.keyCode);
/*
	if(event.keyCode == 45){
		alert("The Value should be POSITIVE number!");
		event.keyCode = 0;
		event.returnvalue = false;
		return false;
	}
*/

	if(type == 'idCard'){
          if(event.keyCode<48 || event.keyCode>57){
			alert("身份证号码必须是数字!");
			event.keyCode = 0;
			event.returnvalue = false;
			return  false;
          }
    }else if(type =='nubmer'){
          if(event.keyCode !=45 && (event.keyCode<48 || event.keyCode>57)){
			//alert("必须是数字!");
			event.keyCode = 0;
			event.returnvalue = false;
			return  false;
          }
    }

}

function checkDateTime(obj) { 

	if(obj.value == "") return true;
	var strTime = obj.value;
	var dateReg = /^(\d{1,4})(-|\/)(\d{1,2})\2(\d{1,2})$/; 
//	return dateReg.test(strTime.trim()); 
	if(dateReg.test(strTime.trim()))
		return true;
	else{
		alert("日期设置无效，请按 “年-月-日” 格式设置！");
		obj.value="";
		obj.focus();
//		document.all.birthday.focus();
		return false;
	}
} 

function changaAge(){
	var spanObj =MM_findObj("age");
	var age = MM_findObj("birthday").value==""? "": (new Date()).getYear()- MM_findObj("birthday").value.substring(0,4);
	spanObj.innerHTML=age+ " 周岁";
}


Ext.onReady(function(){
	Ext.QuickTips.init(); 

    /*====================================================================
     * password; 
     *====================================================================*/
	
    var pwdButton = Ext.get('setPassword');
    pwdButton.on('click', function(){
			var win=new Ext.Window({ 
				layout: 'fit',
				modal: true,
				title:"设置登录密码", 
				renderTo:Ext.getBody(),
				frame:true, 
				plain:true, 
				resizable:false, 
				buttonAlign:"center", 
				//closeAction:"close", 
				//maximizable:true, 
				closable:true, 
				bodyStyle:"padding:10px", 
				width:300, 
				height:180, 
				layout:"form", 
				lableWidth:30, 
				defaults:{xtype:"textfield",width:120}, 
				monitorValid:true,
				items:[
					<%if(!"".equals(emp.getPassword())){%>
					{fieldLabel:"旧的密码", inputType:'password', name:'oldPwd',allowBlank:false, blankText:"必须输入旧密码" },
					<%}%>
					{fieldLabel:"新的密码", inputType:'password', name:'newPwd',allowBlank:false, blankText:"必须输入新密码" },
					{fieldLabel:"重新输入", inputType:'password', name:'renewPwd',allowBlank:false, blankText:"必须再输入新密码"}
				], 
				buttons:[
					{text:"确认",
						handler:function(){
					<%if(!"".equals(emp.getPassword())){%>
							if(document.all.password.value != oldPwd.value){
								alert("旧密码有误，请重新输入！");
								oldPwd.focus();
								return false;
							}
					<%}%>
							
							if(newPwd.value==""){
								alert("请输入新密码！");
								newPwd.focus();
								return false;
							}else if(renewPwd.value==""){
								alert("请再次输入新密码！");
								renewPwd.focus();
								return false;
							}else if(newPwd.value != renewPwd.value){
								alert("新密码2次输入不同，请重新输！");
								newPwd.focus();
								return false;
							}else{
								document.all.newpassword.value = newPwd.value;
								document.all.password.value = newPwd.value;
								
					<%if(!"".equals(emp.getPassword())){%>
								sendRequest(newPwd.value);
					<%}%>
								win.close();
							}
						}},
					{text:"取消",handler:function(){win.hide();}}
				], 
				listeners:{ 
					"show":function(){ 
						//alert("display"); 
					}, 
					"hide":function(){ 
						//alert("hidden"); 
					}, 
					"close":function(){ 
						//alert("close"); 
					} 
				} 
			}) 
	win.setPosition(300, 100);
	win.show(); 
    });


    /*====================================================================
     * education; 
     *====================================================================*/
    var eduButton = Ext.get('educationBtn');

    eduButton.on('click', function(){
   		    var eduD = new Ext.form.DateField({
		        name: 'graduationDate',
		        format: 'Y-m-d',
		        maxValue: new Date(),
		        minValue: '1900-01-01',
		        //disabledDays: [0, 6],
		        //disabledDaysText: '禁止选择该日期',
		        fieldLabel: '毕业日期',
		        //width:200,
		        showToday:true ,
		        editable:false,
		        value: document.all.educationGDate.value                  
		    });

			var form = new Ext.form.FormPanel({
			    baseCls : 'x-plain',
			    anchor : '100%',
				lableWidth:10, 
				defaultType : 'textfield',
				defaults:{xtype:"textfield",width:200}, 
				items:[
					{inputType:'hidden', name:'eduID', value:document.all.educationID.value},
					{inputType:'hidden', name:'oldEduName', value:document.all.education.value},
					{fieldLabel:"学&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;历", name:'eduLevel', value:document.all.education.value},
					{fieldLabel:"毕业院校", inputType:'text', name:'schoolName', value:document.all.educationSchool.value},
					eduD,
					{fieldLabel:"学习专业", inputType:'text', name:'major', value:document.all.educationMajor.value},
					{fieldLabel:"附&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;注", inputType:'text', name:'eduRemark', value:document.all.educationRemark.value}
				]
		    });
		    
			var winEduEdit=new Ext.Window({ 
				layout: 'fit',
				modal: true,
				title:"学历记录编辑", 
				renderTo:Ext.getBody(), 
				frame:true, 
				plain:true, 
				resizable:false, 
				buttonAlign:"center", 
				//closeAction:"close", 
				//maximizable:true, 
				closable:true, 
				bodyStyle:"padding:10px", 
				width:360, 
				height:220, 
				layout:"form", 
				//lableWidth:10, 
				defaults:{xtype:"textfield",width:200}, 
				//monitorValid:true,
				items:form, 
				buttons:[
					{text:"确认",
						handler:function(){
							
							if(document.all.eduLevel.value==""){
								alert("请输入学历名称！");
								document.all.eduLevel.focus();
								return false;
							}else if(document.all.schoolName.value == ""){
								alert("请输入学校名称！");
								document.all.schoolName.focus();
								return false;
							}else if(document.all.graduationDate.value == ""){
								alert("请输入毕业日期！");
								document.all.graduationDate.focus();
								return false;
							}else if(document.all.major.value == ""){
								alert("请输入所学专业！");
								document.all.major.focus();
								return false;
							}														
						
							form.getForm().submit({
						         url : 'orgMaint.do?method=saveEducation',
						         method : 'post',
								 params : {
								 	userid:'<%=emp.getUserID() %>',
								 	oldEdu:'<%=emp.getPoliticalStatus() %>'
								 }	,
						         success : function(form, action) {
						         	//alert(action.result.message);
						         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
							
									document.all.educationID.value = action.result.eduID;
									document.all.education.value = action.result.eduName;
									document.all.educationSchool.value = action.result.eduSchool;
									document.all.educationGDate.value = action.result.eduDate;
									document.all.educationMajor.value = action.result.eduMajor;
									document.all.educationRemark.value = action.result.eduRemark;
	
						         	winEduEdit.close();
						         	
						         },
					         	failure : function(form, action) {
						         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
					         	}
							});
						}},
					{text:"取消",handler:function(){winEduEdit.close();}}
				], 
				listeners:{ 
					"show":function(){ 
						//alert("display"); 
					}, 
					"hide":function(){ 
						//alert("hidden"); 
					}, 
					"close":function(){ 
						//alert("close"); 
					} 
				} 
			}) 
	winEduEdit.setPosition(240, 100);
	winEduEdit.show(); 
    });
    


    /*====================================================================
     * lunchAddress grid bind window ; 
     *====================================================================*/
    var combo = new Ext.form.ComboBox({
	    id:'lunchAddID',
		//blankText: '请选择午餐地点.',
		//allowBlank: false,
        store: <%=emp.getLunchAddressData() %>,
        displayField:'state',
        typeAhead: true,
        mode: 'local',
        forceSelection: true,
        triggerAction: 'all',
        emptyText:'请选择...',
		editable: false, //false则不可编辑，默认为true
        selectOnFocus:true
    });

	var but = new Ext.Button({
		id:'a',
		text:'确定',
		display: false,
		handler:function(){
		
			//alert(Ext.getCmp("lunchAddID").getValue());
			
			<%
			if(Integer.parseInt((String)request.getAttribute(WebKeys.Request_Attribute_Name_CurrentDATE)) > WebKeys.LunchUpdatedTerm ){
			%>
				alert("本功能每月1日至<%=String.valueOf(WebKeys.LunchUpdatedTerm) %>日开放！");
				return false;
				
			<%}else{%>
			comboForm.getForm().submit({
		         url : 'orgMaint.do?method=changeLunchAddress',
		         method : 'post',
				 params : {
				 	userid:'<%=emp.getUserID() %>',
				 	newAddress:Ext.getCmp("lunchAddID").getValue()
				 }	,
		         success : function(form, action) {
		         	//alert(action.result.message);
		         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
			
					document.all.lunchAddress.value = Ext.get("lunchAddID").dom.value;
		         	document.all.lunchAddressID.value = Ext.getCmp("lunchAddID").getValue();
		         	store.load();
		         },
	         	failure : function(form, action) {
		         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
	         	}
			});
			<%}%>
		}		
	});
	
	var jBut = new Ext.Button({
		id:'jb',
		text:'Journal',
		display: false,
		handler:function(){
		
			//alert(Ext.getCmp("lunchAddID").getValue());

			<%
			if(Integer.parseInt((String)request.getAttribute(WebKeys.Request_Attribute_Name_CurrentDATE)) < WebKeys.LunchUpdatedTerm ){
			%>
				alert("本功能每月<%=String.valueOf(WebKeys.LunchUpdatedTerm) %>日以后开放！");
				//return false;
				
			<%}else{%>
			comboForm.getForm().submit({
		         url : 'orgMaint.do?method=lunchAddressJournal',
		         method : 'post',
		         success : function(form, action) {
		         	//alert(action.result.message);
		         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
			
					document.all.lunchAddress.value = Ext.get("lunchAddID").dom.value;
		         	document.all.lunchAddressID.value = Ext.getCmp("lunchAddID").getValue();
		         	store.load();
		         },
	         	failure : function(form, action) {
		         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
	         	}
			});
			<%}%>			

		}		
	});
		
	var comboForm = new Ext.form.FormPanel({

	    baseCls : 'x-plain',
	    anchor : '80%',
	    width:600,
	    
		items:[{
			baseCls : 'x-plain',
			xtype:"panel",
			layout:"column",
			fieldLabel:"选择午餐地点",
			isFormField:true,
			items:[combo,but<%if(((EmployeeObj) request.getSession()
				.getAttribute(WebKeys.Session_Attribute_Name_UserDNA)).getUserID() < 0){
 %>,jBut<%}%>]
		}]					
		//buttons:[{text:"确定"},{text:"取消"}]
    });
    
	var store = new Ext.data.GroupingStore({
		url:'orgMaint.do?method=getLunchAddressList',
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount',
			remoteSort:true,
            fields:[
                 {name:'dept_name'},
                 {name:'user_name'},
                 {name:'old_address_name'},
                 {name:'lunchAddress_name'},
                 {name:'changed_flag'}
           ]
		}),
		sortInfo:{field: 'dept_name', direction: 'ASC'},
		groupField:'changed_flag'
		
	});

	var colM=new Ext.grid.ColumnModel(
		[new Ext.grid.RowNumberer(),
		{header: "部门名称",dataIndex:"dept_name",sortable:true},
		{header: "员工姓名",dataIndex:"user_name",sortable:true},
		{header: "原午餐地点",dataIndex:"old_address_name",sortable:true},
		{header: "新午餐地点",dataIndex:"lunchAddress_name",sortable:true},
		{header: "状态",dataIndex:"changed_flag",sortable:true}
		]
	);  
	
	var gridM = new Ext.grid.GridPanel({
		cm:colM,
		store:store,
		view: new Ext.grid.GroupingView({
            forceFit:true,
            hideGroupedColumn: true, 	//用来分组的数据点这一列是否要显示 
            
            showGroupName: true, 	//用来分组的数据点这一列的header是否要随group name一起显示 
            
            startCollapsed: false,	//一开始进到grid这页，它的group是合起还是展开 
            
            
            groupTextTpl: '{text} ({[values.rs.length]} {[values.rs.length > 1 ? "人" : "人"]})'
        }),
		
        collapsible: true,
        animCollapse: false,
        iconCls: 'icon-grid',
        frame:true,
	   
	  	bbar: new Ext.PagingToolbar({
            pageSize: 0,
            store: store,
            displayInfo: true,
            displayMsg: '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
            emptyMsg: "无显示数据"
        }),
 
 
	  
		height:320,
		autoHeight: false,
		title: '午餐地点统计'
		
		/*
		autoExpandColumn:2,
		stripeRows: true,

      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: true,   
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
       				
		// paging bar on the bottom
       	bbar: new Ext.PagingToolbar({
			pageSize: 20,
			store: store,
			displayInfo: true,
			displayMsg: '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg: "无显示数据"
        })
        */
    }); 	  
    
    var winLunch;
    var lunchButton = Ext.get('lunchAddBtn');
    lunchButton.on('click', function(){
         if(!winLunch){
			winLunch=new Ext.Window({ 
				modal: true,
				title:"午餐地点设置", 
				renderTo:Ext.getBody(), 
				frame:true, 
				plain:true, 
				resizable:false, 
				closeAction:"hide", 
				//maximizable:true, 
				closable:true, 
				bodyStyle:"padding:10px", 
				width:600, 
				height:400, 
				items:[comboForm,gridM]
			}) 
        }
    winLunch.setPosition(120, 80);    
	winLunch.show(); 
	store.removeAll();
	store.load();
    });    

	function biuldLinker(val) {
		if (val.indexOf("^") > 0) {
			var text = val.substring(0, val.indexOf("^"));
			var url = val.substring(val.indexOf("^") + 1, val.length);
			if (text != "0") {
				return "<a href=\"" + url + "\">" + text + "</a>";
			} else {
				return text;
			}
		} else {
			return val;
		}
	}
	Ext.getCmp("lunchAddID").setValue( document.all.lunchAddressID.value );
	
/*
 //grid bind window
	var store=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({url:'orgMaint.do?method=getDeptTotalSummaryStore&<%=WebKeys.Type_Department%>=2'}),
		remoteSort:true,
		reader:new Ext.data.XmlReader({
			totalProperty: 'TotalResults',
			record:"Item"},
		[
			"user_name",
			"proj_code",
			"proj_name",
			"start_date",
			"finished_task",
			"unfinished_task",
			"not_start_task"
		])
	});

	var colM=new Ext.grid.ColumnModel(
		[
		{header: "员工姓名",dataIndex:"user_name",sortable:true, renderer: biuldLinker},
		{header: "项目编码",dataIndex:"proj_code",sortable:true, renderer: biuldLinker},
		{header: "项目名称",dataIndex:"proj_name",sortable:true, renderer: biuldLinker},
		{header: "项目启动日期",dataIndex:"start_date",sortable:true, renderer: Ext.util.Format.dateRenderer('Y-m-d'),align: 'center'},
		{header: "已完成任务数",dataIndex:"finished_task",sortable:true, renderer: biuldLinker,align: 'right'},
		{header: "进行中任务数",dataIndex:"unfinished_task",sortable:true, renderer: biuldLinker,align: 'right'},
		{header: "未开始任务数",dataIndex:"not_start_task",sortable:true, renderer: biuldLinker,align: 'right'}
		]
	);  
	
	store.load({params:{start:0, limit:20}});  
	
	var grid = new Ext.grid.GridPanel({
		cm:colM,
		store:store,
		autoExpandColumn:2,
		stripeRows: true,

      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: true,   
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
       				
		// paging bar on the bottom
       	bbar: new Ext.PagingToolbar({
			pageSize: 20,
			store: store,
			displayInfo: true,
			displayMsg: '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg: "无显示数据"
        })
    }); 	  
    
    var winEDU;
    var deptButton = Ext.get('departmentBtn');
    deptButton.on('click', function(){
         if(!winEDU){
			winEDU=new Ext.Window({ 
				modal: true,
				title:"学历设置", 
				renderTo:Ext.getBody(), 
				frame:true, 
				plain:true, 
				resizable:false, 
				closeAction:"hide", 
				//maximizable:true, 
				closable:true, 
				bodyStyle:"padding:10px", 
				width:800, 
				height:600, 
				items:[grid]
			}) 
        }
	winEDU.show(); 
    });    

	function biuldLinker(val) {
		if (val.indexOf("^") > 0) {
			var text = val.substring(0, val.indexOf("^"));
			var url = val.substring(val.indexOf("^") + 1, val.length);
			if (text != "0") {
				return "<a href=\"" + url + "\">" + text + "</a>";
			} else {
				return text;
			}
		} else {
			return val;
		}
	}
*/



    /*====================================================================
     * upload ; 
     *====================================================================*/

	var uploadBtn = Ext.get('uploadPhoto');
	
	uploadBtn.on('click', function(){
	
		var form = new Ext.form.FormPanel({
		     baseCls : 'x-plain',
		     labelWidth : 70,
		     fileUpload : true,
		     defaultType : 'textfield',
		     items : [{
		        xtype : 'textfield',
		        autoCreate:{tag: "input", type: "file", contenteditable:false},
		        fieldLabel : '上传文件名',
		        id : 'userfile',
		        inputType : 'file',
		        anchor : '100%' // anchor width by percentage
		       }]
		    });
		
		var winUpl = new Ext.Window({
		     modal: true,
		     title : '照片上传',
		     width : 400,
		     height : 100,
		     minWidth : 300,
		     minHeight : 100,
		     layout : 'fit',
		     plain : true,
		     bodyStyle : 'padding:5px;',
		     buttonAlign : 'center',
		     items : form,
		     buttons : [{
		     	text : '上传',
				handler : function() {
			       	if (form.form.isValid()) {
			        	if(Ext.getCmp('userfile').getValue() == ''){
			         		//Ext.MessageBox.alert('错误','请选择你要上传的文件');
			         		alert('请选择你要上传的文件');
			         		return;
			        	}
			        	Ext.MessageBox.wait('正在上传图片, 请稍侯 ...', '提示');   
			        	
			        	/*
			        	Ext.MessageBox.show({
			           		title : '请等待',
			           		msg : '文件正在上传...',
							progressText : '',
							width : 300,
							progress : true,
							closable : false,
							animEl : 'uploadPhoto'
			          	});
			          	
						// this hideous block creates the bogus progress
						var f = function(v){
						     return function(){
						         if(v == 12){
						             Ext.MessageBox.hide();
						             //winUpl.close();
						         }else{
						             var i = v/11;
						             Ext.MessageBox.updateProgress(i, Math.round(100*i)+'% 已完成');
						         }
						    };
						};
						for(var i = 1; i < 13; i++){
						    setTimeout(f(i), i*500);
						};	
						*/		          	
			        	form.getForm().submit({
					         url : '/xMIS/fileUpload.do?type=1&userid=<%=emp.getUserID()%>',
					         method : 'post',
					         params : '',
					         success : function(form, action) {
					         	//alert(action.result.message);
					         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
						
								document.all.userPic.src = action.result.picPath+"?d="+Math.random();

					         	winUpl.close();
					         	
					         },
				         	failure : function(form, action) {
					         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
				         	}
			        	})
			       	}
		      	}
		     }, {
		     	text : '关闭',
		     	handler : function() {
		       		winUpl.close();
		     	}
		   }]
		});
		winUpl.setPosition(250, 100);
		winUpl.show();
	});

});


var XMLHttpReq;

//创建XMLHttpRequest对象       
function createXMLHttpRequest() {
	if(window.XMLHttpRequest) { //Mozilla 浏览器
		XMLHttpReq = new XMLHttpRequest();
	}
	else if (window.ActiveXObject) { // IE浏览器
		try {
			XMLHttpReq = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			try {
				XMLHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e) {}
		}
	}
}

//发送请求函数
function sendRequest(pwd) {
	createXMLHttpRequest();
	var url = "orgMaint.do?method=updatePwd&userid="+<%=emp.getUserID() %>+"&pwd="+pwd;
	XMLHttpReq.open("GET", url, true);
	XMLHttpReq.onreadystatechange = processResponse;//指定响应函数
	XMLHttpReq.send(null);  // 发送请求
}

function processResponse() {
	if (XMLHttpReq.readyState == 4) { // 判断对象状态
		var res = XMLHttpReq.responseText;
    	if (XMLHttpReq.status == 200) { // 信息已经成功返回
			//DisplayHot();
			alert(res);

        } else { //页面不正常
            alert("密码保存失败，请重新操作，谢谢！");

        }
    }
}

function saveEmpInfo(){

	if(!dataValidate())
		return false;

	var form = document.empFrm;
	var pars = '';
	var url = 'orgMaint.do';
	var req = new Ajax.Request(url,{
		method:'post',
		parameters:$(form).serialize(),
		onSuccess : dispatchRequest,
		onFailure : tipMessage
	});		
}

function dispatchRequest(req){
	alert(req.responseText || '0');
	parent.location.reload();
	location.reload(true);
}

function tipMessage(req){
	alert(req.responseText || '');
}

function dataValidate(){
	
	if(MM_findObj("firstName").value == ""){
		alert("姓：必录项！");
		MM_findObj("firstName").focus();
		return false;
	}else if(MM_findObj("lastName").value == ""){
		alert("名：必录项！");
		MM_findObj("lastName").focus();
		return false;
	}else if(MM_findObj("loginName").value == ""){
		alert("登录名：必录项！");
		MM_findObj("loginName").focus();
		return false;
	}else if(!checkNumber(MM_findObj("IDCard").value)){
		alert("身份证号码必须是数字！");
		idCard.focus();
		return false;
	}
	
	
	return true;
}
</script>