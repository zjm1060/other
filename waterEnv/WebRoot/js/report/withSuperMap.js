Ext.define('bitc.report.withSuperMap', {
    extend: 'Ext.form.Panel',
    region: 'center',
    autoScroll: true,
    title:'地图信息',
    initComponent: function () {
        var frameId = this.frameId = bitc.wrenv.Util.getUuid();
        this.html= '<iframe id='+frameId+ ' src="" width="100%" height="100%" frameborder="0" scrolling="auto"></iframe>';
        this.callParent(arguments);
    },
    reLocation:function(smid,smName,objtype,centerxy){

        var type = objtype?objtype:"";
        var url = "http://192.168.117.199:8080/position/map.html?smid="+smid+"&name="+smName+"&objtype="+type+"&centerxy="+centerxy;
        document.getElementById(this.frameId).src=url;
    }
});