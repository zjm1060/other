USE bbz
go
IF OBJECT_ID('sp_getLiulangValue') IS NOT NULL 
    BEGIN
        DROP PROCEDURE sp_getLiulangValue
    END
GO
CREATE PROCEDURE sp_getLiulangValue
    (
      @ap_w_level DECIMAL(5, 3) ,			--闸上水位
      @bp_w_level DECIMAL(5, 3) ,			--闸下水位
      @gate_open_degree_2 DECIMAL(5, 3) ,	--12孔闸开度
      @gate_open_num_2 DECIMAL(5, 3) ,		--12孔闸开启孔数
      @gate_open_degree_1 DECIMAL(5, 3) , 	--28孔闸开度
      @gate_open_num_1 DECIMAL(5, 3),		--28孔闸开启孔数
      @elec_machine INT						--电机 数量
      --@ve NVARCHAR(500) OUTPUT
    )
AS 
    BEGIN 
		
		
        DECLARE @waterHeadPrep DECIMAL(18, 4) --水头系数
        DECLARE @ditchWide DECIMAL(18, 4) --渠宽
        DECLARE @netWidePrep SMALLINT --净宽系数
        DECLARE @yiQiangR DECIMAL(18, 2) --翼墙半径
		
        DECLARE @kongLiuYanLiuStates DECIMAL(18, 3) --孔流堰流判断标志
        DECLARE @yanMoChuLiuZiYouChuLiuStates DECIMAL(18, 3) --自由出流与淹没出流判断标志
		
        DECLARE @liuLiangXiShu DECIMAL(18, 4) --流量系数μ值
        
        DECLARE @elecXiShu DECIMAL(18,4)		--电机流量系数
        DECLARE @elecLiuLiang DECIMAL(18,4)		--电站流量
        
        SET @waterHeadPrep = 9
        SET @ditchWide = 141.7 + 12.65 * 2
        SET @netWidePrep = 10
        SET @yiQiangR = 30
                
        SET @kongLiuYanLiuStates = 0.65
        SET @yanMoChuLiuZiYouChuLiuStates = 0.8
        SET @liuLiangXiShu = 0.951
        
        SET @elecXiShu = 25
		SET @elecLiuLiang = @elec_machine * @elecXiShu
        
        
        IF ( @ap_w_level = 0
             OR @bp_w_level = 0
           ) 
            BEGIN
                SELECT  '0' AS 流量
                RETURN
            END
        ELSE 
            IF ( @gate_open_degree_2 <> 0
                 AND @gate_open_num_2 <> 0
               ) 
                BEGIN
                    SELECT  --measure_date ,
                            @ap_w_level AS 上游水位 ,					--A2
                            @bp_w_level AS 下游水位 ,					--B2
                            @gate_open_degree_2 AS 开度 ,				--C2
                            @gate_open_num_2 AS 开启孔数 ,				--D2
                            @ap_w_level - @waterHeadPrep AS 闸上水头 ,	--E2
                            @bp_w_level - @waterHeadPrep AS 闸下水头 ,	--F2
                            @ditchWide AS 渠宽 ,						--G2
                            @gate_open_num_2 * @netWidePrep AS 净宽 ,	--H2
                            @yiQiangR 翼墙半径 ,						--I2
                            @gate_open_degree_2 / ( @ap_w_level
                                                    - @waterHeadPrep ) 堰流孔流判断值 ,	--B5/B6
                            ( @bp_w_level - @waterHeadPrep ) / ( @ap_w_level
                                                              - @waterHeadPrep ) 淹没出流自由出流判断值	--E5/E6
                    INTO    #waterLevel_1
                
                
                    SELECT  * ,
                            CASE WHEN 堰流孔流判断值 >= @kongLiuYanLiuStates
                                 THEN '堰流'
                                 ELSE '孔流'
                            END AS 堰流与孔流判断 ,														--
                            CASE WHEN 堰流孔流判断值 >= @kongLiuYanLiuStates THEN 10
                                 ELSE 20
                            END AS 堰流与孔流常量 ,														--
                            CASE WHEN 淹没出流自由出流判断值 >= @yanMoChuLiuZiYouChuLiuStates
                                 THEN '淹没出流'
                                 ELSE '自由出流'
                            END AS 淹没出流自由出流判断 ,												--
                            CASE WHEN 淹没出流自由出流判断值 >= @yanMoChuLiuZiYouChuLiuStates
                                 THEN 3
                                 ELSE 5
                            END AS 淹没出流自由出流常量 ,												--
                            翼墙半径 / 渠宽 AS [r/B] ,													--C9/C12
                            净宽 / 渠宽 AS [B/Bo] ,														--D9/D12
                            xmis.f_getWuKanKuanDingYanLiuLangXiShu(翼墙半径 / 渠宽,
                                                              净宽 / 渠宽, 'r/B') AS [m值] ,		--E9/E12
                            闸下水头 / 闸上水头 AS [hs/Ho] ,												--F9
                            xmis.f_getWuKanKuanDingYanLiuLangXiShu(闸下水头 / 闸上水头,
                                                              0, 'null') AS [宽顶堰淹没系数σ值] ,		--G9
                            堰流孔流判断值 AS [e/H] ,													--C15
                            xmis.f_getVerticalShrikCoefficient(堰流孔流判断值) AS [垂直收缩系数ε值] 	--D15
                    INTO    #waterLevel_2
                    FROM    #waterLevel_1
                        
				
                    SELECT  * ,
                            开度 * [垂直收缩系数ε值] AS [收缩断面水深hc] ,								--E15
                            @liuLiangXiShu * [垂直收缩系数ε值] AS [流量系数μ值] 						--F15
                    INTO    #waterLevel_3
                    FROM    #waterLevel_2
                
                    SELECT  * ,
                            @liuLiangXiShu * SQRT(2 * 9.8 * ( 闸上水头 - 收缩断面水深hc )) AS [Vc] 		--H15
                    INTO    #waterLevel_4
                    FROM    #waterLevel_3
                
                
                    SELECT  * ,
                            [收缩断面水深hc] * 0.5 * ( SQRT(1 + 8 * [Vc] * [Vc]
                                                      / ( 9.8 * [收缩断面水深hc] ))
                                                 - 1 ) AS [hc"(hc"＜hs)] 								--I15
                    INTO    #waterLevel_5
                    FROM    #waterLevel_4
                                             
				
                    SELECT  * ,
                            ( 闸下水头 - [hc"(hc"＜hs)] ) / ( 闸上水头 - [hc"(hc"＜hs)] ) AS 潜流比 		--J15
                    INTO    #waterLevel_6
                    FROM    #waterLevel_5
                
                    SELECT  * ,
                            xmis.f_getZhaKongYanMoXiShu(潜流比) AS [σ值] ,								--K15
                            0.6 - 0.176 * CONVERT(DECIMAL(18, 10), 开度 / 闸上水头) AS [自由流流量系数μ值]	--F22
                    INTO    #waterLevel_7
                    FROM    #waterLevel_6
				
                    SELECT  * ,
                            堰流与孔流判断 + 淹没出流自由出流判断 AS 流量类型 ,
                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                              WHEN 13
                              THEN 
							/*
							堰流淹没出流 
							Q=εmσB(2g)^0.5H^1.5
							=E9*G9*H2*(2*9.8)^0.5*E2^1.5
							*/ [m值] * [宽顶堰淹没系数σ值] * 净宽 * SQRT(2 * 9.8)
                                   * POWER(闸上水头, 1.5)
                              WHEN 15 THEN 
							/*
							堰流自由出流
							Q=εmB(2g)^0.5H^1.5
							=E12*H2*(2*9.8)^0.5*E2^1.5
							*/ [m值] * 净宽 * SQRT(2 * 9.8) * POWER(闸上水头, 1.5)
                              WHEN 23
                              THEN 
							/*
							孔流淹没出流
							Q=σμBe(2gH)^0.5
							=K15*F15*H2*C2*SQRT(2*9.8*E2)
							*/ [σ值] * [流量系数μ值] * 净宽 * 开度 * SQRT(2 * 9.8 * 闸上水头)
                              WHEN 25 THEN 
							/*
							孔流自由出流
							Q=μBe(2gH)^0.5
							=F22*E22*D22*SQRT(2*9.8*C22)
							*/ [自由流流量系数μ值] * 净宽 * 开度 * SQRT(2 * 9.8 * 闸上水头)
                            END AS 流量
                    INTO    #waterLevel_8
                    FROM    #waterLevel_7

		
                    SELECT  CASE WHEN [hs/Ho] > 0.98
                                 THEN CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              0), @gate_open_num_1
                                      * ( xmis.f_get28SingleValue(@ap_w_level,
                                                              @bp_w_level,
                                                              @gate_open_degree_1) )+ @elecLiuLiang) )
                                 ELSE CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              0), ( 流量
                                                              + @gate_open_num_1
                                                              * xmis.f_get28SingleValue(@ap_w_level,
                                                              @bp_w_level,
                                                              @gate_open_degree_1) + @elecLiuLiang)))
                            END AS 流量
                    FROM    #waterLevel_8				
					
					return
					
                END
            ELSE 
                IF ( @gate_open_degree_2 = 0
                     OR @gate_open_num_2 = 0
                   ) 
                    BEGIN
                        SELECT  CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18, 0), @gate_open_num_1
                                * ( xmis.f_get28SingleValue(@ap_w_level,
                                                            @bp_w_level,
                                                            @gate_open_degree_1) )+ @elecLiuLiang))  AS 流量
						RETURN 
                    END

                
		
        --RETURN @ve
    END



		/*

DECLARE @vde NVARCHAR(100)

EXEC xmis.sp_getLiulangValue 17.09,17.05,99,12,@vde OUTPUT

SELECT @vde

		*/


--xmis.sp_getLiulangValue 18.53,14.05,1.1,12,0.3,28,2
--xmis.sp_getLiulangValue 0,14.05,1.1,12,0.3,28
--xmis.sp_getLiulangValue 18.53,14.05,0,12,0.3,28
--xmis.sp_getLiulangValue 18.53,14.05,1.1,12,0,28






