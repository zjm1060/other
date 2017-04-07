<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.util.PropertyFilter"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
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
	boolean decisionFlag = emp.hasDecisionRole();
	boolean readOnly = emp.isReadOnly();
	String delimiter = (String)request.getAttribute("delimiter");
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
<script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/conInfoObjCom.js"></script>
       <script type="text/javascript" src="<%=path%>/resources/js/MultiSelect.js"></script>
    <script type="text/javascript" src="<%=path %>/crm/AddressChooseField.js"></script>
<script type="text/javascript" src="<%=path%>/device/DeviceInGridPanel.js"></script>
<script type="text/javascript" src="<%=path%>/device/deviceIn.js"></script>
<style type="text/css">
body .x-panel {
	margin-bottom: 0px;
}
.x-tree-node-icon
{
display:none !important;
}
</style>
<script type="text/javascript">
	var delimiter ="<%=delimiter%>";
	var decisionFlag = <%=decisionFlag%>;
	var readOnly = <%=readOnly%>;
</script>
<script type="text/javascript" src="<%=path%>/contract/editContractOut.js"></script>
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
var RegisterID = "<%=con.getRegisterID()%>";
var lastModificator = "<%=con.getLastModificator()%>";
var contractOpType = "<%=con.getContractOpType()%>";
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
</script>
<title>北京中水科技 X-MIS 综合信息管理系统</title>
</head>
<body>
<div id='main'></div>
<form action="contractMain.do" method="post" name="contractFrmA">
  		<input type="hidden" name="method" value="contract" />
  		<input type="hidden" name="contractID" id="contractIDA"/>
</form>
</body>
</html>
<script type="text/javascript">
var winFlag="";
var contructPersonFlag="";
var comID;
var deptID;
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
		    validateFunc('<%=WebKeys.New_Company_Simple_Type_Company%>','companyName', newCompForm);
		}}, {
					text : "取消",
					handler : function() {
						newCompForm.ownerCt.hide();
					}
				} ]
	});
	var newCompWin;
	var chooseWin;
	showCompWindow = function(title, form ,win) {
		chooseWin=win;
		if (!newCompWin) {
			newCompWin = new Ext.Window({
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
				items : [form]

			});
		}
		newCompWin.setTitle(title);
		newCompWin.show();
	};
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
	         url : __cxtPath+'/crmMaint.do?method=dataValidate',
			 params : {
			 	compID: comID,
			 	deptID:deptID,
			 	type: type
			 },
	         method : 'post',
	         success : function(form, action) {
	          	submitForm();
	         },
         	failure : function(form, action) {
				Ext.Msg.confirm(action.result.messageTitle,action.result.message, function(btn, text){
				    if (btn == 'yes'){
				        submitForm();
				    }
				    if(btn=='no'){
						chooseForm.findById(id).setValue('');
			         	chooseForm.findById(id).focus();
				    }
			});
       	}
	});
		function submitForm(){
			   chooseForm.getForm().submit({
		         url : __cxtPath+'/crmMaint.do?method=createNewCompanySimple',
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
		            	Ext.getCmp('aCompany_id').setValue(action.result.compID);
		            	Ext.getCmp('aCompany_name').setValue(action.result.compName);
		            	Ext.getCmp('aDept_id').setValue(action.result.deptID);
		            	Ext.getCmp('aDept_name').setValue( action.result.deptName);
		            	Ext.getCmp('aEmp_id').setValue( action.result.empID);
		            	Ext.getCmp('aEmp_name').setValue(action.result.empName);
		            	Ext.getCmp("delivery_id").setValue("");
		            	Ext.getCmp("cEmp_name").setValue("");
				    }
				    if(winFlag == "bWin"){
				    	Ext.getCmp('bCompany_id').setValue(action.result.compID);
		            	Ext.getCmp('bCompany_name').setValue(action.result.compName);
		            	Ext.getCmp('bDept_id').setValue(action.result.deptID);
		            	Ext.getCmp('bDept_name').setValue( action.result.deptName);
		            	Ext.getCmp('bEmp_id').setValue( action.result.empID);
		            	Ext.getCmp('bEmp_name').setValue(action.result.empName);
		            	Ext.getCmp("delivery_id").setValue("");
		            	Ext.getCmp("cEmp_name").setValue("");
				     }	
		         	if(winFlag == "aDeptWin"){
		         		Ext.getCmp('aDept_id').setValue(action.result.deptID);
		         		Ext.getCmp('aDept_name').setValue(action.result.deptName);
		           	}
		           	if(winFlag == "bDeptWin"){
		           		Ext.getCmp('bDept_id').setValue(action.result.deptID);
		           		Ext.getCmp('bDept_name').setValue(action.result.deptName);
		           		
		           		Ext.getCmp('bDept_name').specifiedselect = 'dept-old-'+ action.result.deptID;
		           	}	
		           	if(winFlag == "aEmpWin" && contructPersonFlag !="cEmpWin" ){
		           		Ext.getCmp('aEmp_id').setValue(action.result.empID);
		           		Ext.getCmp('aEmp_name').setValue(action.result.empName);
		   	        	
		           	}
		           	if(winFlag == "bEmpWin" && contructPersonFlag !="cEmpWin"){
		           		Ext.getCmp('bEmp_id').setValue(action.result.empID);
		           		Ext.getCmp('bEmp_name').setValue(action.result.empName);
		           		
		           		Ext.getCmp('bEmp_name').specifiedselect = 'empl-old-'+ action.result.empID;
		           	}	
		           	if(contructPersonFlag == "cEmpWin"){
		           		Ext.getCmp('delivery_id').setValue(action.result.empID);
		           		Ext.getCmp('cEmp_name').setValue(action.result.empName);
		           	}
		           	if(newCompWin){
		           		newCompWin.hide();
		           	}
		           	if(newPartWin){
		           		newPartWin.hide();
		           	}
		           	if(newContactPersonWin){
		           		newContactPersonWin.hide();
		           	}
		           	chooseWin.hide();
		         },
			       	failure : function(form, action) {
			        	Ext.Msg.alert(action.result.messageTitle,action.result.message);
			       	}
				}); 
		 	};
}
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
	var	newPartWin;	    
	 showPartWindow = function(title, form,win) {
		chooseWin=win;
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
	var newContactPersonForm = new Ext.form.FormPanel({
	    baseCls : 'x-plain',
	    anchor : '100%',
		lableWidth:10, 
		defaultType : 'textfield',
		defaults:{xtype:"textfield",width:200}, 
		items:[{
			id:'contactPersonName',
			fieldLabel:"乙方代表姓名", 
			inputType:'text', 
			name:'empName', 
			allowBlank:false, 
			msgTarget:'right'
		},{
			id:'empPhone',
			fieldLabel:"办公电话", 
			inputType:'text', 
			regex : /^[0-9]+([-]{1}[0-9]+)+$|^[0-9]*$/,
			invalidText : "办公电话格式有误，请检查！",
			name:'empPhone'
		},{
			xtype : 'numberfield',
			id:'empMobilePhone',
			fieldLabel:"手机号", 
			regex : /^[0-9]{0,15}$/,
			invalidText : "手机号只能输入数字，最多15位，请检查！",
			name:'empMobilePhone'
		},{
			id:'empEmail',
			fieldLabel:"Email", 
			inputType:'text', 
			name:'empEmail'
		},{
			id:'empAddr',
			fieldLabel:"办公地址", 
			inputType:'text', 
			name:'empAddr'
		},{
			id:'empPostCode',
			fieldLabel:"邮政编码", 
			regex : /^[0-9]{6}$/,
			invalidText : "邮政编码只能输入6为数字，请检查！",
			xtype:'numberfield', 
			name:'empPostCode'
		},{
			id:'empPost',
			fieldLabel:"职务", 
			inputType:'text', 
			name:'empPost'
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
	var newContactPersonWin;
    function showContactPersonWin(title,win){
    	chooseWin=win;
		  	newContactPersonWin=new Ext.Window({ 
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
				height:270, 
				layout:"form", 
				//lableWidth:10, 
				defaults:{xtype:"textfield",width:200}, 
				//monitorValid:true,
				listeners: { 'show': function() { newContactPersonForm.findByType('textfield')[0].focus(true, true);  } },
				items: newContactPersonForm 
				
			}) ;
    	Ext.getCmp('contactPersonName').setValue('');
		newContactPersonWin.setTitle(title);
		newContactPersonWin.show();
    }; 
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
                                 					Ext.getCmp('gSourceTypeID').setValue(action.result.stID);
                                 					Ext.getCmp('GSourceTypeName').setValue(action.result.stName);
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
                                      
                                          
                                 	var newFlag=0;				
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
								function showStWin(){
									if(!stWin){
                                 			stWin=new Ext.Window({ 
                                 				modal: true,
                                 				title:"纵向来源信息", 
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
                                 			});
                                         }
                                     	stWin.setPosition(240, 160);    
                                 		stWin.show(); 
                                 		stStore.load({
                                 			params:{
                                 				selectedID: MM_findObj("gSourceTypeID").value==null||MM_findObj("gSourceTypeID").value==""?0: MM_findObj("gSourceTypeID").value
                                 			}
                                 		}); 

                                     }
</script>

