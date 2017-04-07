Ext.ns('bitc.wrenv');
bitc.wrenv.config = {
    menu: [{
        id: 'zdst',
        text: '基础信息',
        action: '',
        leaf: false,
        iconCls: '',
        children: [{
            id: 'sxhl',
            text: '水系河流',
            action: '',
            leaf: false,
            iconCls: '',
            config: {},
            children: [{
                id: 'heliu',
                text: '河流',
                //action: 'bitc.wrenv.common.CardPanel',
                action: 'bitc.wrenv.zdst.heliu',
                leaf: true,
                iconCls: ''
            }, {
                id: 'hupo',
                text: '湖泊',
                action: 'bitc.wrenv.zdst.hupo',
                leaf: true,
                iconCls: ''
            }]
        }, {
            id: 'swcz',
            text: '水文测站',
            action: '',
            leaf: false,
            iconCls: '',
            children:[{
                id: 'swczitem1',
                text: '雨量站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'PP'
                }
            },{
                id: 'swczitem2',
                text: '蒸发站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'BB'
                }
            },{
                id: 'swczitem3',
                text: '河道水位站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'ZZ'
                }
            },{
                id: 'swczitem4',
                text: '堰闸水文站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'DD'
                }
            },{
                id: 'swczitem5',
                text: '河道水文站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'ZQ'
                }
            },{
                id: 'swczitem6',
                text: '潮位站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'TT'
                }
            },{
                id: 'swczitem7',
                text: '水库水文站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'RR'
                }
            },{
                id: 'swczitem8',
                text: '泵站水文站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'DP'
                }
            },{
                id: 'swczitem9',
                text: '地下水站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'ZG'
                }
            },{
                id: 'swczitem10',
                text: '墒情站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'SS'
                }
            },{
                id: 'swczitem',
                text: '分洪水位站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'ZB'
                }
            },{
                id: 'swczitem12',
                text: '地面沉降量测站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'DC'
                }
            },{
                id: 'swczitem13',
                text: '水质站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'WQ'
                }
            },{
                id: 'swczitem14',
                text: '其他测站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'EL'
                }
            },{
                id: 'swczitem15',
                text: '气象站',
                action: 'bitc.wrenv.zdst.shuiwencezhan',
                leaf: true,
                iconCls: '',
                config:{
                    czType:'MM'
                }
            }]
        }, {
            id: 'slgc',
            text: '水利工程',
            action: '',
            leaf: false,
            iconCls: '',
            children: [{
                id: 'shuiku',
                text: '水库',
                action: 'bitc.wrenv.zdst.shuiku',
                leaf: true,
                iconCls: ''
            },{
                id: 'shuidian',
                text: '水电站',
                action: 'bitc.wrenv.zdst.shuidianzhan',
                leaf: true,
                iconCls: ''
            }, {
                id: 'yindiao',
                text: '引调水工程',
                action: 'bitc.wrenv.zdst.yindiaoshuigongcheng',
                leaf: true,
                iconCls: ''
            }, {
                id: 'shuizha',
                text: '水闸',
                action: 'bitc.wrenv.zdst.shuizhaxinxi',
                leaf: true,
                iconCls: ''
            }, {
                id: 'bengzhan',
                text: '泵站',
                action: 'bitc.wrenv.zdst.bengzhangongcheng',
                leaf: true,
                iconCls: ''
            }, {
                id: 'qvdao',
                text: '渠道工程',
                action: 'bitc.wrenv.zdst.qudaogongcheng',
                leaf: true,
                iconCls: ''
            }, {
                id: 'nongcun',
                text: '农村供水工程',
                action: 'bitc.wrenv.zdst.nongcungongshui',
                leaf: true,
                iconCls: ''
            }, {
                id: 'guanqv',
                text: '灌区',
                action: 'bitc.wrenv.zdst.guanquxinxi',
                leaf: true,
                iconCls: ''
            },{
                id: 'lanheba',
                text: '拦河坝',
                action: 'bitc.wrenv.zdst.lanheba',
                leaf: true,
                iconCls: ''
            }]
        },{
            id: 'qsk',
            text: '取水口',
            action: '',
            leaf: false,
            iconCls: '',
            children: [{
                id : 'dibiaoqushuikou',
                text : '地表水取水口',
                action : 'bitc.wrenv.mgmb.qushuikou',
                leaf: true,
                iconCls: ''
            },{
                id : 'qushuijing',
                text : '地下水取水井',
                action : 'bitc.wrenv.mgmb.qushuijing',
                leaf: true,
                iconCls: ''
            },{
                id: 'quyongshuihu',
                text: '取用水户',
                action: 'bitc.wrenv.mgmb.quyongshuihu',
                leaf: true,
                iconCls: ''
            },{
                id: 'quyongshuicezhan',
                text: '取用水测站',
                action: 'bitc.wrenv.mgmb.quyongshuicezhan',
                leaf: true,
                iconCls: ''
            },{
                id: 'quyongshuijiancedian',
                text: '取用水监测点',
                action: 'bitc.wrenv.mgmb.quyongshuijiancedian',
                leaf: true,
                iconCls: ''
            }]
        },{
            id: 'wry',
            text: '污染源',
            action: '',
            leaf: false,
            iconCls: '',
            children: [{
                id: 'ruhepaiwukou',
                text: '入河排污口',
                action: 'bitc.wrenv.mgmb.ruhepaiwukou',
                leaf: true,
                iconCls: ''
            },{
                id: 'wushuichang',
                text: '污水厂处理',
                action: 'bitc.wrenv.mgmb.wushuichang',
                leaf: true,
                iconCls: ''
            }]
        },{
            id: 'sydsgnq',
            text: '水源地及水功能区',
            action: '',
            leaf: false,
            iconCls: '',
            children: [{
                id: 'zilaishuichang',
                text: '自来水厂',
                action: 'bitc.wrenv.mgmb.zilaishuichang',
                leaf: true,
                iconCls: ''
            },{
                id: 'dibiaoshui',
                text: '地表水',
                action: 'bitc.wrenv.mgmb.dibiaoshui',
                leaf: true,
                iconCls: ''
            }, {
                id: 'dixiashui',
                text: '地下水',
                action: 'bitc.wrenv.mgmb.dixiashui',
                leaf: true,
                iconCls: ''
            },{
                id: 'shuigongnengqu',
                text: '水功能区',
                action: 'bitc.wrenv.mgmb.shuigongnengqu',
                leaf: true,
                iconCls: ''
            },{
                id: 'hedaoduanmian',
                text: '河道断面',
                action: 'bitc.wrenv.mgmb.hedaoduanmian',
                leaf: true,
                iconCls: ''
            }]
        }]
    }, {
        id: 'szjc',
        text: '监测信息',
        action: '',
        leaf: false,
        iconCls: '',
        children : [{
            id: 'dmszjc',
            text: '断面监测',
            action: 'bitc.wrenv.szpj.dmshuizhijiance',
            leaf: true,
            iconCls: ''
        },{
            id: 'pwkszjc',
            text: '排污口水质监测',
            //action : '',
            action: 'bitc.wrenv.szpj.pwkshuizhijiance',
            leaf: true,
            iconCls: ''
        },{
            id: 'wrwrhl',
            text: '排污口污染物入河量',
            //action : '',
            action: 'bitc.wrenv.szpj.wrwruheliang',
            leaf: true,
            iconCls: ''
        }]
    },{
        id: 'sdl',
        text: '水动力信息',
        action: '',
        leaf: false,
        iconCls: '',
        children : [{
            id: 'sdldmxx',
            text: '断面基本信息',
            action: 'bitc.wrenv.sdl.dmxinxi',
            leaf: true,
            iconCls: ''
        },{
            id: 'sdldmfc',
            text: '断面水力参数',
            action: 'bitc.wrenv.sdl.dmfenceng',
            leaf: true,
            iconCls: ''
        }]
    },{
        id: 'tfsj',
        text: '应急管理',
        action: '',
        leaf: false,
        iconCls: '',
        children : [{
            id: 'tfsj_wrw',
            text: '污染物',
            action: 'bitc.wrenv.tfsj.wrw',
            leaf: true,
            iconCls: ''
        },{
            id: 'tfsj_fxy',
            text: '污染风险源',
            action: 'bitc.wrenv.tfsj.fxy',
            leaf: true,
            iconCls: ''
        },{
            id: 'tfsj_zhuangjia',
            text: '专家库',
            action: 'bitc.wrenv.tfsj.zhuanjia',
            leaf: true,
            iconCls: ''
        },{
            id: 'tfsj_yuan',
            text: '应急预案',
            action: 'bitc.wrenv.tfsj.plan',
            leaf: true,
            iconCls: ''
        },{
            id: 'tfsj_fagui',
            text: '应急法规',
            action: 'bitc.wrenv.tfsj.laws',
            leaf: true,
            iconCls: ''
        }]
    }, {
        id: 'sys',
        text: '系统管理',
        action: '',
        leaf: false,
        iconCls: '',
        children: [{
            id: 'zidian',
            text: '字典数据库',
            action: 'bitc.wrenv.sys.zidian',
            leaf: true,
            iconCls: ''
        }, {
            id: 'yonghu',
            text: '用户信息数据库',
            action: 'bitc.wrenv.sys.system',
            leaf: true,
            iconCls: ''
        }, {
            id: 'ziyuanguanliqi',
            text: '系统资源监视器',
            action: 'bitc.wrenv.sys.ziyuanguanliqi',
            leaf: true,
            iconCls: ''
        }]
    }//,{
        // id : 'dataSourceManage',
        // text: '报表管理',
        // action: '',
        // leaf: false,
        // iconCls: '',
        // children:[{
        //     id: 'browseRepData',
        //     text: '数据源管理',
        //     action: 'bitc.report.ReportView',
        //     leaf: true,
        //     iconCls: '',
        //     config : {
        //         reportToken: 'browseDataSource'
        //     }
        // },{
        //     id: 'browseRep',
        //     text: '报表管理',
        //     action: 'bitc.report.ReportView',
        //     leaf: true,
        //     iconCls: '',
        //     config: {
        //         reportToken: 'reports'
        //     }
        // }, {
        //     id: 'browseForm',
        //     text: '表单管理',
        //     action: 'bitc.report.ReportView',
        //     leaf: true,
        //     iconCls: '',
        //     config: {
        //         reportToken: 'forms'
        //     }
        // }, {
        //     id: 'testView',
        //     text: '测试报表',
        //     action: 'bitc.report.TestView',
        //     leaf: true,
        //     iconCls: ''
        // }, {
        //     id: 'testGrid',
        //     text: '测试新报表',
        //     action: 'bitc.report.testGrid',
        //     leaf: true,
        //     iconCls: ''
        // }]
    //}
    ]
};