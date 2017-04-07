//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\impl\\ContractDAOImpl.java

package com.bitc.jk.xmis.dao.impl;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.SQLTransientException;
import java.sql.Types;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementCallback;
import org.springframework.jdbc.core.PreparedStatementSetter;
import org.springframework.jdbc.core.ResultSetExtractor;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.core.support.AbstractLobCreatingPreparedStatementCallback;
import org.springframework.jdbc.support.SQLExceptionTranslator;
import org.springframework.jdbc.support.lob.DefaultLobHandler;
import org.springframework.jdbc.support.lob.LobCreator;
import org.springframework.jdbc.support.lob.LobHandler;

import com.bitc.jk.xmis.dao.ContractDAO;
import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.ContractInfoObj;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.DelayReason;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.EstimateIncomeObj;
import com.bitc.jk.xmis.model.EstimatedPayment;
import com.bitc.jk.xmis.model.GContractSourceObj;
import com.bitc.jk.xmis.model.InvoiceObj;
import com.bitc.jk.xmis.model.InvoiceSplitObj;
import com.bitc.jk.xmis.model.MoneySplitObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PaymentConditionObj;
import com.bitc.jk.xmis.model.PaymentDetailObj;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.Stencil;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.FileUploadDownloadService;
import com.bitc.jk.xmis.service.impl.FileUploadDownloadServiceImpl;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.SpringInit;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class ContractDAOImpl implements ContractDAO {
	private static Logger logger = Logger.getLogger(ContractDAOImpl.class);

	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate ajdbcTemplate) {
		this.jt = ajdbcTemplate;
	}

	// private DataSource xMISDB;

	/**
	 * @roseuid 4C1B2D2202AF
	 */
	public ContractDAOImpl() {

	}

	// /**
	// * Access method for the xMISDB property.
	// *
	// * @return the current value of the xMISDB property
	// */
	// public DataSource getXMISDB()
	// {
	// return xMISDB;
	// }
	//
	// /**
	// * Sets the value of the xMISDB property.
	// *
	// * @param aXMISDB the new value of the xMISDB property
	// */
	// public void setXMISDB(DataSource aXMISDB)
	// {
	// xMISDB = aXMISDB;
	// }

	/**
	 * @param isFinished
	 * @return List
	 * @roseuid 4C1B2D2202CE
	 */
	public List getContractList(EmployeeObj userObj, boolean isFinished) {

		logger.info("getContractList:" + userObj.getUserName());
		List resultList = new ArrayList();
		String sqlStr = "SELECT * from view_合同";
		return jt.queryForList(sqlStr);
	}

	/**
	 * @param userObj
	 * @return List
	 * @roseuid 4C1B2D22032C
	 */
	public List getPaymentList(EmployeeObj userObj) {
		return null;
	}

	/**
	 * @param contractObj
	 * @roseuid 4C1B2D22036B
	 */
	public void doSaveGeneralContract(ContractObj contractObj) {

	}

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B2D2203A9
	 */
	public List getPaymentConditionListByID(int contractID) {
		return null;
	}

	/**
	 * @param paymentConditionList
	 * @roseuid 4C1B2D230000
	 */
	public void doSavePaymentCondition(List paymentConditionList) {

	}

	/**
	 * @param paymentConditionObj
	 * @roseuid 4C1B2D23003E
	 */
	public void doUpdatePaymentCondition(PaymentConditionObj paymentConditionObj) {

	}

	/**
	 * @param contractID
	 * @param paymentConditionID
	 * @roseuid 4C1B2D23008C
	 */
	public void doDeletePaymentCondition(int contractID, int paymentConditionID) {

	}

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B2D2300FA
	 */
	public List getInvoiceListByID(int contractID) {
		return null;
	}

	/**
	 * @param invoiceObj
	 * @roseuid 4C1B2D230148
	 */
	public void doSaveInvoice(InvoiceObj invoiceObj) {

	}

	/**
	 * @param invoiceObj
	 * @roseuid 4C1B2D230196
	 */
	public void doUpdateInvoice(InvoiceObj invoiceObj) {

	}

	/**
	 * @param contractID
	 * @param invoiceID
	 * @roseuid 4C1B2D2301D4
	 */
	public void doDeleteInvoice(int contractID, int invoiceID) {

	}

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B2D230251
	 */
	public List getPaymentListByID(int contractID, PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getPaymentListByIDAndFas, pageInfo);

		logger.info("getPaymentListByID ? contractID="
				+ Type.getString(contractID) + sqlStr);

		return jt
				.queryForList(sqlStr, new Object[] { new Integer(contractID) });
	}

	public List getGLIncomeDetailByID(String projectID, PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getGLIncomeByProjectCode, pageInfo);

		logger.info("getGLIncomeDetailByID ? projectID="
				+ Type.getString(projectID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				Type.getString(projectID), Type.getString(projectID) });
	}
	public List getGLElseIncomeDetailByID(String projectID, PageInfo pageInfo,int type) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getGLElseIncomeAmtByProjectID, pageInfo);

		logger.info("getGLElseIncomeDetailByID ? projectID="
				+ Type.getString(projectID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				Type.getString(projectID), Type.getString(projectID), Type.GetInt(type)});
	}
	public List getGLPurchaseDetailByID(String projectID, PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getGLPurchaseByProjectCode, pageInfo);

		logger.info("getGLIncomeDetailByID ? projectID="
				+ Type.getString(projectID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				Type.getString(projectID), Type.getString(projectID) });
	}

	/**
	 * @param paymentDetailObj
	 * @roseuid 4C1B2D23029F
	 */
	public void doSavePaymentInfo(Object paymentDetailObj) {

	}

	/**
	 * @param paymentDetailObj
	 * @roseuid 4C1B2D2302EE
	 */
	public void doUpdatePaymentInfo(Object paymentDetailObj) {

	}

	/**
	 * @param contractID
	 * @param paymentID
	 * @roseuid 4C1B2D23034B
	 */
	public void doDeletePaymentInfo(int contractID, int paymentID) {

	}

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B2D2303C8
	 */
	public List getReferenceContractListByID(int contractID) {
		return null;
	}

	/**
	 * @param contractObj
	 * @return List
	 * @roseuid 4C1B2D24003E
	 */
	public List getProjectListByID(EmployeeObj contractObj) {
		return null;
	}

	public Map getContractInfo(int contractID) {
		logger.info("===> getContractInfo  contractID =" + contractID
				+ SQLScript.getContractInfo);
		return jt.queryForMap(SQLScript.getContractInfo,
				new Object[] { new Integer(contractID) });
	}

	public Map getContractInfoObj(int contractID) {
		logger.info("getContractInfoObj  contractID=" + contractID
				+ SQLScript.getContractInfoObj);
		String sqlExist = SQLScript.existConInfo;
		logger.info("合同基本信息是否已存在--->conID=" + contractID + sqlExist);
		int number = jt.queryForInt(sqlExist, new Object[] { new Integer(
				contractID) });
		if (number > 0) {
			return jt.queryForMap(SQLScript.getContractInfoObj,
					new Object[] { new Integer(contractID) });
		} else {
			return null;
		}
	}

	public int isHadChildContractListByID(int parentID) {

		logger.info("===> isHadChildContractListByID ?"
				+ Type.getString(parentID)
				+ SQLScript.isHadChildContractListByID);

		return jt.queryForInt(SQLScript.isHadChildContractListByID,
				new Object[] { new Integer(parentID) });
	}

	public List getContractTypeList() {

		logger.info("===> getContractTypeList " + SQLScript.getContractType);

		return jt.queryForList(SQLScript.getContractType);
	}

	public List getContractSourceTypeList() {

		logger.info("===> getContractSourceTypeList "
				+ SQLScript.getContractSourceType);

		return jt.queryForList(SQLScript.getContractSourceType);
	}

	public List getCurrencyTypeList() {

		logger.info("===> getContractTypeList " + SQLScript.getCurrencyType);

		return jt.queryForList(SQLScript.getCurrencyType);
	}

	public List getGovernmentContractSourceList(PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getGovernmentContractSourceList, pageInfo);

		// logger.info("===> getGovernmentContractSourceList " + sqlStr);

		return jt.queryForList(SQLScript.getGovernmentContractSourceList);

	}

	public int doCreateGContrctSournce(final GContractSourceObj st,
			final EmployeeObj userDAN) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.GovernmentContractSource.GovermentContractSourceID,
				DBSchema.GovernmentContractSource.TableName)) + 1;

		jt.update(SQLScript.intsertGContractSource,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxID);
						ps.setString(2, st.getGovermentContractSourceName());
						if (st.getCompanyID() > 0)
							ps.setInt(3, st.getCompanyID());
						else
							ps.setNull(3, Types.INTEGER);

						if (st.getDepartmentID() > 0)
							ps.setInt(4, st.getDepartmentID());
						else
							ps.setNull(4, Types.INTEGER);

						if (st.getAmt() > 0)
							ps.setDouble(5, st.getAmt());
						else
							ps.setNull(5, Types.DOUBLE);

						if (st.getCurrencyTypeID() > 0)
							ps.setInt(6, st.getCurrencyTypeID());
						else
							ps.setNull(6, Types.INTEGER);

						ps.setString(7, st.getSpecialty());

						ps.setInt(8, userDAN.getUserID());

					}
				});

		return maxID;

	}

	public void doUpdateGContractSource(final GContractSourceObj st,
			final EmployeeObj userDAN) {

		jt.update(SQLScript.updateGContractSource,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setString(1, st.getGovermentContractSourceName());
						if (st.getCompanyID() > 0)
							ps.setInt(2, st.getCompanyID());
						else
							ps.setNull(2, Types.INTEGER);

						if (st.getDepartmentID() > 0)
							ps.setInt(3, st.getDepartmentID());
						else
							ps.setNull(3, Types.INTEGER);

						if (st.getAmt() > 0)
							ps.setDouble(4, st.getAmt());
						else
							ps.setNull(4, Types.DOUBLE);

						if (st.getCurrencyTypeID() > 0)
							ps.setInt(5, st.getCurrencyTypeID());
						else
							ps.setNull(5, Types.INTEGER);

						ps.setString(6, st.getSpecialty());

						ps.setInt(7, userDAN.getUserID());

						ps.setInt(8, st.getGovermentContractSourceID());

					}
				});

	}

	public void doSetSTonContract(final GContractSourceObj st,
			final EmployeeObj userDNA) {

		jt.update(SQLScript.setSTonContract, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {

				ps.setInt(1, st.getGovermentContractSourceID());
				ps.setInt(2, userDNA.getUserID());
				ps.setInt(3, st.getCompanyID());

			}
		});

	}

	public int existsContractByGST(int stID) {

		return jt.queryForInt(SQLScript.existsContractByGST,
				new Object[] { new Integer(stID) });
	}

	public void doDeleteGContrctSournce(final int stID,
			final EmployeeObj userDNA) {

		jt.update(SQLScript.deleteGContractSource,
				new PreparedStatementSetter() {
					public void setValues(PreparedStatement ps)
							throws SQLException {

						ps.setInt(1, userDNA.getUserID());
						ps.setInt(2, stID);

					}

				});
	}

	public List getParentContractListPositionByID(int contractTypeID,
			int parentID, String searchName, List<StandardQuery> querys,
			PageInfo pageInfo) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListForParentBySerch);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}
		String sqlStr = WebUtil.generateSQLForPosition(generator.getSQL(),
				DBSchema.Contract.ContractID, pageInfo);

		logger.info("===>  getParentContractListPositionByID //"
				+ Type.getString(parentID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), "%" + searchName + "%",
				"%" + searchName + "%", new Integer(parentID) });

	}

	public List getContractListByType(int contractTypeID, String searchName,
			PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getContractListByTypeForSearch, pageInfo);

		logger.info("===>  getContractListByType //"
				+ Type.getString(contractTypeID) + sqlStr);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID,
				"%" + searchName + "%", "%" + searchName + "%" }
				: new Object[] {
						contractTypeID,
						"%" + searchName + "%",
						"%" + searchName + "%",
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);

	}

	public List getContractListByTypeFilterWarning(int contractTypeID,
			int warningType, PageInfo pageInfo, int userID,
			List<QueryParam> params) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);

		for (QueryParam param : params) {
			generator.addAppendSQL(param.getJdbcSQL());
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils
				.replace(SQLScript.getContractListByTypeFilterWarning,
						SQLScript.getContractListByType.replace("#result",
								"#contract"),
						generator.getSQL().replace("#result", "#contract")),
				pageInfo, SQLScript.getResultStringWithPagingOfContract);

		// String sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getContractListByTypeFilterWarning, pageInfo);

		logger.info("===>  getContractListByTypeFilterWarning //"
				+ Type.getString(warningType) + sqlStr);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID, warningType }
				: new Object[] {
						contractTypeID,
						warningType,
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);

	}

	public List getContractListByTypeForCurrency(int contractTypeID,
			String searchName, String currencyFlag, PageInfo pageInfo,
			int userID) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getContractListByTypeForCurrency, pageInfo);

		logger.info("===>  getContractListByTypeForCurrency //"
				+ Type.getString(currencyFlag) + sqlStr);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID,
				"%" + searchName + "%", currencyFlag, "%" + searchName + "%",
				currencyFlag } : new Object[] { contractTypeID,
				"%" + searchName + "%", currencyFlag, "%" + searchName + "%",
				currencyFlag, new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);

	}

	public List getContractListForParent(int contractTypeID, String searchName,
			List<StandardQuery> querys, PageInfo pageInfo) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListForParentBySerch);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);

		logger.info("===>  getContractListForParent //"
				+ Type.getString(contractTypeID) + "//" + searchName + sqlStr);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID,
				"%" + searchName + "%", "%" + searchName + "%" }
				: new Object[] {
						contractTypeID,
						"%" + searchName + "%",
						"%" + searchName + "%",
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) };

		return jt.queryForList(sqlStr, args);

	}

	public List getAttachedListByContractID(int contractID,
			int attachmentTypeID, PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getAttachmentListByRefID, pageInfo);

		logger.info("===>  getAttachedListByContractID //"
				+ Type.getString(contractID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] { new Integer(contractID),
				new Integer(attachmentTypeID) });

	}

	public int doSaveContractAttachment(final AttachmentFile attFile) {

		logger.info("===>  doSaveContractAttachment //" + attFile.getFileName()
				+ "//" + attFile.getFileSize() + SQLScript.createNewAttched);

		final int maxID = jt.queryForInt(SQLScript
				.getMaxIDFromTab(DBSchema.Attachment.AttachmentID,
						DBSchema.Attachment.TableName)) + 1;

		final LobHandler lobHandler = new DefaultLobHandler();

		jt.execute(SQLScript.createNewAttched,
				new AbstractLobCreatingPreparedStatementCallback(lobHandler) {
					protected void setValues(PreparedStatement ps,
							LobCreator lobCreator) throws SQLException {

						ps.setInt(1, maxID);// 附件ID
						ps.setInt(2, attFile.getUploadID());// 上传人ID
						ps.setString(3, attFile.getFileName());// 文件名

						lobCreator.setBlobAsBinaryStream(ps, 4,
								attFile.getFis(), attFile.getFileSize());// 附件内容

						ps.setInt(5, attFile.getAttachmentTypeID());// 附件主体类型
						ps.setString(6, attFile.getRefID());// REFID
					}
				});

		// save BinaryStream
		// jt.update(SQLScript.createNewAttched, new PreparedStatementSetter() {
		//
		// public void setValues(PreparedStatement ps) throws SQLException {
		// ps.setInt(1, maxID);
		// ps.setInt(2, attFile.getUploadID());
		// ps.setString(3, attFile.getFileName());
		// byte[] content = attFile.getContent();
		// //ps.setBytes(4, content);
		//
		// ps.setBinaryStream(4, attFile.getFis(),
		// attFile.getFileSize());//将图片保存
		//
		// ps.setInt(5, attFile.getAttachmentTypeID());
		// ps.setInt(6, attFile.getRefID());
		// }
		// });

		return maxID;
	}

	// public int getAttachmentTypeID(final String tableName,
	// final String fieldName) {
	//
	// logger.info("===> getAttachmentTypeID ? tableName=" + tableName
	// + "&fieldName=" + fieldName + SQLScript.getAttachmentTypeID);
	//
	// return jt.queryForInt(SQLScript.getAttachmentTypeID, new Object[] {
	// tableName, fieldName });
	//
	// }
	public List getAttachmentTypeID(final String tableName,
			final String fieldName) {
		logger.info("===> getAttachmentTypeID ? tableName=" + tableName
				+ "&fieldName=" + fieldName + SQLScript.getAttachmentTypeID);
		return jt.queryForList(SQLScript.getAttachmentTypeID, new Object[] {
				tableName, fieldName });

	}

	public int createNewAttachmentTypeID(final String tableName,
			final String fieldName) {

		logger.info("===> createNewAttachmentTypeID ? tableName=" + tableName
				+ "&fieldName=" + fieldName + SQLScript.createNewAttachmentType);

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.AttachmentType.AttachmentTypeID,
				DBSchema.AttachmentType.TableName)) + 1;

		jt.update(SQLScript.createNewAttachmentType,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxID);
						ps.setString(2, tableName + "." + fieldName);
						ps.setString(3, tableName);
						ps.setString(4, fieldName);

					}
				});

		return maxID;
	}

	public String getAttachmentByID(final int attachmentID,
			final String directPath) {
		logger.info("===> getAttachmentContentByID ? attachmentID="
				+ attachmentID);

		// return jt.queryForList(SQLScript.getAttachmentContent,
		// new Object[] { new Integer(attachmentID) });

		final LobHandler lobHandler = new DefaultLobHandler();
		final AttachmentFile att = new AttachmentFile();

		jt.query(SQLScript.getAttachmentContent, new Object[] { new Integer(
				attachmentID) }, new RowMapper() {
			public Object mapRow(ResultSet rs, int i) throws SQLException {

				String fileName = rs
						.getString(DBSchema.Attachment.AttachmentName);
				InputStream inputStream = lobHandler.getBlobAsBinaryStream(rs,
						DBSchema.Attachment.AttachmentContent);

				if (inputStream == null) {
					int attType = rs
							.getInt(DBSchema.Attachment.AttachmentTypeID);

					att.setFilePath(directPath + Type.getString(attType) + "\\");
				} else {
					att.setFilePath(directPath);

					java.io.File fileOutput = new java.io.File(directPath
							+ fileName);
					if (fileOutput.exists())
						fileOutput.delete();
					FileOutputStream fo;
					try {
						fo = new FileOutputStream(fileOutput);
						int c;

						while ((c = inputStream.read()) != -1) {
							fo.write(c);
						}

						fo.close();

					} catch (FileNotFoundException e) {
						e.printStackTrace();
					} catch (IOException e) {
						e.printStackTrace();
					} catch (Exception e) {
						e.printStackTrace();
					}
				}

				att.setFileName(fileName);
				return null;
			}
		});

		logger.info(att.getFileName() + ":" + att.getFileName());
		return att.getFilePath() + att.getFileName();
	}

	public int doCreateNullContract(final int userID) {

		logger.info("===> doCreateNullContrac " + SQLScript.createNullContract);

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.Contract.ContractID, DBSchema.Contract.TableName)) + 1;

		jt.update(SQLScript.createNullContract, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, maxID);
				ps.setInt(2, userID);
			}
		});
		return maxID;
	}

	public List getCompanyList(int needRemovedCompanyID, PageInfo pageInfo) {

		needRemovedCompanyID = 0;
		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getCompanyListWithoutSelf, pageInfo);

		logger.info("===>  getCompanyList //"
				+ Type.getString(needRemovedCompanyID) + sqlStr);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { needRemovedCompanyID }
				: new Object[] {
						needRemovedCompanyID,
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) };

		return jt.queryForList(sqlStr, args);

		// return jt.queryForList(SQLScript.getCompanyList);
	}

	public List getCompanyPositionByID(int needRemovedID, int companyID,
			PageInfo pageInfo) {

		needRemovedID = 0;
		String sqlStr = WebUtil.generateSQLForPosition(
				SQLScript.getCompanyListWithoutSelf,
				DBSchema.Company.CompanyID, pageInfo);

		logger.info("===>  getCompanyList //" + Type.getString(companyID)
				+ sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(needRemovedID), new Integer(companyID) });

	}

	public List getDeptListBySelfCompanyID(final int companyID,
			PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getDeptListBySelfCompanyID, pageInfo);
		logger.info("===>  getDeptListBySelfCompanyID //"
				+ Type.getString(companyID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] { new Integer(companyID),
				new Integer(companyID) });

	}

	public List getDeptListByCompanyID(final int companyID, PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getDeptListByCompanyID, pageInfo);

		logger.info("===>  getDeptListByCompanyID //"
				+ Type.getString(companyID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] { new Integer(companyID) });

	}

	public void doSaveContractInfo(final ContractObj con, final int userID) {
		logger.info("===> doSaveContractInfo"
				+ Type.getString(con.getDeliveryID()));

		// jt.update(SQLScript.updateContractInfo, new PreparedStatementSetter()
		// {
		//
		// public void setValues(PreparedStatement ps) throws SQLException {
		//
		// ps.setInt(1, con.getContractTypeID());
		// ps.setInt(2, con.getSourceTypeID());
		//
		// if (con.getGovermentContractSourceID() == 0)
		// ps.setNull(3, Types.INTEGER);
		// else
		// ps.setInt(3, con.getGovermentContractSourceID());
		//
		// ps.setString(4, con.getContractNo());
		// ps.setString(5, con.getContractName());
		//
		// if ("".equals(con.getAbbreviation()))
		// ps.setNull(6, Types.VARCHAR);
		// else
		// ps.setString(6, con.getAbbreviation());
		//
		// if ("".equals(con.getContractSummary()))
		// ps.setNull(7, Types.VARCHAR);
		// else
		// ps.setString(7, con.getContractSummary());
		//
		// if (con.getParentID() == 0)
		// ps.setNull(8, Types.INTEGER);
		// else
		// ps.setInt(8, con.getParentID());
		//
		// if ("".equals(con.getWarrantyDefinition()))
		// ps.setNull(9, Types.VARCHAR);
		// else
		// ps.setString(9, con.getWarrantyDefinition());
		//
		// ps.setDate(10, con.getWarrantyStartDate());
		//
		// if (con.getWarrantyEndDate() == null)
		// ps.setNull(11, Types.DATE);
		// else
		// ps.setDate(11, con.getWarrantyEndDate());
		//
		// ps.setDouble(12, con.getContractAmt());
		// ps.setInt(13, con.getCurrencyID());
		// ps.setDate(14, con.getSignatureDate());
		// ps.setInt(15, con.getBuyerID());
		//
		// if (con.getDeptIDOfBuyer() == 0)
		// ps.setNull(16, Types.INTEGER);
		// else
		// ps.setInt(16, con.getDeptIDOfBuyer());
		//
		// if (con.getBuyerRepresentativeID() == 0)
		// ps.setNull(17, Types.INTEGER);
		// else
		// ps.setInt(17, con.getBuyerRepresentativeID());
		//
		// // logger.info(con.getVendorID());
		//
		// ps.setInt(18, con.getVendorID());
		//
		// if (con.getDeptIDOfVendor() == 0)
		// ps.setNull(19, Types.INTEGER);
		// else
		// ps.setInt(19, con.getDeptIDOfVendor());
		//
		// if (con.getVendorRepresentativeID() == 0)
		// ps.setNull(20, Types.INTEGER);
		// else
		// ps.setInt(20, con.getVendorRepresentativeID());
		//
		// if ("".equals(con.getRemark()))
		// ps.setNull(21, Types.VARCHAR);
		// else
		// ps.setString(21, con.getRemark());
		//
		// if (con.getDeliveryID() == 0)
		// ps.setNull(22, Types.INTEGER);
		// else
		// ps.setInt(22, con.getDeliveryID());
		//
		// ps.setInt(23, con.getLastModificator());
		//
		// ps.setFloat(24, con.getWarrantyLimit());
		// ps.setInt(25, con.getWarrantyUnit());
		// ps.setString(26, con.getProjectOBJ().getProjectID());
		// ps.setInt(27, con.getDirectorID());
		// ps.setInt(28, con.getContractID());
		//
		// }
		// });
		jt.update(SQLScript.updateContractInfoNew,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {

						ps.setInt(1, con.getContractTypeID());
						ps.setInt(2, con.getSourceTypeID());

						if (con.getGovermentContractSourceID() == 0)
							ps.setNull(3, Types.INTEGER);
						else
							ps.setInt(3, con.getGovermentContractSourceID());

						ps.setString(4, con.getContractNo());
						ps.setString(5, con.getContractName());

						if ("".equals(con.getAbbreviation()))
							ps.setNull(6, Types.VARCHAR);
						else
							ps.setString(6, con.getAbbreviation());

						if ("".equals(con.getContractSummary()))
							ps.setNull(7, Types.VARCHAR);
						else
							ps.setString(7, con.getContractSummary());

						if (con.getParentID() == 0)
							ps.setNull(8, Types.INTEGER);
						else
							ps.setInt(8, con.getParentID());

						if ("".equals(con.getWarrantyDefinition()))
							ps.setNull(9, Types.VARCHAR);
						else
							ps.setString(9, con.getWarrantyDefinition());

						ps.setDate(10, con.getWarrantyStartDate());

						if (con.getWarrantyEndDate() == null)
							ps.setNull(11, Types.DATE);
						else
							ps.setDate(11, con.getWarrantyEndDate());

						ps.setDouble(12, con.getContractAmt());
						ps.setInt(13, con.getCurrencyID());
						ps.setDate(14, con.getSignatureDate());
						ps.setInt(15, con.getBuyerID());

						if (con.getDeptIDOfBuyer() == 0)
							ps.setNull(16, Types.INTEGER);
						else
							ps.setInt(16, con.getDeptIDOfBuyer());

						if (con.getBuyerRepresentativeID() == 0)
							ps.setNull(17, Types.INTEGER);
						else
							ps.setInt(17, con.getBuyerRepresentativeID());

						// logger.info(con.getVendorID());

						ps.setInt(18, con.getVendorID());

						if (con.getDeptIDOfVendor() == 0)
							ps.setNull(19, Types.INTEGER);
						else
							ps.setInt(19, con.getDeptIDOfVendor());

						if (con.getVendorRepresentativeID() == 0)
							ps.setNull(20, Types.INTEGER);
						else
							ps.setInt(20, con.getVendorRepresentativeID());

						if ("".equals(con.getRemark()))
							ps.setNull(21, Types.VARCHAR);
						else
							ps.setString(21, con.getRemark());

						if (con.getDeliveryID() == 0)
							ps.setNull(22, Types.INTEGER);
						else
							ps.setInt(22, con.getDeliveryID());

						ps.setInt(23, con.getLastModificator());

						ps.setFloat(24, con.getWarrantyLimit());
						ps.setInt(25, con.getWarrantyUnit());
						ps.setString(26, con.getProjectOBJ().getProjectID());
						if (con.getDirectorID() == null) {
							ps.setNull(27, Types.INTEGER);
						} else {
							ps.setInt(27, con.getDirectorID());
						}
						ps.setDate(28, con.getValidityStartDate());
						ps.setDate(29, con.getValidityEndDate());
						ps.setFloat(30, con.getValidityLimit());
						ps.setInt(31, con.getValidityUnit());
						ps.setInt(32, con.getNewflag());
						ps.setInt(33, con.getContractID());

					}
				});
	}

	public List getPaymentConditionList(final int contractID) {
		logger.info("===>  getPaymentConditionList //"
				+ Type.getString(contractID)
				+ SQLScript.getPaymentConditionByContract);

		return jt.queryForList(SQLScript.getPaymentConditionByContract,
				new Object[] { new Integer(contractID) });

	}

	public int doCreateNewProjectStatus(final String projectId,
			final String statusName, final String desc, final int userID) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.ProjectStatus.ProjectStatusID,
				DBSchema.ProjectStatus.TableName)) + 1;

		jt.update(SQLScript.intsertProjectStatus,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxID);
						ps.setString(2, statusName);
						ps.setString(3, projectId);
						ps.setString(4, desc);
					}
				});

		return maxID;
	}

	public int doCreateNewPaymentConditionNull(final int contractID, int userID) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.PaymentCondition.PaymentConditionID,
				DBSchema.PaymentCondition.TableName,
				DBSchema.PaymentCondition.ContractID + " = ?"),
				new Object[] { new Integer(contractID) }) + 1;

		jt.update(SQLScript.insertPaymentCondition,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, contractID);
						ps.setInt(2, maxID);

					}
				});

		return maxID;
	}

	private void doCreateNewPaymentConditionLog(final PaymentConditionObj pay,
			final int userID) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.PaymentConditionManualLog.LogID,
				DBSchema.PaymentConditionManualLog.TableName)) + 1;

		jt.update(SQLScript.insertPaymentConditionLog,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxID);
						ps.setInt(2, pay.getContractID());
						ps.setInt(3, pay.getPaymentConditionID());
						ps.setDate(4, pay.getPaymentDate());
						ps.setDouble(5, pay.getPaymentPercent());
						ps.setDouble(6, pay.getPamymentAmt());
						ps.setString(7, pay.getManualReason());
						ps.setInt(8, userID);

					}
				});
	}

	public int doSavePaymentCondition(final PaymentConditionObj pay,
			final int userID) {

		jt.update(SQLScript.savePaymentCondition,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {

						if (pay.getDescription() == null
								|| "".equals(pay.getDescription()))
							ps.setString(1, "");
						else
							ps.setString(1, pay.getDescription());

						ps.setInt(2, pay.getProjectStatusMapping());
						ps.setInt(3, pay.getDelayDays());
						// if (pay.getPaymentDate() == null)
						// ps.setNull(4, Types.DATE);
						// else
						// ps.setDate(4, pay.getPaymentDate());
						// if (pay.getManualFlag() == null
						// || "false".equals(pay.getManualFlag()))
						// ps.setInt(5, 0);
						// else

						ps.setDouble(4, pay.getPaymentPercent());

						ps.setString(5, pay.getPaymentNodeName());

						ps.setDouble(6, pay.getPamymentAmt());

						// if ("".equals(pay.getManualReason()))
						// ps.setNull(8, Types.CHAR);
						// else
						// ps.setString(8, pay.getManualReason());
						ps.setDate(
								7,
								new java.sql.Date(new java.util.Date()
										.getTime()));
						ps.setInt(8, pay.getLastModificatorID());
						ps.setInt(9, pay.getContractID());
						ps.setInt(10, pay.getPaymentConditionID());

					}
				});

		// if ("true".equals(pay.getManualFlag()))
		// this.doCreateNewPaymentConditionLog(pay, userID);
		return pay.getPaymentConditionID();
	}

	public void doDelPaymentCondition(final int contractID,
			final int paymentConditionID, final int userID) {

		logger.warn("!!!!!! doDelPaymentCondition ? contractID="
				+ Type.getString(contractID) + "& paymentContractID="
				+ Type.getString(paymentConditionID) + "& userID="
				+ Type.getString(userID));

		jt.update(SQLScript.deletePaymentCondition,
				new PreparedStatementSetter() {
					public void setValues(PreparedStatement ps)
							throws SQLException {

						ps.setInt(1, contractID);
						ps.setInt(2, paymentConditionID);

					}

				});
	}

	public List getChildContractListByID(int parentID, PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getChildContractListByID, pageInfo);

		logger.info("===>  getChildContractListByID //"
				+ Type.getString(parentID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] { new Integer(parentID) });

	}

	public int isHadPaymentConditionByID(int contractID) {

		String sqlStr = SQLScript.getCountByIDFromTab(
				DBSchema.PaymentCondition.ContractID,
				DBSchema.PaymentCondition.TableName);

		logger.info("===>  isHadPaymentConditionByID //"
				+ Type.getString(contractID) + sqlStr);

		return jt.queryForInt(sqlStr, new Object[] { new Integer(contractID) });
	}

	public void doUpdatePaymentConditionForDistruted(final int contractID,
			final double contractAmt) {

		logger.info("===>  doUpdatePaymentConditionForDistruted //"
				+ Type.getString(contractID)
				+ SQLScript.updatePaymnetConditionFortributed);

		jt.update(SQLScript.updatePaymnetConditionFortributed,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {

						ps.setDouble(1, contractAmt);
						ps.setInt(2, contractID);

					}
				});
	}

	public double getTotalSumByID(int contractID, int sumType) {

		String sqlStr = "";
		switch (sumType) {
		case WebKeys.Type_Total_Sum_Paymented:
			sqlStr = SQLScript.getTotalPaymentByID;
			break;
		case WebKeys.Type_Total_Sum_Invoiced:
			sqlStr = SQLScript.getTotalInvoicedByID;
			break;

		}

		logger.info("getTotalSumByID ? contractID="
				+ Type.getString(contractID) + sqlStr);

		Double d = (Double) jt.queryForObject(sqlStr,
				new Object[] { new Integer(contractID) }, Double.class);
		return d == null ? 0 : d.doubleValue();
	}

	public List getInvoiceListByID(int contractID, PageInfo pageInfo) {
		String sqlStr = WebUtil.generateSQLStr(SQLScript.getInvoiceListByID,
				pageInfo);

		logger.info("getInvoiceListByID ? contractID="
				+ Type.getString(contractID) + sqlStr);

		return jt
				.queryForList(sqlStr, new Object[] { new Integer(contractID) });
	}

	public List getReceiptListAsInvoiceByID(int contractID, PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getReceiptListAsInvoiceByID, pageInfo);

		logger.info("getReceiptListAsInvoiceByID ? contractID="
				+ Type.getString(contractID) + sqlStr);

		return jt
				.queryForList(sqlStr, new Object[] { new Integer(contractID) });
	}

	public int doCreateNewPaymentDetailNull(final int contractID,
			final int userID) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.ChargedDetail.ChargedID,
				DBSchema.ChargedDetail.TableName,
				DBSchema.ChargedDetail.ContractID + " = ?"),
				new Object[] { new Integer(contractID) }) + 1;

		jt.update(SQLScript.insertChargedDetail, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, contractID);
				ps.setInt(2, maxID);
				ps.setInt(3, userID);

			}
		});
		return maxID;
	}

	public int doSavePaymentDetail(final PaymentDetailObj pay, final int userID) {

		jt.update(SQLScript.saveChargedDetail, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {

				ps.setDouble(1, pay.getPaymentAmt());
				ps.setDate(2, pay.getPaymentDate());
				ps.setInt(3, pay.getTransactorID());
				ps.setInt(4, userID);
				if (pay.getRemark() == null || "".equals(pay.getRemark()))
					ps.setNull(5, Types.VARCHAR);
				else
					ps.setString(5, pay.getRemark());

				ps.setInt(6, pay.getContractID());
				ps.setInt(7, pay.getPaymentID());
			}
		});

		return pay.getPaymentID();
	}

	public void doDelPaymentDetail(final int contractID,
			final int paymentDetailID, final int userID) {

		logger.warn("!!!!!! doDelPaymentDetail ? contractID="
				+ Type.getString(contractID) + "& paymentContractID="
				+ Type.getString(paymentDetailID) + "& userID="
				+ Type.getString(userID));

		jt.update(SQLScript.deleteChargedDetail, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {

				ps.setInt(1, contractID);
				ps.setInt(2, paymentDetailID);

			}

		});
	}

	public int doCreateNewInvoiceDetailNull(final int contractID,
			final int userID) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.InvoiceDetail.InvoiceNo,
				DBSchema.InvoiceDetail.TableName,
				DBSchema.InvoiceDetail.ContractID + " = ?"),
				new Object[] { new Integer(contractID) }) + 1;

		jt.update(SQLScript.insertInvoiceDetail, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, contractID);
				ps.setInt(2, maxID);
				ps.setInt(3, userID);

			}
		});
		return maxID;
	}

	public int doCreateNewReceiptDetailNull(final int contractID,
			final int userID) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.Receipt.ReceiptID, DBSchema.Receipt.TableName,
				DBSchema.Receipt.ContractID + " = ?"),
				new Object[] { new Integer(contractID) }) + 1;

		logger.info("+++++++++++++++++++" + maxID);

		jt.update(SQLScript.insertReceiptDetail, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, contractID);
				ps.setInt(2, maxID);
				ps.setInt(3, userID);
			}
		});
		return maxID;
	}

	/***************************************************************************
	 * invoice
	 * 
	 * txh
	 */

	public void doDelInvoiceDetail(final int contractID,
			final int invoiceDetailID, final int userID) {

		logger.warn("!!!!!! doDelInvoiceDetail ? contractID="
				+ Type.getString(contractID) + "& invoiceDetailID="
				+ Type.getString(invoiceDetailID) + "& userID="
				+ Type.getString(userID));

		jt.update(SQLScript.deleteInvoiceDetail, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {

				ps.setInt(1, contractID);
				ps.setInt(2, invoiceDetailID);

			}

		});
	}

	public void doDelReceiptDetail(final int contractID,
			final int invoiceDetailID, final int userID) {

		logger.warn("!!!!!! doDelReceiptDetail ? contractID="
				+ Type.getString(contractID) + "& invoiceDetailID="
				+ Type.getString(invoiceDetailID) + "& userID="
				+ Type.getString(userID));

		jt.update(SQLScript.deleteReceiptDetail, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, contractID);
				ps.setInt(2, invoiceDetailID);
			}

		});
	}

	public int doSaveReceiptDetail(final InvoiceObj invo, final int userID) {

		jt.update(SQLScript.saveReceiptDetail, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {

				ps.setDouble(1, invo.getInvoiceAMT());
				ps.setDate(2, invo.getInvoiceDate());
				ps.setInt(3, invo.getTransactorID());
				ps.setInt(4, userID);
				if (invo.getInvoiecRemark() == null
						|| "".equals(invo.getInvoiecRemark()))
					ps.setNull(5, Types.VARCHAR);
				else
					ps.setString(5, invo.getInvoiecRemark());

				if (invo.getInvoiceCode() == null
						|| "".equals(invo.getInvoiceCode()))
					ps.setNull(6, Types.VARCHAR);
				else
					ps.setString(6, invo.getInvoiceCode());
				ps.setInt(7, invo.getReceiverId());
				ps.setString(8, invo.getMailCode());
				ps.setInt(9, invo.getContractID());
				ps.setInt(10, invo.getInvoiceID());
			}
		});

		return invo.getInvoiceID();
	}

	public int doSaveInvoiceDetail(final InvoiceObj invo, final int userID) {

		jt.update(SQLScript.saveInvoiceDetail, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {

				ps.setDouble(1, invo.getInvoiceAMT());
				ps.setDate(2, invo.getInvoiceDate());
				ps.setInt(3, invo.getTransactorID());
				ps.setInt(4, userID);
				if (invo.getInvoiecRemark() == null
						|| "".equals(invo.getInvoiecRemark()))
					ps.setNull(5, Types.VARCHAR);
				else
					ps.setString(5, invo.getInvoiecRemark());

				if (invo.getInvoiceCode() == null
						|| "".equals(invo.getInvoiceCode()))
					ps.setNull(6, Types.VARCHAR);
				else
					ps.setString(6, invo.getInvoiceCode());
				ps.setInt(7, invo.getReceiverId());
				ps.setString(8, invo.getMailCode());

				ps.setInt(9, invo.getContractID());
				ps.setInt(10, invo.getInvoiceID());
			}
		});

		return invo.getInvoiceID();
	}

	public List getContractListByIntegrationNewType(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils
				.replace(SQLScript.getContractListByIntegrationNewType,
						SQLScript.getContractListByType.replace("#result",
								"#contract"),
						generator.getSQL().replace("#result", "#contract")),
				pageInfo, SQLScript.getResultStringWithPagingOfContract);

		// String sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getContractListByIntegrationNewType, pageInfo);

		logger.info("===>  getContractListByIntegration //"
				+ Type.getString(contractTypeID) + sqlStr);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID, begDate,
				endDate } : new Object[] { contractTypeID, begDate, endDate,
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };

		return jt.queryForList(sqlStr, args);

	}

	public List getContractListByIntegrationPerformed(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils
				.replace(SQLScript.getRunContractInTimeScope1,
						SQLScript.getContractListByType.replace("#result",
								"#contract"),
						generator.getSQL().replace("#result", "#contract")),
				pageInfo,
				SQLScript.getResultStringWithPagingOfContractByPayInfo);

		// String sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getPerformedContractInTimeScope, pageInfo);

		logger.info("===>  getContractListByIntegrationPerformed //"
				+ Type.getString(contractTypeID) + sqlStr);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID, begDate,
				endDate, begDate, endDate, begDate, endDate, begDate, endDate,
				begDate, begDate, endDate } : new Object[] { contractTypeID,
				begDate, endDate, new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };

		return jt.queryForList(sqlStr, args);

	}

	public List getContractListByIntegrationWarranty(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys) {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils
				.replace(SQLScript.getContractListByIntegrationWarranty,
						SQLScript.getContractListByType.replace("#result",
								"#contract"),
						generator.getSQL().replace("#result", "#contract")),
				pageInfo, SQLScript.getResultStringWithPagingOfContract);

		// String sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getContractListByIntegrationWarranty, pageInfo);

		logger.info("===>  getContractListByIntegrationWarranty //"
				+ Type.getString(contractTypeID) + sqlStr);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID, begDate,
				endDate } : new Object[] { contractTypeID, begDate, endDate,
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);

	}

	public List getSumInfoByIntegrationNewType(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils
				.replace(SQLScript.getSumInfoByIntegrationNewType,
						SQLScript.getContractListByType.replace("#result",
								"#contract"),
						generator.getSQL().replace("#result", "#contract")),
				pageInfo);

		// String sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getSumInfoByIntegrationNewType, pageInfo);

		logger.info("===>  getSumInfoByIntegrationNewType //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), begDate, endDate });

	}

	// getSumInfoByIntegrationPerformed
	public List getSumInfoByIntegrationPerformed(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getRunContractInTimeScope);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils.replace(
				SQLScript.getSumInfoByIntegrationPerformed,
				SQLScript.getRunContractInTimeScope.replace("#result",
						"#contract_timescope"),
				generator.getSQL().replace("#result", "#contract_timescope")),
				pageInfo);

		// String sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getSumInfoByIntegrationPerformed, pageInfo);

		logger.info("===>  getSumInfoByIntegrationPerformed //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), begDate, endDate, begDate,
				endDate, begDate, endDate });

	}

	public List getSumInfoByIntegrationSimpleType(int contractTypeID,
			List<StandardQuery> querys, PageInfo pageInfo, int userID) {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil
				.generateSQLStr(SQLScript
						.getSumInfoByIntegrationSimpleType(generator.getSQL()),
						pageInfo);

		logger.info("===>  getSumInfoByIntegrationSimpleType //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] { new Integer(
				contractTypeID) });

	}

	public List getWarningTotalCountForIntegrationSimpleType(
			int contractTypeID, String searchName, PageInfo pageInfo, int userID) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getWarningTotalCountForIntegrationSimpleType,
				pageInfo);

		logger.info("===>  getWarningTotalCountForIntegrationSimpleType //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), "%" + searchName + "%",
				"%" + searchName + "%" });
	}

	public List getWarningTotalCountForIntegrationSimpleType(
			int contractTypeID, String searchName, PageInfo pageInfo,
			int userID, List<QueryParam> params) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType
						.replace(
								SQLScript.getGLInfoByContractID,
								"\n select #sumInfo.*, CONVERT(DECIMAL(18,2),0) AS MB, CONVERT(DECIMAL(18,2),0) AS unMB, "
										+ "CONVERT(DECIMAL(18,2),0) AS pay_amt "
										+ " into #result "
										+ " from #sumInfo where 1=2"
										+ "\n insert into #result "
										+ " select #sumInfo.*, CONVERT(DECIMAL(9,2),0) AS MB, CONVERT(DECIMAL(9,2),0) AS unMB, "
										+ "CONVERT(DECIMAL(9,2),0) AS pay_amt  "
										+ " from #sumInfo  "));
		for (QueryParam param : params) {
			generator.addAppendSQL(param.getJdbcSQL());
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils
				.replace(
						SQLScript.getWarningTotalCountForIntegrationSimpleType,
						SQLScript.getContractListByType.replace("#result",
								"#contract"),
						generator.getSQL().replace("#result", "#contract")),
				pageInfo);

		logger.info("===>  getWarningTotalCountForIntegrationSimpleType //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), "%" + searchName + "%",
				"%" + searchName + "%" });
	}

	public List getContractListByIntegrationNewTypeForCurrency(
			int contractTypeID, String begDate, String endDate,
			String currencyFlag, PageInfo pageInfo, int userID,
			List<StandardQuery> querys) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil
				.generateSQLStr(
						StringUtils
								.replace(
										SQLScript.getContractListByIntegrationNewTypeForCurrency,
										SQLScript.getContractListByType
												.replace("#result", "#contract"),
										generator.getSQL().replace("#result",
												"#contract")), pageInfo);

		// String sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getContractListByIntegrationNewTypeForCurrency,
		// pageInfo);

		logger.info("===>  getContractListByIntegrationNewTypeForCurrency //"
				+ Type.getString(contractTypeID) + sqlStr);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID, begDate,
				endDate, currencyFlag } : new Object[] { contractTypeID,
				begDate, endDate, currencyFlag,
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);
	}

	public List getContractListByIntegrationPerformedForCurrency(
			int contractTypeID, String begDate, String endDate,
			String currencyFlag, String subIntegrationType, PageInfo pageInfo,
			int userID, List<StandardQuery> querys) {

		String sqlStr = "";

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getRunContractInTimeScope);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		if (WebKeys.Type_subIntegration_Payed.equals(subIntegrationType))

			sqlStr = WebUtil.generateSQLStr(StringUtils.replace(
					SQLScript.getPerformedSubIntegrationForPayed,
					SQLScript.getRunContractInTimeScope.replace("#result",
							"#contract_timescope"),
					generator.getSQL()
							.replace("#result", "#contract_timescope")),
					pageInfo);

		// sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getPerformedSubIntegrationForPayed, pageInfo);
		else if (WebKeys.Type_subIntegration_Invoiced
				.equals(subIntegrationType))
			sqlStr = WebUtil.generateSQLStr(StringUtils.replace(
					SQLScript.getPerformedSubIntegrationForInvoiced,
					SQLScript.getRunContractInTimeScope.replace("#result",
							"#contract_timescope"),
					generator.getSQL()
							.replace("#result", "#contract_timescope")),
					pageInfo);

		// sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getPerformedSubIntegrationForInvoiced, pageInfo);
		else if (WebKeys.Type_subIntegration_Estimate
				.equals(subIntegrationType))
			sqlStr = WebUtil.generateSQLStr(StringUtils.replace(
					SQLScript.getPerformedSubIntegrationForEstimate,
					SQLScript.getRunContractInTimeScope.replace("#result",
							"#contract_timescope"),
					generator.getSQL()
							.replace("#result", "#contract_timescope")),
					pageInfo);

		// sqlStr = WebUtil.generateSQLStr(
		// SQLScript.getPerformedSubIntegrationForEstimate, pageInfo);

		logger.info("===>  getContractListByIntegrationPerformedForCurrency //"
				+ Type.getString(subIntegrationType) + sqlStr);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID, begDate,
				endDate, begDate, endDate, begDate, endDate, currencyFlag }
				: new Object[] {
						contractTypeID,
						begDate,
						endDate,
						begDate,
						endDate,
						begDate,
						endDate,
						currencyFlag,
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);
	}

	public List getRelatedContractList(int contractID, int contractType,
			PageInfo pageInfo) {
		String sqlStr = "";
		switch (contractType) {
		case WebKeys.Type_ContractType_Income:
			sqlStr = SQLScript.getRelatedContractListIncome
					+ "\n order by "
					+ pageInfo.getSort()
					+ " "
					+ pageInfo.getDir()
					+ SQLScript.getRelatedContractListIncomeWithTotalInfoResults;
			break;
		case WebKeys.Type_ContractType_Payout:
			sqlStr = SQLScript.getRelatedContractListPayout
					+ "\n order by "
					+ pageInfo.getSort()
					+ " "
					+ pageInfo.getDir()
					+ SQLScript.getRelatedContractListPayoutWithTotalInfoResults;
			break;
		}
		logger.info("getRelatedContractList ? contractID=" + contractID + "\n"
				+ sqlStr);
		return jt
				.queryForList(sqlStr, new Object[] { new Integer(contractID) });
	}

	public List getContractListByProjectID(String projectID) {
		logger.info("===>  getContractListByProjectID //" + projectID);
		String sqlStr = "SET NOCOUNT ON \nDECLARE @contractType int  set @contractType = 1 \n"
				+ SQLScript.getContractListByProjectID
				+ "\nSELECT * from #result";
		logger.info("===>  getContractListByProjectID //sql" + sqlStr);
		return jt.queryForList(sqlStr, new Object[] { projectID });
	}

	public List getOutContractListByProjectID(String projectID) {
		logger.info("===>  getOutContractListByProjectID //" + projectID);
		String sqlStr = "SET NOCOUNT ON \n"
				+ SQLScript.getOutContractByProjectID;
		return jt.queryForList(sqlStr, new Object[] { projectID });
	}

	public List getContractListByInkling(int contractTypeID,
			List<QueryParam> params, PageInfo pageInfo) {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		String sql = QueryParam.getJdbcSql(params, "or");
		generator.addAppendSQL(sql);
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
		logger.info("===>  getContractListByInkling //" + contractTypeID
				+ sqlStr);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID }
				: new Object[] {
						contractTypeID,
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);
	}

	public List getContractListByQuerys(int contractTypeID,
			List<StandardQuery> querys, PageInfo pageInfo) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}
		logger.info(generator.getSQL());
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo,
				SQLScript.getResultStringWithPagingOfContract);
		
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID }
				: new Object[] {
						contractTypeID,
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) };
		logger.info("---> getContractListByQuerys " + sqlStr);
		return jt.queryForList(sqlStr, args);
	}

	
	public boolean hasFullRightsOnConcrat(int contractID, int contractTypeID,
			List<StandardQuery> querys){
		
		
		// 在sql语句中，收入合同的项目信息在后续sql里实现，所以临时表名用 _1
		// 支出合同，需要 “对应收入合同部门ID”的信息，所以将“对应收入合同部门ID”的处理从后续sql中提前，则临时表名 为 非 _1
		String tempTableName = (contractTypeID == 1) ? "#contractList_1"
				: "#contractList";

		String prevSQL = "\n select * into #result from " + tempTableName
				+ " \n";		
		
		
		
//		if(querys!=null && querys.size()>0){
//			prevSQL = "\n select * into #result from #contractList_1 where 合同ID = "
//				+ Type.getString(contractID) + "  \n";
//		}else{
//			prevSQL = "\n select * into #result from #contractList_1 where 合同ID = "
//				+ Type.getString(contractID) + "  \n";				
//				}
		
		
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByTypeWithoutAccount + prevSQL);
		
		if (contractTypeID == 1)
			querys.add(SimpleParam.buildSimpleParam(RelationalOperator.JDBCSQL,
					"where 合同ID =  ?", Type.getString(contractID)));
		else
			querys.add(SimpleParam.buildSimpleParam(RelationalOperator.JDBCSQL,
					"where 合同ID =  ?", "$" + Type.getString(contractID)
							+ " or 父合同ID = " + Type.getString(contractID) + "$"));

		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		
		
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), new PageInfo());
		
		sqlStr = sqlStr.replace("$'", "");
		sqlStr = sqlStr.replace("'$", "");

		Object[] args = new Object[] { contractTypeID };

		logger.info("---> hasFullRightsOnConcrat " + sqlStr);
		
		List aList = jt.queryForList(sqlStr, args);
		
		return (aList != null && aList.size() > 0) ? true : false;
		 
	}
	
	public List getCompanyListByInkling(StandardQuery query, PageInfo pageInfo) {
		SQLGenerator generator = new SQLGenerator(SQLScript.getCompanyList);
		query.addToSQLGenerator(generator);
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
		logger.info("===>  getCompanyListByInkling //" + sqlStr);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);
	}

	public void doSaveRelatedContract(final int contract1, final int contract2,
			final double amt, final String remark, final int contractID) {
		jt.update(SQLScript.saveContractRelated, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, contract1);
				ps.setInt(2, contract2);
				ps.setDouble(3, amt);
				ps.setString(4, remark);
				ps.setInt(5, contractID);
			}
		});
	}

	public ContractObj getContractInfoByNo(String relatedNo) {
		String sql = StringUtils.replace(SQLScript.getContractInfoByProjectNo,
				"?", "'%" + relatedNo + "'");
		if (logger.isDebugEnabled()) {
			logger.info(sql);
		}
		return (ContractObj) jt.query(sql, new ResultSetExtractor() {
			public Object extractData(ResultSet rs) throws SQLException,
					DataAccessException {
				if (rs.next()) {
					ContractObj con = new ContractObj();
					ProjectObj pro = new ProjectObj();
					pro.setProjectCode(rs
							.getString(DBSchema.Project.ProjectCode));
					con.setProjectOBJ(pro);
					con.setContractID(rs.getInt(DBSchema.Contract.ContractID));
					con.setAbbreviation(rs
							.getString(DBSchema.Contract.Abbreviation));
					return con;
				}
				return null;
			}
		});
	}

	public double getIncomAmtTotalByGl(String projectID) {

		logger.info("getIncomAmtTotalByGl --->"
				+ SQLScript.getGLIncomeByProjectCode);

		List list = jt.queryForList(WebUtil.generateSQLStr(
				SQLScript.getGLIncomeByProjectCode, new PageInfo()),
				new Object[] { projectID, projectID });
		double total = 0.0;
		if (list != null && list.size() > 0) {
			for (int i = 0; i < list.size(); i++) {
				Map map = (Map) list.get(i);
				total += Type.GetDouble(map.get("MB"));
			}
		}
		return total;
	}

	public double getElseIncomAmtTotal(String projectID,int type){
		
		logger.info("getElseIncomAmtTotal --->"
				+ SQLScript.getGLElseIncomeAmtByProjectID);

		List list = jt.queryForList(WebUtil.generateSQLStr(
				SQLScript.getGLElseIncomeAmtByProjectID, new PageInfo()),
				new Object[] { projectID, projectID ,type});
		double total = 0.0;
		if (list != null && list.size() > 0) {
			for (int i = 0; i < list.size(); i++) {
				Map map = (Map) list.get(i);
				total += Type.GetDouble(map.get("MB"));
			}
		}
		return total;
	}
	
	public double getPurAmtTotalByGl(String projectID) {

		logger.info("getPurAmtTotalByGl --->"
				+ SQLScript.getGLPurchaseByProjectCode);

		List list = jt.queryForList(WebUtil.generateSQLStr(
				SQLScript.getGLPurchaseByProjectCode, new PageInfo()),
				new Object[] { projectID, projectID });
		double total = 0.0;
		if (list != null && list.size() > 0) {
			for (int i = 0; i < list.size(); i++) {
				Map map = (Map) list.get(i);
				total += Type.GetDouble(map.get("MB"));
			}
		}
		return total;
	}

	public List getDelayReasonListByContractID(int contractID, PageInfo pageInfo) {
		String sqlStr = SQLScript.getDelayReasonListByContractID;

		logger.info("getDelayReasonListByContractID ? contractID="
				+ Type.getString(contractID) + sqlStr);

		return jt
				.queryForList(sqlStr, new Object[] { new Integer(contractID) });
	}

	public int getWarningDays(int contractID) {
		return jt.queryForInt(SQLScript.getWaringDays,
				new Object[] { new Integer(contractID) });
	}

	public boolean doUpdateDelayInfo(final DelayReason dr) {
		logger.info(" ---> doUpdateDelayInfo");
		boolean successFlag = true;
		try {
			if (dr.getId() == 0) { // 新数据
				this.doInsertDelayInfo(dr);
			} else {
				if (dr.getOperator() == 0) {
					jt.update(SQLScript.updateDelayReasonInfoWithoutOper,
							new PreparedStatementSetter() {
								public void setValues(PreparedStatement ps)
										throws SQLException {
									ps.setString(1, dr.getReason());
									ps.setInt(2, dr.getId());
								}
							});
				} else {
					jt.update(SQLScript.updateDelayReasonInfo,
							new PreparedStatementSetter() {
								public void setValues(PreparedStatement ps)
										throws SQLException {
									ps.setString(1, dr.getReason());
									ps.setInt(2, dr.getOperator());
									ps.setInt(3, dr.getId());
								}
							});
				}
			}
		} catch (Exception e) {
			successFlag = false;
			e.printStackTrace();
		}
		return successFlag;
	}

	private void doInsertDelayInfo(final DelayReason dr) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.DelayReason.ID, DBSchema.DelayReason.TableName)) + 1;

		jt.update(SQLScript.insertDelayReasonInfo,
				new PreparedStatementSetter() {
					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxID);
						ps.setInt(2, dr.getContractID());
						ps.setString(3, dr.getReason());
						ps.setInt(4, dr.getOperator());
						ps.setInt(5, dr.getRegistrant());
					}
				});
	}

	public boolean doDeleteDelayReasonInfo(final DelayReason dr) {
		boolean successFlag = true;
		try {
			jt.update(SQLScript.delDelayReasonInfo,
					new PreparedStatementSetter() {
						public void setValues(PreparedStatement ps)
								throws SQLException {
							ps.setInt(1, dr.getId());
						}
					});
		} catch (Exception e) {
			successFlag = false;
		}
		return successFlag;
	}

	/**
	 * 查询发票拆分信息
	 */
	public List getInvoiceSplitDetailBySimpleInvoice(InvoiceSplitObj iso) {
		String sqlStr = SQLScript.invoiceSplitDetailBySimpleInvoice;
		return jt.queryForList(sqlStr,
				new Object[] { iso.getContractID(), iso.getInvoiceNumber() });
	}

	/**
	 * 查询发票基本拆分信息
	 */
	public List getInvoiceSplitInfoBySimpleInvoice(InvoiceSplitObj iso) {
		String sqlStr = SQLScript.invoiceSplitInfoBySimpleInvoice;
		return jt.queryForList(sqlStr,
				new Object[] { iso.getContractID(), iso.getInvoiceNumber() });
	}

	/**
	 * 删除发票拆分信息
	 */
	public boolean doDeleteInvoiceSplitInfo(final InvoiceSplitObj iso) {
		boolean successFlag = true;
		try {
			jt.update(SQLScript.deleteInvoiceSplitInfo,
					new PreparedStatementSetter() {
						public void setValues(PreparedStatement ps)
								throws SQLException {
							ps.setInt(1, iso.getId());
						}
					});
		} catch (Exception e) {
			successFlag = false;
		}
		return successFlag;
	}

	/**
	 * 新建发票拆分信息
	 */
	public boolean doSaveInvoiceSplitInfo(final InvoiceSplitObj iso) {
		logger.info(" ---> doSaveInvoiceSplitInfo");
		boolean successFlag = true;

		try {
			final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
					DBSchema.InvoiceSplit.ID, DBSchema.InvoiceSplit.TableName)) + 1;
			jt.update(SQLScript.saveInvoiceSplitInfo,
					new PreparedStatementSetter() {
						public void setValues(PreparedStatement ps)
								throws SQLException {
							ps.setInt(1, maxID);
							ps.setInt(2, iso.getContractID());
							ps.setInt(3, iso.getInvoiceNumber());
							ps.setDouble(4, iso.getSplitSum());
							ps.setBoolean(5, iso.getPayFlag());
							ps.setDate(6, new Date(iso.getAnticipatedTime()
									.getTime()));
							ps.setString(7, iso.getRemark());
							ps.setInt(8, iso.getRegistrant());
						}
					});
		} catch (Exception e) {
			successFlag = false;
			e.printStackTrace();
		}
		return successFlag;
	}

	/**
	 * 更新发票拆分信息
	 */
	public boolean doUpdateInvoiceSplitInfo(final InvoiceSplitObj iso) {
		logger.info(" ---> doUpdateInvoiceSplitInfo");
		boolean successFlag = true;

		try {
			jt.update(SQLScript.updateInvoiceSplitInfo,
					new PreparedStatementSetter() {
						public void setValues(PreparedStatement ps)
								throws SQLException {
							ps.setDouble(1, iso.getSplitSum());
							ps.setBoolean(2, iso.getPayFlag());
							ps.setDate(3, new Date(iso.getAnticipatedTime()
									.getTime()));
							ps.setString(4, iso.getRemark());
							ps.setInt(5, iso.getId());
						}
					});
		} catch (Exception e) {
			successFlag = false;
			e.printStackTrace();
		}
		return successFlag;
	}

	/**
	 * 更新‘发票拆分’付款状态
	 */
	public boolean doUpdateInvoiceSplitStatus(final InvoiceSplitObj iso) {
		logger.info(" ---> doUpdateInvoiceSplitStatus");
		boolean successFlag = true;

		try {
			jt.update(SQLScript.updateInvoiceSplitIncomeStatus,
					new PreparedStatementSetter() {
						public void setValues(PreparedStatement ps)
								throws SQLException {
							ps.setBoolean(1, iso.getPayFlag());
							ps.setInt(2, iso.getId());
						}
					});
		} catch (Exception e) {
			successFlag = false;
			e.printStackTrace();
		}
		return successFlag;
	}

	/**
	 * 得到辅助查询列表
	 */
	public List getIncomeAssistList(List<StandardQuery> querys,
			PageInfo pageInfo) {
		SQLGenerator generator = new SQLGenerator(SQLScript.getIncomeAssistList);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
		Object[] args = new Object[] { new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		logger.info("---> getIncomeAssistList " + sqlStr);
		return jt.queryForList(sqlStr, args);
	}

	/**
	 * 通过合同ID 查找付款条件定义的预计金额总和
	 */
	public Map<String, String> getPaymentConditionTotalAmtById(int id) {
		String sqlStr = SQLScript.getPaymentConditionTotalAmtById;
		Object[] args = new Object[] { new Integer(id) };
		List list = jt.queryForList(sqlStr, args);
		Map<String, String> result = null;
		if (list != null && list.size() == 1) {
			Map map = (Map) list.get(0);
			result = new HashMap<String, String>();
			result.put("amt", map.get("amt") + "");
			result.put("percent", map.get("percentage") + "");
		}
		return result;
	}

	public List getContractRelationInfo(int pCID, int relatedID) {
		logger.info("---> getContractRelationInfo " + pCID + ":" + relatedID);
		String sql = SQLScript.getContractRelationInfo;
		logger.info(sql);
		return jt.queryForList(sql, new Object[] { pCID, relatedID });
	}

	public void updateRelatedContract(final int inContractID,
			final String contractNo, final double relateAmt,
			final String remark, final int relatedID, final int outContractID) {
		logger.info("-->updateRelatedContract:" + relatedID);
		String sql = SQLScript.updateRelatedContract;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inContractID);
				ps.setDouble(2, relateAmt);
				ps.setString(3, remark);
				ps.setInt(4, relatedID);
				ps.setInt(5, relatedID);
				ps.setInt(6, outContractID);
				ps.setString(7, contractNo);
				ps.setDouble(8, relateAmt);
			}
		});
	}

	public Stencil getStencilByDeptId(int deptID) {
		logger.info("--->getStencilByDeptId");

		String sql = SQLScript.getStencilByDeptId;
		Stencil stencil = null;
		List list = jt.queryForList(sql, new Object[] { deptID });
		if (list != null && list.size() > 0) {
			Map map = (Map) list.get(0);
			int stencilId = Type.GetInt(map.get(DBSchema.Stencil.StencilID));
			int deptId = Type.GetInt(map.get(DBSchema.Stencil.DeptID));
			String content = Type.getString(map.get(DBSchema.Stencil.Content));
			String keyWords = Type
					.getString(map.get(DBSchema.Stencil.KeyWords));
			stencil = new Stencil(stencilId, deptID, content, keyWords);
		}
		return stencil;
	}

	public String getContractSummaryByConId(int conID) {
		logger.info("--->getContractSummaryByConId");
		String sql = SQLScript.getContractSummaryByConId;
		List list = jt.queryForList(sql, new Object[] { conID });
		return (list != null && list.size() > 0) ? Type.getString((((Map) list
				.get(0)).get("summary"))) : "";
	}

	public int getParentDeptIdByDeptId(final int deptID) {
		logger.info("--->getParentDeptIDByDeptID");
		String sql = SQLScript.getParentDeptIDByDeptID;
		List list = jt.queryForList(sql, new Object[] { deptID });
		int result = 0;
		if (list != null && list.size() > 0) {
			result = Type.getInteger(((Map) list.get(0))
					.get(DBSchema.DeptBelongToDept.ParentsID));
		}
		return result;
	}

	public void doUpdateProjectStatusName(final int psid, final String name) {
		logger.info("-->updateProjectStatusName");
		String sql = SQLScript.updateProjectStatusName;
		try {
			jt.update(sql, new PreparedStatementSetter() {
				public void setValues(PreparedStatement ps) throws SQLException {
					ps.setString(1, name);
					ps.setInt(2, psid);
				}
			});
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void doUpdatePaymentConditionDate(final String projectID,
			final Integer statusId) {
		String sql = "UPDATE  xmis.con_收款条件表"
				+ " SET     收款日期 = DATEADD(DAY, 延迟天数, GETDATE())"
				+ " FROM    xmis.con_收款条件表 p"
				+ "       LEFT JOIN xmis.con_合同 c ON p.合同ID = c.合同ID"
				+ "      LEFT JOIN xmis.prj_项目 pro ON c.项目ID = pro.项目ID"
				+ " WHERE   pro.项目ID =? AND p.对应项目状态=?";
		jt.update(sql, new PreparedStatementSetter() {

			public void setValues(PreparedStatement arg0) throws SQLException {
				arg0.setString(1, projectID);
				arg0.setInt(2, statusId);
			}
		});
	}

	public List getContractListByIntegrationEstimateContractAmt(
			int contractTypeID, String begDate, String endDate,
			PageInfo pageInfo, int userID, List<StandardQuery> querys) {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils
				.replace(SQLScript.getEstimateContractAmtContractInTimeScope,
						SQLScript.getContractListByType.replace("#result",
								"#contract"),
						generator.getSQL().replace("#result", "#contract")),
				pageInfo,
				SQLScript.getResultStringWithPagingOfContractByPayInfo);

		logger.info("===>  getEstimateContractAmtContractInTimeScope //"
				+ Type.getString(contractTypeID) + sqlStr);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID, begDate,
				endDate, begDate, endDate, begDate, endDate, begDate, endDate,
				begDate, begDate, endDate } : new Object[] { contractTypeID,
				begDate, endDate, new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };

		return jt.queryForList(sqlStr, args);
	}

	public List getContractListByIntegrationFinance(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys) {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils
				.replace(SQLScript.getFinanceContractAmtContractInTimeScope,
						SQLScript.getContractListByType.replace("#result",
								"#contract"),
						generator.getSQL().replace("#result", "#contract")),
				pageInfo,
				SQLScript.getResultStringWithPagingOfContractByPayInfo);

		logger.info("===>  getFinanceContractAmtContractInTimeScope //"
				+ Type.getString(contractTypeID) + sqlStr);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID, begDate,
				endDate, begDate, endDate, begDate, endDate, begDate, endDate,
				begDate, begDate, endDate } : new Object[] { contractTypeID,
				begDate, endDate, new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };

		return jt.queryForList(sqlStr, args);
	}

	public List getContractListByIntegrationInvoice(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys) {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractListByType);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}

		String sqlStr = WebUtil.generateSQLStr(StringUtils
				.replace(SQLScript.getInvoiceContractInTimeScope,
						SQLScript.getContractListByType.replace("#result",
								"#contract"),
						generator.getSQL().replace("#result", "#contract")),
				pageInfo,
				SQLScript.getResultStringWithPagingOfContractByPayInfo);

		logger.info("===>  getInvoiceContractInTimeScope //"
				+ Type.getString(contractTypeID) + sqlStr);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { contractTypeID, begDate,
				endDate, begDate, endDate, begDate, endDate, begDate, endDate,
				begDate, begDate, endDate } : new Object[] { contractTypeID,
				begDate, endDate, new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };

		return jt.queryForList(sqlStr, args);
	}

	public int getContractIDByProjectID(String projectID) {
		String sql = "select 合同ID from xmis.con_合同 where 项目ID = ? and 父合同ID is null";
		return jt.queryForInt(sql, new Object[] { projectID });
	}
	
	public int doUpdateContractFlagByProjectID(String projectID) {
		String sql = "update xmis.con_合同 SET 新建标志='2' WHERE 项目ID=?";
		return jt.update(sql, new Object[] { projectID });
	}
	

	public void doSaveEstimatedInfo(final EstimatedPayment ep) {
		String sql = "insert into xmis.con_预计来款明细表 values(?,?,?,?,?,0,?,getdate(),getdate(),?);";
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, ep.getId());
				ps.setString(2, ep.getProjectId());
				ps.setDate(3, ep.getEstimatedDate());
				ps.setDouble(4, ep.getEstimatedAmt());
				ps.setDouble(5, ep.getEstimatedPercent());
				ps.setInt(6, ep.getRegisterId());
				ps.setInt(7, ep.getLastModifiedId());
			}
		});

	}

	// 项目资质字典信息
	public List getContractInfoByParentID(int parentID) {
		String sqlStr = SQLScript.getContractInfoByParentID;
		logger.info("getContractInfoByParentID--->parentID=" + parentID
				+ "   sql:" + sqlStr);
		return jt.queryForList(sqlStr, new Object[] { new Integer(parentID) });
	}

	public List getContractInfoByType(int type) {
		String sqlStr = SQLScript.getContractInfoByType;
		logger.info("getContractInfoByType---->type=" + type + "    sql:"
				+ sqlStr);
		return jt.queryForList(sqlStr, new Object[] { new Integer(type) });
	}

	public List getProjectCodeTypeByParentID(int parentID) {
		String sqlStr = SQLScript.getProjectCodeTypeByParentID;
		return jt.queryForList(sqlStr, new Object[] { new Integer(parentID) });
	}

	public void doSaveContractInfoObj(final ContractInfoObj conInfoObj) {
		String sqlExist = SQLScript.existConInfo;
		String sqlSave = SQLScript.saveContractInfoObj;
		String sqlUpdate = SQLScript.updateContractInfoObj;
		int conID = Type.GetInt(conInfoObj.getContractID());

		int number = jt.queryForInt(sqlExist,
				new Object[] { new Integer(conID) });
		if (number > 0) {
			jt.update(sqlUpdate, new PreparedStatementSetter() {
				public void setValues(PreparedStatement ps) throws SQLException {
					ps.setInt(1, conInfoObj.getProjectResourceParentID());
					ps.setInt(2, conInfoObj.getProjectResourceID());
					ps.setInt(3, conInfoObj.getProjectOperationTypeParentID());
					ps.setInt(4, conInfoObj.getProjectOperationTypeID());
					ps.setInt(5, conInfoObj.getProjectPropertyID());
					ps.setInt(6, conInfoObj.getAccountingAgencyID());
					ps.setInt(7, conInfoObj.getExecutiveOrgID());
					ps.setInt(8, conInfoObj.getContractID());
				}
			});
		} else if (number == 0) {
			jt.update(sqlSave, new PreparedStatementSetter() {
				public void setValues(PreparedStatement ps) throws SQLException {
					ps.setInt(1, conInfoObj.getContractID());
					ps.setInt(2, conInfoObj.getProjectResourceParentID());
					ps.setInt(3, conInfoObj.getProjectResourceID());
					ps.setInt(4, conInfoObj.getProjectOperationTypeParentID());
					ps.setInt(5, conInfoObj.getProjectOperationTypeID());
					ps.setInt(6, conInfoObj.getProjectPropertyID());
					ps.setInt(7, conInfoObj.getAccountingAgencyID());
					ps.setInt(8, conInfoObj.getExecutiveOrgID());
				}
			});
		}
	}

	public List getDeptCode() {
		String sqlStr = SQLScript.getDeptCode;
		return jt.queryForList(sqlStr);
	}

	public List getProjectNumber(String projectNumber) {
		String a = "'%" + projectNumber + "%'";
		String sqlStr = StringUtils.replace(SQLScript.getProjectNumber, "?", a);
		logger.info("getProjectNumber--->a=" + a + sqlStr);
		return jt.queryForList(sqlStr);
	}

	public int existProjectNumber(String projectCode) {
		String sqlStr = StringUtils.replace(SQLScript.existProjectNumber, "?",
				"'" + projectCode + "'");
		return jt.queryForInt(sqlStr);
	}

	public List getConInfoByProjectCode(String projectCode) {
		String sqlStr = StringUtils.replace(SQLScript.getConInfoByProjectCode,
				"?", "'" + projectCode + "'");
		return jt.queryForList(sqlStr);
	}

	// 合同金额分解
	public int doCreateNewConQualifyManageNull(final int type) {
		final int maxTypeID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.ContractManagement.ID,
				DBSchema.ContractManagement.TableName)) + 1;

		jt.update(SQLScript.insertConQualifyManage,
				new PreparedStatementSetter() {
					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxTypeID);
						ps.setInt(2, type);
					}
				});

		return maxTypeID;
	}

	public void doCreateNewConTextSplitNull(final int contractID,
			final int typeID, final int userID) {
		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.conMoneySplit.ID, DBSchema.conMoneySplit.TableName)) + 1;

		jt.update(SQLScript.insertConTextSplit, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, maxID);
				ps.setInt(2, contractID);
				ps.setInt(3, typeID);
				ps.setInt(4, userID);
			}
		});
	}

	public void doSaveConQualifyManage(final MoneySplitObj moneySplit) {
		String sqlUpdate = SQLScript.updateConQualifyManage;
		jt.update(sqlUpdate, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, moneySplit.getName());
				ps.setString(2, moneySplit.getDescription());
				ps.setInt(3, moneySplit.getSplitTypeID());
				ps.setInt(4, moneySplit.getType());
			}
		});
	}

	public void doSaveConMoneySplit(final MoneySplitObj moneySplit) {
		String sqlUpdate = SQLScript.updateConMoneySplit;
		jt.update(sqlUpdate, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setDouble(1, moneySplit.getMoney());
				ps.setDouble(2, moneySplit.getPercent());
				ps.setInt(3, moneySplit.getLastModificatorID());
				ps.setInt(4, moneySplit.getContractID());
				ps.setInt(5, moneySplit.getSplitTypeID());
			}
		});
	}

	public void doDeleteConMoneySplit(final int contractID, final int typeID) {
		String sql = SQLScript.deleteConMoneySplit;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, contractID);
				ps.setInt(2, typeID);
			}
		});
	}

	public void doDeleteQualifyManage(final int typeID) {
		String sql = SQLScript.deleteQualifyManage;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, typeID);
			}
		});
	}

	public List getConTextSplitList(final int contractID, final int type) {
		String sql = SQLScript.getConTextSplitList;
		return jt.queryForList(sql, new Object[] { new Integer(contractID),
				new Integer(type) });
	}

	public int existMoneySplit(final int contractID, final int typeID) {
		String sql = SQLScript.existMoneySplit;
		return jt.queryForInt(sql, new Object[] { new Integer(contractID),
				new Integer(typeID) });
	}

	public List getContractStateList(List<StandardQuery> querys,
			PageInfo pageInfo) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.getContractStateList);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
		logger.info("getContractStateList:" + sqlStr);
		return jt.queryForList(sqlStr, new Object[] { Math.random() });

		// SQLGenerator generator = new SQLGenerator(
		// SQLScript.getContractStateList);
		// for (StandardQuery query : querys) {
		// query.addToSQLGenerator(generator);
		// }
		// String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
		// List alist = (List) jt.execute(sqlStr, new
		// PreparedStatementCallback() {
		// public Object doInPreparedStatement(PreparedStatement arg0)
		// throws SQLException, DataAccessException {
		// ResultSet rs = arg0.executeQuery();
		// List list = new ArrayList();
		// ResultSetMetaData md = rs.getMetaData();
		// int columnCount = md.getColumnCount();
		// while (rs.next()) {
		// Map map = new HashMap();
		// for (int i = 1; i <= columnCount; i++) {
		// map.put(md.getColumnName(i), rs.getObject(i));
		// }
		// list.add(map);
		// }
		// return list;
		// }
		// });
		// return alist;
	}

	public int isEmpBelongDept(int deptId, int empId) {
		String sql = SQLScript.isEmpBelongDept;
		return jt.queryForInt(sql, new Object[] { new Integer(deptId),
				new Integer(empId) });
	}

	public List getPaymentPromptList() {
		String sql = SQLScript.getPaymentPromptList;
		return jt.queryForList(sql, new Object[] { Math.random() });
	}

	public List getScientificName(int deptId) {
		String sql = SQLScript.getScientificName;
		List list = jt.queryForList(sql, new Object[] { deptId });
		return list;
	}

	public List getContractCodeNumber(String contractCode) {
		String a = "'%" + contractCode + "%'";
		String sqlStr = StringUtils.replace(SQLScript.getContractCodeNumber,
				"?", a);
		logger.info("getContractCodeNumber--->a=" + a + sqlStr);
		return jt.queryForList(sqlStr);
	}

	public int existContractCode(String contractCode) {
		String sqlStr = StringUtils.replace(SQLScript.existContractCode, "?",
				"'" + contractCode + "'");
		return jt.queryForInt(sqlStr);
	}

	public void doUpdateEstimatedInfo(final EstimatedPayment ep) {
		String sql = "update xmis.con_预计来款明细表 set 预计到款时间=?,预计金额=?,收款百分比=?,是否已到款=?,最后修改日期=getdate(),最后修改人ID=? where ID=?";
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setDate(1, ep.getEstimatedDate());
				ps.setDouble(2, ep.getEstimatedAmt());
				ps.setDouble(3, ep.getEstimatedPercent());
				ps.setInt(4, ep.getIsPayed());
				ps.setInt(5, ep.getLastModifiedId());
				ps.setString(6, ep.getId());
			}
		});

	}

	public List getProjectEstimatedPaymentList(String projectID) {
		String sql = "select xmis.con_预计来款明细表.ID \n"
				+ " ,xmis.con_预计来款明细表.项目ID \n"
				+ " ,Convert(varchar(10),xmis.con_预计来款明细表.预计到款时间,101) as 预计到款时间 \n"
				+ " ,xmis.con_预计来款明细表.预计金额 \n"
				+ " ,xmis.con_预计来款明细表.收款百分比 \n"
				+ " ,xmis.con_预计来款明细表.是否已到款 \n"
				+ " ,xmis.con_预计来款明细表.登记人ID \n"
				+ " ,xmis.con_预计来款明细表.登记日期 \n"
				+ " ,xmis.con_预计来款明细表.最后修改日期 \n"
				+ " ,xmis.con_预计来款明细表.最后修改人ID \n"
				+ " ,rtrim(xmis.hr_人员.姓氏) + rtrim(xmis.hr_人员.名字) as 登记人 \n"
				+ " from xmis.con_预计来款明细表,xmis.hr_人员 \n"
				+ " where xmis.con_预计来款明细表.登记人ID = xmis.hr_人员.人员ID and xmis.con_预计来款明细表.项目ID = ? order by xmis.con_预计来款明细表.预计到款时间 \n";
		return jt.queryForList(sql, new Object[] { projectID });
	}

	public void doDeleteEstimatedInfo(String estimatedID) {
		String sql = "delete from xmis.con_预计来款明细表 where id = ? and 是否已到款=0";
		jt.update(sql, new Object[] { estimatedID });
	}

	public void doUpdateEstimatedPayedFlag(final EstimatedPayment ep) {
		String sql = "update xmis.con_预计来款明细表 set 是否已到款=1,最后修改日期=getdate(),最后修改人ID=? where ID=?";
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, ep.getLastModifiedId());
				ps.setString(2, ep.getId());
			}
		});

	}

	public void doUpdatePaymentConditionPayedFlag(final PaymentConditionObj pc) {
		String sql = "update [xmis].[xmis].[con_收款条件表] set 是否已到款 = 1,最后修改人ID=?,最后修改日期=GETDATE() where 合同ID=? and 收款条件ID=?";
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, pc.getLastModificatorID());
				ps.setInt(2, pc.getContractID());
				ps.setInt(3, pc.getPaymentConditionID());
			}
		});
	}

	public void doUpdateInvoicePayedFlag(final InvoiceObj iv) {
		String sql = "update [xmis].[xmis].[con_发票开具表] set 是否已到款=1,最后修改人ID=?,最后修改日期=GETDATE() where 合同ID=? and 发票号=?";
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, iv.getLastModificatorID());
				ps.setInt(2, iv.getContractID());
				ps.setInt(3, iv.getInvoiceID());
			}
		});
	}

	public void doUpdateReceiptPayedFlag(final InvoiceObj rp) {
		String sql = "update [xmis].[xmis].[con_收据明细表] set 是否已到款=1,最后修改人ID=?,最后修改日期=GETDATE() where 合同ID=? and 收据ID=?";
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, rp.getLastModificatorID());
				ps.setInt(2, rp.getContractID());
				ps.setInt(3, rp.getReceiverId());
			}
		});
	}

	public double getAmtByConID(int contractId, int flag) {
		List list = null;
		if (flag == 1) {
			list = jt.queryForList(SQLScript.getAmtByConId,
					new Object[] { contractId });
		} else if (flag == 2) {
			list = jt.queryForList(SQLScript.getSumAmtByConID,
					new Object[] { contractId });
		}
		double total = 0.0;
		if (list != null && list.size() > 0) {
			for (int i = 0; i < list.size(); i++) {
				Map map = (Map) list.get(i);
				total += Type.GetDouble(map.get("amt"));
			}
		}
		return total;
	}

	public void updateContractNewflag(final int contractId, final int newflag) {
		String sqlUpdate = SQLScript.updateContractNewflag;
		jt.update(sqlUpdate, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, newflag);
				ps.setInt(2, contractId);
			}
		});

	}

	public List getContractInfoByContractCode(String contractCode) {
		String sqlStr = StringUtils.replace(
				SQLScript.getContractInfoByContractCode, "?", "'"
						+ contractCode + "'");
		return jt.queryForList(sqlStr);
	}

	public List getContractAmtSplitListByQuerys(List<StandardQuery> querys,
			PageInfo pageInfo) {

		SQLGenerator generator = new SQLGenerator(
				SQLScript.contractAmtSplitList);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo,
				SQLScript.getContractAmtSplitListPageInfo);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {  }
				: new Object[] {
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) };
		return jt.queryForList(sqlStr,args);	

	}
	public String getOutContractDelimiter(){
		String sqlStr = SQLScript.getOutContractDelimiter;
		List list = jt.queryForList(sqlStr);
		return (list != null && list.size() > 0) ? Type.getString((((Map) list
				.get(0)).get("delimiter"))) : "";
	}

	public String getIncomeContractPrefix(){
		String sqlStr = SQLScript.getIncomeContractPrefix;
		List list = jt.queryForList(sqlStr);
		return (list != null && list.size() > 0) ? Type.getString((((Map) list
				.get(0)).get("prefix"))) : "";
	}
	
	public void doSavePayoutContractInfo(final ContractObj con, final int userID) {
		logger.info("===> doSaveContractInfo"
				+ Type.getString(con.getDeliveryID()));
		jt.update(SQLScript.doSavePayoutContractInfo,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {

//						ps.setInt(1, con.getContractTypeID());//合同类型ID
						ps.setInt(1, con.getSourceTypeID());

						if (con.getGovermentContractSourceID() == 0)
							ps.setNull(2, Types.INTEGER);
						else
							ps.setInt(2, con.getGovermentContractSourceID());

//						ps.setString(4, con.getContractNo());//合同编号
						ps.setString(3, con.getContractName());

						if ("".equals(con.getAbbreviation()))
							ps.setNull(4, Types.VARCHAR);
						else
							ps.setString(4, con.getAbbreviation());

						if ("".equals(con.getContractSummary()))
							ps.setNull(5, Types.VARCHAR);
						else
							ps.setString(5, con.getContractSummary());

//						if (con.getParentID() == 0)//合同父ID
//							ps.setNull(8, Types.INTEGER);
//						else
//							ps.setInt(8, con.getParentID());

						if ("".equals(con.getWarrantyDefinition()))
							ps.setNull(6, Types.VARCHAR);
						else
							ps.setString(6, con.getWarrantyDefinition());

						ps.setDate(7, con.getWarrantyStartDate());

						if (con.getWarrantyEndDate() == null)
							ps.setNull(8, Types.DATE);
						else
							ps.setDate(8, con.getWarrantyEndDate());

//						ps.setDouble(12, con.getContractAmt());//合同总额
						ps.setInt(9, con.getCurrencyID());
						ps.setDate(10, con.getSignatureDate());
						ps.setInt(11, con.getBuyerID());

						if (con.getDeptIDOfBuyer() == 0)
							ps.setNull(12, Types.INTEGER);
						else
							ps.setInt(12, con.getDeptIDOfBuyer());

						if (con.getBuyerRepresentativeID() == 0)
							ps.setNull(13, Types.INTEGER);
						else
							ps.setInt(13, con.getBuyerRepresentativeID());

						ps.setInt(14, con.getVendorID());

						if (con.getDeptIDOfVendor() == 0)
							ps.setNull(15, Types.INTEGER);
						else
							ps.setInt(15, con.getDeptIDOfVendor());

						if (con.getVendorRepresentativeID() == 0)
							ps.setNull(16, Types.INTEGER);
						else
							ps.setInt(16, con.getVendorRepresentativeID());

						if ("".equals(con.getRemark()))
							ps.setNull(17, Types.VARCHAR);
						else
							ps.setString(17, con.getRemark());

						if (con.getDeliveryID() == 0)
							ps.setNull(18, Types.INTEGER);
						else
							ps.setInt(18, con.getDeliveryID());

						ps.setInt(19, con.getLastModificator());

						ps.setFloat(20, con.getWarrantyLimit());
						ps.setInt(21, con.getWarrantyUnit());
						ps.setString(22, con.getProjectOBJ().getProjectID());
						if (con.getDirectorID() == null) {
							ps.setNull(23, Types.INTEGER);
						} else {
							ps.setInt(23, con.getDirectorID());
						}
						ps.setDate(24, con.getValidityStartDate());
						ps.setDate(25, con.getValidityEndDate());
						ps.setFloat(26, con.getValidityLimit());
						ps.setInt(27, con.getValidityUnit());
						ps.setInt(28, con.getNewflag());
						ps.setInt(29, con.getContractID());

					}
				});
	}
	
	public int isEmpDeptSpecial(int deptId, int empId) {
		String sql = SQLScript.isEmpDeptSpecial;
		return jt.queryForInt(sql, new Object[] { new Integer(empId),
				new Integer(deptId) });
	}
	
	public List getContractSourceTypeID(int level) {
		String sqlStr = "";
		if(level == 0){
			sqlStr = SQLScript.getContractSourceTypeID;
		}
		if(level ==1){
			sqlStr = SQLScript.getContractSourceTypeForEditContract;
		}
		return jt.queryForList(sqlStr);
	}
	
	public List getExcuteDeptInfo(String deptIds) {
		String sql = SQLScript.getExcuteDeptInfo;
		deptIds = "(" + deptIds + ")";
		sql = sql.replace("?", deptIds);
		return jt.queryForList(sql);
	}
	
	public List getOutContractDelimiterStroe(){
		String sqlStr = SQLScript.getOutContractDelimiterStore;
		List list = jt.queryForList(sqlStr);
		return jt.queryForList(sqlStr);
	}

	public Map getContractMessage(String contractID) {
		String sql = "select t1.合同名称,t1.合同编号,t2.code from xmis.con_合同 t1,xmis.org_部门 t2 where t1.乙方部门ID = t2.部门ID and t1.合同ID = ?";
		return jt.queryForMap(sql, new Object[]{contractID});
	}

	public boolean fileIsExist(String fileName) {
		boolean flag = false;
		String sql = "select count(*) from xmis.sys_附件 where 文件名 = ?";
		if(jt.queryForInt(sql, new Object[]{fileName})>0){
			flag = true;
		}
		return flag;
	}

}
