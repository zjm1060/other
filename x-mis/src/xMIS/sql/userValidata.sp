/*
	Procedure :	userValidata.sp  
   	Purpose   :	
	
	Amend History                                       
	---------------------------------------------------------------  
	  No.   Date        	Amend By	Reason  
	---------------------------------------------------------------
	  1   	06/28/2010    	李轩       	create 

*/

Use xmis
go
IF OBJECT_ID('userValidata') IS NOT NULL
BEGIN
    DROP PROCEDURE userValidata
END
GO
create proc userValidata (
			 @loginName VARCHAR(20), @password VARCHAR(20)
)

as

Begin

	DECLARE @user_id INT
	
	IF NOT EXISTS (
	SELECT 
		1 
	FROM 
		xmis.hr_人员 userTab 
	WHERE 
		(CONVERT(VARCHAR(20),userTab.人员ID) = @loginName OR userTab.登录名 = @loginName) 
		AND RTRIM(userTab.密码) = @password
	) 
	RETURN 0
	

	SELECT 
		@user_id = userTab.人员ID 
	FROM 
		xmis.hr_人员 userTab 
	WHERE 
		(CONVERT(VARCHAR(20),userTab.人员ID) = @loginName OR userTab.登录名 = @loginName) 
		AND RTRIM(userTab.密码) = @password


	SELECT 	@user_id USER_ID
		, empDept.部门ID dept_id
		, empDept.职务 POSITION
		, CASE WHEN deptDept.父部门ID IS NULL THEN empDept.部门ID ELSE deptDept.父部门ID END division_id
		
	INTO 	#user
	FROM 	xmis.hr_人员部门关系表 empDept
		, xmis.org_部门归属部门表 deptDept
	WHERE 	empDept.人员ID = @user_id
		AND empDept.是否兼职 =1 
		AND empDept.部门ID *= deptDept.部门ID
		
	
	SELECT	USER_ID
		, dept_id
		, position
		, division_id
		, deptCompy.公司ID company_id
		, RTRIM(emp.姓氏)+RTRIM(emp.名字) emp_name
		, dept.部门名称 dept_name
		, division.部门名称 division_name
		, compy.公司名称 compy_name
		
		
	FROM	#user 
		,xmis.org_部门归属公司表 deptCompy
		,xmis.hr_人员 emp
		,xmis.org_部门 dept
		,xmis.org_部门 division
		,xmis.org_公司 compy
		
	WHERE	division_id = deptCompy.部门ID
		AND deptCompy.是否直属 = 1
		AND emp.人员ID = USER_ID
		AND dept.部门ID = dept_id
		AND division.部门ID = division_id
		AND compy.公司ID = deptCompy.公司ID
		
		

End

return
go
GRANT EXECUTE ON userValidata TO public
go



--userValidata 'jk-admin','admin'

