package bitc.sz.wzEnv.security.dao;

import bitc.sz.wzEnv.query.BaseDAO;
import bitc.sz.wzEnv.security.model.UserImpl;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public class AuthDAO extends BaseDAO {

	public UserImpl getUser(String userCode, String password) {
		String hql = "from UserImpl u where u.name=? and u.password=?";
		return (UserImpl) getSessionFactory().getCurrentSession()
				.createQuery(hql).setString(0, userCode).setString(1, password)
				.uniqueResult();
	}

	public UserImpl getUserByUserName(String userCode) {
		String hql = "from UserImpl u where u.name=?";
		return (UserImpl) getSessionFactory().getCurrentSession()
				.createQuery(hql).setString(0, userCode).uniqueResult();
	}
}
