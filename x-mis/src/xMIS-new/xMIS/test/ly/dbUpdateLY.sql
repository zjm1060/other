     SELECT *  FROM xmis.add_地址
 WHERE 地址ID  LIKE 'x-id-generator%'  
  
  SELECT * FROM xmis.hr_人员 
   WHERE 办公地址ID  LIKE 'x-id-generator%'  
   
--为中水科技的人员 添加默认的办公地址ID
 update xmis.hr_人员 set 办公地址ID='x-id-generator-'+CONVERT(VARCHAR(50),人员ID) WHERE 办公地址ID IS null 
 	and   人员ID IN ( 
 	SELECT 人员ID FROM xmis.hr_人员部门关系表 WHERE 部门ID IN(
  SELECT 部门ID FROM xmis.org_部门归属公司表 WHERE 公司ID=1) )
  
 --更改地址ID、详细地址
 INSERT INTO xmis.add_地址
        ( 地址ID ,
          详细地址 
        )
        SELECT 办公地址ID,办公地址 FROM xmis.hr_人员 WHERE 办公地址ID LIKE 'x-id-generator%'  
        
 --更改城市ID、区县、邮编
 UPDATE xmis.add_地址 
 SET 城市ID='1',
 区县='海淀区',邮编=100038 WHERE 地址ID  LIKE 'x-id-generator%' 
 
 
 
 
  --根据名字查询 某个公司的某人
  SELECT RTRIM(a.姓氏)+RTRIM(a.名字) NAME ,a.人员ID FROM
  xmis.hr_人员 a,
  xmis.hr_人员部门关系表 b,
  xmis.org_部门归属公司表 c 
  WHERE a.人员ID=b.人员ID 
  AND b.部门ID=c.部门ID 
  AND c.公司ID=?
 

 --根据人员ID 查询 此人员节点的路径 即上级的ID
SELECT 公司ID,a.部门ID,a.父部门ID,a.人员ID
  FROM xmis.org_部门归属公司表
  left join (
  SELECT 人员ID  ,hd.部门ID ,
  case when dd.父部门ID IS null then hd.部门ID else dd.父部门ID end as 父部门ID
  FROM xmis.hr_人员部门关系表 as hd
  left join xmis.org_部门归属部门表 dd on  hd.部门ID = dd.部门ID ) a 
  on a.父部门ID = xmis.org_部门归属公司表.部门ID
  where 是否直属 = 1  AND 人员ID = ?
  
 --根据部门ID 查询 此部门节点的路径 即上级的ID  
 SELECT 公司ID
      ,case when dd.父部门ID IS null then dc.部门ID else dd.父部门ID end as 事业部ID
      ,dc.部门ID
  FROM xmis.org_部门归属公司表 dc
  left join xmis.org_部门归属部门表 dd on dc.部门ID = dd.部门ID and dc.是否直属 = 0
  WHERE dc.部门ID = 518
  
 -- 【废】根据部门ID 查询 此部门节点的路径 即上级的ID 改前
 select c.公司ID,a.事业部ID,a.部门ID from xmis.org_部门归属公司表 c
  join (
  select d.部门ID
  ,case when dd.父部门ID IS null then d.部门ID else dd.父部门ID end as 事业部ID
  from xmis.org_部门 as d 
  left join xmis.org_部门归属部门表 dd on d.部门ID = dd.部门ID) as a
  on c.部门ID = a.事业部ID and c.是否直属 = 1
  where a.部门ID = 1
  
  
  
    UPDATE xmis.HR_人员 set 最后修改日期=rtrim(修改时间) where 最后修改日期 IS NULL AND 人员ID in (
  SELECT 人员ID FROM xmis.hr_人员部门关系表 WHERE 部门ID IN(
  SELECT 部门ID FROM xmis.org_部门归属公司表 WHERE 公司ID=1)
  )
  
  UPDATE xmis.HR_人员 set 最后修改人ID=rtrim(修改人ID) where 最后修改人ID IS NULL AND 人员ID in (
  SELECT 人员ID FROM xmis.hr_人员部门关系表 WHERE 部门ID IN(
  SELECT 部门ID FROM xmis.org_部门归属公司表 WHERE 公司ID=1)
  )
  
  
    UPDATE xmis.sec_角色 SET 全体员工标志=1 WHERE 角色ID NOT  IN (5,9)
    UPDATE xmis.sec_角色 SET 全体员工标志=0 WHERE 角色ID   IN (5,9)
    
    
    UPDATE xmis.sec_业务子模块表 SET 业务子模块url='crmMaint.do?method=getCompanyInfo&companyID=1&tab=1' 
    WHERE 业务子模块ID=10
  
    
    
-- 2012-12-3   
     
--角色表增加的三列的数据       
UPDATE xmis.sec_角色 SET 显示标志=0 WHERE 角色ID IN (1,6,10,12,15,16,105,106,107,108)
   UPDATE xmis.sec_角色 SET 显示标志=1 WHERE 角色ID not IN (1,6,10,12,15,16,105,106,107,108)
  
  
  UPDATE xmis.sec_角色 SET 全体员工标志=1 WHERE 角色ID IN (5,9)
	UPDATE xmis.sec_角色 SET 全体员工标志=0 WHERE 角色ID NOT IN (5,9)
		
		UPDATE xmis.sec_角色 SET 排序=1 WHERE 角色ID=2
		UPDATE xmis.sec_角色 SET 排序=200 WHERE 角色ID=3
		UPDATE xmis.sec_角色 SET 排序=202 WHERE 角色ID=4
		UPDATE xmis.sec_角色 SET 排序=300 WHERE 角色ID=5

		UPDATE xmis.sec_角色 SET 排序=101 WHERE 角色ID=7
		UPDATE xmis.sec_角色 SET 排序=203 WHERE 角色ID=8
		UPDATE xmis.sec_角色 SET 排序=201 WHERE 角色ID=11
		UPDATE xmis.sec_角色 SET 排序=100 WHERE 角色ID=14   
-- 修改角色ID为13、水利自动化事业部的人员对应的 [部门级具有的角色标志]=1 
  UPDATE xmis.sec_人员角色 SET [部门级具有的角色标志]=1 
  
  WHERE 角色ID=13 AND 人员ID IN (
   SELECT    p.人员ID  
        
 FROM   
        xmis.sec_人员角色 AS b ,
        xmis.hr_人员 AS p ,
        XMIS.hr_人员部门关系表 pd ,
        XMIS.ORG_部门归属公司表 c ,
        xmis.org_部门 d
 WHERE  
         b.人员ID = p.人员ID
        AND p.人员ID > 0
      
        AND pd.部门ID = c.部门ID
        AND d.部门ID = pd.部门ID 
        AND c.公司ID = 1
        AND c.是否直属 = 1
        AND P.人员ID = PD.人员ID
        AND  b.角色ID=13
         AND d.部门ID = 2
       )   
 
       
  -- 修改角色ID为13、监控事业部的人员对应的 [部门级具有的角色标志]=1 
  UPDATE xmis.sec_人员角色 SET  部门级具有的角色标志 =1 
  
  WHERE 角色ID=13 AND 人员ID IN (
   SELECT     
        p.人员ID  
        
 FROM   xmis.sec_角色 AS a ,
        xmis.sec_人员角色 AS b ,
        xmis.hr_人员 AS p ,
        XMIS.hr_人员部门关系表 pd ,
        XMIS.ORG_部门归属部门表 dd ,
        xmis.xmis.org_部门归属公司表 c ,
        xmis.org_部门 d
 WHERE  a.角色ID = b.角色ID
        AND b.人员ID = p.人员ID
        AND a.显示标志 = 1
        AND a.全体员工标志 = 0
        AND pd.部门ID = dd.部门ID
        AND dd.部门ID = pd.部门ID
        AND dd.父部门ID = d.部门ID
        AND p.人员ID = PD.人员ID
        AND p.人员ID > 0
        AND dd.父部门ID = 1
        AND dd.父部门ID = c.部门ID
        AND  b.角色ID=13
       )
 --
   INSERT INTO xmis.sec_业务子模块表
          ( 业务子模块ID ,
            业务子模块名称 ,
            业务子模块url ,
            业务子模块描述 ,
            业务子模块图标说明 ,
            业务子模块标识 ,
            提交人 ,
            提交时间 ,
            提交状态 ,
            备注 ,
            状态变更记录 ,
            最后修改日期 ,
            最后修改人ID
          )
  VALUES  ( 40 , -- 业务子模块ID - int
            '角色名单' , -- 业务子模块名称 - nvarchar(50)
            'securityMaint.do?method=getSecurityInfoList' , -- 业务子模块url - nvarchar(200)
            '角色名单' , -- 业务子模块描述 - nvarchar(50)
            '' , -- 业务子模块图标说明 - nvarchar(50)
            '' , -- 业务子模块标识 - nvarchar(50)
            '' , -- 提交人 - int
            2012-12-04 , -- 提交时间 - smalldatetime
            0 , -- 提交状态 - int
            N'' , -- 备注 - nvarchar(200)
            N'' , -- 状态变更记录 - nvarchar(200)
            '2012-12-04 00:10:22' , -- 最后修改日期 - datetime
            0  -- 最后修改人ID - int
          )  
          
 ---         
          
   INSERT INTO xmis.sec_业务模块对应表
          ( 业务模块ID ,
            业务子模块ID ,
            业务子模块排列顺序 ,
            最后修改日期 ,
            最后修改人ID
          )
  VALUES  ( 7 , -- 业务模块ID - int
            40 , -- 业务子模块ID - int
            2 , -- 业务子模块排列顺序 - int
            '2012-12-04 00:37:21' , -- 最后修改日期 - datetime
            0  -- 最后修改人ID - int
          )