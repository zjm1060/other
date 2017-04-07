 UPDATE    [xmis].[xmis].[con_合同]
  SET       乙方部门ID = 1
  WHERE     乙方部门ID IN ( 548, 3, 4, 5, 6, 7, 8, 10 )

 insert into  xmis.sys_条件选项 values(23,'execu_name',null,'执行部门',null,null)
 
 insert into xmis.sys_条件选项对应表 values(34,1,23,1,null,11,'where execu_name like ''%?%''',10,null,null)
 
 --调整项目组合查询
 UPDATE [xmis].[sys_条件选项] SET 名称='项目名称' WHERE 选项ID=9
  UPDATE [xmis].[sys_条件选项] SET 名称='项目编码' WHERE 选项ID=10
  DELETE xmis.sys_条件选项对应表 WHERE 选项ID=16

  --prj_项目增加列 执行部门
alter table prj_项目 add 执行部门 int
alter table prj_项目_history add 执行部门 int
--填充执行部门字段
UPDATE xmis.prj_项目 SET 项目经理ID=c.合同负责人
  FROM [xmis].[xmis].[prj_项目] p
  LEFT JOIN xmis.con_合同 c ON p.项目ID = c.项目ID
  WHERE p.项目经理ID IS NULL AND c.合同负责人 IS NOT NULL
  AND c.父合同ID IS NULL AND c.合同类型ID=1
--填充执行部门字段  
UPDATE xmis.prj_项目  SET 执行部门=ed.部门ID
FROM xmis.prj_项目 p 
LEFT JOIN xmis.hr_人员部门关系表 ed ON p.项目经理ID=ed.人员ID
WHERE ed.是否兼职=1