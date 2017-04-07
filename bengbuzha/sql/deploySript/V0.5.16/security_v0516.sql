--为sec_业务模块表 增加字段
alter table sec_业务模块表 add 提交人 int;
alter table sec_业务模块表 add 提交时间 smalldatetime;
alter table sec_业务模块表 add 提交状态 int;
alter table sec_业务模块表 add 备注 nvarchar(200);
alter table sec_业务模块表 add 状态变更记录 nvarchar(200);

--为sec_业务子模块表 增加字段
alter table sec_业务子模块表 add 提交人 int;
alter table sec_业务子模块表 add 提交时间 smalldatetime;
alter table sec_业务子模块表 add 提交状态 int;
alter table sec_业务子模块表 add 备注 nvarchar(200);
alter table sec_业务子模块表 add 状态变更记录 nvarchar(200);

--为sec_业务功能表 增加字段
alter table sec_业务功能表 add 提交人 int;
alter table sec_业务功能表 add 提交时间 smalldatetime;
alter table sec_业务功能表 add 提交状态 int;
alter table sec_业务功能表 add 备注 nvarchar(200);
alter table sec_业务功能表 add 状态变更记录 nvarchar(200);

--删除权限系列表
delete xmis.sec_业务功能字段对应表
delete xmis.sec_业务功能表
delete xmis.sec_业务子模块表
delete xmis.sec_业务数据字段表
delete xmis.sec_业务模块功能对应表
delete xmis.sec_业务模块对应表
delete xmis.sec_业务模块表
delete xmis.sec_功能列表
delete xmis.sec_操作权限表
delete xmis.sec_数据权限定义表
delete xmis.sec_权限参数表
delete xmis.sec_角色
delete xmis.sec_角色功能
delete xmis.sec_角色用户操作权限定义表


--1)执行BCP命令
-- bcp_sec.bat

--2)run below sql

/*
 
--项目负责人
INSERT INTO xmis.sec_人员角色
        ( 人员ID, 角色ID )
SELECT 人员ID, 8 FROM xmis.sec_人员角色 WHERE 角色ID = 4

--项目管理员
INSERT INTO xmis.sec_人员角色
        ( 人员ID, 角色ID )
SELECT 人员ID, 6 FROM xmis.sec_人员角色 WHERE 角色ID = 3

--项目成员
INSERT INTO xmis.sec_人员角色
        ( 人员ID, 角色ID )
SELECT 人员ID, 9 FROM xmis.sec_人员角色 WHERE 角色ID = 5

--公司级项目查询
INSERT INTO xmis.sec_人员角色
        ( 人员ID, 角色ID )
SELECT 人员ID, 10 FROM xmis.sec_人员角色 WHERE 角色ID = 7

--合同决策
INSERT INTO xmis.sec_人员角色
        ( 人员ID, 角色ID )
SELECT 人员ID, 11 FROM xmis.sec_人员角色 WHERE 角色ID IN (3,7) AND 人员ID <> 153

*/