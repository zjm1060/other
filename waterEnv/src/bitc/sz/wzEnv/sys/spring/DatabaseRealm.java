package bitc.sz.wzEnv.sys.spring;

import bitc.sz.wzEnv.security.dao.AuthDAO;
import bitc.sz.wzEnv.security.model.IUser;
import bitc.sz.wzEnv.security.model.Role;
import bitc.sz.wzEnv.security.model.UserImpl;
import org.apache.shiro.authc.*;
import org.apache.shiro.authz.AuthorizationInfo;
import org.apache.shiro.authz.SimpleAuthorizationInfo;
import org.apache.shiro.realm.AuthorizingRealm;
import org.apache.shiro.subject.PrincipalCollection;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * Created by zsz on 2015/10/12.
 */
public class DatabaseRealm extends AuthorizingRealm {
	@Autowired
	private AuthDAO authDAO;

	@Override
	protected AuthorizationInfo doGetAuthorizationInfo(
			PrincipalCollection principals) {
		IUser user = (IUser) principals.getPrimaryPrincipal();
		SimpleAuthorizationInfo info = new SimpleAuthorizationInfo();
		for (Role role : user.getRoles()) {
			info.addRole(role.getC_id());
		}
		return info;
	}

	@Override
	protected AuthenticationInfo doGetAuthenticationInfo(
			AuthenticationToken authcToken) throws AuthenticationException {
		UsernamePasswordToken token = (UsernamePasswordToken) authcToken;
		UserImpl user = authDAO.getUserByUserName(
				token.getUsername());
		if (user == null) {
			throw new UnknownAccountException();
		}
		if (user.isLocked()) {
			throw new LockedAccountException();
		}
		return new SimpleAuthenticationInfo(user, user.getPassword(), getName());
	}
}
