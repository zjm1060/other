package com.bitc.jk.xmis.model.security;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;

public class BizDataScopeObj implements Serializable {

	private static Logger logger = Logger.getLogger(BizDataScopeObj.class);

	/**
	 * 
	 */
	private static final long serialVersionUID = -8062300686250340591L;

	private static String whereStr = " where ";

	private int dataScopeRightID;

	private int dataDefinitionID;

	private String dataDefinitionName;

	private String dataAlias;

	private String dataDefaultValue;

	private int dataTypeID;

	private String dataTypeDefinition;

	private String dataValue;

	private boolean isSensitive;

	private boolean needParsed;

	private int dataBizlogicID;

	//对记录的操作权限，readonly， update
	private int operationID;

	//数据操作符， = in like
	private String operationDefinition;

	private EmployeeObj empObj;

	private String subSqlStr;

	private Map<String, List> bizLogicMap;

	public BizDataScopeObj() {

		Map aMap = new HashMap();

		aMap.put(WebKeys.Biz_Logic_Department_Type, new ArrayList());
		aMap.put(WebKeys.Biz_Logic_User_Type, new ArrayList());

		this.setBizLogicMap(aMap);

	}

	public String getOperationDefinition() {
		return operationDefinition;
	}

	public void setOperationDefinition(String operationDefinition) {
		this.operationDefinition = operationDefinition;
	}

	public int getOperationID() {
		return operationID;
	}

	public void setOperationID(int operationID) {
		this.operationID = operationID;
	}

	public String getDataDefaultValue() {
		return dataDefaultValue;
	}

	public void setDataDefaultValue(String dataDefaultValue) {
		this.dataDefaultValue = dataDefaultValue;
	}

	public int getDataDefinitionID() {
		return dataDefinitionID;
	}

	public void setDataDefinitionID(int dataDefinitionID) {
		this.dataDefinitionID = dataDefinitionID;
	}

	public String getDataDefinitionName() {
		return dataDefinitionName;
	}

	public void setDataDefinitionName(String dataDefinitionName) {
		this.dataDefinitionName = dataDefinitionName;
	}

	public int getDataScopeRightID() {
		return dataScopeRightID;
	}

	public void setDataScopeRightID(int dataScopeRightID) {
		this.dataScopeRightID = dataScopeRightID;
	}

	public String getDataTypeDefinition() {
		return dataTypeDefinition;
	}

	public void setDataTypeDefinition(String dataTypeDefinition) {
		this.dataTypeDefinition = dataTypeDefinition;
	}

	public int getDataTypeID() {
		return dataTypeID;
	}

	public void setDataTypeID(int dataTypeID) {
		this.dataTypeID = dataTypeID;
	}

	public String getDataValue() {
		return dataValue;
	}

	public void setDataValue(String dataValue) {
		this.dataValue = dataValue;
	}

	public boolean isSensitive() {
		return isSensitive;
	}

	public void setSensitive(boolean isSensitive) {
		this.isSensitive = isSensitive;
	}

	public boolean isNeedParsed() {
		return needParsed;
	}

	public void setNeedParsed(boolean needParsed) {
		this.needParsed = needParsed;
	}

	public EmployeeObj getEmpObj() {
		return empObj;
	}

	public void setEmpObj(EmployeeObj empObj) {
		this.empObj = empObj;
	}

	public void buildSubSql() {

		// if (this.isNeedParsed())
		// parsedDataValue();

		this.subSqlStr = (StringUtils.isEmpty(this.dataAlias) ? this.dataDefinitionName
				: this.dataAlias)
				+ " " + this.operationDefinition + " " + this.dataValue;

	}

	public void mergeSubStr(String newSubSqlStr) {

		// String newSqlStr = StringUtils.substring(this.subSqlStr,
		// this.subSqlStr.indexOf("where (") + 7,
		// this.subSqlStr.length() - 1);
		//
		// String existSqlStr = StringUtils.substring(newSubSqlStr,
		// newSubSqlStr.indexOf("where (") + 7, newSubSqlStr.length() - 1);
		String newSqlStr = this.subSqlStr;
		String existSqlStr = newSubSqlStr;
		if (StringUtils.isEmpty(existSqlStr)) {
			// this.subSqlStr = whereStr + "(" + newSqlStr + ")";
			this.subSqlStr = newSqlStr;
		} else if (existSqlStr.indexOf(newSqlStr) < 0) {
			this.subSqlStr = existSqlStr + " or " + newSqlStr;
		} else {
			this.subSqlStr = existSqlStr;
		}
	}

	private void parsedDataValue() {

		// this.dataValue = Type.getString(this.empObj.getDepartmentID());

		if (this.dataValue.equals("self-deptID"))
			this.dataValue = Type.getString(this.empObj.getDepartmentID());
		else if (this.dataValue.equals("self-userID"))
			this.dataValue = Type.getString(this.empObj.getUserID());
		else if (this.dataValue.equals("self-DivID"))
			this.dataValue = "";
		else
			this.dataValue = Type.getString(this.dataValue);

	}

	public String getSubSqlStr() {
		return subSqlStr;
	}

	public void setSubSqlStr(String subSqlStr) {
		this.subSqlStr = subSqlStr;
	}

	public String getDataAlias() {
		return dataAlias;
	}

	public void setDataAlias(String dataAlias) {
		this.dataAlias = dataAlias;
	}

	public int getDataBizlogicID() {
		return dataBizlogicID;
	}

	public void setDataBizlogicID(int dataBizlogicID) {
		this.dataBizlogicID = dataBizlogicID;
	}

	public Map<String, List> getBizLogicMap() {
		return bizLogicMap;
	}

	public void setBizLogicMap(Map<String, List> bizLogicMap) {
		this.bizLogicMap = bizLogicMap;
	}

}
