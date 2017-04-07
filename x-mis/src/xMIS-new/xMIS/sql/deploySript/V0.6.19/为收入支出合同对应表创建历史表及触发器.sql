select * into xmis.con_收入支出合同对应表_history from xmis.con_收入支出合同对应表

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.con_收入支出合同对应表_trigger 
   ON xmis.con_收入支出合同对应表
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @ID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = INSERTED.对应ID FROM INSERTED
			insert into xmis.con_收入支出合同对应表_history select * from xmis.con_收入支出合同对应表 WHERE 对应ID = @ID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = DELETED.对应ID FROM DELETED
			insert into xmis.con_收入支出合同对应表_history select * from xmis.con_收入支出合同对应表 WHERE 对应ID = @ID;
		end
END
GO