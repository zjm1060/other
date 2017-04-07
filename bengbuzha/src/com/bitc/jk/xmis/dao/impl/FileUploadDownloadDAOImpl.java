package com.bitc.jk.xmis.dao.impl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
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

import com.bitc.jk.xmis.dao.FileUploadDownloadDAO;
import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class FileUploadDownloadDAOImpl implements FileUploadDownloadDAO {

	private static Logger logger = Logger
			.getLogger(FileUploadDownloadDAOImpl.class);

	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate ajdbcTemplate) {
		this.jt = ajdbcTemplate;
	}

	public FileUploadDownloadDAOImpl() {

	}

	/**
	 * 获取附件列表
	 * 
	 * @param REFID
	 * @param attachmentTypeID
	 * @return
	 */
	public List getAttachmentList(String REFID, int attachmentTypeID) {
		String sqlStr = SQLScript.getAttachmentList;
		logger.info("getAttachmentList--->REFID=" + REFID
				+ "  attachmentTypeID=" + attachmentTypeID + "   sql:" + sqlStr);
		return jt.queryForList(sqlStr, new Object[] { new String(REFID),
				new Integer(attachmentTypeID) });
	}

	/**
	 * 保存附件
	 * 
	 * @param attFile
	 * @return
	 */
	public boolean doSaveAttachment(final AttachmentFile attFile,final boolean isDocument) {

		logger.info("===>  doSaveAttachment //" + attFile.getFileName() + "//"
				+ attFile.getFileSize() + SQLScript.createNewAttched);
		final int maxID;
		 if(isDocument){
	        maxID = -1*(jt.queryForInt(SQLScript
	    				.getMaxIDFromTab(DBSchema.Attachment.AttachmentID,
	    						DBSchema.Attachment.TableName)) + 1);
	        }else{
	         maxID = jt.queryForInt(SQLScript
	    				.getMaxIDFromTab(DBSchema.Attachment.AttachmentID,
	    						DBSchema.Attachment.TableName)) + 1;
	        }
		final LobHandler lobHandler = new DefaultLobHandler();

		jt.execute(SQLScript.createNewAttched,
				new AbstractLobCreatingPreparedStatementCallback(lobHandler) {
					protected void setValues(PreparedStatement ps,
							LobCreator lobCreator) throws SQLException {

						ps.setInt(1, maxID);// 附件ID
						ps.setInt(2, attFile.getUploadID());// 上传人ID
						ps.setString(3, attFile.getDesc());// 描述
						ps.setString(4, attFile.getFileName());// 文件名

						lobCreator.setBlobAsBinaryStream(ps, 5,
								attFile.getFis(), attFile.getFileSize());// 附件内容

						ps.setInt(6, attFile.getAttachmentTypeID());// 附件主体类型
						ps.setString(7, attFile.getRefID());// REFID
						ps.setInt(8, attFile.getUploadID());// 最后修改人
						ps.setString(9, attFile.getKeyWords());// 关键字
						ps.setString(10, attFile.getFilePath());// 存放路径
						ps.setInt(11, attFile.getDeletedFlag());// 删除标志
					}
				});
		return true;
	}

	public int doSaveAttachmentGetAttacheId(final AttachmentFile attFile) {

		logger.info("===>  doSaveAttachment //" + attFile.getFileName() + "//"
				+ attFile.getFileSize() + SQLScript.createNewAttched);
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
						ps.setString(3, attFile.getDesc());// 描述
						ps.setString(4, attFile.getFileName());// 文件名

						lobCreator.setBlobAsBinaryStream(ps, 5,
								attFile.getFis(), attFile.getFileSize());// 附件内容

						ps.setInt(6, attFile.getAttachmentTypeID());// 附件主体类型
						ps.setString(7, attFile.getRefID());// REFID
						ps.setInt(8, attFile.getUploadID());// 最后修改人
						ps.setString(9, attFile.getKeyWords());// 关键字
						ps.setString(10, attFile.getFilePath());// 存放路径
						ps.setInt(11, attFile.getDeletedFlag());// 删除标志
					}
				});
		return maxID;
	}

	/**
	 * 删除附件
	 * 
	 * @param attchmentID
	 * @return
	 */
	public void doDeleteAttachementById(final int AttachementID) {
		String sqlStr = SQLScript.DeleteAttachmentByID;
		logger.info("deleteAttachementById--->" + sqlStr);
		jt.update(sqlStr, new PreparedStatementSetter() {
			public void setValues(PreparedStatement arg0) throws SQLException {
				arg0.setInt(1, AttachementID);
			}
		});
	}

	/**
	 * 提示上传文件已存在时(覆盖/取消)
	 * 
	 * @param attchmentTypeID
	 * @param fileName
	 * @return
	 */
	public int existValidate(int attachmentTypeID, String REFID,
			String fileName, int type) {
		String sqlStr = SQLScript.existValidate;
		if (type == -1) {
			sqlStr = SQLScript.existValidateForKzhb;
		}
		logger.info("existValidate--->" + sqlStr);
		return jt.queryForInt(
				sqlStr,
				new Object[] { Type.GetInt(attachmentTypeID),
						Type.getString(REFID), Type.getString(fileName) });
	}

	public int existValidateReturnAttID(int attachmentTypeID, String REFID,
			String fileName, int type) {
		String sqlStr = SQLScript.existValidateReturnAttID;
		if (type == -1) {
			sqlStr = SQLScript.existValidateForKzhb;
		}
		logger.info("existValidate--->" + sqlStr);
		List list = jt.queryForList(
				sqlStr,
				new Object[] { Type.GetInt(attachmentTypeID),
						Type.getString(REFID), Type.getString(fileName) });
		if (list != null && list.size() > 0) {
			return Type.GetInt(((Map) list.get(0)).get("附件ID"));
		} else {
			return 0;
		}
	}

	/**
	 * 上传文件已存在时删除原有的
	 * 
	 * @param attchmentTypeID
	 * @param REFID
	 * @param fileName
	 * @return
	 */
	public void doDeleteByConIDFileName(final int attchmentTypeID,
			final String REFID, final String fileName) {
		String sqlDelete = SQLScript.deleteByConIDFileName;
		logger.info(sqlDelete);
		jt.update(sqlDelete, new PreparedStatementSetter() {
			public void setValues(PreparedStatement arg0) throws SQLException {
				arg0.setInt(1, Type.GetInt(attchmentTypeID));
				arg0.setString(2, Type.getString(REFID));
				arg0.setString(3, Type.getString(fileName));
			}
		});
	}

	/**
	 * 获得附件
	 * 
	 * @param attchmentID
	 * @param directPath
	 * @return filePath fileName
	 */
	public String getAttachmentByID(final int attachmentID,
			final String directPath) {
		logger.info("===> getAttachmentContentByID ? attachmentID="
				+ attachmentID);

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
		logger.info(att.getFilePath() + ":" + att.getFileName());
		return att.getFilePath() + att.getFileName();
	}

	@Override
	public AttachmentFile getAttachmentByID(int attachmentID) {
		List list = jt
				.query("SELECT [附件ID] ,[上传时间] ,[上传人] ,[内容描述] ,[文件名] ,[附件内容] ,[附件主体类型ID] ,[附件相关字段] ,"
						+ "[REFID] ,[最后修改日期] ,[最后修改人ID] ,[关键字] ,[存放路径] ,[删除标志] FROM [sys_附件] WHERE [附件ID]=?",
						new Object[] { attachmentID }, new RowMapper() {

							@Override
							public Object mapRow(ResultSet rs, int index)
									throws SQLException {
								if (index != 0) {
									return null;
								}
								AttachmentFile attFile = new AttachmentFile();
								attFile.setId(rs.getInt(1));
								attFile.setUploadDate(rs.getDate(2));
								attFile.setUploadID(rs.getInt(3));
								attFile.setDesc(rs.getString(4));
								attFile.setFileName(rs.getString(5));
								attFile.setAttachmentTypeID(rs.getInt(7));
								attFile.setRefID(rs.getString(9));
								attFile.setFilePath(rs.getString(13));
								return attFile;
							}
						});
		return (AttachmentFile) list.get(0);
	}

	/**
	 * 获取附件列表 科质部
	 * 
	 */
	public List getAttachmentListForKZHB(List<StandardQuery> querys,
			PageInfo pageInfo) {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getAttachmentListForKZHB);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
		logger.info("getAttachmentListForKZHB    SQL :" + sqlStr);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);
	}

	/**
	 * 删除科质部 文件 逻辑删除 将删除标志 置2
	 * */
	public void deleteKzhbFileById(final int attachementID) {
		String sqlStr = SQLScript.DeleteKzhbFileById;
		jt.update(sqlStr, new PreparedStatementSetter() {
			public void setValues(PreparedStatement arg0) throws SQLException {
				arg0.setInt(1, attachementID);
			}
		});
	}

	public void doUpdateAttachmentContentByID(final AttachmentFile attFile) throws FileNotFoundException {
		final InputStream inputStream = new FileInputStream(new File(attFile
					.getFilePath() + attFile.getFileName()));
		final LobHandler lobHandler = new DefaultLobHandler();

		jt.execute(SQLScript.updateAttched,
				new AbstractLobCreatingPreparedStatementCallback(lobHandler) {
					protected void setValues(PreparedStatement ps,
							LobCreator lobCreator) throws SQLException {
							try {
								lobCreator.setBlobAsBinaryStream(ps, 1,
										inputStream, inputStream.available());
							} catch (IOException e) {
								e.printStackTrace();
							}
						 // 附件内容
						ps.setInt(2, attFile.getId());//  附件ID
					}
				});

	}

}
