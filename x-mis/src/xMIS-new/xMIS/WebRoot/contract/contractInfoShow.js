showContractWin = function(val) {
	var ContractInfoPanel = new Ext.Panel({
				width : 584,
				aotoHeight : true,
				// autoScroll : true,
				monitorResize : false,
				autoHeight : true,
				listeners : {
					'expand' : function() {
					}
				},
				frame : true,
				plain : true,
				autoWidth : false,
				autoLoad : {
					url : __cxtPath
							+ '/contractMain.do?method=contract&contractID='
							+ val + '&showType=readonly&d=' + Math.random(),
					scripts : true,
					scope : this
				}// &showType=readonly'}
			});
	var wi = new Ext.Window({
				modal : true,
				title : "合同信息",
				// renderTo:Ext.getBody(),
				frame : true,
				plain : true,
				resizable : true,
				closable : true,
				autoDestroy : true,
				// bodyStyle:"padding:10px",
//				width : 522,
				width : 617,
				height : 500,
				autoScroll : true,
				/*
				 * tbar: [{ ref: '../okBtn', iconCls: 'ok', text: '确定', handler:
				 * function(){ this.scrollTo('top',10,true); //
				 * alert(ContractInfoPanel.height); } }],
				 */
				listeners : {
					'beforedestroy' : function() {

						ContractInfoPanel.destroy();
					},
					'expand' : function() {

						wi.scrollTo(0, 100);
					}
				},
				// autoWidth :true,
				// autoHeight: true,
				// boxMaxHeight:800,
				// contentEl: 'abx',
				// autoLoad: {url:
				// 'contractMain.do?method=contract&contractID='+val+'&showType=readonly&d='+Math.random(),
				// scripts: true, scope: this}//&showType=readonly'}
				items : [ContractInfoPanel]
			});
	// wi.setPosition(420, 5);
	wi.show();
}
