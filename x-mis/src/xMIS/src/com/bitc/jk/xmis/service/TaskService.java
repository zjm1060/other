//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\TaskService.java

package com.bitc.jk.xmis.service;

import java.sql.Date;
import java.util.List;

import com.bitc.jk.xmis.model.TaskObj;

public interface TaskService 
{
   
   /**
    * @param userID
    * @return List
    * @roseuid 4C19C1DE00EA
    */
   public List getNewTaskListByUser(int userID);
   
   /**
    * @param userID
    * @param taskID
    * @return com.bitc.jk.xmis.model.TaskObj
    * @roseuid 4C19C319007D
    */
   public TaskObj getTaskInfoByID(int userID, int taskID);
   
   /**
    * @param userID
    * @param date
    * @return List
    * @roseuid 4C19C34B002E
    */
   public List getNewTaskListByDate(int userID, Date date);
   
   /**
    * @param userID
    * @param groupType
    * @return List
    * @roseuid 4C19C4610119
    */
   public List getNewTaskListByGroup(int userID, int groupType);
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C4C402AF
    */
   public void doReceive(int userID, int taskID);
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C4F100AB
    */
   public void doReject(int userID, int taskID);
   
   /**
    * @param userID
    * @return List
    * @roseuid 4C19C7390148
    */
   public List getMyTaskListByUser(int userID);
   
   /**
    * @param userID
    * @param date
    * @return List
    * @roseuid 4C19C7560196
    */
   public List getMyTaskListByDate(int userID, Date date);
   
   /**
    * @param userID
    * @param groupType
    * @return List
    * @roseuid 4C19C7C20000
    */
   public List getMyTaskListByGroup(int userID, int groupType);
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C7DF0271
    */
   public void doSave(int userID, int taskID);
   
   /**
    * @param userID
    * @roseuid 4C19C7F40280
    */
   public void doForward(int userID);
   
   /**
    * @param userID
    * @return List
    * @roseuid 4C19C897033C
    */
   public List getHisTaskListByUser(int userID);
   
   /**
    * @param userID
    * @param date
    * @return List
    * @roseuid 4C19C8B1000F
    */
   public List getHisTaskListByDate(int userID, Date date);
   
   /**
    * @param userID
    * @param groupType
    * @return List
    * @roseuid 4C19C8D400BB
    */
   public List getHisTaskListByGroup(int userID, int groupType);
}
