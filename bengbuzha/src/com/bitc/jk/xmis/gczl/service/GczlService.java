package com.bitc.jk.xmis.gczl.service;

import java.math.BigDecimal;
import java.util.List;

import com.bitc.jk.xmis.gczl.model.ConcSeam;
import com.bitc.jk.xmis.gczl.model.ConcreteCarb;
import com.bitc.jk.xmis.gczl.model.FlexSeam;
import com.bitc.jk.xmis.gczl.model.HorizontalShift;
import com.bitc.jk.xmis.gczl.model.OphContent;
import com.bitc.jk.xmis.gczl.model.Sink;
import com.bitc.jk.xmis.gczl.model.SteelSCT;
import com.bitc.jk.xmis.gczl.model.riverDm;
import com.bitc.jk.xmis.gczl.model.weather;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface GczlService {

	String getFsInfoListWithGroupQuery(PageInfo pageInfo, String queryStr,
			StandardQuery query);

	String getFsCodeList();

	int doSavefsInfo(FlexSeam info);

	void doDeletefsInfoById(int tab_seed);

	String getFsColumnNameCombo();

	String getSpCodeList();

	String getS_point_codeList(String sp_code);

	int doSaveSinkInfo(Sink info);

	void doDeleteSinkInfoById(int tab_seed);

	String getSinkInfoListWithGroupQuery(PageInfo pageInfo, String queryStr,
			StandardQuery query);

	String getSinkColumnNameCombo();

	String getWeatherCodeList();

	int doSaveWeatherInfo(weather info);

	void doDeleteWeatherInfoById(int tab_seed);

	String getWeatherInfoListWithGroupQuery(PageInfo pageInfo, String queryStr,
			StandardQuery query);

	String getWeatherColumnNameCombo();

	String getRiverDmList();

	int doSaveRiverDmInfo(riverDm info);

	void doDeleteRiverDmInfoById(int tab_seed);

	String getRiverDmInfoListWithGroupQuery(PageInfo pageInfo, String queryStr,
			StandardQuery query);

	String getRiverDmColumnNameCombo();

	String getConcreteCarbList();

	int doSaveConcreteCarbInfo(ConcreteCarb info);

	void doDeleteConcreteCarbInfoById(int tab_seed);

	String getConcreteCarbInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query);

	String getConcreteCarbColumnNameCombo();

	String getSteelSCTList();

	int doSaveSteelSCTInfo(SteelSCT info);

	void doDeleteSteelSCTInfoById(int tab_seed);

	String getSteelSCTInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query);

	String getSteelSCTColumnNameCombo();

	String getHorizontalShiftList();

	int doSaveHorizontalShiftInfo(HorizontalShift info);

	void doDeleteHorizontalShiftInfoById(int tab_seed);

	String getHorizontalShiftInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query);

	String getHorizontalShiftColumnNameCombo();

	BigDecimal getStart_formula(String hori_code);

	String getRiverCodeList();

	String getPlaceCodeList();

	int doSaveOphContentInfo(OphContent info);

	void doDeleteOphContentInfoById(int tab_seed);

	String getOphContentInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query);

	String getOphContentColumnNameCombo();

	String getConcSeamCodeList();

	String getConcMpCodeList(String concSeamCode);

	int doSaveConcSeamInfo(ConcSeam info);

	void doDeleteConcSeamInfoById(int tab_seed);

	String getConcSeamInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query);

	String getConcSeamColumnNameCombo();

	String getHightDegree(PageInfo pageInfo, String last_date, String the_date, String sp_code);

	String getFlexSeam(PageInfo pageInfo, String start_date, String end_date,
			String fs_code);

	String getCeYaStatistics(PageInfo pageInfo, String start_date,
			String end_date, String mptp_code, String mpt_name);

	String getMaxAndMinFlexSeam(String start_date, String end_date,
			String fs_code);

	String getMaxAndMinCeYa(String start_date, String end_date,
			String mptp_code, String mpt_name);

	String getLLValue(PageInfo pageInfo, String start_date, String end_date,
			String type);

	String getWaterLevel(String start_date, String end_date);

	List getLiuLiang(String gate_open_num_1, String gate_open_degree_1,
			String gate_open_num_2, String gate_open_degree_2, String ap,
			String bp,String elec_machine);

	String getSWJWaterLevel(String start_date,
			String end_date);

}
