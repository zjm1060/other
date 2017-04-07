/*
 * ! Ext JS Library 3.2.0 Copyright(c) 2006-2010 Ext JS, Inc.
 * licensing@extjs.com http://www.extjs.com/license
 */
/**
 * @class Ext.ux.form.SearchField
 *        查询输入框。可用于对Grid等Store的查询。用户输入条件后，点击查询按扭或者使用回车键时，该组件将在Store的baseParams中加入用户输入的查询条件，并调用Store的reload方法：
 *
 * <pre><code>
 *            ...
 *            var o = {
 * 				start : 0
 * 			};
 *        this.store.baseParams = this.store.baseParams || {};
 *        this.store.baseParams[this.paramName] = v;
 *        this.store.reload({
 * 				params : o
 * 			});
 *            ...
 * </code>
 * v 为用户输入的值
 * </pre>
 *
 * @extends Ext.form.TwinTriggerField
 */
Ext.define('Ext.ux.form.SearchField', {
    extend: 'Ext.form.TwinTriggerField',
    /**
     * @cfg {Ext.data.Store} store 绑定查询的Store
     */
    initComponent: function () {
        this.callParent(arguments);
        this.on('specialkey', function (f, e) {
            if (e.getKey() == e.ENTER) {
                this.onTrigger2Click();
            }
        }, this);
    },

    validationEvent: false,
    validateOnBlur: false,
    trigger1Cls: Ext.baseCSSPrefix + 'form-clear-trigger',

    trigger2Cls: Ext.baseCSSPrefix + 'form-search-trigger',
    width: 180,
    hasSearch: false,
    /**
     * @cfg {String} paramName
     *      相当于Field的name属性，指定该查询发送到后台时使用的名称。（默认为"query"，即后台使用request.getParameter("query")可获得用户输入的查询条件）
     */
    paramName: 'query',

    afterRender: function(){
        this.callParent();
        this.triggerCell.item(0).setDisplayed(false);
    },
    onTrigger1Click: function () {
        if (this.hasSearch) {
            this.setValue('');
            var o = {
                start: 0
            };
            this.store.getProxy().extraParams = this.store.getProxy().extraParams || {};
            this.store.getProxy().extraParams[this.paramName] = '';
            this.store.reload({
                params: o
            });
            this.triggerCell.item(0).setDisplayed(false);
            this.hasSearch = false;
        }
    },

    onTrigger2Click: function () {
        var v = this.getRawValue();
        if (v.length < 1) {
            this.onTrigger1Click();
            return;
        }
        var o = {
            start: 0
        };
        this.store.getProxy().extraParams = this.store.getProxy().extraParams || {};
        this.store.getProxy().extraParams[this.paramName] = v;
        this.store.reload({
            params: o
        });
        this.hasSearch = true;
        this.triggerCell.item(0).setDisplayed(true);
    }
});