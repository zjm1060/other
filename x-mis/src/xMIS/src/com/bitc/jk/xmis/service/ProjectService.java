//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\ProjectService.java

package com.bitc.jk.xmis.service;

import java.util.List;

import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.FunctionModuleObj;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectTaskObj;

public interface ProjectService 
{
   
   /**
    * @param userID
    * @return List
    * @roseuid 4C19D073002E
    */
   public List getUnfinishedProjectListByUser(int userID);
   
   /**
    * @param projectID
    * @return com.bitc.jk.xmis.model.ProjectObj
    * @roseuid 4C19D2710280
    */
   public ProjectObj getProjectInfoByID(int projectID);
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C19D3080213
    */
   public List getProjectWorkProductListByID(int projectID);
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C1AEE58032C
    */
   public List getFunctionModuleListByID(int projectID);
   
   /**
    * @param functionModuleList
    * @roseuid 4C1AEE58033C
    */
   public void doSaveFunctionModule(List functionModuleList);
   
   /**
    * @param functionModelObj
    * @roseuid 4C1AEE58034B
    */
   public void doUpdateFunctionModule(FunctionModuleObj functionModelObj);
   
   /**
    * @param projectID
    * @param functionModuleID
    * @roseuid 4C1AEE58035B
    */
   public void doDeleteFunctionModule(int projectID, int functionModuleID);
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C1AEE58036B
    */
   public List getDeviceListByID(int projectID);
   
   /**
    * @param deviceList
    * @roseuid 4C1AEF830290
    */
   public void doSaveDeviceList(List deviceList);
   
   /**
    * @param deviceObj
    * @roseuid 4C1AEFA50203
    */
   public void doUpdateDeviceList(DeviceObj deviceObj);
   
   /**
    * @param projectID
    * @param deviceID
    * @roseuid 4C1AF0110213
    */
   public void doDeleteDeviceList(int projectID, int deviceID);
   
   /**
    * @param projectObj
    * @roseuid 4C1B36860232
    */
   public void doSaveProjectInfo(ProjectObj projectObj);
   
   /**
    * @param projectObj
    * @roseuid 4C1EB65401C5
    */
   public void doUpdateProjectInfo(ProjectObj projectObj);
   
   /**
    * @param projectTaskObj
    * @roseuid 4C1B384D003E
    */
   public void doSaveProjectTask(ProjectTaskObj projectTaskObj);
   
   /**
    * @param projectTaskObj
    * @roseuid 4C1EB6A70109
    */
   public void doUpdateProjectTask(ProjectTaskObj projectTaskObj);
   
   /**
    * 1) getProjectListByUser
    * 2) getProjectInfoByID
    * 3) getProjectWorkProductListByID
    * 4) getFunctionModuleListByID
    * 5) getDeviceListByID
    * 6) getTaskListByID
    * @param userID
    * @return com.bitc.jk.xmis.model.ProjectObj
    * @roseuid 4C1EC84D007D
    */
   public ProjectObj getProjectReport(int userID);
   
   public String getProjectStatusStore(String searchName);
}
