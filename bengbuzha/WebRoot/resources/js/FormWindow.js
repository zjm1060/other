Ext.ns('Ext.ux');
Ext.ux.FormWindow = Ext.extend(Ext.Window, {
			initComponent : function() {
				var formConfig = this.formConfig || {};
				this.form = new Ext.form.FormPanel(formConfig);
				this.items = [this.form];
			}
		});