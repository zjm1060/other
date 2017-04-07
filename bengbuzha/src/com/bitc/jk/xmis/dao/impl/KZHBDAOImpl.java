package com.bitc.jk.xmis.dao.impl;

import java.util.List;

import org.springframework.jdbc.core.JdbcTemplate;

import com.bitc.jk.xmis.dao.KZHBDAO;
import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class KZHBDAOImpl extends BaseDAOImpl implements KZHBDAO {
	
	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate ajdbcTemplate) {
		this.jt = ajdbcTemplate;
	}
	public void doDeleteAttachment(AttachmentFile attFile) {
		// TODO Auto-generated method stub
		super.upldateObject(attFile);
		
	}

	public void saveAttachedFile(AttachmentFile attFile) {
		// TODO Auto-generated method stub
		super.saveObject(attFile);
	}

	public int getAttchedFilesListCount(List<QueryParam> params) {
		// TODO Auto-generated method stub
		return super.getTotalCount(params, AttachmentFile.class);
	}

	public void replaceAttachedFile(AttachmentFile attFile) {
		// TODO Auto-generated method stub
		super.upldateObject(attFile);
	}

	public List getAttchedFilesListStore(List<QueryParam> params,
			PageInfo pageInfo) {
		// TODO Auto-generated method stub
		
		return super.getListStore(params, pageInfo, AttachmentFile.class);
		
	}

	public List findAttFileByFileName(AttachmentFile attFile) {
		// TODO Auto-generated method stub
		return super.findObjsByParam(attFile);
	}
	
	public AttachmentFile getAttachmentObjByID(int attID){
		
		return (AttachmentFile)super.ht.load(AttachmentFile.class, new Integer(attID));
	}

	public List getQualifiedSuppliers(StandardQuery query, PageInfo pageInfo) throws Exception {
		String sql = "select " +
				"org2.公司ID AS companyID ,org2.公司名称 as companyName,org.资质情况 as qualification ," +
				"org.为中水科配套产品 as supporting,org.是否为一般纳税人 as taxpayer,org2.网址 as website," +
				"Replace((hr.姓氏 +hr.名字),' ','') as contactName,org.经理ID as contactID," +
				"(add5.区号+'-'+addc.区号+'-'+hr.单位电话) as landline," +
				" org2.注册资金  AS registeredCapital," +
				"Replace((hr2.姓氏 +hr2.名字),' ','') as lastModifiedByName,org.最后修改人ID as lastModifiedByID," +
				"org.最后修改日期 as lastModifiedDate," +
				"CASE WHEN org.是否为一般纳税人 = '0' THEN  '是' " +
				"WHEN org.是否为一般纳税人 = '1' THEN  '否' " +
				"ELSE '' END AS dddddddd " +
				" into  #result " +
				"from " +
				"xmis.org_供方评定调查表 org " +
				"LEFT JOIN xmis.org_公司 org2 " +
				"ON org.公司ID = org2.公司ID " +
				"LEFT JOIN xmis.hr_人员 hr " +
				"ON org.经理ID = hr.人员ID " +
				"LEFT JOIN xmis.add_地址 addd " +
				"ON hr.办公地址ID = addd.地址ID " +
				"LEFT JOIN xmis.add_城市 addc " +
				"ON addd.城市ID = addc.城市ID " +
				"LEFT JOIN xmis.add_省 add3 " +
				"ON addc.省ID = add3.省ID " +
				"LEFT JOIN xmis.add_地区 add4 " +
				" ON add3.地区ID = add4.地区ID " +
				" LEFT JOIN xmis.add_国家及地区 add5 " +
				" ON add4.国家ID = add5.国家ID " +
				" LEFT JOIN xmis.hr_人员 hr2 " +
				" ON org.最后修改人ID = hr2.人员ID ";
		
		SQLGenerator generator = new SQLGenerator(sql);
		if(query !=null){
			query.addToSQLGenerator(generator);
		}
		String sqlty = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0))?new Object[] {} : new Object[] {
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlty,args);
	}

	public List getQualificationList(StandardQuery query, PageInfo pageInfo) throws Exception{
		// TODO Auto-generated method stub
		String sql = " SELECT org1.ID AS id,org1.公司ID AS companyID,org2.公司名称 as companyName ,org1.资质名称 AS qualifiationName,org1.证书号 AS certificateNo ," +
				" org1.证书有效期起 AS certificateValidQi,org1.证书有效期止 AS certificateValidZhi ," +
				"  org1.备注 AS qualificationRemark ," +
				" COUNT(org.资质名称对应ID) AS frequencyOfUse , " +
				"CASE WHEN org1.是否年审  = '0' THEN  '是' " +
				"WHEN org1.是否年审  = '1' THEN  '否' " +
				"ELSE '' END AS examined "+
				"into #result " +
				" FROM xmis.org_资质管理表 org1 " +
				" left join xmis.org_资质使用部门表 org " +
				" on org1.ID = org.资质名称对应ID " +
				" left join xmis.org_公司 org2 " +
				"  on org1.公司ID = org2.公司ID " +
				" WHERE  org1.公司ID =  1 " +
				" GROUP BY org1.ID,org1.公司ID,org2.公司名称,org1.资质名称,org1.证书号,org1.证书有效期起,org1.证书有效期止,org1.是否年审, org1.备注 ";
		SQLGenerator generator = new SQLGenerator(sql);
		if(query !=null){
			query.addToSQLGenerator(generator);
		}
		String sqlty = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0))?new Object[] {} : new Object[] {
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlty,args);
	}

	public List getPatentInfoList(StandardQuery query, PageInfo pageInfo) {
		// TODO Auto-generated method stub
		String sql = " select org.ID as sjid,org.公司ID AS companyID ,org1.公司名称 as companyName,org.专利名称 as patentName,org.类别 as patentsCategory," +
		" org.设计人1ID AS patentedDesignPeopleID1," +
		"org.设计人2ID AS patentedDesignPeopleID2," +
		" org.设计人3ID as patentedDesignPeopleID3," +
		"org.专利权人1ID as patentRightPeopleID1," +
		" org.专利权人2ID as patentRightPeopleID2," +
		"org.专利权人3ID as patentRightPeopleID3," +
		" org.专利权人4ID as patentRightPeopleID4," +
		"org.专利权人5ID as patentRightPeopleID5," +
		"REPLACE((hr1.姓氏+hr1.名字),' ','')+','" +
		"+isnull(REPLACE((hr2.姓氏+hr2.名字),' ',''),'')+','" +
		"+isnull(REPLACE((hr3.姓氏+hr3.名字),' ',''),'') AS patentedDesignPeople," +
		" REPLACE((hr4.姓氏+hr4.名字),' ','')+','" +
		"+isnull(REPLACE((hr5.姓氏+hr5.名字),' ',''),'')+','" +
		"+isnull(REPLACE((hr6.姓氏+hr6.名字),' ',''),'')+','" +
		"+isnull(REPLACE((hr7.姓氏+hr7.名字),' ',''),'')+','" +
		"+isnull(REPLACE((hr8.姓氏+hr8.名字),' ',''),'')  as patentRightPeople," +
		" org.专利号 as patentNumber," +
		" org.证书号 as patentCertificateNo," +
		" CONVERT(VARCHAR,org.申请日 , 120) as applicationTime," +
		" CONVERT(VARCHAR,org.授权公告日 ,120) as authorizeTime," +
		" REPLACE((hr9.姓氏+hr9.名字),' ','') as patentregisterName," +
		" REPLACE((hr10.姓氏+hr10.名字),' ','') as patentlastModifierName," +
		" CONVERT(VARCHAR,org.登记日期 , 120) as patentupdateDate," +
		" CONVERT(VARCHAR,org.最后修改日期 ,120) as patentlastModified," +
		" CONVERT(VARCHAR,org.有效期起, 120) as validFrom," +
		" CONVERT(VARCHAR,org.有效期止, 120) as validOnly " +
		" into #result " +
		" from " +
		" xmis.org_公司专利表 org " +
		" left join xmis.hr_人员 hr1 " +
		" on org.设计人1ID = hr1.人员ID " +
		" left join  xmis.hr_人员 hr2 " +
		" on org.设计人2ID = hr2.人员ID " +
		" left join xmis.hr_人员 hr3 " +
		" on org.设计人3ID = hr3.人员ID " +
		" left join xmis.hr_人员 hr4 " +
		" on org.专利权人1ID = hr4.人员ID " +
		" left join xmis.hr_人员 hr5 " +
		" on org.专利权人2ID = hr5.人员ID " +
		" left join xmis.hr_人员 hr6 " +
		" on org.专利权人3ID = hr6.人员ID " +
		" left join xmis.hr_人员 hr7 " +
		" on org.专利权人4ID = hr7.人员ID " +
		" left join xmis.hr_人员 hr8 " +
		" on org.专利权人5ID = hr8.人员ID " +
		" left join xmis.hr_人员 hr9 " +
		" on org.登记人ID = hr9.人员ID " +
		" left join xmis.hr_人员 hr10 " +
		" on org.最后修改人ID = hr10.人员ID " +
		" left join xmis.org_公司 org1 " +
		" on org.公司ID = org1.公司ID " +
		" where org.公司ID = 1  ";
		SQLGenerator generator = new SQLGenerator(sql);
		if(query !=null){
			query.addToSQLGenerator(generator);
		}
		String sqlty = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0))?new Object[] {} : new Object[] {
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlty,args);
	}

}
