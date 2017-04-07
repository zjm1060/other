package com.bitc.jk.xmis.task;

import java.util.Timer;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

public class TaskListener implements ServletContextListener{
	//在web.xml 配置TaskListener ServletContextListener是监听 Web 应用的生命周期的
	private static final Logger logger=Logger.getLogger(TaskListener.class);
	private Timer timer;

	public void contextDestroyed(ServletContextEvent arg0) {
		this.timer.cancel();
		if(logger.isInfoEnabled()){
			logger.info("Task Listener canceled.");
		}
	}

	//应用开时候调用
	public void contextInitialized(ServletContextEvent sce) {
		if(logger.isInfoEnabled()){
			logger.info("Task Listener starting.");
		}
		ApplicationContext context=WebApplicationContextUtils.getWebApplicationContext(sce.getServletContext());
		this.timer=new Timer();
		this.timer.schedule(new TimerTask(context),5*1000,1000*3600);//延时1秒后重复执行task,周期是1分钟
		this.timer.schedule(new WarnTask(context),10*1000,1000*3600*2);//延时10秒后重复执行task,周期是1天  考勤补报消息提醒
		if(logger.isInfoEnabled()){
			logger.info("Task Listener started.");
		}
	}

}
