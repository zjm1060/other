USE xmis
go
IF OBJECT_ID('sp_getGLaccvouchPerQuarter') IS NOT NULL 
    BEGIN
        DROP PROCEDURE sp_getGLaccvouchPerQuarter
    END
GO
CREATE PROC sp_getGLaccvouchPerQuarter
AS 
    BEGIN
        DECLARE @year CHAR(4) ,
            @TableName NVARCHAR(100) 
            
        SET @year = CONVERT(CHAR(4), YEAR(DATEADD("d", -1, GETDATE())))
        SET @TableName = 'UFDATA_302_' + @year + '.dbo.GL_accvouch'

	--xmis.fas_GL_accvouch
        EXEC ( ' DELETE xmis.fas_GL_accvouch WHERE year = '+@year+' AND iperiod = MONTH(GETDATE()) ' 
        + ' AND CONVERT(NVARCHAR(10),dbill_date,101) = CONVERT(NVARCHAR(10),GETDATE(),101)'
        + ' INSERT INTO xmis.fas_GL_accvouch SELECT ' + @year
        + ',* FROM OPENROWSET(''SQLOLEDB'',''192.168.8.8'';''sa'';''123456'',' + @TableName
        --+ ') WHERE  CONVERT(NVARCHAR(10),dbill_date,101) = CONVERT(CHAR(10),DATEADD("d",-1,GETDATE()),101) ' )
        + ') WHERE iperiod = MONTH(GETDATE()) '
        + ' AND CONVERT(NVARCHAR(10),dbill_date,101) = CONVERT(NVARCHAR(10),GETDATE(),101)'
         )
            
		EXEC xmis.sp_convertGLdataToXmis

    END
	


--sp_getGLaccvouchPerQuarter

