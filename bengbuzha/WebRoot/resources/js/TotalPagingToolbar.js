
Ext.ns('Ext.ux');

Ext.ux.TotalPagingToolbar = Ext.extend(Ext.PagingToolbar,{
	promptMsg : '',
	updateInfo : function(){
        if(this.displayItem){
            var count = this.store.getCount()-1;
            var dmsg = (this.promptMsg.length > 0) ? this.promptMsg + this.displayMsg : this.displayMsg;
            var msg = (count == 0 ||count == -1) ?
                this.emptyMsg : 
                String.format(
                	dmsg,this.cursor+1, this.cursor+count, this.store.getTotalCount()
                );
            this.displayItem.setText(msg);
        }
    },
    bindStore : function(store, initial){
        var doLoad;
        this.promptMsg = '';
        if(!initial && this.store){
            if(store !== this.store && this.store.autoDestroy){
                this.store.destroy();
            }else{
                this.store.un('beforeload', this.beforeLoad, this);
                this.store.un('load', this.onLoad, this);
                this.store.un('exception', this.onLoadError, this);
            }
            if(!store){
                this.store = null;
            }
        }
        if(store){
            store = Ext.StoreMgr.lookup(store);
            store.on({
                scope: this,
                beforeload: this.beforeLoad,
                load: this.onLoad,
                exception: this.onLoadError
            });
            doLoad = true;
        }
        this.store = store;
        if(doLoad){
            this.onLoad(store, null, {});
        }
    }
});


