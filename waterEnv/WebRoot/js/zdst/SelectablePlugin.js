/**
 * Created by zsz on 2015/11/30.
 */
Ext.define('bitc.wrenv.zdst.SelectablePlugin', {
    init: function (grid) {
        grid.selectRecords = function (comparator) {
            this.selectComparator = comparator;
            if (this.rendered) {
                this.startSelect();
            }
        };
        grid.startSelect = function () {
            var me = this;
            me.getSelectionModel().deselectAll();
            if (me.selectComparator) {
                me.getStore().findBy(function (record) {
                    if (me.selectComparator(record)) {
                        me.getSelectionModel().select(record, true);
                    }
                });
            }
        };
        grid.on('afterrender', function () {
            grid.selectRecords(grid.selectComparator);
        });
    }
});
