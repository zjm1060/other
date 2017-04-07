USE bbz
go
IF OBJECT_ID('f_getVerticalShrikCoefficient') IS NOT NULL 
    BEGIN
        DROP FUNCTION f_getVerticalShrikCoefficient
    END
GO
CREATE FUNCTION f_getVerticalShrikCoefficient ( @eH DECIMAL(9, 3))
RETURNS DECIMAL(9, 3)
AS 
    BEGIN
		/* 锐缘平面闸门垂向收缩系数 */
		
        DECLARE @prevEH DECIMAL(9, 3) ,
            @nextEH DECIMAL(9, 3)
        DECLARE @prevVE DECIMAL(9, 4) ,
            @nextVE DECIMAL(9, 4)
        
        DECLARE @ve DECIMAL(9, 4)
        SET @ve = 0
        
        IF EXISTS ( SELECT  1
                    FROM    xmis.gczl_llgsh_锐缘平面闸门垂向收缩系数
                    WHERE   eH = @eH ) 
            BEGIN
                SELECT  @ve = 垂向收缩系数
                FROM    xmis.gczl_llgsh_锐缘平面闸门垂向收缩系数
                WHERE   eH = @eH
            END
        ELSE 
            BEGIN
                SELECT  @prevEH = MAX(eh)
                FROM    xmis.gczl_llgsh_锐缘平面闸门垂向收缩系数
                WHERE   eh < @eH
                
                SELECT  @nextEH = MIN(eh)
                FROM    xmis.gczl_llgsh_锐缘平面闸门垂向收缩系数
                WHERE   eh > @eH
				
                SELECT  @prevVE = 垂向收缩系数
                FROM    xmis.gczl_llgsh_锐缘平面闸门垂向收缩系数
                WHERE   eh = @prevEH
                
                SELECT  @nextVE = 垂向收缩系数
                FROM    xmis.gczl_llgsh_锐缘平面闸门垂向收缩系数
                WHERE   eh = @nextEH
				
				/* 内插法
				Y= Y1+(Y2-Y1)/(X2-X1)*(X-X1)
				*/
				
                SET @ve = @prevVE + (@nextVE - @prevVE)/(@nextEH - @prevEH)*(@eH-@prevEH)
            END
			
        RETURN @ve
    END



		/*
select sqrt(2*9.8) 
select 43.56*sqrt(2*9.8)  *power(8.11000,1.5)

select power(8.11000,1.5)

SELECT * FROM xmis.gczl_llgsh_锐缘平面闸门垂向收缩系数

		
		*/

--select xmis.f_getVerticalShrikCoefficient(0.42)

