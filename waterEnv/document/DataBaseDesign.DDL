--    此 SQL DDL 脚本由 Microsoft Visual Studio (发布日期:LOCAL BUILD) 生成。

--    所用驱动程序: Microsoft Visual Studio － Oracle Server 驱动程序。
--    文档: D:\Ted Li\水环境\code\waterEnv\document\DataBaseDesign.vsd。
--    创建时间: 2016年3月30日 16:26。
--    操作: 来自 Visio 生成向导。
--    已连接的数据源: 没有连接。
--    已连接的服务器: 没有连接。
--    已连接的数据库: 不适用。



-- 创建 env 数据库。
connect internal ;
startup nomount pfile=   ;
spool create_db_env ;

create database ENV; 

create rollback segment env_r0 tablespace system ; 

alter rollback segment  env_r0 online ; 

-- Create Oracle exception file.
create table exceptions ( row_id rowid, owner varchar2(30), table_name varchar2(30), constraint varchar2(30)) ; 


-- 创建新表 SZPJ_SYS_周期定义表。
-- SZPJ_SYS_周期定义表 : szpj_sys_周期定义表 的表
-- 	SN_ID : sn_id 标识 szpj_sys_周期定义表
-- 	IYEAR : iyear 属于 szpj_sys_周期定义表
-- 	周期类型_ID : 周期类型_id 属于 szpj_sys_周期定义表
-- 	BEG_DATE : beg_date 属于 szpj_sys_周期定义表
-- 	END_DATE : end_date 属于 szpj_sys_周期定义表  
create table SZPJ_SYS_周期定义表 (
	SN_ID CHAR(32) not null,
	IYEAR INT null,
	周期类型_ID CHAR(32) null,
	BEG_DATE DATE null,
	END_DATE DATE null, constraint SZPJ_SYS_周期定义表_PK primary key (SN_ID) ); 

-- 创建新表 SZPJ_SYS_周期类型。
-- SZPJ_SYS_周期类型 : szpj_sys_周期类型 的表
-- 	日 day
-- 	周 week
-- 	旬 tenDays
-- 	月 month
-- 	季度 quarterly
-- 	年 year
-- 	丰水期 fengShui
-- 	枯水期 kuShui
-- 	平水期 pingShui
-- 	周期类型_CODE : 周期类型_code 部分标识 szpj_sys_周期类型
-- 	周期名称 : 周期名称 属于 szpj_sys_周期类型
-- 	描述 : 描述 属于 szpj_sys_周期类型
-- 	SHOW_FLAG : show_flag 属于 szpj_sys_周期类型
-- 	排序 : 排序 属于 szpj_sys_周期类型  
create table SZPJ_SYS_周期类型 (
	周期类型_CODE VARCHAR2(20) not null,
	周期名称 VARCHAR2(50) null,
	描述 VARCHAR2(256) null,
	SHOW_FLAG INT null,
	排序 INT null, constraint SZPJ_SYS_周期类型_PK primary key (周期类型_CODE) ); 

-- 创建新表 SZPJ_SYS_取样位置表。
-- SZPJ_SYS_取样位置表 : szpj_sys_取样位置表 的表
-- 	0 --
-- 	1 左
-- 	2 中
-- 	3 右
-- 	取样位置_ID : 取样位置_id 标识 szpj_sys_取样位置表
-- 	取样位置 : 取样位置 属于 szpj_sys_取样位置表  
create table SZPJ_SYS_取样位置表 (
	取样位置_ID CHAR(1) not null,
	取样位置 VARCHAR2(50) null, constraint SZPJ_SYS_取样位置表_PK primary key (取样位置_ID) ); 

-- 创建新表 SZPJ_BIZ_水文观测数据。
-- SZPJ_BIZ_水文观测数据 : szpj_biz_水文观测数据 的表
-- 	SN_ID : sn_id 标识 szpj_biz_水文观测数据
-- 	观测点_ID : uuid 观测站、观测点、断面
-- 	取样位置 : 取样位置 左中右
-- 	观测时间 : YYYY-MM-DD hh:mm:ss
-- 	水位 : 计量单位为米，计至两位小数
-- 	流量 : NUMBERPS(12,3)
-- 	水温 : 摄氏度 计至两位小数
-- 	气温 : 摄氏度 计至两位小数
-- 	备注 : 备注 属于 szpj_biz_水文观测数据  
create table SZPJ_BIZ_水文观测数据 (
	SN_ID CHAR(32) not null,
	观测点_ID CHAR(10) not null,
	取样位置 CHAR(10) not null,
	观测时间 DATE not null,
	水位 NUMBER(6,2) null,
	流量 NUMBER(12,3) null,
	水温 NUMBER(5,2) null,
	气温 NUMBER(5,2) null,
	备注 VARCHAR2(256) null, constraint SZPJ_BIZ_水文观测数据_PK primary key (SN_ID) ); 

-- 创建新表 ZDST_SYS_菜单类别对应表。
-- ZDST_SYS_菜单类别对应表 : zdst_菜单类别对应表 的表
-- 	ID : id 标识 zdst_sys_菜单类别对应表
-- 	MENU_1_ID : menu_1_id 属于 zdst_sys_菜单类别对应表
-- 	MENU_1_NM : menu_1_nm 属于 zdst_sys_菜单类别对应表
-- 	MENU_2_ID : menu_2_id 属于 zdst_sys_菜单类别对应表
-- 	MENU_2_NM : menu_2_nm 属于 zdst_sys_菜单类别对应表
-- 	MENU_3_ID : menu_3_id 属于 zdst_sys_菜单类别对应表
-- 	MENU_3_NM : menu_3_nm 属于 zdst_sys_菜单类别对应表
-- 	ORDER_BY_SN : order_by_sn 属于 zdst_sys_菜单类别对应表
-- 	MAIN_TYPE_ID : main_type_id 属于 zdst_sys_菜单类别对应表
-- 	MAIN_TYPE_NM : main_type_nm 属于 zdst_sys_菜单类别对应表
-- 	LEVEL_1_ID : level_1_id 属于 zdst_sys_菜单类别对应表
-- 	LEVEL_1_NM : level_1_nm 属于 zdst_sys_菜单类别对应表
-- 	LEVEL_2_ID : level_2_id 属于 zdst_sys_菜单类别对应表
-- 	LEVEL_2_NM : level_2_nm 属于 zdst_sys_菜单类别对应表  
create table ZDST_SYS_菜单类别对应表 (
	ID INT not null,
	MENU_1_ID VARCHAR2(10) null,
	MENU_1_NM VARCHAR2(50) null,
	MENU_2_ID VARCHAR2(10) null,
	MENU_2_NM VARCHAR2(50) null,
	MENU_3_ID VARCHAR2(10) null,
	MENU_3_NM VARCHAR2(50) null,
	ORDER_BY_SN INT null,
	MAIN_TYPE_ID VARCHAR2(10) null,
	MAIN_TYPE_NM VARCHAR2(50) null,
	LEVEL_1_ID VARCHAR2(10) null,
	LEVEL_1_NM VARCHAR2(50) null,
	LEVEL_2_ID VARCHAR2(10) null,
	LEVEL_2_NM VARCHAR2(50) null, constraint ZDST_SYS_菜单类别对应表_PK primary key (ID) ); 

-- 创建新表 WR_ST_W_R。
-- WR_ST_W_R : 测站蓄水量信息表
-- 	本表用于存储湖泊、水库的蓄水量信息，该值由测站库（湖）容曲线推求得出
-- 	STCD : 测站代码
-- 	TM : 时间
-- 	W : 蓄水量：测站代码所代表的湖库的蓄水量  
create table WR_ST_W_R (
	STCD CHAR(8) not null,
	TM DATE null,
	W NUMBER(9,3) null, constraint WR_ST_W_R_PK primary key (STCD) ); 

-- 创建新表 REL_Z_V。
-- REL_Z_V : 测站库（湖）容曲线表
-- 	本表用于存储水库（湖）的水位和蓄水量相关关系的率定结果
-- 	STCD : 测站代码
-- 	CAL_TM : 率定时间：该库（湖）曲线的率定时间。率定时间可只准确到日期，填写当日8 时0 分，但
-- 	小时数如果有记录的，宜按记录填写
-- 	PT_NO : 点序号：库（湖）水位和蓄水量对应点在该条库（湖）容曲线中的顺序号
-- 	Z : 水位：测站库（湖）容曲线上指定点的水位
-- 	W : 蓄水量：与库（湖）水位相应的蓄水量，计量单位为百万立方米，计至三位小数
-- 	WAT_SURF_A : 水面面积：与库（湖）水位相应的水面面积，计量单位为平方千米，计至一位小数
-- 	NT : 备注  
create table REL_Z_V (
	STCD CHAR(8) not null,
	CAL_TM DATE not null,
	PT_NO CHAR(4) null,
	Z NUMBER(6,2) null,
	W NUMBER(9,3) null,
	WAT_SURF_A NUMBER(7,1) null,
	NT VARCHAR2(256) null, constraint REL_Z_V_PK primary key (STCD, CAL_TM) ); 

-- 创建新表 WR_ST_CQ_R。
-- WR_ST_CQ_R : 测站推算流量信息表
-- 	本表用于存储由测站水位流量关系曲线推求的流量信息
-- 	STCD : 测站代码
-- 	TM : 观测时间
-- 	Q : 流量  
create table WR_ST_CQ_R (
	STCD CHAR(8) not null,
	TM DATE null,
	Q NUMBER(9,1) null, constraint WR_ST_CQ_R_PK primary key (STCD) ); 

-- 创建新表 REL_Z_Q。
-- REL_Z_Q : 测站水位流量关系曲线表
-- 	本表用于存储河流测站测验断面水位和流量相关关系的率定结果。 
-- 	STCD : 测站代码
-- 	CURV_NM : 曲线名称 测站水位流量关系曲线的名称，用于标示曲线是某年启用的综合曲线还是采用某
-- 	年典型洪水生成的曲线
-- 	ENAB_TM : 启用时间 该测站水位流量关系曲线的启用日期
-- 	PT_NO : 点序号 水位和流量对应点在该条水位流量关系曲线中的顺序号，曲线序号从1开始，按发
-- 	生的时间顺序依次递增。
-- 	Z : 水位
-- 	Q : 流量
-- 	NT : 备注  
create table REL_Z_Q (
	STCD CHAR(8) not null,
	CURV_NM VARCHAR2(30) null,
	ENAB_TM DATE null,
	PT_NO VARCHAR2(4) null,
	Z NUMBER(6,2) null,
	Q NUMBER(9,1) null,
	NT VARCHAR2(256) null, constraint REL_Z_Q_PK primary key (STCD) ); 

-- 创建新表 WR_ST_MQ_R。
-- WR_ST_MQ_R : 测站实测流量信息表
-- 	STCD : 测站代码
-- 	TM : 观测时间 指水情发生的时刻，精确到秒
-- 	Q : 指测定时间通过测验断面的实测流量，计量单位为立方米每秒，计至一位小数  
create table WR_ST_MQ_R (
	STCD CHAR(8) not null,
	TM DATE not null,
	Q NUMBER(9,1) null, constraint WR_ST_MQ_R_PK primary key (STCD, TM) ); 

-- 创建新表 WR_ST_Z_R。
-- WR_ST_Z_R : 测站水位监测信息表
-- 	STCD : 测站代码
-- 	TM : 观测时间 指水情发生的时刻，精确到秒
-- 	Z : 水位 指测定时间的断面水位，计量单位为米，计至两位小数。  
create table WR_ST_Z_R (
	STCD CHAR(8) not null,
	TM DATE not null,
	Z NUMBER(6,2) null, constraint WR_ST_Z_R_PK primary key (STCD, TM) ); 

-- 创建新表 WR_DAY_W_R。
-- WR_DAY_W_R : 取用水监测点日水量信息表
-- 	MP_CD : 监测点代码
-- 	DT : 观测日期：指取水或排水量的统计时间，当天8:00至次日8:00为一个统计周期
-- 	DAY_W : 日水量：指统计周期内通过测验断面的水量，计量单位为立方米，计至一位小数  
create table WR_DAY_W_R (
	MP_CD CHAR(13) not null,
	DT DATE not null,
	DAY_W NUMBER(10,1) null, constraint WR_DAY_W_R_PK primary key (MP_CD, DT) ); 

-- 创建新表 WR_MP_Q_R。
-- WR_MP_Q_R : 取用水监测点流量监测信息表
-- 	MP_CD : 监测点代码 
-- 	TM : 观测时间 指水情发生的时刻，精确到秒
-- 	MP_Q : 取排水流量 指测定时间通过断面的取水或排水流量，计量单位为立方米每小时，计至三位
-- 	小数；以立方米每秒计量的应转化为该单位，1立方米每秒=3600立方米每小时
-- 	ACC_W : 累计水量 计量仪器指示的水量，即自计量仪器安装后水量累加值，计量单位为立方米，计
-- 	至一位小数。  
create table WR_MP_Q_R (
	MP_CD CHAR(13) not null,
	TM DATE not null,
	MP_Q NUMBER(12,3) null,
	ACC_W NUMBER(10,1) null, constraint WR_MP_Q_R_PK primary key (MP_CD, TM) ); 

-- 创建新表 WR_MP_Z_R。
-- WR_MP_Z_R : 取用水监测点水位监测信息表
-- 	MP_CD : 监测点代码
-- 	TM : 观测时间 指水情发生的时刻，精确到秒
-- 	MP_Z : 取用水水位 指测定时间监测点的水位，计量单位为米，计至两位小数  
create table WR_MP_Z_R (
	MP_CD CHAR(13) not null,
	TM DATE not null,
	MP_Z NUMBER(6,2) null, constraint WR_MP_Z_R_PK primary key (MP_CD, TM) ); 

-- 创建新表 SZPJ_SYS_河流水系水质定性评价。
-- SZPJ_SYS_河流水系水质定性评价 : szpj_sys_河流水系水质定性评价 的表
-- 	优	        蓝色	I~III类水质比例≥90%
-- 	良好	        绿色	75%≤I~III类水质比例<90%
-- 	轻度污染	黄色	I~III类水质比例<75%，且劣V类比例<20%
-- 	中度污染	橙色	I~III类水质比例<75%，且20%≤劣V类比例<40%
-- 	重度污染	红色	I~III类水质比例<60%，且劣V类比例≥40%
-- 	SN_ID : sn_id 标识 szpj_sys_河流水系水质定性评价
-- 	I_YEAR : i_year 属于 szpj_sys_河流水系水质定性评价
-- 	水质状况 : 水质状况 属于 szpj_sys_河流水系水质定性评价
-- 	表征颜色 : 表征颜色 属于 szpj_sys_河流水系水质定性评价
-- 	水质类别比例 : 水质类别比例 属于 szpj_sys_河流水系水质定性评价  
create table SZPJ_SYS_河流水系水质定性评价 (
	SN_ID CHAR(32) not null,
	I_YEAR INT null,
	水质状况 VARCHAR2(50) null,
	表征颜色 VARCHAR2(50) null,
	水质类别比例 VARCHAR2(256) null, constraint SZPJ_SYS_河流水系水质定性评价_PK primary key (SN_ID) ); 

-- 创建新表 SZPJ_SYS_断面水质定性评价。
-- SZPJ_SYS_断面水质定性评价 : szpj_sys_断面水质定性评价 的表
-- 	I类水质	        优	        蓝色	饮用水源地一级保护区、珍惜水生生物栖息地、鱼虾类产卵场、仔稚幼鱼的索饵场等
-- 	II类水质	优	        蓝色	饮用水源地一级保护区、珍惜水生生物栖息地、鱼虾类产卵场、仔稚幼鱼的索饵场等
-- 	III类水质	良好	        绿色	饮用水源地二级保护区、鱼虾类越冬场、洄游通道、水产养殖区、游泳区
-- 	IV类水质	轻度污染	黄色	一般工业用水和人体非直接接触的娱乐用水
-- 	V类水质	        中度污染	橙色	农业用水及一般景观用水
-- 	劣V类水质	重度污染	红色	除调节局部气候外，使用功能较差
-- 	SN_ID : sn_id 标识 szpj_sys_断面水质定性评价
-- 	I_YEAR : i_year 属于 szpj_sys_断面水质定性评价
-- 	水质类别_ID : 水质类别_id 属于 szpj_sys_断面水质定性评价
-- 	水质状况 : 水质状况 属于 szpj_sys_断面水质定性评价
-- 	表征颜色 : 表征颜色 属于 szpj_sys_断面水质定性评价
-- 	说明 : 说明 属于 szpj_sys_断面水质定性评价  
create table SZPJ_SYS_断面水质定性评价 (
	SN_ID CHAR(32) not null,
	I_YEAR INT null,
	水质类别_ID CHAR(32) null,
	水质状况 VARCHAR2(50) null,
	表征颜色 VARCHAR2(50) null,
	说明 VARCHAR2(256) null, constraint SZPJ_SYS_断面水质定性评价_PK primary key (SN_ID) ); 

-- 创建新表 SZPJ_BIZ_水质指标观测数据。
-- SZPJ_BIZ_水质指标观测数据 : szpj_biz_水质指标观测数据 的表
-- 	SN_ID : sn_id 标识 szpj_biz_水质指标观测数据
-- 	观测点_ID : uuid 观测站、观测点、断面
-- 	取样位置 : 取样位置 左中右
-- 	观测时间 : YYYY-MM-DD hh:mm:ss
-- 	观测ITEM_ID : 观测项目_id uuid
-- 	观测值 : 观测值 属于 szpj_biz_水质指标观测数据
-- 	说明 : 说明 属于 szpj_biz_水质指标观测数据  
create table SZPJ_BIZ_水质指标观测数据 (
	SN_ID CHAR(32) not null,
	观测点_ID CHAR(32) not null,
	取样位置 CHAR(1) not null,
	观测时间 DATE not null,
	观测ITEM_ID CHAR(32) not null,
	观测值 NUMBER(11,5) not null,
	说明 VARCHAR2(256) null, constraint SZPJ_BIZ_水质指标观测数据_PK primary key (SN_ID) ); 

-- 创建新表 SZPJ_SYS_指标达标值。
-- SZPJ_SYS_指标达标值 : szpj_sys_指标参数值 的表
-- 	SN_ID : uuid
-- 	I_YEAR : i_year 属于 szpj_sys_指标达标值
-- 	ITEM_ID : item_id 属于 szpj_sys_指标达标值
-- 	LEVEL_ID : level_id 属于 szpj_sys_指标达标值
-- 	"VALUE" : value 属于 szpj_sys_指标达标值
-- 	REMARK : remark 属于 szpj_sys_指标达标值  
create table SZPJ_SYS_指标达标值 (
	SN_ID CHAR(32) not null,
	I_YEAR INT null,
	ITEM_ID CHAR(32) null,
	LEVEL_ID CHAR(1) null,
	"VALUE" NUMBER(11,5) null,
	REMARK VARCHAR2(256) null, constraint SZPJ_SYS_指标达标值_PK primary key (SN_ID) ); 

-- 创建新表 SZPJ_SYS_指标项。
-- SZPJ_SYS_指标项 : szpj_sys_参数表 的表
-- 	1 水质评价      1  水环境质量标准基本项目             1~24   对应项目名称 单位毫克每升
-- 	1 水质评价      2  集中式生活饮用水地表水源地补充项   1~5    对应项目名称 单位毫克每升
-- 	2 营养状态评价  1  湖泊水库营养状态评价指标           1~5    对应指标名称 
-- 	ITEM_ID : uuid
-- 	DOMAIN_ID : domain_id 属于 szpj_sys_指标项
-- 	DOMAIN_NAME : domain_name 属于 szpj_sys_指标项
-- 	ITYPE_ID : itype_id 属于 szpj_sys_指标项
-- 	TYPE_NAME : type_name 属于 szpj_sys_指标项
-- 	SYS_ID : sys_id 属于 szpj_sys_指标项
-- 	SYS_NAME : sys_name 属于 szpj_sys_指标项
-- 	REMARK : remark 属于 szpj_sys_指标项  
create table SZPJ_SYS_指标项 (
	ITEM_ID CHAR(32) not null,
	DOMAIN_ID VARCHAR2(8) null,
	DOMAIN_NAME VARCHAR2(50) null,
	ITYPE_ID VARCHAR2(8) null,
	TYPE_NAME VARCHAR2(100) null,
	SYS_ID VARCHAR2(8) null,
	SYS_NAME VARCHAR2(100) null,
	REMARK VARCHAR2(256) null, constraint SZPJ_SYS_指标项_PK primary key (ITEM_ID) ); 

-- 创建新表 MGMB_TYPE_水功能区等级表。
-- MGMB_TYPE_水功能区等级表 : mgmb_type_水功能区等级表 的表
-- 	1 一级
-- 	2 二级
-- 	3 三级
-- 	水功能区等级_ID : 水功能区等级_id 标识 mgmb_type_水功能区等级表
-- 	水功能区等级_NAME : 水功能区等级_name 属于 mgmb_type_水功能区等级表
-- 	REMARK : remark 属于 mgmb_type_水功能区等级表  
create table MGMB_TYPE_水功能区等级表 (
	水功能区等级_ID INT not null,
	水功能区等级_NAME VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint MGMB_TYPE_水功能区等级表_PK primary key (水功能区等级_ID) ); 

-- 创建新表 WR_LHB_T。
-- WR_LHB_T : 拦河坝基本信息表
-- 	LHB_CD : 拦河坝代码
-- 	LHB_NM : 拦河坝名称
-- 	LENGTH : 坝长 单位米
-- 	HIGH : 坝高 单位米
-- 	STORAGE_V : 蓄水量 万立方米  
create table WR_LHB_T (
	LHB_CD VARCHAR2(20) not null,
	LHB_NM VARCHAR2(100) not null,
	LENGTH DECIMAL(9,2) null,
	HIGH DECIMAL(9,2) null,
	STORAGE_V DECIMAL(9,2) null, constraint WR_LHB_T_PK primary key (LHB_CD) ); 

-- 创建新表 MGMB_REL_水功能区与水体。
-- MGMB_REL_水功能区与水体 : mgmb_rel_水功能区与水体 的表
-- 	水功能区_ID : 水功能区_id 标识 mgmb_rel_水功能区与水体
-- 	水体_ID : 水体_id 属于 mgmb_rel_水功能区与水体  
create table MGMB_REL_水功能区与水体 (
	水功能区_ID CHAR(32) not null,
	水体_ID CHAR(32) null, constraint MGMB_REL_水功能区与水体_PK primary key (水功能区_ID) ); 

-- 创建新表 MGMB_REL_断面与水体水功能区。
-- MGMB_REL_断面与水体水功能区 : mgmb_rel_断面与水体水功能区 的表
-- 	河道断面_ID : 河道断面_id 标识 mgmb_rel_断面与水体水功能区
-- 	水体_ID : 水体_id 部分标识 mgmb_rel_断面与水体水功能区
-- 	水功能区_ID : 水功能区_id 属于 mgmb_rel_断面与水体水功能区  
create table MGMB_REL_断面与水体水功能区 (
	河道断面_ID CHAR(32) not null,
	水体_ID CHAR(32) null,
	水功能区_ID CHAR(32) null, constraint MGMB_REL_断面与水体水功能区_PK primary key (河道断面_ID) ); 

-- 创建新表 P_ECOCATE。
-- P_ECOCATE : P_EcoCate 的表
-- 	门类 : 门类 标识 P_EcoCate
-- 	大类 : 大类 属于 P_EcoCate
-- 	中类 : 中类 属于 P_EcoCate
-- 	小类 : 小类 属于 P_EcoCate
-- 	类别名称 : 类别名称 属于 P_EcoCate
-- 	说明 : 说明 属于 P_EcoCate  
create table P_ECOCATE (
	门类 VARCHAR2(10) not null,
	大类 VARCHAR2(10) null,
	中类 VARCHAR2(10) null,
	小类 VARCHAR2(10) null,
	类别名称 VARCHAR2(100) null,
	说明 VARCHAR2(256) null, constraint P_ECOCATE_PK primary key (门类) ); 

-- 创建新表 MGMB_REL_敏感目标重点水体。
-- MGMB_REL_敏感目标重点水体 : mgmb_rel_敏感目标重点水体 的表
-- 	敏感目标对象_ID : 敏感目标对象_id 部分标识 mgmb_rel_敏感目标重点水体
-- 	水体对象_ID : 水体对象_id 标识 mgmb_rel_敏感目标重点水体  
create table MGMB_REL_敏感目标重点水体 (
	敏感目标对象_ID CHAR(32) not null,
	水体对象_ID CHAR(32) not null, constraint MGMB_REL_敏感目标重点水体_PK primary key (水体对象_ID, 敏感目标对象_ID) ); 

-- 创建新表 ZDST_REL_干支流关系表。
-- ZDST_REL_干支流关系表 : zdst_rel_干支流关系表 的表
-- 	支流_ID : 支流_id 部分标识 zdst_rel_干支流关系表
-- 	干流_ID : 干流_id 标识 zdst_rel_干支流关系表  
create table ZDST_REL_干支流关系表 (
	支流_ID VARCHAR2(32) not null,
	干流_ID VARCHAR2(32) not null, constraint ZDST_REL_干支流关系表_PK primary key (支流_ID) ); 

-- 创建新表 ORG_TYPE_行政区类别表。
-- ORG_TYPE_行政区类别表 : “org_type_行政区类别表”的表
-- 	1 县级及县级以上行政区
-- 	2 乡镇级行政区
-- 	行政区级别_ID : 行政区级别_id 标识 org_type_行政区类别表
-- 	行政区级别名称 : 行政区级别名称 属于 org_type_行政区类别表  
create table ORG_TYPE_行政区类别表 (
	行政区级别_ID INT not null,
	行政区级别名称 VARCHAR2(100) null, constraint ORG_TYPE_行政区类别表_PK primary key (行政区级别_ID) ); 

-- 创建新表 P_WR_MENB。
-- P_WR_MENB : 机构隶属关系代码表
-- 	1 隶属于中央 
-- 	2 隶属于省（自治区、直辖市） 
-- 	3 隶属于地（区、市、州、盟） 
-- 	4 隶属于县（区、市、旗） 
-- 	9 其他 
-- 	MENB : MENB 标识 P_WR_MENB
-- 	MENB_NM : MENB_NM 属于 P_WR_MENB  
create table P_WR_MENB (
	MENB CHAR(1) not null,
	MENB_NM VARCHAR2(100) null, constraint P_WR_MENB_PK primary key (MENB) ); 

-- 创建新表 P_WR_ORG_SCAL。
-- P_WR_ORG_SCAL : 机构规格代码表
-- 	1 正部（省）级
-- 	2 副部（省）级 
-- 	3 正厅级 
-- 	4 副厅级 
-- 	5 正处级 
-- 	6 副处级
-- 	7 正科级 
-- 	8 副科级 
-- 	9 其他 
-- 	ORG_SCAL : ORG_SCAL 标识 P_WR_ORG_SCAL
-- 	ORG_SCAL_NM : ORG_SCAL_NM 属于 P_WR_ORG_SCAL  
create table P_WR_ORG_SCAL (
	ORG_SCAL CHAR(1) not null,
	ORG_SCAL_NM VARCHAR2(100) null, constraint P_WR_ORG_SCAL_PK primary key (ORG_SCAL) ); 

-- 创建新表 P_WR_ORG_TP。
-- P_WR_ORG_TP : 机构类型代码表
-- 	1 国家机关（行政主管部门） 
-- 	2 事业单位 
-- 	3 企业 
-- 	4 社会团体 
-- 	5 乡镇水利管理单位 
-- 	9 其他
-- 	ORG_TP : ORG_TP 标识 P_WR_ORG_TP
-- 	ORG_TP_NM : ORG_TP_NM 属于 P_WR_ORG_TP  
create table P_WR_ORG_TP (
	ORG_TP CHAR(1) not null,
	ORG_TP_NM VARCHAR2(100) null, constraint P_WR_ORG_TP_PK primary key (ORG_TP) ); 

-- 创建新表 MGMB_TYPE_敏感目标类别表。
-- MGMB_TYPE_敏感目标类别表 : “mgmb_敏感目标类别表”的表
-- 	1 取水口
-- 	2 取用水户
-- 	3 取用水测站测点
-- 	4 自来水厂
-- 	5 入河排污口
-- 	6 污水处理厂
-- 	7 水功能区
-- 	8 河道断面
-- 	9 水源地
-- 	TYPE_ID : type_id 标识 mgmb_type_敏感目标类别表
-- 	TYPE_NAME : type_name 属于 mgmb_type_敏感目标类别表
-- 	ICO_FLAG : ico_flag 属于 mgmb_type_敏感目标类别表  
create table MGMB_TYPE_敏感目标类别表 (
	TYPE_ID INT not null,
	TYPE_NAME VARCHAR2(100) null,
	ICO_FLAG VARCHAR2(50) null, constraint 敏感目标类别表_PK primary key (TYPE_ID) ); 

-- 创建新表 MGMB_敏感目标对象简表。
-- MGMB_敏感目标对象简表 : mgmb_敏感目标对象简表 的表
-- 	敏感目标对象_ID : 敏感目标对象_id uuid
-- 	TYPE_ID : type_id 属于 mgmb_敏感目标对象简表
-- 	MAP_ID : map_id 属于 mgmb_敏感目标对象简表
-- 	SUPER_OBJ_ID : super_obj_id 属于 mgmb_敏感目标对象简表  
create table MGMB_敏感目标对象简表 (
	敏感目标对象_ID CHAR(32) not null,
	TYPE_ID INT null,
	MAP_ID VARCHAR2(100) null,
	SUPER_OBJ_ID VARCHAR2(100) null, constraint MGMB_敏感目标对象简表_PK primary key (敏感目标对象_ID) ); 

-- 创建新表 MGMB_水功能区界碑简表。
-- MGMB_水功能区界碑简表 : mgmb_水功能区界碑简表 的表
-- 	水功能区界碑_ID : 水功能区界碑_id uuid
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_水功能区界碑简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_水功能区界碑简表
-- 	REMARK : remark 属于 mgmb_水功能区界碑简表  
create table MGMB_水功能区界碑简表 (
	水功能区界碑_ID CHAR(32) not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint MGMB_水功能区界碑简表_PK primary key (水功能区界碑_ID) ); 

-- 创建新表 MGMB_REL_水功能区与水文测站。
-- MGMB_REL_水功能区与水文测站 : mgmb_rel_水功能区与水文测站 的表
-- 	水功能区_ID : 水功能区_id 部分标识 mgmb_rel_水功能区与水文测站
-- 	水文测站_ID : 水文测站_id 部分标识 mgmb_rel_水功能区与水文测站  
create table MGMB_REL_水功能区与水文测站 (
	水功能区_ID CHAR(32) not null,
	水文测站_ID CHAR(32) not null, constraint 水功能区与水文测站_PK primary key (水功能区_ID, 水文测站_ID) ); 

-- 创建新表 WR_WFZ_STEL_B。
-- WR_WFZ_STEL_B : 水功能区界碑信息表
-- 	WFZ_STEL_CD : 水功能区界碑代码
-- 	WFZ_STEL_NM : 水功能区界碑名称
-- 	UP_WFZ_CD : 上游水功能区代码
-- 	DOWN_WFZ_CD : 下游水功能区代码 约束同UP_WFZ_CD
-- 	STEL_TM : 立碑时间
-- 	STEL_ORG : 立碑单位
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_WFZ_STEL_B (
	WFZ_STEL_CD CHAR(9) not null,
	WFZ_STEL_NM VARCHAR2(100) null,
	UP_WFZ_CD CHAR(14) null,
	DOWN_WFZ_CD CHAR(14) null,
	STEL_TM DATE null,
	STEL_ORG VARCHAR2(100) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_WFZ_STEL_B_PK primary key (WFZ_STEL_CD) ); 

-- 创建新表 WR_WFZ_B。
-- WR_WFZ_B : 水功能区基本信息表
-- 	WFZ_CD : 水功能区代码
-- 	WFZ_NM : 水功能区名称
-- 	WFZ_FUN_SEQ : 水功能区功能排序 指对水功能区的主辅功能按次序排列而形成的汉字字符串描述
-- 	WFZ_STAR : 水功能区起始断面 水功能区起始断面的位置
-- 	WFZ_END : 水功能区终止断面 水功能区终止断面的位置
-- 	WFZ_A : 水功能区面积 计量单位为平方公里，计至两位小数
-- 	WFZ_LEN : 水功能区长度 计量单位为千米，计至两位小数
-- 	WFZ_LWL : 水功能区最低水位 计量单位为米，计至两位小数
-- 	WFZ_WQT : 水功能区水质目标
-- 	MON_G : 监控级别
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_WFZ_B (
	WFZ_CD CHAR(14) not null,
	WFZ_NM VARCHAR2(100) not null,
	WFZ_FUN_SEQ VARCHAR2(80) null,
	WFZ_STAR VARCHAR2(100) null,
	WFZ_END VARCHAR2(100) null,
	WFZ_A NUMBER(8,2) null,
	WFZ_LEN NUMBER(8,2) null,
	WFZ_LWL NUMBER(6,2) null,
	WFZ_WQT CHAR(1) null,
	MON_G CHAR(1) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_WFZ_B_PK primary key (WFZ_CD) ); 

-- 创建新表 MGMB_REL_河道断面与水文测站关系表。
-- MGMB_REL_河道断面与水文测站关系表 : mgmb_rel_河道断面与水文测站关系表 的表
-- 	河道断面_ID : 河道断面_id 标识 mgmb_rel_河道断面与水文测站关系表
-- 	水文测站_ID : 水文测站_id 部分标识 mgmb_rel_河道断面与水文测站关系表  
create table MGMB_REL_河道断面与水文测站关系表 (
	河道断面_ID CHAR(32) not null,
	水文测站_ID CHAR(32) not null, constraint 河道断面与水文测站_PK primary key (河道断面_ID, 水文测站_ID) ); 

-- 创建新表 MGMB_河道断面信息简表。
-- MGMB_河道断面信息简表 : mgmb_河道断面信息简表 的表
-- 	河道断面_ID : 河道断面_id uuid
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_河道断面信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_河道断面信息简表
-- 	REMARK : remark 属于 mgmb_河道断面信息简表  
create table MGMB_河道断面信息简表 (
	河道断面_ID CHAR(32) not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint MGMB_河道断面信息简表_PK primary key (河道断面_ID) ); 

-- 创建新表 WR_WFZB_B。
-- WR_WFZB_B : 水功能区监测断面基本信息表
-- 	RB_CD : 河道断面代码
-- 	ST_DT : 设置日期
-- 	MON_CONT : 监测内容
-- 	US_WFC_CD : 上游水功能区代码
-- 	DOWN_WFC_CD : 下游水功能区代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_WFZB_B (
	RB_CD CHAR(11) not null,
	ST_DT DATE null,
	MON_CONT VARCHAR2(100) null,
	US_WFC_CD CHAR(14) null,
	DOWN_WFC_CD CHAR(14) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_WFZB_B_PK primary key (RB_CD) ); 

-- 创建新表 WR_CSB_B。
-- WR_CSB_B : 控制断面基本信息表
-- 	RB_CD : 河道断面代码
-- 	ST_DT : 设置日期
-- 	MON_CONT : 监测内容 描述该控制断面的监测内容，如水位、流量、水质及监测频次等
-- 	CONT_MIN_Z : 控制最低水位 全年控制最低水位，计量单位为米，计至两位小数
-- 	CONT_MIN_Q : 控制最小流量 全年控制最小流量，计量单位为立方米每秒，计至一位小数
-- 	CONT_QUNT : 控制水质要求 简要填写水质控制要求
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_CSB_B (
	RB_CD CHAR(11) not null,
	ST_DT DATE null,
	MON_CONT VARCHAR2(100) null,
	CONT_MIN_Z NUMBER(9,1) null,
	CONT_MIN_Q NUMBER(5,2) null,
	CONT_QUNT VARCHAR2(100) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_CSB_B_PK primary key (RB_CD) ); 

-- 创建新表 P_WR_AB_G。
-- P_WR_AB_G : 行政区界断面级别代码表
-- 	1 国际行政区界断面 
-- 	2 省际行政区界断面 
-- 	3 市际行政区界断面 
-- 	4 县际行政区界断面
-- 	AB_G : AB_G 标识 P_WR_AB_G
-- 	AB_G_NM : AB_G_NM 属于 P_WR_AB_G  
create table P_WR_AB_G (
	AB_G CHAR(1) not null,
	AB_G_NM VARCHAR2(100) null, constraint P_WR_AB_G_PK primary key (AB_G) ); 

-- 创建新表 WR_AB_B。
-- WR_AB_B : 行政区界断面基本信息表
-- 	RB_CD : RB_CD 标识 WR_AB_B
-- 	AB_G : 行政区界断面级别
-- 	US_AD_CD : 上游行政区划代码
-- 	DOWN_AD_CD : 下游行政区划代码  约束同 US_AD_CD
-- 	SUP_ORG_CD : 上级主管单位代码
-- 	FROM_ASS_DT : 开始考核日期
-- 	MAJ_AI : 主要考核指标
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_AB_B (
	RB_CD CHAR(11) not null,
	AB_G CHAR(1) not null,
	US_AD_CD CHAR(6) null,
	DOWN_AD_CD CHAR(6) null,
	SUP_ORG_CD CHAR(9) null,
	FROM_ASS_DT DATE null,
	MAJ_AI VARCHAR2(100) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_AB_B_PK primary key (RB_CD) ); 

-- 创建新表 WR_RB_B。
-- WR_RB_B : 河道断面基本信息表
-- 	RB_CD : 河道断面代码
-- 	RB_NM : 河道断面名称
-- 	LOC : 所在地
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_RB_B (
	RB_CD CHAR(11) not null,
	RB_NM VARCHAR2(100) null,
	LOC VARCHAR2(100) null,
	TS DATE not null,
	NT VARCHAR2(100) null, constraint WR_RB_B_PK primary key (RB_CD) ); 

-- 创建新表 MGMB_REL_污水厂与行政区关系表。
-- MGMB_REL_污水厂与行政区关系表 : mgmb_rel_污水厂与行政区关系表 的表
-- 	污水处理厂_ID : 污水处理厂_id 标识 mgmb_rel_污水厂与行政区关系表
-- 	行政区_ID : 行政区_id 部分标识 mgmb_rel_污水厂与行政区关系表  
create table MGMB_REL_污水厂与行政区关系表 (
	污水处理厂_ID CHAR(32) not null,
	行政区_ID CHAR(32) not null, constraint 污水厂与行政区_PK primary key (污水处理厂_ID, 行政区_ID) ); 

-- 创建新表 WR_AD_B。
-- WR_AD_B : 县级及县级以上行政区划基本信息表
-- 	AD_CD : 行政区划代码
-- 	AD_NM : 行政区划名称
-- 	AD_SHNM : 行政区划简称
-- 	AD_A : 行政区划面积 计量单位为平方公里，计至两位小数
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_AD_B (
	AD_CD CHAR(6) not null,
	AD_NM VARCHAR2(100) not null,
	AD_SHNM VARCHAR2(20) null,
	AD_A NUMBER(9,2) null,
	TS DATE not null,
	NT CHAR(10) null, constraint WR_AD_B_PK primary key (AD_CD) ); 

-- 创建新表 MGMB_REL_入河排污口与水文测站关系表。
-- MGMB_REL_入河排污口与水文测站关系表 : mgmb_rel_入河排污口与水文测站关系表 的表
-- 	入河排污口_ID : 入河排污口_id 标识 mgmb_rel_入河排污口与水文测站关系表
-- 	水文测站_ID : 水文测站_id 部分标识 mgmb_rel_入河排污口与水文测站关系表  
create table MGMB_REL_入河排污口与水文测站关系表 (
	入河排污口_ID CHAR(32) not null,
	水文测站_ID CHAR(32) not null, constraint 入河排污口与水文测站_PK primary key (入河排污口_ID, 水文测站_ID) ); 

-- 创建新表 MGMB_REL_入河排污口与取用水测站关系表。
-- MGMB_REL_入河排污口与取用水测站关系表 : mgmb_rel_入河排污口与取用水测站关系表 的表
-- 	入河排污口_ID : 入河排污口_id 标识 mgmb_rel_入河排污口与取用水测站关系表
-- 	取用水测站点_ID : 取用水测站点_id 部分标识 mgmb_rel_入河排污口与取用水测站关系表  
create table MGMB_REL_入河排污口与取用水测站关系表 (
	入河排污口_ID CHAR(32) not null,
	取用水测站点_ID CHAR(32) not null, constraint 入河排污口与取用水测站_PK primary key (入河排污口_ID, 取用水测站点_ID) ); 

-- 创建新表 MGMB_REL_入河排污口与取用水户关系表。
-- MGMB_REL_入河排污口与取用水户关系表 : mgmb_rel_入河排污口与取用水户关系表 的表
-- 	入河排污口_ID : 入河排污口_id 标识 mgmb_rel_入河排污口与取用水户关系表
-- 	取用水户_ID : 取用水户_id 部分标识 mgmb_rel_入河排污口与取用水户关系表  
create table MGMB_REL_入河排污口与取用水户关系表 (
	入河排污口_ID CHAR(32) not null,
	取用水户_ID CHAR(32) not null, constraint 入河排污口与取用水户_PK primary key (入河排污口_ID, 取用水户_ID) ); 

-- 创建新表 MGMB_REL_入河排污口与水功能区关系冗余表。
-- MGMB_REL_入河排污口与水功能区关系冗余表 : mgmb_rel_入河排污口与水功能区关系冗余表 的表
-- 	入河排污口_ID : 入河排污口_id 标识 mgmb_rel_入河排污口与水功能区关系冗余表
-- 	水功能区_ID : 水功能区_id 部分标识 mgmb_rel_入河排污口与水功能区关系冗余表  
create table MGMB_REL_入河排污口与水功能区关系冗余表 (
	入河排污口_ID CHAR(32) not null,
	水功能区_ID CHAR(32) not null, constraint 入河排污口与水功能区冗余_PK primary key (入河排污口_ID, 水功能区_ID) ); 

-- 创建新表 MGMB_REL_入河排污口与水体主体关系。
-- MGMB_REL_入河排污口与水体主体关系 : mgmb_rel_入河排污口与水体主体关系 的表
-- 	入河排污口_ID : 入河排污口_id 标识 mgmb_rel_入河排污口与水体主体关系
-- 	水体_ID : 水体_id 部分标识 mgmb_rel_入河排污口与水体主体关系  
create table MGMB_REL_入河排污口与水体主体关系 (
	入河排污口_ID CHAR(32) not null,
	水体_ID CHAR(32) not null, constraint 入河排污口与水体主体_PK primary key (入河排污口_ID, 水体_ID) ); 

-- 创建新表 MGMB_入河排污口信息简表。
-- MGMB_入河排污口信息简表 : mgmb_入河排污口信息简表 的表
-- 	入河排污口_ID : 入河排污口_id uuid
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_入河排污口信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_入河排污口信息简表
-- 	REMARK : remark 属于 mgmb_入河排污口信息简表  
create table MGMB_入河排污口信息简表 (
	入河排污口_ID CHAR(32) not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint MGMB_入河排污口信息简表_PK primary key (入河排污口_ID) ); 

-- 创建新表 MGMB_污水处理厂信息简表。
-- MGMB_污水处理厂信息简表 : mgmb_污水处理厂信息简表 的表
-- 	污水处理厂_ID : 污水处理厂_id  uuid
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_污水处理厂信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_污水处理厂信息简表
-- 	REMARK : remark 属于 mgmb_污水处理厂信息简表  
create table MGMB_污水处理厂信息简表 (
	污水处理厂_ID CHAR(32) not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint MGMB_污水处理厂信息简表_PK primary key (污水处理厂_ID) ); 

-- 创建新表 P_WR_IN_RV_TP。
-- P_WR_IN_RV_TP : 入河方式代码表
-- 	1 明渠 
-- 	2 暗管 
-- 	3 泵站 
-- 	4 涵闸
-- 	5 潜没
-- 	9 其他 
-- 	IN_RV_TP : IN_RV_TP 标识 P_WR_IN_RV_TP
-- 	IN_RV_TP_NM : IN_RV_TP_NM 属于 P_WR_IN_RV_TP  
create table P_WR_IN_RV_TP (
	IN_RV_TP CHAR(1) not null,
	IN_RV_TP_NM VARCHAR2(100) null, constraint P_WR_IN_RV_TP_PK primary key (IN_RV_TP) ); 

-- 创建新表 P_WR_EMIS_TP。
-- P_WR_EMIS_TP : 排放方式代码表
-- 	1 连续 
-- 	2 间歇
-- 	EMIS_TP : EMIS_TP 标识 P_WR_EMIS_TP
-- 	EMIS_TP_NM : EMIS_TP_NM 属于 P_WR_EMIS_TP  
create table P_WR_EMIS_TP (
	EMIS_TP CHAR(1) not null,
	EMIS_TP_NM VARCHAR2(100) null, constraint P_WR_EMIS_TP_PK primary key (EMIS_TP) ); 

-- 创建新表 P_WR_PDO_TP。
-- P_WR_PDO_TP : 排污口性质代码表
-- 	1 企业 
-- 	2 市政 
-- 	9 其他 
-- 	PDO_TP : PDO_TP 标识 P_WR_PDO_TP
-- 	PDO_TP_NM : PDO_TP_NM 属于 P_WR_PDO_TP  
create table P_WR_PDO_TP (
	PDO_TP CHAR(1) not null,
	PDO_TP_NM VARCHAR2(100) null, constraint P_WR_PDO_TP_PK primary key (PDO_TP) ); 

-- 创建新表 WR_PDO_B。
-- WR_PDO_B : 入河排污口基本信息表
-- 	WFZ_CD : WFZ_CD 部分标识 WR_PDO_B
-- 	PDO_CD : 入河排污口代码
-- 	PDO_NM : 入河排污口名称
-- 	WDPC_CD : 排污许可证代码
-- 	ADDR : 地址
-- 	PDO_TP : 入河排污口性质
-- 	COMP_DT : 建成日期
-- 	EMIS_TP : 排放方式
-- 	IN_RV_TP : 入河方式
-- 	DWB_NM : 排入水体名称 填直接排入的河流、湖泊、水库名称
-- 	DES_POLL_CAP : 设计日排污能力 计量单位为吨，计至三位小数
-- 	PDO_SIZE : 排污口管径 计量单位为毫米
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_PDO_B (
	WFZ_CD CHAR(14) null,
	PDO_CD CHAR(12) not null,
	PDO_NM VARCHAR2(100) not null,
	WDPC_CD CHAR(18) null,
	ADDR VARCHAR2(100) null,
	PDO_TP CHAR(1) null,
	COMP_DT DATE null,
	EMIS_TP CHAR(1) null,
	IN_RV_TP CHAR(1) null,
	DWB_NM VARCHAR2(100) null,
	DES_POLL_CAP NUMBER(10,3) null,
	PDO_SIZE NUMBER(6,0) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT CHAR(10) null, constraint WR_PDO_B_PK primary key (PDO_CD) ); 

-- 创建新表 P_WR_CONS_SCAL。
-- P_WR_CONS_SCAL : 污水处理厂建设规模代码表
-- 	1 大型  设计处理能力大于10万立方米每天
-- 	2 中型  设计处理能力在1～10万立方米每天
-- 	3 小型  设计处理能力小于1万立方米每天
-- 	CONS_SCAL : CONS_SCAL 标识 P_WR_CONS_SCAL
-- 	CONS_SCAL_NM : CONS_SCAL_NM 属于 P_WR_CONS_SCAL
-- 	REMARK : remark 属于 P_WR_CONS_SCAL  
create table P_WR_CONS_SCAL (
	CONS_SCAL CHAR(10) not null,
	CONS_SCAL_NM CHAR(10) null,
	REMARK CHAR(10) null, constraint P_WR_CONS_SCAL_PK primary key (CONS_SCAL) ); 

-- 创建新表 WR_STP_B。
-- WR_STP_B : 污水处理厂基本信息表
-- 	污水处理工艺：该字段采用文字描述。污水处理流程的选择一般根据城市的自然、地理、经济及人文等各方面的情况，经过技术经济综合比较后确定。常用的工艺处理流程包括沉淀法、普通活性污泥法、SBR法、A/O法、AB法、氧化沟法、生物膜法、稳定塘等。
-- 	污泥处理工艺：水厂污泥处理工艺流程应根据水厂所处社会环境、自然条件及净水工艺确定，一般由调节、浓缩、脱水及泥饼处置四道工序或其中部分工序组成。 
-- 	STP_CD : 污水处理厂代码
-- 	STP_NM : 污水处理厂名称
-- 	TREA_UNIT_CONS : 处理单耗 计算公式：处理单耗=处理耗电量（瓦时）/处理废污水总量（立方米），计量单位为瓦时每立方米，计至两位小数
-- 	ST_CRAF : 污水处理工艺
-- 	SLUD_TREA_CRAF : 污泥处理工艺
-- 	LAND_A : 占地面积 计量单位为平方米，计至两位小数
-- 	SERV_OBJ : 服务对象
-- 	SERV_A : 服务面积 计量单位为平方公里，计至两位小数
-- 	SERV_PP : 服务人口 计量单位为万人，计至两位小数
-- 	CONS_SCAL : 建设规模
-- 	COMP_DT : 建成日期
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_STP_B (
	STP_CD CHAR(9) not null,
	STP_NM VARCHAR2(100) not null,
	TREA_UNIT_CONS NUMBER(4,2) null,
	ST_CRAF VARCHAR2(100) null,
	SLUD_TREA_CRAF VARCHAR2(100) null,
	LAND_A NUMBER(8,2) null,
	SERV_OBJ VARCHAR2(100) null,
	SERV_A NUMBER(5,2) null,
	SERV_PP NUMBER(6,2) null,
	CONS_SCAL CHAR(1) null,
	COMP_DT DATE null,
	TS DATE not null,
	NT VARCHAR2(100) null, constraint WR_STP_B_PK primary key (STP_CD) ); 

-- 创建新表 MGMB_REL_取水测站测点关系表。
-- MGMB_REL_取水测站测点关系表 : mgmb_rel_取水测站测点关系表 的表
-- 	取用水测站_ID : 取用水测站_id 标识 mgmb_rel_取水测站测点关系表
-- 	取用水测点_ID : 取用水测点_id 部分标识 mgmb_rel_取水测站测点关系表  
create table MGMB_REL_取水测站测点关系表 (
	取用水测站_ID CHAR(32) not null,
	取用水测点_ID CHAR(32) not null, constraint 取水测站测点关系表_PK primary key (取用水测站_ID, 取用水测点_ID) ); 

-- 创建新表 MGMB_REL_取水口与取用水测站测点关系表。
-- MGMB_REL_取水口与取用水测站测点关系表 : mgmb_rel_取水口与取用水测站测点关系表 的表
-- 	取水口_ID : 取水口_id 部分标识 mgmb_rel_取水口与取用水测站测点关系表
-- 	取用水测站点_ID : 取用水测站点_id 部分标识 mgmb_rel_取水口与取用水测站测点关系表  
create table MGMB_REL_取水口与取用水测站测点关系表 (
	取水口_ID CHAR(32) not null,
	取用水测站点_ID CHAR(32) not null, constraint 取水口与取用水测站点_PK primary key (取水口_ID, 取用水测站点_ID) ); 

-- 创建新表 MGMB_REL_取水口与取用水户关系表。
-- MGMB_REL_取水口与取用水户关系表 : mgmb_rel_取水口与取用水户关系表 的表
-- 	取水口_ID : 取水口_id 部分标识 mgmb_rel_取水口与取用水户关系表
-- 	取用水户_ID : 取用水户_id 部分标识 mgmb_rel_取水口与取用水户关系表  
create table MGMB_REL_取水口与取用水户关系表 (
	取水口_ID CHAR(32) not null,
	取用水户_ID CHAR(32) not null, constraint 取水口与取用水户_PK primary key (取水口_ID, 取用水户_ID) ); 

-- 创建新表 MGMB_TYPE_取用水测站测点类型表。
-- MGMB_TYPE_取用水测站测点类型表 : “mgmb_取用水测站测点类型表”的表
-- 	1 取用水测站
-- 	2 取用水测点
-- 	TYPE_ID : type_id 标识 mgmb_type_取用水测站测点类型表
-- 	TYPE_NAME : type_name 属于 mgmb_type_取用水测站测点类型表
-- 	ICO_FLAG : ico_flag 属于 mgmb_type_取用水测站测点类型表  
create table MGMB_TYPE_取用水测站测点类型表 (
	TYPE_ID INT not null,
	TYPE_NAME VARCHAR2(100) null,
	ICO_FLAG VARCHAR2(50) null, constraint 取用水测站测点类型表_PK primary key (TYPE_ID) ); 

-- 创建新表 MGMB_取用水测站测点简表。
-- MGMB_取用水测站测点简表 : mgmb_取用水测站测点简表 的表
-- 	取用水测站点_ID : 取用水测站点_id uuid
-- 	TYPE_ID : type_id 属于 mgmb_取用水测站测点简表
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_取用水测站测点简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_取用水测站测点简表
-- 	REMARK : remark 属于 mgmb_取用水测站测点简表  
create table MGMB_取用水测站测点简表 (
	取用水测站点_ID CHAR(32) not null,
	TYPE_ID INT not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint MGMB_取用水测站测点简表_PK primary key (取用水测站点_ID) ); 

-- 创建新表 P_WR_MP_ITEM。
-- P_WR_MP_ITEM : 取用水监测点监测项目代码表
-- 	1 水位 
-- 	2 流量 
-- 	MP_ITEM : MP_ITEM 标识 P_WR_MP_ITEM
-- 	MP_ITEM_NM : MP_ITEM_NM 属于 P_WR_MP_ITEM  
create table P_WR_MP_ITEM (
	MP_ITEM CHAR(1) not null,
	MP_ITEM_NM VARCHAR2(100) null, constraint P_WR_MP_ITEM_PK primary key (MP_ITEM) ); 

-- 创建新表 P_WR_WELL_TP。
-- P_WR_WELL_TP : 测井类型代码表
-- 	1 专用井 
-- 	2 结合井
-- 	WELL_TP : WELL_TP 标识 P_WR_WELL_TP
-- 	WELL_TP_NM : WELL_TP_NM 属于 P_WR_WELL_TP  
create table P_WR_WELL_TP (
	WELL_TP CHAR(1) not null,
	WELL_TP_NM VARCHAR2(100) null, constraint P_WR_WELL_TP_PK primary key (WELL_TP) ); 

-- 创建新表 P_WR_WIUST_TP。
-- P_WR_WIUST_TP : 取用水测站类型代码表
-- 	1 地表水取水站 
-- 	2 地下水取水站 
-- 	3 退（排）水站
-- 	WIUST_TP : WIUST_TP 标识 P_WR_WIUST_TP
-- 	WIUST_TP_NM : WIUST_TP_NM 属于 P_WR_WIUST_TP  
create table P_WR_WIUST_TP (
	WIUST_TP CHAR(1) not null,
	WIUST_TP_NM VARCHAR2(100) null, constraint P_WR_WIUST_TP_PK primary key (WIUST_TP) ); 

-- 创建新表 WR_WIUST_B。
-- WR_WIUST_B : 取用水测站基本信息表
-- 	WIUST_CD : 取用水测站代码
-- 	WIUST_NM : 取用水测站名称
-- 	WIUST_TP : 取用水测站类别
-- 	LOC : 所在地
-- 	DAT_TP : 基面类型
-- 	DAT_ELEV : 基面高程
-- 	MOD_BAS_VAL : 修正基值
-- 	MOD_PARA : 修正参数
-- 	WELL_TP : 测井类型
-- 	WELL_LAND_ELEV : 测井地面高程 单位为米，计至三位小数
-- 	EST_ST_YM : 设站年月
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_WIUST_B (
	WIUST_CD CHAR(10) not null,
	WIUST_NM VARCHAR2(100) not null,
	WIUST_TP CHAR(1) not null,
	LOC VARCHAR2(200) null,
	DAT_TP CHAR(2) null,
	DAT_ELEV NUMBER(7,3) null,
	MOD_BAS_VAL NUMBER(7,3) null,
	MOD_PARA NUMBER(4,2) null,
	WELL_TP CHAR(1) null,
	WELL_LAND_ELEV NUMBER(7,3) null,
	EST_ST_YM CHAR(6) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_WIUST_B_PK primary key (WIUST_CD) ); 

-- 创建新表 WR_MP_B。
-- WR_MP_B : 取用水监测点基本信息表
-- 	MP_CD : 监测点代码
-- 	MP_NM : 监测点名称
-- 	MP_ADDR : 监测点地址
-- 	MP_ITEM : 监测项目
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_MP_B (
	MP_CD CHAR(13) not null,
	MP_NM VARCHAR2(100) not null,
	MP_ADDR VARCHAR2(100) null,
	MP_ITEM CHAR(1) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_MP_B_PK primary key (MP_CD) ); 

-- 创建新表 MGMB_TYPE_取水口类别表。
-- MGMB_TYPE_取水口类别表 : “mgmb_取水口类别表”的表
-- 	1 地表取水口
-- 	2 地下取水井
-- 	TYPE_ID : type_id 标识 mgmb_type_取水口类别表
-- 	TYPE_NAME : type_name 属于 mgmb_type_取水口类别表
-- 	ICO_FLAG : ico_flag 属于 mgmb_type_取水口类别表  
create table MGMB_TYPE_取水口类别表 (
	TYPE_ID INT not null,
	TYPE_NAME VARCHAR2(100) null,
	ICO_FLAG VARCHAR2(50) null, constraint 取水口类别表_PK primary key (TYPE_ID) ); 

-- 创建新表 MGMB_自来水厂信息简表。
-- MGMB_自来水厂信息简表 : mgmb_自来水厂信息简表 的表
-- 	自来水厂_ID : 自来水厂_id uuid
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_自来水厂信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_自来水厂信息简表
-- 	REMARK : remark 属于 mgmb_自来水厂信息简表  
create table MGMB_自来水厂信息简表 (
	自来水厂_ID CHAR(32) not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint MGMB_自来水厂信息简表_PK primary key (自来水厂_ID) ); 

-- 创建新表 MGMB_取用水户信息简表。
-- MGMB_取用水户信息简表 : mgmb_取用水户信息简表 的表
-- 	取用水户_ID : 取用水户_id uuid
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_取用水户信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_取用水户信息简表
-- 	REMARK : remark 属于 mgmb_取用水户信息简表  
create table MGMB_取用水户信息简表 (
	取用水户_ID CHAR(32) not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint MGMB_取用水户信息简表_PK primary key (取用水户_ID) ); 

-- 创建新表 MGMB_水功能区信息简表。
-- MGMB_水功能区信息简表 : mgmb_水功能区信息简表 的表
-- 	水功能区_ID : 水功能区_id uuid
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_水功能区信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_水功能区信息简表
-- 	控制断面_CD : 控制断面_CD 属于 mgmb_水功能区信息简表
-- 	水功能区等级_ID : 水功能区等级_id 属于 mgmb_水功能区信息简表
-- 	REMARK : remark 属于 mgmb_水功能区信息简表  
create table MGMB_水功能区信息简表 (
	水功能区_ID CHAR(32) not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	控制断面_CD VARCHAR2(100) null,
	水功能区等级_ID INT null,
	REMARK VARCHAR2(100) null, constraint MGMB_水功能区信息简表_PK primary key (水功能区_ID) ); 

-- 创建新表 MGMB_取水口信息简表。
-- MGMB_取水口信息简表 : mgmb_取水口信息简表 的表
-- 	取水口_ID : 取水口_id uuid
-- 	TYPE_ID : type_id 属于 mgmb_取水口信息简表
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_取水口信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_取水口信息简表
-- 	REMARK : remark 属于 mgmb_取水口信息简表  
create table MGMB_取水口信息简表 (
	取水口_ID CHAR(32) not null,
	TYPE_ID INT not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint MGMB_取水口信息简表_PK primary key (取水口_ID) ); 

-- 创建新表 WR_WFCT_B。
-- WR_WFCT_B : 自来水厂基本信息表
-- 	日生产能力：水厂的日生产能力，计算公式是：日生产能力＝各台机泵铭牌能力或测定的机泵能力×(1－各台机泵同时开动后的损失因素)×24 小时。计量单位为万 m3/d，计至两位小数。 
-- 	WFCT_CD : 自来水厂代码 填写涉水组织机构代码
-- 	FRESH_V : 清水库容量 计量单位为立方米，计至两位小数
-- 	DAY_CAP : 日生产能力
-- 	PROD_LTP : 出厂管线
-- 	PROD_LTV : 出厂阀门
-- 	IN_LTP : 进厂原水管线
-- 	IN_LTV : 进厂原水阀门
-- 	PUMP1_NUM : 一泵房出水泵数
-- 	PUMP1_H : 一泵房出水泵扬程 计量单位为米，计至两位小数
-- 	PUMP1_Q : 一泵房出水泵流量 计量单位为立方米每秒，计至一位小数。
-- 	PUMP2_NUM : 二泵房出水泵数
-- 	PUMP2_H : 二泵房出水泵扬程 计量单位为米，计至两位小数
-- 	PUMP2_Q : 二泵房出水泵流量 计量单位为立方米每秒，计至一位小数。
-- 	WS_OBJ : 供水对象
-- 	WS_PP : 供水人口 计量单位为万人，计至两位小数
-- 	WW_COND : 取水计量情况
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_WFCT_B (
	WFCT_CD CHAR(9) not null,
	FRESH_V NUMBER(10,2) null,
	DAY_CAP NUMBER(10,2) null,
	PROD_LTP VARCHAR2(100) null,
	PROD_LTV VARCHAR2(100) null,
	IN_LTP VARCHAR2(100) null,
	IN_LTV VARCHAR2(100) null,
	PUMP1_NUM NUMBER(3,0) null,
	PUMP1_H NUMBER(5,2) null,
	PUMP1_Q NUMBER(9,1) null,
	PUMP2_NUM NUMBER(3,0) null,
	PUMP2_H NUMBER(5,2) null,
	PUMP2_Q NUMBER(9,1) null,
	WS_OBJ CHAR(1) null,
	WS_PP NUMBER(6,2) null,
	WW_COND VARCHAR2(100) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_WFCT_B_PK primary key (WFCT_CD) ); 

-- 创建新表 P_WR_MON_G。
-- P_WR_MON_G : 监控级别代码表
-- 	1 国控级 
-- 	2 省控级 
-- 	3 地市级 
-- 	4 区县级 
-- 	9 其他
-- 	MON_G : MON_G 标识 P_WR_MON_G
-- 	MON_G_NM : MON_G_NM 属于 P_WR_MON_G  
create table P_WR_MON_G (
	MON_G CHAR(1) not null,
	MON_G_NM VARCHAR2(100) null, constraint P_WR_MON_G_PK primary key (MON_G) ); 

-- 创建新表 P_WR_ECO_TP。
-- P_WR_ECO_TP : 经济类型代码表
-- 	1 国有全资
-- 	2 集体全资 
-- 	3 私营经济 
-- 	4 个体经济 
-- 	5 联营经济
-- 	6 股份制经济 
-- 	7 外商投资经济 
-- 	8 港澳台投资经济 
-- 	9 其他 
-- 	ECO_TP : ECO_TP 标识 P_WR_ECO_TP
-- 	ECO_TP_NM : ECO_TP_NM 属于 P_WR_ECO_TP  
create table P_WR_ECO_TP (
	ECO_TP CHAR(1) not null,
	ECO_TP_NM VARCHAR2(100) null, constraint P_WR_ECO_TP_PK primary key (ECO_TP) ); 

-- 创建新表 P_WR_TRAD_TP。
-- P_WR_TRAD_TP : 行业类别小类别代码表
-- 	4411 火力发电 
-- 	4412 水力发电 
-- 	4413 核力发电 
-- 	4419 其他能源发电 
-- 	4610 自来水的生产和供应 
-- 	4620 污水处理 
-- 	4690 其他水的处理、利用与分配 
-- 	TRAD_TP : TRAD_TP 标识 P_WR_TRAD_TP
-- 	TRAD_TP_NM : TRAD_TP_NM 属于 P_WR_TRAD_TP  
create table P_WR_TRAD_TP (
	TRAD_TP CHAR(4) not null,
	TRAD_TP_NM VARCHAR2(100) null, constraint P_WR_TRAD_TP_PK primary key (TRAD_TP) ); 

-- 创建新表 WR_WIU_B。
-- WR_WIU_B : 取用水户基本信息表
-- 	行业类别：按照国家标准《国民经济行业分类》（GB/T4754-2011）的规定填写大类代码，其中火力发电、水力发电等行业类别应填小类代码，即采用表9.4.3-2的所规定的代码（小类代码）填写。 
-- 	WIU_CD : 取用水户代码
-- 	WIU_NM : 取用水户名称
-- 	LR_NM : 法人代表
-- 	TRAD_TP : 行业类别 ？？？？？
-- 	ECO_TP : 经济类型
-- 	MON_G : 监控级别
-- 	WEB : 网址
-- 	E_MAIL : 邮箱
-- 	ADDR : 地址
-- 	ZIP : 邮编
-- 	TEL : 办公室电话
-- 	FAX : 传真号码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_WIU_B (
	WIU_CD CHAR(9) not null,
	WIU_NM VARCHAR2(100) not null,
	LR_NM VARCHAR2(20) null,
	TRAD_TP CHAR(4) null,
	ECO_TP CHAR(1) null,
	MON_G CHAR(1) null,
	WEB VARCHAR2(100) null,
	E_MAIL VARCHAR2(100) null,
	ADDR VARCHAR2(100) null,
	ZIP CHAR(6) null,
	TEL VARCHAR2(50) null,
	FAX VARCHAR2(50) null,
	TS DATE not null,
	NT CHAR(10) null, constraint WR_WIU_B_PK primary key (WIU_CD) ); 

-- 创建新表 P_WR_MIND_BED。
-- P_WR_MIND_BED : 开采层位代码表
-- 	1 浅层水 
-- 	2 承压水 
-- 	9 其他 
-- 	MIND_BED : MIND_BED 标识 P_WR_MIND_BED
-- 	MIND_BED_NM : MIND_BED_NM 属于 P_WR_MIND_BED  
create table P_WR_MIND_BED (
	MIND_BED CHAR(1) not null,
	MIND_BED_NM VARCHAR2(100) null, constraint P_WR_MIND_BED_PK primary key (MIND_BED) ); 

-- 创建新表 WR_WELL_B。
-- WR_WELL_B : 地下水取水井基本信息表
-- 	WELL_CD : 地下水取水井代码
-- 	WELL_NM : 地下水取水井名称
-- 	APER : 井径 计量单位为毫米，计至两位小数
-- 	WELL_DEP : 井深 指地下水取水井所在位置地表高程与取水井平均水位差值，计量单位为米，计至两位小数。
-- 	SHAF_SINK_DT : 凿井日期
-- 	MIND_BED : 开采层位
-- 	RUN_COND : 运行状况
-- 	DES_Q : 设计流量 允许通过的最大安全流量，计量单位为立方米每秒，计至一位小数
-- 	STAT_Z : 静水位 指取水井在没有抽水时的水位，计量单位为米，计至两位小数
-- 	DYN_Z : 动水位 抽取流量相对稳定，水面降到一定深度时保持相对稳定的水位，即稳定动水位，计量单位为米，计至两位小数
-- 	WQ_COND : 水质状况 以文字说明形式对地下水井的水质状况作简要描述
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_WELL_B (
	WELL_CD CHAR(12) not null,
	WELL_NM VARCHAR2(100) not null,
	APER NUMBER(6,2) null,
	WELL_DEP NUMBER(6,2) null,
	SHAF_SINK_DT DATE null,
	MIND_BED CHAR(1) null,
	RUN_COND VARCHAR2(100) null,
	DES_Q NUMBER(9,1) null,
	STAT_Z NUMBER(5,2) null,
	DYN_Z NUMBER(5,2) null,
	WQ_COND VARCHAR2(100) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT CHAR(10) null, constraint WR_WELL_B_PK primary key (WELL_CD) ); 

-- 创建新表 P_WR_INT_TP。
-- P_WR_INT_TP : 取水方式代码表
-- 	1 蓄水 
-- 	2 引水 
-- 	3 提水 
-- 	9 其他 
-- 	INT_TP : INT_TP 标识 P_WR_INT_TP
-- 	INT_TP_NM : INT_TP_NM 属于 P_WR_INT_TP  
create table P_WR_INT_TP (
	INT_TP CHAR(1) not null,
	INT_TP_NM VARCHAR2(100) null, constraint P_WR_INT_TP_PK primary key (INT_TP) ); 

-- 创建新表 WR_INT_B。
-- WR_INT_B : 地表水取水口基本信息表
-- 	INT_CD : 地表水取水口代码
-- 	INT_NM : 地表水取水口名称
-- 	INT_TP : 取水方式
-- 	FROM_INT_DT : 开始取水日期
-- 	MAX_PERM_Q : 许可最大流量 计量单位为立方米每秒，计至一位小数。
-- 	DES_Q : 设计流量 允许通过的最大安全流量，计量单位为立方米每秒，计至一位小数
-- 	PERM_WW : 许可总取水量 计量单位为万立方米，计至三位小数
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_INT_B (
	INT_CD CHAR(12) not null,
	INT_NM VARCHAR2(100) not null,
	INT_TP CHAR(1) null,
	FROM_INT_DT DATE null,
	MAX_PERM_Q NUMBER(9,1) null,
	DES_Q NUMBER(9,0) null,
	PERM_WW NUMBER(10,3) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_INT_B_PK primary key (INT_CD) ); 

-- 创建新表 MGMB_REL_水源地与供水行政区关系表。
-- MGMB_REL_水源地与供水行政区关系表 : mgmb_rel_水源地与供水行政区关系表 的表
-- 	水源地_ID : 水源地_id 标识 mgmb_rel_水源地与供水行政区关系表
-- 	行政区_ID : 行政区_id 部分标识 mgmb_rel_水源地与供水行政区关系表  
create table MGMB_REL_水源地与供水行政区关系表 (
	水源地_ID CHAR(32) not null,
	行政区_ID CHAR(32) not null, constraint 水源地与供水行政区_PK primary key (水源地_ID, 行政区_ID) ); 

-- 创建新表 ORG_行政区信息简表。
-- ORG_行政区信息简表 : org_行政区信息简表 的表
-- 	行政区_ID : 行政区_id 属于 uuid
-- 	行政区级别_ID : 行政区级别_id 属于 org_行政区信息简表
-- 	WR_SD_CODE : wr_sd_code 属于 org_行政区信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 org_行政区信息简表
-- 	REMARK : remark 属于 org_行政区信息简表  
create table ORG_行政区信息简表 (
	行政区_ID CHAR(32) not null,
	行政区级别_ID INT not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK CHAR(10) null, constraint ORG_行政区信息简表_PK primary key (行政区_ID) ); 

-- 创建新表 MGMB_REL_水源地与水文测站关系表。
-- MGMB_REL_水源地与水文测站关系表 : mgmb_rel_水源地与水文测站关系表 的表
-- 	水源地_ID : 水源地_id 标识 mgmb_rel_水源地与水文测站关系表
-- 	水文测站_ID : 水文测站_id 部分标识 mgmb_rel_水源地与水文测站关系表  
create table MGMB_REL_水源地与水文测站关系表 (
	水源地_ID CHAR(32) not null,
	水文测站_ID CHAR(32) not null, constraint 水源地与水文测站_PK primary key (水源地_ID, 水文测站_ID) ); 

-- 创建新表 MGMB_REL_水源地与水功能区关系表。
-- MGMB_REL_水源地与水功能区关系表 : mgmb_rel_水源地与水功能区关系表 的表
-- 	水源地_ID : 水源地_id 标识 mgmb_rel_水源地与水功能区关系表
-- 	水功能区_ID : 水功能区_id 部分标识 mgmb_rel_水源地与水功能区关系表  
create table MGMB_REL_水源地与水功能区关系表 (
	水源地_ID CHAR(32) not null,
	水功能区_ID CHAR(32) not null, constraint 水源地与水功能区_PK primary key (水源地_ID, 水功能区_ID) ); 

-- 创建新表 MGMB_TYPE_水源地类别表。
-- MGMB_TYPE_水源地类别表 : “mgmb_水源地类别表”的表
-- 	1 地表水
-- 	2 地下水
-- 	TYPE_ID : type_id 标识 mgmb_type_水源地类别表
-- 	TYPE_NAME : type_name 属于 mgmb_type_水源地类别表
-- 	ICO_FLAG : ico_flag 属于 mgmb_type_水源地类别表  
create table MGMB_TYPE_水源地类别表 (
	TYPE_ID INT not null,
	TYPE_NAME VARCHAR2(100) null,
	ICO_FLAG VARCHAR2(50) null, constraint 水源地类别表_PK primary key (TYPE_ID) ); 

-- 创建新表 MGMB_水源地信息简表。
-- MGMB_水源地信息简表 : mgmb_水源地信息简表 的表
-- 	水源地_ID : 水源地_id uuid
-- 	TYPE_ID : type_id 属于 mgmb_水源地信息简表
-- 	WR_SD_CODE : wr_sd_code 属于 mgmb_水源地信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 mgmb_水源地信息简表
-- 	REMARK : remark 属于 mgmb_水源地信息简表  
create table MGMB_水源地信息简表 (
	水源地_ID CHAR(32) not null,
	TYPE_ID INT not null,
	WR_SD_CODE VARCHAR2(50) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(256) null, constraint MGMB_水源地信息简表_PK primary key (水源地_ID) ); 

-- 创建新表 WR_GWS_B。
-- WR_GWS_B : 地下水水源地基本信息表
-- 	GWS_CD : 地下水水源地代码
-- 	GWS_NM : 地下水水源地名称
-- 	GWS_A : 地下水水源地面积
-- 	RANG_DESC : 范围描述
-- 	WQ_COND : 水质状况 以文字说明形式对水质状况作简要描述
-- 	CONS_COND : 建设状况
-- 	PUT_PROD_TM : 投产时间
-- 	RUN_COND : 运行状况
-- 	AVG_EXP_YD : 多年平均年可开采量 计量单位为百万立方米，计至三位小数
-- 	WS_OBJ : 供水对象
-- 	DES_INT : 设计年取水量 计量单位为百万立方米，计至三位小数
-- 	PERM_INT : 年许可取水量 计量单位为百万立方米，计至三位小数
-- 	WHS_MAN_CD : 水源地管理单位代码
-- 	WHS_APPR_CD : 水源地审批单位代码  约束同 WHS_MAN_CD
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_GWS_B (
	GWS_CD CHAR(12) not null,
	GWS_NM VARCHAR2(100) not null,
	GWS_A NUMBER(7,1) null,
	RANG_DESC VARCHAR2(100) null,
	WQ_COND VARCHAR2(100) null,
	CONS_COND CHAR(1) null,
	PUT_PROD_TM DATE null,
	RUN_COND VARCHAR2(256) null,
	AVG_EXP_YD NUMBER(10,3) null,
	WS_OBJ CHAR(1) null,
	DES_INT NUMBER(10,3) null,
	PERM_INT NUMBER(10,3) null,
	WHS_MAN_CD CHAR(9) null,
	WHS_APPR_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_GWS_B_PK primary key (GWS_CD) ); 

-- 创建新表 P_WR_WS_OBJ。
-- P_WR_WS_OBJ : 供水对象类别表
-- 	1 工业
-- 	2 生活 
-- 	3 公共
-- 	4 混合
-- 	WS_OBJ : WS_OBJ 标识 P_WR_WS_OBJ
-- 	WS_OBJ_NM : WS_OBJ_NM 属于 P_WR_WS_OBJ  
create table P_WR_WS_OBJ (
	WS_OBJ CHAR(1) not null,
	WS_OBJ_NM VARCHAR2(100) null, constraint P_WR_WS_OBJ_PK primary key (WS_OBJ) ); 

-- 创建新表 P_WR_WS_COND。
-- P_WR_WS_COND : 水源供水持续状况代码表
-- 	1 常年 
-- 	2 汛期 
-- 	3 非汛期 
-- 	WS_COND : WS_COND 标识 P_WR_WS_COND
-- 	WS_COND_NM : WS_COND_NM 属于 P_WR_WS_COND  
create table P_WR_WS_COND (
	WS_COND CHAR(1) not null,
	WS_COND_NM VARCHAR2(100) null, constraint P_WR_WS_COND_PK primary key (WS_COND) ); 

-- 创建新表 P_WR_WQ_GOAL。
-- P_WR_WQ_GOAL : 水质类别管理目标表
-- 	1   Ⅰ类，
-- 	12  Ⅰ～II类
-- 	13  Ⅰ～III类
-- 	14  Ⅰ～IV类
-- 	15  Ⅰ～V类
-- 	16  Ⅰ～VI类
-- 	2   Ⅱ类，
-- 	23  Ⅱ～Ⅲ类
-- 	24  Ⅱ～IV类
-- 	25  Ⅱ～V类
-- 	26  Ⅱ～VI类
-- 	3   Ⅲ类，
-- 	34  Ⅲ～IV类
-- 	35  Ⅲ～V类
-- 	36  Ⅲ～VI类
-- 	4   Ⅳ类
-- 	45  Ⅳ～V类
-- 	46  Ⅳ～VI类
-- 	5   Ⅴ类，
-- 	56  V～VI类
-- 	6 劣Ⅴ类水质目标。
-- 	WQ_GOAL : WQ_GOAL 标识 P_WR_WQ_GOAL
-- 	WQ_GOAL_NM : WQ_GOAL_NM 属于 P_WR_WQ_GOAL
-- 	COLOR_FLAG : color_flag 属于 P_WR_WQ_GOAL  
create table P_WR_WQ_GOAL (
	WQ_GOAL VARCHAR2(10) not null,
	WQ_GOAL_NM VARCHAR2(50) null,
	COLOR_FLAG VARCHAR2(50) null, constraint P_WR_WQ_GOAL_PK primary key (WQ_GOAL) ); 

-- 创建新表 P_WR_SWS_TP。
-- P_WR_SWS_TP : 地表水水源地类型代码表
-- 	1 河道 
-- 	2 水库 
-- 	3 湖泊 
-- 	9 其他
-- 	SWS_TP : SWS_TP 标识 P_WR_SWS_TP
-- 	SWS_TP_NM : SWS_TP_NM 属于 P_WR_SWS_TP  
create table P_WR_SWS_TP (
	SWS_TP CHAR(1) not null,
	SWS_TP_NM VARCHAR2(100) null, constraint P_WR_SWS_TP_PK primary key (SWS_TP) ); 

-- 创建新表 ZDST_REL_河流湖泊关系表。
-- ZDST_REL_河流湖泊关系表 : zdst_rel_河流湖泊关系表 的表
-- 	湖泊_ID : 湖泊_id 标识 zdst_rel_河流湖泊关系表
-- 	河流_ID : 河流_id 部分标识 zdst_rel_河流湖泊关系表
-- 	出入标记 : 出入标记 0 流入 1 流出  
create table ZDST_REL_河流湖泊关系表 (
	湖泊_ID VARCHAR2(32) not null,
	河流_ID VARCHAR2(32) not null,
	出入标记 CHAR(1) null, constraint ZDST_REL_河流湖泊关系表_PK primary key (湖泊_ID, 河流_ID) ); 

-- 创建新表 P_WR_SFW_PROP。
-- P_WR_SFW_PROP : 咸淡水属性代码表
-- 	1 淡水     含盐度 < 1 g/L
-- 	2 咸水     1 g/L≤含盐度<35 g/L
-- 	3 盐水     含盐度 ≥ 35 g/L 
-- 	SFW_PROP : SFW_PROP 标识 P_WR_SFW_PROP
-- 	SFW_PROP_NM : SFW_PROP_NM 属于 P_WR_SFW_PROP
-- 	REMARK : remark 属于 P_WR_SFW_PROP  
create table P_WR_SFW_PROP (
	SFW_PROP CHAR(1) not null,
	SFW_PROP_NM VARCHAR2(100) null,
	REMARK VARCHAR2(256) null, constraint P_WR_SFW_PROP_PK primary key (SFW_PROP) ); 

-- 创建新表 WR_LK_B。
-- WR_LK_B : 湖泊基本信息表
-- 	LK_CD : 湖泊代码
-- 	LK_NM : 湖泊名称
-- 	WAT_A : 水面面积 湖泊的常年水面面积，计量单位为平方公里，计至一位小数
-- 	SFW_PROP : 咸淡水属性
-- 	AVG_DEP : 平均水深 计量单位为米，计至两位小数
-- 	MAX_DEP : 最大水深 计量单位为米，计至两位小数
-- 	LK_V : 湖泊容积 计量单位为百万立方米，计至三位小数
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_LK_B (
	LK_CD CHAR(15) not null,
	LK_NM VARCHAR2(100) null,
	WAT_A DECIMAL(7,1) null,
	SFW_PROP CHAR(1) null,
	AVG_DEP NUMBER(5,2) null,
	MAX_DEP NUMBER(5,2) null,
	LK_V NUMBER(9,3) null,
	TS DATE null,
	NT VARCHAR2(256) null, constraint WR_LK_B_PK primary key (LK_CD) ); 

-- 创建新表 ZDST_TYPE_重点水体对象类型表。
-- ZDST_TYPE_重点水体对象类型表 : “zdst_type_重点水体对象类型表”的表
-- 	1 水体主体
-- 	2 水文测站
-- 	3 水利工程
-- 	TYPE_ID : type_id 标识 zdst_type_重点水体对象类型表
-- 	TYPE_NAME : type_name 属于 zdst_type_重点水体对象类型表
-- 	ICO_FLAG : ico_flag 属于 zdst_type_重点水体对象类型表  
create table ZDST_TYPE_重点水体对象类型表 (
	TYPE_ID INT not null,
	TYPE_NAME VARCHAR2(100) null,
	ICO_FLAG VARCHAR2(50) null, constraint ZDST_TYPE_重点水体对象类型表_PK primary key (TYPE_ID) ); 

-- 创建新表 ZDST_重点水体对象简表。
-- ZDST_重点水体对象简表 : zdst_重点水体对象简表 的表
-- 	水体对象_ID : 水体对象_id 标识 zdst_重点水体对象简表
-- 	TYPE_ID : type_id 属于 zdst_重点水体对象简表
-- 	MAP_ID : map_id 属于 zdst_重点水体对象简表
-- 	SUPER_OBJ_ID : super_obj_id 属于 zdst_重点水体对象简表  
create table ZDST_重点水体对象简表 (
	水体对象_ID CHAR(32) not null,
	TYPE_ID INT null,
	MAP_ID VARCHAR2(100) null,
	SUPER_OBJ_ID VARCHAR2(100) null, constraint ZDST_重点水体对象简表_PK primary key (水体对象_ID) ); 

-- 创建新表 ZDST_REL_水利工程项目与水体关系表。
-- ZDST_REL_水利工程项目与水体关系表 : zdst_rel_水利工程项目与水体关系表 的表
-- 	水利工程项目_ID : 水利工程项目_id 标识 zdst_rel_水利工程项目与水体关系表
-- 	水体_ID : 水体_id 部分标识 zdst_rel_水利工程项目与水体关系表  
create table ZDST_REL_水利工程项目与水体关系表 (
	水利工程项目_ID CHAR(32) not null,
	水体_ID CHAR(32) not null, constraint ZDST_REL_水利工程项目与水体关系表_PK primary key (水利工程项目_ID, 水体_ID) ); 

-- 创建新表 ZDST_TYPE_水利工程项目类别表。
-- ZDST_TYPE_水利工程项目类别表 : “zdst_水利工程项目类别表”的表
-- 	1 水电站
-- 	2 引调水工程
-- 	3 水闸
-- 	4 泵站
-- 	5 渠道工程
-- 	6 农村供水工程
-- 	7 灌区
-- 	8.拦河坝
-- 	TYPE_ID : type_id 标识 zdst_type_水利工程项目类别表
-- 	TYPE_NAME : type_name 属于 zdst_type_水利工程项目类别表
-- 	ICO_FLAG : ico_flag 属于 zdst_type_水利工程项目类别表  
create table ZDST_TYPE_水利工程项目类别表 (
	TYPE_ID INT not null,
	TYPE_NAME VARCHAR2(100) null,
	ICO_FLAG CHAR(10) null, constraint ZDST_TYPE_水利工程项目类别表_PK primary key (TYPE_ID) ); 

-- 创建新表 ZDST_水利工程项目简表。
-- ZDST_水利工程项目简表 : zdst_水利工程项目简表 的表
-- 	水利工程项目_ID : 水利工程项目_id uuid
-- 	TYPE_ID : type_id 属于 zdst_水利工程项目简表
-- 	WR_SD_CODE : wr_sd_code 属于 zdst_水利工程项目简表
-- 	经纬度坐标 : 经纬度坐标 属于 zdst_水利工程项目简表
-- 	REMARK : remark 属于 zdst_水利工程项目简表  
create table ZDST_水利工程项目简表 (
	水利工程项目_ID CHAR(32) not null,
	TYPE_ID INT not null,
	WR_SD_CODE VARCHAR2(32) not null,
	经纬度坐标 VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint ZDST_水利工程项目简表_PK primary key (水利工程项目_ID) ); 

-- 创建新表 P_WR_IRR_SCAL。
-- P_WR_IRR_SCAL : 灌区规模代码表
-- 	1 大型     灌溉面积 30 万亩以上 
-- 	2 中型     灌溉面积 1～30 万亩 
-- 	3 小型     灌溉面积1 万亩以下
-- 	IRR_SCAL : IRR_SCAL 标识 P_WR_IRR_SCAL
-- 	IRR_SCAL_NM : IRR_SCAL_NM 属于 P_WR_IRR_SCAL
-- 	REMARK : remark 属于 P_WR_IRR_SCAL  
create table P_WR_IRR_SCAL (
	IRR_SCAL CHAR(1) not null,
	IRR_SCAL_NM VARCHAR2(100) null,
	REMARK VARCHAR2(100) null, constraint P_WR_IRR_SCAL_PK primary key (IRR_SCAL) ); 

-- 创建新表 P_WR_IRR_TP。
-- P_WR_IRR_TP : 灌区类型代码表
-- 	1 水库灌区 
-- 	2 引水灌区 
-- 	3 提水灌区 
-- 	4 井灌区 
-- 	5 混合灌区 
-- 	IRR_TP : IRR_TP 标识 P_WR_IRR_TP
-- 	IRR_TP_NM : IRR_TP_NM 属于 P_WR_IRR_TP  
create table P_WR_IRR_TP (
	IRR_TP CHAR(1) not null,
	IRR_TP_NM VARCHAR2(100) null, constraint P_WR_IRR_TP_PK primary key (IRR_TP) ); 

-- 创建新表 WR_IRR_B。
-- WR_IRR_B : 灌区基本信息表
-- 	IRR_CD : 灌区代码
-- 	IRR_NM : 灌区名称
-- 	IRR_TP : 灌区类型
-- 	IRR_SCAL : 灌区规模
-- 	DES_IRR_A : 设计灌溉面积 计量单位为万亩，计至两位小数
-- 	IRR_A : 总灌溉面积 计量单位为万亩，计至两位小数
-- 	FIELD_IRR_A : 其中耕地灌溉面积 计量单位为万亩，计至两位小数
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_IRR_B (
	IRR_CD CHAR(12) not null,
	IRR_NM VARCHAR2(100) not null,
	IRR_TP CHAR(1) null,
	IRR_SCAL CHAR(1) null,
	DES_IRR_A NUMBER(10,2) null,
	IRR_A NUMBER(10,2) null,
	FIELD_IRR_A NUMBER(10,2) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_IRR_B_PK primary key (IRR_CD) ); 

-- 创建新表 P_WR_CHARG_FORM。
-- P_WR_CHARG_FORM : 收费形式代码表
-- 	CHARG_FORM : CHARG_FORM 标识 P_WR_CHARG_FORM
-- 	CHARG_FORM_NM : CHARG_FORM_NM 属于 P_WR_CHARG_FORM  
create table P_WR_CHARG_FORM (
	CHARG_FORM CHAR(1) not null,
	CHARG_FORM_NM VARCHAR2(100) null, constraint P_WR_CHARG_FORM_PK primary key (CHARG_FORM) ); 

-- 创建新表 P_WR_CWS_TP。
-- P_WR_CWS_TP : 农村供水工程类型表
-- 	1 集中式Ⅰ型      日供水规模≥10000m3
-- 	 
-- 	2 集中式Ⅱ型      10000m3>日供水规模≥5000m3
-- 	 
-- 	3 集中式Ⅲ型      5000m3>日供水规模≥1000m3
-- 	 
-- 	4 集中式Ⅳ型      1000m3>日供水规模≥200m3
-- 	 
-- 	5 集中式Ⅴ型      200m3>日供水规模 
-- 	6 分散式          分散式供水工程
-- 	CWS_TP : CWS_TP 标识 P_WR_CWS_TP
-- 	CWS_TP_NM : CWS_TP_NM 属于 P_WR_CWS_TP
-- 	REMARK : remark 属于 P_WR_CWS_TP  
create table P_WR_CWS_TP (
	CWS_TP CHAR(1) not null,
	CWS_TP_NM VARCHAR2(100) null,
	REMARK VARCHAR2(200) null, constraint P_WR_CWS_TP_PK primary key (CWS_TP) ); 

-- 创建新表 WR_CWS_B。
-- WR_CWS_B : 农村供水工程基本信息表
-- 	CWS_CD : 农村供水工程代码
-- 	CWS_NM : 农村供水工程名称
-- 	CWS_TP : 农村供水工程类型
-- 	DES_WS_SCAL : 设计供水日规模 计量单位为立方米，计至一位小数
-- 	DES_WS_PP : 设计供水人口
-- 	SAN_LIC_SN : 卫生许可证编号
-- 	BEN_VIL_NUM : 受益行政村数量
-- 	CHARG_FORM : 收费形式
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_CWS_B (
	CWS_CD CHAR(12) not null,
	CWS_NM VARCHAR2(100) not null,
	CWS_TP CHAR(1) not null,
	DES_WS_SCAL NUMBER(8,1) null,
	DES_WS_PP NUMBER(8,0) null,
	SAN_LIC_SN VARCHAR2(50) null,
	BEN_VIL_NUM NUMBER(3,0) null,
	CHARG_FORM CHAR(1) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_CWS_B_PK primary key (CWS_CD) ); 

-- 创建新表 P_WR_CHAN_G。
-- P_WR_CHAN_G : 渠道级别代码表
-- 	1 总干渠 
-- 	2 干渠 
-- 	3 支渠 
-- 	4 斗渠 
-- 	9 其他（或不明确） 
-- 	CHAN_G : CHAN_G 标识 P_WR_CHAN_G
-- 	CHAN_G_NM : CHAN_G_NM 属于 P_WR_CHAN_G  
create table P_WR_CHAN_G (
	CHAN_G CHAR(10) not null,
	CHAN_G_NM CHAR(10) null, constraint P_WR_CHAN_G_PK primary key (CHAN_G) ); 

-- 创建新表 P_WR_PUMP_TP。
-- P_WR_PUMP_TP : 泵站类型代码表
-- 	1 排水泵站 
-- 	2 供水泵站 
-- 	3 供排结合泵站 
-- 	PUMP_TP : PUMP_TP 标识 P_WR_PUMP_TP
-- 	PUMP_TP_NM : PUMP_TP_NM 属于 P_WR_PUMP_TP  
create table P_WR_PUMP_TP (
	PUMP_TP CHAR(1) not null,
	PUMP_TP_NM VARCHAR2(100) null, constraint P_WR_PUMP_TP_PK primary key (PUMP_TP) ); 

-- 创建新表 WR_PUMP_B。
-- WR_PUMP_B : 泵站工程基本信息表
-- 	PUMP_CD : 泵站工程代码
-- 	PUMP_NM : 泵站名称
-- 	LOC : 所在地
-- 	PROJ_SCAL : 工程规模
-- 	PUMP_TP : 泵站类型
-- 	INS_Q : 装机流量 计量单位为立方米每秒，计至一位小数
-- 	DES_HEAD : 设计扬程 计量单位为米，计至两位小数
-- 	PUMP_NUM : 水泵数量 
-- 	ALL_EQU_WW : 设备总取水能力 计量单位为立方米/秒，计至三位小数
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_PUMP_B (
	PUMP_CD CHAR(12) not null,
	PUMP_NM VARCHAR2(100) not null,
	LOC VARCHAR2(200) null,
	PROJ_SCAL CHAR(1) null,
	PUMP_TP CHAR(1) null,
	INS_Q NUMBER(9,1) null,
	DES_HEAD NUMBER(5,2) null,
	PUMP_NUM NUMBER(3,0) null,
	ALL_EQU_WW NUMBER(8,3) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_PUMP_B_PK primary key (PUMP_CD) ); 

-- 创建新表 WR_GATE_B。
-- WR_GATE_B : WR_GATE_B 的表
-- 	GATE_CD : 水闸工程代码
-- 	GATE_NM : 水闸名称
-- 	LOC : 所在地
-- 	PROJ_SCAL : 工程规模
-- 	GATE_HOLE_NUM : 闸孔数量
-- 	GATE_TW : 闸孔总净宽 计量单位为米，计至一位小数
-- 	DES_GATE_Q : 设计过闸流量 计量单位为立方米每秒，计至一位小数
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_GATE_B (
	GATE_CD CHAR(12) not null,
	GATE_NM VARCHAR2(100) not null,
	LOC VARCHAR2(200) null,
	PROJ_SCAL CHAR(1) null,
	GATE_HOLE_NUM NUMBER(3,0) null,
	GATE_TW NUMBER(4,1) null,
	DES_GATE_Q NUMBER(9,1) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_GATE_B_PK primary key (GATE_CD) ); 

-- 创建新表 P_WR_CONS_COND。
-- P_WR_CONS_COND : 建设状况代码表
-- 	1 已建 
-- 	2 在建 
-- 	3 待建
-- 	CONS_COND : CONS_COND 标识 P_WR_CONS_COND
-- 	CONS_COND_NM : CONS_COND_NM 属于 P_WR_CONS_COND  
create table P_WR_CONS_COND (
	CONS_COND CHAR(1) not null,
	CONS_COND_NM VARCHAR2(100) null, constraint P_WR_CONS_COND_PK primary key (CONS_COND) ); 

-- 创建新表 P_WR_WDE_TP。
-- P_WR_WDE_TP : 引调水方式代码表
-- 	1 提水式
-- 	2 自流式
-- 	WDE_TP : WDE_TP 标识 P_WR_WDE_TP
-- 	WDE_TP_NM : WDE_TP_NM 属于 P_WR_WDE_TP  
create table P_WR_WDE_TP (
	WDE_TP CHAR(1) not null,
	WDE_TP_NM VARCHAR2(100) null, constraint P_WR_WDE_TP_PK primary key (WDE_TP) ); 

-- 创建新表 WR_SWS_B。
-- WR_SWS_B : 地表水水源地基本信息表
-- 	SWS_CD : 地表水水源地代码
-- 	SWS_NM : 地表水水源地名称
-- 	SWS_TP : 地表水水源地类型
-- 	WAT_A : 水面面积 水源地常年水域总面积，包括一级保护区、二级保护区等水域面积，单位为平方公里，计至两位小数
-- 	WQ_GOAL : 水质目标 
-- 	CONS_COND : 建设状况
-- 	PUT_PROD_TM : 投产时间 若是在建和待建工程根据项目建设计划则填计划投产时间
-- 	RUN_COND : 运行状况
-- 	WS_COND : 水源供水持续状况
-- 	WS_OBJ : 供水对象 指水厂的供水对象，分为工业、生活、公共、混合等类型
-- 	DYWS_PP : 设计年供水人口 计量单位为万人，计至两位小数
-- 	DYWS_W : 设计年供水量 计量单位是百万立方米，计至三位小数
-- 	DDWS_W : 设计日供水量 计量单位是百万立方米，计至三位小数
-- 	WHS_MAN_CD : 水源地管理单位代码
-- 	WHS_APPR_CD : 水源地审批单位代码 约束同 WHS_MAN_CD
-- 	EM_CD : 应急管理单位代码 约束同 WHS_MAN_CD
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_SWS_B (
	SWS_CD CHAR(12) not null,
	SWS_NM VARCHAR2(100) not null,
	SWS_TP CHAR(1) null,
	WAT_A NUMBER(7,1) null,
	WQ_GOAL VARCHAR2(6) null,
	CONS_COND CHAR(1) null,
	PUT_PROD_TM DATE null,
	RUN_COND VARCHAR2(256) null,
	WS_COND CHAR(1) null,
	WS_OBJ CHAR(1) null,
	DYWS_PP NUMBER(6,2) null,
	DYWS_W NUMBER(10,3) null,
	DDWS_W NUMBER(10,3) null,
	WHS_MAN_CD CHAR(9) null,
	WHS_APPR_CD CHAR(9) null,
	EM_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_SWS_B_PK primary key (SWS_CD) ); 

-- 创建新表 WR_WDE_B。
-- WR_WDE_B : WR_WDE_B 的表
-- 	WDE_CD : 引（调）水工程代码
-- 	WDE_NM : 引（调）水工程名称
-- 	SWS_CD : 地表水水源地代码
-- 	LINE_A : 输水线路区
-- 	TOT_LINE_LEN : 输水干线总长 计量单位为千米，计至三位小数
-- 	INTK_A : 受水区范围
-- 	WDE_TP : 引调水方式
-- 	CONS_COND : 建设状况
-- 	DES_WDE_Q : 设计引调水流量 计量单位为立方米每秒，计至一位小数
-- 	DES_YR_DW : 设计年引调水量 计量单位为百万立方米，计至三位小数
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_WDE_B (
	WDE_CD CHAR(12) not null,
	WDE_NM VARCHAR2(100) not null,
	SWS_CD CHAR(12) null,
	LINE_A VARCHAR2(256) null,
	TOT_LINE_LEN NUMBER(7,3) null,
	INTK_A VARCHAR2(256) null,
	WDE_TP CHAR(1) null,
	CONS_COND CHAR(1) null,
	DES_WDE_Q NUMBER(9,1) null,
	DES_YR_DW NUMBER(10,3) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_WDE_B_PK primary key (WDE_CD) ); 

-- 创建新表 P_WR_HP_TP。
-- P_WR_HP_TP : 水电站类型代码表
-- 	1 闸坝试水电站
-- 	2 引水式水电站
-- 	3 混合式水电站
-- 	4 抽水蓄能式水电站
-- 	5 潮汐式水电站
-- 	HP_TP : HP_TP 标识 P_WR_HP_TP
-- 	HP_TP_NM : 水电站类型名称  
create table P_WR_HP_TP (
	HP_TP CHAR(1) not null,
	HP_TP_NM VARCHAR2(100) null, constraint P_WR_HP_TP_PK primary key (HP_TP) ); 

-- 创建新表 WR_HP_B。
-- WR_HP_B : WR_HP_B 的表
-- 	HP_CD : 水电站代码
-- 	HP_NM : 水电站名称
-- 	HP_TP : 水电站类型
-- 	INS_CAP : 装机容量 计量单位为千瓦(kW)，计至一位小数
-- 	AVG_EG : 多年平均发电量 计量单位为万千瓦时，计至一位小数
-- 	RES_CD : 所在水库代码
-- 	ENG_MAN_CD : 管理单位代码 
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_HP_B (
	HP_CD CHAR(12) not null,
	HP_NM VARCHAR2(100) not null,
	HP_TP CHAR(1) null,
	INS_CAP NUMBER(9,1) null,
	AVG_EG NUMBER(10,1) null,
	RES_CD CHAR(12) not null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_HP_B_PK primary key (HP_CD) ); 

-- 创建新表 ZDST_REL_水库河流关系表。
-- ZDST_REL_水库河流关系表 : zdst_rel_水库河流关系表 的表
-- 	水库_ID : 水库_id 标识 zdst_rel_水库河流关系表
-- 	河流_ID : 河流_id 部分标识 zdst_rel_水库河流关系表
-- 	出入库标记 : 出入库标记 0 入库 1出库  
create table ZDST_REL_水库河流关系表 (
	水库_ID VARCHAR2(32) not null,
	河流_ID VARCHAR2(32) not null,
	出入库标记 CHAR(1) null, constraint ZDST_REL_水库河流关系表_PK primary key (水库_ID, 河流_ID) ); 

-- 创建新表 ZDST_TYPE_水文测站使用性质。
-- ZDST_TYPE_水文测站使用性质 : “sys_水文测站使用性质”的表
-- 	0 使用
-- 	1 维护
-- 	2 停用
-- 	使用性质 : 使用性质 标识 zdst_type_水文测站使用性质
-- 	使用性质名称 : 使用性质名称 属于 zdst_type_水文测站使用性质
-- 	ICO_FLAG : ico_flag 属于 zdst_type_水文测站使用性质  
create table ZDST_TYPE_水文测站使用性质 (
	使用性质 CHAR(1) not null,
	使用性质名称 VARCHAR2(100) null,
	ICO_FLAG VARCHAR2(50) null, constraint ZDST_TYPE_水文测站使用性质_PK primary key (使用性质) ); 

-- 创建新表 ZDST_TYPE_水文测站建站性质表。
-- ZDST_TYPE_水文测站建站性质表 : “sys_水文测站建站性质表”的表
-- 	0 常备
-- 	1 临时
-- 	建站性质 : 建站性质 标识 zdst_type_水文测站建站性质表
-- 	建站性质名称 : 建站性质名称 属于 zdst_type_水文测站建站性质表
-- 	ICO_FLAG : ico_flag 属于 zdst_type_水文测站建站性质表  
create table ZDST_TYPE_水文测站建站性质表 (
	建站性质 CHAR(1) not null,
	建站性质名称 VARCHAR2(100) null,
	ICO_FLAG VARCHAR2(50) null, constraint ZDST_TYPE_水文测站建站性质表_PK primary key (建站性质) ); 

-- 创建新表 P_WR_WATER_FLOW。
-- P_WR_WATER_FLOW : 流量类型代码表
-- 	1 河道流量
-- 	2 湖库进出水流量
-- 	3 泵站流量
-- 	4 堰闸流量
-- 	5 取水流量
-- 	6 排水流量
-- 	7 泉涌流量
-- 	9 其他 
-- 	ITEM_Q : ITEM_Q 标识 P_WR_WATER_FLOW
-- 	ITEM_Q_NM : ITEM_Q_NM 属于 P_WR_WATER_FLOW  
create table P_WR_WATER_FLOW (
	ITEM_Q CHAR(1) not null,
	ITEM_Q_NM VARCHAR2(100) null, constraint P_WR_WATER_FLOW_PK primary key (ITEM_Q) ); 

-- 创建新表 P_WR_WATER_LEVEL。
-- P_WR_WATER_LEVEL : 水位类型代码表
-- 	1 河道水位
-- 	2 湖库水位
-- 	3 堰闸水位
-- 	4 泵站水位
-- 	5 地下水位
-- 	6 潮水位
-- 	9 其他
-- 	"ITEM_Z  " : ITEM_Z   标识 P_WR_WATER_LEVEL
-- 	ITEM_Z_NM : ITEM_Z_NM 属于 P_WR_WATER_LEVEL  
create table P_WR_WATER_LEVEL (
	"ITEM_Z  " CHAR(1) not null,
	ITEM_Z_NM VARCHAR2(100) null, constraint P_WR_WATER_LEVEL_PK primary key ("ITEM_Z  ") ); 

-- 创建新表 WR_STAT_ITEM_B。
-- WR_STAT_ITEM_B : 水文测站施测项目信息表
-- 	本表存储水文测站的施测项目信息
-- 	STCD : 测站代码
-- 	ITEM_P : 降水项目 是1 否0
-- 	ITEM_E : 蒸发及蒸发辅助项目  是1 否0
-- 	ITEM_Z : 水位项目
-- 	ITEM_Q : 流量项目
-- 	ITEM_GW_DEP : 地下水埋深项目  是1 否0
-- 	ITEM_WQ : 水质项目  是1 否0
-- 	ITEM_LAND_SUBS : 地面沉降量项目 是1 否0
-- 	ITEM_SAND : 泥沙项目  是1 否0
-- 	ITEM_ICESL : 冰凌项目 是1 否0
-- 	ITEM_TIDE : 潮汐项目  是1 否0
-- 	ITEM_OTHE : 其他项目 除以上所列施测项目以外的其他施测项目，如有多个施测项目则用“|”隔开
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_STAT_ITEM_B (
	STCD CHAR(8) not null,
	ITEM_P CHAR(1) null,
	ITEM_E CHAR(1) null,
	ITEM_Z CHAR(1) null,
	ITEM_Q CHAR(1) null,
	ITEM_GW_DEP CHAR(1) null,
	ITEM_WQ CHAR(1) null,
	ITEM_LAND_SUBS CHAR(1) null,
	ITEM_SAND CHAR(1) null,
	ITEM_ICESL CHAR(1) null,
	ITEM_TIDE CHAR(1) null,
	ITEM_OTHE VARCHAR2(50) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_STAT_ITEM_B_PK primary key (STCD) ); 

-- 创建新表 ZDST_水文测站信息简表。
-- ZDST_水文测站信息简表 : zdst_水文测站信息简表 的表
-- 	水文测站_ID : 水文测站ID uuid
-- 	WR_SD_CODE : wr_sd_code 标识 zdst_水文测站信息简表
-- 	经纬度坐标 : 经纬度坐标 属于 zdst_水文测站信息简表
-- 	建站性质 : 建站性质 0 常备 1 临时
-- 	使用性质 : 使用性质 属于 zdst_水文测站信息简表
-- 	REMARK : remark 属于 zdst_水文测站信息简表  
create table ZDST_水文测站信息简表 (
	水文测站_ID CHAR(32) not null,
	WR_SD_CODE VARCHAR2(32) not null,
	经纬度坐标 VARCHAR2(100) null,
	建站性质 CHAR(1) null,
	使用性质 CHAR(1) null,
	REMARK VARCHAR2(256) null, constraint ZDST_水文测站信息简表_PK primary key (水文测站_ID) ); 

-- 创建新表 ZDST_REL_水体与观测站关系表。
-- ZDST_REL_水体与观测站关系表 : zdst_rel_水体与观测站关系表 的表
-- 	水体_ID : 水体_id 标识 zdst_rel_水体与观测站关系表
-- 	水文测站_ID : 水文测站_id 部分标识 zdst_rel_水体与观测站关系表  
create table ZDST_REL_水体与观测站关系表 (
	水体_ID VARCHAR2(32) not null,
	水文测站_ID VARCHAR2(32) not null, constraint ZDST_REL_水体与观测站关系表_PK primary key (水体_ID, 水文测站_ID) ); 

-- 创建新表 P_WR_MON_TP_A。
-- P_WR_MON_TP_A : 监测方式A代码表
-- 	1 自动 
-- 	2 人工 
-- 	3 自动与人工相结合
-- 	MON_TP_A : 代码
-- 	MON_TP_NM : 监测方式  
create table P_WR_MON_TP_A (
	MON_TP_A CHAR(1) not null,
	MON_TP_NM VARCHAR2(100) null, constraint P_WR_MON_TP_A_PK primary key (MON_TP_A) ); 

-- 创建新表 P_WR_FLOW_DIR。
-- P_WR_FLOW_DIR : 水流流向代码表
-- 	1 单向
-- 	2 双向
-- 	3 无
-- 	FLOW_DIR : 代码
-- 	FLOW_DIR_NM : 水流流向  
create table P_WR_FLOW_DIR (
	FLOW_DIR CHAR(1) not null,
	FLOW_DIR_NM VARCHAR2(100) null, constraint P_WR_FLOW_DIR_PK primary key (FLOW_DIR) ); 

-- 创建新表 P_WR_STBK。
-- P_WR_STBK : 测站岸别代码表
-- 	0 左岸
-- 	1 右岸
-- 	2 不在河流上
-- 	STBK : 代码
-- 	STBK_NM : 测站岸别  
create table P_WR_STBK (
	STBK CHAR(1) not null,
	STBK_NM VARCHAR2(100) null, constraint P_WR_STBK_PK primary key (STBK) ); 

-- 创建新表 P_WR_ST_TP。
-- P_WR_ST_TP : 测站类别代码表
-- 	MM 气象站  1
-- 	PP 雨量站  2
-- 	BB 蒸发站  3
-- 	ZZ 河道水位站  4
-- 	DD 堰闸水文站  5
-- 	ZQ 河道水文站  6
-- 	TT 潮位站  7
-- 	RR 水库水文站  8
-- 	DP 泵站水文站  9
-- 	ZG 地下水站  10
-- 	SS 墒情站  11
-- 	ZB 分洪水位站  12
-- 	DC 地面沉降量测站  13
-- 	WQ 水质站  14
-- 	EL 其他测站  15
-- 	ST_TP : 代码
-- 	ST_TP_NM : 名称
-- 	ST_TP_SN : 排列序号
-- 	COLOR_FLAG : color_flag 属于 P_WR_ST_TP  
create table P_WR_ST_TP (
	ST_TP CHAR(2) not null,
	ST_TP_NM VARCHAR2(100) null,
	ST_TP_SN INT null,
	COLOR_FLAG VARCHAR2(50) null, constraint P_WR_ST_TP_PK primary key (ST_TP) ); 

-- 创建新表 WR_STAT_B。
-- WR_STAT_B : 水文测站基本信息表
-- 	本表存储除取用水测站以外的测站基本信息，包括水文（水位）站、水质站、潮位站等，详见测站类别表
-- 	测站岸别：水文测站站房位于河流的左岸或右岸的代码，按表8.3.1-3要求取值。若该河流为
-- 	往复流，则以河流主轴延伸方向来判别河流是东西方向还是南北方向，如为东西向河流则以北
-- 	岸为左岸，南岸为右岸；如为南北向河流则以东岸为左岸，西岸为右岸。
-- 	STCD : 测站代码
-- 	ST_NM : 测站名称
-- 	ST_TP : 测站类别
-- 	LOC : 所在地
-- 	STBK : 测站岸别
-- 	FLOW_DIR : 水流流向
-- 	DAT_TP : 基面类型
-- 	DAT_ELEV : 基面高程 计量单位 米
-- 	MOD_BAS_VAL : 修正基值 计量单位 米
-- 	MOD_PARA : 修正参数
-- 	MON_TP_A : 监测方式A
-- 	MON_TP_B : 监测方式B
-- 	EST_ST_YM : 设站年月
-- 	ENG_MAN_CD : 管理单位代码
-- 	PIN_YIN_CD : 拼音码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_STAT_B (
	STCD CHAR(8) not null,
	ST_NM VARCHAR2(100) not null,
	ST_TP CHAR(2) not null,
	LOC VARCHAR2(200) null,
	STBK CHAR(1) null,
	FLOW_DIR CHAR(1) null,
	DAT_TP CHAR(2) null,
	DAT_ELEV NUMBER(7,3) null,
	MOD_BAS_VAL NUMBER(7,3) null,
	MOD_PARA NUMBER(4,2) null,
	MON_TP_A CHAR(1) null,
	MON_TP_B CHAR(1) null,
	EST_ST_YM CHAR(6) null,
	ENG_MAN_CD CHAR(9) null,
	PIN_YIN_CD CHAR(6) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_STAT_B_PK primary key (STCD) ); 

-- 创建新表 WR_ADMA_B。
-- WR_ADMA_B : 涉水组织机构基本信息表
-- 	ORG_CD : 组织机构代码
-- 	ORG_NM : 机构名称
-- 	ORG_SHNM : 机构简称
-- 	ORG_TP : 所属机构类型
-- 	LR_NM : 法人代表
-- 	ORG_SCAL : 机构规格
-- 	MENB : 隶属关系
-- 	WEB : 网址
-- 	E_MAIL : 邮箱
-- 	ADDR : 地址
-- 	ZIP : 邮编
-- 	TEL : 办公室电话
-- 	FAX : 传真号码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_ADMA_B (
	ORG_CD CHAR(9) not null,
	ORG_NM VARCHAR2(100) not null,
	ORG_SHNM VARCHAR2(40) null,
	ORG_TP CHAR(1) null,
	LR_NM VARCHAR2(20) null,
	ORG_SCAL CHAR(1) null,
	MENB CHAR(1) null,
	WEB VARCHAR2(100) null,
	E_MAIL VARCHAR2(100) null,
	ADDR VARCHAR2(100) null,
	ZIP CHAR(6) null,
	TEL VARCHAR2(22) null,
	FAX VARCHAR2(22) null,
	TS DATE null,
	NT VARCHAR2(256) null, constraint WR_ADMA_B_PK primary key (ORG_CD) ); 

-- 创建新表 WR_TOWN_B。
-- WR_TOWN_B : 乡镇级行政区划代码表
-- 	TOWN_CD : 乡镇行政区划代码
-- 	TOWN_NM : 乡镇行政区划名称
-- 	AD_SHNM : 行政区划简称
-- 	AD_A : 行政区划面积
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_TOWN_B (
	TOWN_CD CHAR(9) not null,
	TOWN_NM VARCHAR2(100) not null,
	AD_SHNM VARCHAR2(20) null,
	AD_A NUMBER(9,2) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_TOWN_B_PK primary key (TOWN_CD) ); 

-- 创建新表 P_WR_DAT_TP。
-- P_WR_DAT_TP : 基面类型代码表
-- 	01 1985年国家高程基准 
-- 	02 1954年黄海高程系
-- 	03 1956年黄海高程系
-- 	04 榆林 
-- 	05 吴淞基面
-- 	06 珠江高程系
-- 	07 大沽高程系
-- 	08 大连高程系
-- 	09 波罗的海水准
-- 	10 渤海高程系
-- 	11 海防高程系
-- 	12 海口秀英港
-- 	99 其他
-- 	DAT_TP : 基面类型代码
-- 	DAT_TP_NM : 基面类型名称  
create table P_WR_DAT_TP (
	DAT_TP CHAR(2) not null,
	DAT_TP_NM VARCHAR2(100) null, constraint P_WR_DAT_TP_PK primary key (DAT_TP) ); 

-- 创建新表 P_WR_RES_REG_TP。
-- P_WR_RES_REG_TP : 水库调节方式代码表
-- 	1 无调节
-- 	2 日调节
-- 	3 周调节
-- 	4 年调节
-- 	5 多年调节
-- 	6 不完全年调节
-- 	7 不完全多年调节
-- 	RES_REG_TP : RES_REG_TP 标识 P_WR_RES_REG_TP
-- 	RES_REG_TP_NM : RES_REG_TP_NM 属于 P_WR_RES_REG_TP  
create table P_WR_RES_REG_TP (
	RES_REG_TP CHAR(1) not null,
	RES_REG_TP_NM VARCHAR2(100) null, constraint P_WR_RES_REG_TP_PK primary key (RES_REG_TP) ); 

-- 创建新表 P_WR_PROJ_SCAL。
-- P_WR_PROJ_SCAL : 工程规模表
-- 	1 大（1）型
-- 	2 大（2）型
-- 	3 中型
-- 	4 小（1）型
-- 	5 小（2）型
-- 	PROJ_SCAL : PROJ_SCAL 标识 P_WR_PROJ_SCAL
-- 	PROJ_SCAL_NM : PROJ_SCAL_NM 属于 P_WR_PROJ_SCAL  
create table P_WR_PROJ_SCAL (
	PROJ_SCAL CHAR(1) not null,
	PROJ_SCAL_NM VARCHAR2(100) null, constraint P_WR_PROJ_SCAL_PK primary key (PROJ_SCAL) ); 

-- 创建新表 WR_RES_B。
-- WR_RES_B : 水库基本信息表
-- 	RES_CD : 水库代码
-- 	RES_NM : 水库名称
-- 	LOC : 所在地
-- 	COMP_YM : 建成年月
-- 	PROJ_SCAL : 工程规模
-- 	DAT_TP : 基面类型
-- 	CAT_A : 集水面积 是指坝址以上的流域面积，计量单位为平方公里，计至两位小数
-- 	DES_FZ : 设计洪水位 是指水库遇设计标准洪水时，在坝前达到的最高水位，计量单位为米，计至三位小数
-- 	TOT_V : 总库容 计量单位为百万立方米，计至三位小数
-- 	FRSC_V : 调洪库容 计量单位为百万立方米，计至三位小数
-- 	NORM_WZ : 正常储水位 计量单位为米，计至三位小数
-- 	CF_Z : 校核洪水位 计量单位为米，计至三位小数
-- 	UTIL_V : 兴利库容 计量单位为百万立方米，计至三位小数
-- 	FS_Z : 防洪限制水位 计量单位为米，计至三位小数
-- 	FS_Z_V : 防洪限制水位库容 计量单位为百万立方米，计至三位小数
-- 	DEAD_Z : 死水位 计量单位为米，计至三位小数
-- 	DEAD_V : 死库容 计量单位为百万立方米，计至三位小数
-- 	RES_REG_TP : 水库调节方式
-- 	MIN_DISC : 最小下泄流量 计量单位为立方米每秒，计至一位小数
-- 	ST_END_LEN : 发电引水口至尾水口河道长度 计量单位为米
-- 	RHCC : 水库枢纽建筑物组成 
-- 	END_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_RES_B (
	RES_CD CHAR(12) not null,
	RES_NM VARCHAR2(100) not null,
	LOC VARCHAR2(200) null,
	COMP_YM CHAR(6) null,
	PROJ_SCAL CHAR(1) null,
	DAT_TP CHAR(2) null,
	CAT_A NUMBER(8,1) null,
	DES_FZ NUMBER(7,3) null,
	TOT_V NUMBER(10,3) null,
	FRSC_V NUMBER(9,3) null,
	NORM_WZ NUMBER(7,3) null,
	CF_Z NUMBER(7,3) null,
	UTIL_V NUMBER(9,3) null,
	FS_Z NUMBER(7,3) null,
	FS_Z_V NUMBER(10,3) null,
	DEAD_Z NUMBER(7,3) null,
	DEAD_V NUMBER(9,3) null,
	RES_REG_TP CHAR(1) null,
	MIN_DISC NUMBER(9,1) null,
	ST_END_LEN NUMBER(6,0) null,
	RHCC VARCHAR2(256) null,
	END_MAN_CD CHAR(9) null,
	TS DATE null,
	NT VARCHAR2(256) null, constraint WR_RES_B_PK primary key (RES_CD) ); 

-- 创建新表 P_WR_RV_G。
-- P_WR_RV_G : 河流等级代码表
-- 	0 干流河流
-- 	1 一级河流
-- 	2 二级河流
-- 	3 三级河流
-- 	4 四级河流
-- 	5 五级河流
-- 	6 六级河流
-- 	9 平原区、区间河流
-- 	RV_G : 河流等级代码
-- 	RV_G_NM : 河流等级名称  
create table P_WR_RV_G (
	RV_G CHAR(1) not null,
	RV_G_NM VARCHAR2(100) null, constraint P_WR_RV_G_PK primary key (RV_G) ); 

-- 创建新表 WR_RV_B。
-- WR_RV_B : 河流基本信息表
-- 	RV_CD : 河流代码
-- 	RV_NM : 河流名称
-- 	RV_G : 河流等级
-- 	RV_LEN : 河流长度 河流干流的长度，计量单位为千米，计至一位小数
-- 	RV_AG : 河流平均比降 计量单位千分制（‰），计至两位小数
-- 	IF_INN_RV : 是否为内流河 布尔型，如果是内流河，该处填“1”，如果不是则填“0”
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_RV_B (
	RV_CD CHAR(12) not null,
	RV_NM VARCHAR2(100) not null,
	RV_G CHAR(1) not null,
	RV_LEN NUMBER(8,1) null,
	RV_AG NUMBER(4,2) null,
	IF_INN_RV CHAR(1) null,
	TS DATE null,
	NT VARCHAR2(256) null, constraint WR_RV_B_PK primary key (RV_CD) ); 

-- 创建新表 ZDST_水体主体信息简表。
-- ZDST_水体主体信息简表 : “sys_水体主体信息简表”的表
-- 	水体_id 为 uuid 编码，专为本系统设定
-- 	standard_code 对应 《国家水资源监控能力建设项目标准》制定的水体代码，如河流代码、水库代码
-- 	水体_ID :  uuid 
-- 	TYPE_ID : type_id 属于 zdst_水体主体信息简表
-- 	WR_SD_CODE : 对应行标标准代码
-- 	经纬度坐标 : 经纬度坐标 属于 zdst_水体主体信息简表
-- 	DELETE_FLAG : delete_flag 删除标记 0正常 1删除
-- 	REMARK : remark 属于 zdst_水体主体信息简表  
create table ZDST_水体主体信息简表 (
	水体_ID CHAR(32) not null,
	TYPE_ID INT not null,
	WR_SD_CODE VARCHAR2(30) not null,
	经纬度坐标 VARCHAR2(100) null,
	DELETE_FLAG CHAR(1) null,
	REMARK VARCHAR2(200) null, constraint ZDST_水体主体信息简表_PK primary key (水体_ID) ); 

-- 创建新表 ZDST_TYPE_水体主体类型表。
-- ZDST_TYPE_水体主体类型表 : “sys_水体主体类型”的表
-- 	1 河流
-- 	2 水库 
-- 	3 湖泊
-- 	TYPE_ID : type_id 标识 
-- 	TYPE_NAME : type_name 属于 zdst_type_水体主体类型表
-- 	ICO_FLAG : ico_flag 属于 zdst_type_水体主体类型表  
create table ZDST_TYPE_水体主体类型表 (
	TYPE_ID INT not null,
	TYPE_NAME VARCHAR2(100) null,
	ICO_FLAG VARCHAR2(50) null, constraint ZDST_TYPE_水体主体类型表_PK primary key (TYPE_ID) ); 

-- 创建新表 P_WR_MON_TP_B。
-- P_WR_MON_TP_B : 监测方式B代码表
-- 	4 驻测 
-- 	5 巡测 
-- 	6 驻测与巡测相结合
-- 	MON_TP_B : 代码
-- 	MON_TP_NM : 监测方式  
create table P_WR_MON_TP_B (
	MON_TP_B CHAR(1) not null,
	MON_TP_NM VARCHAR2(100) null, constraint P_WR_MON_TP_B_PK primary key (MON_TP_B) ); 

-- 创建新表 WR_CHAN_B。
-- WR_CHAN_B : 渠道工程基本信息表
-- 	CHAN_CD : 渠道工程代码
-- 	CHAN_NM : 渠道名称
-- 	CHAN_G : 渠道级别
-- 	CHAN_DES_Q : 渠道设计流量 计量单位立方米每秒 计至一位小数
-- 	LEAK_R : 渗漏系数 计至三位小数
-- 	CHAN_LEN : 渠道长度 计量单位为米，计至一位小数
-- 	LIN_LEN : 衬砌长度 计量单位为米，计至一位小数
-- 	ENG_MAN_CD : 管理单位代码
-- 	TS : 时间戳
-- 	NT : 备注  
create table WR_CHAN_B (
	CHAN_CD CHAR(12) not null,
	CHAN_NM VARCHAR2(100) not null,
	CHAN_G CHAR(1) null,
	CHAN_DES_Q NUMBER(9,1) null,
	LEAK_R NUMBER(4,3) null,
	CHAN_LEN NUMBER(6,1) null,
	LIN_LEN NUMBER(6,1) null,
	ENG_MAN_CD CHAR(9) null,
	TS DATE not null,
	NT VARCHAR2(256) null, constraint WR_CHAN_B_PK primary key (CHAN_CD) ); 

-- 创建新表 MGMB_REL_取用水户与取用水测站点表。
-- MGMB_REL_取用水户与取用水测站点表 : mgmb_rel_取用水户与取用水测站点表 的表
-- 	取用水测站点_ID : 取用水测站点_id 标识 mgmb_rel_取用水户与取用水测站点表
-- 	取用水户_ID : 取用水户_id 部分标识 mgmb_rel_取用水户与取用水测站点表  
create table MGMB_REL_取用水户与取用水测站点表 (
	取用水测站点_ID CHAR(32) not null,
	取用水户_ID CHAR(32) not null, constraint 取用水户与取用水测站点_PK primary key (取用水测站点_ID, 取用水户_ID) ); 

-- 在表 MGMB_REL_水功能区与水体 中添加外键约束。
alter table MGMB_REL_水功能区与水体
	add constraint MGMB_水功能区与水体_FK1 foreign key (
		水功能区_ID)
	 references MGMB_水功能区信息简表 (
		水功能区_ID); 

alter table MGMB_REL_水功能区与水体
	add constraint ZDST_水功能区与水体_FK2 foreign key (
		水体_ID)
	 references ZDST_水体主体信息简表 (
		水体_ID); 

-- 在表 MGMB_敏感目标对象简表 中添加外键约束。
alter table MGMB_敏感目标对象简表
	add constraint MGMB_OBJTYP_MGMB_OBJ_FK1 foreign key (
		TYPE_ID)
	 references MGMB_TYPE_敏感目标类别表 (
		TYPE_ID); 

-- 在表 MGMB_REL_水功能区与水文测站 中添加外键约束。
alter table MGMB_REL_水功能区与水文测站
	add constraint MGMB_SGNQ_MGMB_SWCZ_FK1 foreign key (
		水功能区_ID)
	 references MGMB_水功能区信息简表 (
		水功能区_ID); 

alter table MGMB_REL_水功能区与水文测站
	add constraint MGMB_SGNQ_MGMB_SWCZ_FK2 foreign key (
		水文测站_ID)
	 references ZDST_水文测站信息简表 (
		水文测站_ID); 

-- 在表 WR_WFZ_STEL_B 中添加外键约束。
alter table WR_WFZ_STEL_B
	add constraint WR_WFZ_B_WR_WFZ_STEL_B_FK1 foreign key (
		UP_WFZ_CD)
	 references WR_WFZ_B (
		WFZ_CD); 

-- 在表 WR_WFZ_B 中添加外键约束。
alter table WR_WFZ_B
	add constraint P_WR_WQ_GOAL_WR_WFZ_B_FK1 foreign key (
		WFZ_WQT)
	 references P_WR_WQ_GOAL (
		WQ_GOAL); 

alter table WR_WFZ_B
	add constraint P_WR_MON_G_WR_WFZ_B_FK1 foreign key (
		MON_G)
	 references P_WR_MON_G (
		MON_G); 

-- 在表 MGMB_REL_河道断面与水文测站关系表 中添加外键约束。
alter table MGMB_REL_河道断面与水文测站关系表
	add constraint MGMB_HDDM_MGMB_SWCZ_FK1 foreign key (
		河道断面_ID)
	 references MGMB_河道断面信息简表 (
		河道断面_ID); 

alter table MGMB_REL_河道断面与水文测站关系表
	add constraint MGMB_HDDM_MGMB_SWCZ_FK2 foreign key (
		水文测站_ID)
	 references ZDST_水文测站信息简表 (
		水文测站_ID); 

-- 在表 WR_WFZB_B 中添加外键约束。
alter table WR_WFZB_B
	add constraint WR_RB_B_WR_WFZB_B_FK1 foreign key (
		RB_CD)
	 references WR_RB_B (
		RB_CD); 

-- 在表 WR_CSB_B 中添加外键约束。
alter table WR_CSB_B
	add constraint WR_RB_B_WR_CSB_B_FK1 foreign key (
		RB_CD)
	 references WR_RB_B (
		RB_CD); 

-- 在表 WR_AB_B 中添加外键约束。
alter table WR_AB_B
	add constraint P_WR_AB_G_WR_AB_B_FK1 foreign key (
		AB_G)
	 references P_WR_AB_G (
		AB_G); 

alter table WR_AB_B
	add constraint WR_ADMA_B_WR_AB_B_FK1 foreign key (
		SUP_ORG_CD)
	 references WR_ADMA_B (
		ORG_CD); 

alter table WR_AB_B
	add constraint WR_RB_B_WR_AB_B_FK1 foreign key (
		RB_CD)
	 references WR_RB_B (
		RB_CD); 

alter table WR_AB_B
	add constraint WR_AD_B_WR_AB_B_FK1 foreign key (
		US_AD_CD)
	 references WR_AD_B (
		AD_CD); 

-- 在表 MGMB_REL_污水厂与行政区关系表 中添加外键约束。
alter table MGMB_REL_污水厂与行政区关系表
	add constraint ORG_XZQ_WSCH_FK2 foreign key (
		污水处理厂_ID)
	 references MGMB_污水处理厂信息简表 (
		污水处理厂_ID); 

alter table MGMB_REL_污水厂与行政区关系表
	add constraint ORG_XZQ_WSCH_FK2 foreign key (
		行政区_ID)
	 references ORG_行政区信息简表 (
		行政区_ID); 

-- 在表 MGMB_REL_入河排污口与水文测站关系表 中添加外键约束。
alter table MGMB_REL_入河排污口与水文测站关系表
	add constraint MGMB_PWKL_SWCZ_FK1 foreign key (
		入河排污口_ID)
	 references MGMB_入河排污口信息简表 (
		入河排污口_ID); 

alter table MGMB_REL_入河排污口与水文测站关系表
	add constraint MGMB_PWKL_SWCZ_FK2 foreign key (
		水文测站_ID)
	 references ZDST_水文测站信息简表 (
		水文测站_ID); 

-- 在表 MGMB_REL_入河排污口与取用水测站关系表 中添加外键约束。
alter table MGMB_REL_入河排污口与取用水测站关系表
	add constraint MGMB_PWK_QYSCZ_FK1 foreign key (
		入河排污口_ID)
	 references MGMB_入河排污口信息简表 (
		入河排污口_ID); 

alter table MGMB_REL_入河排污口与取用水测站关系表
	add constraint MGMB_PWK_QYSCZ_FK2 foreign key (
		取用水测站点_ID)
	 references MGMB_取用水测站测点简表 (
		取用水测站点_ID); 

-- 在表 MGMB_REL_入河排污口与取用水户关系表 中添加外键约束。
alter table MGMB_REL_入河排污口与取用水户关系表
	add constraint MGMB_PWK_QYSH_FK1 foreign key (
		入河排污口_ID)
	 references MGMB_入河排污口信息简表 (
		入河排污口_ID); 

alter table MGMB_REL_入河排污口与取用水户关系表
	add constraint MGMB_PWK_QYSH_FK2 foreign key (
		取用水户_ID)
	 references MGMB_取用水户信息简表 (
		取用水户_ID); 

-- 在表 MGMB_REL_入河排污口与水功能区关系冗余表 中添加外键约束。
alter table MGMB_REL_入河排污口与水功能区关系冗余表
	add constraint MGMB_PWK_SGNQRY_FK1 foreign key (
		入河排污口_ID)
	 references MGMB_入河排污口信息简表 (
		入河排污口_ID); 

alter table MGMB_REL_入河排污口与水功能区关系冗余表
	add constraint MGMB_PWK_SGNQRY_FK2 foreign key (
		水功能区_ID)
	 references MGMB_水功能区信息简表 (
		水功能区_ID); 

-- 在表 MGMB_REL_入河排污口与水体主体关系 中添加外键约束。
alter table MGMB_REL_入河排污口与水体主体关系
	add constraint MGMB_PWK_STZT_FK1 foreign key (
		入河排污口_ID)
	 references MGMB_入河排污口信息简表 (
		入河排污口_ID); 

alter table MGMB_REL_入河排污口与水体主体关系
	add constraint MGMB_PWK_STZT_FK2 foreign key (
		水体_ID)
	 references ZDST_水体主体信息简表 (
		水体_ID); 

-- 在表 WR_PDO_B 中添加外键约束。
alter table WR_PDO_B
	add constraint P_WR_PDO_TP_WR_PDO_B_FK1 foreign key (
		PDO_TP)
	 references P_WR_PDO_TP (
		PDO_TP); 

alter table WR_PDO_B
	add constraint P_WR_EMIS_TP_WR_PDO_B_FK1 foreign key (
		EMIS_TP)
	 references P_WR_EMIS_TP (
		EMIS_TP); 

alter table WR_PDO_B
	add constraint P_WR_IN_RV_TP_WR_PDO_B_FK1 foreign key (
		IN_RV_TP)
	 references P_WR_IN_RV_TP (
		IN_RV_TP); 

alter table WR_PDO_B
	add constraint WR_ADMA_B_WR_PDO_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

alter table WR_PDO_B
	add constraint WR_WFZ_B_WR_PDO_B_FK1 foreign key (
		WFZ_CD)
	 references WR_WFZ_B (
		WFZ_CD); 

-- 在表 WR_STP_B 中添加外键约束。
alter table WR_STP_B
	add constraint P_WR_CONS_SCAL_WR_STP_B_FK1 foreign key (
		CONS_SCAL)
	 references P_WR_CONS_SCAL (
		CONS_SCAL); 

-- 在表 MGMB_REL_取水口与取用水测站测点关系表 中添加外键约束。
alter table MGMB_REL_取水口与取用水测站测点关系表
	add constraint MGMB_QSK_QYSCZ_FK1 foreign key (
		取水口_ID)
	 references MGMB_取水口信息简表 (
		取水口_ID); 

alter table MGMB_REL_取水口与取用水测站测点关系表
	add constraint MGMB_QSK_QYSCZ_FK2 foreign key (
		取用水测站点_ID)
	 references MGMB_取用水测站测点简表 (
		取用水测站点_ID); 

-- 在表 MGMB_REL_取水口与取用水户关系表 中添加外键约束。
alter table MGMB_REL_取水口与取用水户关系表
	add constraint MGMB_QYSH_QSK_FK1 foreign key (
		取水口_ID)
	 references MGMB_取水口信息简表 (
		取水口_ID); 

alter table MGMB_REL_取水口与取用水户关系表
	add constraint MGMB_QYSH_QSK_FK2 foreign key (
		取用水户_ID)
	 references MGMB_取用水户信息简表 (
		取用水户_ID); 

-- 在表 MGMB_取用水测站测点简表 中添加外键约束。
alter table MGMB_取用水测站测点简表
	add constraint MGMB_QYSCZTYPE_QYS_FK1 foreign key (
		TYPE_ID)
	 references MGMB_TYPE_取用水测站测点类型表 (
		TYPE_ID); 

-- 在表 WR_WIUST_B 中添加外键约束。
alter table WR_WIUST_B
	add constraint P_WR_WIUST_TP_WR_WIUST_B_FK1 foreign key (
		WIUST_TP)
	 references P_WR_WIUST_TP (
		WIUST_TP); 

alter table WR_WIUST_B
	add constraint P_WR_DAT_TP_WR_WIUST_B_FK1 foreign key (
		DAT_TP)
	 references P_WR_DAT_TP (
		DAT_TP); 

alter table WR_WIUST_B
	add constraint P_WR_WELL_TP_WR_WIUST_B_FK1 foreign key (
		WELL_TP)
	 references P_WR_WELL_TP (
		WELL_TP); 

alter table WR_WIUST_B
	add constraint WR_ADMA_B_WR_WIUST_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_MP_B 中添加外键约束。
alter table WR_MP_B
	add constraint P_WR_MP_ITEM_WR_MP_B_FK1 foreign key (
		MP_ITEM)
	 references P_WR_MP_ITEM (
		MP_ITEM); 

-- 在表 MGMB_取水口信息简表 中添加外键约束。
alter table MGMB_取水口信息简表
	add constraint MGMB_QSKTYPE_QSK_FK1 foreign key (
		TYPE_ID)
	 references MGMB_TYPE_取水口类别表 (
		TYPE_ID); 

-- 在表 WR_WFCT_B 中添加外键约束。
alter table WR_WFCT_B
	add constraint P_WR_WS_OBJ_WR_WFCT_B_FK1 foreign key (
		WS_OBJ)
	 references P_WR_WS_OBJ (
		WS_OBJ); 

alter table WR_WFCT_B
	add constraint WR_ADMA_B_WR_WFCT_B_FK1 foreign key (
		WFCT_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_WIU_B 中添加外键约束。
alter table WR_WIU_B
	add constraint P_WR_TRAD_TP_WR_WIU_B_FK1 foreign key (
		TRAD_TP)
	 references P_WR_TRAD_TP (
		TRAD_TP); 

alter table WR_WIU_B
	add constraint P_WR_ECO_TP_WR_WIU_B_FK1 foreign key (
		ECO_TP)
	 references P_WR_ECO_TP (
		ECO_TP); 

alter table WR_WIU_B
	add constraint P_WR_MON_G_WR_WIU_B_FK1 foreign key (
		MON_G)
	 references P_WR_MON_G (
		MON_G); 

-- 在表 WR_WELL_B 中添加外键约束。
alter table WR_WELL_B
	add constraint P_WR_MIND_BED_WR_WELL_B_FK1 foreign key (
		MIND_BED)
	 references P_WR_MIND_BED (
		MIND_BED); 

alter table WR_WELL_B
	add constraint WR_ADMA_B_WR_WELL_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_INT_B 中添加外键约束。
alter table WR_INT_B
	add constraint P_WR_INT_TP_WR_INT_B_FK1 foreign key (
		INT_TP)
	 references P_WR_INT_TP (
		INT_TP); 

alter table WR_INT_B
	add constraint WR_ADMA_B_WR_INT_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 MGMB_REL_水源地与供水行政区关系表 中添加外键约束。
alter table MGMB_REL_水源地与供水行政区关系表
	add constraint MGMB_SYD_GSXZQ_FK1 foreign key (
		水源地_ID)
	 references MGMB_水源地信息简表 (
		水源地_ID); 

alter table MGMB_REL_水源地与供水行政区关系表
	add constraint MGMB_SYD_GSXZQ_FK2 foreign key (
		行政区_ID)
	 references ORG_行政区信息简表 (
		行政区_ID); 

-- 在表 ORG_行政区信息简表 中添加外键约束。
alter table ORG_行政区信息简表
	add constraint ORG_XZQTYPE_XZQ_FK1 foreign key (
		行政区级别_ID)
	 references ORG_TYPE_行政区类别表 (
		行政区级别_ID); 

-- 在表 MGMB_REL_水源地与水文测站关系表 中添加外键约束。
alter table MGMB_REL_水源地与水文测站关系表
	add constraint MGMB_SYD_SWCZ_FK1 foreign key (
		水源地_ID)
	 references MGMB_水源地信息简表 (
		水源地_ID); 

alter table MGMB_REL_水源地与水文测站关系表
	add constraint MGMB_SYD_SWCZ_FK2 foreign key (
		水文测站_ID)
	 references ZDST_水文测站信息简表 (
		水文测站_ID); 

-- 在表 MGMB_REL_水源地与水功能区关系表 中添加外键约束。
alter table MGMB_REL_水源地与水功能区关系表
	add constraint MGMB_SYD_SGNQ_FK1 foreign key (
		水源地_ID)
	 references MGMB_水源地信息简表 (
		水源地_ID); 

alter table MGMB_REL_水源地与水功能区关系表
	add constraint MGMB_SYD_SGNQ_FK2 foreign key (
		水功能区_ID)
	 references MGMB_水功能区信息简表 (
		水功能区_ID); 

-- 在表 MGMB_水源地信息简表 中添加外键约束。
alter table MGMB_水源地信息简表
	add constraint MGMB_SHDTYPE_SYD_FK1 foreign key (
		TYPE_ID)
	 references MGMB_TYPE_水源地类别表 (
		TYPE_ID); 

-- 在表 WR_GWS_B 中添加外键约束。
alter table WR_GWS_B
	add constraint P_WR_CONS_COND_WR_GWS_B_FK1 foreign key (
		CONS_COND)
	 references P_WR_CONS_COND (
		CONS_COND); 

alter table WR_GWS_B
	add constraint P_WR_WS_OBJ_WR_GWS_B_FK1 foreign key (
		WS_OBJ)
	 references P_WR_WS_OBJ (
		WS_OBJ); 

alter table WR_GWS_B
	add constraint WR_ADMA_B_WR_GWS_B_FK1 foreign key (
		WHS_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_LK_B 中添加外键约束。
alter table WR_LK_B
	add constraint P_WR_SFW_PROP_WR_LK_B_FK1 foreign key (
		SFW_PROP)
	 references P_WR_SFW_PROP (
		SFW_PROP); 

-- 在表 ZDST_重点水体对象简表 中添加外键约束。
alter table ZDST_重点水体对象简表
	add constraint ZDST_TYPE_OBJ_FK1 foreign key (
		TYPE_ID)
	 references ZDST_TYPE_重点水体对象类型表 (
		TYPE_ID); 

-- 在表 ZDST_REL_水利工程项目与水体关系表 中添加外键约束。
alter table ZDST_REL_水利工程项目与水体关系表
	add constraint ZDST_PRJ_ST_FK1 foreign key (
		水利工程项目_ID)
	 references ZDST_水利工程项目简表 (
		水利工程项目_ID); 

alter table ZDST_REL_水利工程项目与水体关系表
	add constraint ZDST_PRJ_ST_FK2 foreign key (
		水体_ID)
	 references ZDST_水体主体信息简表 (
		水体_ID); 

-- 在表 ZDST_水利工程项目简表 中添加外键约束。
alter table ZDST_水利工程项目简表
	add constraint ZDST_PRJTYPE_PRJ_FK1 foreign key (
		TYPE_ID)
	 references ZDST_TYPE_水利工程项目类别表 (
		TYPE_ID); 

-- 在表 WR_IRR_B 中添加外键约束。
alter table WR_IRR_B
	add constraint P_WR_IRR_TP_WR_IRR_B_FK1 foreign key (
		IRR_TP)
	 references P_WR_IRR_TP (
		IRR_TP); 

alter table WR_IRR_B
	add constraint WR_ADMA_B_WR_IRR_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

alter table WR_IRR_B
	add constraint P_WR_IRR_SCAL_WR_IRR_B_FK1 foreign key (
		IRR_SCAL)
	 references P_WR_IRR_SCAL (
		IRR_SCAL); 

-- 在表 WR_CWS_B 中添加外键约束。
alter table WR_CWS_B
	add constraint P_WR_CWS_TP_WR_CWS_B_FK1 foreign key (
		CWS_TP)
	 references P_WR_CWS_TP (
		CWS_TP); 

alter table WR_CWS_B
	add constraint P_WR_CHARG_FORM_WR_CWS_B_FK1 foreign key (
		CHARG_FORM)
	 references P_WR_CHARG_FORM (
		CHARG_FORM); 

alter table WR_CWS_B
	add constraint WR_ADMA_B_WR_CWS_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_PUMP_B 中添加外键约束。
alter table WR_PUMP_B
	add constraint P_WR_PROJ_SCAL_WR_PUMP_B_FK1 foreign key (
		PROJ_SCAL)
	 references P_WR_PROJ_SCAL (
		PROJ_SCAL); 

alter table WR_PUMP_B
	add constraint P_WR_PUMP_TP_WR_PUMP_B_FK1 foreign key (
		PUMP_TP)
	 references P_WR_PUMP_TP (
		PUMP_TP); 

alter table WR_PUMP_B
	add constraint WR_ADMA_B_WR_PUMP_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_GATE_B 中添加外键约束。
alter table WR_GATE_B
	add constraint WR_ADMA_B_WR_GATE_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

alter table WR_GATE_B
	add constraint P_WR_PROJ_SCAL_WR_GATE_B_FK1 foreign key (
		PROJ_SCAL)
	 references P_WR_PROJ_SCAL (
		PROJ_SCAL); 

-- 在表 WR_SWS_B 中添加外键约束。
alter table WR_SWS_B
	add constraint P_WR_SWS_TP_WR_SWS_B_FK1 foreign key (
		SWS_TP)
	 references P_WR_SWS_TP (
		SWS_TP); 

alter table WR_SWS_B
	add constraint P_WR_WQ_GOAL_WR_SWS_B_FK1 foreign key (
		WQ_GOAL)
	 references P_WR_WQ_GOAL (
		WQ_GOAL); 

alter table WR_SWS_B
	add constraint P_WR_CONS_COND_WR_SWS_B_FK1 foreign key (
		CONS_COND)
	 references P_WR_CONS_COND (
		CONS_COND); 

alter table WR_SWS_B
	add constraint P_WR_WS_COND_WR_SWS_B_FK1 foreign key (
		WS_COND)
	 references P_WR_WS_COND (
		WS_COND); 

alter table WR_SWS_B
	add constraint P_WR_WS_OBJ_WR_SWS_B_FK1 foreign key (
		WS_OBJ)
	 references P_WR_WS_OBJ (
		WS_OBJ); 

alter table WR_SWS_B
	add constraint WR_ADMA_B_WR_SWS_B_FK1 foreign key (
		WHS_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_WDE_B 中添加外键约束。
alter table WR_WDE_B
	add constraint WR_ADMA_B_WR_WDE_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

alter table WR_WDE_B
	add constraint WR_SWS_B_WR_WDE_B_FK1 foreign key (
		SWS_CD)
	 references WR_SWS_B (
		SWS_CD); 

alter table WR_WDE_B
	add constraint P_WR_WDE_TP_WR_WDE_B_FK1 foreign key (
		WDE_TP)
	 references P_WR_WDE_TP (
		WDE_TP); 

alter table WR_WDE_B
	add constraint P_WR_CONS_COND_WR_WDE_B_FK1 foreign key (
		CONS_COND)
	 references P_WR_CONS_COND (
		CONS_COND); 

-- 在表 WR_HP_B 中添加外键约束。
alter table WR_HP_B
	add constraint P_WR_HP_TP_WR_HP_B_FK1 foreign key (
		HP_TP)
	 references P_WR_HP_TP (
		HP_TP); 

alter table WR_HP_B
	add constraint WR_RES_B_WR_HP_B_FK1 foreign key (
		RES_CD)
	 references WR_RES_B (
		RES_CD); 

alter table WR_HP_B
	add constraint WR_ADMA_B_WR_HP_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_STAT_ITEM_B 中添加外键约束。
alter table WR_STAT_ITEM_B
	add constraint WATER_LEVEL_WR_STAT_ITEM_B_FK1 foreign key (
		ITEM_Z)
	 references P_WR_WATER_LEVEL (
		"ITEM_Z  "); 

alter table WR_STAT_ITEM_B
	add constraint WATER_FLOW_WR_STAT_ITEM_B_FK1 foreign key (
		ITEM_Q)
	 references P_WR_WATER_FLOW (
		ITEM_Q); 

alter table WR_STAT_ITEM_B
	add constraint WR_STAT_B_WR_STAT_ITEM_B_FK1 foreign key (
		STCD)
	 references WR_STAT_B (
		STCD); 

-- 在表 ZDST_水文测站信息简表 中添加外键约束。
alter table ZDST_水文测站信息简表
	add constraint ZDST_SWCZTYPE_SWCZ_FK1 foreign key (
		建站性质)
	 references ZDST_TYPE_水文测站建站性质表 (
		建站性质); 

alter table ZDST_水文测站信息简表
	add constraint ZDST_SWCZTYPE_SWCZ_FK1 foreign key (
		使用性质)
	 references ZDST_TYPE_水文测站使用性质 (
		使用性质); 

-- 在表 ZDST_REL_水体与观测站关系表 中添加外键约束。
alter table ZDST_REL_水体与观测站关系表
	add constraint ZDST_OBJ_SWCZ_FK1 foreign key (
		水体_ID)
	 references ZDST_水体主体信息简表 (
		水体_ID); 

alter table ZDST_REL_水体与观测站关系表
	add constraint ZDST_OBJ_SWCZ_FK1 foreign key (
		水文测站_ID)
	 references ZDST_水文测站信息简表 (
		水文测站_ID); 

-- 在表 WR_STAT_B 中添加外键约束。
alter table WR_STAT_B
	add constraint P_WR_ST_TP_WR_STAT_B_FK1 foreign key (
		ST_TP)
	 references P_WR_ST_TP (
		ST_TP); 

alter table WR_STAT_B
	add constraint P_WR_STBK_WR_STAT_B_FK1 foreign key (
		STBK)
	 references P_WR_STBK (
		STBK); 

alter table WR_STAT_B
	add constraint P_WR_FLOW_DIR_WR_STAT_B_FK1 foreign key (
		FLOW_DIR)
	 references P_WR_FLOW_DIR (
		FLOW_DIR); 

alter table WR_STAT_B
	add constraint P_WR_DAT_TP_WR_STAT_B_FK1 foreign key (
		DAT_TP)
	 references P_WR_DAT_TP (
		DAT_TP); 

alter table WR_STAT_B
	add constraint P_WR_MON_TP_A_WR_STAT_B_FK1 foreign key (
		MON_TP_A)
	 references P_WR_MON_TP_A (
		MON_TP_A); 

alter table WR_STAT_B
	add constraint P_WR_MON_TP_B_WR_STAT_B_FK1 foreign key (
		MON_TP_B)
	 references P_WR_MON_TP_B (
		MON_TP_B); 

alter table WR_STAT_B
	add constraint WR_ADMA_B_WR_STAT_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_ADMA_B 中添加外键约束。
alter table WR_ADMA_B
	add constraint P_WR_ORG_TP_WR_ADMA_B_FK1 foreign key (
		ORG_TP)
	 references P_WR_ORG_TP (
		ORG_TP); 

alter table WR_ADMA_B
	add constraint P_WR_ORG_SCAL_WR_ADMA_B_FK1 foreign key (
		ORG_SCAL)
	 references P_WR_ORG_SCAL (
		ORG_SCAL); 

alter table WR_ADMA_B
	add constraint P_WR_MENB_WR_ADMA_B_FK1 foreign key (
		MENB)
	 references P_WR_MENB (
		MENB); 

-- 在表 WR_RES_B 中添加外键约束。
alter table WR_RES_B
	add constraint P_WR_PROJ_SCAL_WR_RES_B_FK1 foreign key (
		PROJ_SCAL)
	 references P_WR_PROJ_SCAL (
		PROJ_SCAL); 

alter table WR_RES_B
	add constraint P_WR_RES_REG_TP_WR_RES_B_FK1 foreign key (
		RES_REG_TP)
	 references P_WR_RES_REG_TP (
		RES_REG_TP); 

alter table WR_RES_B
	add constraint P_WR_DAT_TP_WR_RES_B_FK1 foreign key (
		DAT_TP)
	 references P_WR_DAT_TP (
		DAT_TP); 

alter table WR_RES_B
	add constraint WR_ADMA_B_WR_RES_B_FK1 foreign key (
		END_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 WR_RV_B 中添加外键约束。
alter table WR_RV_B
	add constraint P_WR_RV_G_WR_RV_B_FK1 foreign key (
		RV_G)
	 references P_WR_RV_G (
		RV_G); 

-- 在表 ZDST_水体主体信息简表 中添加外键约束。
alter table ZDST_水体主体信息简表
	add constraint ZDST_OBJTYPE_OBJ_FK1 foreign key (
		TYPE_ID)
	 references ZDST_TYPE_水体主体类型表 (
		TYPE_ID); 

-- 在表 WR_CHAN_B 中添加外键约束。
alter table WR_CHAN_B
	add constraint P_WR_CHAN_G_WR_CHAN_B_FK1 foreign key (
		CHAN_G)
	 references P_WR_CHAN_G (
		CHAN_G); 

alter table WR_CHAN_B
	add constraint WR_ADMA_B_WR_CHAN_B_FK1 foreign key (
		ENG_MAN_CD)
	 references WR_ADMA_B (
		ORG_CD); 

-- 在表 MGMB_REL_取用水户与取用水测站点表 中添加外键约束。
alter table MGMB_REL_取用水户与取用水测站点表
	add constraint MGMB_QSCZ_QYSH_FK1 foreign key (
		取用水测站点_ID)
	 references MGMB_取用水测站测点简表 (
		取用水测站点_ID); 

alter table MGMB_REL_取用水户与取用水测站点表
	add constraint MGMB_QSCZ_QYSH_FK2 foreign key (
		取用水户_ID)
	 references MGMB_取用水户信息简表 (
		取用水户_ID); 


-- 这是 Microsoft Visual Studio 生成的 SQL DDL 脚本的末尾。
