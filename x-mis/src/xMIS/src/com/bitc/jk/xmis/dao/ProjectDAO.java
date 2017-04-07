//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\ProjectDAO.java

package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.FunctionModuleObj;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectTaskObj;

public interface ProjectDAO 
{
   
   /**
    * @param userID
    * @param isFinished - False: unfinished; True: Finished(all of contracts with 
    * project finished done); Null: All(False + True)
    * @return List
    * @roseuid 4C19D1920186
    */
   public List getProjectListByUser(int userID, Boolean isFinished);
   
   /**
    * @param projectID
    * @return com.bitc.jk.xmis.model.ProjectObj
    * @roseuid 4C19D26B005D
    */
   public ProjectObj getProjectInfoByID(int projectID);
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C19D34E02FD
    */
   public List getProjectWorkProductListByID(int projectID);
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C1AEE9601A5
    */
   public List getFunctionModuleListByID(int projectID);
   
   /**
    * @param functionModuleList
    * @roseuid 4C1AEE9601B5
    */
   public void doSaveFunctionModule(List functionModuleList);
   
   /**
    * @param functionModelObj
    * @roseuid 4C1AEE9601C5
    */
   public void doUpdateFunctionModule(FunctionModuleObj functionModelObj);
   
   /**
    * @param projectID
    * @param functionModuleID
    * @roseuid 4C1AEE9601C7
    */
   public void doDeleteFunctionModule(int projectID, int functionModuleID);
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C1AEE9601D4
    */
   public List getDeviceListByID(int projectID);
   
   /**
    * @param deviceList
    * @roseuid 4C1AEF9F03B9
    */
   public void doSaveDeviceList(List deviceList);
   
   /**
    * @param deviceObj
    * @roseuid 4C1AEFC80251
    */
   public void doUpdateDeviceList(DeviceObj deviceObj);
   
   /**
    * @param projectID
    * @param deviceID
    * @roseuid 4C1AEFCC008C
    */
   public void doDeleteDeviceList(int projectID, int deviceID);
   
   /**
    * @param projectObj
    * @roseuid 4C1B36BA037A
    */
   public void doSaveProjectInfo(ProjectObj projectObj);
   
   /**
    * @param projectObj
    * @roseuid 4C1EB68A009C
    */
   public void doUpdateProjectInfo(ProjectObj projectObj);
   
   /**
    * @param projectTaskObj
    * @roseuid 4C1B39E0005D
    */
   public void doSaveProjectTask(ProjectTaskObj projectTaskObj);
   
   /**
    * @param projectTaskObj
    * @roseuid 4C1EB67A0242
    */
   public void doUpdateProjectTask(ProjectTaskObj projectTaskObj);
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C1ECFDA037A
    */
   public List getDepartmentListByID(int projectID);
   
   /**
    * @param projectID
    * @roseuid 4C1ED00D0148
    */
   public void getEmployeeListByID(int projectID);
   
   /**
    * @param projectID
    * @return List
    * @roseuid 4C1ED01F030D
    */
   public List getTaskListByID(int projectID);
   
   public List getProjectStatusStore(String searchName);
}
