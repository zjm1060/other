select * into xmis.con_收据明细表_history from xmis.con_收据明细表

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.con_收据明细表_trigger 
   ON xmis.con_收据明细表
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @cID INT,@iID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @cID = INSERTED.合同ID,@iID = INSERTED.收据ID FROM INSERTED
			insert into xmis.con_收据明细表_history select * from xmis.con_收据明细表 WHERE 合同ID = @cID and 收据ID = @iID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @cID = DELETED.合同ID,@iID = DELETED.收据ID FROM DELETED
			insert into xmis.con_收据明细表_history select * from xmis.con_收据明细表 WHERE 合同ID = @cID and 收据ID = @iID;
		end
END
GO