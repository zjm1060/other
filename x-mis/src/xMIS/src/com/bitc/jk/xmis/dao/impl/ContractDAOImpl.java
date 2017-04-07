//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\impl\\ContractDAOImpl.java

package com.bitc.jk.xmis.dao.impl;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Types;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementSetter;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.core.support.AbstractLobCreatingPreparedStatementCallback;
import org.springframework.jdbc.support.lob.DefaultLobHandler;
import org.springframework.jdbc.support.lob.LobCreator;
import org.springframework.jdbc.support.lob.LobHandler;

import com.bitc.jk.xmis.dao.ContractDAO;
import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.GContractSourceObj;
import com.bitc.jk.xmis.model.InvoiceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PaymentConditionObj;
import com.bitc.jk.xmis.model.PaymentDetailObj;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

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

		String sqlStr = WebUtil.generateSQLStr(SQLScript.getPaymentListByID,
				pageInfo);

		logger.info("getPaymentListByID ? contractID="
				+ Type.getString(contractID) + sqlStr);

		return jt
				.queryForList(sqlStr, new Object[] { new Integer(contractID) });
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

		logger.info("===> getContractInfo" + SQLScript.getContractInfo);

		return jt.queryForMap(SQLScript.getContractInfo,
				new Object[] { new Integer(contractID) });

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

		logger.info("===> getGovernmentContractSourceList " + sqlStr);

		return jt.queryForList(sqlStr);

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
			int parentID, String searchName, PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLForPosition(
				SQLScript.getContractListForParentBySerch,
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

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), "%" + searchName + "%",
				"%" + searchName + "%", "%" + searchName + "%", new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });

	}

	public List getContractListByTypeFilterWarning(int contractTypeID,
			int warningType, PageInfo pageInfo, int userID) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getContractListByTypeFilterWarning, pageInfo);

		logger.info("===>  getContractListByTypeFilterWarning //"
				+ Type.getString(warningType) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), new Integer(warningType),
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });

	}
	
	public List getContractListByTypeForCurrency(int contractTypeID,
			String searchName, String currencyFlag, PageInfo pageInfo,
			int userID){

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getContractListByTypeForCurrency, pageInfo);

		logger.info("===>  getContractListByTypeForCurrency //"
				+ Type.getString(currencyFlag) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), 
				"%" + searchName + "%",
				currencyFlag,
				"%" + searchName + "%", 
				currencyFlag,
				"%" + searchName + "%", 
				currencyFlag,
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });
		
	}

	public List getContractListForParent(int contractTypeID, String searchName,
			PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getContractListForParentBySerch, pageInfo);

		logger.info("===>  getContractListForParent //"
				+ Type.getString(contractTypeID) + "//" + searchName + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), "%" + searchName + "%",
				"%" + searchName + "%", "%" + searchName + "%", new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });

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

						ps.setInt(1, maxID);
						ps.setInt(2, attFile.getUploadID());
						ps.setString(3, attFile.getFileName());

						lobCreator.setBlobAsBinaryStream(ps, 4, attFile
								.getFis(), attFile.getFileSize());

						ps.setInt(5, attFile.getAttachmentTypeID());
						ps.setInt(6, attFile.getRefID());

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

	public int getAttachmentTypeID(final String tableName,
			final String fieldName) {

		logger.info("===> getAttachmentTypeID ? tableName=" + tableName
				+ "&fieldName=" + fieldName + SQLScript.getAttachmentTypeID);

		return jt.queryForInt(SQLScript.getAttachmentTypeID, new Object[] {
				tableName, fieldName });
	}

	public int createNewAttachmentTypeID(final String tableName,
			final String fieldName) {

		logger
				.info("===> createNewAttachmentTypeID ? tableName=" + tableName
						+ "&fieldName=" + fieldName
						+ SQLScript.createNewAttachmentType);

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

				java.io.File fileOutput = new java.io.File(directPath
						+ fileName);
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

				att.setFileName(fileName);
				return null;
			}
		});

		logger.info(att.getFileName());
		return directPath + att.getFileName();

		// get BinaryStream
		// Object inputStream = jt.execute(SQLScript.getAttachmentContent,
		// new CallableStatementCallback() {
		//		
		// public Object doInCallableStatement(CallableStatement cs)
		// throws SQLException, DataAccessException {
		// cs.setInt(1, new Integer(attachmentID));
		// ResultSet rs = cs.executeQuery();
		// // InputStream iStream = null;
		// while (rs.next()) {
		// InputStream iStream = rs
		// .getBinaryStream(DBSchema.Attachment.AttachmentContent);//
		// 读取数据库Binary流
		//		
		// // InputStream inputStream = blob.getBinaryStream();
		// String fileName = rs
		// .getString(DBSchema.Attachment.AttachmentName);
		// System.out.println(fileName);
		// fileName = "d://" + fileName.trim();
		// System.out.println(fileName);
		//		
		// File fileOutput = new File(fileName);
		// FileOutputStream fo;
		// try {
		// fo = new FileOutputStream(fileOutput);
		// int c;
		//		
		// while ((c = iStream.read()) != -1) {
		// fo.write(c);
		// }
		//		
		// fo.close();
		// System.out.println("Blob reserved");
		// } catch (FileNotFoundException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// } catch (IOException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		// }
		//		
		// return null;
		// }
		// });

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

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(needRemovedCompanyID),
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });

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

		jt.update(SQLScript.updateContractInfo, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {

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

				ps.setInt(23, con.getRegisterID());

				ps.setFloat(24, con.getWarrantyLimit());
				ps.setInt(25, con.getWarrantyUnit());

				ps.setInt(26, con.getContractID());

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

	public int doCreateNewProjectStatus(final String statusName,
			final String desc, final int userID) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.ProjectStatus.ProjectStatusID,
				DBSchema.ProjectStatus.TableName)) + 1;

		jt.update(SQLScript.intsertProjectStatus,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxID);
						ps.setString(2, statusName);
						ps.setString(3, desc);
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
	
	private void doCreateNewPaymentConditionLog(final PaymentConditionObj pay, final int userID){
		
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
						ps.setInt(5, pay.getPaymentPercent());
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
							ps.setNull(1, Types.VARCHAR);
						else
							ps.setString(1, pay.getDescription());

						ps.setInt(2, pay.getProjectStatusMapping());

						// if (pay.getDelayDays() == 0)
						// ps.setNull(5, Types.INTEGER);
						// else
						ps.setInt(3, pay.getDelayDays());

						if (pay.getPaymentDate() == null)
							ps.setNull(4, Types.DATE);
						else
							ps.setDate(4, pay.getPaymentDate());

						if (pay.getManualFlag() == null
								|| "false".equals(pay.getManualFlag()))
							ps.setInt(5, 0);
						else
							ps.setInt(5, 1);

						if (pay.getPaymentPercent() == 0)
							ps.setNull(6, Types.INTEGER);
						else
							ps.setInt(6, pay.getPaymentPercent());

						ps.setDouble(7, pay.getPamymentAmt());

						if("".equals(pay.getManualReason()))
							ps.setNull(8, Types.CHAR);
						else
							ps.setString(8,	pay.getManualReason());

						ps.setInt(9, pay.getContractID());
						ps.setInt(10, pay.getPaymentConditionID());

					}
				});

		if("true".equals(pay.getManualFlag())) this.doCreateNewPaymentConditionLog(pay, userID);
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

		return jt
				.queryForLong(sqlStr, new Object[] { new Integer(contractID) });
	}

	public List getInvoiceListByID(int contractID, PageInfo pageInfo) {

		String sqlStr = WebUtil.generateSQLStr(SQLScript.getInvoiceListByID,
				pageInfo);

		logger.info("getInvoiceListByID ? contractID="
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
		
		logger.info("+++++++++++++++++++" + maxID);

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
		
		logger.info("+++++++++++++++++++" + maxID);

		jt.update(SQLScript.insertInvoiceDetail, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, contractID);
				ps.setInt(2, maxID);
				ps.setInt(3, userID);

			}
		});
		return maxID;
	}	
	
	/**********
	 * invoice
	 * 
	 * txh
	 */

	public void doDelInvoiceDetail(final int contractID, final int invoiceDetailID, final int userID){


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
	
	public int doSaveInvoiceDetail(final InvoiceObj invo, final int userID) {

		jt.update(SQLScript.saveInvoiceDetail, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {

				
				ps.setDouble(1, invo.getInvoiceAMT());
				ps.setDate(2, invo.getInvoiceDate());
				ps.setInt(3, invo.getTransactorID());
				ps.setInt(4, userID);
				if (invo.getInvoiecRemark() == null || "".equals(invo.getInvoiecRemark()))
					ps.setNull(5, Types.VARCHAR);
				else
					ps.setString(5, invo.getInvoiecRemark());
				
				if (invo.getInvoiceCode() == null || "".equals(invo.getInvoiceCode()))
					ps.setNull(6, Types.VARCHAR);
				else
					ps.setString(6, invo.getInvoiceCode());

				ps.setInt(7, invo.getContractID());
				ps.setInt(8, invo.getInvoiceID());
			}
		});

		return invo.getInvoiceID();
	}

	public List getContractListByIntegrationNewType(int contractTypeID, String begDate,
			String endDate, PageInfo pageInfo, int userID) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getContractListByIntegrationNewType, pageInfo);

		logger.info("===>  getContractListByIntegration //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), begDate,
				endDate, new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });

	}
	
	public List getContractListByIntegrationPerformed(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID){

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getRunContractInTimeScope, pageInfo);

		logger.info("===>  getContractListByIntegrationPerformed //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), 
				begDate,
				endDate, 
				begDate,
				endDate,
				begDate,
				endDate, 
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });
		
	}
	
	
	public List getContractListByIntegrationWarranty(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID){

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getContractListByIntegrationWarranty, pageInfo);

		logger.info("===>  getContractListByIntegrationWarranty //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), 
				begDate,
				endDate, 
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });
		
	}	
	public List getSumInfoByIntegrationNewType(int contractTypeID, String begDate,
			String endDate, PageInfo pageInfo, int userID) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getSumInfoByIntegrationNewType, pageInfo);

		logger.info("===>  getSumInfoByIntegrationNewType //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), begDate,
				endDate });

	}
	
//	getSumInfoByIntegrationPerformed
	public List getSumInfoByIntegrationPerformed(int contractTypeID, String begDate,
			String endDate, PageInfo pageInfo, int userID) {
	
		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getSumInfoByIntegrationPerformed, pageInfo);

		logger.info("===>  getSumInfoByIntegrationPerformed //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), 
				begDate,
				endDate, 
				begDate,
				endDate,
				begDate,
				endDate 
				});

	}	
	
	
	public List getSumInfoByIntegrationSimpleType(int contractTypeID, String searchName,
			PageInfo pageInfo, int userID) {

		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getSumInfoByIntegrationSimpleType, pageInfo);

		logger.info("===>  getSumInfoByIntegrationSimpleType //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), "%" + searchName + "%", "%" + searchName + "%",
				"%" + searchName + "%" });

	}	
	
	public List getWarningTotalCountForIntegrationSimpleType(int contractTypeID, String searchName,
				PageInfo pageInfo, int userID){
		
		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getWarningTotalCountForIntegrationSimpleType, pageInfo);

		logger.info("===>  getWarningTotalCountForIntegrationSimpleType //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), "%" + searchName + "%","%" + searchName + "%",
				"%" + searchName + "%" });
	}

	
	public List getContractListByIntegrationNewTypeForCurrency(
			int contractTypeID, String begDate, String endDate,
			String currencyFlag, PageInfo pageInfo, int userID){
		
		String sqlStr = WebUtil.generateSQLStr(
				SQLScript.getContractListByIntegrationNewTypeForCurrency, pageInfo);

		logger.info("===>  getContractListByIntegrationNewTypeForCurrency //"
				+ Type.getString(contractTypeID) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), begDate,
				endDate,
				currencyFlag,
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });
	}
	
	public List getContractListByIntegrationPerformedForCurrency(
			int contractTypeID, String begDate, String endDate,
			String currencyFlag, String subIntegrationType, PageInfo pageInfo, int userID){
		
		String sqlStr = "";
		
		if(WebKeys.Type_subIntegration_Payed.equals(subIntegrationType))
			sqlStr = WebUtil.generateSQLStr(
				SQLScript.getPerformedSubIntegrationForPayed, pageInfo);
		else if(WebKeys.Type_subIntegration_Invoiced.equals(subIntegrationType))
			sqlStr = WebUtil.generateSQLStr(
					SQLScript.getPerformedSubIntegrationForInvoiced, pageInfo);
		else if(WebKeys.Type_subIntegration_Estimate.equals(subIntegrationType))
			sqlStr = WebUtil.generateSQLStr(
					SQLScript.getPerformedSubIntegrationForEstimate, pageInfo);

		logger.info("===>  getContractListByIntegrationPerformedForCurrency //"
				+ Type.getString(subIntegrationType) + sqlStr);

		return jt.queryForList(sqlStr, new Object[] {
				new Integer(contractTypeID), 
				begDate,
				endDate,
				begDate,
				endDate,
				begDate,
				endDate,
				currencyFlag,
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });
	}	
}
