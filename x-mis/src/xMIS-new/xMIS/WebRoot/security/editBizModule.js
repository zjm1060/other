function changedNode(id,text,newParentId,oldParentId,isChangeName,isDrag){
	var obj = new Object();
	obj.id = id;   //树节点ID string
	obj.text = text;    //节点文本 string
	obj.newParentId = newParentId;   //拖拽后父节点ID string
	obj.oldParentId = oldParentId;   //拖拽前父节点ID string
	obj.isChangeName = isChangeName; //节点是否改名 boolean
	obj.isDrag = isDrag;  //节点是否被拖拽 boolean
	obj.string = function(){
		return ''+id + ':' + text + ':'+newParentId+':'+oldParentId+':'+isChangeName+':'+isDrag;
	}
	return obj;
}

function changeName(array,obj){
	var added = false;
	for(var i=0;i<array.length;i++){
		if(array[i].id == obj.id){
			array[i] = changedNode(array[i].id,obj.text,array[i].newParentId,array[i].oldParentId,obj.isChangeName,array[i].isDrag);
			added = true;
		}
	}
	if(!added){
		array[array.length] = obj;
	}
}
function dragNode(array,obj){
	var added = false;
	var isOld = obj.id.split('-')[1];
	for(var i=0;i<array.length;i++){
		if(array[i].id == obj.id){
			if(isOld == 1){
				if(array[i].isDrag){
					array[i] = changedNode(array[i].id,array[i].text,obj.newParentId,array[i].oldParentId,array[i].isChangeName,array[i].isDrag);
				}else{
					array[i] = changedNode(array[i].id,array[i].text,obj.newParentId,obj.oldParentId,array[i].isChangeName,obj.isDrag);
				}
			}else{
				array[i] = changedNode(array[i].id,array[i].text,obj.newParentId,'',array[i].isChangeName,obj.isDrag);
			}
			added = true;
		}
	}
	if(!added){
		array[array.length] = obj;
	}
}

function nodeRelationMapping(nodeId,parentId){
	var obj = new Object();
	obj.id = nodeId;
	obj.parentId = parentId;
	return obj;
}
Ext.onReady(function(){
	var selected;   //当前选择的节点
	var changednodesObj = new Array();   
	var changednodes = new Array();   //chuan zhi yongzhege
	var nodesMapping = new Array();
	
	var bizModuleRecord = Ext.data.Record.create([{
			name : 'bizModule-name',
			type : 'string'
		},{
			name : 'bizModule-url',
			type : 'string'
		},{
			name : 'bizModule-description',
			type : 'string'
		},{
			name : 'bizModule-identification',
			type : 'string'
		},{
			name : 'bizModule-px',
			type : 'number'
		},{
			name : 'bizModule-commitStatus',
			type : 'number',
			convert : function(v){
				if(v==0){
					return ''
				}else{
					return v;
				}
			}
		},{
			name : 'bizModule-commitStatusName',
			type : 'string'
		},{
			
		}]);
	
	
	var ColM = new Ext.grid.ColumnModel([{
		header : '子模块名称',
		dataIndex : ''
	},{
		header : '父模块名称',
		dataIndex : ''
	},{
		header : '子模块排序',
		dataIndex : ''
	}]);
	
	var statusStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath + '/securityMaint.do?method=getAllStatusList',
			method : "post"
		}),
		autoLoad : true,
		reader : new Ext.data.JsonReader( {}, Ext.data.Record.create( [ {
				name : 'commitStatus',
				type : 'int',
				mapping : '状态ID'
			}, {
				name : 'commitStatusName',
				type : 'string',
				mapping : '状态名称'
			} ]))
	});
	
	
	//业务模块store
	var bizModuleStore = new Ext.data.Store({
		
	});
	
	var panel2 = new Ext.form.FormPanel({
		//title : '2',
		id : 'panel2',
		hidden : true,
		items : [{
			text : 'aaa'
		},{
			text : 'aaa'
		}]
	});
	
	var panel3 = new Ext.form.FormPanel({
		//title : '3',
		id : 'panel3',
		hidden : true,
		items : [{
			text : 'bbb'
		},{
			text : 'bbb'
		}]
	});
	
	//业务模块Panel
	var bizModule = new Ext.form.FormPanel({
		id : 'panel1',
		height : 450,
		width :850,
		border : true,
		reader : new Ext.data.JsonReader({},bizModuleRecord),
		hidden : true,
		items : [{
			xtype : 'panel',
			height : 450,
			border : true,
			labelAlign : 'right',
			items : [{
				xtype : 'panel',
				width : 450,
				layout : 'form',
				border : false,
				bodyStyle : 'padding : 8,0,0,0',
				defaults : {
					xtype : 'textfield',
					width : 280
				},
				items : [{
				fieldLabel : '名称',
				name : 'bizModule-name',
				id : 'bizModule-name'
			},{
				fieldLabel : 'url',
				xtype : 'textarea',
				name : 'bizModule-url',
				id : 'bizModule-url'
			},{
				fieldLabel : '描述',
				name : 'bizModule-description',
				id : 'bizModule-description'
			},{
				fieldLabel : '图标说明',
				name : 'bizModule-iconDeclare',
				id : 'bizModule-iconDeclare'
			},{
				fieldLabel : '标识',
				name : 'bizModule-identification',
				id : 'bizModule-identification'
			},{
				fieldLabel : '排序序号',
				xtype : 'numberfield',
				name : 'bizModule-px',
				id : 'bizModule-px'
			},{
				fieldLabel : '模块开发状态',
				xtype : 'combo',
				emptyText : '请选择模块状态',
				editable : false,
				store : statusStore,
				mode : 'remote',
				triggerAction : 'all',
				displayField : 'commitStatusName',
				valueField : 'commitStatus',
				name : 'bizModule-commitStatus',
				id : 'bizModule-commitStatus'
			},{
				fieldLabel : '备注',
				xtype : 'textarea',
				name : 'bizModule-remark',
				id : 'bizModule-remark'
			}]
			}
			]
			
		}]
	});
	
	
	
	var mainPanel = new Ext.Panel({
		title : '&nbsp;',
		region : 'center',
		autoScroll : true,
		border : false,
		items : [bizModule,panel2,panel3],
		tbar : [{
			xtype : 'button',
			text : '保存',
			id : 'saveInfo',
			iconCls : 'save',
			handler : function() {
				
			}
		}]
	});
	
	mainPanel.getTopToolbar().hide();
	
	var treeloader = new Ext.tree.TreeLoader({
			dataUrl : String.format('{0}/securityMaint.do?method=securityTreeLoader', __cxtPath),
			baseParams : this.params
	});
	
	var tree = new com.bitc.xmis.SecurityTreePanel({
		title : '权限管理',
		collapsible : true, //是否有右上方展开收缩条
		collapseMode:'mini', //将滚动收缩条模式‘迷你’
		split : true, 
		autoScroll : true, // 滚动条
		animate : true, // 展开,收缩动画
		enableDD : true, //节点是否可拖动
		border : false, //是否有边框
		lines : true, 
		singleExpand : false, //只有一个节点展开
		region : 'west',
		loader : treeloader,
		listeners : {
			'append' : function(tree, parent,node,index){
				if(node.id.substring(0,node.id.indexOf('-'))!="1"){
					var exist = false;
					for(var i=0;i<nodesMapping.length;i++){
						if(nodesMapping[i].id == node.id){
							exist = true;
							break;
						}
					}
					if(!exist){
						nodesMapping[nodesMapping.length] = nodeRelationMapping(node.id,parent.id);
					}
				}
			},
			'click' : function(node){
				//显示tbar
				mainPanel.getTopToolbar().setVisible(true);
				var args = node.id.split('-');
				var level = args[0];        //级别
				var oldFlag = args[1];      //是否是已有数据 1为已有；0为新数据
				var dataID = args[2];       //数据ID
				
				
				
				//先判断是新数据还是已有数据
				if('1' == oldFlag){ //已有数据
					Ext.getCmp("panel1").getForm().load({
						url : __cxtPath + '/securityMaint.do?method=getBizObjById',
						success : function(response,config) {
							var action = Ext.decode(config.responseText);
						},
						failure : function() {
							
						},
						method : "post",
							params : {
							bizID : node.id
						}
					});
					
					
					
				}else if('0' == oldFlag){ //新数据
					
				}else{
					Ext.Msg.alert('异常','出现异常，请重试或与管理员联系！');
				}
		
				if(node.id.substring(0,node.id.indexOf('-'))=='2'){
					bizModule.setVisible(true);
					panel2.setVisible(false);
					panel3.setVisible(false);
					mainPanel.setTitle('编辑业务模块');
					mainPanel.doLayout();
				}
				if(node.id.substring(0,node.id.indexOf('-'))=='3'){
					bizModule.setVisible(false);
					panel2.setVisible(true);
					panel3.setVisible(false);
					mainPanel.setTitle('2');
					mainPanel.doLayout();
				}
				if(node.id.substring(0,node.id.indexOf('-'))=='4'){
					bizModule.setVisible(false);
					panel2.setVisible(false);
					panel3.setVisible(true);
					mainPanel.setTitle('3');
					mainPanel.doLayout();
				}
			},
			'contextmenu' : function(node, e){
				e.preventDefault();//关闭默认的菜单，以避免弹出两个菜单
				//新建类型 1:新建模块;2：新建子模块 ;3：新建业务功能;
				var currentType = node.id.substring(0,node.id.indexOf('-'));
				//每次点击右键时，将‘addModule’,‘deleteModule’置为可见
				Ext.getCmp('addModule').setVisible(true) ;
				Ext.getCmp('deleteModule').setVisible(true) ;
				//更改右键菜单类型，修改可见性及菜单项名称
				var model = tree.getSelectionModel();//获取选择模型 
				selected = model.select(node); //选中并得到右键点击的节点
				var args = selected.id.split('-');
				if(currentType=='1'){
					Ext.getCmp('addModule').setText('新建模块');
					Ext.getCmp('deleteModule').hide();
					
				}else if(currentType=='2'){
					Ext.getCmp('addModule').setText('新建子模块');
					Ext.getCmp('deleteModule').setText('删除模块');
				}else if(currentType=='3'){
					Ext.getCmp('addModule').setText('新建功能');
					Ext.getCmp('deleteModule').setText('删除子模块');
				}else if(currentType=='4'){
					Ext.getCmp('addModule').hide();
					Ext.getCmp('deleteModule').setText('删除功能');
				}
				if(args[1]=='0'){
					if(selected.childNodes.length==''){
						Ext.getCmp('deleteModule').setVisible(true);
					}else{
						Ext.getCmp('deleteModule').setVisible(false);
					}
				}else{
					if(!selected.isLeaf()){
						Ext.getCmp('deleteModule').setVisible(false);
					}
				}
					
				contextMenu.showAt(e.getXY());
			},
			'containercontextmenu' : function(){
				
			},
			'nodedragover' : function(e){
				//e.target --> target node
				//e.dropNode -->current node
				//保证同一级别的拖拽
				var targetNode = e.target;
				var currentNode = e.dropNode;
				//得到2个节点级别
				var targetLevel = parseInt(targetNode.id.substring(0,targetNode.id.indexOf('-')));
				var currentLevel = parseInt(currentNode.id.substring(0,currentNode.id.indexOf('-')));
				//判断是否允许拖拽
				if(currentLevel - targetLevel == 1){
					var isOld = currentNode.id.split('-')[1];
					if(isOld=="1"){
						for(var i = 0;i<nodesMapping.length;i++){
							if(nodesMapping[i].id == currentNode.id){
								dragNode(changednodesObj,changedNode(currentNode.id,currentNode.text,targetNode.id,nodesMapping[i].parentId,false,true));
							}
						}
					}else{
						dragNode(changednodesObj,changedNode(currentNode.id,currentNode.text,targetNode.id,'',false,true));
					}
				}else{
					//不允许跨级拖拽
					e.cancel = true;
				}
				
				
				
			}
			
	}
		
	});
	
	var contextMenu = new Ext.menu.Menu({
		id : 'rootMenu',
		items : [{
			text : "add",
			iconCls : "add",
			id : 'addModule',
			pressed : true,
			handler : function() {
				if(selected.isLeaf()){
					selected.leaf = false;
				}
				var parentLevel = parseInt(selected.id.substring(0,selected.id.indexOf('-')));
				var level = parentLevel + 1;
				var text;
				switch(parentLevel){
					case 1 :  text = '新建模块';
					break;
					case 2 : text = '新建子模块';
					break;
					case 3 : text = '新建功能';
					break;
					default : text = '新建项'
				}
				node = new Ext.tree.TreeNode( {
					text : text,
					id : level + '-0-' + new Date().format("YmdHis")+ parseInt(Math.random()*1000000)
				});
				changednodesObj[changednodesObj.length] = changedNode(node.id,node.text,selected.id,"",false,false);
				selected.expand(); //展开节点 
				selected.appendChild(node); //增加子节点 
			}
		},{
			text : "delete",
			iconCls : "delete",
			id : 'deleteModule',
			pressed : true,
			handler : function() {
				var args = selected.id.split('-');
				if(args[1]=='0'){ //删除未保存数据
					Ext.MessageBox.confirm('信息提示','该项并未保存，请确认是否删除' + selected.text,function(btn) {
						if (btn == 'yes') {
							for(var i=0;i<changednodesObj.length;i++){
								if(changednodesObj[i].id = selected.id){
									changednodesObj.splice(i,1);
									break;
								}
							}
							var pnode = selected.parentNode;
							selected.remove();
							if(!pnode.hasChildNodes()){
								pnode.leaf = true;
							}
						}
					});
				}else{ //已存在数据删除
					Ext.MessageBox.confirm('信息提示','请确认是否删除' + selected.text,function(btn) {
						if (btn == 'yes') {
							Ext.Ajax.request({
								url : __cxtPath + '/securityMaint.do?method=deleteOperation',
								success : function(response,config) {
									var pnode = selected.parentNode;
//									treeloader.load(tree.getRootNode(),function(n){
//										tree.expandPath(pnode.getPath());
//									});
									
								},
								failure : function() {
									//Ext.MessageBox.alert("提示",response.responseText);
								},
								method : "post",
								params : {
									nodeId : selected.id
								}
							});
						}
					});
				}

			}
		},{
			text : "保存全部",
			iconCls : "ok",
			id: "saveAll",
			pressed : true,
			handler : function(){
				alert(changednodesObj.length)
				for(var i= 0;i<changednodesObj.length;i++){
					changednodes[i] = changednodesObj[i].string();
				}
				Ext.Ajax.request({
					url : __cxtPath + '/securityMaint.do?method=saveAllOperation',
					success : function(response,config) {
						treeloader.load(tree.getRootNode(),function(n){
							tree.getRootNode().expand();
						});
						
						selected = null;   //当前选择的节点
						changednodesObj.splice(0,changednodesObj.length);
						changednodes.splice(0,changednodes.length);
						changednodes.splice(0,nodesMapping);
					},
					failure : function() {
						//Ext.MessageBox.alert("提示",response.responseText);
					},
					method : "post",
					params : {
						allNode : changednodes
					}
				});
			}
		}]
	});
	
	var treeEditor = new Ext.tree.TreeEditor(tree, {
		id : 'treeEdit',
		allowBlank : false
	});
	
	treeEditor.on("complete", function(editor, newValue) {
		changeName(changednodesObj,changedNode(editor.editNode.id,newValue,'','',true,false));
	});
	
	//布局管理
	new Ext.Viewport({
		layout : 'border',
		border : false,
		items : [mainPanel,tree]
	});
})