select * into xmis.con_合同_history from con_合同 where 1=2

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER xmis.con_合同_trigger 
   ON xmis.con_合同
   AFTER UPDATE
AS 
BEGIN
	SET NOCOUNT ON;
	insert into xmis.con_合同_history select * from deleted;

END
GO
