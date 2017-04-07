Ext.define('bitc.report.ReportView', {
    extend: 'Ext.panel.Panel',
    reportToken: null,
    reportCondition: null,
    layout: 'border',
    gridConfig: {},
    columnConfig: {},
    rowNumberShow:true,
    reportTopHidden:true,
    defaultGridWidth : true,
    parameterValues:null,
    requires: ['bitc.report.widget.DeleteButton'],
    /**
     * @cfg {Array}
     * <pre>
     *     [{
     *          text:'',
     *          iconCls:'',
     *          handler:function(record,event){
     *              ...
     *          },
     *          scope:xx
     *      },...]
     *
     * </pre>
     */
    rowContextMenu: null,
    initComponent: function () {
        var me = this;
        Ext.Ajax.request({
            url: __path + '/report/meta.do',
            params: {
                reportToken: this.reportToken
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var action = result.attributes.meta;
                    var store;
                    store = new Ext.data.Store({
                        proxy: {
                            type: 'ajax',
                            url: __path + '/report/data.do',
                            reader: {
                                type: 'json',
                                root: 'rows'
                            },
                            extraParams: {
                                reportToken: this.reportToken,
                                reportCondition: this.reportCondition,
                                parameterValues:this.parameterValues
                            }
                        },
                        groupField:this.groupField,
                        remoteSort: true,
                        fields: action.readerMetaData.fields,
                        autoLoad: true,
                        listeners: {
                            exception: function (proxy, exceptiontype, actiontype, option, response) {
                                if (exceptiontype == 'response') {
                                    var r = Ext.decode(response.responseText);
                                    Ext.Msg.alert(r.title, r.message);
                                }
                            }
                        }
                    });
                    var plugins = [];
                    if (!Ext.isEmpty(action.plugins)) {
                        var ps = action.plugins.split(',');
                        for (var i = 0; i < ps.length; i++) {
                            plugins.push(Ext.create(ps[i]));
                        }
                    }
                    var  dockedItems = [];
                        dockedItems.push({
                            xtype: 'toolbar',
                            items: [
                                '->', {
                                    xtype: 'button',
                                    tooltip: '刷新',
                                    hidden:me.reportTopHidden,
                                    iconCls: 'x-tbar-loading',
                                    handler: function () {
                                        this.grid.getStore().reload();
                                    },
                                    scope: this
                                }, {
                                    xtype: 'button',
                                    text: '修改该报表',
                                    hidden:me.reportTopHidden,
                                    handler: function () {
                                        var data = {
                                            id: 'editReport-' + action.id,
                                            text: '报表修改-' + action.title,
                                            action: 'bitc.report.ReportManage',
                                            config: {
                                                reportId: action.id,
                                                reportToken: action.name
                                            }
                                        };
                                        addWorkTab(data);
                                    },
                                    scope: this
                                }
                            ]
                        });
                    if (action.paging) {
                        dockedItems.push({
                            xtype: 'pagingtoolbar',
                            store: store,
                            dock: 'bottom',
                            displayInfo: true,
                            plugins: [{
                                ptype: 'export2excel',
                                exportName: action.title
                            }]
                        });
                    }
                    if(this.defaultGridWidth==true){
                        Ext.each(action.cm, function (item) {
                            var widthLength = item.header.length * 30;
                            if(widthLength < 100){
                                widthLength = 100;
                            }
                            Ext.apply(item, {'width':widthLength});
                            if (this.columnConfig[item.dataIndex]) {
                                Ext.apply(item, this.columnConfig[item.dataIndex]);
                            }
                        }, this);
                    }else{
                        Ext.each(action.cm, function (item) {
                            Ext.apply(item, {'flex':1});
                            if (this.columnConfig[item.dataIndex]) {
                                Ext.apply(item, this.columnConfig[item.dataIndex]);
                            }
                        }, this);
                    }

                    var grid = this.grid = new Ext.grid.Panel(Ext.apply({
                        columns: this.rowNumberShow?[{xtype: 'rownumberer',width: 40}].concat(action.cm):action.cm,
                        store: store,
                        autoScroll: true,
                        cellTip : true,
                        dockedItems: dockedItems,
                        region: 'center',
                        emptyText: '没有数据',
                        plugins: plugins,
                        viewConfig: {
                            enableTextSelection: true
                        }
                    }, this.gridConfig));
                    if (this.rowContextMenu) {
                        var contextMenuCfg = {
                            items: []
                        };
                        Ext.each(this.rowContextMenu, function (item, index) {
                            contextMenuCfg.items.push({
                                text: item.text,
                                iconCls: item.iconCls,
                                callback: item.handler,
                                callbackScope: item.scope,
                                handler: this.onRowContextMenu,
                                rawConfig: item,
                                scope: this
                            });
                        }, this);
                        var contextMenu = Ext.create('Ext.menu.Menu', contextMenuCfg);
                        this.grid.on('itemcontextmenu', function (g, record, item, index, event) {
                            event.stopEvent();
                            contextMenu.currentRecord = record;
                            contextMenu.showAt(event.getX(), event.getY());
                            return false;
                        });
                    }
                    this.add(grid);
                    this.doLayout();
                    this.fireEvent('gridready', this, grid);
                } else {
                    Ext.Msg.alert(result.title, result.message);
                }
            },
            scope: this
        });
        this.addEvents('gridready');
        this.callParent(arguments);
    },
    reload: function (condition, append) {
        if (condition) {
            if (append) {
                this.grid.getStore().proxy.extraParams['reportCondition'] = this.reportCondition == null ? condition : this.reportCondition + " and (" + condition + ")";
                this.grid.getStore().reload();
            } else {
                this.reportCondition = condition;
                this.grid.getStore().proxy.extraParams['reportCondition'] = condition;
                this.grid.getStore().reload();
            }
        } else {
            this.grid.getStore().proxy.extraParams['reportCondition'] = this.reportCondition;
            this.grid.getStore().reload();
        }
    },
    repStoreReload:function(defaultValue){
        this.grid.getStore().proxy.extraParams['parameterValues'] = defaultValue;
        this.grid.getStore().reload();
    },
    load: function (condition) {

        if (condition) {
            this.reportCondition = condition;
            this.grid.getStore().proxy.extraParams['reportCondition'] = condition;
            this.grid.getStore().load();
        }
    },
    onRowContextMenu: function (widget, event) {
        if (widget.callback) {
            widget.callback.call(widget.callbackScope || this, widget.rawConfig, widget.ownerCt.currentRecord, event);
        }
    }
});
