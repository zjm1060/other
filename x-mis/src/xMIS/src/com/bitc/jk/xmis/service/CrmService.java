package com.bitc.jk.xmis.service;

import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.util.DuplicatedException;

public interface CrmService {

	public String dataValidate(CompanySimpleObj comp);
	public String doCreateNewCompanySimple(CompanySimpleObj comp,
			EmployeeObj userDNA) throws DuplicatedException, Exception ;
	
	public String getPeasonListByCompany(int compID, EmployeeObj userDNA);

	public String getContactPersonBySearchName(int compID, String searchName,
			EmployeeObj userDNA);

}
