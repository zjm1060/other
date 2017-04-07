/* old values of contractID & 乙方公司ID
3770	11
3791	11
3744	11
3768	11
3891	11
3890	11
3905	11
3889	11
4287	11
3784	11
4468	11
3754	11
3705	57
3843	11
*/

--将公司ID为11和57，名称是中水科技的公司ID改为1
update xmis.con_合同 set 乙方公司ID = 1 where 合同类型ID= 1 and 乙方公司ID in (11,57)


--处理公司ID为2（中水科自动化）的合同信息里的乙方部门ID，以方便缺省不显示该公司合同数据
begin
	DECLARE @deptID INT
	SELECT  @deptID = MAX(部门ID)
	FROM    xmis.org_部门
	
	SELECT  *
	INTO    #dept
	FROM    xmis.org_部门
	WHERE   部门ID IN ( SELECT DISTINCT
	                            乙方部门ID
	                  FROM      xmis.con_合同
	                  WHERE     乙方公司ID = 2 )
	
	UPDATE  #dept
	SET     部门ID = 部门ID + @deptID
	
	INSERT  INTO xmis.org_部门
	        ( 部门ID ,
	          部门名称 ,
	          名称缩写 ,
	          电话1 ,
	          传真1 ,
	          电话2 ,
	          传真2 ,
	          负责人ID ,
	          联系人ID ,
	          网址 ,
	          邮箱 ,
	          人数 ,
	          地址 ,
	          禁用 ,
	          是否关心业务数据
	        )
	        SELECT  *
	        FROM    #dept
	
	UPDATE  xmis.con_合同
	SET     乙方部门ID = @deptID + 乙方部门ID
	WHERE   乙方公司ID = 2
end


--添加可查询中水科自动化合同数据的角色及权限
begin

	DECLARE @roleID INT
	SELECT @roleID  = MAX(角色ID)+1 FROM  xmis.sec_角色
	
	INSERT INTO xmis.sec_角色 
	        ( 角色ID, 角色名称, 角色描述, 密级 )
	VALUES  ( @roleID, -- 角色ID - smallint
	          '中水科自动化', -- 角色名称 - char(30)
	          '查询中水科自动化合同信息', -- 角色描述 - nvarchar(1000)
	          99  -- 密级 - smallint
	          )
	          
	DECLARE @rightID INT
	SELECT @rightID = MAX(ID) FROM xmis.sec_角色用户操作权限定义表
	
	INSERT INTO xmis.sec_角色用户操作权限定义表
	        ( ID ,
	          角色或人员ID ,
	          业务模块ID ,
	          业务子模块ID ,
	          业务功能ID ,
	          操作权限ID ,
	          是否是角色定义
	        )
	VALUES  ( @rightID+1 , -- ID - int
	          @roleID , -- 角色或人员ID - int
	          2 , -- 业务模块ID - int
	          5 , -- 业务子模块ID - int
	          6 , -- 业务功能ID - int
	          5 , -- 操作权限ID - int
	          1  -- 是否是角色定义 - bit
	        )
	INSERT INTO xmis.sec_角色用户操作权限定义表
	        ( ID ,
	          角色或人员ID ,
	          业务模块ID ,
	          业务子模块ID ,
	          业务功能ID ,
	          操作权限ID ,
	          是否是角色定义
	        )
	VALUES  ( @rightID+2 , -- ID - int
	          @roleID , -- 角色或人员ID - int
	          2 , -- 业务模块ID - int
	          3 , -- 业务子模块ID - int
	          12 , -- 业务功能ID - int
	          5 , -- 操作权限ID - int
	          1  -- 是否是角色定义 - bit
	        )
	        
	DECLARE @iid INT
	SELECT @iid = MAX(数据权限ID) FROM xmis.sec_数据权限定义表
	
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
	VALUES  ( @iid+1 , -- 数据权限ID - int
	          @rightID+1 , -- 权限ID - int
	          9 , -- 数据字段ID - int
	          1 , -- 运算符ID - int
	          2 , -- 数据字段值 - nvarchar(100)
	          0 , -- 是否是敏感字段 - bit
	          NULL , -- sql子句 - nvarchar(1000)
	          0  -- 是否需要解析 - bit
	        )
	
	
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
	VALUES  ( @iid+2 , -- 数据权限ID - int
	          @rightID+2 , -- 权限ID - int
	          9 , -- 数据字段ID - int
	          1 , -- 运算符ID - int
	          2 , -- 数据字段值 - nvarchar(100)
	          0 , -- 是否是敏感字段 - bit
	          NULL , -- sql子句 - nvarchar(1000)
	          0  -- 是否需要解析 - bit
	        )       
	
	/*        
	DECLARE @roleID INT
	SELECT @roleID  = 角色ID FROM  xmis.sec_角色  where 角色名称 = '中水科自动化'
	        
	INSERT INTO xmis.sec_人员角色 
	        ( 人员ID, 角色ID )
	VALUES  ( 153, -- 人员ID - int
	          @roleID  -- 角色ID - smallint
	          )
	*/         
end 
         