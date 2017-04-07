USE bbz
go
IF OBJECT_ID('sp_getLiulangTotalValue') IS NOT NULL 
    BEGIN
        DROP PROCEDURE sp_getLiulangTotalValue
    END
GO
CREATE PROCEDURE sp_getLiulangTotalValue
    (
      @begDate CHAR(10) ,
      @endDate CHAR(10) ,
      @floodgateType SMALLINT 
    )
AS 
    BEGIN 

        DECLARE @ve DECIMAL(9, 4)
        SET @ve = 0
		SET NOCOUNT ON 
        DECLARE @waterHeadPrep DECIMAL(18, 4) --水头系数
        DECLARE @ditchWide DECIMAL(18, 4) --渠宽
        DECLARE @netWidePrep SMALLINT --净宽系数
        DECLARE @yiQiangR DECIMAL(18, 2) --翼墙半径
		
        DECLARE @kongLiuYanLiuStates DECIMAL(18, 3) --孔流堰流判断标志
        DECLARE @yanMoChuLiuZiYouChuLiuStates DECIMAL(18, 3) --自由出流与淹没出流判断标志
		
        DECLARE @liuLiangXiShu DECIMAL(18, 4) --流量系数μ值


        DECLARE @elecXiShu DECIMAL(18,4)		--电机流量系数
        
        --IF @floodgateType = 12 
			--12孔闸 流量计算
        BEGIN
            SET @waterHeadPrep = 9
            SET @ditchWide = 141.7 + 12.65 * 2
            SET @netWidePrep = 10
            SET @yiQiangR = 30
                
            SET @kongLiuYanLiuStates = 0.65
            SET @yanMoChuLiuZiYouChuLiuStates = 0.8
            SET @liuLiangXiShu = 0.951
                
            SELECT  tab_seed ,
                    measure_date ,
                    ap_w_level AS 上游水位 ,					--A2
                    bp_w_level AS 下游水位 ,					--B2
                    gate_open_degree_2 AS 开度 ,				--C2
                    gate_open_num_2 AS 开启孔数 ,				--D2
                    ap_w_level - @waterHeadPrep AS 闸上水头 ,	--E2
                    bp_w_level - @waterHeadPrep AS 闸下水头 ,	--F2
                    @ditchWide AS 渠宽 ,						--G2
                    gate_open_num_2 * @netWidePrep AS 净宽 ,	--H2
                    @yiQiangR 翼墙半径 ,						--I2
                    gate_open_degree_2 / ( ap_w_level - @waterHeadPrep ) 堰流孔流判断值 ,	--B5/B6
                    ( bp_w_level - @waterHeadPrep ) / ( ap_w_level
                                                        - @waterHeadPrep ) 淹没出流自由出流判断值	--E5/E6
            INTO    #waterLevel_1
            FROM    xmis.gczl_water_level_info
            WHERE   measure_date < DATEADD(D, 1, @endDate)
                    AND measure_date >= @begDate
                    AND gate_open_num_2 > 0		--开闸放水
                    AND gate_open_degree_2 > 0
                
            SELECT  * ,
                    CASE WHEN 堰流孔流判断值 >= @kongLiuYanLiuStates THEN '堰流'
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
                    xmis.f_getWuKanKuanDingYanLiuLangXiShu(翼墙半径 / 渠宽, 净宽 / 渠宽,
                                                           'r/B') AS [m值] ,		--E9/E12
                    闸下水头 / 闸上水头 AS [hs/Ho] ,												--F9
                    xmis.f_getWuKanKuanDingYanLiuLangXiShu(闸下水头 / 闸上水头, 0,
                                                           'null') AS [宽顶堰淹没系数σ值] ,		--G9
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
                    [收缩断面水深hc] * 0.5 * ( SQRT(1 + 8 * [Vc] * [Vc] / ( 9.8
                                                              * [收缩断面水深hc] ))
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

			--28孔闸流量
            SELECT  tab_seed ,
                    measure_date ,
                    ap_w_level AS 上游水位 ,					--A2
                    bp_w_level AS 下游水位 ,					--B2
                    gate_open_degree_1 AS 开度 ,				--C2
                    gate_open_num_1 AS 开启孔数 ,				--D2
                    ap_w_level - @waterHeadPrep AS 闸上水头 ,	--E2
                    bp_w_level - @waterHeadPrep AS 闸下水头 ,	--F2
                    gate_open_num_1 * xmis.f_get28SingleValue(ap_w_level,
                                                              bp_w_level,
                                                              gate_open_degree_1) AS 流量
            INTO    #waterLevel_9
            FROM    xmis.gczl_water_level_info
            WHERE   measure_date < DATEADD(D, 1, @endDate)
                    AND measure_date >= @begDate
                    AND gate_open_num_1 > 0		--开闸放水
                    AND gate_open_degree_1 > 0
                    
			--电站流量
			SET @elecXiShu = 25
			
            SELECT  tab_seed ,
                    measure_date ,
                    ap_w_level AS 上游水位 ,					--A2
                    bp_w_level AS 下游水位 ,					--B2
                    gate_open_degree_1 AS 开度 ,				--C2
                    gate_open_num_1 AS 开启孔数 ,				--D2
                    ap_w_level - @waterHeadPrep AS 闸上水头 ,	--E2
                    bp_w_level - @waterHeadPrep AS 闸下水头 ,	--F2
                    elec_machine AS 电机数 ,
                    CONVERT(DECIMAL(18,2), @elecXiShu * elec_machine) AS 电站流量
            INTO #waterLevel_10
            FROM xmis.gczl_water_level_info WHERE elec_machine >0
			
                
            IF @begDate = @endDate 
                BEGIN
                
                    SELECT  CONVERT(NVARCHAR(100), NULL) AS A ,
                            CONVERT(NVARCHAR(100), NULL) AS B ,
                            CONVERT(NVARCHAR(100), NULL) AS C ,
                            CONVERT(NVARCHAR(100), NULL) AS D ,
                            CONVERT(NVARCHAR(100), NULL) AS E ,
                            CONVERT(NVARCHAR(100), NULL) AS F ,
                            CONVERT(NVARCHAR(100), NULL) AS G ,
                            CONVERT(NVARCHAR(100), NULL) AS H ,
                            CONVERT(NVARCHAR(100), NULL) AS I ,
                            CONVERT(NVARCHAR(100), NULL) AS J ,
                            CONVERT(NVARCHAR(100), NULL) AS K ,
                            CONVERT(NVARCHAR(100), NULL) AS L
                    INTO    #water_Temp
                       
					
                    IF EXISTS ( SELECT  1
                                FROM    #waterLevel_8
                                WHERE   CONVERT(CHAR(10), measure_date, 23) = @begDate ) 
                        BEGIN
                            INSERT  INTO #water_Temp
                                    SELECT  '<font color=red>12孔闸流量明细</font>' AS A ,
                                            '' AS B ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    UNION ALL
                                    SELECT  '上游水位' ,
                                            '下游水位' ,
                                            '开度e' ,
                                            '开启孔数n' ,
                                            '闸上水头H' ,
                                            '闸下水头h' ,
                                            '渠宽B' ,
                                            '净宽b' ,
                                            '翼墙半径r' ,
                                            '' ,
                                            '' ,
                                            ''
                                    UNION ALL
                                    SELECT  CONVERT(NVARCHAR(50), 上游水位) ,
                                            CONVERT(NVARCHAR(50), 下游水位) ,
                                            CONVERT(NVARCHAR(50), 开度) ,
                                            CONVERT(NVARCHAR(50), 开启孔数) ,
                                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 闸上水头)) ,
                                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 闸下水头)) ,
                                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 渠宽)) ,
                                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 净宽)) ,
                                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 翼墙半径)) ,
                                            '' ,
                                            '' ,
                                            ''
                                    FROM    #waterLevel_8
                                    UNION ALL
                                    SELECT  '' AS A ,
                                            '' AS B ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    UNION ALL
                                    SELECT  '' AS A ,
                                            '' AS B ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    UNION ALL
                                    SELECT  CASE 堰流与孔流常量
                                              WHEN 10 THEN '堰流'
                                              ELSE '孔流'
                                            END ,
                                            'e/H' ,
                                            '' ,
                                            '' ,
                                            '' ,
                                            CASE 淹没出流自由出流常量
                                              WHEN 3 THEN '淹没出流'
                                              ELSE '自由出流'
                                            END ,
                                            'hs/Ho' ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    FROM    #waterLevel_8
                                    UNION ALL
                                    SELECT  CASE 堰流与孔流常量
                                              WHEN 10 THEN 'e/H≥0.65'
                                              ELSE 'e/H＜0.65'
                                            END ,
                                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 堰流孔流判断值)) ,
                                            '' ,
                                            '' ,
                                            '' ,
                                            CASE 淹没出流自由出流常量
                                              WHEN 3 THEN 'hs/Ho≥0.8'
                                              ELSE 'hs/Ho＜0.8'
                                            END ,
                                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 淹没出流自由出流判断值)) ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    FROM    #waterLevel_8
                                    UNION ALL
                                    SELECT  '' AS A ,
                                            '' AS B ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    UNION ALL
                                    SELECT  '' AS A ,
                                            '' AS B ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    UNION ALL
                                    SELECT  '流量类型' ,
                                            '计算公式' ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   'r/B'
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   'r/B'
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   'e/H'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   '闸上水头H'
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   'b/Bo'
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   'b/Bo'
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   '垂直收缩系数ε值'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   '开度e'
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   '查m值'
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   '查m值'
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   '收缩断面水深hc'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   '净宽B'
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   'hs/Ho'
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   '流量'
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   '流量系数μ值'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   '流量系数μ值'
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   '查σ值'
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   '闸上水头H'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   '流量'
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   '流量'
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   'Vc'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   ''
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   'hc"(hc"＜hs)'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   ''
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   '潜流比'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   ''
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   '查σ值'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   ''
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   '流量'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END
                                    FROM    #waterLevel_8
                                    UNION ALL
                                    SELECT  流量类型 ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   'Q=εmσB(2g)^0.5H^1.5'
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   'Q=εmB(2g)^0.5H^1.5'
                                              WHEN 23 THEN 
									--孔流淹没出流
                                                   'Q=σμBe(2gH)^0.5'
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   'Q=μBe(2gH)^0.5'
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13
                                              THEN 
									--堰流淹没出流 
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), [r/B]))
                                              WHEN 15
                                              THEN 
									--堰流自由出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), [r/B]))
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), [e/H]))
                                              WHEN 25
                                              THEN 
									--孔流自由出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 闸上水头))
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13
                                              THEN 
									--堰流淹没出流 
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), [B/Bo]))
                                              WHEN 15
                                              THEN 
									--堰流自由出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), [B/Bo]))
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(9,
                                                              4), [垂直收缩系数ε值]))
                                              WHEN 25
                                              THEN 
									--孔流自由出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 开度))
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13
                                              THEN 
									--堰流淹没出流 
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(9,
                                                              3), [m值]))
                                              WHEN 15
                                              THEN 
									--堰流自由出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(9,
                                                              3), [m值]))
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(9,
                                                              3), [收缩断面水深hc]))
                                              WHEN 25
                                              THEN 
									--孔流自由出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 净宽))
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13
                                              THEN 
									--堰流淹没出流 
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), [hs/Ho]))
                                              WHEN 15
                                              THEN 
									--堰流自由出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              4), 流量))
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(9,
                                                              6), [流量系数μ值]))
                                              WHEN 25
                                              THEN 
									--孔流自由出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(9,
                                                              6), [自由流流量系数μ值]))
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13
                                              THEN 
									--堰流淹没出流 
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), [宽顶堰淹没系数σ值]))
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 闸上水头))
                                              WHEN 25
                                              THEN 
									--孔流自由出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              4), 流量))
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13
                                              THEN 
									--堰流淹没出流
                                                   CASE WHEN [hs/Ho] > 0.98
                                                        THEN '<font color=red> hs/Ho＞0.98,无法计算流量</font>'
                                                        ELSE CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              4), 流量))
                                                   END
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(9,
                                                              6), [Vc]))
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   ''
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), [hc"(hc"＜hs)]))
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   ''
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(5,
                                                              2), 潜流比))
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   ''
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(9,
                                                              3), [σ值]))
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END ,
                                            CASE 堰流与孔流常量 + 淹没出流自由出流常量
                                              WHEN 13 THEN 
									--堰流淹没出流 
                                                   ''
                                              WHEN 15 THEN 
									--堰流自由出流
                                                   ''
                                              WHEN 23
                                              THEN 
									--孔流淹没出流
                                                   CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              4), 流量))
                                              WHEN 25 THEN 
									--孔流自由出流
                                                   ''
                                            END
                                    FROM    #waterLevel_8
                        END
                        
                    IF EXISTS ( SELECT  1
                                FROM    #waterLevel_9
                                WHERE   CONVERT(CHAR(10), measure_date, 23) = @begDate ) 
                        BEGIN
                            INSERT  INTO #water_Temp
                                    SELECT  '' AS A ,
                                            '' AS B ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    UNION ALL
                                    SELECT  '' AS A ,
                                            '' AS B ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    UNION ALL
                                    SELECT  '<font color=red>28孔闸流量:</font>' ,
                                            流量 ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    FROM    #waterLevel_9
                                    WHERE   CONVERT(CHAR(10), measure_date, 23) = @begDate
                                    UNION ALL
                                    SELECT  '' AS A ,
                                            '' AS B ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    UNION ALL
                                    SELECT  '' AS A ,
                                            '' AS B ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    UNION ALL
                                    SELECT  '<font color=red>电站流量:</font>' ,
                                            电站流量 ,
                                            '' AS C ,
                                            '' AS D ,
                                            '' AS E ,
                                            '' AS F ,
                                            '' AS G ,
                                            '' AS H ,
                                            '' AS I ,
                                            '' AS J ,
                                            '' AS K ,
                                            '' AS L
                                    FROM    #waterLevel_10
                                    WHERE   CONVERT(CHAR(10), measure_date, 23) = @begDate
                        END
                        
                    SELECT  *
                    FROM    #water_Temp
					
                END
            ELSE 
                BEGIN
					
					
                    SELECT  w_8.tab_seed, CONVERT(CHAR(10), w_8.measure_date, 23) 日期 ,
                            w_8.上游水位 ,
                            w_8.下游水位 ,
                            w_8.开度 AS [12孔闸开度] ,
                            w_8.开启孔数 AS [12孔闸开启孔数] ,
                            w_9.开度 AS [28孔闸开度] ,
                            w_9.开启孔数 AS [28孔闸开启孔数] ,
                            CONVERT(DECIMAL(5, 2), w_8.闸上水头) AS 闸上水头 ,
                            CONVERT(DECIMAL(5, 2), w_8.闸下水头) AS 闸下水头 ,
                            w_8.流量类型 AS [12孔闸流量类型] ,
                            CASE WHEN [hs/Ho] > 0.98
                                 THEN '<font color=red> hs/Ho＞0.98,无法计算流量</font>'
                                 ELSE CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              2), w_8.流量))
                            END AS [12孔闸流量] ,
                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18, 2), w_9.流量)) AS [28孔闸流量] ,
                            CASE WHEN [hs/Ho] > 0.98
                                 THEN CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              2), w_9.流量))
                                 ELSE CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              2), ( w_8.流量
                                                              + w_9.流量 )))
                            END AS 总流量
					INTO #waterLevel_11                            
                    FROM    #waterLevel_8 AS w_8 ,
                            #waterLevel_9 AS w_9
                    WHERE   w_8.tab_seed = w_9.tab_seed
                    UNION
                    SELECT  w_8.tab_seed, CONVERT(CHAR(10), w_8.measure_date, 23) 日期 ,
                            w_8.上游水位 ,
                            w_8.下游水位 ,
                            w_8.开度 AS [12孔闸开度] ,
                            w_8.开启孔数 AS [12孔闸开启孔数] ,
                            NULL AS [28孔闸开度] ,
                            NULL AS [28孔闸开启孔数] ,
                            CONVERT(DECIMAL(5, 2), w_8.闸上水头) AS 闸上水头 ,
                            CONVERT(DECIMAL(5, 2), w_8.闸下水头) AS 闸下水头 ,
                            w_8.流量类型 AS [12孔闸流量类型] ,
                            CASE WHEN [hs/Ho] > 0.98
                                 THEN '<font color=red> hs/Ho＞0.98,无法计算流量</font>'
                                 ELSE CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              2), w_8.流量))
                            END AS [12孔闸流量] ,
                            NULL AS [28孔闸流量] ,
                            CASE WHEN [hs/Ho] > 0.98
                                 THEN '<font color=red> hs/Ho＞0.98,无法计算流量</font>'
                                 ELSE CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18,
                                                              2), ( w_8.流量 )))
                            END AS 总流量
                    FROM    #waterLevel_8 AS w_8 ,
                            #waterLevel_9 AS w_9
                    WHERE   w_8.tab_seed NOT IN ( SELECT    tab_seed
                                                  FROM      #waterLevel_9 )
                    UNION
                    SELECT  w_9.tab_seed, CONVERT(CHAR(10), w_9.measure_date, 23) 日期 ,
                            w_9.上游水位 ,
                            w_9.下游水位 ,
                            NULL AS [12孔闸开度] ,
                            NULL AS [12孔闸开启孔数] ,
                            w_9.开度 AS [28孔闸开度] ,
                            w_9.开启孔数 AS [28孔闸开启孔数] ,
                            CONVERT(DECIMAL(5, 2), w_9.闸上水头) AS 闸上水头 ,
                            CONVERT(DECIMAL(5, 2), w_9.闸下水头) AS 闸下水头 ,
                            NULL AS [12孔闸流量类型] ,
                            NULL AS [12孔闸流量] ,
                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18, 2), w_9.流量)) AS [28孔闸流量] ,
                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18, 2), ( w_9.流量 ))) AS 总流量
                    FROM    #waterLevel_8 AS w_8 ,
                            #waterLevel_9 AS w_9
                    WHERE   w_9.tab_seed NOT IN ( SELECT    tab_seed
                                                  FROM      #waterLevel_8 )
                    --ORDER BY 日期 DESC
                    
                    SELECT  w_11.日期 ,
							w_11.上游水位,
							w_11.下游水位,
                            w_11.[12孔闸开度] ,
                            w_11.[12孔闸开启孔数] ,
                            w_11.[28孔闸开度] ,
                            w_11.[28孔闸开启孔数] ,
                            w_10.电机数,
                            w_11.闸上水头 ,
                            w_11.闸下水头 ,
                            w_11.[12孔闸流量类型] ,
                            w_11.[12孔闸流量] ,
                            w_11.[28孔闸流量] ,
                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18, 2),w_10.电站流量)) AS 电站流量,
                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18, 2),总流量+w_10.电站流量)) AS 总流量
                    FROM #waterLevel_11 w_11, #waterLevel_10 w_10
                    WHERE w_11.tab_seed = w_10.tab_seed
                    UNION 
                    SELECT  w_11.日期 ,
							w_11.上游水位,
							w_11.下游水位,
                            w_11.[12孔闸开度] ,
                            w_11.[12孔闸开启孔数] ,
                            w_11.[28孔闸开度] ,
                            w_11.[28孔闸开启孔数] ,
                            NULL AS 电机数,
                            w_11.闸上水头 ,
                            w_11.闸下水头 ,
                            w_11.[12孔闸流量类型] ,
                            w_11.[12孔闸流量] ,
                            w_11.[28孔闸流量] ,
                            NULL 电站流量,
                            w_11.总流量
                    FROM #waterLevel_11 w_11, #waterLevel_10 w_10
                    WHERE w_11.tab_seed NOT IN (SELECT tab_seed FROM  #waterLevel_10)
                    UNION 
                    SELECT  CONVERT(CHAR(10), w_10.measure_date, 23) ,
							w_10.上游水位,
							w_10.下游水位,
                            NULL [12孔闸开度] ,
                            NULL [12孔闸开启孔数] ,
                            NULL [28孔闸开度] ,
                            NULL [28孔闸开启孔数] ,
                            w_10.电机数,
                            CONVERT(DECIMAL(5, 2), w_10.闸上水头) AS 闸上水头 ,
                            CONVERT(DECIMAL(5, 2), w_10.闸下水头) AS 闸下水头 ,
                            NULL [12孔闸流量类型] ,
                            NULL [12孔闸流量] ,
                            NULL [28孔闸流量] ,
                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18, 2),w_10.电站流量)) as 电站流量,
                            CONVERT(NVARCHAR(50), CONVERT(DECIMAL(18, 2),w_10.电站流量)) AS 总流量
                    FROM #waterLevel_11 w_11, #waterLevel_10 w_10 
                    WHERE w_10.tab_seed NOT IN (SELECT tab_seed FROM  #waterLevel_11)
                    ORDER BY 日期 DESC
                    
                    
					
                END
			
        END
        --ELSE
			--28孔闸 流量计算
          --  BEGIN
            --    RETURN NULL
          --  END
			
        --RETURN @ve
    END



		/*
select sqrt(2*9.8) 
select 43.56*sqrt(2*9.8)  *power(8.11000,1.5)

select power(8.11000,1.5)

SELECT * FROM xmis.gczl_llgsh_锐缘平面闸门垂向收缩系数


		
		*/

--SELECT  0.951 * SQRT(2 * 9.8 * ( 8.11 - 2.576 ))

--xmis.sp_getLiulangTotalValue '2010-09-17','2015-09-17',12
--xmis.sp_getLiulangTotalValue '2014-05-05','2014-05-05',12
--xmis.sp_getLiulangTotalValue '2013-09-28','2013-09-28',12
--xmis.sp_getLiulangTotalValue '2011-11-21','2011-11-21',12


    