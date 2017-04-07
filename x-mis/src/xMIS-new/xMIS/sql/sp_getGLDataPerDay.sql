USE xmis
go
IF OBJECT_ID('sp_getGLaccvouchPerDay') IS NOT NULL 
    BEGIN
        DROP PROCEDURE sp_getGLaccvouchPerDay
    END
GO
CREATE PROC sp_getGLaccvouchPerDay
AS 
    BEGIN
        DECLARE @year CHAR(4) ,
            @TableName NVARCHAR(100) ,
            @er INT
            
        SET @year = CONVERT(CHAR(4), YEAR(DATEADD("d", -1, GETDATE())))
        SET @TableName = 'UFDATA_302_' + @year + '.dbo.GL_accvouch'

	--xmis.fas_GL_accvouch
        EXEC ( ' DELETE xmis.fas_GL_accvouch WHERE year = '+@year+' AND iperiod <= MONTH(GETDATE()) ' 
        + ' INSERT INTO xmis.fas_GL_accvouch SELECT ' + @year
        + ',* FROM OPENROWSET(''SQLOLEDB'',''192.168.8.8'';''sa'';''123456'',' + @TableName
        --+ ') WHERE  CONVERT(NVARCHAR(10),dbill_date,101) = CONVERT(CHAR(10),DATEADD("d",-1,GETDATE()),101) ' )
        + ') WHERE iperiod <= MONTH(GETDATE()) ' )

/*
DELETE  xmis.fas_GL_accvouch
WHERE   year = 2012
        AND iperiod <= MONTH(GETDATE())
INSERT  INTO xmis.fas_GL_accvouch
        SELECT  2012 ,
                *
        FROM    OPENROWSET('SQLOLEDB', '192.168.8.8'; 'sa'; '123456',
                           UFDATA_302_2012.dbo.GL_accvouch)
        WHERE   iperiod <= MONTH(GETDATE()) 
            
*/

	--xmis.fas_code; xmis.fas_deptartment; xmis.fas_fitemss00; xmis.fas_person
        DECLARE @subTableName NVARCHAR(100) ,
            @execSQL VARCHAR(8000)
                
        DECLARE cur CURSOR  FOR SELECT  ccode FROM xmis.fas_codeType WHERE type = 3 

        OPEN cur
        FETCH NEXT FROM cur INTO @subTableName

        WHILE @@fetch_status = 0 
            BEGIN

                SET @execSQL = 'DELETE xmis.fas_' + @subTableName
                    + ' where year = ' + @year + 'INSERT INTO xmis.fas_'
                    + @subTableName + ' SELECT ' + @year
                    + ',* FROM OPENROWSET(
						''SQLOLEDB'',''192.168.8.8'';''sa'';''123456'','
                    + 'UFDATA_302_' + @year + '.dbo.' + @subTableName + ') '
        
			--PRINT @subTableName
			
                IF ( @subTableName = 'fitemss00' ) 
                    BEGIN
                        SET @execSQL = REPLACE(@execSQL, @year + ',*',
                                               @year + ',*,NULL')
                    END
			
			--PRINT @execSQL
		           
                EXEC (@execSQL)
                FETCH NEXT FROM cur INTO @subTableName
			
            END
        CLOSE cur
        DEALLOCATE cur

	--project_code mapping
        BEGIN
            SELECT DISTINCT
                    citemcode ,
                    CONVERT(NVARCHAR(100), NULL) AS proj_code
            INTO    #citem_proj
            FROM    xmis.fas_fitemss00 

            UPDATE  #citem_proj
            SET     proj_code = xmis.f_UpdateProjCodeOnFas(citemcode)

            --SELECT  *             FROM    #citem_proj            WHERE   proj_code IS NULL

            UPDATE  #citem_proj
            SET     proj_code = xmis.fas_项目编码勘误表.project_code
            FROM    #citem_proj ,
                    xmis.fas_项目编码勘误表
            WHERE   #citem_proj.citemcode = xmis.fas_项目编码勘误表.citemcode


            UPDATE  xmis.fas_fitemss00
            SET     project_code = #citem_proj.proj_code
            FROM    xmis.fas_fitemss00 ,
                    #citem_proj
            WHERE   xmis.fas_fitemss00.citemcode = #citem_proj.citemcode
		
        END

	-- Re-create index on xmis.fas table                            
        BEGIN
            DROP INDEX xmis.con_合同.i_contratct_conType
            CREATE INDEX i_contratct_conType ON xmis.con_合同(合同类型ID,合同ID)

            DROP INDEX xmis.fas_GL_accvouch.i_GLaccvouch_yii
            CREATE INDEX i_GLaccvouch_yii ON xmis.fas_GL_accvouch(year,iperiod,ino_id )

            DROP INDEX xmis.fas_GL_accvouch.i_GLaccvouch_yiicc
            CREATE INDEX i_GLaccvouch_yiicc ON xmis.fas_GL_accvouch(year,iperiod,ino_id, citem_id)

            DROP INDEX xmis.fas_GL_accvouch.i_GLaccvouch_cc
            CREATE INDEX i_GLaccvouch_cc ON xmis.fas_GL_accvouch(ccode,citem_id)

            DROP INDEX xmis.fas_fitemss00.i_fitemss00_ycpb
            CREATE INDEX i_fitemss00_ycpb ON xmis.fas_fitemss00(year,citemcode)

            DROP INDEX xmis.fas_GL_accass.i_GLaccess_yi
            CREATE INDEX i_GLaccess_yi ON xmis.fas_GL_accass(year,iperiod,citem_id)

            DROP INDEX xmis.fas_GL_accass.i_GLaccess_yicc
            CREATE INDEX i_GLaccess_yicc ON xmis.fas_GL_accass(year,iperiod,citem_class,ccode)

            DROP INDEX xmis.fas_codeType.i_codeType_ccode
            CREATE INDEX i_codeType_ccode ON xmis.fas_codeType(ccode)
	
		
        END
        
        EXEC xmis.sp_convertGLdataToXmis
end        


--sp_getGLaccvouchPerDay