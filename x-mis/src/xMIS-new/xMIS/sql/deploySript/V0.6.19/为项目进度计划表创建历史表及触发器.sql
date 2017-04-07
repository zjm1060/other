select * into xmis.prj_项目进度计划_history from xmis.prj_项目进度计划

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.prj_项目进度计划_trigger 
   ON xmis.prj_项目进度计划
   AFTER INSERT,UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	DECLARE @ID INT
	
	IF EXISTS (SELECT 1 FROM INSERTED ) AND NOT EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = INSERTED.ID FROM INSERTED
			insert into xmis.prj_项目进度计划_history select * from xmis.prj_项目进度计划 WHERE ID = @ID;
		end
	ELSE IF EXISTS (SELECT 1 FROM INSERTED ) AND EXISTS (SELECT 1 FROM DELETED)
		begin
			SELECT @ID = DELETED.ID FROM DELETED
			insert into xmis.prj_项目进度计划_history select * from xmis.prj_项目进度计划 WHERE ID = @ID;
		end
END
GO