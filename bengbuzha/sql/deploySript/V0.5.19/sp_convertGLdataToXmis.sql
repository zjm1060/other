USE xmis
go
IF OBJECT_ID('sp_convertGLdataToXmis') IS NOT NULL 
    BEGIN
        DROP PROCEDURE sp_convertGLdataToXmis
    END
GO
CREATE PROC sp_convertGLdataToXmis
AS 
    BEGIN
		
		--发生额

        TRUNCATE TABLE xmis.fas_income

        SELECT  xmis.fas_GL_accvouch.year ,
                xmis.fas_GL_accvouch.iperiod ,
                xmis.fas_GL_accvouch.ino_id ,
                citem_id ,
                SUM(mc) - SUM(md) income_amt
        INTO    #income1
        FROM    xmis.fas_GL_accvouch ( INDEX=i_GLaccvouch_yiicc ) ,
                xmis.fas_codeType
        WHERE   xmis.fas_GL_accvouch.iperiod > 0
                AND xmis.fas_GL_accvouch.iperiod <= 12
                AND NOT ( year = 2011
                          AND iperiod = 8
                          AND ino_id IN ( 327, 466 )
                        )
                AND xmis.fas_codeType.type = 0
                AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode
        GROUP BY xmis.fas_GL_accvouch.year ,
                xmis.fas_GL_accvouch.iperiod ,
                xmis.fas_GL_accvouch.ino_id ,
                citem_id
                
        INSERT  INTO xmis.fas_income
                ( year ,
                  iperiod ,
                  ino_id ,
                  citem_id ,
                  income_amt ,
                  project_code
		        
                )
                SELECT  #income1.* ,
                        xmis.fas_fitemss00.project_code
                FROM    #income1 ,
                        xmis.fas_fitemss00
                WHERE   #income1.year = xmis.fas_fitemss00.year
                        AND #income1.citem_id = xmis.fas_fitemss00.citemcode
                        AND xmis.fas_fitemss00.bclose = 0
                        AND project_code IS NOT NULL
		
        DROP INDEX xmis.fas_income.i_t_income_yiic             
        CREATE INDEX i_t_income_yiic ON xmis.fas_income(year,iperiod,ino_id,citem_id, project_code) 

		--来款期初余额

        TRUNCATE TABLE xmis.fas_qch
		
        INSERT  INTO xmis.fas_qch
                ( citem_id ,
                  income_mb 
                )
                SELECT  citem_id ,
                        CASE WHEN cbegind_c LIKE '借' THEN -1 * mb
                             ELSE 0
                        END AS income_mb
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
        
        
		--采购支出期初余额
        TRUNCATE TABLE xmis.fas_qcjcb
		
        INSERT  INTO xmis.fas_qcjcb
                ( citem_id ,
                  payout_mb 
                )
                SELECT  citem_id ,
                        CASE WHEN cbegind_c = '借' THEN mb
                             ELSE 0
                        END AS payout_mb
                FROM    xmis.fas_GL_accass ,
                        xmis.fas_codeType
                WHERE   year = 2008
                        AND iperiod = 1
                        AND citem_class = 00
                        AND xmis.fas_codeType.type = 1
                        AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode
                UNION ALL
                SELECT  citem_id ,
                        CASE WHEN cbegind_c = '贷' THEN -1 * mb
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
                FROM    xmis.fas_GL_accvouch ( INDEX=i_GLaccvouch_cc )
                WHERE   xmis.fas_GL_accvouch.iperiod > 0
                        AND xmis.fas_GL_accvouch.iperiod <= 12
                        AND ccode IN ( '41010187', '4101012033' )
                GROUP BY citem_id
                UNION ALL
                SELECT  citem_id ,
                        SUM(md)
                FROM    xmis.fas_GL_accvouch ( INDEX=i_GLaccvouch_yii )
                WHERE   ( year = 2011
                          AND iperiod = 8
                          AND ino_id = 327
                        )
                GROUP BY citem_id 		 


		-- 来款合计 
        SELECT  project_code ,
                SUM(income_mb) AS MB
        INTO    #gl
        FROM    xmis.fas_qch ,
                xmis.fas_fitemss00
        WHERE   xmis.fas_fitemss00.year = 2008
                AND xmis.fas_qch.citem_id = xmis.fas_fitemss00.citemcode
        GROUP BY project_code
        UNION ALL
        SELECT  xmis.fas_income.project_code ,
                SUM(xmis.fas_income.income_amt)
        FROM    xmis.fas_income
        GROUP BY xmis.fas_income.project_code
        HAVING  SUM(income_amt) <> 0
        SELECT  project_code ,
                SUM(MB) MB
        INTO    #glTotal
        FROM    #gl
        GROUP BY project_code 
		
		--  采购支出合计 
        TRUNCATE TABLE xmis.fas_glInfo

        SELECT  year ,
                citem_id ,
                SUM(md - mc) AS pay_amt
        INTO    #purchase1
        FROM    xmis.fas_GL_accvouch ( INDEX=i_GLaccvouch_yii ) ,
                xmis.fas_codeType
        WHERE   xmis.fas_GL_accvouch.iperiod > 0
                AND xmis.fas_GL_accvouch.iperiod <= 12
                AND NOT ( year = 2011
                          AND iperiod = 8
                          AND ino_id IN ( 327, 466 )
                        )
                AND xmis.fas_codeType.type = 1
                AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode
        GROUP BY YEAR ,
                citem_id
        HAVING  SUM(md - mc) <> 0
        SELECT  project_code ,
                SUM(payout_mb) AS pay_amt
        INTO    #purchase
        FROM    xmis.fas_qcjcb ,
                xmis.fas_fitemss00
        WHERE   xmis.fas_fitemss00.year = 2008
                AND xmis.fas_qcjcb.citem_id = xmis.fas_fitemss00.citemcode
        GROUP BY project_code
        UNION ALL
        SELECT  project_code ,
                SUM(pay_amt)
        FROM    #purchase1 ,
                xmis.fas_fitemss00
        WHERE   #purchase1.year = xmis.fas_fitemss00.year
                AND #purchase1.citem_id = xmis.fas_fitemss00.citemcode
                AND xmis.fas_fitemss00.bclose = 0
                AND project_code IS NOT NULL
        GROUP BY project_code
        
		
        SELECT  project_code ,
                SUM(pay_amt) pay_amt
        INTO    #purchaseTotal
        FROM    #purchase
        GROUP BY project_code

        INSERT  INTO xmis.fas_glInfo
                ( project_code ,
                  MB ,
                  pay_amt 
                )
                SELECT  #glTotal.project_code ,
                        MB ,
                        pay_amt
                FROM    #glTotal ,
                        #purchaseTotal
                WHERE   #glTotal.project_code *= #purchaseTotal.project_code         
        
    END
	


--sp_convertGLdataToXmis

