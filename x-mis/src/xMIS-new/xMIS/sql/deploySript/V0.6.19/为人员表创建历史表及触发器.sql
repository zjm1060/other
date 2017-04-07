select * into xmis.hr_人员_history from xmis.hr_人员

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.hr_人员_trigger 
   ON xmis.hr_人员
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @ID INT, @oldID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = INSERTED.人员ID FROM INSERTED
			insert into xmis.hr_人员_history select * from xmis.hr_人员 WHERE 人员ID = @ID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = DELETED.人员ID FROM DELETED
			insert into xmis.hr_人员_history select * from xmis.hr_人员 WHERE 人员ID = @ID;
		end
END
GO