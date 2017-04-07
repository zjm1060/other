package com.riversql.quartz;

import com.riversql.task.Task;
import org.quartz.Job;
import org.quartz.JobExecutionContext;
import org.quartz.JobExecutionException;

/**
 * Created by zsz on 2015/8/18.
 */
public class DefaultJob implements Job {
    @Override
    public void execute(JobExecutionContext jobExecutionContext) throws JobExecutionException {
        Task task = (Task) jobExecutionContext.getJobDetail().getJobDataMap().get("bean");
        task.execute();
    }
}
