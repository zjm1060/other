package com.bitc.jk.xmis.gczl.dao;

import java.math.BigDecimal;
import java.util.List;

import com.bitc.jk.xmis.applymanage.model.CeyaInfo;
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

public interface GczlDao {

	List getFsInfoListWithGroupQuery(String queryStr, PageInfo pageInfo,
			StandardQuery query);

	List getFsCodeList();

	int doSavefsInfo(FlexSeam info);

	void doDeletefsInfoById(int tab_seed);

	List getFsColumnNameCombo();

	List getSpCodeList();

	List getS_point_codeList(String sp_code);

	int doSaveSinkInfo(Sink info);

	void doDeleteSinkInfoById(int tab_seed);

	List getSinkInfoListWithGroupQuery(String queryStr, PageInfo pageInfo,
			StandardQuery query);

	List getSinkColumnNameCombo();

	List getWeatherCodeList();

	int doSaveWeatherInfo(weather info);

	void doDeleteWeatherInfoById(int tab_seed);

	List getWeatherInfoListWithGroupQuery(String queryStr, PageInfo pageInfo,
			StandardQuery query);

	List getWeatherColumnNameCombo();

	List getRiverDmList();

	int doSaveRiverDmInfo(riverDm info);

	void doDeleteRiverDmInfoById(int tab_seed);

	List getRiverDmInfoListWithGroupQuery(String queryStr, PageInfo pageInfo,
			StandardQuery query);

	List getRiverDmColumnNameCombo();

	List getConcreteCarbList();

	int doSaveConcreteCarbInfo(ConcreteCarb info);

	void doDeleteConcreteCarbInfoById(int tab_seed);

	List getConcreteCarbInfoListWithGroupQuery(String queryStr,
			PageInfo pageInfo, StandardQuery query);

	List getConcreteCarbColumnNameCombo();

	List getSteelSCTList();

	int doSaveSteelSCTInfo(SteelSCT info);

	void doDeleteSteelSCTInfoById(int tab_seed);

	List getSteelSCTInfoListWithGroupQuery(String queryStr, PageInfo pageInfo,
			StandardQuery query);

	List getSteelSCTColumnNameCombo();

	List getHorizontalShiftList();

	int doSaveHorizontalShiftInfo(HorizontalShift info);

	void doDeleteHorizontalShiftInfoById(int tab_seed);

	List getHorizontalShiftInfoListWithGroupQuery(String queryStr,
			PageInfo pageInfo, StandardQuery query);

	List getHorizontalShiftColumnNameCombo();

	BigDecimal getStart_formula(String hori_code);

	List getRiverCodeList();

	List getPlaceCodeList();

	int doSaveOphContentInfo(OphContent info);

	void doDeleteOphContentInfoById(int tab_seed);

	List getOphContentInfoListWithGroupQuery(String queryStr,
			PageInfo pageInfo, StandardQuery query);

	List getOphContentColumnNameCombo();

	List getConcSeamCodeList();

	List getConcMpCodeList(String concSeamCode);

	int doSaveConcSeamInfo(ConcSeam info);

	void doDeleteConcSeamInfoById(int tab_seed);

	List getConcSeamInfoListWithGroupQuery(String queryStr, PageInfo pageInfo,
			StandardQuery query);

	List getConcSeamColumnNameCombo();

	List getHightDegree(String last_date, String the_date, String sp_code);

	List getFlexSeam(String start_date, String end_date, String fs_code);

	List getMaxAndMinFlexSeam(String start_date, String end_date, String fs_code);

	List getCeYaStatistics(String start_date, String end_date, String mptp_code,
			String mpt_name);

	List getMaxAndMinCeYa(String start_date, String end_date, String mptp_code,
			String mpt_name);

	List getLLValue(String start_date, String end_date, String type);

	List getWaterLevel(String start_date, String end_date);

	List getLiuLiang(String gate_open_num_1, String gate_open_degree_1,
			String gate_open_num_2, String gate_open_degree_2, String ap,
			String bp,String elec_machine);

	List getSWJWaterLevel(String start_date, String end_date);

}
