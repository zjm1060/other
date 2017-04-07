package com.bitc.jk.xmis.workflow.dao;

import java.sql.SQLException;
import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.hibernate.Hibernate;
import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.criterion.DetachedCriteria;
import org.hibernate.criterion.Order;
import org.hibernate.criterion.Restrictions;
import org.springframework.orm.hibernate3.HibernateCallback;
import org.springframework.orm.hibernate3.HibernateTemplate;

import com.bitc.jk.xmis.dao.impl.BaseDAOImpl;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.SpringInit;
import com.bitc.jk.xmis.workflow.model.Todo;

public class TodoDAO extends BaseDAOImpl {

	public void saveTodo(Todo todo) {
		ht.save(todo);
	}
	public void saveOrUpadateTodo(Todo todo) {
		ht.saveOrUpdate(todo);
	}

	@SuppressWarnings("unchecked")
	public List<Todo> findTodoByUserId(String userId) {
		return ht.findByNamedQuery("findTodoByUserId", userId);
	}

	@SuppressWarnings("unchecked")
	public Todo findTodoByAppId(String model, String appId) {
		List<Todo> list = ht.find("from Todo t where t.model=? and t.appId=?",
				new Object[] { model, appId });
		if (list != null && list.size() > 0) {
			return list.get(0);
		}
		return null;
	}

	//2013-11-06提交
	public Todo findTodoById(String id) {
		return (Todo) ht.get(Todo.class, id);
	}

	public void deleteTodo(final String id) {
		ht.executeWithNativeSession(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				session.createQuery("delete Todo t where t.id=?")
						.setString(0, id).executeUpdate();
				return null;
			}
		});
	}

	public void deleteTodoByAppId(final String model, final String appId) {
		ht.executeWithNativeSession(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				session.createQuery(
						"delete Todo t where t.model=? and t.appId=?")
						.setString(0, model).setString(1, appId)
						.executeUpdate();
				return null;
			}
		});
	}

	public HibernateTemplate getHt() {
		return ht;
	}

	public void setHt(HibernateTemplate ht) {
		this.ht = ht;
	}

	@SuppressWarnings("unchecked")
	public List<Todo> findTodoByUserId(String userId, int firstResult,
			int maxResults) {
		DetachedCriteria criteria = DetachedCriteria.forClass(Todo.class);
		criteria.add(Restrictions.eq("userID", userId))
				.add(Restrictions.lt("state", 10))
				.addOrder(Order.asc("priority"))
				.addOrder(Order.desc("createTime"));
		return ht.findByCriteria(criteria, firstResult, maxResults);
	}

	@SuppressWarnings("unchecked")
	public List<Todo> findTodoByQuery(StandardQuery query, PageInfo pageInfo) {
		if(ht == null){
			ht = (HibernateTemplate)SpringInit.getBean("ht");
			if(ht == null){
				return null;
			}
		}
		DetachedCriteria criteria = this.buildCriteria(query, Todo.class);
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
	
//		return getListStore(query, pageInfo, Todo.class);
	}

	@SuppressWarnings("unchecked")
	public List<Todo> getTodoListByTextAndUserID(final String text,final String userID) {
		return   ht.executeFind(new HibernateCallback() {
			
			public Object doInHibernate(Session session) throws HibernateException,
					SQLException {
				String hql = "from Todo t where t.text=? and t.userID=? and t.state=0 ";
				Query query = session.createQuery(hql).setString(0, text).setString(1, userID);
				return query.list() ;
			}
		});
	}

}
