package com.bitc.jk.xmis.export;

import java.util.Map;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.WebUtil;

public class formatExport implements Formater{

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {
		EmployeeObj emp = WebUtil.getCurrentEmployee();

		boolean decisionFlag = emp.hasDecisionRole();
		if(!decisionFlag){
			return "--";
		}else{
			return value;
		}
	}

}
