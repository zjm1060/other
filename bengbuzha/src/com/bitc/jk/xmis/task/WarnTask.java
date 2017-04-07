package com.bitc.jk.xmis.task;

import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import com.bitc.jk.xmis.service.MissAttendanceService;

public class WarnTask extends java.util.TimerTask{
	private static final Logger logger=Logger.getLogger(WarnTask.class);
	private ApplicationContext context;
	
	public WarnTask(ApplicationContext context) {
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
		try {
			MissAttendanceService missAttenceService =(MissAttendanceService) context.getBean("missAttenceService");
			missAttenceService.doWarnInfo();
		} catch (Exception e) {
			logger.info(e);
		}
	}
}
