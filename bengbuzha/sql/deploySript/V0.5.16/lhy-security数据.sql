
insert into xmis.sec_业务模块表 values(6,'物资管理','','物资管理',6,'menu-device','DeviceMaint',null,null,null,null,null)
insert into xmis.sec_业务模块表 values(7,'What’s New','','What’s New',99,'','',null,null,null,null,null)


insert into xmis.sec_业务子模块表 values(11,'到款辅助查询','contractMain.do?method=incomeAssistSearch','',null,'income-assist-search',null,null,null,null,null)
insert into xmis.sec_业务子模块表 values(12,'我的采购申请','device/prList.jsp','我的采购申请',null,'device-my-pq',null,null,null,null,null)
insert into xmis.sec_业务子模块表 values(13,'采购申请列表','device/prList.jsp?type=all','采购申请列表',null,'device-pq-list',null,null,null,null,null)
insert into xmis.sec_业务子模块表 values(14,'X-MIS信息管理系统','developProgressMaint.do?method=getDevelopProgressList','',null,'',null,null,null,null,null)
insert into xmis.sec_业务子模块表 values(15,'待采购物资','device/tobePurchaseList.jsp','待采购物资列表',null,'device-tobep-list',null,null,null,null,null)

insert into xmis.sec_业务模块对应表 values(1,12,3)
update xmis.sec_业务模块对应表 set 业务子模块ID = 11,业务子模块排列顺序=5 where 业务模块ID =2 and 业务子模块ID=4 and 业务子模块排列顺序=4
insert into xmis.sec_业务模块对应表 values(6,13,1)
insert into xmis.sec_业务模块对应表 values(6,15,2)
insert into xmis.sec_业务模块对应表 values(7,14,1)

insert into xmis.sec_业务功能表 values(16,'到款辅助查询','contractMain.do?method=getIncomeAssistList',null,null,20,null,null,null,null,null)
insert into xmis.sec_业务功能表 values(17,'我的采购申请','device/prList.jsp',null,null,21,null,null,null,null,null)
insert into xmis.sec_业务功能表 values(21,'查询待采购物资','device/tobePurchaseList.jsp','device/tobePurchaseList.jsp',null,22,null,null,null,null,null)
insert into xmis.sec_业务功能表 values(18,'采购申请列表','',null,null,23,null,null,null,null,null)
insert into xmis.sec_业务功能表 values(19,'收入合同报警综合查询','contractMain.do?method=getContractList&searchtype=integration',null,null,24,null,null,null,null,null)
insert into xmis.sec_业务功能表 values(6,'收入合同综合查询报警数量统计','contractMain.do?method=warningTotalCountForSum&contractTypeID=1',null,null,25,null,null,null,null,null)
insert into xmis.sec_业务功能表 values(20,'项目进度查询','projectMain.do?method=getProjectList&show=true',null,null,26,null,null,null,null,null)

insert into xmis.sec_业务模块功能对应表 values(8,14)
insert into xmis.sec_业务模块功能对应表 values(12,17)
insert into xmis.sec_业务模块功能对应表 values(15,21)
