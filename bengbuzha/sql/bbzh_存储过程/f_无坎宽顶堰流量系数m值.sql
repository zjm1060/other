USE bbz
go
IF OBJECT_ID('f_getWuKanKuanDingYanLiuLangXiShu') IS NOT NULL 
    BEGIN
        DROP FUNCTION f_getWuKanKuanDingYanLiuLangXiShu
    END
GO
CREATE FUNCTION f_getWuKanKuanDingYanLiuLangXiShu
    (
      @rB DECIMAL(9, 3) ,
      @BBo DECIMAL(9, 3) ,
      @type NVARCHAR(20)
    )
RETURNS DECIMAL(9, 4)
AS 
    BEGIN 
    
        DECLARE @returnValue DECIMAL(9, 4)
        SET @returnValue = 0
        IF @type != 'null' 
            BEGIN
			/* 无坎宽顶堰流量系数m值 */
				
				IF @rB>0.5 
					SET @rB=0.5
					
                SELECT  @returnValue = value
                FROM    xmis.gczl_llgs_无坎宽顶堰流量系数
                WHERE   type = @type
                        AND x_value = CONVERT(DECIMAL(5,1),@BBo)
                        AND y_value = CASE @rB WHEN 0.05 THEN @rB ELSE CONVERT(DECIMAL(5,1),@rB) END
				
            END
		ELSE	
			BEGIN
			/*宽顶堰淹没系数*/	
			
				SELECT @returnValue = value from xmis.gczl_llgs_宽顶堰淹没系数 WHERE hsH = CONVERT(DECIMAL(5,1),@rB)
			END		
				    
        
			
        RETURN @returnValue
    END



		/*
select sqrt(2*9.8) 
select 43.56*sqrt(2*9.8)  *power(8.11000,1.5)

select power(8.11000,1.5)

SELECT * FROM xmis.gczl_llgsh_锐缘平面闸门垂向收缩系数
		
		*/

--select xmis.f_getWuKanKuanDingYanLiuLangXiShu(0.18,0.72,'r/B')
--select xmis.f_getWuKanKuanDingYanLiuLangXiShu(0.9,0.9,'null')

