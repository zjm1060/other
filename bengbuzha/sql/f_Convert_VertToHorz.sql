Use xmis
go
IF OBJECT_ID('f_Convert_VertToHorz') IS NOT NULL
BEGIN
    DROP FUNCTION f_Convert_VertToHorz
END
GO
CREATE FUNCTION f_Convert_VertToHorz(@id INT, @type NVARCHAR(20))
RETURNS VARCHAR(200)
AS 
    BEGIN 

        DECLARE @horzStr NVARCHAR(2000)
        SET @horzStr = CONVERT(NVARCHAR(10), @id)
        
        IF ( @type = 'DeptBelongToDept' ) 
            SELECT  @horzStr = CONVERT(NVARCHAR(10), 部门ID) + ',' + @horzStr
            FROM    xmis.org_部门归属部门表
            WHERE   父部门ID = @id

        --IF LEN(@horzStr) > 0 
          --  SET @horzStr = LEFT(@horzStr, LEN(@horzStr) - 1)


        RETURN @horzStr
    END

