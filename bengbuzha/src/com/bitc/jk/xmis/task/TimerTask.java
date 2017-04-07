package com.bitc.jk.xmis.task;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;

public class TimerTask extends java.util.TimerTask{
	private static final Logger logger=Logger.getLogger(TimerTask.class);
	private ApplicationContext context;
	
	public TimerTask(ApplicationContext context) {
		super();
		this.context = context;
	}

	public ApplicationContext getContext() {
		return context;
	}

	public void setContext(ApplicationContext context) {
		this.context = context;
	}

	public void run() {
		TaskDAO taskDAO=(TaskDAO) context.getBean("taskDAO");
		List<Task> tasks = taskDAO.getAllActiveTask();
		for (Task task : tasks) {
			try {
				//执行任务
				Object object=context.getBean(task.getCommandBeanName());
				if(object==null ||!(object instanceof Command)){
					throw new RuntimeException("命令Bean错误！");
				}
				Command command=(Command) object;
				command.execute(context, task);
				task.setState(1);
				taskDAO.saveOrUpdateObject(task);
			} catch (Exception e) {
				logger.error(e.getMessage(),e);
				task.setState(-1);//出错任务状态-1
				taskDAO.saveOrUpdateObject(task);
			}

		}
	}
}
