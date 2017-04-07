Ext.ns('xmis.chartStatistics.menu');
xmis.chartStatistics.menu = new Ext.Panel({
							width:800,
							region:'north',
							border:false,
							layout:'column',
							buttons:[
								{
								xtype:'button',
								id:'sexbtn',
								text:'性别统计图',
								handler: function(){
								  var url = __cxtPath
								+ '/chartStatistics/sexChart.jsp';
						parent.xMis.turn('wf-openchart-', '性别统计图', url, 't',
								true);
								}
							},{
								xtype:'button',
								id:'agebtn',
								text:'年龄统计图',
								handler: function(){
								  var url = __cxtPath
								+ '/chartStatistics/ageChart.jsp';
						parent.xMis.turn('wf-openchart-', '年龄统计图', url, 't',
								true);
								}
							},{
								xtype:'button',
								id:'zzmmbtn',
								text:'政治面貌统计图',
								handler: function(){
								  var url = __cxtPath
								+ '/chartStatistics/zzmmChart.jsp';
						parent.xMis.turn('wf-openchart-', '政治面貌统计图', url, 't',
								true);
								}
							},{
								xtype:'button',
								id:'dutybtn',
								text:'职务统计图',
								handler: function(){
								  var url = __cxtPath
								+ '/chartStatistics/dutyChart.jsp';
						parent.xMis.turn('wf-openchart-', '职务统计图', url, 't',
								true);
								}
							},{
								xtype:'button',
								id:'titlesbtn',
								text:'技术职称统计图',
								handler: function(){
								  var url = __cxtPath
								+ '/chartStatistics/titlesChart.jsp';
						parent.xMis.turn('wf-openchart-', '技术职称统计图', url, 't',
								true);
								}
							},{
								xtype:'button',
								id:'jsjbbtn',
								text:'专业技术级别统计图',
								handler: function(){
								  var url = __cxtPath
								+ '/chartStatistics/jsjbChart.jsp';
						parent.xMis.turn('wf-openchart-', '专业技术级别统计图', url, 't',
								true);
								}
							},{
								xtype:'button',
								text:'文化程度统计图',
								id:'whcdbtn',
								handler: function(){
								  var url = __cxtPath
								+ '/chartStatistics/whcdChart.jsp';
						parent.xMis.turn('wf-openchart-', '文化程度统计图', url, 't',
								true);
								}
							},{
								xtype:'button',
								id:'zgsfbtn',
								text:'职工身份统计图',
								handler: function(){
								  var url = __cxtPath
								+ '/chartStatistics/zgsfChart.jsp';
						parent.xMis.turn('wf-openchart-', '职工身份统计图', url, 't',
								true);
								}
							},{
								xtype:'button',
								text:'在职员工统计图',
								id:'zzygbtn',
								handler: function(){
								  var url = __cxtPath
								+ '/chartStatistics/zzygChart.jsp';
						parent.xMis.turn('wf-openchart-', '在职员工统计图', url, 't',
								true);
								}
							}
							]
							
					});