select * into xmis.org_公司_history from xmis.org_公司

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.org_公司_trigger 
   ON xmis.org_公司
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @compID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @compID = INSERTED.公司ID FROM INSERTED
			insert into xmis.org_公司_history select * from xmis.org_公司 WHERE 公司ID = @compID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @compID = DELETED.公司ID FROM DELETED
			insert into xmis.org_公司_history select * from xmis.org_公司 WHERE 公司ID = @compID;
		end
END
GO