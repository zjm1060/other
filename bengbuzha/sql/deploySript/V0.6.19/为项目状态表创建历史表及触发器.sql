select * into xmis.prj_项目状态_history from xmis.prj_项目状态

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.prj_项目状态_trigger 
   ON xmis.prj_项目状态
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @ID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = INSERTED.项目状态ID FROM INSERTED
			insert into xmis.prj_项目状态_history select * from xmis.prj_项目状态 WHERE 项目状态ID = @ID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = DELETED.项目状态ID FROM DELETED
			insert into xmis.prj_项目状态_history select * from xmis.prj_项目状态 WHERE 项目状态ID = @ID;
		end
END
GO