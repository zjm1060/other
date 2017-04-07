//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\ProjectTaskObj.java

package src;

import java.sql.Date;

public class ProjectTaskObj 
{
   private int taskID;
   private String taskName;
   private int projectID;
   private String projectName;
   private Date beginDate;
   private Date endDate;
   private int previousTaskID;
   private int assigedToDeptID;
   private String assignedToDeptName;
   private int assigedToEmpID;
   private String assigedToEmpName;
   private int taskSponsorID;
   private String taskSponsorName;
   private String taskScheme;
   private String taskDifficulty;
   private Date taskETADate;
   private int taskTypeID;
   private String taskTypeName;
   private int regularTaskID;
   private String regularTaskName;
   private String remark;
   
   /**
 * @return the remark
 */
public String getRemark() {
	return remark;
}

/**
 * @param remark the remark to set
 */
public void setRemark(String remark) {
	this.remark = remark;
}

/**
    * @roseuid 4C1F0422033C
    */
   public ProjectTaskObj() 
   {
    
   }
   
   /**
    * Access method for the taskID property.
    * 
    * @return   the current value of the taskID property
    */
   public int getTaskID() 
   {
      return taskID;
   }
   
   /**
    * Sets the value of the taskID property.
    * 
    * @param aTaskID the new value of the taskID property
    */
   public void setTaskID(int aTaskID) 
   {
      taskID = aTaskID;
   }
   
   /**
    * Access method for the taskName property.
    * 
    * @return   the current value of the taskName property
    */
   public String getTaskName() 
   {
      return taskName;
   }
   
   /**
    * Sets the value of the taskName property.
    * 
    * @param aTaskName the new value of the taskName property
    */
   public void setTaskName(String aTaskName) 
   {
      taskName = aTaskName;
   }
   
   /**
    * Access method for the projectID property.
    * 
    * @return   the current value of the projectID property
    */
   public int getProjectID() 
   {
      return projectID;
   }
   
   /**
    * Sets the value of the projectID property.
    * 
    * @param aProjectID the new value of the projectID property
    */
   public void setProjectID(int aProjectID) 
   {
      projectID = aProjectID;
   }
   
   /**
    * Access method for the projectName property.
    * 
    * @return   the current value of the projectName property
    */
   public String getProjectName() 
   {
      return projectName;
   }
   
   /**
    * Sets the value of the projectName property.
    * 
    * @param aProjectName the new value of the projectName property
    */
   public void setProjectName(String aProjectName) 
   {
      projectName = aProjectName;
   }
   
   /**
    * Access method for the beginDate property.
    * 
    * @return   the current value of the beginDate property
    */
   public Date getBeginDate() 
   {
      return beginDate;
   }
   
   /**
    * Sets the value of the beginDate property.
    * 
    * @param aBeginDate the new value of the beginDate property
    */
   public void setBeginDate(Date aBeginDate) 
   {
      beginDate = aBeginDate;
   }
   
   /**
    * Access method for the endDate property.
    * 
    * @return   the current value of the endDate property
    */
   public Date getEndDate() 
   {
      return endDate;
   }
   
   /**
    * Sets the value of the endDate property.
    * 
    * @param aEndDate the new value of the endDate property
    */
   public void setEndDate(Date aEndDate) 
   {
      endDate = aEndDate;
   }
   
   /**
    * Access method for the previousTaskID property.
    * 
    * @return   the current value of the previousTaskID property
    */
   public int getPreviousTaskID() 
   {
      return previousTaskID;
   }
   
   /**
    * Sets the value of the previousTaskID property.
    * 
    * @param aPreviousTaskID the new value of the previousTaskID property
    */
   public void setPreviousTaskID(int aPreviousTaskID) 
   {
      previousTaskID = aPreviousTaskID;
   }
   
   /**
    * Access method for the assigedToDeptID property.
    * 
    * @return   the current value of the assigedToDeptID property
    */
   public int getAssigedToDeptID() 
   {
      return assigedToDeptID;
   }
   
   /**
    * Sets the value of the assigedToDeptID property.
    * 
    * @param aAssigedToDeptID the new value of the assigedToDeptID property
    */
   public void setAssigedToDeptID(int aAssigedToDeptID) 
   {
      assigedToDeptID = aAssigedToDeptID;
   }
   
   /**
    * Access method for the assignedToDeptName property.
    * 
    * @return   the current value of the assignedToDeptName property
    */
   public String getAssignedToDeptName() 
   {
      return assignedToDeptName;
   }
   
   /**
    * Sets the value of the assignedToDeptName property.
    * 
    * @param aAssignedToDeptName the new value of the assignedToDeptName property
    */
   public void setAssignedToDeptName(String aAssignedToDeptName) 
   {
      assignedToDeptName = aAssignedToDeptName;
   }
   
   /**
    * Access method for the assigedToEmpID property.
    * 
    * @return   the current value of the assigedToEmpID property
    */
   public int getAssigedToEmpID() 
   {
      return assigedToEmpID;
   }
   
   /**
    * Sets the value of the assigedToEmpID property.
    * 
    * @param aAssigedToEmpID the new value of the assigedToEmpID property
    */
   public void setAssigedToEmpID(int aAssigedToEmpID) 
   {
      assigedToEmpID = aAssigedToEmpID;
   }
   
   /**
    * Access method for the assigedToEmpName property.
    * 
    * @return   the current value of the assigedToEmpName property
    */
   public String getAssigedToEmpName() 
   {
      return assigedToEmpName;
   }
   
   /**
    * Sets the value of the assigedToEmpName property.
    * 
    * @param aAssigedToEmpName the new value of the assigedToEmpName property
    */
   public void setAssigedToEmpName(String aAssigedToEmpName) 
   {
      assigedToEmpName = aAssigedToEmpName;
   }
   
   /**
    * Access method for the taskSponsorID property.
    * 
    * @return   the current value of the taskSponsorID property
    */
   public int getTaskSponsorID() 
   {
      return taskSponsorID;
   }
   
   /**
    * Sets the value of the taskSponsorID property.
    * 
    * @param aTaskSponsorID the new value of the taskSponsorID property
    */
   public void setTaskSponsorID(int aTaskSponsorID) 
   {
      taskSponsorID = aTaskSponsorID;
   }
   
   /**
    * Access method for the taskSponsorName property.
    * 
    * @return   the current value of the taskSponsorName property
    */
   public String getTaskSponsorName() 
   {
      return taskSponsorName;
   }
   
   /**
    * Sets the value of the taskSponsorName property.
    * 
    * @param aTaskSponsorName the new value of the taskSponsorName property
    */
   public void setTaskSponsorName(String aTaskSponsorName) 
   {
      taskSponsorName = aTaskSponsorName;
   }
   
   /**
    * Access method for the taskScheme property.
    * 
    * @return   the current value of the taskScheme property
    */
   public String getTaskScheme() 
   {
      return taskScheme;
   }
   
   /**
    * Sets the value of the taskScheme property.
    * 
    * @param aTaskScheme the new value of the taskScheme property
    */
   public void setTaskScheme(String aTaskScheme) 
   {
      taskScheme = aTaskScheme;
   }
   
   /**
    * Access method for the taskDifficulty property.
    * 
    * @return   the current value of the taskDifficulty property
    */
   public String getTaskDifficulty() 
   {
      return taskDifficulty;
   }
   
   /**
    * Sets the value of the taskDifficulty property.
    * 
    * @param aTaskDifficulty the new value of the taskDifficulty property
    */
   public void setTaskDifficulty(String aTaskDifficulty) 
   {
      taskDifficulty = aTaskDifficulty;
   }
   
   /**
    * Access method for the taskETADate property.
    * 
    * @return   the current value of the taskETADate property
    */
   public Date getTaskETADate() 
   {
      return taskETADate;
   }
   
   /**
    * Sets the value of the taskETADate property.
    * 
    * @param aTaskETADate the new value of the taskETADate property
    */
   public void setTaskETADate(Date aTaskETADate) 
   {
      taskETADate = aTaskETADate;
   }
   
   /**
    * Access method for the taskTypeID property.
    * 
    * @return   the current value of the taskTypeID property
    */
   public int getTaskTypeID() 
   {
      return taskTypeID;
   }
   
   /**
    * Sets the value of the taskTypeID property.
    * 
    * @param aTaskTypeID the new value of the taskTypeID property
    */
   public void setTaskTypeID(int aTaskTypeID) 
   {
      taskTypeID = aTaskTypeID;
   }
   
   /**
    * Access method for the taskTypeName property.
    * 
    * @return   the current value of the taskTypeName property
    */
   public String getTaskTypeName() 
   {
      return taskTypeName;
   }
   
   /**
    * Sets the value of the taskTypeName property.
    * 
    * @param aTaskTypeName the new value of the taskTypeName property
    */
   public void setTaskTypeName(String aTaskTypeName) 
   {
      taskTypeName = aTaskTypeName;
   }
   
   /**
    * Access method for the regularTaskID property.
    * 
    * @return   the current value of the regularTaskID property
    */
   public int getRegularTaskID() 
   {
      return regularTaskID;
   }
   
   /**
    * Sets the value of the regularTaskID property.
    * 
    * @param aRegularTaskID the new value of the regularTaskID property
    */
   public void setRegularTaskID(int aRegularTaskID) 
   {
      regularTaskID = aRegularTaskID;
   }
   
   /**
    * Access method for the regularTaskName property.
    * 
    * @return   the current value of the regularTaskName property
    */
   public String getRegularTaskName() 
   {
      return regularTaskName;
   }
   
   /**
    * Sets the value of the regularTaskName property.
    * 
    * @param aRegularTaskName the new value of the regularTaskName property
    */
   public void setRegularTaskName(String aRegularTaskName) 
   {
      regularTaskName = aRegularTaskName;
   }
}
