package com.riversql.actions;

import com.bitc.exchange.util.UUIDUtils;
import com.riversql.JSONAction;
import com.riversql.entities.ImportResult;
import com.riversql.quartz.SchedulerManage;
import com.riversql.task.Command;
import com.riversql.task.CommandFactory;
import com.riversql.task.impl.MultiTask;
import org.apache.commons.io.FileUtils;
import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;
import org.quartz.SchedulerException;

import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import javax.persistence.Query;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.lang.reflect.Method;
import java.text.ParseException;
import java.util.List;

/**
 * Created by zsz on 2015/8/18.
 */
public class TaskAction implements JSONAction {
    private String method;
    private int taskId;
    private String taskName;
    private String taskTrigger;
    private int commandId;
    private int sortNo;
    private String commandClassName;
    private String commandCofig;
    @Override
    public JSONObject execute(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws Exception {
        Class c = this.getClass();
        Method m = c.getMethod(method, HttpServletRequest.class, HttpServletResponse.class, EntityManager.class, EntityTransaction.class);
        if (m != null) {
            return (JSONObject)m.invoke(this, request, response, em, et);
        }
        return null;
    }

    public JSONObject getTasks(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException {
        Query query =  em.createQuery("select o from MultiTask o ");
        List<MultiTask> list =  query.getResultList();
        JSONArray arr=new JSONArray();
        for(int i=0;i<list.size();i++){
            //arr.put(cols.get(i));
            JSONObject obj=new JSONObject();
             obj.put("id",(list.get(i)).getId());
            obj.put("name",(list.get(i)).getName());
            obj.put("cronTrigger",(list.get(i)).getCronTrigger());
            obj.put("state",(list.get(i)).getState());
            arr.put(obj);
        }
        JSONObject ret=new JSONObject();
        ret.put("tasks",arr);
        return ret;
    }
    public JSONObject getTaskDetail(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException {

        MultiTask multiTask = em.find(MultiTask.class,taskId);
        Query query1 =  em.createQuery("select o1 from CommandFactory o1 where o1.multiTask = ?1");
        query1.setParameter(1, multiTask);
        List<CommandFactory> list =  query1.getResultList();
        JSONArray arr=new JSONArray();
        for(int i=0;i<list.size();i++){
            JSONObject obj=new JSONObject();
            obj.put("id",(list.get(i)).getId());
            obj.put("className",(list.get(i)).getClassName());
            obj.put("config",(list.get(i)).getConfig());
            obj.put("sortNo",(list.get(i)).getSortNo());
            arr.put(obj);
        }
        JSONObject ret=new JSONObject();
        ret.put("taskDetail",arr);
        return ret;
    }

    public JSONObject getCommandDetail(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException {

        CommandFactory  commandFactory= em.find(CommandFactory.class,commandId);
        JSONArray arr=new JSONArray();
            JSONObject obj=new JSONObject();
            obj.put("id",commandFactory.getId());
            obj.put("className", commandFactory.getClassName());
            obj.put("config",commandFactory.getConfig());
            arr.put(obj);
        JSONObject ret=new JSONObject();
        ret.put("taskDetail",arr);
        return ret;
    }

    public JSONObject saveTaskInfo(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException, ParseException, SchedulerException {
        if(taskId > -1){
            MultiTask multiTask = em.find(MultiTask.class,taskId);
            multiTask.setName(taskName);
            multiTask.setCronTrigger(taskTrigger);
            em.merge(multiTask);
            if(multiTask.getState() == 1){
                SchedulerManage.modifyTrigger(multiTask);
            }
        }else{
            MultiTask multiTask = new MultiTask();
            multiTask.setName(taskName);
            multiTask.setCronTrigger(taskTrigger);
            em.persist(multiTask);
        }
        return null;
    }
    public JSONObject saveCommand(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException {
            if(commandId > -1){
                CommandFactory commandFactory = em.find(CommandFactory.class,commandId);
                commandFactory.setClassName(commandClassName);
                commandFactory.setSortNo(sortNo);
                em.merge(commandFactory);
            }else {
                MultiTask multiTask =em.find(MultiTask.class,taskId);
                CommandFactory commandFactory = new CommandFactory();
                commandFactory.setClassName(commandClassName);
                commandFactory.setSortNo(sortNo);
                multiTask.addCommand(commandFactory);
                em.merge(multiTask);
            }
        return null;
    }
    public JSONObject deleteTask(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException, SchedulerException {
            MultiTask multiTask = em.find(MultiTask.class,taskId);
            if(multiTask.getState() == 1){
                SchedulerManage.stopTask(multiTask);
            }
            em.remove(multiTask);
        return null;
    }
    public JSONObject deleteCommand(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException {
              CommandFactory commandFactory = em.find(CommandFactory.class,commandId);
            em.remove(commandFactory);
        return null;
    }
    public JSONObject getCommandConfig(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException, UnsupportedEncodingException {
        CommandFactory commandFactory = em.find(CommandFactory.class, commandId);
        if (commandFactory != null && commandFactory.getConfigXML() != null) {
            byte[] bytes = commandFactory.getConfigXML();
//                String xmlConfig = bytes.toString();
            StringBuffer sbuf = new StringBuffer();
            for (int i = 0; i < bytes.length; i++) {
                sbuf.append(bytes[i]);
            }
            String xmlConfig = new String(bytes, "GBK");
//                String xmlConfig = sbuf.toString();
            JSONObject ret = new JSONObject();
            ret.put("commandConfig", xmlConfig);
            return ret;
        }
        JSONObject ret = new JSONObject();
        ret.put("commandConfig", "");
        return ret;
    }
    public JSONObject saveCommandConfig(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException, IOException {
        if(commandCofig !=null && !"".equals(commandCofig)) {
            String fileName = UUIDUtils.nextCode();
            byte[] bytes = commandCofig.getBytes();
            CommandFactory commandFactory = em.find(CommandFactory.class,commandId);
            commandFactory.setConfig(fileName);
            commandFactory.setConfigXML(bytes);
            em.merge(commandFactory);
        }
        return  null;
    }
    public JSONObject startTask(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException, ParseException, SchedulerException {
        if(taskId>-1) {
            Query query = em.createQuery("select o from MultiTask o where o.id = ?1");
            query.setParameter(1, taskId);
            MultiTask multiTask = (MultiTask) query.getSingleResult();
            multiTask.setState(1);
            SchedulerManage.addTask(multiTask);
        }
        return null;
    }
    public JSONObject stopTask(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException, SchedulerException {
        if(taskId>-1) {
            Query query = em.createQuery("select o from MultiTask o where o.id = ?1");
            query.setParameter(1, taskId);
            MultiTask multiTask = (MultiTask) query.getSingleResult();
            multiTask.setState(0);
            SchedulerManage.stopTask(multiTask);
        }
        return null;
    }
    public JSONObject getImportLog(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws JSONException {
        Query query =  em.createQuery("select o from ImportResult o order by o.logDate desc");
        List<ImportResult> list =  query.getResultList();
        JSONArray arr=new JSONArray();
        for(int i=0;i<list.size();i++){
            //arr.put(cols.get(i));
            JSONObject obj=new JSONObject();
            obj.put("id",(list.get(i)).getId());
            obj.put("fileName",(list.get(i)).getFileName());
            obj.put("tableNames",(list.get(i)).getTableNames());
            obj.put("state",(list.get(i)).getResult());
            obj.put("logDate",(list.get(i)).getLogDate());
            arr.put(obj);
        }
        JSONObject ret=new JSONObject();
        ret.put("importLog",arr);
        return ret;
    }
    public String getMethod() {
        return method;
    }

    public void setMethod(String method) {
        this.method = method;
    }

    public int getCommandId() {
        return commandId;
    }

    public void setCommandId(int commandId) {
        this.commandId = commandId;
    }

    public int getTaskId() {
        return taskId;
    }

    public void setTaskId(int taskId) {
        this.taskId = taskId;
    }

    public String getTaskName() {
        return taskName;
    }

    public void setTaskName(String taskName) {
        this.taskName = taskName;
    }

    public String getTaskTrigger() {
        return taskTrigger;
    }

    public void setTaskTrigger(String taskTrigger) {
        this.taskTrigger = taskTrigger;
    }

    public int getSortNo() {
        return sortNo;
    }

    public void setSortNo(int sortNo) {
        this.sortNo = sortNo;
    }

    public String getCommandClassName() {
        return commandClassName;
    }

    public void setCommandClassName(String commandClassName) {
        this.commandClassName = commandClassName;
    }

    public String getCommandCofig() {
        return commandCofig;
    }

    public void setCommandCofig(String commandCofig) {
        this.commandCofig = commandCofig;
    }
}