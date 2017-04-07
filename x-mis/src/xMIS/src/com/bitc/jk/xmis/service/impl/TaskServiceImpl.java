//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\TaskServiceImpl.java

package com.bitc.jk.xmis.service.impl;

import java.sql.Date;
import java.util.List;

import org.apache.log4j.Logger;

import com.bitc.jk.xmis.dao.TaskDAO;
import com.bitc.jk.xmis.model.TaskObj;
import com.bitc.jk.xmis.service.TaskService;

public class TaskServiceImpl implements TaskService 
{
	private static Logger logger = Logger.getLogger(TaskServiceImpl.class);
   private TaskDAO taskDAO;
   
   /**
    * @roseuid 4C19C97B03C8
    */
   public TaskServiceImpl() 
   {
    
   }
   
   /**
    * Access method for the taskDAO property.
    * 
    * @return   the current value of the taskDAO property
    */
   public TaskDAO getTaskDAO() 
   {
      return taskDAO;
   }
   
   /**
    * Sets the value of the taskDAO property.
    * 
    * @param aTaskDAO the new value of the taskDAO property
    */
   public void setTaskDAO(TaskDAO aTaskDAO) 
   {
      taskDAO = aTaskDAO;
   }
   
   /**
    * @param userID
    * @return TaskList
    * @roseuid 4C19C97B03D8
    */
   public List getNewTaskListByUser(int userID) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param taskID
    * @return com.bitc.jk.xmis.model.TaskObj
    * @roseuid 4C19C97C001F
    */
   public TaskObj getTaskInfoByID(int userID, int taskID) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param date
    * @return TaskList
    * @roseuid 4C19C97C007D
    */
   public List getNewTaskListByDate(int userID, Date date) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param groupType
    * @return TaskList
    * @roseuid 4C19C97C00EA
    */
   public List getNewTaskListByGroup(int userID, int groupType) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C97C0148
    */
   public void doReceive(int userID, int taskID) 
   {
    
   }
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C97C01A5
    */
   public void doReject(int userID, int taskID) 
   {
    
   }
   
   /**
    * @param userID
    * @return TaskList
    * @roseuid 4C19C97C0213
    */
   public List getMyTaskListByUser(int userID) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param date
    * @return TaskList
    * @roseuid 4C19C97C0251
    */
   public List getMyTaskListByDate(int userID, Date date) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param groupType
    * @return TaskList
    * @roseuid 4C19C97C02CE
    */
   public List getMyTaskListByGroup(int userID, int groupType) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C97C0399
    */
   public void doSave(int userID, int taskID) 
   {
    
   }
   
   /**
    * @param userID
    * @roseuid 4C19C97D001F
    */
   public void doForward(int userID) 
   {
    
   }
   
   /**
    * @param userID
    * @return TaskList
    * @roseuid 4C19C97D006D
    */
   public List getHisTaskListByUser(int userID) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param date
    * @return TaskList
    * @roseuid 4C19C97D00BB
    */
   public List getHisTaskListByDate(int userID, Date date) 
   {
    return null;
   }
   
   /**
    * @param userID
    * @param groupType
    * @return TaskList
    * @roseuid 4C19C97D0148
    */
   public List getHisTaskListByGroup(int userID, int groupType) 
   {
    return null;
   }
}
