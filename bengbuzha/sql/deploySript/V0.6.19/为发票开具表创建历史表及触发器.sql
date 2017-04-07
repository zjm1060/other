select * into xmis.con_发票开具表_history from xmis.con_发票开具表

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.con_发票开具表_trigger 
   ON xmis.con_发票开具表
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @cID INT,@iID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @cID = INSERTED.合同ID,@iID = INSERTED.发票号 FROM INSERTED
			insert into xmis.con_发票开具表_history select * from xmis.con_发票开具表 WHERE 合同ID = @cID and 发票号 = @iID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @cID = DELETED.合同ID,@iID = DELETED.发票号 FROM DELETED
			insert into xmis.con_发票开具表_history select * from xmis.con_发票开具表 WHERE 合同ID = @cID and 发票号 = @iID;
		end
END
GO