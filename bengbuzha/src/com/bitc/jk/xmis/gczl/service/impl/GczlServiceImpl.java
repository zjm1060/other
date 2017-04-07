package com.bitc.jk.xmis.gczl.service.impl;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

import com.bitc.jk.xmis.applymanage.model.CeyaInfo;
import com.bitc.jk.xmis.gczl.dao.GczlDao;
import com.bitc.jk.xmis.gczl.model.ConcSeam;
import com.bitc.jk.xmis.gczl.model.ConcreteCarb;
import com.bitc.jk.xmis.gczl.model.FlexSeam;
import com.bitc.jk.xmis.gczl.model.HorizontalShift;
import com.bitc.jk.xmis.gczl.model.OphContent;
import com.bitc.jk.xmis.gczl.model.Sink;
import com.bitc.jk.xmis.gczl.model.SteelSCT;
import com.bitc.jk.xmis.gczl.model.riverDm;
import com.bitc.jk.xmis.gczl.model.weather;
import com.bitc.jk.xmis.gczl.service.GczlService;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class GczlServiceImpl implements GczlService{

	private GczlDao gczlDao;

	public GczlDao getGczlDao() {
		return gczlDao;
	}

	public void setGczlDao(GczlDao gczlDao) {
		this.gczlDao = gczlDao;
	}

	@Override
	public String getFsInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query) {
		List list = gczlDao.getFsInfoListWithGroupQuery(
				queryStr, pageInfo, query);
		return WebUtil.buildStoreForJson(list, "");
	}

	@Override
	public String getFsCodeList() {
		List list = gczlDao.getFsCodeList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public int doSavefsInfo(FlexSeam info) {
		return gczlDao.doSavefsInfo(info);
	}

	@Override
	public void doDeletefsInfoById(int tab_seed) {
		gczlDao.doDeletefsInfoById(tab_seed);
		
	}

	@Override
	public String getFsColumnNameCombo() {
		List list = gczlDao.getFsColumnNameCombo();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getSpCodeList() {
		List list = gczlDao.getSpCodeList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getS_point_codeList(String sp_code) {
		List list = gczlDao.getS_point_codeList(sp_code);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public int doSaveSinkInfo(Sink info) {
		return gczlDao.doSaveSinkInfo(info);
	}

	@Override
	public void doDeleteSinkInfoById(int tab_seed) {
		gczlDao.doDeleteSinkInfoById(tab_seed);
	}

	@Override
	public String getSinkInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query) {
		List list = gczlDao.getSinkInfoListWithGroupQuery(
				queryStr, pageInfo, query);
		return WebUtil.buildStoreForJson(list, "");
	}

	@Override
	public String getSinkColumnNameCombo() {
		List list = gczlDao.getSinkColumnNameCombo();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getWeatherCodeList() {
		List list = gczlDao.getWeatherCodeList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public int doSaveWeatherInfo(weather info) {
		return gczlDao.doSaveWeatherInfo(info);
	}

	@Override
	public void doDeleteWeatherInfoById(int tab_seed) {
		gczlDao.doDeleteWeatherInfoById(tab_seed);
		
	}

	@Override
	public String getWeatherInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query) {
		List list = gczlDao.getWeatherInfoListWithGroupQuery(
				queryStr, pageInfo, query);
		return WebUtil.buildStoreForJson(list, "");
	}

	@Override
	public String getWeatherColumnNameCombo() {
		List list = gczlDao.getWeatherColumnNameCombo();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getRiverDmList() {
		List list = gczlDao.getRiverDmList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public int doSaveRiverDmInfo(riverDm info) {
		return gczlDao.doSaveRiverDmInfo(info);
	}

	@Override
	public void doDeleteRiverDmInfoById(int tab_seed) {
		gczlDao.doDeleteRiverDmInfoById(tab_seed);
		
	}

	@Override
	public String getRiverDmInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query) {
		List list = gczlDao.getRiverDmInfoListWithGroupQuery(
				queryStr, pageInfo, query);
		return WebUtil.buildStoreForJson(list, "");
	}

	@Override
	public String getRiverDmColumnNameCombo() {
		List list = gczlDao.getRiverDmColumnNameCombo();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getConcreteCarbList() {
		List list = gczlDao.getConcreteCarbList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public int doSaveConcreteCarbInfo(ConcreteCarb info) {
		return gczlDao.doSaveConcreteCarbInfo(info);
	}

	@Override
	public void doDeleteConcreteCarbInfoById(int tab_seed) {
		gczlDao.doDeleteConcreteCarbInfoById(tab_seed);
		
	}

	@Override
	public String getConcreteCarbInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query) {
		List list = gczlDao.getConcreteCarbInfoListWithGroupQuery(
				queryStr, pageInfo, query);
		return WebUtil.buildStoreForJson(list, "");
	}

	@Override
	public String getConcreteCarbColumnNameCombo() {
		List list = gczlDao.getConcreteCarbColumnNameCombo();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getSteelSCTList() {
		List list = gczlDao.getSteelSCTList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public int doSaveSteelSCTInfo(SteelSCT info) {
		return gczlDao.doSaveSteelSCTInfo(info);
	}

	@Override
	public void doDeleteSteelSCTInfoById(int tab_seed) {
		gczlDao.doDeleteSteelSCTInfoById(tab_seed);
		
	}

	@Override
	public String getSteelSCTInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query) {
		List list = gczlDao.getSteelSCTInfoListWithGroupQuery(
				queryStr, pageInfo, query);
		return WebUtil.buildStoreForJson(list, "");
	}

	@Override
	public String getSteelSCTColumnNameCombo() {
		List list = gczlDao.getSteelSCTColumnNameCombo();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getHorizontalShiftList() {
		List list = gczlDao.getHorizontalShiftList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public int doSaveHorizontalShiftInfo(HorizontalShift info) {
		return gczlDao.doSaveHorizontalShiftInfo(info);
	}

	@Override
	public void doDeleteHorizontalShiftInfoById(int tab_seed) {
		gczlDao.doDeleteHorizontalShiftInfoById(tab_seed);
		
	}

	@Override
	public String getHorizontalShiftInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query) {
		List list = gczlDao.getHorizontalShiftInfoListWithGroupQuery(
				queryStr, pageInfo, query);
		return WebUtil.buildStoreForJson(list, "");
	}

	@Override
	public String getHorizontalShiftColumnNameCombo() {
		List list = gczlDao.getHorizontalShiftColumnNameCombo();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public BigDecimal getStart_formula(String hori_code) {
		return gczlDao.getStart_formula(hori_code);
	}

	@Override
	public String getRiverCodeList() {
		List list = gczlDao.getRiverCodeList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getPlaceCodeList() {
		List list = gczlDao.getPlaceCodeList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public int doSaveOphContentInfo(OphContent info) {
		return gczlDao.doSaveOphContentInfo(info);
	}

	@Override
	public void doDeleteOphContentInfoById(int tab_seed) {
		gczlDao.doDeleteOphContentInfoById(tab_seed);
		
	}

	@Override
	public String getOphContentInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query) {
		List list = gczlDao.getOphContentInfoListWithGroupQuery(
				queryStr, pageInfo, query);
		return WebUtil.buildStoreForJson(list, "");
	}

	@Override
	public String getOphContentColumnNameCombo() {
		List list = gczlDao.getOphContentColumnNameCombo();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getConcSeamCodeList() {
		List list = gczlDao.getConcSeamCodeList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public String getConcMpCodeList(String concSeamCode) {
		List list = gczlDao.getConcMpCodeList(concSeamCode);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	@Override
	public int doSaveConcSeamInfo(ConcSeam info) {
		return gczlDao.doSaveConcSeamInfo(info);
	}

	@Override
	public void doDeleteConcSeamInfoById(int tab_seed) {
		// TODO Auto-generated method stub
		gczlDao.doDeleteConcSeamInfoById(tab_seed);
	}

	@Override
	public String getConcSeamInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query) {
		List list = gczlDao.getConcSeamInfoListWithGroupQuery(
				queryStr, pageInfo, query);
		return WebUtil.buildStoreForJson(list, "");
	}

	@Override
	public String getConcSeamColumnNameCombo() {
		List list = gczlDao.getConcSeamColumnNameCombo();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	public List getListWithPageInfo(List list, PageInfo pageInfo) {
		List resultList = new ArrayList();
		int limit = pageInfo.getLimit();
		int start = pageInfo.getStart();
		int size = list.size();
		int limitSize = start + limit;
		if (limitSize > size) {
			limitSize = size;
		}
		if (limit == 0 || list.size() < limit) {
			return list;
		} else {
			for (int i = start; i < limitSize; i++) {
				resultList.add(list.get(i));
			}
			return resultList;
		}

	}

	@Override
	public String getHightDegree(PageInfo pageInfo, String last_date, String the_date,
			String sp_code) {
		List list = gczlDao.getHightDegree(last_date,the_date,sp_code);

		List aList = getListWithPageInfo(list, pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
				list.size());
	}

	@Override
	public String getFlexSeam(PageInfo pageInfo, String start_date,
			String end_date, String fs_code) {
		List list = gczlDao.getFlexSeam(start_date,end_date,fs_code);

		List aList = getListWithPageInfo(list, pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
				list.size());
	}


	@Override
	public String getMaxAndMinFlexSeam(String start_date, String end_date,
			String fs_code) {
		List list =  gczlDao.getMaxAndMinFlexSeam(start_date,end_date,fs_code);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(list,
				list.size());
	}

	@Override
	public String getCeYaStatistics(PageInfo pageInfo, String start_date,
			String end_date, String mptp_code, String mpt_name) {
		List list = gczlDao.getCeYaStatistics(start_date,end_date,mptp_code,mpt_name);

		List aList = getListWithPageInfo(list, pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
				list.size());
	}

	public String getMaxAndMinCeYa(String start_date, String end_date,
			String mptp_code, String mpt_name) {
		List list =  gczlDao.getMaxAndMinCeYa(start_date,end_date,mptp_code,mpt_name);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(list,
				list.size());
	}
	public String getLLValue( PageInfo pageInfo,String start_date,
			String end_date, String type) {
		List list = gczlDao.getLLValue(start_date,end_date,type);
	
		List aList = getListWithPageInfo(list, pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
				list.size());
	}

	@Override
	public String getWaterLevel(String start_date, String end_date) {
		List list =  gczlDao.getWaterLevel(start_date,end_date);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	@Override
	public List getLiuLiang(String gate_open_num_1, String gate_open_degree_1,
			String gate_open_num_2, String gate_open_degree_2, String ap,
			String bp,String elec_machine) {
		return gczlDao.getLiuLiang(gate_open_num_1,gate_open_degree_1,gate_open_num_2,gate_open_degree_2,ap,bp,elec_machine);
	}

	@Override
	public String getSWJWaterLevel(String start_date,
			String end_date) {
		// TODO Auto-generated method stub
		return WebUtil.buildStoreForJsonWithoutPageInfo(gczlDao.getSWJWaterLevel(start_date,end_date),"");
	}
}
