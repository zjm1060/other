 package com.bitc.jk.xmis.gczl.dao.impl;

import java.math.BigDecimal;
import java.util.List;
import java.util.Map;

import org.springframework.jdbc.core.JdbcTemplate;

import com.bitc.jk.xmis.applymanage.model.CeyaInfo;
import com.bitc.jk.xmis.dao.impl.BaseDAOImpl;
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
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

	public class GczlDaoImpl extends BaseDAOImpl implements GczlDao {

		private JdbcTemplate jt;

		public JdbcTemplate getJt() {
			return jt;
		}

		public void setJt(JdbcTemplate jt) {
			this.jt = jt;
		}

		@Override
		public List getFsInfoListWithGroupQuery(String queryStr,
				PageInfo pageInfo, StandardQuery query) {

			String sql = SQLScript.getFsInfoListWithGroupQuery;
			SQLGenerator generator = null;
			if(queryStr!=null && !("").equals(queryStr)){
				 generator = new SQLGenerator(sql+" select * into #result  from #temp where "+queryStr);
			}else{
				generator = new SQLGenerator(sql+" select * into #result  from #temp    " );
				
			}
			if(query!=null){
				query.addToSQLGenerator(generator);
			}
			
			String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0)) ? new Object[] {} : new Object[] {
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sqlStr, args);
		
		}

		@Override
		public List getFsCodeList() {
			String sql = SQLScript.getFsCodeList;
			return jt.queryForList(sql);
		}

		@Override
		public int doSavefsInfo(FlexSeam info) {
			if (info.getTab_seed() != 0) {
				ht.update(info);
			} else {
				int maxID = jt.queryForInt(SQLScript.getMaxIDFromFsInfo) + 1;
				info.setTab_seed(maxID);
				String unit_code = (String) jt.queryForMap(SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
				info.setUnit_code(unit_code);
				ht.save(info);
			}
			return info.getTab_seed();
		}

		@Override
		public void doDeletefsInfoById(int tab_seed) {
			String sql = SQLScript.doDeleteFsInfoById;
			jt.update(sql, new Object[] { tab_seed });
		}

		@Override
		public List getFsColumnNameCombo() {
			String sql = SQLScript.getFsColumnNameCombo;
			return jt.queryForList(sql);
		}

		@Override
		public List getSpCodeList() {
			String sql = SQLScript.getSpCodeList;
			return jt.queryForList(sql);
		}

		@Override
		public List getS_point_codeList(String sp_code) {
			String sql = SQLScript.getS_point_codeList;
			return jt.queryForList(sql,new Object[]{sp_code});
		}

		@Override
		public int doSaveSinkInfo(Sink info) {
			if (info.getTab_seed() != 0) {
				List list = jt.queryForList(SQLScript.getS_point_nameFromSinkInfo,new Object[] {info.getS_point_code(),info.getSp_code() });
				String s_point_name = "";
				if (list != null && list.size() > 0) {
					Map map = (Map) list.get(0);
					s_point_name =  (String)map.get("s_point_name");
				}
				info.setS_point_name(s_point_name);
				ht.update(info);
			} else {
				int maxID = jt.queryForInt(SQLScript.getMaxIDFromSinkInfo) + 1;
				List list = jt.queryForList(SQLScript.getS_point_nameFromSinkInfo,new Object[] { info.getS_point_code(),info.getSp_code() });
				String s_point_name = "";
				if (list != null && list.size() > 0) {
					Map map = (Map) list.get(0);
					s_point_name =  (String)map.get("s_point_name");
				}
				String unit_code = (String) jt.queryForMap(SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
				info.setUnit_code(unit_code);
				info.setTab_seed(maxID);
				info.setS_point_name(s_point_name);
				ht.save(info);
			}
			return info.getTab_seed();
		}

		@Override
		public void doDeleteSinkInfoById(int tab_seed) {
			String sql = SQLScript.doDeleteSinkInfoById;
			jt.update(sql, new Object[] { tab_seed });
		}

		@Override
		public List getSinkInfoListWithGroupQuery(String queryStr,
				PageInfo pageInfo, StandardQuery query) {
			String sql = SQLScript.getSinkInfoListWithGroupQuery;
			SQLGenerator generator = null;
			if(queryStr!=null && !("").equals(queryStr)){
				 generator = new SQLGenerator(sql+" select * into #result  from #temp where "+queryStr);
			}else{
				generator = new SQLGenerator(sql+" select * into #result  from #temp    " );
				
			}
			if(query!=null){
				query.addToSQLGenerator(generator);
			}
			
			String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0)) ? new Object[] {} : new Object[] {
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sqlStr, args);
		}

		@Override
		public List getSinkColumnNameCombo() {
			String sql = SQLScript.getSinkColumnNameCombo;
			return jt.queryForList(sql);
		}

		@Override
		public List getWeatherCodeList() {
			String sql = SQLScript.getWeatherCodeList;
			return jt.queryForList(sql);
		}

		@Override
		public int doSaveWeatherInfo(weather info) {
			if (info.getTab_seed() != 0) {
				
					String sql = "select weather_code from [bbz].[xmis].[gczl_weather_state_dict] where weather_state = ?";
					List list = jt.queryForList(sql,new Object[]{info.getSky_status()});
					String str = "";
					if(list.size()>0){
						str = (String) ((Map)list.get(0)).get("weather_code");
						info.setSky_status(str);
				}
				ht.update(info);
			} else {
				int maxID = jt.queryForInt(SQLScript.getMaxIDFromWeatherInfo) + 1;
				info.setTab_seed(maxID);
				String unit_code = (String) jt.queryForMap(SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
				info.setUnit_code(unit_code);
				ht.save(info);
			}
			return info.getTab_seed();
		}

		@Override
		public void doDeleteWeatherInfoById(int tab_seed) {
			String sql = SQLScript.doDeleteWeatherInfoById;
			jt.update(sql, new Object[] { tab_seed });
			
		}

		@Override
		public List getWeatherInfoListWithGroupQuery(String queryStr,
				PageInfo pageInfo, StandardQuery query) {

			String sql = SQLScript.getWeatherInfoListWithGroupQuery;
			SQLGenerator generator = null;
			if(queryStr!=null && !("").equals(queryStr)){
				 generator = new SQLGenerator(sql+" select * into #result  from #temp where "+queryStr);
			}else{
				generator = new SQLGenerator(sql+" select * into #result  from #temp    " );
				
			}
			if(query!=null){
				query.addToSQLGenerator(generator);
			}
			
			String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0)) ? new Object[] {} : new Object[] {
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sqlStr, args);
		
		}

		@Override
		public List getWeatherColumnNameCombo() {
			String sql = SQLScript.getWeatherColumnNameCombo;
			return jt.queryForList(sql);
		}

		@Override
		public List getRiverDmList() {
			String sql = SQLScript.getRiverDmList;
			return jt.queryForList(sql);
		}

		@Override
		public int doSaveRiverDmInfo(riverDm info) {
			if (info.getTab_seed() != 0) {
				ht.update(info);
			} else {
				int maxID = jt.queryForInt(SQLScript.getMaxIDFromRiverDmInfo) + 1;
				info.setTab_seed(maxID);
				String unit_code = (String) jt.queryForMap(SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
				info.setUnit_code(unit_code);
				ht.save(info);
			}
			return info.getTab_seed();
		}

		@Override
		public void doDeleteRiverDmInfoById(int tab_seed) {
			String sql = SQLScript.doDeleteRiverDmInfoById;
			jt.update(sql, new Object[] { tab_seed });
			
		}

		@Override
		public List getRiverDmInfoListWithGroupQuery(String queryStr,
				PageInfo pageInfo, StandardQuery query) {

			String sql = SQLScript.getRiverDmInfoListWithGroupQuery;
			SQLGenerator generator = null;
			if(queryStr!=null && !("").equals(queryStr)){
				 generator = new SQLGenerator(sql+" select * into #result  from #temp where "+queryStr);
			}else{
				generator = new SQLGenerator(sql+" select * into #result  from #temp    " );
				
			}
			if(query!=null){
				query.addToSQLGenerator(generator);
			}
			
			String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0)) ? new Object[] {} : new Object[] {
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sqlStr, args);
		
		}

		@Override
		public List getRiverDmColumnNameCombo() {
			String sql = SQLScript.getRiverDmColumnNameCombo;
			return jt.queryForList(sql);
		}

		@Override
		public List getConcreteCarbList() {
			String sql = SQLScript.getConcreteCarbList;
			return jt.queryForList(sql);
		}

		@Override
		public int doSaveConcreteCarbInfo(ConcreteCarb info) {
			if (info.getTab_seed() != 0) {
				ht.update(info);
			} else {
				int maxID = jt.queryForInt(SQLScript.getMaxIDFromConcreteCarbInfo) + 1;
				info.setTab_seed(maxID);
				String unit_code = (String) jt.queryForMap(SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
				info.setUnit_code(unit_code);
				ht.save(info);
			}
			return info.getTab_seed();
		}

		@Override
		public void doDeleteConcreteCarbInfoById(int tab_seed) {
			String sql = SQLScript.doDeleteConcreteCarbInfoById;
			jt.update(sql, new Object[] { tab_seed });
		}

		@Override
		public List getConcreteCarbInfoListWithGroupQuery(String queryStr,
				PageInfo pageInfo, StandardQuery query) {

			String sql = SQLScript.getConcreteCarbInfoListWithGroupQuery;
			SQLGenerator generator = null;
			if(queryStr!=null && !("").equals(queryStr)){
				 generator = new SQLGenerator(sql+" select * into #result  from #temp where "+queryStr);
			}else{
				generator = new SQLGenerator(sql+" select * into #result  from #temp    " );
				
			}
			if(query!=null){
				query.addToSQLGenerator(generator);
			}
			
			String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0)) ? new Object[] {} : new Object[] {
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sqlStr, args);
		
		}

		@Override
		public List getConcreteCarbColumnNameCombo() {
			String sql = SQLScript.getConcreteCarbColumnNameCombo;
			return jt.queryForList(sql);
		}

		@Override
		public List getSteelSCTList() {
			String sql = SQLScript.getSteelSCTList;
			return jt.queryForList(sql);
		}

		@Override
		public int doSaveSteelSCTInfo(SteelSCT info) {
			if (info.getTab_seed() != 0) {
				ht.update(info);
			} else {
				int maxID = jt.queryForInt(SQLScript.getMaxIDFromSteelSCTInfo) + 1;
				info.setTab_seed(maxID);
				String unit_code = (String) jt.queryForMap(SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
				info.setUnit_code(unit_code);
				ht.save(info);
			}
			return info.getTab_seed();
		}

		@Override
		public void doDeleteSteelSCTInfoById(int tab_seed) {
			String sql = SQLScript.doDeleteSteelSCTInfoById;
			jt.update(sql, new Object[] { tab_seed });
		}

		@Override
		public List getSteelSCTInfoListWithGroupQuery(String queryStr,
				PageInfo pageInfo, StandardQuery query) {

			String sql = SQLScript.getSteelSCTInfoListWithGroupQuery;
			SQLGenerator generator = null;
			if(queryStr!=null && !("").equals(queryStr)){
				 generator = new SQLGenerator(sql+" select * into #result  from #temp where "+queryStr);
			}else{
				generator = new SQLGenerator(sql+" select * into #result  from #temp    " );
				
			}
			if(query!=null){
				query.addToSQLGenerator(generator);
			}
			
			String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0)) ? new Object[] {} : new Object[] {
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sqlStr, args);
		
		}

		@Override
		public List getSteelSCTColumnNameCombo() {
			String sql = SQLScript.getSteelSCTColumnNameCombo;
			return jt.queryForList(sql);
		}

		@Override
		public List getHorizontalShiftList() {
			String sql = SQLScript.getHorizontalShiftList;
			return jt.queryForList(sql);
		}

		@Override
		public int doSaveHorizontalShiftInfo(HorizontalShift info) {
			if (info.getTab_seed() != 0) {
				ht.update(info);
			} else {
				int maxID = jt.queryForInt(SQLScript.getMaxIDFromHorizontalShiftInfo) + 1;
				info.setTab_seed(maxID);
				String unit_code = (String) jt.queryForMap(SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
				info.setUnit_code(unit_code);
				ht.save(info);
			}
			return info.getTab_seed();
		}

		@Override
		public void doDeleteHorizontalShiftInfoById(int tab_seed) {
			String sql = SQLScript.doDeleteHorizontalShiftInfoById;
			jt.update(sql, new Object[] { tab_seed });
		}

		@Override
		public List getHorizontalShiftInfoListWithGroupQuery(String queryStr,
				PageInfo pageInfo, StandardQuery query) {

			String sql = SQLScript.getHorizontalShiftInfoListWithGroupQuery;
			SQLGenerator generator = null;
			if(queryStr!=null && !("").equals(queryStr)){
				 generator = new SQLGenerator(sql+" select * into #result  from #temp where "+queryStr);
			}else{
				generator = new SQLGenerator(sql+" select * into #result  from #temp    " );
				
			}
			if(query!=null){
				query.addToSQLGenerator(generator);
			}
			
			String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0)) ? new Object[] {} : new Object[] {
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sqlStr, args);
		
		}

		@Override
		public List getHorizontalShiftColumnNameCombo() {
			String sql = SQLScript.getHorizontalShiftColumnNameCombo;
			return jt.queryForList(sql);
		}

		@Override
		public BigDecimal getStart_formula(String hori_code) {
			String sql ="select start_formula from [bbz].[xmis].[gczl_horizon_place_dict] where hori_code=?";
			List list = jt.queryForList(sql, new Object[] { hori_code });
			Map map = (Map)list.get(0);
		    //String a = 	(String) map.get("start_formula");
			BigDecimal value =(BigDecimal) map.get("start_formula");
		    return value;
		}

		@Override
		public List getRiverCodeList() {
			String sql = SQLScript.getRiverCodeList;
			return jt.queryForList(sql);
		}

		@Override
		public List getPlaceCodeList() {
			String sql = SQLScript.getPlaceCodeList;
			return jt.queryForList(sql);
		}

		@Override
		public int doSaveOphContentInfo(OphContent info) {
			if (info.getTab_seed() != 0) {
				ht.update(info);
			} else {
				int maxID = jt.queryForInt(SQLScript.getMaxIDFromOphContentInfo) + 1;
				info.setTab_seed(maxID);
				String unit_code = (String) jt.queryForMap(SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
				info.setUnit_code(unit_code);
				ht.save(info);
			}
			return info.getTab_seed();
		}

		@Override
		public void doDeleteOphContentInfoById(int tab_seed) {
			String sql = SQLScript.doDeleteOphContentInfoById;
			jt.update(sql, new Object[] { tab_seed });
		}

		@Override
		public List getOphContentInfoListWithGroupQuery(String queryStr,
				PageInfo pageInfo, StandardQuery query) {

			String sql = SQLScript.getOphContentInfoListWithGroupQuery;
			SQLGenerator generator = null;
			if(queryStr!=null && !("").equals(queryStr)){
				 generator = new SQLGenerator(sql+" select * into #result  from #temp where "+queryStr);
			}else{
				generator = new SQLGenerator(sql+" select * into #result  from #temp    " );
				
			}
			if(query!=null){
				query.addToSQLGenerator(generator);
			}
			
			String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0)) ? new Object[] {} : new Object[] {
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sqlStr, args);
		
		}

		@Override
		public List getOphContentColumnNameCombo() {
			String sql = SQLScript.getOphContentColumnNameCombo;
			return jt.queryForList(sql);
		}

		@Override
		public List getConcSeamCodeList() {
			String sql = SQLScript.getConcSeamCodeList;
			return jt.queryForList(sql);
		}

		@Override
		public List getConcMpCodeList(String concSeamCode) {
			String sql = SQLScript.getConcMpCodeList;
			return jt.queryForList(sql,new Object[]{concSeamCode});
		}

		@Override
		public int doSaveConcSeamInfo(ConcSeam info) {
			if (info.getTab_seed() != 0) {
				ht.update(info);
			} else {
				int maxID = jt.queryForInt(SQLScript.getMaxIDFromConcSteamInfo) + 1;
				info.setTab_seed(maxID);
				String unit_code = (String) jt.queryForMap(SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
				info.setUnit_code(unit_code);
				ht.save(info);
			}
			return info.getTab_seed();
		}

		@Override
		public void doDeleteConcSeamInfoById(int tab_seed) {
			String sql = SQLScript.doDeleteConcSeamInfoById;
			jt.update(sql, new Object[] { tab_seed });
		}

		@Override
		public List getConcSeamInfoListWithGroupQuery(String queryStr,
				PageInfo pageInfo, StandardQuery query) {

			String sql = SQLScript.getConcSeamInfoListWithGroupQuery;
			SQLGenerator generator = null;
			if(queryStr!=null && !("").equals(queryStr)){
				 generator = new SQLGenerator(sql+" select * into #result  from #temp where "+queryStr);
			}else{
				generator = new SQLGenerator(sql+" select * into #result  from #temp    " );
				
			}
			if(query!=null){
				query.addToSQLGenerator(generator);
			}
			
			String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0)) ? new Object[] {} : new Object[] {
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sqlStr, args);
		
		}

		@Override
		public List getConcSeamColumnNameCombo() {
			String sql = SQLScript.getConcSeamColumnNameCombo;
			return jt.queryForList(sql);
		}

		@Override
		public List getHightDegree(String last_date, String the_date,
				String sp_code) {
			String last = last_date.substring(0,10);
			String the = the_date.substring(0,10);
			String sql = SQLScript.getHightDegree;
			return jt.queryForList(sql,new Object[]{last,the,sp_code,last,the});
		}

		@Override
		public List getFlexSeam(String start_date, String end_date,
				String fs_code) {
			String end = end_date.substring(0,10)+" 23:59:59";
			String sql = SQLScript.getFlexSeam;
			 List result = jt.queryForList(sql,new Object[]{start_date,end,fs_code});
		 return result;
		}

		@Override
		public List getMaxAndMinFlexSeam(String start_date, String end_date,
				String fs_code) {
			String end = end_date.substring(0,10)+" 23:59:59";
			String getMin = SQLScript.getMaxAndMinFlexSeam;
			 return jt.queryForList(getMin,new Object[]{start_date,end,fs_code});
		}

		@Override
		public List getCeYaStatistics(String start_date, String end_date,
				String mptp_code, String mpt_name) {
			String end = end_date.substring(0,10)+" 23:59:59";;
			String sql = SQLScript.getCeYaStatistics;
			return jt.queryForList(sql,new Object[]{start_date,end,mptp_code,mpt_name});
		}

		@Override
		public List getMaxAndMinCeYa(String start_date, String end_date,
				String mptp_code, String mpt_name) {
			String end = end_date.substring(0,10)+" 23:59:59";
			String getMin = SQLScript.getMaxAndMinCeYa;
			 return jt.queryForList(getMin,new Object[]{start_date,end,mpt_name,mptp_code});
		
		}
		@Override
		public List getLLValue(String start_date, String end_date,
				String type) {
			String sql = "{call xmis.sp_getLiulangTotalValue (?,?,?)}";
			 List result = jt.queryForList(sql,new Object[]{start_date,end_date,type});
		 return result;
		}

		@Override
		public List getWaterLevel(String start_date, String end_date) {
			String end = end_date.substring(0,10)+" 23:59:59";;
			String sql = SQLScript.getWaterLevel;
			return jt.queryForList(sql,new Object[]{start_date,end});
		}

		@Override
		public List getLiuLiang(String gate_open_num_1,
				String gate_open_degree_1, String gate_open_num_2,
				String gate_open_degree_2, String ap, String bp,String elec_machine) {
			String sql = "{call xmis.sp_getLiulangValue (?,?,?,?,?,?,?)}";
			 List result = jt.queryForList(sql,new Object[]{ap,bp,gate_open_degree_2,gate_open_num_2,gate_open_degree_1,gate_open_num_1,elec_machine});
		 return result;
		}

		@Override
		public List getSWJWaterLevel(String start_date, String end_date) {
			String sql = SQLScript.getSWJWaterLevel;
			return jt.queryForList(sql,new Object[]{start_date,end_date});
		}
}
