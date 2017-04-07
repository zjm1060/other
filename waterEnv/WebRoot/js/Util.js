Ext.applyIf(Array.prototype, {
    indexOf: function (o, from) {
        var len = this.length;
        from = from || 0;
        from += (from < 0) ? len : 0;
        for (; from < len; ++from) {
            if (this[from] === o) {
                return from;
            }
        }
        return -1;
    }
});
function isEmptyObject(e) {
    var t;
    for (t in e)
        return !1;
    return !0
}
Ext.apply(Ext.form.field.VTypes, {
    decimal: function (val, field) {
        var re = new RegExp("^\\d{1," + field.maxNumber1 + "}(\\.\\d{1," + field.maxNumber2 + "})?$");
        return re.test(val);
    },
    decimalText:'请输入正确的数据类型',
    LngLat: function (val,field) {
        var re = new RegExp("^(\\-)?\\d{1,}(\\.\\d{1,})?\\,(\\-)?\\d{1,}(\\.\\d{1,})?$");
        return re.test(val);
    },
    LngLatText:'请输入正确的经纬度',
    float: function (val, field) {
        var re = new RegExp("^\\d{1," + field.maxNumber1 + "}\\.\\d{" + field.maxNumber2 + "," + field.maxNumber2 + "}$");
        return re.test(val);
    },
    floatText:'请输入正确的数据类型'
});
Ext.Loader.setConfig({
    enabled: true
});
Ext.Loader.setPath('Ext.ux', __path + '/extjs/ux');
Ext.Loader.setPath('bitc.wrenv', __path + '/js');
Ext.Loader.setPath('bitc.report', __path + '/js/report');
delete Ext.tip.Tip.prototype.minWidth;
if (Ext.isIE10) {
    Ext.supports.Direct2DBug = true;
}
Ext.Date.use24HourTime = true;
// 处理超时及信息提示
Ext.Ajax.on('requestcomplete', function (conn, response, options) {
    try {
        if (response.responseText.indexOf('<!--tag#login-->') >= 0) {
            Ext.create('bitc.plan.view.LoginWindow', {
                modal: true,
                listeners: {
                    login: function (w) {
                        w.close();
                    }
                }
            }).show();
            return;
        }
        var json = Ext.decode(response.responseText);
        if (json && !json.success && json.timeout) {
            // window.location.href = json.redirectUri;
            Ext.create('bitc.plan.view.LoginWindow', {
                modal: true,
                listeners: {
                    login: function (w) {
                        w.close();
                    }
                }
            }).show();
        }

        if (json && !json.success && json.showMessage) {
            Ext.Msg.alert(json.messageTitle, json.message);
            return false;
        }

    } catch (e) {
    }
});
Ext.define('bitc.wrenv.Util', {

    statics: {
        getPhoneNumber: function (phones, name) {
            if (phones == null || !(phones instanceof Array)
                || phones.length == 0) {
                return null;
            }
            for (i = 0; i < phones.length; i++) {
                if (phones[i].name == name) {
                    return phones[i].number;
                }
            }
        },
        getAreaCode: function (phones, name) {
            if (phones == null || !(phones instanceof Array)
                || phones.length == 0) {
                return null;
            }
            for (i = 0; i < phones.length; i++) {
                if (phones[i].name == name) {
                    return phones[i].areaCode;
                }
            }
        },
        hasRole: function (roleId) {
            if (__roles == null) {
                return false;
            }
            for (var i = 0; i < __roles.length; i++) {
                if (__roles[i].id == roleId) {
                    return true;
                }
            }
            return false;
        },
        /**
         *
         * @param {}
         *            name
         * @param {Function}
         *            callback params: success,habit
         * @param {}
         *            scope
         */
        getHabit: function (name, callback, scope) {
            if (!callback) {
                callback = Ext.emptyFn;
            }
            Ext.Ajax.request({
                method: 'GET',
                url: __path + '/habit/get/' + name + '.do',
                success: function (response) {
                    var result = Ext.decode(response.responseText);
                    if (result.success) {
                        var habit = result.attributes.habit;
                        callback.call(scope || window, true, habit);
                    } else {
                        callback.call(scope || window, false);
                    }
                },
                failure: function () {
                    callback.call(scope || window, false);
                }
            });
        },
        saveHabit: function (name, content, callback, scope) {
            if (!callback) {
                callback = Ext.emptyFn;
            }
            Ext.Ajax.request({
                method: 'POST',
                url: __path + '/habit/save.do',
                params: {
                    name: name,
                    content: content
                },
                success: function (response) {
                    var result = Ext.decode(response.responseText);
                    if (result.success) {
                        callback.call(scope || window, true);
                    } else {
                        callback.call(scope || window, false);
                    }
                },
                failure: function () {
                    callback.call(scope || window, false);
                }
            });
        },
        getUuid: function() {
            var chars = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'.split('');
            var uuid = new Array(36), rnd=0, r;
            for (var i = 0; i < 36; i++) {
                if (i==8 || i==13 ||  i==18 || i==23) {
                    uuid[i] = '-';
                } else if (i==14) {
                    uuid[i] = '4';
                } else {
                    if (rnd <= 0x02) rnd = 0x2000000 + (Math.random()*0x1000000)|0;
                    r = rnd & 0xf;
                    rnd = rnd >> 4;
                    uuid[i] = chars[(i == 19) ? (r & 0x3) | 0x8 : r];
                }
            }
            return uuid.join('');
        }
    }
});
Ext.override(Ext.form.RadioGroup, {
    setValue: function (value) {
        if (Ext.isEmpty(value)) {
            return;
        }
        this.reset();
        this.items.each(function (c) {
            if (value == c.inputValue) {
                c.setValue(true);
            }
        });
    }
});
Ext.override(Ext.form.CheckboxGroup, {
    setValue: function (value) {
        if (!Ext.isArray(value)) {
            value = [value];
        }
        this.reset();
        this.items.each(function (c) {
            if (value.indexOf(c.inputValue) > -1) {
                c.setValue(true);
            }
        });
    },
    getValue: function () {
        var checked = this.getChecked();
        var value = [];
        for (i = 0; i < checked.length; i++) {
            value.push(checked[i].inputValue);
        }
        return value;
    },
    getCheckbox: function (inputValue) {
        var check = false;
        this.items.each(function (c) {
            if (inputValue == c.inputValue) {
                check = c;
                return false;
            }
        });
        return check;
    }
});
Ext.override(Ext.grid.GridPanel, {
    afterRender: function () {
        this.callParent(arguments);
        if (!this.cellTip) {
            return;
        }
        var view = this.getView();
        this.tip = new Ext.ToolTip({
            target: view.el,
            delegate: '.x-grid-cell-inner',
            trackMouse: true,
            renderTo: document.body,
            listeners: {
                beforeshow: function updateTipBody(tip) {
                    if (Ext.isEmpty(tip.triggerElement.innerText)) {
                        return false;
                    }
                    tip.update(tip.triggerElement.innerText);
                }
            }
        });
    }
});
Ext.define('bitc.wrenv.Export2ExcelButton', {
    alias: 'plugin.export2excel',
    constructor: function (config) {
        if (config) {
            Ext.apply(this, config);
        }
    },
    init: function (pagingtoolbar) {
        this.pagingtoolbar = pagingtoolbar;
        var button = new Ext.Button({
            iconCls: 'xls',
            text: this.text || '导出数据',
            menu: {
                items: [{
                    text: '导出本页数据',
                    handler: function () {
                        this.onExport(true);
                    },
                    scope: this
                }, {
                    text: '导出所有数据',
                    handler: function () {
                        this.onExport(false);
                    },
                    scope: this
                }]
            }
        });
        //var index = pagingtoolbar.items.indexOf(pagingtoolbar.refresh);
        pagingtoolbar.insert(11, ['-', button]);
        pagingtoolbar.doLayout();
    },
    onExport: function (currentPage) {
        var grid = this.pagingtoolbar.up('gridpanel');
        Ext.MessageBox.show({
            title: '请稍候',
            msg: '正在导出...',
            width: 300,
            wait: true,
            waitConfig: {
                interval: 200
            },
            closable: false
        });
        var store = grid.store;
        var storeUrl = store.proxy.url;
        //var url = __path + '/export.do?method=excel';
        var url = __path + storeUrl;
        // var columns = grid.getColumnModel().getColumnsBy(function(c) {
        // return !c.hidden;
        // });
        var columns = grid.columns;

        var cms = new Array();
        var fields = {
            items: store.model.getFields(),
            get: function (name) {
                for (var i = 0; i < this.items.length; i++) {
                    var item = this.items[i];
                    if (item.name == name) {
                        return item;
                    }
                }
                return null;
            }
        };
        //grid.getView().beforeColMenuShow();// gridview 拼接表头方法 处理groupheader的情况
        for (var i = 0; i < columns.length; i++) {
            var col = columns[i];
            if (col.hidden === true || col.xtype == 'rownumberer') {// 列是否可见
                continue;
            }
            var header = col.text ? col.text : '';
            //if (!grid.getColumnModel().isMenuDisabled(i)) { // 当列的菜单没有被禁用
            //    header = grid.getView().colMenu.get('col-' + col.id).text; // text
            //    // 是
            //    // group的表头与列名的拼接
            //}
            if (col.dataIndex || col.dataIndex == '') {
                var field = fields.get(col.dataIndex.split('.')[0]);
                var c = {
                    dataIndex: col.dataIndex,
                    header: header,
                    name: field ? field.name : '',
                    mapping: field ? (field.mapping == null
                        ? field.name
                        : field.mapping) : '',
                    width: col.width
                };
                if (col.exportFormatter) { // 当页面的某一列配置exportFormatter定义导出的样式
                    c.exportFormatter = col.exportFormatter.toString();
                } else {
                    c.exportFormatter = null;
                }
                cms.push(c);
            }
        }
        var params = {
            itsaexporttoexcelrequest: true,
            exp_name: this.exportName || (new Date().getTime()),
            start: (store.currentPage-1)*store.pageSize,
            limit: store.pageSize,
            exp_cms: Ext.encode(cms),
            exp_formatter: Ext.encode(this.formatter || [])
        };
        if (store.proxy.extraParams) {
            Ext.apply(params, store.proxy.extraParams);
        }
        if (store.lastOptions && store.lastOptions.params) {
            Ext.apply(params, store.lastOptions.params);
        }

        var sorters = [];
        for (var i = 0; i < store.sorters.length; i++) {
            sorters.push({
                property: store.sorters.items[0].property,
                direction: store.sorters.items[0].direction
            });
        }
        if (sorters.length > 0) {
            params['sorter'] = Ext.encode(sorters);
        }

        if (!currentPage) {
            params['start'] = 0;
            params['limit'] = 0;
        }
        // Create form panel. It contains a basic form that we need for the file download.
        var form = Ext.create('Ext.form.Panel', {
            standardSubmit: true,
            url: url,
            method: 'post'
        });

        // Call the submit to begin the file download.
        form.submit({
            target: '_blank', // Avoids leaving the page.
            params: params
        });
        Ext.defer(function () {
            Ext.destroy(form);
            form = null;
            Ext.Msg.hide();
        }, 1000);
        //
        //Ext.Ajax.request({
        //    url: url,
        //    method: 'post',
        //    params: params,
        //    timeout: 2 * 60 * 1000,
        //    success: function (response, opts) {
        //        var result = Ext.decode(response.responseText);
        //        if (result.success) {
        //            var path = encodeURI(result.attributes.downloadpath);
        //            // window.open(__cxtPath + path);
        //            location.href = __path + path;
        //            Ext.Msg.hide();
        //        } else {
        //            Ext.Msg.alert(result.messageTitle, result.message);
        //        }
        //    },
        //    failure: function () {
        //    }
        //});
    }
});