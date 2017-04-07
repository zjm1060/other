package com.riversql.quartz;

import com.riversql.task.Task;
import org.quartz.*;
import org.quartz.impl.StdSchedulerFactory;

import java.text.ParseException;
import java.util.Date;


/**
 * Created by zsz on 2015/8/18.
 */
public class SchedulerManage {

    public static void startTask(Task task) throws SchedulerException, ParseException {
        Scheduler sch = StdSchedulerFactory.getDefaultScheduler();
        JobDetail jobDetail = new JobDetail(task.getName(), "DEFAULT", DefaultJob.class);
        jobDetail.getJobDataMap().put("bean", task);
        //创建一个Trigger
        CronTrigger cronTrigger = new CronTrigger(task.getName() + "_TRIGGER", task.getName() + "_TRIGGER_GROUP");
        CronExpression cexp = new CronExpression(task.getCronTrigger());
        cronTrigger.setCronExpression(cexp);
        sch.scheduleJob(jobDetail, cronTrigger);
        if (!sch.isStarted()) {
            sch.start();
        }
    }

//    public static void startUp(String triggerName, String triggerGroupName, Class Job, String time) throws SchedulerException, ParseException {
//        Scheduler sch = StdSchedulerFactory.getDefaultScheduler();
//        //创建一个JobDetail
//        JobDetail jobDetail = new JobDetail("jobDetail-s1", "jobDetailGroup-s1", Job);
//        //创建一个Trigger
//        CronTrigger cronTrigger = new CronTrigger(triggerName, triggerGroupName);
//        CronExpression cexp = new CronExpression(time);
//        cronTrigger.setCronExpression(cexp);
//        sch.scheduleJob(jobDetail, cronTrigger);
//        if (!sch.isStarted()) {
//            sch.start();
//        }
//    }

//    public static void modifyTrigger(String triggerName, String triggerGroupName, String time)
//            throws SchedulerException, ParseException {
//        try {
//            Scheduler scheduler = StdSchedulerFactory.getDefaultScheduler();
//            CronTrigger cronTrigger = (CronTrigger) scheduler.getTrigger(triggerName, triggerGroupName);
//            if (null != cronTrigger) {
//                cronTrigger.setJobName(cronTrigger.getJobName());
//                cronTrigger.setCronExpression(new CronExpression(time));
//                cronTrigger.setStartTime(new Date());
//                scheduler.rescheduleJob(cronTrigger.getName(), triggerGroupName, cronTrigger);
//            }
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//    }

    public static void modifyTrigger(Task task)
            throws SchedulerException, ParseException {
        try {
            Scheduler scheduler = StdSchedulerFactory.getDefaultScheduler();
            CronTrigger cronTrigger = (CronTrigger) scheduler.getTrigger(task.getName()+"_TRIGGER",task.getName()+"_TRIGGER_GROUP");
            if (null != cronTrigger) {
                cronTrigger.setJobName(cronTrigger.getJobName());
                cronTrigger.setCronExpression(new CronExpression(task.getCronTrigger()));
                cronTrigger.setStartTime(new Date());
                scheduler.rescheduleJob(cronTrigger.getName(), task.getName()+"_TRIGGER_GROUP", cronTrigger);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

//    public static void addJob(String triggerName, String triggerGroupName, Class Job, String time)
//            throws SchedulerException, ParseException {
//        Scheduler sch = StdSchedulerFactory.getDefaultScheduler();
//        //创建一个JobDetail
//        JobDetail jobDetail = new JobDetail("jobDetail-s2", "jobDetailGroup-s2", Job);
//        CronTrigger cronTrigger = new CronTrigger(triggerName, triggerGroupName);
//        CronExpression cexp = new CronExpression(time);
//        cronTrigger.setCronExpression(cexp);
//        sch.scheduleJob(jobDetail, cronTrigger);
//    }
    public static void addTask(Task task) throws SchedulerException, ParseException {
        Scheduler sch = StdSchedulerFactory.getDefaultScheduler();
        JobDetail jobDetail = new JobDetail(task.getName(), "DEFAULT", DefaultJob.class);
        jobDetail.getJobDataMap().put("bean", task);
        //创建一个Trigger
        CronTrigger cronTrigger = new CronTrigger(task.getName() + "_TRIGGER", task.getName() + "_TRIGGER_GROUP");
        CronExpression cexp = new CronExpression(task.getCronTrigger());
        cronTrigger.setCronExpression(cexp);
        sch.scheduleJob(jobDetail, cronTrigger);
        if (!sch.isStarted()) {
            sch.start();
        }
    }
//    public static void pauseTask(String triggerName, String triggerGroupName) throws SchedulerException {
//        Scheduler sch = StdSchedulerFactory.getDefaultScheduler();
//        sch.pauseTrigger(triggerName, triggerGroupName);
//    }
    public static void pauseTask(Task task) throws SchedulerException {
        Scheduler sch = StdSchedulerFactory.getDefaultScheduler();
        sch.pauseTrigger(task.getName() + "_TRIGGER", task.getName() + "_TRIGGER_GROUP");
    }
    public static void stopTask(Task task)throws SchedulerException {
        Scheduler sch = StdSchedulerFactory.getDefaultScheduler();
        sch.unscheduleJob(task.getName() + "_TRIGGER", task.getName() + "_TRIGGER_GROUP");
    }
    public static void start() throws SchedulerException {
        Scheduler sch = StdSchedulerFactory.getDefaultScheduler();
        if (!sch.isStarted()) {
            sch.start();
        }
    }
    public static void stop()throws SchedulerException {
        Scheduler sch = StdSchedulerFactory.getDefaultScheduler();
        if (!sch.isShutdown()) {
            sch.shutdown();
        }
    }
}
