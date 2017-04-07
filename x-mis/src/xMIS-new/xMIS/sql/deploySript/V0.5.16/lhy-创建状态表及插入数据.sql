--创建sys_状态表
create table sys_状态(
状态ID int not null,
状态名称 nvarchar(50) not null
)

--为sys_状态表插入数据
insert into sys_状态 values(1,'需求');
insert into sys_状态 values(2,'设计');
insert into sys_状态 values(3,'开发');
insert into sys_状态 values(4,'完成');
insert into sys_状态 values(5,'上线');

