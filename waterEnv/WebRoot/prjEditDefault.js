function addWorkTab(data) {
    if (Ext.isEmpty(data.action)) {
        Ext.Msg.alert('提示', '该模块正在建设中...');
        return;
    }
    var tab = Ext.getCmp('tab-' + data.id);
    if (tab == null) {
        tab = Ext.create(data.action, Ext.apply(data.config || {}, {
            id: 'tab-' + data.id,
            title: data.text,
            closable: true
        }));
        Ext.getCmp('workspace').add(tab);
    }
    Ext.getCmp('workspace').setActiveTab(tab);
    //return tab;
}
function init(menus) {
    var store = Ext.create('Ext.data.TreeStore', {
        fields: ['id', 'text', 'leaf', 'action', 'config'],
        root: {
            expanded: true,
            children: menus
        }
    });
    var menupanel = Ext.create('Ext.tree.Panel', {
        title: "导航菜单",
        region: "west",
        width: 230,
        collapsible: true,
        minSize: 200,
        maxSize: 300,
        rootVisible: false,
        useArrows: true,
        singleExpand: true,
        store: store,
        scope: this,
        listeners: {
            'itemclick': function (view, record) {
                if (record.data.leaf) {
                    addWorkTab(record.data);
                } else {
                    view.toggle(record);
                }
            }
        }
    });
    Ext.create('Ext.container.Viewport', {
        layout: "border",
        items: [menupanel, {
            id: 'workspace',
            region: 'center',
            layout: "border",
            xtype: 'tabpanel',
            autoScroll: true
            //items : []
        }]
    });
}
Ext.onReady(function () {
    init(bitc.wrenv.config.menu);
});
