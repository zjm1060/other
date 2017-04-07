/*
	Procedure :	departmentTree.sp  
   	Purpose   :	
	
	Amend History                                       
	---------------------------------------------------------------  
	  No.   Date        	Amend By	Reason  
	---------------------------------------------------------------
	  1   	06/28/2010    	李轩       	create 

*/

Use xmis
go
IF OBJECT_ID('departmentTree') IS NOT NULL
BEGIN
    DROP PROCEDURE departmentTree
END
GO
create proc departmentTree (
			@divisionID int
)

as

BEGIN

SELECT 	xmis.org_部门归属部门表.部门ID dept_id
	,xmis.org_部门.部门名称 dept_name
INTO #deptTemp
FROM	xmis.org_部门归属部门表, xmis.org_部门
WHERE 	xmis.org_部门归属部门表.父部门ID = @divisionID 
	AND xmis.org_部门归属部门表.部门ID = xmis.org_部门.部门ID
	
	
SELECT #deptTemp.*,xmis.hr_人员部门关系表.人员ID user_id INTO #deptTree
FROM #deptTemp, xmis.hr_人员部门关系表
WHERE xmis.hr_人员部门关系表.部门ID =* #deptTemp.dept_id AND  人员ID>0


SELECT #deptTree.*, RTRIM(xmis.hr_人员.姓氏)+RTRIM(xmis.hr_人员.名字) user_name FROM #deptTree, xmis.hr_人员
WHERE #deptTree.user_id *= xmis.hr_人员.人员ID ORDER BY dept_id

	
End

return
go
GRANT EXECUTE ON departmentTree TO public
go


--departmentTree 1



