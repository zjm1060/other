
ALTER TABLE xmis.con_合同 ALTER COLUMN 备注 NVARCHAR(800)
ALTER TABLE xmis.con_合同 ALTER COLUMN 合同概要 NVARCHAR(2500)


SELECT  *
INTO    #parent
FROM    xmis.con_合同
WHERE   合同ID IN ( SELECT DISTINCT
                            父合同ID
                  FROM      xmis.con_合同
                  WHERE     合同类型ID = 1
                            AND 父合同ID IS NOT NULL )
                            --父合同
UNION ALL
SELECT  *
FROM    xmis.con_合同
WHERE   合同类型ID = 1
        AND 父合同ID IS NOT NULL
        --子合同
        

UPDATE  #parent
SET     合同概要 = ''
WHERE   合同概要 IS NULL
UPDATE  #parent
SET     备注 = ''
WHERE   备注 IS NULL


--合并子合同的总金额，摘要，备注，并加入到父合同
DECLARE cur CURSOR  FOR SELECT 合同ID,父合同ID,合同总额,合同概要,备注  FROM #parent WHERE 父合同ID IS NOT NULL
DECLARE @contratID INT ,
    @parentID INT ,
    @amt DECIMAL(16, 9) ,
    @abs NVARCHAR(2800) ,
    @remark NVARCHAR(500)

OPEN cur
FETCH NEXT FROM cur INTO @contratID , @parentID , @amt , @abs , @remark 
    
WHILE @@fetch_status = 0 
    BEGIN
        IF ( @parentID IS NOT NULL )
			-- child contract
            BEGIN
                
                UPDATE  #parent
                SET     合同总额 = 合同总额 + @amt ,
                        合同概要 = 合同概要 + ',' + CONVERT(NVARCHAR(100), @contratID)
                        + ')' + @abs ,
                        备注 = CONVERT(NVARCHAR(500), 备注) + ','
                        + CONVERT(NVARCHAR(100), @contratID) + ')' + @remark
                WHERE   合同ID = @parentID
            END

        FETCH NEXT FROM cur INTO @contratID , @parentID , @amt , @abs , @remark 

    END
CLOSE cur
DEALLOCATE cur

UPDATE  #parent
SET     父合同ID = 合同ID
WHERE   父合同ID IS NULL

--create new contractID
DECLARE @maxID INT
SELECT  @maxID = MAX(xmis.con_合同.合同ID)
FROM    xmis.con_合同

DECLARE cur1 CURSOR  FOR SELECT 父合同ID FROM #parent WHERE 合同ID = 父合同ID
DECLARE @oldID INT ,
    @forCount INT
SELECT  @forCount = 1

OPEN cur1
FETCH NEXT FROM cur1 INTO @oldID 
    
WHILE @@fetch_status = 0 
    BEGIN
        UPDATE  #parent
        SET     父合同ID = @maxID + @forCount
        WHERE   父合同ID = @oldID
                
        SELECT  @forCount = @forCount + 1
        FETCH NEXT FROM cur1 INTO @oldID 

    END
CLOSE cur1
DEALLOCATE cur1

--将原先的父合同变为父合同ID为自己的子合同
UPDATE  xmis.con_合同
SET     父合同ID = 合同ID
WHERE   合同ID IN ( SELECT  DISTINCT
                            父合同ID
                  FROM      xmis.con_合同
                  WHERE     合同类型ID = 1
                            AND 父合同ID IS NOT NULL )
                            --父合同

--insert new contract
INSERT  INTO xmis.con_合同
        ( 合同ID ,
          合同编号 ,
          合同名称 ,
          简称 ,
          父合同ID ,
          签订日期 ,
          甲方公司ID ,
          甲方部门ID ,
          乙方公司ID ,
          乙方部门ID ,
          合同总额 ,
          货币ID ,
          合同类型ID ,
          甲方代表ID ,
          乙方代表ID ,
          合同概要 ,
          备注 ,
          登记人ID ,
          更新时间 ,
          合同来源类型ID ,
          横向合同来源ID ,
          质保期起 ,
          质保期止 ,
          质保期定义 ,
          发货联系人ID ,
          质保期限 ,
          质保期计量单位 ,
          项目ID ,
          合同负责人
        )
        SELECT  父合同ID , --new contractID
                合同编号 ,
                合同名称 ,
                简称 ,
                NULL ,
                签订日期 ,
                甲方公司ID ,
                甲方部门ID ,
                乙方公司ID ,
                乙方部门ID ,
                合同总额 ,
                货币ID ,
                合同类型ID ,
                甲方代表ID ,
                乙方代表ID ,
                合同概要 ,
                备注 ,
                登记人ID ,
                更新时间 ,
                合同来源类型ID ,
                横向合同来源ID ,
                质保期起 ,
                质保期止 ,
                质保期定义 ,
                发货联系人ID ,
                质保期限 ,
                质保期计量单位 ,
                项目ID ,
                合同负责人
        FROM    #parent
        WHERE   合同ID IN ( SELECT    父合同ID
                          FROM      xmis.con_合同
                          WHERE     合同类型ID = 1
                                    AND 父合同ID = 合同ID )
                            --父合同



--付款条件
--SELECT * FROM xmis.con_收款条件表 WHERE 合同ID IN (SELECT 合同ID FROM #parent WHERE 父合同ID IS NOT null)
--SELECT * FROM xmis.con_收款条件表 WHERE 合同ID IN (SELECT 合同ID FROM #parent WHERE 父合同ID IS null)

DROP  TABLE #paymentcondition
SELECT  *
INTO    #paymentcondition
FROM    xmis.con_收款条件表
WHERE   1 = 2
go
ALTER TABLE #paymentcondition ADD iid INT IDENTITY(1,1)
go
INSERT  INTO #paymentcondition
        ( 合同ID ,
          收款条件ID ,
          收款条件描述 ,
          对应项目状态 ,
          延迟天数 ,
          收款日期 ,
          手动设置 ,
          收款百分比 ,
          收款金额 ,
          手动设置原因
        )
        SELECT  合同ID ,
                收款条件ID ,
                收款条件描述 ,
                对应项目状态 ,
                延迟天数 ,
                收款日期 ,
                手动设置 ,
                收款百分比 ,
                收款金额 ,
                手动设置原因
        FROM    xmis.con_收款条件表
        WHERE   合同ID IN ( SELECT    合同ID
                          FROM      #parent )
        ORDER BY 收款条件ID


UPDATE  #paymentcondition
SET     合同ID = 父合同ID
FROM    #parent ,
        #paymentcondition
WHERE   #parent.合同ID = #paymentcondition.合同ID
        AND 父合同ID IS NOT NULL
        
INSERT  INTO xmis.con_收款条件表
        ( 合同ID ,
          收款条件ID ,
          收款条件描述 ,
          对应项目状态 ,
          延迟天数 ,
          收款日期 ,
          手动设置 ,
          收款百分比 ,
          收款金额 ,
          手动设置原因
        )
        SELECT  合同ID ,
                iid ,
                收款条件描述 ,
                对应项目状态 ,
                延迟天数 ,
                收款日期 ,
                手动设置 ,
                收款百分比 ,
                收款金额 ,
                手动设置原因
        FROM    #paymentcondition


--收款明细
--SELECT * FROM xmis.con_收款明细表 WHERE 合同ID IN (SELECT 合同ID FROM #parent WHERE 父合同ID IS NOT null)
--SELECT * FROM xmis.con_收款明细表 WHERE 合同ID IN (SELECT 合同ID FROM #parent WHERE 父合同ID IS  null)

--DROP TABLE #paymentdetal
SELECT  *
INTO    #paymentdetal
FROM    xmis.con_收款明细表
WHERE   1 = 2
go
ALTER TABLE #paymentdetal ADD iid INT IDENTITY(1,1)
go

INSERT  INTO #paymentdetal
        ( 合同ID ,
          收款ID ,
          收款金额 ,
          收款日期 ,
          经办人 ,
          票据扫描 ,
          登记人 ,
          登记日期 ,
          备注
        )
        SELECT  合同ID ,
                收款ID ,
                收款金额 ,
                收款日期 ,
                经办人 ,
                票据扫描 ,
                登记人 ,
                登记日期 ,
                备注
        FROM    xmis.con_收款明细表
        WHERE   合同ID IN ( SELECT    合同ID
                          FROM      #parent )
        ORDER BY 收款ID


UPDATE  #paymentdetal
SET     合同ID = 父合同ID
FROM    #parent ,
        #paymentdetal
WHERE   #parent.合同ID = #paymentdetal.合同ID
        AND 父合同ID IS NOT NULL


INSERT  INTO xmis.con_收款明细表
        ( 合同ID ,
          收款ID ,
          收款金额 ,
          收款日期 ,
          经办人 ,
          票据扫描 ,
          登记人 ,
          登记日期 ,
          备注
        )
        SELECT  合同ID ,
                iid ,
                收款金额 ,
                收款日期 ,
                经办人 ,
                票据扫描 ,
                登记人 ,
                登记日期 ,
                备注
        FROM    #paymentdetal



--票据明细
--SELECT  * FROM    xmis.con_发票开具表 WHERE   合同ID IN ( SELECT    合同ID                  FROM      #parent                  WHERE     父合同ID IS NOT NULL )
--UNION ALL 
--SELECT  * FROM    xmis.con_发票开具表 WHERE   合同ID IN ( SELECT    合同ID                  FROM      #parent                  WHERE     父合同ID IS  NULL )


SELECT  *
INTO    #invoiceDetial
FROM    xmis.con_发票开具表
WHERE   1 = 2
go
ALTER TABLE #invoiceDetial ADD iid INT IDENTITY(1,1)
go
INSERT  INTO #invoiceDetial
        ( 合同ID ,
          发票号 ,
          发票金额 ,
          开具日期 ,
          经办人 ,
          扫描件 ,
          登记人 ,
          登记日期 ,
          备注 ,
          发票编号 ,
          收件人 ,
          邮件编号
        )
        SELECT  合同ID ,
                发票号 ,
                发票金额 ,
                开具日期 ,
                经办人 ,
                扫描件 ,
                登记人 ,
                登记日期 ,
                备注 ,
                发票编号 ,
                收件人 ,
                邮件编号
        FROM    xmis.con_发票开具表
        WHERE   合同ID IN ( SELECT    合同ID
                          FROM      #parent )
        ORDER BY 发票号


UPDATE  #invoiceDetial
SET     合同ID = 父合同ID
FROM    #parent ,
        #invoiceDetial
WHERE   #parent.合同ID = #invoiceDetial.合同ID
        AND 父合同ID IS NOT NULL


INSERT  INTO xmis.con_发票开具表
        ( 合同ID ,
          发票号 ,
          发票金额 ,
          开具日期 ,
          经办人 ,
          扫描件 ,
          登记人 ,
          登记日期 ,
          备注 ,
          发票编号 ,
          收件人 ,
          邮件编号
        )
        SELECT  合同ID ,
                iid ,
                发票金额 ,
                开具日期 ,
                经办人 ,
                扫描件 ,
                登记人 ,
                登记日期 ,
                备注 ,
                发票编号 ,
                收件人 ,
                邮件编号
        FROM    #invoiceDetial


--收据明细表
--SELECT * FROM xmis.con_收据明细表 WHERE 合同ID IN (SELECT 合同ID FROM #parent WHERE 父合同ID IS NOT null)
--UNION ALL 
--SELECT  * FROM    xmis.con_收据明细表 WHERE   合同ID IN ( SELECT    合同ID                   FROM      #parent                  WHERE     父合同ID IS  NULL )

SELECT  *
INTO    #receiptDetial
FROM    xmis.con_收据明细表
WHERE   1 = 2
go
ALTER TABLE #receiptDetial ADD iid INT IDENTITY(1,1)
go

INSERT  INTO #receiptDetial
        ( 收据ID ,
          收据金额 ,
          收据日期 ,
          经办人 ,
          票据扫描 ,
          合同ID ,
          登记人 ,
          登记日期 ,
          备注 ,
          收据编号 ,
          收件人 ,
          邮件编号
        )
        SELECT  收据ID ,
                收据金额 ,
                收据日期 ,
                经办人 ,
                票据扫描 ,
                合同ID ,
                登记人 ,
                登记日期 ,
                备注 ,
                收据编号 ,
                收件人 ,
                邮件编号
        FROM    xmis.con_收据明细表
        WHERE   合同ID IN ( SELECT    合同ID
                          FROM      #parent )
        ORDER BY 收据ID


UPDATE  #receiptDetial
SET     合同ID = 父合同ID
FROM    #parent ,
        #receiptDetial
WHERE   #parent.合同ID = #receiptDetial.合同ID
        AND 父合同ID IS NOT NULL
        
        
INSERT  INTO xmis.con_收据明细表
        ( 收据ID ,
          收据金额 ,
          收据日期 ,
          经办人 ,
          票据扫描 ,
          合同ID ,
          登记人 ,
          登记日期 ,
          备注 ,
          收据编号 ,
          收件人 ,
          邮件编号
        )
        SELECT  iid ,
                收据金额 ,
                收据日期 ,
                经办人 ,
                票据扫描 ,
                合同ID ,
                登记人 ,
                登记日期 ,
                备注 ,
                收据编号 ,
                收件人 ,
                邮件编号
        FROM    #receiptDetial
        

--收支对应
--SELECT  * FROM    xmis.con_收入支出合同对应表 WHERE   收入合同ID IN ( SELECT  合同ID FROM    #parent WHERE   父合同ID IS NOT NULL ) 
--SELECT  * FROM    xmis.con_收入支出合同对应表 WHERE   收入合同ID IN ( SELECT  合同ID FROM    #parent WHERE   父合同ID IS NULL ) 


SELECT  *
INTO    #incomeMapping
FROM    xmis.con_收入支出合同对应表
WHERE   1 = 2
go
ALTER TABLE #incomeMapping ADD iid INT IDENTITY(1,1)
go
INSERT  INTO #incomeMapping
        ( 收入合同ID ,
          支出合同ID ,
          对应ID ,
          金额 ,
          备注 ,
          支出子合同ID 
        )
        SELECT  收入合同ID ,
                支出合同ID ,
                对应ID ,
                金额 ,
                备注 ,
                支出子合同ID
        FROM    xmis.con_收入支出合同对应表
        WHERE   收入合同ID IN ( SELECT  合同ID
                            FROM    #parent )
        ORDER BY 对应ID


UPDATE  #incomeMapping
SET     收入合同ID = 父合同ID
FROM    #parent ,
        #incomeMapping
WHERE   #parent.合同ID = #incomeMapping.收入合同ID
        AND 父合同ID IS NOT NULL
        
INSERT  INTO xmis.con_收入支出合同对应表
        ( 收入合同ID ,
          支出合同ID ,
          对应ID ,
          金额 ,
          备注 ,
          支出子合同ID 
        )
        SELECT  收入合同ID ,
                支出合同ID ,
                iid ,
                金额 ,
                备注 ,
                支出子合同ID
        FROM    #incomeMapping


s
--附件
--SELECT  COUNT(*) FROM    xmis.sys_附件
--SELECT DISTINCT 附件主体类型ID FROM    xmis.sys_附件 

SELECT  *
INTO    #attachment
FROM    xmis.sys_附件
WHERE   附件主体类型ID = 1
        AND 1 = 2 
go        
ALTER TABLE #attachment ADD iid INT IDENTITY(1,1)
go
INSERT  INTO #attachment
        ( 附件ID ,
          上传时间 ,
          上传人 ,
          内容描述 ,
          文件名 ,
          附件内容 ,
          附件主体类型ID ,
          附件相关字段 ,
          REFID
        )
        SELECT  附件ID ,
                上传时间 ,
                上传人 ,
                内容描述 ,
                文件名 ,
                附件内容 ,
                附件主体类型ID ,
                附件相关字段 ,
                REFID
        FROM    xmis.sys_附件
        ORDER BY 附件ID

UPDATE  #attachment
SET     REFID = 父合同ID
FROM    #parent ,
        #attachment
WHERE   #parent.合同ID = #attachment.REFID
        AND 父合同ID IS NOT NULL
        
        
INSERT  INTO xmis.sys_附件
        ( 附件ID ,
          上传时间 ,
          上传人 ,
          内容描述 ,
          文件名 ,
          附件内容 ,
          附件主体类型ID ,
          附件相关字段 ,
          REFID
        )
        SELECT  iid ,
                上传时间 ,
                上传人 ,
                内容描述 ,
                文件名 ,
                附件内容 ,
                附件主体类型ID ,
                附件相关字段 ,
                REFID
        FROM    #attachment
        