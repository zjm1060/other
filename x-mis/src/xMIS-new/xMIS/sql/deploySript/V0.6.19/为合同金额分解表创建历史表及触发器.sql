select * into xmis.con_合同金额分解_history from xmis.con_合同金额分解

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.con_合同金额分解_trigger 
   ON xmis.con_合同金额分解
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @ID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = INSERTED.ID FROM INSERTED
			insert into xmis.con_合同金额分解_history select * from xmis.con_合同金额分解 WHERE ID = @ID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = DELETED.ID FROM DELETED
			insert into xmis.con_合同金额分解_history select * from xmis.con_合同金额分解 WHERE ID = @ID;
		end
END
GO