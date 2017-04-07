package bitc.sz.wzEnv.query;

import java.io.Serializable;
import java.util.HashMap;
import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.hibernate.Criteria;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.criterion.DetachedCriteria;
import org.hibernate.criterion.Example;
import org.hibernate.criterion.Projections;
import org.hibernate.sql.JoinType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;


@Repository
@SuppressWarnings("unchecked")
public class BaseDAO {
	@Autowired
	protected SessionFactory sessionFactory;

	public long getCountByDetachedCriteria(DetachedCriteria criteria) {
		return (Long) criteria.getExecutableCriteria(
				getSessionFactory().getCurrentSession()).uniqueResult();
	}

	public <T> List<T> findByDetachedCriteria(DetachedCriteria criteria) {
		return criteria.getExecutableCriteria(
				getSessionFactory().getCurrentSession()).list();
	}

	public <T> T load(Class<T> clazz, Serializable id) {
		return (T) sessionFactory.getCurrentSession().load(clazz, id);
	}

	public <T> T get(Class<T> clazz, Serializable id) {
		return (T) sessionFactory.getCurrentSession().get(clazz, id);
	}

	public void saveOrUpdate(Object object) {
		sessionFactory.getCurrentSession().saveOrUpdate(object);
	}

	public <T> List<T> findAll(Class<T> clazz) {
		return (List<T>) sessionFactory.getCurrentSession()
				.createCriteria(clazz).list();
	}

	public <T> List<T> findAll(Class<T> clazz, bitc.sz.wzEnv.query.PagingInfo pageInfo) {
		Criteria criteria = sessionFactory.getCurrentSession().createCriteria(
				clazz);
		if (pageInfo != null && pageInfo.isPaging()) {
			criteria.setFirstResult(pageInfo.getFirst()).setMaxResults(
					pageInfo.getRows());
		}
		if (pageInfo != null && pageInfo.getSort() != null) {
			criteria.addOrder(pageInfo.buildOrder());
		}
		return (List<T>) criteria.list();
	}

	public <T> List<T> findAll(Class<T> clazz, Object example) {
		return (List<T>) sessionFactory.getCurrentSession()
				.createCriteria(clazz).add(Example.create(example)).list();
	}

	public <T> List<T> findAll(Class<T> clazz, Object example, PagingInfo pageInfo) {
		Criteria criteria = sessionFactory.getCurrentSession()
				.createCriteria(clazz).add(Example.create(example));
		if (pageInfo != null && pageInfo.isPaging()) {
			criteria.setFirstResult(pageInfo.getFirst()).setMaxResults(
					pageInfo.getRows());
		}
		if (pageInfo != null && pageInfo.getSort() != null) {
			criteria.addOrder(pageInfo.buildOrder());
		}
		return (List<T>) criteria.list();
	}

	public <T> Long findAllCount(Class<T> clazz) {
		return (Long) sessionFactory.getCurrentSession().createCriteria(clazz)
				.setProjection(Projections.rowCount()).uniqueResult();
	}

	public <T> Long findAllCount(Class<T> clazz, Object example) {
		return (Long) sessionFactory.getCurrentSession().createCriteria(clazz)
				.add(Example.create(example))
				.setProjection(Projections.rowCount()).uniqueResult();
	}

	public <T> List<T> findAll(DetachedCriteria criteria, Class<T> clazz) {
		return criteria.getExecutableCriteria(
				getSessionFactory().getCurrentSession()).list();
	}

	public void delete(Object object) {
		getSessionFactory().getCurrentSession().delete(object);
	}

	public Long getTotalCount(IStandardQuery query, Class<?> clas) {
		final DetachedCriteria criteria = buildCriteria(query, clas);
		criteria.setProjection(Projections.rowCount());
		Session session = getSessionFactory().getCurrentSession();
		return (Long) criteria.getExecutableCriteria(session).uniqueResult();
	}

	public <T> List<T> query(IStandardQuery query, Class<T> clas) {
		return query(query, null, clas);
	}

	public <T> List<T> query(IStandardQuery query, PagingInfo pageInfo,
			Class<T> clas) {
		HashMap<String, DetachedCriteria> aliasSet = new HashMap<String, DetachedCriteria>();
		DetachedCriteria dCriteria = this.buildCriteria(query, clas, aliasSet);
		if (pageInfo != null && !StringUtils.isEmpty(pageInfo.getSort())
				&& !StringUtils.isEmpty(pageInfo.getSort())) {
			if (pageInfo.getSort().indexOf('.') >= 0) {
				int index = pageInfo.getSort().lastIndexOf('.');
				String aliasKey = pageInfo.getSort().substring(0, index);
				if (!aliasSet.containsKey(aliasKey)) {
					String[] keys = StringUtils.split(aliasKey, ',');
					String alias = keys[0];
					dCriteria.createCriteria(alias, alias,
							JoinType.LEFT_OUTER_JOIN);
					for (int i = 1; i < keys.length - 1; i++) {
						alias += keys[i];
					}
				}
			}
			dCriteria.addOrder(pageInfo.buildOrder());
		}
		Criteria criteria = dCriteria.getExecutableCriteria(getSessionFactory()
				.getCurrentSession());
		if (pageInfo != null && pageInfo.isPaging()) {
			criteria.setFirstResult(pageInfo.getFirst()).setMaxResults(
					pageInfo.getRows());
		}
		return criteria.list();
	}

	public DetachedCriteria buildCriteria(IStandardQuery query, Class<?> clas) {
		DetachedCriteria criteria = DetachedCriteria.forClass(clas);
		if (query != null) {
			HashMap<String, DetachedCriteria> aliasSet = new HashMap<String, DetachedCriteria>();
			query.addToDetachedCriteria(criteria, aliasSet);
		}
		return criteria;
	}

	public DetachedCriteria buildCriteria(IStandardQuery query, Class<?> clas,
			HashMap<String, DetachedCriteria> aliasSet) {
		DetachedCriteria criteria = DetachedCriteria.forClass(clas);
		if (query != null) {
			if (aliasSet == null) {
				aliasSet = new HashMap<String, DetachedCriteria>();
			}
			query.addToDetachedCriteria(criteria, aliasSet);
		}
		return criteria;
	}

	public SessionFactory getSessionFactory() {
		return sessionFactory;
	}

	public void setSessionFactory(SessionFactory sessionFactory) {
		this.sessionFactory = sessionFactory;
	}
}
