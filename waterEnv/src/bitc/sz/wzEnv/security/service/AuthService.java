package bitc.sz.wzEnv.security.service;

import bitc.sz.wzEnv.security.dao.AuthDAO;
import bitc.sz.wzEnv.security.model.UserImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import javax.security.sasl.AuthenticationException;
import java.util.List;

@Service
@Transactional(rollbackFor = Exception.class)
public class AuthService implements IAuthService {
	@Autowired
	private AuthDAO authDAO;

	@Transactional(propagation = Propagation.NOT_SUPPORTED, readOnly = true)
	public UserImpl getUserByUserName(String userCode) {
		return authDAO.getUserByUserName(userCode);
	}
}
