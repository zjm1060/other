/*!
 * Ext JS Library 3.4.0
 * Copyright(c) 2006-2011 Sencha Inc.
 * licensing@sencha.com
 * http://www.sencha.com/license
 */
Ext.onReady(function() {
    var SaleRecord = Ext.data.Record.create([
        {name: '角色名称',   type: 'string'},
        {name: '角色说明',  type: 'string'},
        {name: '部门名称',     type: 'string'},
        {name: '角色排序',     type: 'int'},
        {name: '姓名',    type: 'string'}
    ]);
    
    var myStore = new Ext.data.Store({
        url: 'securityMaint.do?method=getSecurityInfoListExtTest',
        autoLoad: true,
        reader: new Ext.data.JsonReader({
            root: 'rows'
        }, SaleRecord)
    });
    
    var pivotGrid = new Ext.grid.PivotGrid({
        title     : '角色信息',
        width     : 800,
        height    : 1259,
        store     : myStore,
        measure   : '角色名称',
        renderTo  : 'docbody',
        viewConfig: {
            title: 'Sales Performance'
        },
        
        leftAxis: [
            {
                width: 80,
                dataIndex: '角色名称'
            },
            {
                width: 90,
                dataIndex: '角色说明'
            },
            {
                width: 90,
                dataIndex: '部门名称'
            },
            {
                width: 90,
                dataIndex: '姓名'
            }
        ]
//        ,
//        topAxis: [
//            {
//                width: 190,
//                dataIndex: '角色排序'
//            }
//        ]
    });
});