package bitc.sz.wzEnv.security.service;

import bitc.sz.wzEnv.security.model.UserImpl;

public interface IAuthService{

	public abstract UserImpl getUserByUserName(String userCode);
}