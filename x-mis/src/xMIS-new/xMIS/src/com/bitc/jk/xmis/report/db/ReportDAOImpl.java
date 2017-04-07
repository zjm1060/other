package com.bitc.jk.xmis.report.db;

import java.util.List;

import org.hibernate.criterion.DetachedCriteria;
import org.springframework.orm.hibernate3.HibernateTemplate;

import com.bitc.jk.xmis.report.model.DBConnect;
import com.bitc.jk.xmis.report.model.Report;

public class ReportDAOImpl {
	private HibernateTemplate ht;

	public HibernateTemplate getHt() {
		return ht;
	}

	public void setHt(HibernateTemplate ht) {
		this.ht = ht;
	}

	@SuppressWarnings("unchecked")
	public List<DBConnect> getDbConnects() {
		DetachedCriteria criteria = DetachedCriteria.forClass(DBConnect.class);
		return ht.findByCriteria(criteria);
	}

	public void saveReport(Report report) {
		ht.save(report);
	}

	@SuppressWarnings("rawtypes")
	public Report findReportByName(String token) {
		List list = ht
				.find("from Report r left join fetch r.columns where r.name=?",
						token);
		if (list != null && list.size() > 0) {
			return (Report) list.get(0);
		}
		return null;
	}
}
