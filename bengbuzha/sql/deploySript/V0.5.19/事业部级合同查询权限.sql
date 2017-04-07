--SELECT * FROM xmis.sec_角色 ORDER BY 角色ID

DECLARE @iid INT
SELECT @iid = MAX(角色ID) + 1 FROM xmis.sec_角色
INSERT INTO xmis.sec_角色
        ( 角色ID, 角色名称, 角色描述, 密级 )
VALUES  ( @iid, -- 角色ID - smallint
          '事业部级合同查询员', -- 角色名称 - char(30)
          '查询本事业部级合同及项目信息', -- 角色描述 - nvarchar(1000)
          -1  -- 密级 - smallint
          )


INSERT INTO xmis.sec_人员角色        ( 人员ID, 角色ID )
SELECT xmis.hr_人员.人员ID, @iid FROM xmis.hr_人员,xmis.hr_人员部门关系表,xmis.org_部门归属部门表
WHERE xmis.hr_人员.人员ID = xmis.hr_人员部门关系表.人员ID AND xmis.hr_人员部门关系表.部门ID  = xmis.org_部门归属部门表.部门ID
AND (xmis.org_部门归属部门表.父部门ID = 1 ) AND xmis.hr_人员.人员ID > 0


DECLARE @maxRoleID INT , @maxDataRoleID INT
SELECT @maxRoleID = MAX(ID) FROM xmis.sec_角色用户操作权限定义表
SELECT @maxDataRoleID = MAX( 数据权限ID ) FROM xmis.sec_数据权限定义表

--SELECT @iid, @maxRoleID, @maxDataRoleID
--13	52	26

INSERT INTO xmis.sec_角色用户操作权限定义表
        ( ID ,
          角色或人员ID ,
          业务模块ID ,
          业务子模块ID ,
          业务功能ID ,
          操作权限ID ,
          是否是角色定义
        )
SELECT @maxRoleID+1,@iid,业务模块ID,业务子模块ID,业务功能ID,操作权限ID,是否是角色定义 FROM xmis.sec_角色用户操作权限定义表 WHERE ID = 21

INSERT INTO xmis.sec_数据权限定义表
        ( 数据权限ID ,
          权限ID ,
          数据字段ID ,
          运算符ID ,
          数据字段值 ,
          是否是敏感字段 ,
          sql子句 ,
          是否需要解析
        )
SELECT @maxDataRoleID+1,@maxRoleID+1,数据字段ID,运算符ID,'self-divID',是否是敏感字段,sql子句,是否需要解析 FROM xmis.sec_数据权限定义表 WHERE 权限ID = 21



INSERT INTO xmis.sec_角色用户操作权限定义表
        ( ID ,
          角色或人员ID ,
          业务模块ID ,
          业务子模块ID ,
          业务功能ID ,
          操作权限ID ,
          是否是角色定义
        )
SELECT @maxRoleID+2,@iid,业务模块ID,业务子模块ID,业务功能ID,操作权限ID,是否是角色定义 FROM xmis.sec_角色用户操作权限定义表 WHERE ID = 22

INSERT INTO xmis.sec_数据权限定义表
        ( 数据权限ID ,
          权限ID ,
          数据字段ID ,
          运算符ID ,
          数据字段值 ,
          是否是敏感字段 ,
          sql子句 ,
          是否需要解析
        )
SELECT @maxDataRoleID+2,@maxRoleID+2,数据字段ID,运算符ID,'self-divID',是否是敏感字段,sql子句,是否需要解析 FROM xmis.sec_数据权限定义表 WHERE 权限ID = 22


INSERT INTO xmis.sec_角色用户操作权限定义表
        ( ID ,
          角色或人员ID ,
          业务模块ID ,
          业务子模块ID ,
          业务功能ID ,
          操作权限ID ,
          是否是角色定义
        )
SELECT @maxRoleID+3,@iid,业务模块ID,业务子模块ID,业务功能ID,操作权限ID,是否是角色定义 FROM xmis.sec_角色用户操作权限定义表 WHERE ID = 34

INSERT INTO xmis.sec_数据权限定义表
        ( 数据权限ID ,
          权限ID ,
          数据字段ID ,
          运算符ID ,
          数据字段值 ,
          是否是敏感字段 ,
          sql子句 ,
          是否需要解析
        )
SELECT @maxDataRoleID+3,@maxRoleID+3,数据字段ID,运算符ID,'self-divID',是否是敏感字段,sql子句,是否需要解析 FROM xmis.sec_数据权限定义表 WHERE 权限ID = 34




--SELECT * FROM xmis.sec_角色用户操作权限定义表 WHERE 角色或人员ID = 7

--SELECT * FROM xmis.sec_数据权限定义表 WHERE 权限ID IN (SELECT ID FROM xmis.sec_角色用户操作权限定义表 WHERE 角色或人员ID = 7)







