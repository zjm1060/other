package com.bitc.jk.xmis.dao.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.List;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.log4j.Logger;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementSetter;
import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.orm.hibernate3.HibernateCallback;

import com.bitc.jk.xmis.dao.DeviceDAO;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.device.DeviceInObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.device.DeviceInfo;
import com.bitc.jk.xmis.model.device.PurchaseRequisition;
import com.bitc.jk.xmis.model.device.PurchaseRequisitionDetail;
import com.bitc.jk.xmis.model.device.PurchasingDetail;
import com.bitc.jk.xmis.model.device.PurchasingList;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.Power;
import com.bitc.jk.xmis.model.sys.Weight;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class DeviceDAOImpl extends BaseDAOImpl implements DeviceDAO {
	
	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate ajdbcTemplate) {
		this.jt = ajdbcTemplate;
	}

	/**
	 * 查询物资信息库
	 * */
	public List<DeviceInfo> getDeviceInfoListByQuery(StandardQuery query,
			PageInfo pageInfo) {
		
		return getListStore(query,pageInfo, DeviceInfo.class);
	}

	public int getDeviceInfoCountByQuery(StandardQuery query, PageInfo pageInfo) {
		return getTotalCount(query, DeviceInfo.class);
	}

	public void savePurchaseRequisition(PurchaseRequisition requisition) {
		saveOrUpdateObject(requisition);
	}

	public PurchaseRequisition getPurchaseRequisition(String requestId) {
		return load(PurchaseRequisition.class, requestId);
	}

	public List<PurchaseRequisition> getPurchaseRequisitionByQuery(
			StandardQuery query, PageInfo info) {
		return getListStore(query, info, PurchaseRequisition.class);
	}

	@SuppressWarnings("unchecked")
	public void saveDeviceInfo(DeviceObj device) {
		List<DeviceInfo> list = ht.find(
				"from DeviceInfo d where d.name=? and d.modelType=?",
				new Object[] { device.getName(), device.getModelType() });
		if (list.size() == 0) {
			DeviceInfo info = new DeviceInfo();
			try {
				BeanUtils.copyProperties(info, device);
				info.setId(null);
				saveDeviceInfo(info);
			} catch (Exception e) {
				Logger.getLogger(this.getClass()).error("自动新増设备至项目信息库出错。", e);
			}
		}
	}

	public void saveDeviceInfo(DeviceInfo info) {
		saveOrUpdateObject(info);
	}

	public List<PurchaseRequisitionDetail> getTobePurchaseList(
			StandardQuery query, PageInfo info) {
		return getListStore(query, info, PurchaseRequisitionDetail.class);
	}

	public int getTobePurchaseListCount(StandardQuery query) {
		return getTotalCount(query, PurchaseRequisitionDetail.class);
	}

	public List<PurchasingList> getPurchasingListList(StandardQuery query,
			PageInfo pageInfo) {
		return getListStore(query, pageInfo, PurchasingList.class);
	}

	public int getPurchasingListListCount(StandardQuery query) {
		return getTotalCount(query, PurchasingList.class);
	}

	public PurchasingList getPurchasingList(String purchasingId) {
		return load(PurchasingList.class, purchasingId);
	}

	public PurchaseRequisitionDetail getPurchaseRequisitionDetail(
			String requisitionDetailId) {
		return load(PurchaseRequisitionDetail.class, requisitionDetailId);
	}

	public void savePurchasingList(PurchasingList purchasingList) {
		saveOrUpdateObject(purchasingList);
	}

	public void savePurchaseRequisitionDetail(PurchaseRequisitionDetail detail) {
		saveOrUpdateObject(detail);
	}
	public List getRecordpurdelID(final String purdelID) {
		// TODO Auto-generated method stub
		final String hql ="select " +
								"new map(purdel.contractID.contractID as contractID," +
								"purdel.contractID.contractNo as contractNo," +
								"purdel.contractID.contractNo as contranonono," +
								"purdel.contractID.contractName as contractName," +
								"purdel.number as number," +
								"(purdel.number*purdel.univalent) as numberun," +
								"purdel.univalent as univalent," +
								"purdel.deviceInfo.materiaName as deviceName," +
								"purdel.deviceInfo.id as deviceID," +
								"purdel.expectArrivalDate as expectArrivalDate," +
								"purdel.deviceInfo.modelType as modelType," +
								"purdel.deviceInfo.standard as standard ," +
								"purdel.deviceInfo.weight as weight," +
								"purdel.deviceInfo.referencePrice as referencePrice," +
								"purdel.deviceInfo.manufacturer as manufacturer," +
								"purdel.deviceInfo.stopTime as stopTime," +
								"purdel.deviceInfo.newupdateDate as newupdateDate," +
								"purdel.deviceInfo.listingDate as listingDate," +
								"purdel.technicalRequirement as technicalRequirement," +
								"purdel.remark as remark," +
								"purdel.registrationDate as registrationDate," +
								"purdel.subscriber.userID as userID," +
								"purdel.subscriber.userName as userName," +
								"purdel.id as purdelID," +
								"purdel.purchasing.id as purchasing," +
								"purdel.lastModifier.userID as lastModifier," +
								"purdel.lastModifier.userName as lastModifierName," +
								"purdel.lastModified as lastModified," +
								"purdel.deviceInfo.currency.name as currencyName " +
								" ) from "
								+ " PurchasingDetail purdel "
								+ "where   purdel.id = ?";
		List list = ht.executeFind(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				Query query = session.createQuery(hql)
						.setString(0, purdelID);
				return query.list();
			}
		});
		return list;
	}
	
	public void doSaveDeviceIn(DeviceInObj deviceIn) {
		ht.save(deviceIn);
	}

	@SuppressWarnings("rawtypes")
	
	public List getWareHouseCombo(String fid) {
		return jt.queryForList(SQLScript.getWareHouseCombo,new Object[]{fid});
	}
	
	
	public List getShelfCombo(String fid) {
		return jt.queryForList(SQLScript.getShelfCombo,new Object[]{fid});
	}
	
	
	public List getPositionCombo(String fid) {
		return jt.queryForList(SQLScript.getPositionCombo,new Object[]{fid});
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	
	public List getPorjectDeviceInfoList(String projectId) {
		return jt.queryForList(SQLScript.getPorjectDeviceInfoList,new Object[]{projectId});
	}

	@SuppressWarnings("rawtypes")
	
	public List getDeviceInList(StandardQuery query, PageInfo pageInfo) {
		SQLGenerator generator = new SQLGenerator(SQLScript.getDeviceInList);
		if(query !=null){
			query.addToSQLGenerator(generator);
		}
		String sql = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0))?new Object[] {} : new Object[] {
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sql,args);
	}

	
	public void doAddWareHouseCombo(final String fid,final  String name, final String managerID) {
		jt.update(SQLScript.doAddWareHouse,new PreparedStatementSetter() {
			
			
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, UUIDUtils.nextCode());
				ps.setString(2, name);
				ps.setString(3, fid);
				ps.setInt(4, Type.GetInt(managerID));
				ps.setInt(5, WebUtil.getCurrentEmployee().getUserID());
			}
		});
		
	}

	
	public String doAddShelfCombo(final String fid,final  String desc, final String code) {
		final String uid = UUIDUtils.nextCode();
		int n = jt.update(SQLScript.doAddShelf,new PreparedStatementSetter() {
			
			
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, uid);
				ps.setString(2, desc);
				ps.setString(3, code);
				ps.setString(4,fid);
				ps.setInt(5, WebUtil.getCurrentEmployee().getUserID());
			}
		});
		if(n>0){
			return uid;
		} 
		return "";
		
	}

	
	public void doAddPositionCombo(final String fid,final  String desc, final String code) {
		jt.update(SQLScript.doAddPositon,new PreparedStatementSetter() {
			
			
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, UUIDUtils.nextCode());
				ps.setString(2, code);
				ps.setString(3, desc);
				ps.setString(4,fid);
				ps.setInt(5, WebUtil.getCurrentEmployee().getUserID());
			}
		});
		
	}

	@SuppressWarnings("rawtypes")
	
	public List getArrivalDeviceInfos(String contractId, String deviceInfoId) {
		return jt.queryForList(SQLScript.getArrivalDeviceInfos, new Object[]{contractId,deviceInfoId});
	}

	/*
	 * (non-Javadoc) 采购清单明细查询
	 * 
	 * @see
	 * com.bitc.jk.xmis.dao.DeviceDAO#getPurchasingListByOutContract(java.lang
	 * .Integer)
	 */
	@SuppressWarnings("unchecked")
	public List getPurchasingListByOutContract(final Integer contractId) {
		final String sql = "select sum(deva.单价 * deva.数量) sumAmt,con.合同编号,con.合同ID   " +
							"INTO #AAA  " +
							"FROM xmis.dev_采购清单明细    deva,xmis.con_合同   con   " +
							"where deva.支出合同ID=con.合同ID " +
							"GROUP BY con.合同编号,con.合同ID       " +
							
							" SELECT 物资ID,支出合同ID AS 支出合同ID,SUM(到货数量) AS 到货数量  " +  
							 " INTO #tmp2  " +   
							"  FROM xmis.dev_到货清单 " +     
							"  GROUP BY  物资ID,支出合同ID " + 


							" SELECT devc.ID AS 明细ID,devc.项目物资ID,devc.支出合同ID, #tmp2.到货数量 " +
							" into #tttt " +  
							"  FROM xmis.dev_采购清单明细 devc " + 
							"  LEFT JOIN #tmp2   " + 
							"  ON devc.支出合同ID = #tmp2.支出合同ID AND devc.项目物资ID = #tmp2.物资ID " + 
							"  GROUP BY devc.ID,devc.支出合同ID,#tmp2.到货数量,devc.项目物资ID     " +
							
							
							" select  " +
							"deva.单价*deva.数量 as numberun," +
							"con.合同总额 -sumAmt as difference," +
							"con.合同总额 as yyy," +
							"con.合同编号 +'-->合同剩余金额: '+  (str(con.合同总额 -sumAmt))+' 元' as rrrr," +
							"deva.支出合同ID as contractID," +
							"con.合同编号 as contractNo," +
							"con.合同编号 as contranonono," +
							"con.合同名称 as contractName," +
							"con.合同总额 as contractAmt," +
							"deva.单价 as univalent," +
							"deva.数量 as number," +
							"(deva.数量-#tttt.到货数量) AS 未到货," +
							"devc.物资名称 as deviceName, " +
							"devc.停产时间 as stopTime," +
							"devc.登记日期 as newupdateDate," +
							"devc.投入市场日期 as listingDate," +
							"devc.物资ID as deviceID," +
							"deva.ID as purdelID," +
							
							"#tttt.到货数量 AS 到货数量  " +
							"FROM    xmis.dev_采购清单明细  deva," +
							"xmis.con_合同 con ," +
							"xmis.dev_物资信息库 devc ," +
							"xmis.hr_人员 hr1 ,xmis.hr_人员 hr2 ,xmis.dev_采购清单 devb ,#AAA,#tttt " +
							"WHERE   deva.支出合同ID = con.合同ID " +
							"AND deva.项目物资ID = devc.物资ID " +
							"AND deva.登记人 = hr1.人员ID " +
							"AND deva.最后修改人ID = hr2.人员ID " +
							"AND deva.清单ID = devb.ID " +
							"AND con.合同ID = #AAA.合同ID " +
							"AND deva.ID = #tttt.明细ID   " +
							"AND devb.支出合同ID = ? " +
							//"order by deva.登记日期  desc "; 
							"ORDER BY rrrr,deva.登记日期  desc";
		return   jt.queryForList(sql, new Object[] { contractId });
	}


	@SuppressWarnings("unchecked")
	public List getRelationProjectID(final Integer contractId) {

		return null;
	}

	/**
	 * 添加采购清单
	 * */
	public void addMaterialsPurchasingList(PurchasingList purchasingList) {
		saveObject(purchasingList);
	}
	/**
	 * 
	 * 查询采购清单
	 * */
	public String getMaterialsPurchasingList(final Integer contractId) {
		final String hql = " select pch.id  from PurchasingList pch where pch.contractId = ?";
		List list = ht.executeFind(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				Query query = session.createQuery(hql)
						.setInteger(0, contractId);
				return query.list();
			}
		});
		if (list != null && list.size() > 0) {
			return (String) list.get(0);

		}
		return null;
	}

	/**
	 * 添加采购清单明细
	 * */

	public void addMaterialsPurchasingDetail(final String message_id,
			 final String s, final PurchasingDetail pl,
			final DeviceInfo dfo, final ContractObj cop,final SimpleEmployee see) {
		// TODO Auto-generated method stub
		String sql = "insert into xmis.dev_采购清单明细"
				+ " (ID,清单ID,项目物资ID,数量,技术要求,期望到货日期,备注,支出合同ID,单价,登记日期,登记人,最后修改人ID,最后修改日期) " + " values"
				+ "(?,?,?,?,?,?,?,?,?,GETDATE(),?,?,GETDATE())";
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, message_id);
				ps.setString(2, s);
				ps.setString(3, dfo.getId());
				ps.setInt(4, pl.getNumber());
				ps.setString(5, pl.getTechnicalRequirement());
				ps.setDate(6, (java.sql.Date) pl.getExpectArrivalDate());
				ps.setString(7, pl.getRemark());
				ps.setInt(8, cop.getContractID());
				ps.setFloat(9, pl.getUnivalent());
//				ps.setDate(10,(java.sql.Date) pl.getRegistrationDate());
				ps.setInt(10, see.getUserID());
				ps.setInt(11, see.getUserID());
//				ps.setDate(12, (java.sql.Date) pl.getLastModified());
			}
		});

	}
/**
 * 删除采购清单明细
 * */
	public void doDeletePurchasingDetail(String purdelID) {
		// TODO Auto-generated method stub
		String sql ="delete from xmis.dev_采购清单明细  where ID=?";
		jt.update(sql, new Object[] { purdelID });
	}
/**
 * 查询是否重复购买
 * */
	public List getPurchasingDetaildouble(String contractID, String materialID,Integer contractId) {
		String sql = "select A.支出合同ID, D.物资名称,E.合同总额   from xmis.dev_采购清单 A ,xmis.dev_采购清单明细 B,xmis.dev_物资信息库 D,xmis.con_合同 E  " +
				"where A.ID = B.清单ID " +
				"and E.合同ID = A.支出合同ID " +
				"and B.项目物资ID = D.物资ID " +
				"and B.支出合同ID=? " +
				"and D.物资ID=? " +
				"and E.合同ID=?";
		return   jt.queryForList(sql, new Object[] { contractID,materialID,contractId });
	}
	
public List getPurchasingDe(final String projectID, final Integer deviceID) {
	// TODO Auto-generated method stub
	final String hql ="from PurchasingDetail p where p.deviceInfo.id=? and p.projectObj.projectID=?";
	List list = ht.executeFind(new HibernateCallback() {
		public Object doInHibernate(Session session)
				throws HibernateException, SQLException {
			Query query = session.createQuery(hql)
					.setString(0, projectID).setInteger(1, deviceID);
			return query.list();
		}
	});
	return list;
}
/**
 * 查询选定项目已消费金额
 * */
public List getcontractSum(final String valueField) {
	// TODO Auto-generated method stub
	final String hql = "select new map(pd.number as bnumber,pd.univalent as bunivalent) from PurchasingDetail pd where pd.contractID.contractID = ?";
	List list = ht.executeFind(new HibernateCallback() {
		public Object doInHibernate(Session session)
				throws HibernateException, SQLException {
			Query query = session.createQuery(hql)
					.setString(0, valueField);
			return query.list();
		}
	});
	return list;
}
/**
 * 修改采购清单明细
 * */
public void doUpdatePurchasingDetail(final PurchasingDetail pl, final DeviceInfo dfo,
		final SimpleEmployee see) {
	// TODO Auto-generated method stub
	final String sql = "update xmis.dev_采购清单明细 set " +
						"项目物资ID = ?," +
						"数量 = ?," +
						"技术要求 =?," +
						"期望到货日期 = ?," +
						"备注 = ?," +
						"最后修改日期 = GETDATE()," +
						"最后修改人ID = ?," +
						"单价 = ? " +
						"where ID =? ";
	jt.update(sql, new PreparedStatementSetter() {
		public void setValues(PreparedStatement ps) throws SQLException {
			ps.setString(1, dfo.getId());
			ps.setInt(2, pl.getNumber());
			ps.setString(3, pl.getTechnicalRequirement());
			ps.setDate(4, (java.sql.Date) pl.getExpectArrivalDate());
			ps.setString(5, pl.getRemark());
//			ps.setDate(6, (java.sql.Date) pl.getLastModified());
			ps.setInt(6, see.getUserID());
			ps.setFloat(7, pl.getUnivalent());
			ps.setString(8, pl.getId());
		}
	});
}

/**
 * 查询项目单价数量
 * */
public List getPurchasingDetailTotal(final String contractID) {
	// TODO Auto-generated method stub
	final String hql ="select new map(pl.contractID.contractAmt as tcontractAmt,pl.number as tnumber,pl.univalent as tunivalent ) from PurchasingDetail pl where pl.contractID = ?";
	List list = ht.executeFind(new HibernateCallback() {
		public Object doInHibernate(Session session)
				throws HibernateException, SQLException {
			Query query = session.createQuery(hql)
					.setString(0, contractID);
			return query.list();
		}
	});
	return list;
}



public void doAddMaterialInformation(DeviceInfo dfo) {
	// TODO Auto-generated method stub
	saveObject(dfo);
}

public List getMaterialsInfomation(final String id) {
	// TODO Auto-generated method stub
	final String hql="select " +
				"new map(dfo.id as id," +
				"dfo.materiaName as materiaName," +
				"dfo.modelType as modelType," +
				"dfo.standard as standard," +
				"dfo.weight as weight," +
				"dfo.power as power," +
				"dfo.referencePrice as referencePrice," +
				"dfo.currency.id as currency," +
				"dfo.stopTime as stopTime," +
				"dfo.guarantees as guarantees," +
				"dfo.lastModified as lastModified," +
				"dfo.lastModifier.userID as lastModifier," +
				"dfo.lastModifier.userName as lastModifierName," +
				"dfo.manufacturer.companyID as manufacturer," +
				"dfo.manufacturer.companyName as manufacturerName," +
				"dfo.configuration as configuration," +
				"dfo.volume as volume," +
				"dfo.materialsBusiness as materialsBusiness," +
				"dfo.materialItself as materialItself," +
				"dfo.materialUse as materialUse," +
				"dfo.newestPrice as newestPrice," +
				"dfo.listingDate as listingDate," +
				"dfo.powerSuppliesUnit.id as powerSuppliesUnit," +
				"dfo.powerSuppliesUnit.name as powerSuppliesUnitName," +
				"dfo.materialWeight.id as materialWeight," +
				"dfo.materialWeight.name as materialWeightName ) from DeviceInfo dfo where dfo.id = ? ";
	List list = ht.executeFind(new HibernateCallback() {
		public Object doInHibernate(Session session)
				throws HibernateException, SQLException {
			Query query = session.createQuery(hql)
					.setString(0, id);
			return query.list();
		}
	});
	return list;
}

public void seveMaterialsInformation(final DeviceInfo dfo) {
	saveOrUpdateObject(dfo);
}
public List getMateerials(final String id){
	return jt.queryForList("{call xmis.sp_materialsPriceInformation(?)}", new Object[]{ id});
}
public DeviceInfo getMaterialnfoObjById(String id) {
	// TODO Auto-generated method stub
	return (DeviceInfo) ht.load(DeviceInfo.class, id);
}

public void doAddMaterialInformation(DeviceInfo dfo, Currency crr,
		CompanyObj cob, Power pr, Weight wt) {
	// TODO Auto-generated method stub
	
}
/**
 * 查询物资列表
 * */
public List getDeviceInfoListStoreByQuery(StandardQuery query, PageInfo pageInfo) throws Exception {
	String sql = "select dev.物资ID,con_1.乙方公司ID,sum(qd.数量) as 数量,org.公司名称 as 公司名称  " + 
						"into #sl1 " + 
						"from dev_物资信息库 dev " + 
						"left outer join " + 
						"dev_采购清单明细 qd " + 
						"on dev.物资ID = qd.项目物资ID " + 
						"left outer join " + 
						"con_合同 con_1 " + 
						"on qd.支出合同ID = con_1.合同ID  " + 
						"left outer join " + 
						"org_公司 org " + 
						"on con_1.乙方公司ID = org.公司ID  " + 
						"group by dev.物资ID,con_1.乙方公司ID,org.公司名称 " + 
						"select 物资ID,max(数量) 最大数量 into #sl2 from #sl1  group by 物资ID " + 
						"select #sl2.物资ID,#sl1.乙方公司ID,#sl1.公司名称 into #sl3 from #sl2 left join #sl1  on #sl1.物资ID=#sl2.物资ID and #sl2.最大数量=#sl1.数量 " + 
						"select  dev.物资ID,prj.项目ID ,sum(dev_1.数量) as  t,prj.项目名称 as 项目名称 ,prj.项目编码 as 项目编码  " + 
						"  into #tmp1 " + 
						"    from dev_物资信息库 dev " + 
						"  left outer join " + 
						"  dev_采购清单明细 dev_1 " + 
						"    on dev.物资ID = dev_1.项目物资ID " + 
						"  left outer join " + 
						"  con_收入支出合同对应表 con " + 
						"on dev_1.支出合同ID = con.支出子合同ID " + 
						" left outer join " + 
						"con_合同 con_1 " + 
						"  on con.收入合同ID = con_1.合同ID " + 
						"left outer join " + 
						"prj_项目 prj " + 
						"  on con_1.项目ID = prj.项目ID  " + 
						"  group by 物资ID, prj.项目ID,prj.项目名称,prj.项目编码 " + 
						" select 物资ID,max(#tmp1.t) 最多次使用 into #tmp2 from #tmp1  group by 物资ID " + 
						"select #tmp2.物资ID,#tmp1.项目ID ,#tmp1.项目名称,#tmp1.项目编码  into #tmp3 from #tmp2 left join #tmp1 on #tmp1.物资ID = #tmp2.物资ID and #tmp2.最多次使用 = #tmp1.t " +
						
						" select 项目物资ID,max(dev.最后修改日期) as 最后修改日期  " +
						" into #te1 " +
						" from xmis.dev_采购清单明细 dev " +
						" group by dev.项目物资ID " +
						" select dev.项目物资ID as 最近物资ID ,dev.单价 AS 最新单价,dev.最后修改日期  as 最新价格登记日期  " +
						" into #te2 " +
						" from #te1 " +
						" left join xmis.dev_采购清单明细 dev on #te1.项目物资ID =dev.项目物资ID " +
						" and dev.最后修改日期 = #te1.最后修改日期  " +
						
						" select 项目物资ID ,MIN(dev.单价) as 单价  " +
						" into #tr1 " +
						" from xmis.dev_采购清单明细 dev " +
						" group by dev.项目物资ID " +
						" select dev.项目物资ID as 最低物资ID,dev.单价 AS 最低单价,dev.最后修改日期 as 最低价格登记日期  " +
						" into #tr2 " +
						" from #tr1" +
						" left join xmis.dev_采购清单明细 dev on #tr1.项目物资ID =dev.项目物资ID" +
						" and dev.单价 = #tr1.单价  " + 
						
						" select dev.*,#sl3.公司名称,#sl3.乙方公司ID,#tmp3.项目ID,#tmp3.项目名称 ,#tmp3.项目编码, " +
						" sys1.状态 as 物资业务属性名称 ,sys2.状态 AS 物资自身属性名称 ," +
						" sys3.状态 as 物资用途名称 ," + 
						" org.公司名称 as 生产厂家名称," +
						" syshb.货币名称 as 货币名称," +
						"#te2.最新单价,#te2.最新价格登记日期,#tr2.最低单价,#tr2.最低价格登记日期," +
						
						" CASE WHEN dev.停产时间 >= getdate() " +
						" AND dev.停产时间 > dev.投入市场日期 THEN '在产' " +
						
						" WHEN  dev.停产时间 IS NULL " +
						" AND dev.投入市场日期 < getdate() THEN '在产' " +
						
						" WHEN  dev.停产时间<= getdate() " +
						" AND dev.投入市场日期 IS NOT NULL THEN '停产' " +
						
						" WHEN  dev.投入市场日期 IS NULL " +
						" AND dev.停产时间 <=getdate() THEN '停产' " +
						
						" WHEN  dev.投入市场日期 >getdate() " +
						" AND dev.停产时间 > getdate() THEN '' " +
						
						" WHEN  dev.投入市场日期 IS NULL " +
						" AND dev.停产时间 > getdate() THEN '在产' " +
						
						" ELSE '' " +
						" END AS 物资生产状态名称 " +
						" into  #result from " +
						" dev_物资信息库 dev " + 
						" left join " + 
						" #tmp3 " + 
						"  on " + 
						"  dev.物资ID = #tmp3.物资ID " + 
						"  left join " +
						" #sl3 " +
						" on " +
						" dev.物资ID = #sl3.物资ID " +
						"left join " + 
						"    org_公司 org " + 
						"  on " + 
						"    dev.生产厂家ID = org.公司ID " + 
						"  left join " + 
						"    sys_物资信息业务属性 sys1 " + 
						"  on " + 
						"    dev.物资业务属性 = sys1.状态ID " + 
						"  left join " + 
						"    sys_物资自身属性 sys2 " + 
						"  on " + 
						"    dev.物资自身属性 = sys2.状态ID " + 
						"  left join " + 
						"    sys_物资用途 sys3 " + 
						"  on " + 
						"    dev.物资用途 = sys3.状态ID " +
						" left join " +
						" #tr2 " +
						" on dev.物资ID = #tr2.最低物资ID " +
						" left join  " +
						" #te2 " +
						" on dev.物资ID = #te2.最近物资ID " +
						" left join " +
						" xmis.sys_货币 syshb " +
						" on dev.货币ID = syshb.货币ID " +
						" order by dev.物资名称  ";
					SQLGenerator generator = new SQLGenerator(sql);
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

		public List getMaximumSupplier(String materialsID) {
			String sql = "select dev.项目物资ID,org.公司ID,org.公司名称,sum(dev.数量) as 数量  " +
					"from dev_采购清单明细 dev " +
					"left join " +
					"con_合同 con " +
					"on " +
					"dev.支出合同ID = con.合同ID " +
					"left join " +
					"org_公司 org " +
					"on " +
					"con.乙方公司ID = org.公司ID " +
					"WHERE dev.项目物资ID = ? " +
					"GROUP BY dev.项目物资ID,org.公司ID ,org.公司名称   " +
					"order by 数量  DESC ";
			return jt.queryForList(sql, new Object[] { materialsID });
		}

		public List getUseProject(String materialsID) {
			String sql = "select dev.项目物资ID,prj.项目ID, prj.项目编码,prj.项目名称 ,SUM(dev.数量) AS 数量   from " +
					"dev_采购清单明细 dev " +
					"left join " +
					"con_收入支出合同对应表 con " +
					"on " +
					"dev.支出合同ID = con.支出子合同ID " +
					"left join " +
					"con_合同 con1 " +
					"on " +
					"con.收入合同ID = con1.合同ID " +
					"left join " +
					"prj_项目 prj " +
					"on " +
					"con1.项目ID = prj.项目ID " +
					"where dev.项目物资ID = ? " +
					"group by dev.项目物资ID,prj.项目ID, prj.项目编码,prj.项目名称   " +
					"order by 数量 desc ";
			return jt.queryForList(sql, new Object[] { materialsID });
		}

		
		public List getDeviceInfoList(StandardQuery query, PageInfo pageInfo) {
			SQLGenerator generator = new SQLGenerator(SQLScript.getDeviceInfoList);
			if(query !=null){
				query.addToSQLGenerator(generator);
			}
			String sql = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0))?new Object[] {} : new Object[] {
							new Integer(pageInfo.getStart() + 1),
							new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sql,args);
		}

		
		public List getUseStypeList() {
			return jt.queryForList(SQLScript.getUseStypeList);
		}

		
		public List getDeviceInListByContractId(StandardQuery query, PageInfo pageInfo) {
			
			SQLGenerator generator = new SQLGenerator(SQLScript.getDeviceInListByContractId);
			if(query !=null){
				query.addToSQLGenerator(generator);
			}
			String sql = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0))?new Object[] {} : new Object[] {
							new Integer(pageInfo.getStart() + 1),
							new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sql,args);
		}

		@SuppressWarnings("rawtypes")
		
		public List getDeviceInInfoByID(String id) {
			return jt.queryForList(SQLScript.getDeviceInfoObjById, new Object[]{id});
		}

		
		public void delDeviceInObjByID(String id) {
			jt.execute("delete FROM  xmis.dev_到货清单 WHERE ID='"+id+"'");
		}

		
		public void doUpdateDeviceIn(DeviceInObj deviceIn) {
			ht.update(deviceIn);
		}

		@SuppressWarnings("rawtypes")
		
		public List getDeviceAddressList(StandardQuery query, PageInfo pageInfo) {
			
			SQLGenerator generator = new SQLGenerator(SQLScript.getDeviceAddressList);
			if(query !=null){
				query.addToSQLGenerator(generator);
			}
			String sql = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0))?new Object[] {} : new Object[] {
							new Integer(pageInfo.getStart() + 1),
							new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
			return jt.queryForList(sql,args);
		}

		@SuppressWarnings("rawtypes")
		
		public List getDeviceInfoListWithPurchaseList(String contractId) {
			return jt.queryForList(SQLScript.getDeviceInfoListWithPurchaseList,new Object[]{contractId});
		}

		@SuppressWarnings("rawtypes")
		public List getContactListByProjectIdCombo(String fid) {
			return jt.queryForList(SQLScript.getContactListByProjectIdCombo, new Object[]{fid});
		}

		
		public List getPurchaseSumNum(String deviceInfoId,String purchaseListId) {
			return jt.queryForList(SQLScript.getPurchaseSumNum, new Object[]{deviceInfoId,purchaseListId});
			}

		
		public List getImportDeviceInfoList(StandardQuery query, PageInfo pageInfo) {
			SQLGenerator generator = new SQLGenerator(SQLScript.getImportDeviceInfoList);
			if(query !=null){
				query.addToSQLGenerator(generator);
			}
			String sql = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
			Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
					.getLimit() == 0))?new Object[] {} : new Object[] {
							new Integer(pageInfo.getStart() + 1),
							new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
					return jt.queryForList(sql,args);
		}


}
