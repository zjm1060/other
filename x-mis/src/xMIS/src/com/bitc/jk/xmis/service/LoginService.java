//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\LoginService.java

package com.bitc.jk.xmis.service;

import java.util.List;

import com.bitc.jk.xmis.model.EmployeeObj;

public interface LoginService 
{
   
   /**
    * @param loginName
    * @param password
    * @return com.bitc.jk.xmis.model.EmployeeObj
    * @roseuid 4C198BC0008C
    */
   public EmployeeObj userValidete(String loginName, String password) throws Exception;
   
   /**
    * @param userID
    * @return List
    * @roseuid 4C198C0501F4
    */
   public List getUserRight(int userID);
   
   public String buildLeftMenu(String treeName, EmployeeObj userDNA);
}
