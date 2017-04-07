package com.bitc.jk.xmis.workflow.dao;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.springframework.orm.hibernate3.HibernateTemplate;

import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;
import com.bitc.jk.xmis.workflow.model.WfCalendar;

public class WfCalendarDAO {
	private HibernateTemplate ht;

	private static final String format = "yyyyMMdd";

	/**
	 * @param date
	 *            以8字符形式表示的日期，如19861008
	 * @return
	 * @throws WorkflowException
	 */
	public Boolean isBusinessDay(String date) {
		WfCalendar cal = (WfCalendar) ht
				.findByNamedQuery("findCalByDate", date);
		if (cal == null) {
			return null;
		}
		return cal.getType() == WfCalendar.TYPE_WEEKDAYS;
	}

	public Boolean isBusinessDay(Date date) {
		String s = DateUtil.formatDate(date, format);
		return isBusinessDay(s);
	}

	/**
	 * @param year
	 * @param month
	 *            1-12
	 * @return
	 */
	@SuppressWarnings("unchecked")
	public List<WfCalendar> getWfCalendar(int year, int month) {
		StringBuilder base = new StringBuilder().append(year).append(
				StringUtils.leftPad(String.valueOf(month), 2, '0'));
		String min = base.toString() + "00";
		String max = base.toString() + "32";
		List<WfCalendar> list = ht
				.find("from WfCalendar c where c.date between ? and ? order by c.date",
						new Object[] { min, max });
		if (list.size() > 0) {
			return list;
		} else {
			return generateWfCalendar(year, month);
		}
	}

	/**
	 * @param year
	 * @param month
	 *            1-12
	 * @return
	 */
	private List<WfCalendar> generateWfCalendar(int year, int month) {
		List<WfCalendar> list = new ArrayList<WfCalendar>();
		Calendar calendar = Calendar.getInstance();
		calendar.set(Calendar.YEAR, year);
		calendar.set(Calendar.MONTH, month - 1);
		int maxDay = calendar.getActualMaximum(Calendar.DAY_OF_MONTH);
		StringBuilder base = new StringBuilder();
		WfCalendar wfCalendar;
		for (int i = 1; i <= maxDay; i++) {
			base.delete(0, base.length());
			calendar.set(Calendar.DAY_OF_MONTH, i);
			String date = base.append(year)
					.append(StringUtils.leftPad(String.valueOf(month), 2, '0'))
					.append(StringUtils.leftPad(String.valueOf(i), 2, '0'))
					.toString();
			wfCalendar = new WfCalendar();
			wfCalendar.setDate(date);
			int dayOfWeek = calendar.get(Calendar.DAY_OF_WEEK);
			int type = (dayOfWeek == Calendar.SATURDAY || dayOfWeek == Calendar.SUNDAY) ? WfCalendar.TYPE_WEEKEDNS
					: WfCalendar.TYPE_WEEKDAYS;
			wfCalendar.setType(type);
			wfCalendar.setDayOfWeek(dayOfWeek);
			saveWfCalendar(wfCalendar);
			list.add(wfCalendar);
		}
		return list;
	}

	public void saveWfCalendar(WfCalendar wfCalendar) {
		ht.save(wfCalendar);
	}

	public HibernateTemplate getHt() {
		return ht;
	}

	public void setHt(HibernateTemplate ht) {
		this.ht = ht;
	}

	public static void main(String[] args) {
		Calendar calendar = Calendar.getInstance();
		calendar.set(Calendar.MONTH, 1);
		System.out.println(calendar.getActualMaximum(Calendar.DAY_OF_MONTH));
		int maxDay = calendar.getActualMaximum(Calendar.DAY_OF_MONTH);
		StringBuilder base = new StringBuilder();
		for (int i = 1; i <= maxDay; i++) {
			base.delete(0, base.length());
			String date = base.append("2011")
					.append(StringUtils.leftPad(String.valueOf(1), 2, '0'))
					.append(StringUtils.leftPad(String.valueOf(i), 2, '0'))
					.toString();
			System.out.println(date);

		}
	}
	
	public List<WfCalendar> getWfCalendarBeforeDate(String onedate) {
		String hql = "from WfCalendar where date<=? AND type=0 order by date desc";
		return ht.find(hql, new Object[]{onedate});
	}
}
