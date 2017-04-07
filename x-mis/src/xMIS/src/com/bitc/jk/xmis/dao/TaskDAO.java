//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\TaskDAO.java

package com.bitc.jk.xmis.dao;

import java.sql.Date;
import java.util.List;

import com.bitc.jk.xmis.model.TaskObj;

public interface TaskDAO 
{
   
   /**
    * @param userID
    * @param taskType - 1 - new task; 2 - my task; 3 - historical task forwarded to 
    * others but without finished; 4 - historical task with finished; 0 - all 
    * (1+2+3+4)
    * @return List
    * @roseuid 4C19C2B10280
    */
   public List getTaskListByUser(int userID, int taskType);
   
   /**
    * @param userID
    * @param taskID
    * @return com.bitc.jk.xmis.model.TaskObj
    * @roseuid 4C19C2D102AF
    */
   public TaskObj getTaskInfoByID(int userID, int taskID);
   
   /**
    * @param userID
    * @param date
    * @param taskType
    * @return List
    * @roseuid 4C19C365006D
    */
   public List getTaskListByDate(int userID, Date date, int taskType);
   
   /**
    * @param userID
    * @param groupType - groupType based on TaskGrouopType table
    * sample:
    * 1 - task with project
    * 2 - task with contract
    * 3 - task with meeting
    * .........
    * @param taskType
    * @return List
    * @roseuid 4C19C4970232
    */
   public List getTaskListByGroup(int userID, int groupType, int taskType);
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C5700138
    */
   public void doReceive(int userID, int taskID);
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C58702FD
    */
   public void doReject(int userID, int taskID);
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C80B037A
    */
   public void doSave(int userID, int taskID);
   
   /**
    * @param userID
    * @param taskID
    * @roseuid 4C19C81B0290
    */
   public void doForward(int userID, int taskID);
}
