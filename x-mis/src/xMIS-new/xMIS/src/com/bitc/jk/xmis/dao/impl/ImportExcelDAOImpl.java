package com.bitc.jk.xmis.dao.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementSetter;
import org.springframework.orm.hibernate3.HibernateCallback;

import com.bitc.jk.xmis.dao.ImportExcelDAO;
import com.bitc.jk.xmis.model.DeviceSheetImportInfo;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;

public class ImportExcelDAOImpl extends BaseDAOImpl  implements ImportExcelDAO {

	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate ajdbcTemplate) {
		this.jt = ajdbcTemplate;
	}

	@SuppressWarnings("rawtypes")
	public int doSaveDeviceList(Map<String,String>  dataMap, String sheetUuid,String levelIdFront) {
		Iterator iter = dataMap.entrySet().iterator();
		
		String columnName = "";
		String data = "";
		while(iter.hasNext()){
			Map.Entry c = (Entry) iter.next();
			String key = Type.getString(c.getKey());
			if (!key.equals(WebKeys.ROW)) {
				columnName += key.concat(",");
				if (key.equals(WebKeys.UNIT_PRICE)
						|| key.equals(WebKeys.SUM_PRICE)
						|| key.equals(WebKeys.COST_PRICE)) {
					data += "'" + Type.getInteger(c.getValue()!=null?c.getValue().toString().trim():c.getValue()) + "',";
				} else if(key.equals(WebKeys.LEVEL_ID)){
					data += "'" + levelIdFront+Type.getString(c.getValue()) + "',";
					
				}else{
					data += "'" + Type.getString(c.getValue()) + "',";
				}
			}
		}
		
		if(columnName.length()>1){
			columnName = columnName.substring(0, columnName.length()-1);
		}
		if(data.length()>1){
			data = data.substring(0, data.length()-1);
		}
		String sql = "INSERT INTO xmis.dev_设备清单导入信息 (\n id,sheet_uuid,"
			+ columnName
			+ "\n) VALUES  (\n  '"
			+ UUIDUtils.nextCode()
			+ "','"+sheetUuid+"'," + data + "\n)";
			return jt.update(sql);
	}

	public String doSaveDevceSheetImportInfo(DeviceSheetImportInfo sheetInfo) {
		if(sheetInfo.getSheetUuid()!=null && !sheetInfo.getSheetUuid().equals("")){
			ht.update(sheetInfo);
		}else{
			ht.save(sheetInfo);
		}
		return sheetInfo.getSheetUuid();
	}

	@SuppressWarnings("unchecked")
	public DeviceSheetImportInfo getDeviceSheetImportInfoByInfo(final int fileId,
			final String sheetName) {
		List<DeviceSheetImportInfo> list = new ArrayList<DeviceSheetImportInfo>();
		list = ht.executeFind(new HibernateCallback() {
			
			public Object doInHibernate(Session session) throws HibernateException,
					SQLException {
				String hql = "from DeviceSheetImportInfo t where t.fileId="+fileId+" and t.sheetName='"+sheetName+"'";
				Query query = session.createQuery(hql);
				return query.list();
			}
		});
		if(list !=null && list.size()>0){
			return list.get(0);
		}
		return null;
	}

	public void delExistImportInfo(final int attID) {
		String sql = SQLScript.delExistImportInfo;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement arg0) throws SQLException {
				arg0.setInt(1, attID);
			
			}
		});
	}

	public void delExistSheetInfo(final int attID) {
		String sql = SQLScript.delExistSheetInfo;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement arg0) throws SQLException {
				arg0.setInt(1, attID);
			
			}
		});
	}

	public void delAttachById(final int attID) {
		String sql = SQLScript.delAttachById;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement arg0) throws SQLException {
				arg0.setInt(1, attID);
			
			}
		});
	}

//	public int getSheetIdByFileId(int attID) {
//		String sql = SQLScript.getImportDeviceSheetCount;
//		return jt.queryForInt(sql,new Object[]{attID});
//	}

 
}
