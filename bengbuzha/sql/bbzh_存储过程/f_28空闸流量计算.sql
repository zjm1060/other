USE bbz
go
IF OBJECT_ID('f_get28SingleValue') IS NOT NULL 
    BEGIN
        DROP FUNCTION f_get28SingleValue
    END
GO
CREATE FUNCTION f_get28SingleValue
    (
      @ap_w_level DECIMAL(5, 3) ,			--闸上水位
      @bp_w_level DECIMAL(5, 3) ,			--闸下水位
      @gate_open_degree_1 DECIMAL(5, 3) 	--28孔闸开度
    )
RETURNS DECIMAL(9, 3)
AS 
    BEGIN
		/* 
		28孔闸单孔流量计算 
		
		z是水位差（闸上水位-闸下水位），开度0.60米，要在e=0.58和e=0.67之间内插。
		
		例如z=2.25,闸门开度为e=0.60米时，先查e=0.58米时，流量=29，e=0.67米时，流量=34.2,
		每厘米流量为34.2-29=5.2,0.67-0.58=0.09，每厘米流量5.2/9=0.58，
		再计算e=0.60时的流量，29+0.58*2=30.16
		
		*/
		
        DECLARE @first_E DECIMAL(9, 3) ,		--开度范围
            @sec_E DECIMAL(9, 3)
        DECLARE @first_value DECIMAL(9, 4) ,
            @sec_value DECIMAL(9, 4)
        
        DECLARE @z DECIMAL(9, 4) ,
            @singleValue DECIMAL(9, 2)
        
        SET @z = @ap_w_level - @bp_w_level
        SET @singleValue = 0
        
        IF EXISTS ( SELECT  1
                    FROM    xmis.gczl_llgs_28孔节制闸单孔流量检数表
                    WHERE   e_value = @gate_open_degree_1 ) 
            BEGIN
                SELECT  @singleValue = value
                FROM    xmis.gczl_llgs_28孔节制闸单孔流量检数表
                WHERE   e_value = @gate_open_degree_1
                        AND z_value = @z
            END
        ELSE 
            BEGIN
                SELECT  @first_E = MAX(e_value)
                FROM    xmis.gczl_llgs_28孔节制闸单孔流量检数表
                WHERE   e_value < @gate_open_degree_1
                
                SELECT  @sec_E = MIN(e_value)
                FROM    xmis.gczl_llgs_28孔节制闸单孔流量检数表
                WHERE   e_value > @gate_open_degree_1
				
                SELECT  @first_value = value
                FROM    xmis.gczl_llgs_28孔节制闸单孔流量检数表
                WHERE   e_value = @first_E
                        AND z_value = @z
                
                SELECT  @sec_value = value
                FROM    xmis.gczl_llgs_28孔节制闸单孔流量检数表
                WHERE   e_value = @sec_E
                        AND z_value = @z
				
				/* 内插法
				Y= Y1+(Y2-Y1)/(X2-X1)*(X-X1)
				*/
				
				
                SET @singleValue = @first_value + ( @sec_value - @first_value )
                    / ( @sec_E - @first_E ) * ( @gate_open_degree_1 - @first_E )
                    
            END

        IF ( @singleValue IS NULL ) 
            SET @singleValue = 0

			
        RETURN   @singleValue

    END




--select xmis.f_get28SingleValue(18.53,18.05,0.3)



