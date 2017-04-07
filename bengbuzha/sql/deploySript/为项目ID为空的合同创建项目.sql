INSERT  INTO prj_项目
        SELECT  'CONTRACT-' + CONVERT(NVARCHAR(30), s.合同ID) AS 项目ID ,
                NULL AS [项目编码] ,
                s.合同ID AS [项目合同ID] ,
                NULL AS [启动日期] ,
                -1 AS [项目状态ID] ,
                1 AS [项目类型ID] ,
                s.合同名称 AS [项目名称] ,
                s.合同负责人 AS [项目经理ID] ,
                s.合同负责人 AS [创建人ID] ,
                s.签订日期 AS [创建时间] ,
                GETDATE() [最后修改日期] ,
                -1 AS [最后修改人ID]
        FROM    [xmis].[xmis].[con_合同] s
        WHERE   s.合同类型ID = 1
                AND s.项目ID IS NULL ;
        
        
UPDATE  [xmis].[xmis].[con_合同]
SET     项目ID = 'CONTRACT-' + CONVERT(NVARCHAR(30), 合同ID)
WHERE   合同类型ID = 1
        AND 项目ID IS NULL ;
  