USE xmis
go
IF OBJECT_ID('f_UpdateProjCodeOnFas') IS NOT NULL 
    BEGIN
        DROP FUNCTION f_UpdateProjCodeOnFas
    END
GO
CREATE FUNCTION f_UpdateProjCodeOnFas ( @citemID NVARCHAR(200) )
RETURNS VARCHAR(200)
AS 
    BEGIN 
		
        DECLARE @projectCode NVARCHAR(200) ,
            @count INT ,
            @icount INT
        
        SET @projectCode = CONVERT(NVARCHAR(200), NULL)

        SELECT  @count = COUNT(*)
        FROM    xmis.prj_项目
        WHERE   RTRIM(LTRIM(REPLACE(项目编码, '-', ''))) LIKE '%'
                + REPLACE(@citemID, '000', '')  --@citemID
        
        
        SELECT  @icount = COUNT(*)
        FROM    xmis.prj_项目
        WHERE   RTRIM(LTRIM(REPLACE(项目编码, '-', ''))) LIKE '%' + @citemID  --@citemID
        
        IF ( @count = 1 ) 
            SELECT  @projectCode = 项目编码
            FROM    xmis.prj_项目
            WHERE   RTRIM(LTRIM(REPLACE(项目编码, '-', ''))) LIKE '%'
                    + REPLACE(@citemID, '000', '')  --@citemID
			/*
        ELSE 
            BEGIN
                SELECT  @count = COUNT(*)
                FROM    xmis.prj_项目
                WHERE   REPLACE(项目编码, '-', '') + '000' LIKE '%' + @citemID
                IF ( @count = 1 ) 
                    SELECT  @projectCode = 项目编码
                    FROM    xmis.prj_项目
                    WHERE   REPLACE(项目编码, '-', '') + '000' LIKE '%' + @citemID
            END
         */
		
        ELSE 
            IF ( @icount = 1 ) 
                SELECT  @projectCode = 项目编码
                FROM    xmis.prj_项目
                WHERE   RTRIM(LTRIM(REPLACE(项目编码, '-', ''))) LIKE '%'
                        + @citemID  --@citemID

        RETURN @projectCode

    END




--select xmis.f_UpdateProjCodeOnFas('JK2005GC003000')


