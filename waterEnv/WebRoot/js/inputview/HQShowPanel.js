Ext.define('bitc.wrenv.inputview.HQShowPanel', {
    extend: 'Ext.panel.Panel',
    autoScroll: true,
    data:null,
    xData:null,
    initComponent: function () {
        this.mainDivId = this.getId() + '-main';
        this.html = "<div style='width:650px;height:550px' id='" + this.mainDivId + "'></div>";
        this.callParent(arguments);
    },
    afterRender: function () {
        this.callParent(arguments);
        this.drawEcharts();
    },
    drawEcharts:function(){
        var me = this;
        var myChart = echarts.init(document.getElementById(this.mainDivId));
        var option = {
            tooltip: {
                trigger: 'axis',
                formatter: function (params) {
                    return '流量：'+params[0].name+ '<br>水位：' +params[0].value;
                },
                axisPointer: {
                    animation: false
                }
            },
            xAxis: {
                name:'流量',
                type: 'category',
                data:me.xData,
                boundaryGap: false,
                splitLine: {
                    show: false
                }
            },
            yAxis: {
                name:'水位',
                type: 'value',
                scale:true,
                splitLine: {
                    show: true
                }
            },
            series: [{
                name: '水位 ',
                type: 'line',
                data: me.data
            }]
        }
        myChart.setOption(option);
    }
});
