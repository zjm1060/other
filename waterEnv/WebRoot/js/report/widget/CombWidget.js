Ext.define('bitc.report.widget.CombWidget', {
    alias: 'widget.repcombo',
    extend: 'Ext.form.field.ComboBox',
    anchor: '90%',
    valueField: 'valueColumn',
    displayField: 'displayColumn',
    editable: false,
    triggerAction: "all",
    msgTarget: "side",
    /**
     * @cfg {String} valueColumn
     * 鍊煎垪
     */
    valueColumn: null,
    /**
     * @cfg {String} displayColumn
     *显示列
     */
    displayColumn: null,
    /**
     * @cfg {String} tableName
     *
     */
    tableName: null,
    /**
     * @cfg {String} condition
     * 取值条件
     */
    condition: null,
    /**
     * @cfg {String} dbName
     * 数据源
     *
     */
    dbName: null,
    initComponent: function () {
        this.store = new Ext.data.Store({
            proxy: {
                type: 'ajax',
                url: __path + '/form/combo.do',
                reader: {
                    type: 'json',
                    root: 'rows',
                    idProperty: 'valueColumn',
                    totalProperty: 'total'
                },
                extraParams: {
                    dbName: this.dbName,
                    valueColumn: this.valueColumn,
                    displayColumn: this.displayColumn,
                    tableName: this.tableName,
                    condition: this.condition
                }
            },
            autoLoad: true,
            fields: ["valueColumn", "displayColumn"]
            //,
            //listeners:{
            //    load:function(){
            //        this.setValue(this.value);
            //    },
            //    scope:this
            //}
        });
        this.callParent(arguments);
    }
})
;
