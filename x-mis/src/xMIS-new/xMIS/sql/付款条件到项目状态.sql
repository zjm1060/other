/****** Script for SelectTopNRows command from SSMS  ******/
DECLARE @maxID INT
SELECT  @maxID = MAX(项目状态ID)
FROM    xmis.prj_项目状态

SELECT  @maxID + 1000 AS maxid ,
        IDENTITY( INT,1,1 ) AS 项目状态ID ,
        ' ' AS 项目状态 ,
        p.收款条件描述 AS 项目状态解释 ,
        '1' AS 是否自定义 ,
        c.项目ID ,
        p.最后修改日期 ,
        p.最后修改人ID ,
        p.合同ID ,
        p.收款条件ID
INTO    #temp
FROM    [xmis].[xmis].[con_收款条件表] p
        LEFT JOIN xmis.con_合同 c ON p.合同ID = c.合同ID
WHERE   对应项目状态 = 0


SELECT  *
FROM    #temp
 
INSERT  INTO xmis.prj_项目状态
        SELECT  t.maxid + t.项目状态ID AS 项目状态ID ,
                t.项目状态,
                t.项目状态解释 ,
                t.是否自定义 ,
                t.项目ID ,
                t.最后修改日期 ,
                t.最后修改人ID
        FROM    #temp t
 
UPDATE  [xmis].[con_收款条件表]
SET     对应项目状态 = t.maxid + t.项目状态ID
FROM    #temp t
        LEFT JOIN xmis.[con_收款条件表] p ON p.合同ID = t.合同ID
                                        AND p.收款条件ID = t.收款条件ID
                                        
