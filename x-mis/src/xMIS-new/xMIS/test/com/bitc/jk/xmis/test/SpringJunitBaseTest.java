package com.bitc.jk.xmis.test;

import javax.naming.NamingException;
import javax.transaction.HeuristicMixedException;
import javax.transaction.HeuristicRollbackException;
import javax.transaction.NotSupportedException;
import javax.transaction.RollbackException;
import javax.transaction.SystemException;
import javax.transaction.UserTransaction;

import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.springframework.mock.jndi.SimpleNamingContextBuilder;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.util.WebUtil;
import com.microsoft.sqlserver.jdbc.SQLServerDataSource;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = { "file:WebRoot/WEB-INF/spring-config/applicationContext.xml" })
public abstract class SpringJunitBaseTest extends
		AbstractJUnit4SpringContextTests {
	@BeforeClass
	public static void jndiPrepare() {
		try {
			SQLServerDataSource dataSource = new SQLServerDataSource();
			dataSource
					.setURL("jdbc:sqlserver://192.169.11.27:1433;DatabaseName=xmis");
			dataSource.setUser("xmis-uat");
			dataSource.setPassword("xmis");
			SimpleNamingContextBuilder builder = SimpleNamingContextBuilder
					.emptyActivatedContextBuilder();
			builder.bind("java:comp/env/jdbc/xmis", dataSource);
			builder.bind("java:comp/UserTransaction", new UserTransactionImpl());
			
			EmployeeObj obj = new EmployeeObj();
			obj.setUserID(-1);
			WebUtil.setCurrentEmployee(obj);
		} catch (NamingException e) {
			e.printStackTrace();
		}
	}

	public static class UserTransactionImpl implements UserTransaction {

		public void begin() throws NotSupportedException, SystemException {
			// TODO Auto-generated method stub

		}

		public void commit() throws RollbackException, HeuristicMixedException,
				HeuristicRollbackException, SecurityException,
				IllegalStateException, SystemException {
			// TODO Auto-generated method stub

		}

		public int getStatus() throws SystemException {
			// TODO Auto-generated method stub
			return 0;
		}

		public void rollback() throws IllegalStateException, SecurityException,
				SystemException {
			// TODO Auto-generated method stub

		}

		public void setRollbackOnly() throws IllegalStateException,
				SystemException {
			// TODO Auto-generated method stub

		}

		public void setTransactionTimeout(int arg0) throws SystemException {
			// TODO Auto-generated method stub

		}

	}
}