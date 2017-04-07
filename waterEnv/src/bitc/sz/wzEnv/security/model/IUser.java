package bitc.sz.wzEnv.security.model;

import java.io.Serializable;
import java.util.Set;

public interface IUser extends Serializable {
	
	String getId();

	String getName();

	Set<Role> getRoles();
}
