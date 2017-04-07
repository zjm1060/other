select * into xmis.prj_项目_history from xmis.prj_项目

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.prj_项目_trigger 
   ON xmis.prj_项目
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @ID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = INSERTED.项目ID FROM INSERTED
			insert into xmis.prj_项目_history select * from xmis.prj_项目 WHERE 项目ID = @ID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = DELETED.项目ID FROM DELETED
			insert into xmis.prj_项目_history select * from xmis.prj_项目 WHERE 项目ID = @ID;
		end
END
GO