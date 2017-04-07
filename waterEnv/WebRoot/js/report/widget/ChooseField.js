/**
 * @class Ext.ux.form.ChooseField
 * @extends Ext.form.TwinTriggerField 选择组件
 * @author bomwu
 * @version 1.0
 */
Ext.define('bitc.report.widget.ChooseField', {
    extend: 'Ext.form.TwinTriggerField',
    alias: 'widget.repchoosefield',
    reportToken: null,
    gridConfig: null,
    hideTrigger1: true,
    initComponent: function () {
        this.callParent(arguments);
        this.addEvents(
            /**
             * @event beforeshowwin 选择窗口显示前触发
             * @param {Ext.ux.form.ChooseField}
             *            choosefield this field
             */
            'beforeshowwin',
            /**
             * @event aftershowwin 选择窗口显示后触发
             * @param {Ext.ux.form.ChooseField}
             *            choosefield this field
             */
            'aftershowwin');
    },
    // private
    onRender: function (ct, position) {
        this.callParent(arguments);
        this.triggerCell.item(0).setDisplayed(!this.hideTrigger1);
        if (this.tooltip && this.tooltip.text)
            new Ext.ToolTip({
                target: this.id,
                trackMouse: false,
                draggable: false,
                maxWidth: 200,
                minWidth: 100,
                title: this.tooltip.title,
                html: this.tooltip.text
            });
    },

    onlySearch: false,
    validationEvent: false,
    validateOnBlur: false,
    trigger1Cls: Ext.baseCSSPrefix + 'form-clear-trigger',
    trigger2Cls: Ext.baseCSSPrefix + 'form-search-trigger',
    editable: false,
    /**
     * @property hasSearch 是否已经查询
     * @type Boolean
     */
    hasSearch: false,
    // private
    hasInit: false,
    /**
     * @cfg {Function} setValueFun 返回选择结果的回调函数
     */
    setValueFun: Ext.emptyFn,
    /**
     * @cfg {Function} clearValueFun 清除选择结果的回调函数
     */
    clearValueFun: Ext.emptyFn,
    /**
     * @cfg {String} chooseUrl 选择资源的URL，此URL请求的Response应该是一个JSON对象
     */
    /**
     * @cfg {Function} baseParamFun
     *      用于获取数据基础参数的回调函数，返回值将会用于Store的baseParams属性 应该返回一个JSON对象 例：
     *
     * <pre><code>
     *  function() {
			 * 		return {
			 * 			contractTypeID : Ext .getCmp('contractType').getValue()
			 * 		};
			 * }
     * </code></pre>
     *
     */
    baseParamFun: function () {
        return {};
    },
    /**
     * @cfg {Function} paramFun 用于获取数据参数的回调函数应该返回一个JSON对象 例：
     *
     * <pre><code>
     *  function() {
			 *  	return {
			 *  		start : 0,
			 *  		limit : 20
			 *  	};
			 *  }
     * </code>
     * </pre>
     */
    paramFun: function () {
        return {};
    },
    /**
     * @cfg {Object} winConfig 用于配置选择窗口
     */
    search: function () {
        if (this.searchField) {
            this.searchField.onTrigger2Click();
            this.hasSearch = true;
        }
    },

    onTrigger1Click: function () {
        this.clearValueFun.call(this.scope || window);
        this.triggerCell.item(0).setDisplayed(false);
    },

    onTrigger2Click: function () {
        this.triggerCell.item(0).setDisplayed(true);
        if (this.disabled) {
            return;
        }
        if (!this.fireEvent('beforeshowwin', this)) {
            return;
        }
        if (this.hasInit) {
            this.window.show();
            this.fireEvent('aftershowwin', this);
            return;
        }
        this.initWindow();
    },
    // private
    initWindow: function () {

        var me = this;
        var grid = Ext.create('bitc.report.ReportView', {
            region: 'center',
            reportToken: this.reportToken,
            gridConfig: this.gridConfig
        });
        var wintbar = [{
            xtype: 'button',
            text: '选定',
            scope: this,
            handler: function () {
                var record = grid.grid.getSelectionModel().getSelection();
                me.setValueFun.call(this.scope || window, record[0]);
                this.triggerCell.item(0).setDisplayed(true);
                this.window.hide();
            }
        }, '-', {
            text: '取消',
            scope: this,
            handler: function () {
                this.window.hide();
            }
        }];
        if (me.searchBar) {
            var searchConfig = me.searchFieldConfig;
            wintbar.push('-');
            searchConfig.reportView = grid;
            var searchField = new Ext.ux.InklingSearchField(searchConfig);
            wintbar.push(searchField);
        }
        var windowConfig = {
            modal: true,
            items: [grid],
            tbar: {
                xtype: 'toolbar',
                items: wintbar
            }
        };
        Ext.apply(windowConfig, me.winConfig||{});
        this.window = new Ext.ChooseWindow(windowConfig);
        this.hasInit = true;
        this.window.show();
        this.fireEvent('aftershowwin', this);
    }
});
Ext.ChooseWindow = Ext.extend(Ext.Window, {
    initComponent: function () {
        this.callParent(arguments);
    },
    width: 500,
    height: 425,
    closeAction: 'hide',
    layout: 'border',
    callback: function () {
    }
});
