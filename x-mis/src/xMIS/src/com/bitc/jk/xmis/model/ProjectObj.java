//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\ProjectObj.java

package com.bitc.jk.xmis.model;

import java.sql.Date;
import java.util.List;

public class ProjectObj 
{
   private int projectID;
   private String projectCode;
   private String projectName;
   private int contractID;
   private Date startDate;
   private int currentStatusID;
   private String currentStatusName;
   private int projectTypeID;
   private String projectTypeName;
   private List contractList;
   private List departmentList;
   private List employeeList;
   private List workProductList;
   private List functionList;
   private List deviceList;
   
   /**
    * @roseuid 4C1F042001C5
    */
   public ProjectObj() 
   {
    
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
    * Access method for the projectCode property.
    * 
    * @return   the current value of the projectCode property
    */
   public String getProjectCode() 
   {
      return projectCode;
   }
   
   /**
    * Sets the value of the projectCode property.
    * 
    * @param aProjectCode the new value of the projectCode property
    */
   public void setProjectCode(String aProjectCode) 
   {
      projectCode = aProjectCode;
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
    * Access method for the contractID property.
    * 
    * @return   the current value of the contractID property
    */
   public int getContractID() 
   {
      return contractID;
   }
   
   /**
    * Sets the value of the contractID property.
    * 
    * @param aContractID the new value of the contractID property
    */
   public void setContractID(int aContractID) 
   {
      contractID = aContractID;
   }
   
   /**
    * Access method for the startDate property.
    * 
    * @return   the current value of the startDate property
    */
   public Date getStartDate() 
   {
      return startDate;
   }
   
   /**
    * Sets the value of the startDate property.
    * 
    * @param aStartDate the new value of the startDate property
    */
   public void setStartDate(Date aStartDate) 
   {
      startDate = aStartDate;
   }
   
   /**
    * Access method for the currentStatusID property.
    * 
    * @return   the current value of the currentStatusID property
    */
   public int getCurrentStatusID() 
   {
      return currentStatusID;
   }
   
   /**
    * Sets the value of the currentStatusID property.
    * 
    * @param aCurrentStatusID the new value of the currentStatusID property
    */
   public void setCurrentStatusID(int aCurrentStatusID) 
   {
      currentStatusID = aCurrentStatusID;
   }
   
   /**
    * Access method for the currentStatusName property.
    * 
    * @return   the current value of the currentStatusName property
    */
   public String getCurrentStatusName() 
   {
      return currentStatusName;
   }
   
   /**
    * Sets the value of the currentStatusName property.
    * 
    * @param aCurrentStatusName the new value of the currentStatusName property
    */
   public void setCurrentStatusName(String aCurrentStatusName) 
   {
      currentStatusName = aCurrentStatusName;
   }
   
   /**
    * Access method for the projectTypeID property.
    * 
    * @return   the current value of the projectTypeID property
    */
   public int getProjectTypeID() 
   {
      return projectTypeID;
   }
   
   /**
    * Sets the value of the projectTypeID property.
    * 
    * @param aProjectTypeID the new value of the projectTypeID property
    */
   public void setProjectTypeID(int aProjectTypeID) 
   {
      projectTypeID = aProjectTypeID;
   }
   
   /**
    * Access method for the projectTypeName property.
    * 
    * @return   the current value of the projectTypeName property
    */
   public String getProjectTypeName() 
   {
      return projectTypeName;
   }
   
   /**
    * Sets the value of the projectTypeName property.
    * 
    * @param aProjectTypeName the new value of the projectTypeName property
    */
   public void setProjectTypeName(String aProjectTypeName) 
   {
      projectTypeName = aProjectTypeName;
   }
   
   /**
    * Access method for the contractList property.
    * 
    * @return   the current value of the contractList property
    */
   public List getContractList() 
   {
      return contractList;
   }
   
   /**
    * Sets the value of the contractList property.
    * 
    * @param aContractList the new value of the contractList property
    */
   public void setContractList(List aContractList) 
   {
      contractList = aContractList;
   }
   
   /**
    * Access method for the departmentList property.
    * 
    * @return   the current value of the departmentList property
    */
   public List getDepartmentList() 
   {
      return departmentList;
   }
   
   /**
    * Sets the value of the departmentList property.
    * 
    * @param aDepartmentList the new value of the departmentList property
    */
   public void setDepartmentList(List aDepartmentList) 
   {
      departmentList = aDepartmentList;
   }
   
   /**
    * Access method for the employeeList property.
    * 
    * @return   the current value of the employeeList property
    */
   public List getEmployeeList() 
   {
      return employeeList;
   }
   
   /**
    * Sets the value of the employeeList property.
    * 
    * @param aEmployeeList the new value of the employeeList property
    */
   public void setEmployeeList(List aEmployeeList) 
   {
      employeeList = aEmployeeList;
   }
   
   /**
    * Access method for the workProductList property.
    * 
    * @return   the current value of the workProductList property
    */
   public List getWorkProductList() 
   {
      return workProductList;
   }
   
   /**
    * Sets the value of the workProductList property.
    * 
    * @param aWorkProductList the new value of the workProductList property
    */
   public void setWorkProductList(List aWorkProductList) 
   {
      workProductList = aWorkProductList;
   }
   
   /**
    * Access method for the functionList property.
    * 
    * @return   the current value of the functionList property
    */
   public List getFunctionList() 
   {
      return functionList;
   }
   
   /**
    * Sets the value of the functionList property.
    * 
    * @param aFunctionList the new value of the functionList property
    */
   public void setFunctionList(List aFunctionList) 
   {
      functionList = aFunctionList;
   }
   
   /**
    * Access method for the deviceList property.
    * 
    * @return   the current value of the deviceList property
    */
   public List getDeviceList() 
   {
      return deviceList;
   }
   
   /**
    * Sets the value of the deviceList property.
    * 
    * @param aDeviceList the new value of the deviceList property
    */
   public void setDeviceList(List aDeviceList) 
   {
      deviceList = aDeviceList;
   }
}
