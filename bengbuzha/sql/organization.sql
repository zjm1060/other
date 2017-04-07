USE [backup]
CREATE TABLE #dept(SID INT IDENTITY(1,1), NAME NVARCHAR(40))
INSERT INTO #dept
        ( NAME )
SELECT DISTINCT 单位 FROM dbo.公司名单 WHERE 单位 NOT IN ('监控系统事业部')

CREATE TABLE #emp(SID INT IDENTITY(1,1), NAME NVARCHAR(40), dept_name NVARCHAR(40), sex NVARCHAR(2), post NVARCHAR(20))
INSERT INTO #emp
        ( NAME, dept_name,sex,post )
SELECT 姓名,单位,性别,职位名称 FROM dbo.公司名单 WHERE 单位 NOT IN ('水利自动化事业部', '监控系统事业部')




DECLARE @maxDeptID INT
SELECT @maxDeptID = MAX(部门ID) FROM xmis.xmis.org_部门

DECLARE @maxEmpID INT
SELECT @maxEmpID = MAX(xmis.xmis.hr_人员.人员ID) FROM xmis.xmis.hr_人员

--人员
INSERT INTO xmis.xmis.hr_人员
        ( 人员ID ,
          名字 ,
          姓氏 ,
          性别ID ,
          职务 ,
          修改人ID ,
          修改时间 ,
          密码 ,
          登录名 
        )
SELECT @maxEmpID+ SID, SUBSTRING(#emp.NAME,2,LEN(NAME)-1), SUBSTRING(#emp.NAME,0,2),CASE WHEN sex = '男' THEN 1 ELSE 0 END,
post,-1,GETDATE(),'000000',NAME
FROM #emp


--部门
INSERT INTO xmis.xmis.org_部门
        ( 部门ID ,
          部门名称 ,
          名称缩写 
        )
SELECT   @maxDeptID + sid , -- 部门ID - int
          name , -- 部门名称 - varchar(100)
          name  -- 名称缩写 - varchar(40)
         FROM #dept

--人员部门关系
INSERT INTO xmis.xmis.hr_人员部门关系表
        ( 人员ID, 部门ID, 是否兼职, ID )
SELECT @maxEmpID + #emp.SID, @maxDeptID + #dept.SID, 1,@maxEmpID + #emp.SID+'-' + @maxDeptID + #dept.SID FROM #dept,#emp WHERE dept_name = #dept.NAME 

--部门归属公司
INSERT INTO xmis.xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
SELECT  1, @maxDeptID + sid ,1 FROM #dept

--人员角色
INSERT INTO xmis.xmis.sec_人员角色
        ( 人员ID, 角色ID )
SELECT @maxEmpID + SID, 5
FROM #emp



