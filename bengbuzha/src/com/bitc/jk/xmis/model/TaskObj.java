//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\TaskObj.java

package com.bitc.jk.xmis.model;

import java.sql.Date;

public class TaskObj 
{
   private int taskID;
   private String taskName;
   private String status;
   private Date date;
   private int operatorID;
   private int groupType;
   
   /**
    * @roseuid 4C19C97D029F
    */
   public TaskObj() 
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
    * Access method for the status property.
    * 
    * @return   the current value of the status property
    */
   public String getStatus() 
   {
      return status;
   }
   
   /**
    * Sets the value of the status property.
    * 
    * @param aStatus the new value of the status property
    */
   public void setStatus(String aStatus) 
   {
      status = aStatus;
   }
   
   /**
    * Access method for the date property.
    * 
    * @return   the current value of the date property
    */
   public Date getDate() 
   {
      return date;
   }
   
   /**
    * Sets the value of the date property.
    * 
    * @param aDate the new value of the date property
    */
   public void setDate(Date aDate) 
   {
      date = aDate;
   }
   
   /**
    * Access method for the operatorID property.
    * 
    * @return   the current value of the operatorID property
    */
   public int getOperatorID() 
   {
      return operatorID;
   }
   
   /**
    * Sets the value of the operatorID property.
    * 
    * @param aOperatorID the new value of the operatorID property
    */
   public void setOperatorID(int aOperatorID) 
   {
      operatorID = aOperatorID;
   }
   
   /**
    * Access method for the groupType property.
    * 
    * @return   the current value of the groupType property
    */
   public int getGroupType() 
   {
      return groupType;
   }
   
   /**
    * Sets the value of the groupType property.
    * 
    * @param aGroupType the new value of the groupType property
    */
   public void setGroupType(int aGroupType) 
   {
      groupType = aGroupType;
   }
}
