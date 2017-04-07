USE [xmis]
GO
/****** Object:  StoredProcedure [xmis].[sp_getGLaccvouchPerQuarter]    Script Date: 05/03/2012 17:11:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROC [xmis].[sp_getGLaccvouchPerQuarter]
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

