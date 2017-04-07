SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  proc departmentTree (
			@userID int
)

as

Begin

DECLARE @deptID INT

SELECT @deptID = xmis.hr_人员部门关系表.部门ID  FROM xmis.hr_人员部门关系表 WHERE xmis.hr_人员部门关系表.人员ID = @userID

SELECT xmis.org_部门.部门ID dept_id,xmis.org_部门.部门名称 dept_name INTO #deptTemp
FROM xmis.org_部门, xmis.org_部门归属部门表
WHERE
xmis.org_部门归属部门表.部门ID = xmis.org_部门.部门ID AND 
xmis.org_部门归属部门表.父部门ID = (
SELECT xmis.org_部门归属部门表.父部门ID FROM xmis.org_部门归属部门表 WHERE 部门ID = @deptID )


SELECT #deptTemp.*,xmis.hr_人员部门关系表.人员ID user_id INTO #deptTree
FROM #deptTemp, xmis.hr_人员部门关系表
WHERE xmis.hr_人员部门关系表.部门ID =* #deptTemp.dept_id AND  人员ID>0


SELECT #deptTree.*, RTRIM(xmis.hr_人员.姓氏)+RTRIM(xmis.hr_人员.名字) user_name FROM #deptTree, xmis.hr_人员
WHERE #deptTree.user_id *= xmis.hr_人员.人员ID ORDER BY dept_id

	
End

return

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

