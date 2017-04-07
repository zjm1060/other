package com.bitc.jk.xmis.dao.impl;

import java.io.Serializable;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.hibernate.criterion.DetachedCriteria;
import org.hibernate.criterion.Order;
import org.hibernate.criterion.Projections;
import org.springframework.orm.hibernate3.HibernateCallback;
import org.springframework.orm.hibernate3.HibernateTemplate;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

@SuppressWarnings("unchecked")
public class BaseDAOImpl {

	public static Logger logger = Logger.getLogger(BaseDAOImpl.class);

	protected HibernateTemplate ht;

	public <T> T load(Class<T> cls, Serializable id) {
		return (T) ht.load(cls, id);
	}

	public <T> T get(Class<T> cls, Serializable id) {
		return (T) ht.get(cls, id);
	}

	public <T> List<T> findObjsByParam(T o) {
		return findObjsByParam(o, -1, -1);
	}

	public <T> List<T> findObjsByParam(T o, int firstResult, int maxResults) {
		return ht.findByExample(o, firstResult, maxResults);
	}

	public void saveObject(Object o) {
		ht.save(o);
	}

	public void saveOrUpdateObject(Object o) {
		ht.saveOrUpdate(o);
	}

	public void upldateObject(Object o) {
		ht.clear();
		ht.update(o);
	}

	public void deleteObject(Object o) {
		ht.delete(o);
	}

	public int getTotalCount(final List<QueryParam> params, final Class<?> clas) {
		final DetachedCriteria criteria = buildCriteria(params, clas);
		criteria.setProjection(Projections.rowCount());
		Integer count = (Integer) ht
				.executeWithNativeSession(new HibernateCallback() {
					public Object doInHibernate(Session session)
							throws HibernateException, SQLException {
						return criteria.getExecutableCriteria(session)
								.uniqueResult();
					}
				});
		return count.intValue();
	}

	public int getTotalCount(final StandardQuery query, final Class<?> clas) {
		final DetachedCriteria criteria = buildCriteria(query, clas);
		criteria.setProjection(Projections.rowCount());
		Integer count = (Integer) ht
				.executeWithNativeSession(new HibernateCallback() {
					public Object doInHibernate(Session session)
							throws HibernateException, SQLException {
						return criteria.getExecutableCriteria(session)
								.uniqueResult();
					}
				});
		return count.intValue();
	}

	public int getTotalCountByInkling(final List<QueryParam> params,
			final Class<?> clas) {
		final DetachedCriteria criteria = buildCriteria(params, clas);
		criteria.setProjection(Projections.rowCount());
		Integer count = (Integer) ht
				.executeWithNativeSession(new HibernateCallback() {
					public Object doInHibernate(Session session)
							throws HibernateException, SQLException {
						return criteria.getExecutableCriteria(session)
								.uniqueResult();
					}
				});
		return count.intValue();
	}

	public <T> List<T> getListStore(List<QueryParam> params, PageInfo pageInfo,
			Class<T> clas) {
		DetachedCriteria criteria = this.buildCriteria(params, clas);
		if (!StringUtils.isEmpty(pageInfo.getSort())
				&& !StringUtils.isEmpty(pageInfo.getDir())) {
			criteria.addOrder("ASC".equals(pageInfo.getDir()) ? Order
					.asc(pageInfo.getSort()) : Order.desc(pageInfo.getSort()));
		}
		int firstResult = pageInfo.getStart();
		int maxResults = pageInfo.getLimit();
		if (maxResults == 0) {
			return ht.findByCriteria(criteria);
		} else {
			return ht.findByCriteria(criteria, firstResult, maxResults);
		}
	}

	public <T> List<T> getListStore(StandardQuery query, Class<T> clas) {
		return getListStore(query, new PageInfo(), clas);
	}

	public <T> List<T> getListStore(StandardQuery query, PageInfo pageInfo,
			Class<T> clas) {
		DetachedCriteria criteria = this.buildCriteria(query, clas);
		if (!StringUtils.isEmpty(pageInfo.getSort())
				&& !StringUtils.isEmpty(pageInfo.getDir())) {
			criteria.addOrder(PageInfo.ASC.equals(pageInfo.getDir()) ? Order
					.asc(pageInfo.getSort()) : Order.desc(pageInfo.getSort()));
		}
		int firstResult = pageInfo.getStart();
		int maxResults = pageInfo.getLimit();
		if (maxResults == 0) {
			return ht.findByCriteria(criteria);
		} else {
			return ht.findByCriteria(criteria, firstResult, maxResults);
		}
	}

	public <T> List<T> getListStoreByInkling(List<QueryParam> params,
			PageInfo pageInfo, Class<T> clas) {
		DetachedCriteria criteria = this.buildCriteriaByInkling(params, clas);
		if (!StringUtils.isEmpty(pageInfo.getSort())
				&& !StringUtils.isEmpty(pageInfo.getDir())) {
			criteria.addOrder("ASC".equals(pageInfo.getDir()) ? Order
					.asc(pageInfo.getSort()) : Order.desc(pageInfo.getSort()));
		}
		int firstResult = pageInfo.getStart();
		int maxResults = pageInfo.getLimit();
		if (maxResults == 0) {
			return ht.findByCriteria(criteria);
		} else {
			return ht.findByCriteria(criteria, firstResult, maxResults);
		}
	}

	public DetachedCriteria buildCriteria(List<QueryParam> params, Class<?> clas) {

		DetachedCriteria criteria = DetachedCriteria.forClass(clas);
		for (Iterator<QueryParam> it = params.iterator(); it.hasNext();) {
			criteria = it.next().addToDetachedCriteria(criteria);
		}
		return criteria;
	}

	public DetachedCriteria buildCriteria(StandardQuery query, Class<?> clas) {
		DetachedCriteria criteria = DetachedCriteria.forClass(clas);
		if (query != null) {
			HashMap<String, DetachedCriteria> aliasSet= new HashMap<String, DetachedCriteria>();
			query.addToDetachedCriteria(criteria, aliasSet);
		}
		return criteria;
	}

	public DetachedCriteria buildCriteriaByInkling(List<QueryParam> params,
			Class<?> clas) {
//		logger.info("buildCriteriaByInkling");
		DetachedCriteria criteria = DetachedCriteria.forClass(clas);
		criteria.add(QueryParam.getRestrictionsOr(params));
		return criteria;
	}

	public HibernateTemplate getHt() {
		return ht;
	}

	public void setHt(HibernateTemplate ht) {
		this.ht = ht;
	}
}
