//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\impl\\TaskDAOImpl.java

package com.bitc.jk.xmis.dao.impl;

import java.sql.Date;
import java.util.List;

import javax.sql.DataSource;

import org.apache.log4j.Logger;

import com.bitc.jk.xmis.dao.TaskDAO;
import com.bitc.jk.xmis.model.TaskObj;

public class TaskDAOImpl implements TaskDAO 
{
	private static Logger logger = Logger.getLogger(TaskDAOImpl.class);
  private DataSource xMISDB;
   
   /**
    * @roseuid 4C19C97B0000
    */
   public TaskDAOImpl() 
   {
    
   }
   
   /**
    * Access method for the xMISDB property.
    * 
    * @return   the current value of the xMISDB property
    */
   public DataSource getXMISDB() 
   {
      return xMISDB;
   }
   
   /**
    * Sets the value of the xMISDB property.
    * 
    * @param aXMISDB the new value of the xMISDB property
    */
   public void setXMISDB(DataSource aXMISDB) 
   {
      xMISDB = aXMISDB;
   }
   
   /**
    * @param userID
    * @param taskType
    * @return TaskList
    * @roseuid 4C19C97B000F
    */
   public List getTaskListByUser(int userID, int taskType) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param taskID
    * @return com.bitc.jk.xmis.model.TaskObj
    * @roseuid 4C19C97B005D
    */
   public TaskObj getTaskInfoByID(int userID, int taskID) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param date
    * @param taskType
    * @return TaskList
    * @roseuid 4C19C97B00BB
    */
   public List getNewTaskListByDate(int userID, Date date, int taskType) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param groupType
    * @param taskType
    * @return TaskList
    * @roseuid 4C19C97B0148
    */
   public List getNewTaskListByGroup(int userID, int groupType, int taskType) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C97B01C5
    */
   public void doReceive(int userID, int taskID) 
   {
    
   }
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C97B0222
    */
   public void doReject(int userID, int taskID) 
   {
    
   }
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C97B0290
    */
   public void doSave(int userID, int taskID) 
   {
    
   }
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C97B02FD
    */
   public void doForward(int userID, int taskID) 
   {
    
   }
   
   /**
    * @param userID
    * @param date
    * @param taskType
    * @return TaskList
    * @roseuid 4C19CC6A0109
    */
   public List getTaskListByDate(int userID, Date date, int taskType) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param groupType
    * @param taskType
    * @return TaskList
    * @roseuid 4C19CC6A01A5
    */
   public List getTaskListByGroup(int userID, int groupType, int taskType) 
   {
    return null;
   }
}
