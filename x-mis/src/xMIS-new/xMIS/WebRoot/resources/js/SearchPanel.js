Ext.ns('Ext.ux');
/**
 * @constructor
 * @class Ext.ux.SearchPanel
 * @extends Ext.Panel
 */
Ext.ux.SearchPanel = Ext.extend(Ext.Panel, {
	initComponent : function() {
		var comboConfig = {
			id : this.id + '_combo'
		};
		Ext.apply(comboConfig, this.comboConfig || {});
		this.combo = new Ext.form.ComboBox(comboConfig);
		var searchConfig = {
			id : this.id + '_searchfield',
			panel : this,
			paramName : comboConfig.value
		};
		Ext.apply(searchConfig, this.searchConfig || {});
		this.searchField = new Ext.ux.form.SearchField(searchConfig);
		this.searchField.store.baseParams[this.searchTypeName] = comboConfig.value;
		var combo = this.combo;
		var searchField = this.searchField;
		this.items = [combo, searchField];
		Ext.ux.SearchPanel.superclass.initComponent.call(this);
		this.combo.on('select', function(combo, record, index) {
					this.searchField.paramName = record
							.get(this.combo.valueField);
					this.searchField.store.baseParams[this.searchTypeName] = record
							.get(this.combo.valueField);
				}, this);
	},
	layout : 'column',
	border : false,
	searchTypeName : 'selIds'
});
Ext.reg('searchpanel', Ext.ux.SearchPanel);
