Ext.define('bitc.wrenv.sys.ziyuanguanliqi', {
    extend: "Ext.panel.Panel",
    autoScroll: true,
    initComponent: function () {
        this.mainDivId1 = this.getId() + '-main1';
        this.mainDivId2 = this.getId() + '-main2';
        this.html = "<div id='" + this.mainDivId1 + "'></div><div id='"
            + this.mainDivId2 + "'></div>";
        this.callParent(arguments);
    },
    afterRender: function () {
        this.callParent(arguments);
        this.drawEcharts();
    },
    drawEcharts: function () {
        this.appendDiv(0);
        this.b_appendDiv(1);
        for (var i = 2; i < 4; i++) {
            this.s_appendDiv(i);
        }
        this.newGauge();
        this.newPie();
        this.newBar();
    },
    // ajax获取cpu的数据
    loadCpuData: function () {
        Ext.Ajax.request({
            url: __path+"/dataController/getCpuData.do",
            method: "post",
            async: false,
            success: function (response) {
                var resp = Ext.decode(response.responseText);
                this.cpuData = resp.attributes.cpu;
            },
            scope: this
        });
    },
    // ajax获取内存的数据
    loadMemData: function () {
        Ext.Ajax.request({
            url: "dataController/getPhysicalMemory.do",
            method: "post",
            async: false,
            success: function (response, options) {
                var respText = Ext.decode(response.responseText);
                this.memValue = respText;
            },
            scope: this
        });
    },

    // ajax获取硬盘的数据
    loadDiskData: function () {
        Ext.Ajax.request({
            url: "dataController/getDiskData.do",
            method: "post",
            async: false,
            success: function (response, options) {
                var respText = Ext.decode(response.responseText);
                this.diskValue = respText;
            },
            scope: this

        });
    },

    // cpu的数据在仪表盘中显示
    newGauge: function () {
        // 基于准备好的dom，初始化echarts图表

        var option = {
            title: {
                text: 'CPU使用情况'
            },
            tooltip: {
                formatter: "{a} <br/>{c}%"
            },
            series: [{
                name: '总使用率',
                type: 'gauge',
                z: 3,
                min: 0,
                max: 100,
                splitNumber: 10,
                axisLine: { // 坐标轴线
                    lineStyle: { // 属性lineStyle控制线条样式
                        width: 10
                    }
                },
                axisTick: { // 坐标轴小标记
                    length: 15, // 属性length控制线长
                    lineStyle: { // 属性lineStyle控制线条样式
                        color: 'auto'
                    }
                },
                splitLine: { // 分隔线
                    length: 20, // 属性length控制线长
                    lineStyle: { // 属性lineStyle（详见lineStyle）控制线条样式
                        color: 'auto'
                    }
                },
                title: {
                    textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                        fontWeight: 'bolder',
                        fontSize: 20,
                        fontStyle: 'italic'
                    }
                },
                detail: {
                    formatter: '{value}%',
                    textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                        fontWeight: 'bolder'
                    }
                },
                data: [{
                    value: 40,
                    name: 'km/h'
                }]
            }, {
                name: '系统使用率',
                type: 'gauge',
                center: ['30%', '55%'], // 默认全局居中
                radius: '50%',
                min: 0,
                max: 100,
                endAngle: 45,
                splitNumber: 5,
                axisLine: { // 坐标轴线
                    lineStyle: { // 属性lineStyle控制线条样式
                        width: 8
                    }
                },
                axisTick: { // 坐标轴小标记
                    length: 12, // 属性length控制线长
                    lineStyle: { // 属性lineStyle控制线条样式
                        color: 'auto'
                    }
                },
                splitLine: { // 分隔线
                    length: 20, // 属性length控制线长
                    lineStyle: { // 属性lineStyle（详见lineStyle）控制线条样式
                        color: 'auto'
                    }
                },
                pointer: {
                    width: 5
                },
                title: {
                    offsetCenter: [0, '-30%']
                    // x, y，单位px
                },
                detail: {
                    formatter: '{value}%',
                    textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                        fontWeight: 'bolder'
                    }
                },
                data: [{
                    value: 1.5,
                    name: 'x1000 r/min'
                }]
            }, {
                name: '用户使用率',
                type: 'gauge',
                center: ['70%', '50%'], // 默认全局居中
                radius: '50%',
                min: 0,
                max: 100,
                startAngle: 135,
                endAngle: -45,
                splitNumber: 5,
                axisLine: { // 坐标轴线
                    lineStyle: { // 属性lineStyle控制线条样式
                        width: 8
                    }
                },
                axisTick: { // 坐标轴小标记
                    length: 12, // 属性length控制线长
                    lineStyle: { // 属性lineStyle控制线条样式
                        color: 'auto'
                    }
                },
                splitLine: { // 分隔线
                    length: 20, // 属性length控制线长
                    lineStyle: { // 属性lineStyle（详见lineStyle）控制线条样式
                        color: 'auto'
                    }
                },
                pointer: {
                    width: 5
                },
                title: {
                    offsetCenter: [0, '-30%']
                    // x, y，单位px
                },
                detail: {
                    formatter: '{value}%',
                    textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                        fontWeight: 'bolder'
                    }
                },
                data: [{
                    value: 1.5,
                    name: 'x1000 r/min'
                }]
            }]
        };
        var me = this;
        this.loadCpuData();
        this.setCpuValue(option, me.cpuData);
        this.cpuTimer = setInterval(function () {
            me.loadCpuData();
            me.setCpuValue(option, me.cpuData);
        }, 30000);
    },
    setCpuValue: function (option, value) {
        var chart = echarts
            .init(document.getElementById(this.mainDivId1 + "0"));
        option.series[0].data[0].value = value.combine.substr(0,
            value.combine.length - 1);
        option.series[1].data[0].value = value.sys.substr(0, value.sys.length
            - 1);
        option.series[2].data[0].value = value.user.substr(0, value.user.length
            - 1);
        option.series[0].data[0].name = '总使用率';
        option.series[1].data[0].name = '系统使用率';
        option.series[2].data[0].name = '用户使用率';

        chart.setOption(option);
    },
    appendDiv: function (i) {
        var html = "<div id='"
            + this.mainDivId1
            + i
            + "' style='height:400px ;width:1234px; margin:auto auto; border:1px solid #87CEEB'></div>";
        document.getElementById(this.mainDivId1).innerHTML = document
                .getElementById(this.mainDivId1).innerHTML
            + html;
    },
    s_appendDiv: function (i) {
        var html = "<div id='"
            + this.mainDivId2
            + (i + 1)
            + "' style='height:400px ;width:600px; margin:auto auto;margin-top:5px;margin-left:32px;float:left;border:1px solid #87CEEB'></div>";
        document.getElementById(this.mainDivId2 + 1).innerHTML = document
                .getElementById(this.mainDivId2 + 1).innerHTML
            + html;
    },
    b_appendDiv: function (i) {
        var html = "<div id='"
            + this.mainDivId2
            + i
            + "' style='height:400px ;width:1300px; margin:auto auto;'></div>";
        document.getElementById(this.mainDivId2).innerHTML = document
                .getElementById(this.mainDivId2).innerHTML
            + html;
    },
    // 内存中的数据在饼图中显示
    newPie: function () {
        var option = {
            title: {
                text: '内存使用情况',
                x: 'center'
            },
            tooltip: {
                trigger: 'item',
                formatter: "{a} <br/>{b} : {c}G ({d}%)"
            },
            legend: {
                orient: 'vertical',
                x: 'left',
                data: ['已使用内存', '闲置内存']
            },
            calculable: true,
            series: [{
                type: 'pie',
                radius: '55%',
                center: ['50%', '60%'],
                data: [{
                    value: 50,
                    name: '已使用内存',
                    itemStyle: {
                        normal: {
                            label: {
                                formatter: '{b} :\r\n {c}G ({d}%)'
                            }
                        }
                    }
                }, {
                    value: 50,
                    name: '闲置内存',
                    itemStyle: {
                        normal: {
                            label: {
                                formatter: '{b} :\r\n {c}G ({d}%)'
                            }
                        }
                    }
                }]
            }]
        };
        var me = this;
        this.loadMemData();
        this.setMemValue(option, me.memValue);
        this.memTimer = setInterval(function () {
            me.loadMemData();
            me.setMemValue(option, me.memValue);
        }, 30000);
        // console.info(memValue);
        // myChart.setOption(option);
    },
    setMemValue: function (option, value) {
        var chart = echarts
            .init(document.getElementById(this.mainDivId2 + "3"));
        option.series[0].data[0].value = value.use_mem;
        option.series[0].data[1].value = value.free_mem;
        option.series[0].data[0].name = '已使用内存';
        option.series[0].data[1].name = '闲置内存';
        chart.setOption(option);
    },
    newBar: function () {
        // console.info(this.diskValue);
        var diskName = [];
        var series = [{
            name: '已使用磁盘空间',
            type: 'bar',
            stack: '总量',
            barGap: '30%', // 柱间距离，默认为柱形宽度的30%，可设固定值
            barCategoryGap: '50%',
            itemStyle: {
                normal: {
                    label: {
                        show: true,
                        position: 'insideRight',
                        formatter: '{c}G'
                    }
                }
            },
            data: []
        }, {
            name: '闲置磁盘空间',
            type: 'bar',
            stack: '总量',
            barGap: '30%', // 柱间距离，默认为柱形宽度的30%，可设固定值
            barCategoryGap: '50%',
            itemStyle: {
                normal: {
                    label: {
                        show: true,
                        position: 'insideRight',
                        formatter: '{c}G'
                    }
                }
            },
            data: []
        }];
        var option = {
            title: {
                text: '磁盘使用情况',
                x: 'center'
            },
            tooltip: {
                trigger: 'axis',
                axisPointer: { // 坐标轴指示器，坐标轴触发有效
                    type: 'shadow' // 默认为直线，可选为：'line' | 'shadow'
                },
                formatter: function (params) {
                    var res = params[0].name;
                    for (var i = 0, l = params.length; i < l; i++) {
                        res += '<br/>' + params[i].seriesName + ' : '
                            + params[i].value + 'G';
                    }
                    return res;
                }
            },
            legend: {
                orient: 'vertical',
                x: 'left',
                data: ["已使用磁盘空间", "闲置磁盘空间"]
            },
            calculable: true,
            xAxis: [{
                type: 'value'
            }],
            yAxis: [{
                type: 'category',
                data: diskName
            }],
            series: series
        };

        // for (var i = 0; i < this.diskValue.length; i++) {
        var me = this;
        me.loadDiskData();
        me.setDiskValue(option, me.diskValue, diskName);
        this.diskTimer = setInterval(function () {
            diskName = [];
            series[0].data = [];
            series[1].data = [];
            me.loadDiskData();
            me.setDiskValue(option, me.diskValue, diskName);
        }, 60000);
        // }
    },
    setDiskValue: function (option, value, diskName) {
        var chart = echarts
            .init(document.getElementById(this.mainDivId2 + "4"));
        for (var i = value.length - 1; i >= 0; i--) {
            diskName.push(value[i].diskName + "盘");
            option.series[0].data.push(value[i].use_disk);
            option.series[1].data.push(value[i].free_disk);
        }
        chart.setOption(option);
    },
    beforeDestroy: function () {
        clearInterval(this.cpuTimer);
        clearInterval(this.memTimer);
        clearInterval(this.diskTimer);
        this.callParent(arguments);
    }
});
