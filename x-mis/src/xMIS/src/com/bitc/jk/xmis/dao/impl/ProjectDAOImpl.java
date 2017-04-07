//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\impl\\ProjectDAOImpl.java

package com.bitc.jk.xmis.dao.impl;

import java.util.List;

import javax.sql.DataSource;

import org.apache.log4j.Logger;
import org.springframework.jdbc.core.JdbcTemplate;

import com.bitc.jk.xmis.dao.ProjectDAO;
import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.FunctionModuleObj;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectTaskObj;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;

public class ProjectDAOImpl implements ProjectDAO {
	private static Logger logger = Logger.getLogger(ProjectDAOImpl.class);

	private JdbcTemplate jt;

	/**
	 * @return the jt
	 */
	public JdbcTemplate getJt() {
		return jt;
	}

	/**
	 * @param jt
	 *            the jt to set
	 */
	public void setJt(JdbcTemplate jt) {
		this.jt = jt;
	}

	private DataSource xMISDB;

	/**
	 * @roseuid 4C1F04200280
	 */
	public ProjectDAOImpl() {

	}

	/**
	 * @param userID
	 * @param isFinished
	 * @return List
	 * @roseuid 4C1F04200290
	 */
	public List getProjectListByUser(int userID, Boolean isFinished) {
		return null;
	}

	/**
	 * @param projectID
	 * @return com.bitc.jk.xmis.model.ProjectObj
	 * @roseuid 4C1F042002DE
	 */
	public ProjectObj getProjectInfoByID(int projectID) {
		return null;
	}

	/**
	 * @param projectID
	 * @return List
	 * @roseuid 4C1F0420031C
	 */
	public List getProjectWorkProductListByID(int projectID) {
		return null;
	}

	/**
	 * @param projectID
	 * @return List
	 * @roseuid 4C1F0420034B
	 */
	public List getFunctionModuleListByID(int projectID) {
		return null;
	}

	/**
	 * @param functionModuleList
	 * @roseuid 4C1F0420037A
	 */
	public void doSaveFunctionModule(List functionModuleList) {

	}

	/**
	 * @param functionModelObj
	 * @roseuid 4C1F042003B9
	 */
	public void doUpdateFunctionModule(FunctionModuleObj functionModelObj) {

	}

	/**
	 * @param projectID
	 * @param functionModuleID
	 * @roseuid 4C1F04210000
	 */
	public void doDeleteFunctionModule(int projectID, int functionModuleID) {

	}

	/**
	 * @param projectID
	 * @return List
	 * @roseuid 4C1F0421005D
	 */
	public List getDeviceListByID(int projectID) {
		return null;
	}

	/**
	 * @param deviceList
	 * @roseuid 4C1F0421009C
	 */
	public void doSaveDeviceList(List deviceList) {

	}

	/**
	 * @param deviceObj
	 * @roseuid 4C1F042100DA
	 */
	public void doUpdateDeviceList(DeviceObj deviceObj) {

	}

	/**
	 * @param projectID
	 * @param deviceID
	 * @roseuid 4C1F04210119
	 */
	public void doDeleteDeviceList(int projectID, int deviceID) {

	}

	/**
	 * @param projectObj
	 * @roseuid 4C1F04210186
	 */
	public void doSaveProjectInfo(ProjectObj projectObj) {

	}

	/**
	 * @param projectObj
	 * @roseuid 4C1F042101C5
	 */
	public void doUpdateProjectInfo(ProjectObj projectObj) {

	}

	/**
	 * @param projectTaskObj
	 * @roseuid 4C1F04210203
	 */
	public void doSaveProjectTask(ProjectTaskObj projectTaskObj) {

	}

	/**
	 * @param projectTaskObj
	 * @roseuid 4C1F04210251
	 */
	public void doUpdateProjectTask(ProjectTaskObj projectTaskObj) {

	}

	/**
	 * @param projectID
	 * @return List
	 * @roseuid 4C1F04210290
	 */
	public List getDepartmentListByID(int projectID) {
		return null;
	}

	/**
	 * @param projectID
	 * @roseuid 4C1F042102DE
	 */
	public void getEmployeeListByID(int projectID) {

	}

	/**
	 * @param projectID
	 * @return List
	 * @roseuid 4C1F0421032C
	 */
	public List getTaskListByID(int projectID) {
		return null;
	}

	public List getProjectStatusStore(String searchName) {

		logger.info("===>  getProjectStatusStore //"
				+ SQLScript.getAllProjectStatus);

		// return jt.queryForList(SQLScript.getAllProjectStatus);
		return jt.queryForList(SQLScript.getAllProjectStatus,
				new Object[] { "%" + searchName + "%" });

	}
}
