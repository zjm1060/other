insert into xmis.con_合同_history select * from xmis.con_合同

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.con_合同_trigger 
   ON xmis.con_合同
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @ID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = INSERTED.合同ID FROM INSERTED
			insert into xmis.con_合同_history select * from xmis.con_合同 WHERE 合同ID = @ID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = DELETED.合同ID FROM DELETED
			insert into xmis.con_合同_history select * from xmis.con_合同 WHERE 合同ID = @ID;
		end
END
GO