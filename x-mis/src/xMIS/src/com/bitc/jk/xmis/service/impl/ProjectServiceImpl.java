//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\ProjectServiceImpl.java

package com.bitc.jk.xmis.service.impl;

import java.util.List;

import org.apache.log4j.Logger;

import com.bitc.jk.xmis.dao.ProjectDAO;
import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.FunctionModuleObj;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectTaskObj;
import com.bitc.jk.xmis.service.ProjectService;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

public class ProjectServiceImpl implements ProjectService 
{
	private static Logger logger = Logger.getLogger(ProjectServiceImpl.class);
  private ProjectDAO projectDAO;
   
   /**
    * @roseuid 4C1F041F004E
    */
   public ProjectServiceImpl() 
   {
    
   }
   
   /**
    * Access method for the projectDAO property.
    * 
    * @return   the current value of the projectDAO property
    */
   public ProjectDAO getProjectDAO() 
   {
      return projectDAO;
   }
   
   /**
    * Sets the value of the projectDAO property.
    * 
    * @param aProjectDAO the new value of the projectDAO property
    */
   public void setProjectDAO(ProjectDAO aProjectDAO) 
   {
      projectDAO = aProjectDAO;
   }
   
   /**
    * @param userID
    * @return List
    * @roseuid 4C1F041F008C
    */
   public List getUnfinishedProjectListByUser(int userID) 
   {
    return null;
   }
   
   /**
    * @param projectID
    * @return com.bitc.jk.xmis.model.ProjectObj
    * @roseuid 4C1F041F00EA
    */
   public ProjectObj getProjectInfoByID(int projectID) 
   {
    return null;
   }
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C1F041F0119
    */
   public List getProjectWorkProductListByID(int projectID) 
   {
    return null;
   }
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C1F041F0148
    */
   public List getFunctionModuleListByID(int projectID) 
   {
    return null;
   }
   
   /**
    * @param functionModuleList
    * @roseuid 4C1F041F0177
    */
   public void doSaveFunctionModule(List functionModuleList) 
   {
    
   }
   
   /**
    * @param functionModelObj
    * @roseuid 4C1F041F01B5
    */
   public void doUpdateFunctionModule(FunctionModuleObj functionModelObj) 
   {
    
   }
   
   /**
    * @param projectID
    * @param functionModuleID
    * @roseuid 4C1F041F01E4
    */
   public void doDeleteFunctionModule(int projectID, int functionModuleID) 
   {
    
   }
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C1F041F0242
    */
   public List getDeviceListByID(int projectID) 
   {
    return null;
   }
   
   /**
    * @param deviceList
    * @roseuid 4C1F041F0280
    */
   public void doSaveDeviceList(List deviceList) 
   {
    
   }
   
   /**
    * @param deviceObj
    * @roseuid 4C1F041F02BF
    */
   public void doUpdateDeviceList(DeviceObj deviceObj) 
   {
    
   }
   
   /**
    * @param projectID
    * @param deviceID
    * @roseuid 4C1F041F030D
    */
   public void doDeleteDeviceList(int projectID, int deviceID) 
   {
    
   }
   
   /**
    * @param projectObj
    * @roseuid 4C1F041F036B
    */
   public void doSaveProjectInfo(ProjectObj projectObj) 
   {
    
   }
   
   /**
    * @param projectObj
    * @roseuid 4C1F041F03A9
    */
   public void doUpdateProjectInfo(ProjectObj projectObj) 
   {
    
   }
   
   /**
    * @param projectTaskObj
    * @roseuid 4C1F0420000F
    */
   public void doSaveProjectTask(ProjectTaskObj projectTaskObj) 
   {
    
   }
   
   /**
    * @param projectTaskObj
    * @roseuid 4C1F0420004E
    */
   public void doUpdateProjectTask(ProjectTaskObj projectTaskObj) 
   {
    
   }
   
   /**
    * @param userID
    * @return com.bitc.jk.xmis.model.ProjectObj
    * @roseuid 4C1F0420009C
    */
   public ProjectObj getProjectReport(int userID) 
   {
    return null;
   }
   
   
   public String getProjectStatusStore(String searchName){
	   
		logger.info("--->  getProjectStatusStore "
			);

		return WebUtil.buildStoreForJsonWithoutPageInfo(projectDAO
				.getProjectStatusStore(searchName),
				WebKeys.Action_Method_Name_ProjectStatus);
		
   }
}
