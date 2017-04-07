<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.util.PropertyFilter"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@ page import="com.bitc.jk.xmis.util.Type"%>
<%@ page import="com.bitc.jk.xmis.model.Stencil" %>
<%@ page import="com.bitc.jk.xmis.model.ContractInfoObj" %>
<!DOCTYPE HTML>
<%
	ContractObj con = (ContractObj) request
			.getAttribute(WebKeys.Request_Attribute_Name_ContractObject);
	EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(
			WebKeys.Session_Attribute_Name_UserDNA);
	String parentID=(String)request.getAttribute("parentID");
	String parentNo=(String)request.getAttribute("parentNo");
	String parentName=(String)request.getAttribute("parentName");
	String parentAbb=(String)request.getAttribute("parentAbb");
	String contractType=request.getParameter("contractType");
	boolean hasContractControlOnCompanyLevel = emp.hasContractControlOnCompanyLevel();
	boolean readOnly = emp.isReadOnly();
	boolean decisionFlag = emp.hasDecisionRole();
	String aaa = emp.getDepartmentName();
	String prefix = (String)request.getAttribute("prefix");
%>
<html>
<head>
<%@include file="../jsCss.jsp" %>
<script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/SearchPanel.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/MultiSearchPanel.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ChooseField.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/InklingSearchField.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/RowEditor.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/CheckColumn.js"></script>
<script type="text/javascript" src="<%=path %>/contract/glIncomeList.js"></script>
<script type="text/javascript" src="<%=path %>/contract/glPurchaseList.js"></script>
<script type="text/javascript" src="<%=path %>/contract/glElseIncomeList.js"></script>
<script type="text/javascript" src="<%=path %>/resources/js/WebKeys.jsp"></script>
<script type="text/javascript" src="<%=path %>/contract/contractInfoShow.js"></script>
<script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
<script type="text/javascript" src="<%=path %>/contract/delayReasonGridPanel.js"></script>
<script type="text/javascript" src="<%=path %>/resources/js/Attention.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/GridRowEditForm.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/NavButtonPanel.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
<script type="text/javascript" src="<%=path%>/project/ProjectSchedule.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/conInfoObjCom.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/CustomNumberField.js"></script>

<style type="text/css">
body .x-panel {
	margin-bottom: 0px;
}
.x-tree-node-icon
{
display:none !important;
}
#totalAmtMsg td{
	FONT-SIZE: 12px;
	FONT-FAMILY: tahoma, arial, helvetica, sans-serif;
}
</style>
<script type="text/javascript">
	var prefix = "<%=prefix%>";
	var readOnly = <%=readOnly%>;
	var decisionFlag = <%=decisionFlag%>;
</script>
<script type="text/javascript" src="<%=path%>/contract/editContract.js"></script>
<script type="text/javascript">
var hasContractControlOnCompanyLevel = <%=hasContractControlOnCompanyLevel%>;
var contractType = null;
var contract = null;
<%if (con != null && con.getContractID() != 0) {
	out.print("contract = " + con.toJsonString());
}else if(contractType!=null){
	out.print("contractType = "+contractType);
}%>
var parentID = null;
<%if(parentID!=null){
	out.print("parentID = '" + parentID +"';");
	out.print("var parentNo = '" + (parentNo==null?"":parentNo) +"';");
	out.print("var parentName = '" + (parentName==null?"":parentName) +"';");
	out.print("var parentAbb = '" + (parentAbb==null?"":parentAbb) +"';");
}%>
var empName="<%=emp.getUserName()%>";
var empDeptID = "<%=emp.getDepartmentID()%>";
var empDeptName = "<%=emp.getDepartmentName()%>";
var RegisterID = "<%=con.getRegisterID()%>";
var lastModificator = "<%=con.getLastModificator()%>";
var contractOpType = "<%=con.getContractOpType()%>";
var contractIdInPay = "<%=con.getContractID() %>";
function contractTypeChange(type){
	if(type==1){
		Ext.getCmp("bCompany_id").setValue("<%=emp.getCompanyID()%>".trim());
		Ext.getCmp("bCompany_name").setValue("<%=emp.getCompanyName()%>".trim());
		Ext.getCmp("aCompany_id").setValue('');
		Ext.getCmp("aCompany_name").setValue('');
	}else{
		Ext.getCmp("bCompany_id").setValue('');
		Ext.getCmp("bCompany_name").setValue('');
		Ext.getCmp("aCompany_id").setValue("<%=emp.getCompanyID()%>".trim());
		Ext.getCmp("aCompany_name").setValue("<%=emp.getCompanyName()%>".trim());
		}
	}

var parentID = "<%=((ContractObj) request
					.getAttribute(WebKeys.Request_Attribute_Name_ContractObject))
					.getContractID()%>";

var childContractID = "<%=DBSchema.Contract.ContractID%>";
var childContractNo = "<%=DBSchema.Contract.ContractNo%>";
var childContractName = "<%=DBSchema.Contract.ContractName%>";
var childAbbreviation = "<%=DBSchema.Contract.Abbreviation%>";
var childSignatureDate = "<%=DBSchema.Contract.SignatureDate%>";
//var childParentID = "<%=DBSchema.Contract.ParentID%>";
var childContractAmt = "<%=DBSchema.Contract.ContractAmt%>";
var childCurrenyGraph = "<%=DBSchema.CurrencyType.CurrenyGraph%>";
var childExchangeRate = "<%=DBSchema.CurrencyType.ExchangeRate%>";
var childTotalAmtByID = "<%=DBSchema.ChargedDetail.TotalAmtByID%>";
var childUnChargedAmtTotal = "<%=DBSchema.ChargedDetail.UnChargedAmtTotal%>";

var childInvoiceTotalAmt = "<%=DBSchema.InvoiceDetail.TotalAmtByID%>";
var childInvoiceUnAccountAmt = "<%=DBSchema.InvoiceDetail.UnAccountAmtTotalSum%>";

var childIncomAmtTotalByGl = "MB";
var childPurAmtTotalByGl = "pay_amt";
var unIncomAmtTotalByGl = "unMB";
var hasChild = false;
var stencilFlag = false;    //是否存在摘要模版
var stencilContent = null;
var stencilKeyWords = null;
var summaryResult = null;
var existOldSummary = '';
</script>
		<title>北京中水科技 X-MIS 综合信息管理系统</title>
	</head>
	<body>
		<div id='main'></div>
		<form action="contractMain.do" method="post" name="contractFrmA">
			<input type="hidden" name="method" value="contract" />
			<input type="hidden" name="contractID" id="contractIDA" />
		</form>
		<%
			if (con.isHadChild()) {
		%>
		<script type="text/javascript">
hasChild = true;
</script>
<!--<jsp:include page="<%=WebKeys.Include_JSP_File_ChildContractList %>" /> -->
<%
	}
%>
<jsp:include page="<%=WebKeys.Include_JSP_File_paymentList %>" />
</body>
</html>
<script type="text/javascript">
var winFlag="";
var contructPersonFlag="";
var comID;
var deptID;
var empType;
var newCompForm = new Ext.form.FormPanel({
    baseCls : 'x-plain',
    anchor : '100%',
	lableWidth:10, 
	defaultType : 'textfield',
	defaults : {
		xtype : "textfield",
		width : 200
	},
	items : [ {
		id : 'companyName',
		fieldLabel : "公司名称",
		inputType : 'text',
		name : 'compName',
		allowBlank : false,
		blankText : "公司名称不能为空",
		msgTarget : 'under'
	}, {
		id : 'shotCompName',
		fieldLabel : "公司简称",
		inputType : 'text',
		name : 'compAbb'
	}, {
		id : 'compPartName',
		fieldLabel : "部门名称",
		inputType : 'text',
		name : 'deptName'
	}, {
		id : 'compDelegate',
		fieldLabel : "公司代表",
		inputType : 'text',
		name : 'empName'
	} ],

	buttons : [
			{
				text : "创建",
				handler : function() {
					validateFunc(
							'<%=WebKeys.New_Company_Simple_Type_Company%>',
							'companyName', newCompForm);
				}
			}, {
				text : "取消",
				handler : function() {
					newCompForm.ownerCt.hide();
				}
			} ]
});
var newCompWin;
var chooseWin;
showCompWindow = function(title, form, win) {
	chooseWin = win;
	if (!newCompWin) {
		newCompWin = new Ext.Window( {
			layout : 'fit',
			modal : true,
			title : title,
			renderTo : Ext.getBody(),
			frame : true,
			plain : true,
			resizable : false,
			buttonAlign : "center",
			//closeAction:"close", 
			//maximizable:true, 
			closable : false,
			bodyStyle : "padding:10px",
			width : 360,
			//height:auto, 
			layout : "form",
			//lableWidth:10, 
			defaults : {
				xtype : "textfield",
				width : 200
			},
			//monitorValid:true,
			listeners : {
				'show' : function() {
					this.findByType('textfield')[0].focus(true, true);
				}
			},
			items : [ form ]

		});
	}
	newCompWin.setTitle(title);
	newCompWin.show();
};
function validateFunc(type, id, chooseForm) {
	if (chooseForm.findById(id).getValue() == "") {
		Ext.Msg.alert("请输名称！", "名称不能为空！");
		chooseForm.findById(id).focus();
		return true;
	}
	if (winFlag == "aDeptWin") {
		comID = document.getElementById('aCompany_id').value;
	}
	if (winFlag == "bDeptWin") {
		comID = document.getElementById('bCompany_id').value;
	}
	if (winFlag == "cDeptWin") {
		var record = stGrid.getSelectionModel().getSelected();
		comID = record.get('st_company_id');
	}
	if (winFlag == "aEmpWin") {
		comID = document.getElementById('aCompany_id').value;
		deptID = document.getElementById('aDept_id').value;
	}
	if (winFlag == "bEmpWin") {
		comID = document.getElementById('bCompany_id').value;
		deptID = document.getElementById('bDept_id').value;
	}
	if (contructPersonFlag == "cEmpWin") {
		comID = document.getElementById('aCompany_id').value;
		deptID = document.getElementById('aDept_id').value;
	}

	chooseForm.getForm().submit(
			{
				url : __cxtPath + '/crmMaint.do?method=dataValidate',
				params : {
					compID : comID,
					deptID : deptID,
					type : type
				},
				method : 'post',
				success : function(form, action) {
					submitForm();
				},
				failure : function(form, action) {
					Ext.Msg.confirm(action.result.messageTitle,
							action.result.message, function(btn, text) {
								if (btn == 'yes') {
									submitForm();
								}
								if (btn == 'no') {
									chooseForm.findById(id).setValue('');
									chooseForm.findById(id).focus();
								}
							});
				}
			});
	function submitForm() {
		chooseForm
				.getForm()
				.submit(
						{
							url : __cxtPath + '/crmMaint.do?method=createNewCompanySimple',
							params : {
								compID : comID,
								deptID : deptID,
								type : type,
								empAddr : Ext.getCmp('empAddr').getValue(),
								empPhone : Ext.getCmp('empPhone').getValue(),
								empPostCode : Ext.getCmp('empPostCode')
										.getValue(),
								empPost : Ext.getCmp('empPost').getValue(),
								empMobilePhone : Ext.getCmp('empMobilePhone')
										.getValue(),
								empEmail : Ext.getCmp('empEmail').getValue(),
								empType : empType
							},
							method : 'post',
							success : function(form, action) {
								empType = '';
								Ext.Msg.alert(action.result.messageTitle,
										action.result.message);
								if (winFlag == "aWin") {
									if (contructPersonFlag != "cEmpWin") {
										Ext.getCmp('aCompany_id').setValue(
												action.result.compID);
										Ext.getCmp('aCompany_name').setValue(
												action.result.compName);
										Ext.getCmp('aDept_id').setValue(
												action.result.deptID);
										Ext.getCmp('aDept_name').setValue(
												action.result.deptName);
										Ext.getCmp('aEmp_id').setValue(
												action.result.empID);
										Ext.getCmp('aEmp_name').setValue(
												action.result.empName);
										Ext.getCmp("delivery_id").setValue("");
										Ext.getCmp("cEmp_name").setValue("");
									}
								}
								if (winFlag == "bWin") {
									Ext.getCmp('bCompany_id').setValue(
											action.result.compID);
									Ext.getCmp('bCompany_name').setValue(
											action.result.compName);
									Ext.getCmp('bDept_id').setValue(
											action.result.deptID);
									Ext.getCmp('bDept_name').setValue(
											action.result.deptName);
									Ext.getCmp('bEmp_id').setValue(
											action.result.empID);
									Ext.getCmp('bEmp_name').setValue(
											action.result.empName);
									Ext.getCmp("delivery_id").setValue("");
									Ext.getCmp("cEmp_name").setValue("");
								}
								if (winFlag == "cDeptWin") {
									var record = stGrid.getSelectionModel().getSelected();
									record.set('st_dept_id',action.result.deptID);
									stDeptName.setValue(action.result.deptName);
								}
								if (winFlag == "cWin") {
									var record = stGrid.getSelectionModel().getSelected();
									record.set('st_company_id',action.result.compID);
									stCompName.setValue(action.result.compName);
									record.set('st_dept_id','');
									stDeptName.setValue('');
									stCompID.setValue(action.result.compID);
								}
								if (winFlag == "aDeptWin") {
									Ext.getCmp('aDept_id').setValue(
											action.result.deptID);
									Ext.getCmp('aDept_name').setValue(
											action.result.deptName);
									Ext.getCmp('aDept_name').specifiedselect = 'dept-old-' + action.result.deptID;
								}
								if (winFlag == "bDeptWin") {
									Ext.getCmp('bDept_id').setValue(
											action.result.deptID);
									Ext.getCmp('bDept_name').setValue(
											action.result.deptName);
								}
								if (winFlag == "aEmpWin"
										&& contructPersonFlag != "cEmpWin") {
									Ext.getCmp('aEmp_id').setValue(
											action.result.empID);
									Ext.getCmp('aEmp_name').setValue(
											action.result.empName);
									Ext.getCmp('aEmp_name').specifiedselect = 'empl-old-' + action.result.empID;
								}
								if (winFlag == "bEmpWin"
										&& contructPersonFlag != "cEmpWin") {
									Ext.getCmp('bEmp_id').setValue(
											action.result.empID);
									Ext.getCmpd('bEmp_name').setValue(
											action.result.empName);
								}
								if (contructPersonFlag == "cEmpWin") {
									Ext.getCmp('delivery_id').setValue(
											action.result.empID);
									Ext.getCmp('cEmp_name').setValue(
											action.result.empName);
									Ext.getCmp('cEmp_name').specifiedselect = 'empl-old-' + action.result.empID;
									Ext.getCmp('deliveryPost').setValue(Ext.getCmp('empPost').getValue());
									Ext.getCmp('deliveryPhone').setValue(Ext.getCmp('empPhone').getValue());
									Ext.getCmp('deliveryEmail').setValue(Ext.getCmp('empEmail').getValue());
									Ext.getCmp('deliveryMobilePhone').setValue(Ext.getCmp('empMobilePhone').getValue());
									if(Ext.getCmp('empPostCode').getValue()==0){
										Ext.getCmp('deliveryPostCode').setValue('');
									}else{
										Ext.getCmp('deliveryPostCode').setValue(Ext.getCmp('empPostCode').getValue());
									}
								}
								if (newCompWin) {
									newCompWin.hide();
								}
								if (newPartWin) {
									newPartWin.hide();
								}
								if (newContactPersonWin) {
									newContactPersonWin.hide();
								}
								if (newConsigneeWin) {
									newConsigneeWin.hide();
								}
								chooseWin.hide();
							},
							failure : function(form, action) {
								Ext.Msg.alert(action.result.messageTitle,
										action.result.message);
							}
						});
	}
	;
}
var newpartForm = new Ext.form.FormPanel( { //新建部门
			baseCls : 'x-plain',
			anchor : '100%',
			lableWidth : 10,
			defaultType : 'textfield',
			defaults : {
				xtype : "textfield",
				width : 200
			},
			items : [ {
				fieldLabel : "部门名称",
				inputType : 'text',
				id : 'deptName',
				allowBlank : false,
				blankText : "部门名称不能为空",
				msgTarget : 'under'
			} ],
			buttons : [
					{
						text : "创建",
						id : 'deptConfermBtn',
						handler : function() {
							validateFunc(
									'<%=WebKeys.New_Company_Simple_Type_Department%>',
									'deptName', newpartForm);

						}
					}, {
						text : "取消",
						handler : function() {
							newpartForm.ownerCt.hide();
						}
					} ]
		});
var newPartWin;
showPartWindow = function(title, form, win) {
	chooseWin = win;
	if (!newPartWin) {
		newPartWin = new Ext.Window( {
			layout : 'fit',
			modal : true,
			title : title,
			renderTo : Ext.getBody(),
			frame : true,
			plain : true,
			resizable : false,
			buttonAlign : "center",
			closable : false,
			bodyStyle : "padding:10px",
			width : 360,
			layout : "form",
			defaults : {
				xtype : "textfield",
				width : 200
			},
			listeners : {
				'show' : function() {
					this.findByType('textfield')[0].focus(true, true);
				}
			},
			items : form
		});
	}
	;
	//alert(newCompWin.getItemId());
	Ext.getCmp('deptName').setValue('');
	newPartWin.setTitle(title);
	newPartWin.show();
};

var newContactPersonForm = new Ext.form.FormPanel(
		{
			baseCls : 'x-plain',
			anchor : '100%',
			lableWidth : 10,
			defaultType : 'textfield',
			defaults : {
				xtype : "textfield",
				width : 200
			},
			items : [ {
				id : 'contactPersonName',
				fieldLabel : "甲方代表姓名",
				inputType : 'text',
				name : 'empName',
				allowBlank : false,
				msgTarget : 'right'
			}, {
				id : 'contactPersonPhone',
				fieldLabel : "办公电话",
				inputType : 'text',
				regex : /^[0-9]+([-]{1}[0-9]+)+$|^[0-9]*$/,
				invalidText : "办公电话格式有误，请检查！",
				name : 'empPhone'
			}, {
				xtype : 'textfield',
				id : 'contactPersonMobilePhone',
				fieldLabel : "手机号",
				regex : /^[0-9]{0,15}$/,
				invalidText : "手机号只能输入数字，请检查！",
				name : 'empMobilePhone'
			}, {
				id : 'contactPersonEmail',
				fieldLabel : "Email",
				inputType : 'text',
				name : 'empEmail'
			}, {
				id : 'contactPersonAddr',
				fieldLabel : "办公地址",
				inputType : 'text',
				name : 'empAddr'
			}, {
				id : 'contactPersonPostCode',
				fieldLabel : "邮政编码",
				regex : /^[0-9]{6}$/,
				invalidText : "邮政编码只能输入6为数字，请检查！",
				name : 'empPostCode'
			}, {
				id : 'contactPersonPost',
				fieldLabel : "职务",
				inputType : 'text',
				name : 'empPost'
			} ],
			buttons : [
					{
						text : "创建",
						id : 'PersonConfermBtn',
						handler : function() {
							validateFunc(
									'<%=WebKeys.New_Company_Simple_Type_ContactPerson%>',
									'contactPersonName', newContactPersonForm);
						}
					}, {
						text : "取消",
						handler : function() {
							newContactPersonForm.ownerCt.hide();
						}
					} ]
		});

var newContactPersonWin;
showContactPersonWin = function(title, win) {
	chooseWin = win;
	contructPersonFlag = '';
	newContactPersonWin = new Ext.Window( {
		layout : 'fit',
		modal : true,
		title : title,
		renderTo : Ext.getBody(),
		frame : true,
		plain : true,
		resizable : false,
		buttonAlign : "center",
		closable : false,
		bodyStyle : "padding:10px",
		width : 360,
		height : 270,
		layout : "form",
		//lableWidth:10, 
		defaults : {
			xtype : "textfield",
			width : 200
		},
		//monitorValid:true,
		listeners : {
			'show' : function() {
				newContactPersonForm.findByType('textfield')[0].focus(true,
						true);
			}
		},
		items : newContactPersonForm
	});

	Ext.getCmp('contactPersonName').setValue('');
	newContactPersonWin.setTitle(title);
	newContactPersonWin.show();
};

var newConsigneeFrom = new Ext.form.FormPanel(
		{
			baseCls : 'x-plain',
			anchor : '100%',
			lableWidth : 10,
			defaultType : 'textfield',
			defaults : {
				xtype : "textfield",
				width : 200
			},
			layout : 'form',
			items : [ {
				id : 'ConsigneeName',
				fieldLabel : "联系人姓名",
				inputType : 'text',
				name : 'empName',
				allowBlank : false,
				msgTarget : 'right'
			}, {
				id : 'empPhone',
				fieldLabel : "办公电话",
				inputType : 'text',
				regex : /^[0-9]+([-]{1}[0-9]+)+$|^[0-9]*$/,
				invalidText : "办公电话格式有误，请检查！",
				name : 'empPhone'
			}, {
				xtype : 'textfield',
				id : 'empMobilePhone',
				fieldLabel : "手机号",
				regex : /^[0-9]{0,15}$/,
				invalidText : "手机号只能输入数字，请检查！",
				name : 'empMobilePhone'
			}, {
				id : 'empEmail',
				fieldLabel : "Email",
				inputType : 'text',
				name : 'empEmail'
			}, {
				id : 'empAddr',
				fieldLabel : "办公地址",
				inputType : 'text',
				name : 'empAddr'
			}, {
				id : 'empPostCode',
				fieldLabel : "邮政编码",
				xtype : 'textfield',
				regex : /^[0-9]{6}$/,
				invalidText : "邮政编码只能输入6为数字，请检查！",
				name : 'empPostCode'
			}, {
				id : 'empPost',
				fieldLabel : "职务",
				inputType : 'text',
				name : 'empPost'
			} ],
			buttons : [
					{
						text : "创建",
						id : 'ConsigneeBtn',
						handler : function() {
							validateFunc(
									'<%=WebKeys.New_Company_Simple_Type_ContactPerson%>',
									'ConsigneeName', newConsigneeFrom);
						}
					}, {
						text : "取消",
						handler : function() {
							newConsigneeFrom.ownerCt.hide();
						}
					} ]
		});

var newConsigneeWin;
showConsigneeWin = function(title, win) {
	chooseWin = win;
	winFlag = '';
	newConsigneeWin = new Ext.Window( {
		layout : 'fit',
		modal : true,
		title : title,
		renderTo : Ext.getBody(),
		frame : true,
		plain : true,
		resizable : false,
		buttonAlign : "center",
		closable : false,
		bodyStyle : "padding:10px",
		width : 360,
		height : 270,
		layout : "form",
		defaults : {
			xtype : "textfield",
			width : 200
		},
		listeners : {
			'show' : function() {
				newConsigneeFrom.findByType('textfield')[0].focus(true, true);
			}
		},
		items : newConsigneeFrom

	});
	empType = "linkMan";
	Ext.getCmp('contactPersonName').setValue('');
	newConsigneeWin.setTitle(title);
	newConsigneeWin.show();
};

//纵向合同来源信息
var st = Ext.data.Record.create( [ {
	name : 'st_id',
	type : 'int'
}, {
	name : 'st_name',
	type : 'string'
}, {
	name : 'st_company_id',
	type : 'int'
}, {
	name : 'st_company_name',
	type : 'string'

}, {
	name : 'st_dept_id',
	type : 'int'
}, {
	name : 'st_dept_name',
	type : 'string'
}, {
	name : 'st_amt',
	type : 'float'
}, {
	name : 'st_currency_id',
	type : 'int'
}, {
	name : 'st_currency_name',
	type : 'string'
}, {
	name : 'st_remark',
	type : 'string'
}, {
	name : 'st_selected',
	type : 'bool'
} ]);

var stStore = new Ext.data.GroupingStore( {
	url : 'contractMain.do?method=getGovernmentContractSourceList',
	remoteSort : false,
	reader : new Ext.data.JsonReader( {
		root : 'rows',
		totalProperty : 'totalCount',
		fields : st
	}),
	sortInfo : {
		field : 'st_id',
		direction : 'ASC'
	}

});

var stEditor = new Ext.ux.grid.RowEditor( {
	saveText : '确认',
	cancelText : '取消',
	clicksToEdit : 2
});

stEditor.on( {
	scope : this,
	beforeedit : function() {
		//stGrid.removeBtn.setDisabled(true);
		//stGrid.newBtn.setDisabled(true);
},

afteredit : function(roweditor, changes, record, rowIndex) {
	
	stForm.getForm().submit( {
		url : 'contractMain.do?method=saveGovernmentContractSource',
		method : 'post',
		params : {
			contractID : '<%=con.getContractID()%>',
			stID : record.get('st_id'),
			stName : record.get('st_name'),
			companyID : record.get('st_company_id'),
			deptID : record.get('st_dept_id'),
			amt : record.get('st_amt'),
			currencyType : Ext.getCmp('st_currency_id').getValue(),
			remark : record.get('st_remark'),
			selected : record.get('st_selected')
		},
		success : function(form, action) {
			Ext.Msg.alert(action.result.messageTitle, action.result.message);

			if (record.get('st_selected') == true) {
				Ext.getCmp('gSourceTypeID').setValue(action.result.stID);
				Ext.getCmp('GSourceTypeName').setValue(action.result.stName);
				stEditor.stopEditing();
				stWin.hide();
			} else {
				if (document.all.gSourceTypeID.value == action.result.stID) {
					document.all.gSourceTypeID.value = 0;
					Ext.getCmp('GSourceTypeName').setValue('');
				}
			}
			stStore.load( {
				params : {
					selectedID : MM_findObj("gSourceTypeID").value
				}
			});
			newFlag = 0;

		},
		failure : function(form, action) {
			Ext.Msg.alert(action.result.messageTitle, action.result.message);
		}
	});
	stGrid.removeBtn.setDisabled(false);
	stGrid.newBtn.setDisabled(false);
},
canceledit : function() {
	if (newFlag == 1) {
		// stEditor.stopEditing();
	var s = stGrid.getSelectionModel().getSelections();
	for ( var i = 0, r; r = s[i]; i++) {
		stStore.remove(r);
	}
	;
	stGrid.getView().refresh();
	newFlag = 0;
}
stGrid.removeBtn.setDisabled(false);
stGrid.newBtn.setDisabled(false);

}
});

var stCheckbox = new Ext.grid.CheckColumn( {
	header : '选中',
	dataIndex : 'st_selected',
	align : 'center',
	width : 35,

	editor : {
		xtype : 'checkbox'
	}
});

var stCompID = new Ext.form.TextField( {
});

var stCompName = new Ext.ux.form.ChooseField( {
	fieldLabel : '来源单位',
	anchor : 50,
	winConfig : {
		title : '来源单位设置'
	},
	baseParamFun : function() {
		winFlag = "cWin";
		return {
			start : 0,
			limit : 20
		};
	},
	chooseUrl : __cxtPath + '/resources/choose/company.json.jsp',
	setValueFun : function(o) {
		var record = stGrid.getSelectionModel().getSelected();
		record.set('st_company_id',o.get(DBSchema.Company.CompanyID));
		record.set('st_company_name',o.get(DBSchema.Company.CompanyID));
		stCompID.setValue(o.get(DBSchema.Company.CompanyID));
		stCompName.setValue(o.get(DBSchema.Company.CompanyName));
		stDeptID.setValue('');
		stDeptName.setValue('');
		record.set('st_dept_id','');
	},
	clearValueFun : function() {
		var record = stGrid.getSelectionModel().getSelected();
		record.set('st_company_id','');
		record.set('st_dept_id','');
		stCompID.setValue('');
		stCompName.setValue('');
		stDeptID.setValue('');
		stDeptName.setValue('');
	}
});

var stDeptID = new Ext.form.TextField( {
});

var stDeptName = new Ext.ux.form.ChooseOrgTreeField( {
	fieldLabel : '来源部门',
	disable : true,
	anchor : '90%',
	winConfig : {
		title : '来源部门设置',
		tbar : [ {
			ref : '../addBtn',
			iconCls : 'add',
			align : 'right',
			text : '新建',
			handler : function() {
				winFlag = "cDeptWin";
				showPartWindow('新部门', newpartForm, this.ownerCt.ownerCt);
			}
		} ]
	},
	singleSelect : true,
	onlyPerson : false,
	rootConfig : function() {
		return {
			id : 'comp-old-' + stCompID.getValue()
		}
	},
	dataUrl : '/orgMaint.do?method=getOrganizationTree',
	setValueFun : function(result) {
		var deptName = '';
		var deptID = '';
		for ( var i = 0; i < result.length; i++) {
			if (i == result.length - 1) {
				deptID = deptID + result[i].id;
				deptName = deptName + result[i].text;
			} else {
				deptID = deptID + result[i].id + ',';
				deptName = deptName + result[i].text + ','
			}
		}
		var record = stGrid.getSelectionModel().getSelected();
		record.set('st_dept_id',deptID.split('-')[2]);
		stDeptID.setValue(deptID.split('-')[2]);
		stDeptName.setValue(deptName);
	},
	clearValueFun : function() {
		var record = stGrid.getSelectionModel().getSelected();
		record.set('st_dept_id','');
		stDeptID.setValue('');
		stDeptName.setValue('');
	},
	listeners : {
		'beforeShowWin' : function() {
			var aCmpId = stCompID.getValue();
			if (!aCmpId || aCmpId.trim() == "" || aCmpId=='0') {
				Ext.Msg.alert("提示", "请先选择来源单位。");
				return false;
			}
		}
	}
})

var newFlag = 0;
var stGrid= new Ext.grid.GridPanel(
		{
			store : stStore,
			height : 350,
			region : 'center',
			id : 'stGrid',
			stripeRows : true, //True表示使用不同颜色间隔行，默认为false
			margins : '0 5 5 5',
			autoExpandColumn : 'st_name',
			plugins : [ stEditor ],
			view : new Ext.grid.GroupingView( {
				markDirty : false
			}),
			tbar : [
					{
						ref : '../newBtn',
						iconCls : 'add',
						text : '新建',
						handler : function() {
							var e = new st( {
								st_name : '',
								st_company_name : '',
								st_dept_name : '',
								st_amt : '',
								st_currencyType : '',
								st_remark : '',
								st_selected : true
							});
							stEditor.stopEditing();
							stStore.insert(0, e);
							stGrid.getView().refresh();
							stGrid.getSelectionModel().selectRow(0);
							stEditor.startEditing(0);
							newFlag = 1;
						}
					},
					'-',
					{
						ref : '../removeBtn',
						iconCls : 'remove',
						text : '删除',
						disabled : true,
						handler : function() {

							var record = stGrid.getSelectionModel()
									.getSelected();

							if (record.get('st_selected') == true) {
								Ext.Msg.alert("提示", record.get('st_name')
										+ "已被选中，不能删除！");
								return false;
							}

							stForm
									.getForm()
									.submit(
											{
												url : 'contractMain.do?method=delGovernmentContractSource',
												method : 'post',
												params : {
													stID : record.get('st_id'),
													stName : record
															.get('st_name')
												},
												success : function(form, action) {
													//alert(action.result.message);
													Ext.Msg
															.alert(
																	action.result.messageTitle,
																	action.result.message);

													stEditor.stopEditing();

													var s = stGrid
															.getSelectionModel()
															.getSelections();
													for ( var i = 0, r; r = s[i]; i++) {
														stStore.remove(r);
													}
													;
													stGrid.getView().refresh();
													newFlag = 0;
												},
												failure : function(form, action) {
													Ext.Msg
															.alert(
																	action.result.messageTitle,
																	action.result.message);
												}
											});

						}
					} ],

			columns : [
					new Ext.grid.RowNumberer(),stCheckbox,
					{
						header : '来源ID',
						dataIndex : 'st_id',
						hidden : true
					},
					{
						id : 'st_name',
						header : '来源名称',
						dataIndex : 'st_name',
						sortable : true,
						editor : {
							xtype : 'textfield',
							allowBlank : false
						}
					},
					{
						header : '来源单位ID',
						dataIndex : 'st_company_id',
						hidden : true,
						editor : stCompID
					},
					{
						header : '来源单位简称',
						dataIndex : 'st_company_name',
						sortable : true,
						editor : stCompName
					},
					{
						header : '来源部门ID',
						dataIndex : 'st_dept_id',
						hidden : true,
						editor : stDeptID
					},
					{
						header : '来源部门简称',
						dataIndex : 'st_dept_name',
						sortable : true,
						editor : stDeptName
					}, {
						xtype : 'numbercolumn',
						header : '资助额度',
						dataIndex : 'st_amt',
						sortable : true,
						editor : {
							xtype : 'numberfield',
							minValue : 0
						}
					}, {
						header : '币种',
						dataIndex : 'st_currency_id',
						sortable : true,
						renderer : function(value, cellmeta, record) {
							return record.get('st_currency_name');
						},
						editor : new com.bitc.xmis.Combos.CurrencyCombo({
							anchor : '85%',
							id : 'st_currency_id'
						})
					}, {
						header : '特点',
						dataIndex : 'st_remark',
						sortable : false,
						editor : {
							xtype : 'textfield'
						}
					} ]
		});

stGrid.getSelectionModel().on('selectionchange', function(sm) {
	stGrid.removeBtn.setDisabled(sm.getCount() < 1);
});

var stForm = new Ext.form.FormPanel( {
	baseCls : 'x-plain',
	anchor : '100%',
	items : [ stGrid ]
});

var stWin;
function showStWin() {
	if (!stWin) {
		stWin = new Ext.Window( {
			modal : true,
			title : "纵向来源信息",
			renderTo : Ext.getBody(),
			frame : true,
			plain : true,
			resizable : false,
			closeAction : "hide",
			closable : true,
			bodyStyle : "padding:10px",
			width : 800,
			height : 400,
			items : [ stForm ]
		});
		
	}
	stStore.load( {
			params : {
				selectedID : MM_findObj("gSourceTypeID").value == null
						|| MM_findObj("gSourceTypeID").value == "" ? 0
						: MM_findObj("gSourceTypeID").value
			}
		});
	stWin.setPosition(180, 100);
	stWin.show();
}
</script>
<script type="text/javascript">
function showGLIncomeList(contractAbb, contractID) {
	showGLIncomeListWindow(contractAbb, contractID);
}
function showGLPurchaseList(contractAbb, contractID,title) {
	showGLPurchaseListWindow(contractAbb, contractID,title);
}
function showGLElseIncomeList(contractAbb, contractID) {
	showGLElseIncomeListWindow(contractAbb, contractID);
}
</script>

