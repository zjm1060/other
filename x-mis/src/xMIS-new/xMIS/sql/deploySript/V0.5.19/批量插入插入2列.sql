SELECT distinct Name into #tableName1 FROM SysObjects Where XType='U' ORDER BY Name

select * into #tableName from #tableName1 where Name not like 'fas%'

use xmis
begin
DECLARE @TableName varchar(100);
DECLARE cur CURSOR FOR SELECT Name FROM #tableName 
    
    OPEN cur
    FETCH NEXT FROM cur INTO @TableName

    WHILE @@fetch_status = 0 
        BEGIN
			EXEC( 'alter table xmis.'+@TableName+' add 最后修改日期 datetime,最后修改人ID int');
            FETCH NEXT FROM cur INTO @TableName
        END
    CLOSE cur
    DEALLOCATE cur
end



