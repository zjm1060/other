package com.bitc.jk.xmis.workflow.fire;

import java.util.Date;

import org.fireflow.engine.calendar.DefaultCalendarService;

import com.bitc.jk.xmis.workflow.dao.WfCalendarDAO;

public class CalendarService extends DefaultCalendarService {
	private WfCalendarDAO wfCalendarDAO;

	@Override
	public boolean isBusinessDay(Date d) {
		Boolean b = wfCalendarDAO.isBusinessDay(d);
		if (b == null) {
			return super.isBusinessDay(d);
		}
		return b;
	}

	public WfCalendarDAO getWfCalendarDAO() {
		return wfCalendarDAO;
	}

	public void setWfCalendarDAO(WfCalendarDAO wfCalendarDAO) {
		this.wfCalendarDAO = wfCalendarDAO;
	}

}
