Ext.define('bitc.report.plugins.GroupgridPlugin', {
    init: function (grid) {
        //grid.getStore().group( '测站类别ID');
        grid.getStore().group(grid.groupField);
    }
});