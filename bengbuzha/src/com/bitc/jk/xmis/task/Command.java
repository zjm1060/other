package com.bitc.jk.xmis.task;

import org.springframework.context.ApplicationContext;

public interface Command {
	void execute(ApplicationContext context,Task task);
}
