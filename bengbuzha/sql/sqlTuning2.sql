SET NOCOUNT ON 
DECLARE @contractType INT
SET @contractType = 1 


SELECT  xmis.con_合同.* ,
        CONVERT(VARCHAR(10), xmis.con_合同.质保期起, 1) AS start_date ,
        CONVERT(VARCHAR(10), xmis.con_合同.质保期止, 1) AS end_date ,
        CONVERT(VARCHAR(10), xmis.con_合同.更新时间, 1) AS update_date ,
        xmis.prj_项目.项目编码 ,
        xmis.prj_项目.项目名称 ,
        contract.合同编号 contractNo ,
        contract.简称 abbreviation ,
        aCompany.公司名称 aCompany_name ,
        aCompany.名称缩写 aCompany_abb ,
        bCompany.公司名称 bCompany_name ,
        aDept.部门名称 aDept_name ,
        bDept.部门名称 bDept_name ,
        RTRIM(aEmp.姓氏) + RTRIM(aEmp.名字) aEmp_name ,
        RTRIM(bEmp.姓氏) + RTRIM(bEmp.名字) bEmp_name ,
        RTRIM(cEmp.姓氏) + RTRIM(cEmp.名字) cEmp_name ,
        xmis.con_横向合同来源.名称 AS 横向合同来源 ,
        CASE WHEN xmis.con_合同.货币ID IS NULL THEN '￥'
             WHEN xmis.con_合同.货币ID = -1 THEN '￥'
             ELSE xmis.sys_货币.货币符号
        END AS 货币符号 ,
        CONVERT(SMALLINT, NULL) AS 对应收入合同部门ID ,
        CONVERT(SMALLINT, NULL) AS 对应收入合同负责人
INTO    #contractList
FROM    xmis.con_合同 ,
        xmis.prj_项目 ,
        xmis.con_合同 AS contract ,
        xmis.org_公司 AS aCompany ,
        xmis.org_公司 AS bCompany ,
        xmis.org_部门 AS aDept ,
        xmis.org_部门 AS bDept ,
        xmis.hr_人员 AS aEmp ,
        xmis.hr_人员 AS bEmp ,
        xmis.hr_人员 AS cEmp ,
        xmis.con_横向合同来源 ,
        xmis.sys_货币
WHERE   xmis.con_合同.合同类型ID = @contractType
        AND xmis.prj_项目.项目ID =* xmis.con_合同.项目ID
        AND xmis.con_合同.父合同ID *= contract.合同ID
        AND aCompany.公司ID =* xmis.con_合同.甲方公司ID
        AND bCompany.公司ID =* xmis.con_合同.乙方公司ID
        AND aDept.部门ID =* xmis.con_合同.甲方部门ID
        AND bDept.部门ID =* xmis.con_合同.乙方部门ID
        AND aEmp.人员ID =* xmis.con_合同.甲方代表ID
        AND bEmp.人员ID =* xmis.con_合同.乙方代表ID
        AND cEmp.人员ID =* xmis.con_合同.登记人ID
        AND xmis.con_横向合同来源.横向合同来源ID =* xmis.con_合同.横向合同来源ID
        AND xmis.con_合同.货币ID*=xmis.sys_货币.货币ID
 

DELETE  #contractList
WHERE   合同类型ID = 2
        AND 合同ID IN ( SELECT DISTINCT
                                父合同ID
                      FROM      xmis.con_合同
                      WHERE     合同类型ID = 2 )

SELECT  合同ID ,
        SUM(收款金额) AS 收款合计
INTO    #charged
FROM    xmis.con_收款明细表
GROUP BY 合同ID

SELECT  合同ID ,
        SUM(发票金额) AS 已开发票金额合计
INTO    #invoice
FROM    xmis.con_发票开具表
GROUP BY 合同ID

SELECT  合同ID ,
        SUM(收据金额) AS 已开收据金额合计
INTO    #receipt
FROM    xmis.con_收据明细表
GROUP BY 合同ID

SELECT  xmis.con_收款条件表.合同ID ,
        xmis.con_收款条件表.收款日期
INTO    #warning_date
FROM    xmis.con_收款条件表

SELECT  xmis.con_收款条件表.合同ID ,
        #warning_date.收款日期 AS warning_date ,
        SUM(xmis.con_收款条件表.收款金额) AS warning_amt ,
        0 AS warninged_flag ,
        NULL AS warning_days
INTO    #warning_detail
FROM    xmis.con_收款条件表 ,
        #warning_date
WHERE   xmis.con_收款条件表.合同ID = #warning_date.合同ID
        AND ( xmis.con_收款条件表.收款日期 <= #warning_date.收款日期
              OR #warning_date.收款日期 IS NULL
            )
GROUP BY xmis.con_收款条件表.合同ID ,
        #warning_date.收款日期

UPDATE  #warning_detail
SET     warning_amt = warning_amt - 收款合计 ,
        warninged_flag = 1
FROM    #charged ,
        #warning_detail
WHERE   #charged.合同ID = #warning_detail.合同ID
        AND 收款合计 >= warning_amt
UPDATE  #warning_detail
SET     warning_days = CASE WHEN DATEDIFF(d, GETDATE(), warning_date) < -30
                            THEN -30
                            WHEN DATEDIFF(d, GETDATE(), warning_date) >= -30
                                 AND DATEDIFF(d, GETDATE(), warning_date) < -15
                            THEN -15
                            WHEN DATEDIFF(d, GETDATE(), warning_date) >= -15
                                 AND DATEDIFF(d, GETDATE(), warning_date) < -7
                            THEN -7
                            WHEN DATEDIFF(d, GETDATE(), warning_date) >= -7
                                 AND DATEDIFF(d, GETDATE(), warning_date) < 0
                            THEN -1
                            WHEN DATEDIFF(d, GETDATE(), warning_date) = 0
                            THEN 1
                            WHEN DATEDIFF(d, GETDATE(), warning_date) > 0
                                 AND DATEDIFF(d, GETDATE(), warning_date) <= 7
                            THEN 7
                            WHEN DATEDIFF(d, GETDATE(), warning_date) > 7
                                 AND DATEDIFF(d, GETDATE(), warning_date) <= 15
                            THEN 15
                            WHEN DATEDIFF(d, GETDATE(), warning_date) > 15
                                 AND DATEDIFF(d, GETDATE(), warning_date) <= 30
                            THEN 30
                            WHEN warning_date IS NULL THEN NULL
                            ELSE DATEDIFF(d, GETDATE(), warning_date)
                       END
WHERE   warninged_flag = 0
SELECT  合同ID ,
        MAX(warning_date) AS warning_date ,
        MAX(warning_amt) AS warning_amt ,
        MAX(warning_days) AS warning_days
INTO    #warning
FROM    #warning_detail
WHERE   warning_days <= 30
        AND warninged_flag = 0
GROUP BY 合同ID

SELECT  xmis.con_收款条件表.合同ID ,
        MIN(xmis.con_收款条件表.收款日期) AS warning_date ,
        0 AS warning_amt ,
        31 AS warning_days
INTO    #warning_info
FROM    xmis.con_收款条件表 ,
        #warning
WHERE   xmis.con_收款条件表.合同ID NOT IN ( SELECT 合同ID
                                     FROM   #warning )
        AND 收款日期 >= GETDATE()
GROUP BY xmis.con_收款条件表.合同ID
UPDATE  #warning_info
SET     warning_amt = xmis.con_收款条件表.收款金额
FROM    xmis.con_收款条件表 ,
        #warning_info
WHERE   xmis.con_收款条件表.合同ID = #warning_info.合同ID
        AND xmis.con_收款条件表.收款日期 = #warning_info.warning_date
INSERT  INTO #warning
        SELECT  *
        FROM    #warning_info 

SELECT  #contractList.* ,
        CASE WHEN #charged.收款合计 IS NULL THEN 0
             ELSE #charged.收款合计
        END AS 收款合计 ,
        #contractList.合同总额 - CASE WHEN #charged.收款合计 IS NULL THEN 0
                                  ELSE #charged.收款合计
                             END AS 未收款金额合计 ,
        CASE WHEN #invoice.已开发票金额合计 IS NULL THEN 0
             ELSE #invoice.已开发票金额合计
        END AS 已开发票金额合计 ,
        CASE WHEN #receipt.已开收据金额合计 IS NULL THEN 0
             ELSE #receipt.已开收据金额合计
        END AS 已开收据金额合计 ,
        CASE WHEN #invoice.已开发票金额合计 IS NULL THEN 0
             WHEN #invoice.已开发票金额合计 = 0 THEN 0
             ELSE #invoice.已开发票金额合计 - CASE WHEN #charged.收款合计 IS NULL THEN 0
                                           ELSE #charged.收款合计
                                      END
        END AS 已开票未到账金额合计 ,
        #warning.warning_date ,
        #warning.warning_amt - CASE WHEN #charged.收款合计 IS NULL THEN 0
                                    ELSE #charged.收款合计
                               END AS warning_amt ,
        CASE WHEN #warning.warning_days IS NULL THEN -31
             ELSE #warning.warning_days
        END AS warning_days
INTO    #sumInfo
FROM    #contractList ,
        #charged ,
        #invoice ,
        #warning ,
        #receipt
WHERE   #contractList.合同ID*=#charged.合同ID
        AND #contractList.合同ID*=#invoice.合同ID
        AND #contractList.合同ID*= #warning.合同ID
        AND #contractList.合同ID*= #receipt.合同ID
SELECT  #sumInfo.* ,
        CONVERT(DECIMAL(18, 2), 0) AS MB ,
        CONVERT(DECIMAL(18, 2), 0) AS unMB ,
        CONVERT(DECIMAL(18, 2), 0) AS pay_amt
INTO    #prev
FROM    #sumInfo
WHERE   1 = 2
IF ( @contractType = 1 ) 
    BEGIN
        SELECT  DISTINCT
                xmis.fas_GL_accvouch.year ,
                xmis.fas_GL_accvouch.iperiod ,
                xmis.fas_GL_accvouch.ino_id
        INTO    #income_vouch
        FROM    xmis.fas_GL_accvouch
        WHERE   xmis.fas_GL_accvouch.iperiod > 0
                AND xmis.fas_GL_accvouch.iperiod <= 12
                AND NOT ( year = 2011
                          AND iperiod = 8
                          AND ino_id IN ( 327, 466 )
                        )
        CREATE INDEX i_t_incom_yii ON #income_vouch(year,iperiod,ino_id)
        SELECT  xmis.fas_GL_accvouch.year ,
                xmis.fas_GL_accvouch.iperiod ,
                xmis.fas_GL_accvouch.ino_id ,
                SUM(mc) - SUM(md) income_amt ,
                project_code ,
                citem_id
        INTO    #income
        FROM    xmis.fas_GL_accvouch ,
                #income_vouch ,
                xmis.fas_fitemss00 ,
                xmis.fas_codeType
        WHERE   xmis.fas_GL_accvouch.year = xmis.fas_fitemss00.year
                AND xmis.fas_GL_accvouch.citem_id = xmis.fas_fitemss00.citemcode
                AND xmis.fas_fitemss00.bclose = 0
                AND xmis.fas_GL_accvouch.year = #income_vouch.year
                AND xmis.fas_GL_accvouch.iperiod = #income_vouch.iperiod
                AND xmis.fas_GL_accvouch.ino_id = #income_vouch.ino_id
                AND xmis.fas_codeType.type = 0
                AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode
                AND project_code IS NOT NULL
        GROUP BY xmis.fas_GL_accvouch.year ,
                xmis.fas_GL_accvouch.iperiod ,
                xmis.fas_GL_accvouch.ino_id ,
                project_code ,
                citem_id
        SELECT  citem_id ,
                CASE WHEN cbegind_c LIKE '借' THEN -1 * mb
                     ELSE 0
                END AS income_mb
        INTO    #qch
        FROM    xmis.fas_GL_accass ,
                xmis.fas_codeType
        WHERE   year = 2008
                AND iperiod = 1
                AND citem_class = 00
                AND xmis.fas_codeType.type = 0
                AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode
        UNION ALL
        SELECT  citem_id ,
                CASE WHEN cbegind_c LIKE '贷' THEN mb
                     ELSE 0
                END
        FROM    xmis.fas_GL_accass ,
                xmis.fas_codeType
        WHERE   year = 2008
                AND iperiod = 1
                AND citem_class = 00
                AND xmis.fas_codeType.type = 0
                AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode
        UNION ALL
        SELECT  citem_id ,
                SUM(mc)
        FROM    xmis.fas_GL_accvouch
        WHERE   xmis.fas_GL_accvouch.iperiod > 0
                AND xmis.fas_GL_accvouch.iperiod <= 12
                AND ccode = '51010102'
                OR ( year = 2011
                     AND iperiod = 8
                     AND ino_id = 466
                   )
        GROUP BY citem_id
        SELECT  citem_id ,
                CASE WHEN cbegind_c LIKE '借' THEN mb
                     ELSE 0
                END AS payout_mb
        INTO    #qcjcb
        FROM    xmis.fas_GL_accass ,
                xmis.fas_codeType
        WHERE   year = 2008
                AND iperiod = 1
                AND citem_class = 00
                AND xmis.fas_codeType.type = 1
                AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode
        UNION ALL
        SELECT  citem_id ,
                CASE WHEN cbegind_c LIKE '贷' THEN -1 * mb
                     ELSE 0
                END
        FROM    xmis.fas_GL_accass ,
                xmis.fas_codeType
        WHERE   year = 2008
                AND iperiod = 1
                AND citem_class = 00
                AND xmis.fas_codeType.type = 1
                AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode
        UNION ALL
        SELECT  citem_id ,
                SUM(md)
        FROM    xmis.fas_GL_accvouch
        WHERE   xmis.fas_GL_accvouch.iperiod > 0
                AND xmis.fas_GL_accvouch.iperiod <= 12
                AND ccode IN ( '41010187', '4101012033' )
                OR ( year = 2011
                     AND iperiod = 8
                     AND ino_id = 327
                   )
        GROUP BY citem_id
        SELECT  project_code ,
                SUM(income_mb) AS MB
        INTO    #gl
        FROM    #qch ,
                xmis.fas_fitemss00
        WHERE   xmis.fas_fitemss00.year = 2008
                AND #qch.citem_id = xmis.fas_fitemss00.citemcode
        GROUP BY project_code
        UNION ALL
        SELECT  #income.project_code ,
                SUM(mc) - SUM(md)
        FROM    xmis.fas_GL_accvouch ,
                #income ,
                xmis.fas_codeType
        WHERE   xmis.fas_GL_accvouch.year = #income.year
                AND xmis.fas_GL_accvouch.iperiod = #income.iperiod
                AND xmis.fas_GL_accvouch.ino_id = #income.ino_id
                AND xmis.fas_GL_accvouch.citem_id = #income.citem_id
                AND xmis.fas_codeType.type = 0
                AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode
        GROUP BY #income.project_code
        HAVING  SUM(income_amt) <> 0
        SELECT  project_code ,
                SUM(MB) MB
        INTO    #glTotal
        FROM    #gl
        GROUP BY project_code
        SELECT  project_code ,
                SUM(payout_mb) AS pay_amt
        INTO    #purchase
        FROM    #qcjcb ,
                xmis.fas_fitemss00
        WHERE   xmis.fas_fitemss00.year = 2008
                AND #qcjcb.citem_id = xmis.fas_fitemss00.citemcode
        GROUP BY project_code
        UNION ALL
        SELECT  project_code ,
                SUM(md - mc)
        FROM    xmis.fas_GL_accvouch ,
                #income_vouch ,
                xmis.fas_fitemss00 ,
                xmis.fas_codeType
        WHERE   xmis.fas_GL_accvouch.year = xmis.fas_fitemss00.year
                AND xmis.fas_GL_accvouch.citem_id = xmis.fas_fitemss00.citemcode
                AND xmis.fas_fitemss00.bclose = 0
                AND xmis.fas_GL_accvouch.year = #income_vouch.year
                AND xmis.fas_GL_accvouch.iperiod = #income_vouch.iperiod
                AND xmis.fas_GL_accvouch.ino_id = #income_vouch.ino_id
                AND xmis.fas_codeType.type = 1
                AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode
                AND project_code IS NOT NULL
        GROUP BY project_code
        HAVING  SUM(md - mc) <> 0
        SELECT  project_code ,
                SUM(pay_amt) pay_amt
        INTO    #purchaseTotal
        FROM    #purchase
        GROUP BY project_code
        SELECT  #glTotal.project_code ,
                MB ,
                pay_amt
        INTO    #glInfo
        FROM    #glTotal ,
                #purchaseTotal
        WHERE   #glTotal.project_code *= #purchaseTotal.project_code
        INSERT  INTO #prev
                SELECT  #sumInfo.* ,
                        CASE WHEN MB IS NULL THEN 0
                             ELSE MB
                        END AS MB ,
                        合同总额 - CASE WHEN MB IS NULL THEN 0
                                    ELSE MB
                               END AS unMB ,
                        CASE WHEN pay_amt IS NULL THEN 0
                             ELSE pay_amt
                        END AS pay_amt
                FROM    #sumInfo ,
                        #glInfo
                WHERE   #sumInfo.项目编码 *= #glInfo.project_code
    END
ELSE 
    BEGIN
        INSERT  INTO #prev
                SELECT  #sumInfo.* ,
                        CONVERT(DECIMAL(9, 2), 0) AS MB ,
                        CONVERT(DECIMAL(9, 2), 0) AS unMB ,
                        CONVERT(DECIMAL(9, 2), 0) AS pay_amt
                FROM    #sumInfo
    END 
IF @contractType = 2 
    BEGIN 

        UPDATE  #prev
        SET     对应收入合同部门ID = xmis.con_合同.乙方部门ID ,
                对应收入合同负责人 = xmis.con_合同.合同负责人
        FROM    xmis.con_合同 ,
                #prev ,
                xmis.con_收入支出合同对应表
        WHERE   xmis.con_收入支出合同对应表.支出子合同ID = #prev.合同ID
                AND xmis.con_合同.合同ID = xmis.con_收入支出合同对应表.收入合同ID
    END 
SELECT  #prev.*
INTO    #result
FROM    #prev
WHERE   ( 乙方部门ID IN ( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ) )

SELECT  #result.*
INTO    #resultC
FROM    #result
WHERE   1 = 2

INSERT  INTO #resultC
        SELECT  *
        FROM    #result
        ORDER BY MB DESC

SELECT  IDENTITY( INT,1,1 ) AS row_no ,
        #resultC.*
INTO    #resultA
FROM    #resultC 

SELECT  *
FROM    #resultA
WHERE   row_no = ( SELECT   MAX(row_no)
                   FROM     #resultA
                 )
UNION ALL
SELECT  *
FROM    #resultA
WHERE   row_no >= 1
        AND row_no < 21