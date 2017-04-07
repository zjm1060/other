<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<!DOCTYPE HTML>
<%
ContractObj con = (ContractObj)request.getAttribute(WebKeys.Request_Attribute_Name_ContractObject);
EmployeeObj emp = (EmployeeObj)request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

%>

<html>
  <head>
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<script type="text/javascript" src="/xMIS/resources/js/calendarPopup.js"></script>

<script>
var calx = new CalendarPopup("calendarDiv");

</script>

  </head>
  <body>
  <form action="contractMain.do" method="post" name="contractFrm">
  	<input type=hidden name=method value=saveContractInfo />
    <TABLE class=grid id=contract cellSpacing=1 cellPadding=1 border=0 width=800px>
	  <tr class=even>
	    <TD class=info width=60px noWrap ><font color=red>*</font>合同类型</TD>
	    <td noWrap >
	    <input type=hidden name=contractTypeID value=<%=con.getContractTypeID() %>>
	    <% String disabled = con.getContractTypeID()==0?"": "disabled=disabled"; %>
	    
	    &nbsp;<select style="BACKGROUND-COLOR: #ffffcc" name="contractType" id="contractType" onchange=enabledParentBtn(this) <%=disabled %>  >
	    	<option name="abc" value="0">---</option>
	    <%
				int mapsize = con.getContractTypeMap().size();
				Iterator it = con.getContractTypeMap().entrySet().iterator();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					String selected = ((Integer)entry.getKey()).intValue() == con.getContractTypeID()?"selected":"";
	     %>
	     	<option name="<%=String.valueOf(entry.getValue()).trim()%>" value="<%=String.valueOf(entry.getKey())%>" <%=selected %>><%=String.valueOf(entry.getValue())%></option>
	     <%} %>
	    </td>
	    <TD class=info width=60px noWrap ><font color=red>*</font>合同来源</TD>
	    <td colSpan=2>
	    &nbsp;<select style="BACKGROUND-COLOR: #ffffcc" name="contractSourceType" id="contractSourceType" onchange=enabledSourceType(this)>
	    	<option value="0">---</option>
	    <%
				mapsize = con.getContractSourceTypeMap().size();
				it = con.getContractSourceTypeMap().entrySet().iterator();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					String selected = ((Integer)entry.getKey()).intValue() == con.getSourceTypeID()?"selected":"";
	     %>
	     	<option value="<%=String.valueOf(entry.getKey())%>" <%=selected %>><%=String.valueOf(entry.getValue())%></option>
	     <%} %>
	    	</select>	    
	    </td>
	    
	    <%String sourceTypeDisabled = con.getGovermentContractSourceID()>0?"":"disabled=true"; %>
	    <TD class=info noWrap><input type=button class=blue_button_40_20 name=sourceTypeBtn value=来源信息 <%=sourceTypeDisabled %>>
	    <input type=hidden name=gSourceTypeID id=gSourceTypeID value=<%=con.getGovermentContractSourceID() %>>
	    </TD>
	    <td colSpan=2>
	    &nbsp;<SPAN id=GSourceTypeName><%=con.getGovermentContractSourceName()==null?"":con.getGovermentContractSourceName() %></SPAN>
	    </td>
	  </tr>
	  <tr class=even>
	    <TD class=info width=60px noWrap ><font color=red>*</font>合同编号</TD>
	    <td noWrap >
	    	<input type=hidden name=contractID id=contractID value=<%=con.getContractID() %>>
			&nbsp;<INPUT class=IDCard name=contractCode value="<%=con.getContractNo()==null?"":con.getContractNo() %>" >
	    </td>
	    <TD class=info width=60px noWrap >
	    <input type=button class=blue_button_40_20 name=educationBtn value=项目编号 >
	    </TD>
	    <td colSpan=2>
	    <input type=hidden name=projID value=<%=con.getProjectOBJ().getProjectID() %>>
	    &nbsp;<a href="#"><%=con.getProjectOBJ().getProjectCode()==null?"":con.getProjectOBJ().getProjectCode() %></a>
	    </td>
	    <TD class=info width=60px noWrap>&nbsp;项目名称
	    </TD>
	    <td colSpan=2>&nbsp;<a href="#"><%=con.getProjectOBJ().getProjectName()==null?"":con.getProjectOBJ().getProjectName() %></a>
	    </td>
	  </tr>	  
	  <tr class=even>
	    <TD class=info width=60px noWrap rowSpan=2><font color=red>*</font>合同名称</TD>
	    <td colSpan=4 noWrap rowSpan=2>
	    	&nbsp;<TEXTAREA name="contractName" class="textarea_contract" cols="" rows="2" ><%=con.getContractName()==null?"":con.getContractName() %></textarea>
	    	
	    </td>
	    <TD class=info width=60px noWrap ><font color=red>*</font>合同简称</TD>
	    <td noWrap colSpan=2 >
			&nbsp;<INPUT class=text_long name=abb value="<%=con.getAbbreviation()==null?"":con.getAbbreviation() %>" >	    
	    </td>
	  </tr>
	  <tr class=even>
	  <% String parentDisabled = con.getContractTypeID() == 0?"disabled=true":""; %>
	    <td rowSpan=3 noWrap>&nbsp;<input type=button class=blue_button_40_20 name=parentContractBtn value=父合同 <%=parentDisabled %>></td>
	    <td colSpan=2 >
	    	<input type=hidden id=parentID name=parentID value=<%=con.getParentID() %>>
			&nbsp;<SPAN id=parentNo>编号：<%=con.getParentCode()==null?"":con.getParentCode() %> </SPAN>	    
	    </td>
	  </tr>
	  <tr class=even>
	    <TD width=60px class=info rowSpan=2>&nbsp;合同摘要</TD>
	    <td colSpan=4 rowSpan=2>
	    	&nbsp;<TEXTAREA name="summary" class="textarea_contract" cols="" rows="2" ><%=con.getContractSummary()==null?"":con.getContractSummary() %></textarea>
	    </td>
	    <td colSpan=2 >&nbsp;<SPAN id=parentName>名称：<%=con.getParentName()==null?"":con.getParentName() %></SPAN></td>
	  </tr>
	  <tr class=even>
	    <td colSpan=2 >&nbsp;<SPAN id=parentAbb>简称：<%=con.getParentAbb()==null?"":con.getParentAbb() %></SPAN></td>
	  </tr>
	  <tr class=even>
	    <TD width=60px class=info rowSpan=3>质保条款<br>定 义</TD>
	    <td colSpan=4 rowSpan=3>
	    	&nbsp;<TEXTAREA name="warrantyDefinition" class="textarea_contract" cols="" rows="3" ><%=con.getWarrantyDefinition()==null?"":con.getWarrantyDefinition() %></textarea>
	    </td>
	    <TD width=60px class=info noWrap><font color=red>*</font>质保期限</TD>
	    <td noWrap>&nbsp;<input class=integer type=text name=warrantyLimit value="<%=(int)con.getWarrantyLimit() %>" size=6 onkeypress=doKey('nubmer') onchange = limitCalculate("warrantyStartDate","warrantyShow","warrantyLimit","warrantyUnit","warrantyEndDate")>
	    	&nbsp;<SELECT style="BACKGROUND-COLOR: #ffffcc" name="warrantyUnit" onchange=limitCalculate("warrantyStartDate","warrantyShow","warrantyLimit","warrantyUnit","warrantyEndDate") >
	     	<%
	     	String daySelected = "", monthSelected = "", yearSelected ="";
	     	switch(con.getWarrantyUnit()){
	     		case WebKeys.Contract_Obj_Warranty_Unit_Def_DAY:
	     			daySelected = "selected";
	     			break;
	     		case WebKeys.Contract_Obj_Warranty_Unit_Def_MONTH:
	     			monthSelected = "selected";
	     			break;
	     		case WebKeys.Contract_Obj_Warranty_Unit_Def_YEAR:
	     			yearSelected = "selected";
	     			break;
	     		default:
	     			daySelected = "selected";
	     			break;
	     	}
	     	
	     	%>
	     	<option name="日" value="<%=WebKeys.Contract_Obj_Warranty_Unit_Def_DAY %>" <%=daySelected %>>日</option>
	     	<option name="月" value="<%=WebKeys.Contract_Obj_Warranty_Unit_Def_MONTH %>" <%=monthSelected %>>月</option>
	     	<option name="年" value="<%=WebKeys.Contract_Obj_Warranty_Unit_Def_YEAR %>" <%=yearSelected %>>年</option>
	    	</SELECT>	
	    </td>
	    <td rowSpan=6 width=140px valign=top><div id=attachedFileDiv width=140px></div><br>
		    
	    <br></td>
	  </tr>
	  <tr class=even>
	  	    <TD width=60px class=info noWrap><font color=red>*</font>质保期起</TD>
	    <td noWrap>
	    	&nbsp;<INPUT class=date name=warrantyStartDate size=11 readonly value="<%=con.getWarrantyStartDate()==null?"":con.getWarrantyStartDate().toString().trim().substring(0,10) %>" onChange="checkDateTime(this)" onkeypress=doKey('nubmer') onpropertychange=limitCalculate('warrantyStartDate','warrantyShow','warrantyLimit','warrantyUnit','warrantyEndDate')>
			<a id=anchor1x onclick="calx.select(MM_findObj('warrantyStartDate'),'anchor1x','yyyy-MM-dd'); return false;" href="#">
		    	<IMG height=13 alt=calendar src="/xMIS/resources/images/calendar_2.gif" width=13 align=middle border=0 ></IMG></a>
	    </td>
	  </tr>
	  <tr class=even>
	    <TD width=60px class=info noWrap height=20>&nbsp;&nbsp;质保期止</TD>
	    <td noWrap>
	    	<INPUT name=warrantyEndDate type=hidden value=""/>
	    	&nbsp;<SPAN id=warrantyShow><%=con.getWarrantyEndDate()==null?"":con.getWarrantyEndDate() %> </SPAN>	    
	    </td>
	  </tr>
	  <tr class=even>
	    <TD class=info width=60px noWrap><font color=red>*</font>合同金额</TD>
	    <td noWrap colSpan=4>
	    	<input type=hidden name=contractAmt value="<%=con.getContractAmt()>0?con.getContractAmt():0 %>"> 
			&nbsp;<INPUT size=50 class=currency id=amt name=amt value="<%=con.getContractAmt() %>" onkeypress=currencyCheck(this) onblur=formatCurrencyOnPage(this,1) onFocus=restoreAmt(this) >	    
	    &nbsp;<select style="BACKGROUND-COLOR: #ffffcc" name="currencyType" id="currencyType" onchange=formatCurrencyOnPage(MM_findObj("amt"),2)>
	    <%
				mapsize = con.getCurrencyTypeMap().size();
				it = con.getCurrencyTypeMap().entrySet().iterator();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					String selected = ((Integer)entry.getKey()).intValue() == con.getCurrencyID()?"selected":"";
	     %>
	     	<option name=<%=con.getCurrencyCodeMap().get(entry.getKey()) %> value="<%=String.valueOf(entry.getKey())%>" <%=selected %>><%=String.valueOf(entry.getValue())%></option>
	     <%} %>
	    	</select>	
			
	    </td>
	    <TD width=60px class=info noWrap><font color=red>*</font>签订日期</TD>
	    <td noWrap>
	    	&nbsp;<INPUT class=date name=signatureDate size=11 readonly value="<%=con.getSignatureDate()==null?"":con.getSignatureDate().toString().trim().substring(0,10) %>" onChange="checkDateTime(this)" onkeypress=doKey('nubmer')>
			<a id=anchor3x onclick="calx.select(MM_findObj('signatureDate'),'anchor3x','yyyy-MM-dd'); return false;" href="#">
		    	<IMG height=13 alt=calendar src="/xMIS/resources/images/calendar_2.gif" width=13 align=middle border=0 ></IMG></a>
	    </td>
	  </tr>	  	
	  <tr class=even>
	    <TD class=info width=60px noWrap >
	    <%String companyDisabled = con.getBuyerID()>0?"disabled=true":""; %>
	    <input type=button class=blue_button_40_20 name=aCompanyBtn value=甲方公司 <%=companyDisabled %>>
	    </TD>
	    <td noWrap colSpan=2>
	    	<input type=hidden id=aCompany_id name=aCompany_id value=<%=con.getBuyerID() %>>
			&nbsp;<INPUT class=text_readonly_long id=aCompany_name value="<%=con.getBuyer()==null? "":con.getBuyer() %>" readonly>	    
	    </td>
	    <TD class=info width=60px noWrap >
	    <input type=button class=blue_button_40_20 name=aDeptBtn value=甲方部门 >
	    </TD>
	    <td>
	    	<input type=hidden id=aDept_id name=aDept_id value=<%=con.getDeptIDOfBuyer() %>>
			&nbsp;<INPUT class=text_readonly_normal id=aDept_name value="<%=con.getDeptNameOfBuyer()==null?"":con.getDeptNameOfBuyer() %>"  size=14 readonly>	    
	    </td>
	    <TD class=info width=60px noWrap >
	    <input type=button class=blue_button_40_20 name=aEmpBtn value=甲方代表 >
	    </TD>
	    <td noWrap>
	    	<input type=hidden name=aEmp_id value=<%=con.getBuyerRepresentativeID() %>>
			&nbsp;<INPUT class=text_readonly_short size=14 name=aEmp_name value="<%=con.getBuyerRepresentativeName()==null?"":con.getBuyerRepresentativeName() %>" readonly>	    
	    </td>
	  </tr>	  
	  <tr class=even>
	    <TD class=info width=60px noWrap >
	    <%companyDisabled = con.getVendorID()>0?"disabled=true":""; %>
	    <input type=button class=blue_button_40_20 name=bCompanyBtn value=乙方公司 <%=companyDisabled %>>
	    </TD>
	    <td noWrap colSpan=2>
	    	<input type=hidden id=bCompany_id name=bCompany_id value=<%=con.getVendorID() %>>
			&nbsp;<INPUT class=text_readonly_long id=bCompany_name value="<%=con.getVendorName()==null?"":con.getVendorName() %>" readonly>	    
	    </td>
	    <TD class=info width=60px noWrap >
	    <input type=button class=blue_button_40_20 name=bDeptBtn value=乙方部门 >
	    </TD>
	    <td>
	    	<input type=hidden id=bDept_id name=bDept_id value=<%=con.getDeptIDOfVendor() %>>
			&nbsp;<INPUT class=text_readonly_normal size=14 id=bDept_name value="<%=con.getDeptNameOfVendor()==null?"":con.getDeptNameOfVendor() %>" readonly>	    
	    </td>
	    <TD class=info width=60px noWrap >
	    <input type=button class=blue_button_40_20 name=bEmpBtn value=乙方代表 >
	    </TD>
	    <td noWrap>
	    	<input type=hidden name=bEmp_id value=<%=con.getVendorRepresentativeID() %>>
			&nbsp;<INPUT class=text_readonly_short size=14 name=bEmp_name value="<%=con.getVendorRepresentativeName()==null?"":con.getVendorRepresentativeName() %>" readonly>	    
	    </td>
	  </tr>	  
	  <tr class=even>
	    <TD width=60px class=info rowSpan=3>备注</TD>
	    <td colSpan=4 rowSpan=3>
	    	&nbsp;<TEXTAREA name="remark" class="textarea_contract" cols="" rows="3" ><%=con.getRemark()==null?"":con.getRemark() %></textarea>
	    </td>
	    <TD class=info width=60px noWrap >
	    <input type=button class=blue_button_40_20 name=cEmpBtn value=收货人 >
	    </TD>
	    <td noWrap>
	    	<input type=hidden name=delivery_id value=<%=con.getDeliveryID() %>>
			&nbsp;<INPUT class=text_readonly_short size=14 name=cEmp_name value="<%=con.getDeliveryName()==null?"":con.getDeliveryName() %>" readonly>	    
	    </td>
	    <td rowSpan=3 align=center>
	    	<table>
	    		<tr><td style="line-height:300px;"><input type=button class=blue_button name="uploadAttachment" value="上传附件" /></td></tr>
	    		<tr><td style="line-height:300px;"><input type=button class=blue_button value="信息保存" onclick="saveContractInfo()" /></td></tr>
	    	</table>
	    </td>
	  </tr>
	  <tr class=even>
	    <TD width=60px class=info noWrap height=20>登记日期</TD>
	    <td noWrap>&nbsp;<SPAN id=entryDate><%=con.getUpdateDate()==null?"":con.getUpdateDate().toString().trim().substring(0,10) %></SPAN></td>
	  </tr>	    
	  <tr class=even>
	    <TD width=60px class=info noWrap height=20>登记人</TD>
	    <td noWrap>&nbsp;<SPAN id=entryName><%=con.getRegisterName()==null?"":con.getRegisterName() %></SPAN></td>
	  </tr>	    
	  
	  <%if(con.getContractAmt()!=0 || con.getEarnedTotalAmt()!=0 || con.getArrearageTotalAmt()!=0){ %>
	  <tr class=even><td colSpan=8 noWrap height=30><SPAN id=totalAmtMsg></SPAN></td></tr>
	  <%} %>
	 </table>
	  
	
	</form>
	
	<form action="contractMain.do" method="post" name="contractFrmA">
  		<input type=hidden name=method value=contract />
  		<input type=hidden name=contractID />
	</form>
<br>
  <table width=800>
  	<tr><td>
		<%if(con.getContractID() > 0 && con.getContractTypeID()== 1 && con.getContractAmt()>0){ %>
			<jsp:include page="<%=WebKeys.Include_JSP_File_paymentCondition %>" />
			<script language="javascript">
				setContractIdInPy( document.contractFrm.all.contractID.value);
				setContractAmtInPay(MM_findObj("contractAmt").value);
				var paymentCondiFlag = 0;
			</script>
		<%} %>
		<%if(con.isHadChild()){ %>
			<jsp:include page="<%=WebKeys.Include_JSP_File_ChildContractList %>" />
		<%} %>
	<jsp:include page="<%=WebKeys.Include_JSP_File_paymentList %>" />
	<jsp:include page="<%=WebKeys.Include_JSP_File_InvoiceList %>" />
	
	<!-- jsp:param name="contractID4PaymentList" value="" /-->
	<!--/jsp:include-->
	</td></tr>
  </table>
	
	
  </body>
</html>





<script language="javascript">
var amtReadOnly = false;

var Upload_Application_Contract_Type = "<%=WebKeys.Upload_Application_Contract_Type %>";
var Upload_Application_Download_Type = "<%=WebKeys.Upload_Application_Download_Type %>";

<%if(con.getContractAmt()!=0 || con.getEarnedTotalAmt()!=0 || con.getArrearageTotalAmt()!=0){ 
	if(con.getContractTypeID()==1){
	
		//String linkStr = con.getEarnedTotalAmt()==0?"":"<a id=paymentListLink href=#>";
		//String linkPostFix = con.getEarnedTotalAmt()==0?"":"</a>";
%>

MM_findObj("totalAmtMsg").innerHTML = "&nbsp;&nbsp;已收合同金额：" 
	+ <%=con.getEarnedTotalAmt()==0?"\"\"":"\"<a href=# id=paymentListLink_" + con.getContractID() + " onClick=showPaymentList('" + con.getAbbreviation() + "',"+ con.getContractID()+") >\"" %>
	+ formatCurrency(MM_findObj("currencyType").options[MM_findObj("currencyType").selectedIndex].name, '<%=con.getEarnedTotalAmt() %>', Color_Font_Contract_Total_Charged_Amt) 
	+ <%=con.getEarnedTotalAmt()==0?"\"\"":"\"</a>\"" %>
	+ "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;未收合同金额："
	+ formatCurrency(MM_findObj("currencyType").options[MM_findObj("currencyType").selectedIndex].name, '<%=con.getArrearageTotalAmt() %>', 'red')
	+ "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;已开发票金额："
	//+ formatCurrency(MM_findObj("currencyType").options[MM_findObj("currencyType").selectedIndex].name, '<%=con.getInvoicedTotalAmt() %>', Color_Font_Contract_Total_Invoiced_Amt)

	+ <%=con.getInvoicedTotalAmt()==0?"\"\"":"\"<a href=# id=invoiceListLink_" + con.getContractID() + " onClick=showInvoiceList('" + con.getAbbreviation() + "',"+ con.getContractID()+") >\"" %>
	+ formatCurrency(MM_findObj("currencyType").options[MM_findObj("currencyType").selectedIndex].name, '<%=con.getInvoicedTotalAmt() %>', Color_Font_Contract_Total_Invoiced_Amt) 
	+ <%=con.getInvoicedTotalAmt()==0?"\"\"":"\"</a>\"" %>

	+ "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;已开发票未到账金额："
	+ formatCurrency(MM_findObj("currencyType").options[MM_findObj("currencyType").selectedIndex].name, '<%=con.getUnCreditedTotalAmt() %>', Color_Font_Contract_Total_UnAccount_Amt)

	;
<%	}
}%>

function currencyCheck(obj){


	
	if(event.keyCode == 46 && obj.value.indexOf(".")>0){
		event.keyCode = 0;
		event.returnvalue = false;
		return  false;
	}else{
		doKey('float');
	}		
}

formatCurrencyOnPage(MM_findObj("amt"), 1);

function formatCurrencyOnPage(obj, type){
	initCurrencyField(obj);
	
	var currenyFlag = MM_findObj("currencyType").options[MM_findObj("currencyType").selectedIndex].name ;
	
	if(MM_findObj("currencyType").options[MM_findObj("currencyType").selectedIndex].text.trim() == "欧元")
		currenyFlag = "€";

	if(type == 2)
		restoreAmt(obj);
	else
		MM_findObj("contractAmt").value = obj.value;

	obj.value = currenyFlag+formatNumber(obj.value,2);
}


function restoreAmt(obj){
	if(amtReadOnly) {
		alert("付款条件已定义，合同金额无法修改。如需修改请联系系统管理员，谢谢！");

	}
	
	obj.value = parseFloat(MM_findObj("contractAmt").value );
}

function enabledSourceType(obj){

	var sourceType = MM_findObj("sourceTypeBtn");
	
	if(obj[obj.value].text == "横向合同")
		sourceType.disabled = false;
	else{
		sourceType.disabled = true;
		document.all.gSourceTypeID.value = 0;
		document.all.GSourceTypeName.innerHTML = "";
	}
}

function enabledParentBtn(obj){

		MM_findObj("contractTypeID").value = obj.value;

        document.getElementById('parentID').value = "";
        document.getElementById('parentNo').innerHTML = "编号：";
        document.getElementById('parentName').innerHTML = "名称：";
        document.getElementById('parentAbb').innerHTML = "简称：";

	var parentBtn = MM_findObj("parentContractBtn");
	
	if(obj.value > 0)
		parentBtn.disabled = false;
	else{
		parentBtn.disabled = true;
	}
		
	var name = obj.options[obj.selectedIndex].name;

	document.getElementById("aCompany_id").value = "";
	document.getElementById("aCompany_name").value = "";
	document.getElementById("bCompany_id").value = "";
	document.getElementById("bCompany_name").value = "";

	document.getElementById("aDept_id").value = "";
	document.getElementById("aDept_name").value = "";
	document.getElementById("bDept_id").value = "";
	document.getElementById("bDept_name").value = "";
	
	document.getElementById("aEmp_id").value = "";
	document.getElementById("aEmp_name").value = "";
	document.getElementById("bEmp_id").value = "";
	document.getElementById("bEmp_name").value = "";
	document.getElementById("delivery_id").value = "";
	document.getElementById("cEmp_name").value = "";
		

//	MM_findObj("bCompanyBtn").disabled = true;
//	MM_findObj("aCompanyBtn").disabled = true;
		
	if(name == "收入合同"){
		document.getElementById("bCompany_id").value = "<%=emp.getCompanyID() %>";
		document.getElementById("bCompany_name").value = "<%=emp.getCompanyName() %>";
//		MM_findObj("aCompanyBtn").disabled = false;

	}else if(name == "支出合同"){
		document.getElementById("aCompany_id").value = "<%=emp.getCompanyID() %>";
		document.getElementById("aCompany_name").value = "<%=emp.getCompanyName() %>";
//		MM_findObj("bCompanyBtn").disabled = false;
	}		

}

function saveContractInfo(){

	if(!dataValidate())
		return false;

	var date = new Date();
	MM_findObj("entryDate").innerHTML = date.getYear() +'-' + ((date.getMonth()+1)<10?'0'+(date.getMonth()+1):(date.getMonth()+1)) + '-' +(date.getDate()<10?'0'+date.getDate():date.getDate());
	MM_findObj("entryName").innerHTML = "<%=emp.getUserName() %>";
	
	var form = document.contractFrm;
	var pars = '';
	var url = 'contractMain.do';
	var req = new Ajax.Request(url,{
		method:'post',
		parameters:$(form).serialize(),
		onSuccess : refreshPage,
		onFailure : tipMessage
	});		
	
}

function refreshPage(req){

//	alert(req.responseText.substring(0,req.responseText.indexOf("^")) || '0');
	document.contractFrmA.all.contractID.value = req.responseText.substring(req.responseText.indexOf("^")+1);
	document.contractFrmA.submit();		
}
function dataValidate(){

	if(MM_findObj("contractTypeID").value==0){
		alert("请选择合同类型！");
		MM_findObj("contractType").focus();
		return false;
	}else if(MM_findObj("contractSourceType").value==0){
		alert("请选择合同来源！");
		MM_findObj("contractSourceType").focus();
		return false;
	}else if(MM_findObj("contractSourceType")[MM_findObj("contractSourceType").value].text.trim() == "横向合同" 
		&& MM_findObj("gSourceTypeID").value == 0){
		alert("请选择横向合同来源！");
		//MM_findObj("sourceTypeBtn").focus();
		return false;
	}else if(MM_findObj("contractCode").value.trim() == ""){
		alert("合同编号：必录项！");
		MM_findObj("contractCode").focus();
		return false;
	}else if(MM_findObj("contractName").value.trim() == ""){
		alert("合同名称：必录项！");
		MM_findObj("contractName").focus();
		return false;
	}else if(MM_findObj("abb").value.trim() == ""){
		alert("合同简称：必录项！");
		MM_findObj("abb").focus();
		return false;
	}else if(parseFloat(MM_findObj("warrantyLimit").value.trim()) == 0){
		alert("质保期限：必录项！");
		MM_findObj("warrantyLimit").focus();
		return false;
	}else if(!checkNumber(parseFloat(MM_findObj("warrantyLimit").value.trim()))){
		alert("质保期限必须是数字！");
		MM_findObj("warrantyLimit").focus();
		return false;
	}else if(MM_findObj("warrantyStartDate").value.trim() == ""){
		alert("质保期起：必录项！");
		MM_findObj("warrantyStartDate").focus();
		return false;
	}else if(MM_findObj("signatureDate").value.trim() == ""){
		alert("签订日期：必录项！");
		MM_findObj("signatureDate").focus();
		return false; 
	}else if(parseInt(MM_findObj("contractAmt").value.trim()) == 0){
		alert("合同金额: 必录项！");
		MM_findObj("amt").focus();
		return false;
	}else if(f_check_float(MM_findObj("contractAmt").value.trim())){
		alert("合同金额必须是数字！");
		MM_findObj("contractAmt").focus();
		return false;
	}else if(MM_findObj("aCompany_id").value.trim() == ""){
		alert("甲方公司:必录项");
		return false;
	}else if(MM_findObj("bCompany_id").value.trim() == ""){
		alert("乙方公司：必录项");
		return false;
	}
	
	
	return true;
}
/*
var buttonName, companyID;
function setParams(obj){
	
	buttonName = obj.name;
	if(obj.name == "aEmpBtn" || obj.name == "aDeptBtn")
		companyID = document.getElementById("aCompany_id").value;
	else if(obj.name == "bEmpBtn" || obj.name == "bDeptBtn")
		companyID = document.getElementById("bCompany_id").value;
	else
		companyID = "";
		
}
*/



Ext.onReady(function(){
    Ext.QuickTips.init();
	var stFm = Ext.form;
	var newFlag = 0;
	var deptStoreFlag = 0;
	
	function nameLinker(val){    
		var text = val.substring(0, val.indexOf("^"));
		var url = val.substring(val.indexOf("^")+1, val.length);
		if(text != '0'){
			return '<a href="'+url+'">'+text+'</a>'   
		}
	　　else return text;    
　　}    	
	
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
	
    /*====================================================================
     * GSourceType 
     *====================================================================*/
     
    var st = Ext.data.Record.create([
    {	name: 'st_id',
    	type: 'int'
    },{
        name: 'st_name',
        type: 'string'
    }, {
        name: 'st_company_id',
        type: 'int'
    }, {
        name: 'st_company_name',
        type: 'string'
       
    }, {
        name: 'st_dept_id',
        type: 'int'
    }, {
        name: 'st_dept_name',
        type: 'string'
    },{
        name: 'st_amt',
        type: 'float'
    },{
        name: 'st_currency_id',
        type: 'int'
    },{
        name: 'st_currency_name',
        type: 'string'
    },{
        name: 'st_remark',
        type: 'string'
    },{
        name: 'st_selected',
        type: 'bool'
    }]);
    
	var stStore = new Ext.data.GroupingStore({
		url:'contractMain.do?method=getGovernmentContractSourceList',
		remoteSort:false,
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount',
            fields:st
		}),
		sortInfo:{field: 'st_id', direction: 'ASC'}
		
	}); 
	
		
    var stEditor = new Ext.ux.grid.RowEditor({
        saveText: '确认',
        cancelText: '取消',
        clicksToEdit: 2 //点击鼠标几次触发更新
    });

	stEditor.on({ 
	  scope: this, 
		
	  beforeedit: function(){

	  	stGrid.removeBtn.setDisabled(true);
	  	stGrid.newBtn.setDisabled(true);
	  	var compId =stGrid.getSelectionModel().getSelected().get('st_company_id');
	  	if(compId != '' && compId != undefined){
	  		departmentStroe.removeAll();  
			departmentStroe.load({
				params:{
					compID:compId
				},			
				callback:function(record, options ,success){
					if(success == true){
					}else{
						Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
					}
				}
			}); 
  		
  		}
	  	
	  },
	  		
	  afteredit: function(roweditor, changes, record, rowIndex) { 
	  	
	  	if(isNaN(edCompanyCombo.getValue())){
	  		edCompanyCombo.setValue(stGrid.getSelectionModel().getSelected().get('st_company_id'));
	  		}
	  	if(isNaN(edCurrencyCombo.getValue())){
	  		edCurrencyCombo.setValue(stGrid.getSelectionModel().getSelected().get('st_currency_id'));
	  	}
	  	if(isNaN(edDeptCombo.getValue())){
	  		edDeptCombo.setValue(stGrid.getSelectionModel().getSelected().get('st_dept_id'));
	  	}	
	  		
		stForm.getForm().submit({
	         url : 'contractMain.do?method=saveGovernmentContractSource',
	         method : 'post',
			 params : {
			 	contractID: '<%=con.getContractID() %>',
			 	stID: record.get('st_id'),
			 	stName: record.get('st_name'),
			 	companyID: edCompanyCombo.getValue(),
			 	deptID: edDeptCombo.getValue(),
			 	amt: record.get('st_amt'),
			 	currencyType: edCurrencyCombo.getValue(),
			 	remark: record.get('st_remark'),
			 	selected: record.get('st_selected')
			 }	,
	         success : function(form, action) {
	         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
				
				if(record.get('st_selected') == true){
					document.all.gSourceTypeID.value = action.result.stID;
					document.all.GSourceTypeName.innerHTML = action.result.stName;
					stEditor.stopEditing();
		         	//stWin.close();
		         	stWin.hide();
				}else{
					if(document.all.gSourceTypeID.value == action.result.stID){
						document.all.gSourceTypeID.value = 0;
						document.all.GSourceTypeName.innerHTML = "";
					}
					stStore.load({
						params:{
							selectedID: MM_findObj("gSourceTypeID").value
						}
					});				
				}
				newFlag = 0;
         	
	         },
         	failure : function(form, action) {
	         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
         	}
		});
		
		stGrid.removeBtn.setDisabled(false);
	  	stGrid.newBtn.setDisabled(false);
		
	  },
	  canceledit: function() {
	  	if(newFlag == 1) {
               // stEditor.stopEditing();
                var s = stGrid.getSelectionModel().getSelections();
                for(var i = 0, r; r = s[i]; i++){
                    stStore.remove(r);
                };
				stGrid.getView().refresh();
				newFlag = 0;
	  	}
	  	stGrid.removeBtn.setDisabled(false);
	  	stGrid.newBtn.setDisabled(false);
	  	
	  }
	});

    var stCheckbox = new Ext.grid.CheckColumn({
            header: 'Active',
            dataIndex: 'st_selected',
            align: 'center',
            width: 50,

            editor: {
                xtype: 'checkbox'
            }
    });    
    
    var companyStroe = new Ext.data.JsonStore({
    		data:<%=con.getCompanyDataStore() %>,
    		fields: [
    			{ name: 'compID', type: 'int' },
    			{ name: 'compAbb', type: 'string' },
    			{ name: 'compName', type: 'string' }
          	]  	
	});
	
	var edCompanyCombo = new Ext.form.ComboBox({
      		id: 'stEdCompany',
      		dataIndex: 'st_company_id',
      		store: companyStroe,
			valueField: 'compID',
			displayField: 'compAbb',

	        mode: 'local',
       		editable: false,//false则不可编辑，默认为true
	        forceSelection: false,//true to restrict the selected value to one of the values in the list, false to allow the user to set arbitrary text into the field 
	        selectOnFocus:true,//true to select any existing text in the field immediately on focus. Only applies when editable = true 

            typeAhead: true,
            triggerAction: 'all',
            lazyRender: true,
            listClass: 'x-combo-list-small',
            listeners :{
            	'change':function(){
            	var comId =this.getValue();
	            	departmentStroe.removeAll();  
						departmentStroe.load({
							params:{
								compID:comId
							},			
							callback:function(record, options ,success){
								if(success == true){
								}else{
									Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
								}
							}
						}); 
            	},
            	'select':function(){
            	edDeptCombo.setValue('');
            	}
            }
            
            
    });
    var departmentStroe = new Ext.data.Store({
    		proxy: new Ext.data.HttpProxy({
			url:'contractMain.do?method=getDeptListStroeByCompany' 
		}),
		remoteSort:true,
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount',
			remoteSort:true,
            fields:[
                 {name:'parentName'},
                 {name:'deptID'},
                 {name:'deptName'}
           ]
		})
	});
	
	edDeptCombo =new Ext.form.ComboBox({
      		id: 'stEdDept',
      		dataIndex: 'st_dept_name',//?
      		store: departmentStroe,
			valueField: 'deptID',
			displayField: 'deptName',

	        mode: 'local',
       		editable: false ,//false则不可编辑，默认为true
	        forceSelection: true,
	       selectOnFocus:true,

            typeAhead: true,
            triggerAction: 'all',
            lazyRender: true,
            listClass: 'x-combo-list-small'
     });   
    
    var currencyStroe = new Ext.data.JsonStore({
    		data:<%=con.getCurrencyDataStore() %>,
    		fields: [
    			{ name: 'currencyID', type: 'int' },
    			{ name: 'currencyName', type: 'string' }
          	]  	
	});
	var edCurrencyCombo = new Ext.form.ComboBox({
      		id: 'stEdCurrency',
      		dataIndex: 'st_currency_name',//?
      		store: currencyStroe,
			valueField: 'currencyID',
			displayField: 'currencyName',

	        mode: 'local',
       		editable: false, //false则不可编辑，默认为true
	        forceSelection: true,
	        selectOnFocus:true,

            typeAhead: true,
            triggerAction: 'all',
            lazyRender: true,
            listClass: 'x-combo-list-small'
     });   
     
         
    				
    var stGrid = new Ext.grid.GridPanel({
        store: stStore,
        //width: 760,
        height:350,
        region:'center',
        stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
        margins: '0 5 5 5',
        autoExpandColumn: 'st_name',
        plugins: [stEditor],
        view: new Ext.grid.GroupingView({
            markDirty: false
        }),
        tbar: [{
        	ref: '../newBtn',
            iconCls: 'add',
            text: '新建',
            handler: function(){
                var e = new st({
                    st_name: '',
                    st_company_name: '',
                    st_dept_name: '',
                    st_amt: '',
                    st_currencyType: '',
                    st_remark: '',
                    st_selected: true
                });
                stEditor.stopEditing();
                stStore.insert(0, e);
                stGrid.getView().refresh();
                stGrid.getSelectionModel().selectRow(0);
                stEditor.startEditing(0);
                newFlag = 1;
            }
        }, '-', {
            ref: '../removeBtn',
            iconCls: 'remove',
            text: '删除',
            disabled: true,
            handler: function(){
            	
            	var record = stGrid.getSelectionModel().getSelected();
            	
            	if(record.get('st_selected') == true){
            		Ext.Msg.alert("提示", record.get('st_name') + "已被选中，不能删除！");
            		return false;
            	}

				stForm.getForm().submit({
			         url : 'contractMain.do?method=delGovernmentContractSource',
			         method : 'post',
					 params : {
					 	stID: record.get('st_id'),
					 	stName: record.get('st_name')
					 }	,
			         success : function(form, action) {
			         	//alert(action.result.message);
			         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
				
		                stEditor.stopEditing();
		                
		            	var s = stGrid.getSelectionModel().getSelections();
		                for(var i = 0, r; r = s[i]; i++){
		                    stStore.remove(r);
		                };
						stGrid.getView().refresh();
						newFlag = 0;
			         },
		         	failure : function(form, action) {
			         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
		         	}
				});            
            
            }
        }],

        columns: [
        new Ext.grid.RowNumberer(),
        {
            header: '来源ID',
            dataIndex: 'st_id',
            hidden: true
           // sortable: true
        },{
            id: 'st_name',
            header: '来源名称',
            dataIndex: 'st_name',
            sortable: true,
            editor: {
                xtype: 'textfield',
                allowBlank: false
            }
        },{
            header: '单位ID',
            dataIndex: 'st_company_id',
            hidden: true
           // sortable: true
        },{
            header: '来源单位简称',
            dataIndex: 'st_company_name',
            sortable: true,
            editor: edCompanyCombo
        },{
            header: '部门ID',
            dataIndex: 'st_dept_id',
            hidden: true
           // sortable: true
        },{
            header: '来源部门简称',
            dataIndex:'st_dept_name',
            sortable: true,
            editor: edDeptCombo/*new stFm.ComboBox({
            		id: 'stDept',
                    typeAhead: true,
                    triggerAction: 'all',
                    //transform: 'dept',
                    lazyRender: true,
                    listClass: 'x-combo-list-small'
                })*/
        },{
            xtype: 'numbercolumn',
            header: '资助额度',
            dataIndex: 'st_amt',
           // format: '0,0.00',
            sortable: true,
            editor: {
                xtype: 'numberfield',
                minValue: 0
            }
        },{
            header: '币种',
            dataIndex: 'st_currency_name',
            sortable: true,
            editor: edCurrencyCombo
            	/*new stFm.ComboBox({
            		id: 'stCurreny',
                    typeAhead: true,
                    triggerAction: 'all',
                    //transform: 'currency',
                    lazyRender: true,
                    listClass: 'x-combo-list-small'
                })*/
        },{
            header: '特点',
            dataIndex: 'st_remark',
            sortable: false,
            editor: {
                xtype: 'textfield'
            }
        },
        stCheckbox
        ]
    });    
	
    stGrid.getSelectionModel().on('selectionchange', function(sm){
        stGrid.removeBtn.setDisabled(sm.getCount() < 1);
    });

	var stForm = new Ext.form.FormPanel({
	    baseCls : 'x-plain',
	    anchor : '100%',
		items:[stGrid]
    });
		          
    var stWin;
    var stBtn = Ext.get('sourceTypeBtn');
    stBtn.on('click', function(){
         if(!stWin){
			stWin=new Ext.Window({ 
				modal: true,
				title:"横向来源信息", 
				renderTo:Ext.getBody(), 
				frame:true, 
				plain:true, 
				resizable:false, 
				closeAction:"hide", 
				//maximizable:true, 
				closable:true, 
				bodyStyle:"padding:10px", 
				width:800, 
				height:400, 
				items:[stForm]
			}) 
        }
    	stWin.setPosition(240, 160);    
		stWin.show(); 
		stStore.load({
			params:{
				selectedID: MM_findObj("gSourceTypeID").value
			}
		}); 

	

    });  


    /*====================================================================
     * ParentContractList
     *====================================================================*/
     
     var searchCompField = new Ext.ux.form.SearchField({
     	id: 'searchCompField',
		store: pcStore,
		width: 155,
		
		onTrigger2Click : function(){//点击查询按钮或回车调用该方法   
			//peasonGrid.getView().expandAllGroups();
			pcStore.removeAll();
			//pcStore.proxy.setUrl('contractMain.do?method=getContractListForParentStroe');
			pcStore.setBaseParam("searchName", this.getRawValue());
			pcStore.setBaseParam("contractTypeID", document.getElementById('contractType').value);
			pcStore.load({
				params:{
					start:0, 
					limit:20
				},
 			callback:function(record, options ,success){
				if(success == true){
				}else{
					Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
				}
			}
			});  
		}   
    });
     
    var searchCompForm = new Ext.form.FormPanel({
	    baseCls : 'x-plain',
	    anchor : '100%',
	    width:600,
	    
		items:[{
			baseCls : 'x-plain',
			xtype: "panel",
			layout:  "column",
			fieldLabel: "合同简称/编号",
			lableWidth:300,
			isFormField:true,
			//listeners: { 'afterrender': function() {this.findByType('textfiled')[0].focus(true, true);  alert('showform!');} },
			items:	[			
					//{xtype:"textfield", inputType:'hidden', id:'searchChompID'},
					//{xtype:"textfield",width:120, inputType:'text', id:'searchStr'},
					searchCompField
					]
		}]					
		
    });
	var pcStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url:'contractMain.do?method=getContractListForParentStroe' 
		}),
		remoteSort:true,
		sortInfo:{field: '<%=DBSchema.Contract.Abbreviation %>', direction: 'ASC'},
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount',
			remoteSort:true,
            fields:[
                 {name:'<%=DBSchema.Contract.ContractID %>'},
                 {name:'<%=DBSchema.Contract.ContractNo %>'},
                 {name:'<%=DBSchema.Contract.ContractName %>'},
                 {name:'<%=DBSchema.Contract.Abbreviation %>'},
                 {name:'<%=DBSchema.Contract.GovermentContractSourceName %>'},
                 {name:'<%=DBSchema.Contract.ContractAmt %>'},
                 {name:'aCompany_abb'},
                 {name:'project_id'},
                 {name:'project_name'},
                 {name:'pageNo'}
           ]
		})
	});
	
    var pcWin;
	var pcSM= new Ext.grid.CheckboxSelectionModel({
		header: '',
		singleSelect:true
	 }); 

	var pcColM=new Ext.grid.ColumnModel(
		[
		pcSM,
		{header: "合同ID",dataIndex:"<%=DBSchema.Contract.ContractID %>", sortable:false, hidden:true},
		{header: "合同编号",dataIndex:"<%=DBSchema.Contract.ContractNo %>", sortable:true, renderer: biuldLinker},
		{header: "合同名称",dataIndex:"<%=DBSchema.Contract.ContractName %>", sortable:true, renderer: biuldLinker},
		{header: "合同简称",dataIndex:"<%=DBSchema.Contract.Abbreviation %>", sortable:true, renderer: biuldLinker},
		{header: "来源类型",dataIndex:"<%=DBSchema.Contract.GovermentContractSourceName %>", sortable:true, renderer: biuldLinker},
		{header: "合同金额",dataIndex:"<%=DBSchema.Contract.ContractAmt %>", sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data['<%=DBSchema.CurrencyType.CurrenyGraph %>'], value, '');
			}, 
			align: 'right'},
		{header: "甲方公司",dataIndex:"aCompany_abb", sortable:true, renderer: biuldLinker},
		{header: "项目名称",dataIndex:"project_name", sortable:true, renderer: biuldLinker}
		]
	);  
	
	var pcGrid = new Ext.grid.GridPanel({
		sm:pcSM,
		cm:pcColM,
		store:pcStore,
		disableSelection: false,
		autoExpandColumn:4,
		stripeRows: true,
		layout:'fit',
      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:true,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: false,  
		autoWidth: false,
		//height:Ext.get("pcWin").getHeight(),
		height: 520,
		width: '100%',
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
       	loadMask: {msg:'正在加载数据，请稍侯……'},

        tbar: [{
        	ref: '../okBtn',
            iconCls: 'ok',
            text: '确定',
            handler: function(){
            	var record = pcGrid.getSelectionModel().getSelected();
            	
            	if(document.contractFrm.all.contractID.value == record.get('<%=DBSchema.Contract.ContractID %>')){
					Ext.Msg.alert("<%=WebKeys.Ext_Message_Alert_Title %>","<%=WebKeys.Ext_Parent_Contract_Choose_Duplicate %>");
            	}else{
            	document.getElementById('parentID').value = record.get('<%=DBSchema.Contract.ContractID %>');
            	document.getElementById('parentNo').innerHTML = "编号："+record.get('<%=DBSchema.Contract.ContractNo %>');
            	document.getElementById('parentName').innerHTML = "名称："+record.get('<%=DBSchema.Contract.ContractName %>');
            	document.getElementById('parentAbb').innerHTML = "简称："+record.get('<%=DBSchema.Contract.Abbreviation %>');
            	
            	pcWin.hide();
            	}
            }
        },'-',{
            ref: '../cancelBtn',
            iconCls: 'remove',
            text: '取消',
            disabled: false,
            handler: function(){
				pcWin.hide();
				//there is an err of dom.style when window.close() on (window + grid), so shuold use window.hide() to close
            }
        }],      
         				
		// paging bar on the bottom
       	bbar: new Ext.PagingToolbar({
			pageSize: 20,
			store: pcStore,
			displayInfo: true,
			displayMsg: '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg: "无显示数据"
        })
    }); 	  
    
  pcStore.on('load',function(store,records,options){
  
  	var selectValue = document.getElementById('parentID').value;
    var arr=[];
    for(var i=0;i<records.length;i++){
      var record = records[i];
      if(selectValue ==record.get('<%=DBSchema.Contract.ContractID %>')){
        arr.push(record);
      }
    }
    pcSM.selectRecords(arr);
  },this,{delay:100})

    
  //  var pcWin;
    var contractType;
    var pcBtn = Ext.get('parentContractBtn');
    pcBtn.on('click', function(){
    	contractType = document.getElementById('contractType').value;
         if(!pcWin){
			pcWin=new Ext.Window({ 
				modal: true,
				title:"父合同选择", 
				renderTo:Ext.getBody(), 
				
				frame:true, 
				plain:true, 
				resizable:false, 
				closeAction:"hide", 
				//maximizable:true, 
				closable:true, 
				bodyStyle:"padding:10px", 
				width : 800,//'100%',   
				height:600, 
				//listeners: { 'show': function() {this.findByType('searchfield')[0].focus(true, true);  } },
				//listeners: { 'show': function() {Ext.getEmp('searchCompField').focus(true, true);  } },
				items:[searchCompForm,pcGrid]
			}) 		
        }
        Ext.getCmp('searchCompField').setValue('');
		pcWin.show(); 
		
		//if(document.getElementById("parentID").value.trim() == ""||pcStore.getCount()==0){
			pcStore.removeAll();  
			pcStore.setBaseParam("searchName", "");
			pcStore.setBaseParam("contractTypeID", document.getElementById('contractType').value);
			pcStore.load({
				params:{
					parentID: document.getElementById('parentID').value,
					start:0, 
					limit:20
					},
    			callback:function(record, options ,success){
    				
					if(success == true){
						if(record[0].get("pageNo")>0)
							pcGrid.getBottomToolbar().changePage(record[0].get('pageNo'));
					
					}else{
						Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
					}
				}
			}); 
			
			pcGrid.getView().refresh();
		//}
    });    

    /*====================================================================
     * Show Attached files
     *====================================================================*/
     
	var attStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url:'contractMain.do?method=getAttachedListStroe'
		}),
		remoteSort:true,
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount',
			remoteSort:true,
            fields:[
                 {name:'<%=DBSchema.Attachment.AttachmentID %>'},
                 {name:'<%=DBSchema.Attachment.AttachmentName %>'}
           ]
		})
	});   
    attStore.load({
    	params:{
    		contractID: document.contractFrm.all.contractID.value  
   		}
    	});
    
	var attCM=new Ext.grid.ColumnModel(
		[{
			header: '其他相关文件', 
			width:Ext.get("attachedFileDiv").getWidth(), 
			dataIndex: '<%=DBSchema.Attachment.AttachmentName %>',
			renderer: nameLinker
		}]
	);
	
    var attGrid = new Ext.grid.GridPanel({

    	renderTo:"attachedFileDiv",
       	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
        enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
        cm: attCM,
        store: attStore,
        stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
        autoExpandColumn: 0,
		height:Ext.get("attachedFileDiv").getHeight(),
		width:Ext.get("attachedFileDiv").getWidth(),
        stateful: true,
        autoHeight: true, 
        stateId: 'attGrid',
        loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
        forceFit:true //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
        
       
    });	  
    

    /*====================================================================
     * Upload Attached files
     *====================================================================*/

	var uploadBtn = Ext.get('uploadAttachment');
	
	uploadBtn.on('click', function(){
	
	/*
		if(MM_findObj("contractType").value==0){
			alert("请选择合同类型！");
			MM_findObj("contractType").focus();
			return false;
		}
	*/		
	
		var form ;
		if(!form){
			form= new Ext.form.FormPanel({
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
		        contenteditable: false,
		        anchor : '100%' // anchor width by percentage,
		       }]
		    })
		   }; 
		
		var winUpl;
		if(!winUpl){
		  winUpl = new Ext.Window({
		     modal: true,
		     title : '附件上传',
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
			        	Ext.MessageBox.wait('正在上传附件, 请稍侯 ...', '提示');   
			        	
			        	form.getForm().submit({
					         url : '/xMIS/fileUpload.do?type=' +Upload_Application_Contract_Type,
					         method : 'post',
					         params : {
					         		conID: document.contractFrm.all.contractID.value,
					         		conType: MM_findObj("contractType").value,
					         		conName: MM_findObj("abb").value
			 							},
					         success : function(form, action) {
					         	//alert(action.result.message);
					         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
					         	
					         	document.contractFrm.all.contractID.value = action.result.contractID;
					         	//MM_findObj("abb").value = action.result.contractAbb;
					         	
							    attStore.load({
							    	params:{
							    		contractID: document.contractFrm.all.contractID.value 
							    		},
							    	callback:function(record, options ,success){
										if(success == true){
										}else{
											Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
										}
									}
							    	});
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
		})
		};
		winUpl.setPosition(400, 200);
		winUpl.show();
	});
	
    /*====================================================================
     * Vendor/Buyer; Department Of Vendor/Buyer; Representative Of Vendor/Buyer
     *====================================================================*/

    
	var compStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url:'contractMain.do?method=getCompanyListStroe' 
		}),
		remoteSort:true,
		sortInfo:{field: '<%=DBSchema.Company.CompanyName %>', direction: 'ASC'},  
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount',
			remoteSort:true,
            fields:[
                 {name:'<%=DBSchema.Company.CompanyID %>'},
                 {name:'<%=DBSchema.Company.CompanyName %>'},
                 {name:'<%=DBSchema.Company.Abbreviation %>'},
                 {name:'<%=DBSchema.Province.ProvinceName %>'},
                 {name:'pageNo'}
           ]
		})
	});

  compStore.on('load',function(store,records,options){
  	if(records[0].get("pageNo")>0){
  	}else{
  	var selectValue;
  	if(winFlag == "aWin"){
  		selectValue = document.getElementById('aCompany_id').value;
  	}
  	if(winFlag == "bWin"){
  		selectValue = document.getElementById('bCompany_id').value;
  	}
    var arr=[];
   	for(var i=0;i<records.length;i++){
        var record = records[i];
	    if(selectValue ==  record.get('<%=DBSchema.Company.CompanyID %>')){
	        arr.push(record);
	        break;
	    };    
    };
    compSM.selectRecords(arr);
    var record = compGrid.getSelectionModel().getSelected();
			var index = compGrid.getStore().indexOf(record); 
			if(index != -1){
				compGrid.getView().scrollToRecord(record);
			}	 
    
    }
   /* if(document.getElementById("bCompany_name").value.trim() != ''&& document.getElementById("bCompany_name").value.trim()){
			var record = compGrid.getSelectionModel().getSelected();
			var index = compGrid.getStore().indexOf(record); 
			if(index != -1){
				compGrid.getView().scrollToRecord(record);
			}	  
				
		};*/
  },this,{delay:100})
  
	var compSM= new Ext.grid.CheckboxSelectionModel({
		header: '',
		singleSelect:true
	 }); 

	var compColM=new Ext.grid.ColumnModel(
		[
		compSM,
		{header: "公司ID",dataIndex:"<%=DBSchema.Company.CompanyID %>", sortable:true, hidden:true},
		{header: "公司名称",dataIndex:"<%=DBSchema.Company.CompanyName %>", sortable:true},
		{header: "简称",dataIndex:"<%=DBSchema.Company.Abbreviation %>", sortable:true},
		{header: "省份",dataIndex:"<%=DBSchema.Province.ProvinceName %>", sortable:true}
		]
	);  
	
	var compGrid = new Ext.grid.GridPanel({
		sm:	compSM,
		cm: compColM,
		store: compStore,
		autoExpandColumn:2,
		stripeRows: true,

      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: false,   
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
       	//滚动到指定行
		viewConfig:{   
		  forceFit: true,   
		  deferEmptyText: true,   
		  emptyText: "无相关数据",   
		  
		  //滚动Grid到指定的Record   
		  scrollToRecord:function(record){   
		    var index = this.grid.getStore().indexOf(record);   
		    this.scrollToRow(index);   
		  },   
		  
		  //滚动Grid到指定的列   
		  scrollToRow:function(rowIndex){   
		    var firstRow = Ext.get(this.getRow(0));   
		    var row = Ext.get(this.getRow(rowIndex));   
		    var distance = row.getOffsetsTo(firstRow)[1];   
		    this.scroller.dom.scrollTop = distance;   
		  }   
		},  
		height: 250,
        tbar: [{
        	ref: '../okBtn',
            iconCls: 'ok',
            text: '确定',
            handler: function(){
            	var record = compGrid.getSelectionModel().getSelected();
            	if(record == 'undefined' ||record ==undefined){Ext.Msg.alert("未选择","请先选择！");return true;}
            	if(winFlag == "aWin"){
	            	document.getElementById('aCompany_id').value = record.get('<%=DBSchema.Company.CompanyID %>');
    	        	document.getElementById('aCompany_name').value = record.get('<%=DBSchema.Company.CompanyName %>');
    	    		document.getElementById('aDept_id').value = "";
    	        	document.getElementById('aDept_name').value = "";
	            	document.getElementById('aEmp_id').value = "";
    	        	document.getElementById('aEmp_name').value = "";
    	        	document.getElementById("delivery_id").value = "";
					document.getElementById("cEmp_name").value = "";
            	}
            	if(winFlag == "bWin"){
	            	document.getElementById('bCompany_id').value = record.get('<%=DBSchema.Company.CompanyID %>');
    	        	document.getElementById('bCompany_name').value = record.get('<%=DBSchema.Company.CompanyName %>');
	        		document.getElementById('bDept_id').value =  "";
    	        	document.getElementById('bDept_name').value = "";
	            	document.getElementById('bEmp_id').value = "";
    	        	document.getElementById('bEmp_name').value = "";
    	        	document.getElementById("delivery_id").value = "";
					document.getElementById("cEmp_name").value = "";
        	    	
            	}
       	    	winFlag = "";
       	    	compGrid.ownerCt.hide();
            }
        },'-',{
            ref: '../cancelBtn',
            iconCls: 'remove',
            text: '取消',
            disabled: false,
            handler: function(){
				compGrid.ownerCt.hide();
				//if(bCompanyWin) bCompanyWin.hide();
				//there is an err of dom.style when window.close() on (window + grid), so shuold use window.hide() to close
            }
        },'->',{
            ref: '../addBtn',
            iconCls: 'add',
            align: 'right',
            text: '新建',
            disabled: false,
            handler: function(){
            
            showCompWindow('新公司', newCompForm);
				
				Ext.getCmp('companyName').setValue('');
				Ext.getCmp('shotCompName').setValue('');
				Ext.getCmp('compPartName').setValue('');
				Ext.getCmp('compDelegate').setValue('');
							
            }
        }],       	
        bbar: new Ext.PagingToolbar({
			pageSize: 20,
			store: compStore,
			displayInfo: true,
			displayMsg: '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg: "无显示数据"
        })    
         				

    }); 	  
    


    var winFlag ;
    var contructPersonFlag = '';
    var renameFlag='';
    
    var aCompanyBtn = Ext.get('aCompanyBtn');
    aCompanyBtn.on('click', function(){
		
		showGridWindow("甲方公司设置",compGrid);
			compStore.removeAll();  
			compStore.load({
				params:{
					compID: document.getElementById("aCompany_id").value,
					start:0, 
					limit:20
				},			
				callback:function(record, options ,success){
					if(success == true){
						if(record[0].get('pageNo')>0){
							compGrid.getBottomToolbar().changePage(record[0].get('pageNo'));
						}
					}else{
						Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
					}
				}
			}); 
			
			compGrid.getView().refresh();
			
	/*	if(document.getElementById("aCompany_name").value.trim() != ''){
			var record = compGrid.getSelectionModel().getSelected();
			var index = compGrid.getStore().indexOf(record); 
			if(index != -1){
				//alert('2224');
				compGrid.getView().scrollToRecord(record);
				//compGrid.getView().focusRow(18); 
			}	  
				
		}; */
		winFlag = "aWin";
			 
	  });    
	
    //var bCompanyWin;
    var bCompanyBtn = Ext.get('bCompanyBtn');
    bCompanyBtn.on('click', function(){
		showGridWindow("乙方公司设置",compGrid);
		compStore.removeAll();  
		compStore.load({
			params:{
				compID: document.getElementById("bCompany_id").value,
				start:0, 
				limit:20
			},
			callback:function(record, options ,success){

				if(success == true){
					if(record[0].get('pageNo')>0){
						compGrid.getBottomToolbar().changePage(record[0].get('pageNo'));
					}
				}else{
					Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
				}
			}
		}); 
		compGrid.getView().refresh();
		
		
		winFlag = "bWin";
		 
    });    

    /*====================================================================
     * CompanyForm
     *====================================================================*/	  
    var newCompForm = new Ext.form.FormPanel({
	    baseCls : 'x-plain',
	    anchor : '100%',
		lableWidth:10, 
		defaultType : 'textfield',
		defaults:{xtype:"textfield",width:200}, 
		items:[
			{id:'companyName',fieldLabel:"公司名称", inputType:'text', name:'compName', allowBlank:false, blankText:"公司名称不能为空",
			 msgTarget:'under'
		 	},
			{id:'shotCompName',fieldLabel:"公司简称", inputType:'text', name:'compAbb'},
			{id:'compPartName',fieldLabel:"部门名称", inputType:'text', name:'deptName'},
			{id:'compDelegate',fieldLabel:"公司代表", inputType:'text', name:'empName'}
		],
		
		buttons:[{text:"创建",
			handler:function(){
			    validateFunc('<%=WebKeys.New_Company_Simple_Type_Company %>','companyName',newCompForm);
						
				}
			},
			{
				text:"取消",
				handler:function(){ 
			       newCompForm.ownerCt.hide();
		        }        
			 }
				]
    });    
    
	/*====================================================================
     * showGridWindow ; 
     *====================================================================*/
	
	var GridWin;
     showGridWindow = function(title, grid) {
     	if(!GridWin){	
	     	GridWin=new Ext.Window({ 
					modal: true,
					title:title, 
					renderTo:Ext.getBody(), 
					frame:true, 
					plain:true, 
					resizable:false, 
					closeAction:"hide", 
					//maximizable:true, 
					closable:true, 
					bodyStyle:"padding:10px", 
					width:500, 
					height:300, 
					items:grid
				}) 
     	};
     	GridWin.setTitle(title);
     	GridWin.show();
     	
    };    
    
    
  	/*====================================================================
     * showCompWindow ; 
     *====================================================================*/	
     var	newCompWin;	    
	 showCompWindow = function(title, form) {
	 	if(!newCompWin){
			newCompWin=new Ext.Window({ 			
					layout: 'fit',
					modal: true,
					title :title,
					renderTo:Ext.getBody(), 
					frame:true, 
					plain:true, 
					resizable:false, 
					buttonAlign:"center", 
					//closeAction:"close", 
					//maximizable:true, 
					closable:false, 
					bodyStyle:"padding:10px", 
					width:360, 
					//height:auto, 
					layout:"form", 
					//lableWidth:10, 
					defaults:{xtype:"textfield",width:200}, 
					//monitorValid:true,
					listeners: { 'show': function() { this.findByType('textfield')[0].focus(true, true);  } },
				    items : form
					
			
			});
		};	
		//alert(newCompWin.getItemId());
		newCompWin.setTitle(title);
		newCompWin.show();
	};
	
    


	var comID;
	var deptID;
	
    /*====================================================================
     * Department Of Vendor/Buyer; 
     *====================================================================*/

    
	var deptStore=new Ext.data.GroupingStore({
		proxy: new Ext.data.HttpProxy({
			url:'contractMain.do?method=getDeptListStroeByCompany' 
		}),
		remoteSort:true,
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount',
			remoteSort:true,
            fields:[
                 {name:'parentName'},
                 {name:'deptID'},
                 {name:'deptName'},
           ]
		}),
		groupField:'parentName'
	});

	var deptSM= new Ext.grid.CheckboxSelectionModel({
		header: '',
		singleSelect:true
	 }); 
	var deptColM=new Ext.grid.ColumnModel(
		[
		deptSM,
		{header: "事业部名称",dataIndex:"parentName", sortable:true },
		{header: "部门ID",dataIndex:"deptID", sortable:true, hidden:true},
		{header: "部门名称",dataIndex:"deptName", sortable:true}
		]
	);  
	
	
	var deptGrid = new Ext.grid.GridPanel({
		sm:	deptSM,
		cm: deptColM,
		store: deptStore,
		
		view: new Ext.grid.GroupingView({
            forceFit:true,
            hideGroupedColumn: true, 	//用来分组的数据点这一列是否要显示 
            
            showGroupName: false, 	//用来分组的数据点这一列的header是否要随group name一起显示 
            
            startCollapsed: false	//一开始进到grid这页，它的group是合起还是展开 
        }),
        
              
		autoExpandColumn:3,
		stripeRows: true,

      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: false,   
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
		height:250, 
        tbar: [{
        	ref: '../okBtn',
            iconCls: 'ok',
            text: '确定',
            handler: function(){
            	var record = deptGrid.getSelectionModel().getSelected();
            	if(record == 'undefined' ||record ==undefined){Ext.Msg.alert("未选择","请先选择！");return true;}
            	if(winFlag == "aDeptWin"){
	            	document.getElementById('aDept_id').value = record.get('deptID');
    	        	document.getElementById('aDept_name').value = record.get('deptName');
        	    	deptGrid.ownerCt.hide();
            	}
            	if(winFlag == "bDeptWin"){
	            	document.getElementById('bDept_id').value = record.get('deptID');
    	        	document.getElementById('bDept_name').value = record.get('deptName');
        	    	deptGrid.ownerCt.hide();
            	}
       	    	winFlag = "";
            }
        },'-',{
            ref: '../cancelBtn',
            iconCls: 'remove',
            text: '取消',
            disabled: false,
            handler: function(){
				 deptGrid.ownerCt.hide();
				
				//there is an err of dom.style when window.close() on (window + grid), so shuold use window.hide() to close
            }
        },'->',{
            ref: '../addBtn',
            iconCls: 'add',
            align: 'right',
            text: '新建',
            disabled: true,
            handler: function(){
            showPartWindow('新部门', newpartForm);
			//Ext.getCmp("deptName").clearInvalid() 
            }
        }]    
         				

    }); 	  
    
  deptStore.on('load',function(store,records,options){
    if(deptStoreFlag == 1){
	  	var selectValue;
	  	if(winFlag == "aDeptWin"){
	  		selectValue = document.getElementById('aDept_id').value;
	  	}
	  	if(winFlag == "bDeptWin"){
	  		selectValue = document.getElementById('bDept_id').value;
	  	}
	    var arr=[];
	    for(var i=0;i<records.length;i++){
	        var record = records[i];
		    if(selectValue ==  record.get('deptID')){
		        arr.push(record);
		    };
	    
	    };
	   /* for(var i=0;i<records.length;i++){
	      var record = records[i];
	      var isMatch = selectValue.indexOf(record.get('deptID'))!=-1;
	      if(isMatch){
	        arr.push(record);
	      }
	    }*/
	    deptSM.selectRecords(arr);
	}
	deptStoreFlag = 0;
  },this,{delay:100})

   // var aDeptWin;
    var aDeptBtn = Ext.get('aDeptBtn');
     /*====================================================================
     * 自动滚动到已选部门行 ; 
     *====================================================================*/
    
    function deptClick(e, el, args) {
        if(document.getElementById(args.eleName).value.trim() != ''){
			var record = deptGrid.getSelectionModel().getSelected();
			var index = deptGrid.getStore().indexOf(record); 						
			if(index != -1){
				deptGrid.getView().focusRow(index); 
			}	  				
		};   
    }; 
    Ext.get('aDeptBtn').on('click', deptClick, this, {   
        single: false,   
        buffer: 700,
        eleName: "aDept_name"

    });
    Ext.get('bDeptBtn').on('click', deptClick, this, {   
        single: false,   
        buffer: 700,
        eleName: "bDept_name"

    });  
    aDeptBtn.on('click', function(){
       
        if(document.getElementById("aCompany_name").value == ""){
        	Ext.Msg.alert("<%=WebKeys.Ext_Message_Alert_Title %>","<%=WebKeys.Ext_Message_For_Company_Null%>" + "甲方公司。");
        	return false;
        }
    	deptStoreFlag = 1;
		showPartGridWindow("甲方部门设置",deptGrid);
		deptStore.removeAll();  
		deptStore.load({
			params:{
				compID: document.getElementById("aCompany_id").value//根据公司compID  确定store
			},			
			callback:function(record, options ,success){
			if(success == true){
				}else{
					Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
				}
			}
		}); 
		
		deptGrid.getView().refresh();
		winFlag = "aDeptWin";  //点了哪一行的部门，然后确定调什么数据库
		
		deptGrid.addBtn.setDisabled(true);
		if(document.getElementById("aCompany_id").value>0 && document.getElementById("aCompany_id").value != <%=emp.getCompanyID() %>)
			deptGrid.addBtn.setDisabled(false);
	
		 
    });    
	
    var bDeptBtn = Ext.get('bDeptBtn');
    bDeptBtn.on('click', function(){
	    if(document.getElementById("bCompany_name").value == ""){
	        Ext.Msg.alert("<%=WebKeys.Ext_Message_Alert_Title %>","<%=WebKeys.Ext_Message_For_Company_Null%>" + "乙方公司。");
	        return false;
	    }
    	deptStoreFlag = 1;
		showPartGridWindow("乙方部门设置",deptGrid)
		deptStore.removeAll();  
		deptStore.load({
			params:{
				compID: document.getElementById("bCompany_id").value
			},			
			callback:function(record, options ,success){
				if(success == true){
				}else{
					Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
				}
			}
		}); 
		
		
		deptGrid.getView().refresh();
		winFlag = "bDeptWin";
		
		deptGrid.addBtn.setDisabled(true);
		if(document.getElementById("bCompany_id").value>0 && document.getElementById("bCompany_id").value != <%=emp.getCompanyID() %>)
			deptGrid.addBtn.setDisabled(false);
	
		
		 
    });      
		
    /*====================================================================
     * PartForm
     *====================================================================*/	
    
     var newpartForm = new Ext.form.FormPanel({  //新建部门
				    baseCls : 'x-plain',
				    anchor : '100%',
					lableWidth:10, 
					defaultType : 'textfield',
					defaults:{xtype:"textfield",width:200}, 
					items:[
						{fieldLabel:"部门名称", inputType:'text', id:'deptName', allowBlank:false, blankText:"部门名称不能为空", msgTarget:'under'
										}],
					buttons:[{text:"创建",
							id:'deptConfermBtn',
							handler:function(){
							    validateFunc('<%=WebKeys.New_Company_Simple_Type_Department %>','deptName',newpartForm);

							}
						},
						{text:"取消",handler:function(){newpartForm.ownerCt.hide();}}]
		 });


    
    /*====================================================================
     * showPartGridWindow ; 
     *====================================================================*/
	
	var partGridWindow;
     showPartGridWindow = function(title, grid) {
     	if(!partGridWindow){	
	     	partGridWindow=new Ext.Window({ 
					modal: true,
					title:title, 
					renderTo:Ext.getBody(), 
					frame:true, 
					plain:true, 
					resizable:false, 
					closeAction:"hide", 
					//maximizable:true, 
					closable:true, 
					bodyStyle:"padding:10px", 
					width:400, 
					height:300, 
					items: grid
				}) 
     	};
     	partGridWindow.setTitle(title);
     	partGridWindow.show();
     	//grid.getRow(2).focus();
    };
    /*====================================================================
     * showPartWindow ; 
     *====================================================================*/
    
    var	newPartWin;	    
	 showPartWindow = function(title, form) {
	 	if(!newPartWin){
			newPartWin=new Ext.Window({ 			
				layout: 'fit',
				modal: true,
				title :title,
				renderTo:Ext.getBody(), 
				frame:true, 
				plain:true, 
				resizable:false, 
				buttonAlign:"center",  
				closable:false, 
				bodyStyle:"padding:10px", 
				width:360, 
				layout:"form", 
				defaults:{xtype:"textfield",width:200}, 
				listeners: { 'show': function() { this.findByType('textfield')[0].focus(true, true);  } },
			    items : form
			});
		};	
		//alert(newCompWin.getItemId());
		Ext.getCmp('deptName').setValue('');
		newPartWin.setTitle(title);
		newPartWin.show();
		
		
	};
	
	
    /*====================================================================
     * Contact Person ; 
     *====================================================================*/
	var searchField = new Ext.ux.form.SearchField({
		store: peasonStore,
		width: 155,
		
		onTrigger2Click : function(){//点击查询按钮或回车调用该方法   
			peasonGrid.getView().expandAllGroups();
			peasonStore.removeAll();
			peasonStore.proxy.setUrl('crmMaint.do?method=getContactPersonBySearchName',true);
			peasonStore.load({
				params:{
					compID: searchForm.findById("compID").getValue(),//Ext.getCmp("compID").value,
					searchName: this.getRawValue()
				},
				callback:function(record, options ,success){
				if(success == true){
				}else{
					Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
				}
			}
			});  
		}   
    });

	var searchForm = new Ext.form.FormPanel({
	    baseCls : 'x-plain',
	    anchor : '100%',
	    width:600,
	    
		items:[{
			baseCls : 'x-plain',
			xtype: "panel",
			layout:  "column",
			fieldLabel: "联系人名称",
			lableWidth:10,
			isFormField:true,
			items:	[			
					{xtype:"textfield", inputType:'hidden', id:'compID'},
					//{xtype:"textfield",width:120, inputType:'text', id:'searchStr'},
					searchField
					]
		}]					
		
    });
    
    	
	var peasonSM= new Ext.grid.CheckboxSelectionModel({
		header: '',
		singleSelect:true
	 }); 
	 
	var peasonColM=new Ext.grid.ColumnModel(
		[
		peasonSM,
		{header: "部门名称",dataIndex: "deptName", sortable:true , hidden:true},
		{header: "联系人ID",dataIndex: "<%=DBSchema.EmployeeBelongToDept.EmployeeID %>", sortable:true, hidden:true},
		{header: "联系人名称",dataIndex: "user_name", sortable:true}
		]
	);  

	var peasonStore=new Ext.data.GroupingStore({
		proxy: new Ext.data.HttpProxy({
			url:'crmMaint.do?method=getContactPersonByCompany' 
		}),
		remoteSort:true,
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount',
			remoteSort:true,
            fields:[
                 {name:'deptName'},
                 {name:'<%=DBSchema.EmployeeBelongToDept.EmployeeID %>'},
                 {name:'user_name'},
           ]
		}),
		groupField:'deptName'
	});
	
	var peasonView = new Ext.grid.GroupingView({
            forceFit:true,
  
		 // deferEmptyText: true,   
		  emptyText: "无相关数据",
		  //listeners: { 'show': function() { this.findByType('textfield')[0].focus(true, true);  } },
		  listeners:{'beforerefresh' :function(){} },   
		  
		  //滚动Grid到指定的Record   
		  scrollToRecord:function(record){   
		    var index = this.grid.getStore().indexOf(record);   
		    this.scrollToRow(index);   
		  },   
		  
		  //滚动Grid到指定的列   
		  scrollToRow:function(rowIndex){   
		    var firstRow = Ext.get(this.getRow(0));   
		    var row = Ext.get(this.getRow(rowIndex)); 
		      
		    var distance = row.getOffsetsTo(firstRow)[1];
		    //alert('firstR='+firstRow+'row='+row+'distance='+distance);   
		    this.scroller.dom.scrollTop = 120;//distance;   
		  },
            hideGroupedColumn: false, 	//用来分组的数据点这一列是否要显示 
            
            showGroupName: false ,	//用来分组的数据点这一列的header是否要随group name一起显示 
            
            startCollapsed:true	//一开始进到grid这页，它的group是合起还是展开 
        });

	/*====================================================================
     * newContactPersonFrom ; 
     *====================================================================*/
    
	var newContactPersonForm = new Ext.form.FormPanel({
	    baseCls : 'x-plain',
	    anchor : '100%',
		lableWidth:10, 
		defaultType : 'textfield',
		defaults:{xtype:"textfield",width:200}, 
		items:[
			{id:'contactPersonName',fieldLabel:"联系人名称", inputType:'text', name:'empName', allowBlank:false, blankText:"联系人名称不能为空", msgTarget:'under'
		}],
		buttons:[
			{text:"创建",
			//disabled:true,
			 id:'PersonConfermBtn',
				handler:function(){
				validateFunc('<%=WebKeys.New_Company_Simple_Type_ContactPerson %>','contactPersonName',newContactPersonForm);
				}},
			{text:"取消",handler:function(){newContactPersonForm.ownerCt.hide();}}
		]
    });
    
    /*====================================================================
     * newContactPersonWin ; 
     *====================================================================*/
    var newContactPersonWin;
    function showContactPersonWin(title){
        if(!newContactPersonWin){	
		  	var newContactPersonWin=new Ext.Window({ 
				layout: 'fit',
				modal: true,
				title:title, 
				renderTo:Ext.getBody(), 
				frame:true, 
				plain:true, 
				resizable:false, 
				buttonAlign:"center", 
				closable:false, 
				bodyStyle:"padding:10px", 
				width:360, 
				height:130, 
				layout:"form", 
				//lableWidth:10, 
				defaults:{xtype:"textfield",width:200}, 
				//monitorValid:true,
				listeners: { 'show': function() { newContactPersonForm.findByType('textfield')[0].focus(true, true);  } },
				items: newContactPersonForm 
				
			}) ;
    	};
    	Ext.getCmp('contactPersonName').setValue('');
		newContactPersonWin.setTitle(title);
		newContactPersonWin.show();
    }; 
	
	var peasonGrid = new Ext.grid.GridPanel({
		sm:	peasonSM,
		cm: peasonColM,
		store: peasonStore,
		listeners:{},
		
		view: peasonView,
		autoExpandColumn:3,
		stripeRows: true,

      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: false,   
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
		height:320,
		
        tbar: [{
        	ref: '../okBtn',
            iconCls: 'ok',
            text: '确定',
            handler: function(){
           	   // var record = deptGrid.getSelectionModel().getSelected();
            	var record = peasonGrid.getSelectionModel().getSelected();
         		if(record == 'undefined' ||record ==undefined){Ext.Msg.alert("未选择","请先选择！");return true;}
            	if(winFlag == "aEmpWin" && contructPersonFlag !="cEmpWin" ){
	            	document.getElementById('aEmp_id').value = record.get('<%=DBSchema.EmployeeBelongToDept.EmployeeID %>');
    	        	document.getElementById('aEmp_name').value = record.get('user_name');
            	}
            	if(winFlag == "bEmpWin" && contructPersonFlag !="cEmpWin" ){
	            	document.getElementById('bEmp_id').value = record.get('<%=DBSchema.EmployeeBelongToDept.EmployeeID %>');
    	        	document.getElementById('bEmp_name').value = record.get('user_name');
            	}
            	if(contructPersonFlag == "cEmpWin"){
            		document.getElementById('delivery_id').value = record.get('<%=DBSchema.EmployeeBelongToDept.EmployeeID %>');
    	        	document.getElementById('cEmp_name').value = record.get('user_name');
            	    //contructPersonFlag ='';
            	}	
            	peasonWin.hide();
       	    	winFlag = "";
            }
        },'-',{
            ref: '../cancelBtn',
            iconCls: 'remove',
            text: '取消',
            disabled: false,
            handler: function(){
				if(peasonWin) peasonWin.hide();
				
				//there is an err of dom.style when window.close() on (window + grid), so shuold use window.hide() to close
            }
        },'->',{
            ref: '../addBtn',
            iconCls: 'add',
            align: 'right',
            text: '新建',
            id:'addContactPerson',
            disabled: true,
            handler: function(){
            	showContactPersonWin("新联系人");
            	Ext.getCmp("contactPersonName").setValue('');
            	if(contructPersonFlag == "cEmpWin"){
            		newContactPersonForm.ownerCt.setTitle("新收货人");
            	};
            
            }
        }] ,   
        afterrender :function(){alert('111');}				

    }); 	
    
    peasonStore.on('load',function(store,records,options){
  
  	var selectValue;
  	if(winFlag == "aEmpWin"){
  		selectValue = document.getElementById('aEmp_id').value;
  	}
  	if(winFlag == "bEmpWin"){
  		selectValue = document.getElementById('bEmp_id').value;
  	}
  	if(contructPersonFlag == "cEmpWin"){
  		selectValue = document.getElementById('delivery_id').value;
  	}

    var arr=[];
    for(var i=0;i<records.length;i++){
        var record = records[i];
	    if(selectValue ==  record.get("<%=DBSchema.EmployeeBelongToDept.EmployeeID %>")){
	        arr.push(record);
	    };
    
    }
    peasonSM.selectRecords(arr);
  },this,{delay:100})
    
	var peasonWin = new Ext.Window({ 
		modal: true,
		renderTo:Ext.getBody(), 
		frame:true, 
		plain:true, 
		resizable:false, 
		closeAction:"hide", 
		closable:true, 
		bodyStyle:"padding:10px", 
		width:300, 
		height:400, 
		items:[searchForm,peasonGrid]	
	});
			
    var aEmpBtn = Ext.get('aEmpBtn');
    aEmpBtn.on('click', function(){
       if(document.getElementById("aCompany_name").value == ""){
	        Ext.Msg.alert("<%=WebKeys.Ext_Message_Alert_Title %>","<%=WebKeys.Ext_Message_For_Company_Null%>" + "甲方公司。");
	        return false;
	    }
		openPeasonWin(aEmpBtn.getValue(), Ext.get('aCompany_id').getValue());
		contructPersonFlag = "";
		winFlag = "aEmpWin";  //点了哪一行的联系人
		

		Ext.getCmp('addContactPerson').disable();
		if(document.getElementById("aCompany_id").value>0 && document.getElementById("aCompany_id").value != <%=emp.getCompanyID() %>)
		   Ext.getCmp('addContactPerson').enable();
		
	
     });
     /*====================================================================
     * 自动滚动到选择人员行 ; 
     *====================================================================*/
    function testClick(e, el, args) {
        if(document.getElementById(args.eleName).value.trim() != ''){
			var record = peasonGrid.getSelectionModel().getSelected();
			var index = peasonGrid.getStore().indexOf(record); 						
			if(index != -1){
				peasonView.focusRow(index); 
			}	  				
		};   
    }; 
    Ext.get('aEmpBtn').on('click', testClick, this, {   
        single: false,   
        buffer: 1000,
        eleName: "aEmp_name"

    }); 
    Ext.get('bEmpBtn').on('click', testClick, this, {   
        single: false,   
        buffer: 1000,
        eleName: "bEmp_name"

    }); 
    Ext.get('cEmpBtn').on('click', testClick, this, {   
        single: false,   
        buffer: 1000,
        eleName: "cEmp_name"

    });   
     

    var bEmpBtn = Ext.get('bEmpBtn');
    bEmpBtn.on('click', function(){
           if(document.getElementById("bCompany_name").value == ""){
	        Ext.Msg.alert("<%=WebKeys.Ext_Message_Alert_Title %>","<%=WebKeys.Ext_Message_For_Company_Null%>" + "乙方公司。");
	        return false;
	    }
		openPeasonWin(bEmpBtn.getValue(), Ext.get('bCompany_id').getValue());//点了哪一行 就把那一行的公司ID传参
		winFlag = "bEmpWin";  //点了哪一行的联系人
		contructPersonFlag = "";
		
		Ext.getCmp('addContactPerson').disable();
		if(document.getElementById("bCompany_id").value>0 && document.getElementById("bCompany_id").value != <%=emp.getCompanyID() %>)
		{	
		
			Ext.getCmp('addContactPerson').enable();};
     });
   
     
     
    var cEmpBtn = Ext.get('cEmpBtn');
    
    cEmpBtn.on('click', function(){
    	
    	if((MM_findObj("aCompany_id").value.trim()=="" || MM_findObj("aCompany_id").value==0) && (MM_findObj("bCompany_id").value.trim()=="" || MM_findObj("bCompany_id").value==0)){
	           Ext.Msg.alert("<%=WebKeys.Ext_Message_Alert_Title %>","<%=WebKeys.Ext_Message_For_Company_Null%>" + "公司。");
	           return false;
		}
    	
    
	    if(document.getElementById("aCompany_id").value == <%=emp.getCompanyID() %> || document.getElementById("aCompany_id").value==0){
	       if(document.getElementById("bCompany_name").value ==""){
	           Ext.Msg.alert("<%=WebKeys.Ext_Message_Alert_Title %>","<%=WebKeys.Ext_Message_For_Company_Null%>" + "乙方公司。");
	           return false;
	       }
	    
	    } 
	    if(document.getElementById("bCompany_id").value == <%=emp.getCompanyID() %>||document.getElementById("aCompany_id").value==0){
		    if(document.getElementById("aCompany_name").value ==""){
		           Ext.Msg.alert("<%=WebKeys.Ext_Message_Alert_Title %>","<%=WebKeys.Ext_Message_For_Company_Null%>" + "甲方公司。");
		           return false;
		       }
	    
	    }
    
	    if(document.getElementById("bCompany_id").value>0 && document.getElementById("bCompany_id").value != <%=emp.getCompanyID() %>)
		{
			openPeasonWin(Ext.get('bCompany_name').getValue()+cEmpBtn.getValue(), Ext.get('bCompany_id').getValue());
			winFlag = "bEmpWin"; 
		}else{
		
			openPeasonWin(Ext.get('aCompany_name').getValue()+cEmpBtn.getValue(), Ext.get('aCompany_id').getValue());
			winFlag = "aEmpWin"; 
		}
		contructPersonFlag = "cEmpWin";
		Ext.getCmp('addContactPerson').enable();
     });
  
  	/*====================================================================
     * validateFunc ; 
     *====================================================================*/
     
 	function validateFunc(type,id,chooseForm){ 
		if(chooseForm.findById(id).getValue() ==""){
		    Ext.Msg.alert("请输名称！","名称不能为空！");
			chooseForm.findById(id).focus();
			return  true;
		}	
		if(winFlag == "aDeptWin"){
			comID = document.getElementById('aCompany_id').value;
		}
		if(winFlag == "bDeptWin"){
			comID = document.getElementById('bCompany_id').value;
		}
		if(winFlag == "aEmpWin"){
			comID = document.getElementById('aCompany_id').value;
			deptID =document.getElementById('aDept_id').value;
		}
		if(winFlag == "bEmpWin"){
			comID = document.getElementById('bCompany_id').value;
			deptID =document.getElementById('bDept_id').value;
		}

	
		chooseForm.getForm().submit({
	         url : 'crmMaint.do?method=dataValidate',
			 params : {
			 	compID: comID,
			 	deptID:deptID,
			 	type: type
			 }	,
	         method : 'post',
	         success : function(form, action) {
	          submitForm();
	         },
         	failure : function(form, action) {
				Ext.Msg.confirm(action.result.messageTitle,action.result.message, function(btn, text){
				    if (btn == 'yes'){
				        submitForm();
				        };
				    if(btn=='no'){
						chooseForm.findById(id).setValue('');
			         	chooseForm.findById(id).focus();
				    };
			});
       	}
	});
function submitForm(){
				 
	   chooseForm.getForm().submit({
         url : 'crmMaint.do?method=createNewCompanySimple',
		 params : {
		 	compID: comID,
		 	deptID:deptID,
		 	type: type
		 },
         method : 'post',
		
         success : function(form, action) {
         	//alert(action.result.message);
         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
         	if(winFlag == "aWin"){
            	document.getElementById('aCompany_id').value = action.result.compID;
   	        	document.getElementById('aCompany_name').value = action.result.compName;
            	document.getElementById('aDept_id').value = action.result.deptID;
   	        	document.getElementById('aDept_name').value = action.result.deptName;
            	document.getElementById('aEmp_id').value = action.result.empID;
   	        	document.getElementById('aEmp_name').value = action.result.empName;
   	        	document.getElementById("delivery_id").value = "";
				document.getElementById("cEmp_name").value = "";
   	        	
		    	        	
		    }
		    if(winFlag == "bWin"){
            	document.getElementById('bCompany_id').value = action.result.compID;
   	        	document.getElementById('bCompany_name').value = action.result.compName;
            	document.getElementById('bDept_id').value = action.result.deptID;
   	        	document.getElementById('bDept_name').value = action.result.deptName;
            	document.getElementById('bEmp_id').value = action.result.empID;
   	        	document.getElementById('bEmp_name').value = action.result.empName;
   	        	document.getElementById("delivery_id").value = "";
				document.getElementById("cEmp_name").value = "";
	    	   
		     }	
		    
		    
         	if(winFlag == "aDeptWin"){
            	document.getElementById('aDept_id').value = action.result.deptID;
   	        	document.getElementById('aDept_name').value = action.result.deptName;
           	}
           	if(winFlag == "bDeptWin"){
            	document.getElementById('bDept_id').value = action.result.deptID;
   	        	document.getElementById('bDept_name').value = action.result.deptName;	    	        
           	}	
			
			
           	if(winFlag == "aEmpWin" && contructPersonFlag !="cEmpWin" ){
            	document.getElementById('aEmp_id').value = action.result.empID;
   	        	document.getElementById('aEmp_name').value = action.result.empName;
   	        	
           	}
           	if(winFlag == "bEmpWin" && contructPersonFlag !="cEmpWin"){
            	document.getElementById('bEmp_id').value = action.result.empID;
   	        	document.getElementById('bEmp_name').value = action.result.empName;
           	}	
           	if(contructPersonFlag == "cEmpWin"){
           		document.getElementById('delivery_id').value = action.result.empID;
   	        	document.getElementById('cEmp_name').value = action.result.empName;
           	    //contructPersonFlag ='';
           	}
           	if(chooseForm == newCompForm){
            	compGrid.ownerCt.hide();
            	newCompForm.ownerCt.hide();		
           	}
           	if(chooseForm == newpartForm ){
           	deptGrid.ownerCt.hide();
			newpartForm.ownerCt.hide();				
			}
			if(chooseForm == newContactPersonForm){
         	newContactPersonForm.ownerCt.hide();
         	peasonWin.hide();
         	}
         },
	       	failure : function(form, action) {
	        	Ext.Msg.alert(action.result.messageTitle,action.result.message);
	       	}
		}); 

 	};
};

    
    function openPeasonWin(title, compID){
		Ext.getCmp("compID").setValue(compID);
		
    	peasonWin.setTitle(title);
	    peasonWin.setPosition(120, 80);    
		peasonWin.show(); 
		peasonStore.removeAll();
		peasonStore.proxy.setUrl('crmMaint.do?method=getContactPersonByCompany',true);
		peasonStore.load({
			params:{
				compID: compID
			},
			callback:function(record, options ,success){
				if(success == true){
				}else{
					Ext.Msg.alert("<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>","<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>");
				}
			}
		}); 
		//peasonGrid.getView().refresh();
		searchField.setValue("");
    
    };    

	
});
</script>
