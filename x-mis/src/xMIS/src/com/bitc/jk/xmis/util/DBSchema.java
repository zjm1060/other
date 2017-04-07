package com.bitc.jk.xmis.util;

public class DBSchema {
	public class Contract {

		public static final String TableName = "xmis.con_合同";

		public static final String ContractID = "合同ID";

		public static final String ContractNo = "合同编号";

		public static final String ContractName = "合同名称";

		public static final String Abbreviation = "简称";

		public static final String ParentID = "父合同ID";
		
		public static final String ParentContractNo = "contractNo";
		
		public static final String ParentContractAbb = "abbreviation";

		public static final String SignatureDate = "签订日期";

		public static final String BuyerID = "甲方公司ID";

		public static final String DeptIDOfBuyer = "甲方部门ID";

		public static final String Buyer = "甲方公司";
		public static final String BuyerAbb = "aCompany_abb";

		//public static final String DeptNameOfBuyer = "甲方部门";
		public static final String DeptNameOfBuyer = "aDept_name";
		

		public static final String VendorID = "乙方公司ID";

		public static final String DeptIDOfVendor = "乙方部门ID";

		public static final String VendorName = "乙方公司";
		public static final String VendorAbb = "bCompany_abb";

//		public static final String DeptNameOfVendor = "乙方部门";
		public static final String DeptNameOfVendor = "bDept_name";

		public static final String ContractAmt = "合同总额";
		public static final String ContractAmtCount = "合同数量";

		public static final String CurrencyID = "货币ID";

		public static final String CurrencyName = "货币名称";

		public static final String ContractTypeID = "合同类型ID";

		public static final String ContractTypeName = "合同类型";

		public static final String BuyerRepresentativeID = "甲方代表ID";

		public static final String BuyerRepresentativeSurname = "甲方代表姓氏";

		public static final String BuyerRepresentativeName = "甲方代表名字";
		
		public static final String VendorRepName = "bEmp_name";

		public static final String VendorRepresentativeID = "乙方代表ID";

		public static final String VendorRepresentativeSurname = "乙方代表姓氏";

		public static final String VendorRepresentativeName = "乙方代表名字";

		public static final String ContractSummary = "合同概要";

		public static final String BuyerRepName = "aEmp_name";
		public static final String Remark = "备注";
		
		public static final String RegisterID = "登记人ID";

		public static final String RegisterSurname = "登记人姓氏";

		public static final String RegisterName = "登记人名字";
		
		public static final String RepName = "cEmp_name";

		public static final String UpdateDate = "更新时间";

		public static final String SourceTypeID = "合同来源类型ID";

		public static final String SourceTypeName = "合同来源类型";

		public static final String GovermentContractSourceID = "横向合同来源ID";

		public static final String GovermentContractSourceName = "横向合同来源";
		
		public static final String WarrantyStartDate = "质保期起";
		
		public static final String WarrantyEndDate = "质保期止";
		
		public static final String WarrantyDefinition = "质保期定义";
		
		public static final String WarrantyLimit = "质保期限";
		
		public static final String WarrantyUnit = "质保期计量单位";
		
		public static final String DeliveryID = "发货联系人ID";
		
		public static final String DeliveryName = "delivery_name";

	}
	
	public class GovernmentContractSource{
		public static final String TableName = "xmis.con_横向合同来源";
		public static final String GovermentContractSourceID = "横向合同来源ID";
		public static final String GovermentContractSourceName = "名称";
		public static final String CompanyID = "来源单位ID";
		public static final String DepartmentID = "来源部门ID";
		public static final String FundingLimit = "资助额度上限";
		public static final String CurrencyTypeID = "货币ID";
		public static final String Specialty = "特点";
		public static final String Template = "申请模板";
		public static final String UpdateDate = "变更日期";
		public static final String UpdateID = "变更人";
		public static final String Actived = "是否禁用";
	}
	
	public class ContractType{
		public static final String TableName = "xmis.con_合同类型";
		public static final String ContractTypeID = "合同类型ID";
		public static final String ContractTypeName = "合同类型";
	}
	
	
	public class ContractSourceType{
		public static final String TableName = "xmis.con_合同来源类型";
		public static final String ContractSourceTypeID = "合同来源类型ID";
		public static final String ContractSourceTypeName = "合同来源类型";
	}	
	
	public class CurrencyType {
		public static final String TableName = "xmis.sys_货币";
		public static final String CurrencyID = "货币ID";
		public static final String CurrencyName = "货币名称";
		public static final String ExchangeRate = "对人民币汇率";
		public static final String CurrenyGraph = "货币符号";
		public static final String ExchangedAmt = "兑换金额";
	}

	public class Employee {
		public static final String TableName = "xmis.hr_人员";

		public static final String ID = "人员ID";

		public static final String LastName = "名字";

		public static final String FirstName = "姓氏";
		
		public static final String IDCard = "身份证号";

		public static final String SexID = "性别ID";

		public static final String OfficeAddressID = "办公地址ID";

		public static final String HomeAddress = "家庭地址";

		public static final String Position = "职务";

		public static final String MobileNo = "移动电话";

		public static final String OfficeTelNo = "单位电话";

		public static final String ExtNo = "单位分机";

		public static final String HomeTelNo = "住宅电话";

		public static final String FaxNo = "传真号码";

		public static final String Email = "电子邮件";

		public static final String SpouseName = "配偶姓名";

		public static final String IssueName = "孩子姓名";

		public static final String NativePlace = "家乡";

		public static final String EduicationLevelID = "学历ID";

		public static final String Remark = "附注";

		public static final String WebSite = "主页";

		public static final String PoliticalStatus = "政治面貌ID";

		public static final String Hobby = "爱好";

		public static final String Specialty = "特长";

		public static final String Photo = "照片";

		public static final String MaritalStatus = "婚姻状态ID";

		public static final String UpdateID = "修改人ID";

		public static final String UpdateDate = "修改时间";

		public static final String Password = "密码";

		public static final String DisabledFlag = "禁用";

		public static final String ContralFile = "控制文件";

		public static final String LoginName = "登录名";
		
		public static final String JoinDate = "入职日期";
		public static final String Birthday = "出生日期";
		public static final String ZipCode = "家庭邮编";
		public static final String IssueSex = "子女性别";
		public static final String imgName = "照片文件名";
		
		public static final String aEmpName = "aEmp_name";
		public static final String bEmpName = "bEmp_name";
	}

	public class DeptBelongToDept {
		public static final String TableName = "xmis.org_部门归属部门表";

		public static final String DeptID = "部门ID";

		public static final String ParentsID = "父部门ID";

	}

	public class Department {
		public static final String TableName = "xmis.org_部门";

		public static final String DepartmentID = "部门ID";

		public static final String DepartmentName = "部门名称";

		public static final String Abbreviation = "名称缩写";

		public static final String TelephoneNo1 = "电话1";

		public static final String FaxNo1 = "传真1";

		public static final String TelephoneNo2 = "电话2";

		public static final String FaxNo2 = "传真2";

		public static final String ManagerID = "负责人ID";

		public static final String ContactID = "联系人ID";

		public static final String WebSite = "网址";

		public static final String Email = "邮箱";

		public static final String QtyOfMember = "人数";

		public static final String Address = "地址";
		
		public static final String DisabledFlag = "禁用";

	}

	public class ProjectTask {
		public static final String TableName = "xmis.prj_项目任务表";

		public static final String ProjectTaskID = "项目任务ID";

		public static final String ProjectID = "项目ID";

		public static final String SponsorID = "布置人ID";

		public static final String AssigedToDeptID = "承接部门ID";

		public static final String TaskScheme = "建议方案";

		public static final String TaskDifficulty = "技术难点";

		public static final String RegularTaskID = "常见任务ID";

		public static final String TaskETADate = "建议完成时间";

		public static final String TaskTypeID = "任务类型ID";

	}

	public class Project {

		public static final String TableName = "xmis.prj_项目";

		public static final String ProjectID = "项目ID";

		public static final String ProjectCode = "项目编码";
		
		public static final String ProjectName = "项目名称";

		public static final String ContractID = "项目合同ID";

		public static final String StartDate = "启动日期";

		public static final String ProjectStatusID = "项目状态ID";

		public static final String ProjectTypeID = "项目类型ID";
	}

	public class ProjectStatus{
		public static final String TableName = "xmis.prj_项目状态";

		public static final String ProjectStatusID = "项目状态ID";
		public static final String ProjectStatusName = "项目状态";
		public static final String ProjectStatusDescription = "项目状态解释";
		public static final String SelfDefinitionFlag = "是否自定义";
	}
	
	
	public class PaymentCondition{
		public static final String TableName = "xmis.con_收款条件表";
		public static final String ContractID = "合同ID";
		public static final String PaymentConditionID = "收款条件ID";
		public static final String PaymentConditionDescription = "收款条件描述";
		public static final String ProjectStatusID = "对应项目状态";
		public static final String ProjectStatusName = "项目状态";
		public static final String DelayDays = "延迟天数";
		public static final String PaymentDate = "收款日期";
		public static final String PaymentDateDisplayed = "payment_date";
		public static final String ManualFlag = "手动设置";
		public static final String PaymentPercent = "收款百分比";
		public static final String PaymentAmt = "收款金额";			
		public static final String ManualReason = "手动设置原因";	
		
	}
	
	public class PaymentConditionManualLog{
		public static final String TableName = "xmis.con_收款条件手动变更表";
		public static final String LogID = "变更ID";
		public static final String ContractID = "合同ID";
		public static final String PaymentConditionID = "收款条件ID";
		public static final String PaymentDate = "收款日期";
		public static final String PaymentPercent = "收款百分比";
		public static final String PaymentAmt = "收款金额";			
		public static final String ChangedReason = "变更原因";	
		public static final String ChangedDate = "变更日期";	
		public static final String ChangedID = "变更人";	
		
		
	}	
	
	/*
	 * Merge to 收款明细表
	 */
	public class PaymentDetail {
		public static final String TableName = "xmis.con_付款明细表";

		public static final String PaymentID = "付款ID";

		public static final String PaymentAmt = "付款金额";

		public static final String PaymentDate = "付款日期";

		public static final String TransactorID = "经办人";

		public static final String ScanAttachment = "票据扫描";

		public static final String ContractID = "合同ID";
	}

	public class ChargedDetail {
		public static final String TableName = "xmis.con_收款明细表";

		public static final String ChargedID = "收款ID";

		public static final String ChargedAmt = "收款金额";

		public static final String ChargedDate = "收款日期";
		public static final String ChargedDateDisplayed = "chargedDate";

		public static final String TransactorID = "经办人";

		public static final String ScanAttachment = "票据扫描";

		public static final String ContractID = "合同ID";
		
		public static final String UpdateID = "登记人";
		
		public static final String UpdateDate = "登记日期";
		public static final String UpdateDateDisplay = "updateDate";
		
		public static final String UnChargedAmtTotal = "未收款金额合计";

		public static final String TotalAmtByID = "收款合计";
		
		public static final String ChargedAmtTotalSum = "收款总计";
		
		public static final String UnChargedAmtTotalSum = "未收款总计";
		
		public static final String ChargedRemark = "备注";
		

	}

	public class InvoiceDetail{
		public static final String TableName = "xmis.con_发票开具表";
		public static final String ContractID = "合同ID";
		public static final String InvoiceNo = "发票号";
		public static final String InvoiceAmt = "发票金额";
		public static final String InvoiceDate = "开具日期";
		public static final String InvoiceDateDisplayed = "invocieDate";
		public static final String TransactorID = "经办人";
		public static final String ScanAttachment = "扫描件";
		public static final String UpdateID = "登记人";
		public static final String UpdateDate = "登记日期";
		public static final String UpdateDateDisplayed = "updateDate";
		public static final String Remark = "备注";
		public static final String InvoiceCode = "发票编号";
		public static final String TotalAmtByID = "已开发票金额合计";
		public static final String UnAccountAmtTotalSum = "已开票未到账金额合计";
		
	}
	
	public class EmployeeBelongToDept {
		public static final String TableName = "xmis.hr_人员部门关系表";

		public static final String EmployeeID = "人员ID";

		public static final String DepartmentID = "部门ID";

		public static final String Position = "职务";

		public static final String IsPartTime = "是否兼职";
	}

	public class MemberOfProjectTask {
		public static final String TableName = "xmis.prj_项目任务成员";

		public static final String ProjectTaskID = "项目任务ID";

		public static final String MemberID = "项目成员ID";

		public static final String TaskDescriptioin = "承担任务";

		public static final String IsLeader = "是否负责人";
	}

	public class PlanOfProjectTask {
		public static final String TableName = "xmis.prj_任务进度计划";

		public static final String ProjectTaskID = "项目任务ID";

		public static final String TaskName = "任务名称";

		public static final String BengineDate = "开始日期";

		public static final String EndDate = "结束日期";

		public static final String PreviousTaskID = "前置任务ID";

		public static final String LeaderID = "负责人ID";

	}

	public class Address{
		public static final String TabelName = "xmis.add_地址";
		public static final String AddressID = "地址ID";
		public static final String CityID = "城市ID";
		public static final String District = "区县";
		public static final String ZipCode = "邮编";
		public static final String AddressInfo = "详细地址";
		
	}
	
	public class City{
		public static final String TableName = "xmis.add_城市";
		public static final String CityID = "城市ID";
		public static final String CityCode = "城市编码";
		public static final String CityName = "城市名称";
		public static final String Abbreviation = "城市简称";
		public static final String AreaCode = "区号";
		public static final String ProvinceID = "省ID";
		
		
	}
	
	public class Province{
		public static final String TableName = "xmis.add_省";
		public static final String ProvinceID = "省ID";
		public static final String ProvinceCode = "省编码";
		public static final String ProvinceName = "省名称";
		public static final String Abbreviation = "省简称";
		public static final String AreaID = "地区ID";
	}

	public class DepartmentBelongToCompany{
		public static final String TabelName = "xmis.org_部门归属公司表";
		public static final String CompanyID = "公司ID";
		public static final String DepartmentID = "部门ID"; 
		public static final String DirectlyFlag = "是否直属";
	}
	
	public class Company{
		public static final String TableName = "xmis.org_公司";
		public static final String CompanyID = "公司ID";
		public static final String CompanyName = "公司名称";
		public static final String Abbreviation = "名称缩写";
		public static final String BusinessRelationID = "业务关系ID";
		public static final String AddressID = "地址ID";
		public static final String TelephoneNo1 = "电话1" ;
		public static final String FaxNo1 = "传真1";
		public static final String TelephoneNo2 = "电话2";
		public static final String FaxNo2 = "传真2";
		public static final String WebSite = "网址";
		public static final String EmailAddress = "邮箱";
		public static final String BankName = "开户银行";
		public static final String AccountNo = "账号";
		public static final String TaxNo = "税号";
		public static final String CompanyScaleID = "公司规模ID";
		public static final String LegalRepresentativeID = "法人代表ID";
		public static final String ContactPersonID = "联系人ID" ;
		public static final String CustomerLevelID = "客户级别ID";
		public static final String CreditLevelID = "信用等级ID";
		public static final String OrganizationCode = "机构代码";
		public static final String RegisteredCapital = "注册资金" ;
		public static final String Remark = "备注";
		public static final String CustomerSourceID = "客户来源ID" ;
		public static final String BusinessScope = "经营范围";
		public static final String UpdateID = "修改人ID";
		public static final String UpdateDate = "修改时间";
		public static final String SecurityLevelID = "保密级别";
	}
	
	public class CustomerLevel{
		public static final String TableName = "xmis.org_客户级别";
		public static final String CustomerLevelID = "客户级别ID";
		public static final String CustomerLevelName = "名称";
		public static final String CustomerLevelDefinition = "描述";
	}
	
	public class CreditLevel{
		public static final String TableName = "xmis.org_信用等级";
		public static final String CreditLevelID = "信用等级ID";
		public static final String CreditLevelName = "名称";
		public static final String CreditLevelDefinition = "描述";
	}
	
	public class BusinessRelation{
		public static final String TableName = "xmis.org_业务关系";
		public static final String BusinessRelationID = "业务关系ID";
		public static final String BusinessRelationName = "关系名称";
		public static final String BusinessRelationDefinition = "关系描述";
	}

	public class CompanyScale{
		public static final String TableName = "xmis.org_公司规模";
		public static final String CompanyScaleID = "公司规模ID";
		public static final String CompanyScaleName = "名称";
		public static final String CompanyScaleDefinition = "描述";
	}
	
	public class ContactPerson{
		public static final String TableName = "xmis.crm_联系人";
		public static final String ContactPersonID = "联系人ID";
		public static final String FirstName = "姓氏";
		public static final String LastName = "名字";
		public static final String CompanyID = "公司ID";
		public static final String DepartmentID = "部门ID";
		public static final String UpdateID = "修改人ID";
		public static final String UpdateDate = "修改时间";
	}
	
	public class EducationLevel{
		public static final String TableName = "xmis.sys_学历";
		public static final String EducationLevelID = "学历ID";
		public static final String EducationLevelName = "学历";
		public static final String SchoolName = "毕业院校";
		public static final String GraduationDate = "毕业时间";
		public static final String Major = "专业";
		public static final String Remark = "附注";
	}
	
	public class PoliticalStatus{
		public static final String TableName = "xmis.sys_政治面貌";
		public static final String PoliticalStatusID= "政治面貌ID";
		public static final String PoliticalStatusName = "政治面貌";
	}
	
	public class EmployeeLunchAddress{
		public static final String TableName = "xmis.hr_午餐地址登记表";
		public static final String UpdateDate = "变更日期";
		public static final String EmployeeID = "人员ID";
		public static final String LunchAddressID = "餐馆ID";
		public static final String UpdateID = "变更人ID";
		public static final String BookedFlag = "确认标志";
	}
	
	public class LunchAddress{
		public static final String TableName = "xmis.sys_午餐地址表";
		public static final String LunchAddressID = "餐馆ID";
		public static final String LunchAddressName = "餐馆名称";
		public static final String LunchAddress = "餐馆地址";
	}
	
	public class Attachment{
		public static final String TableName = "xmis.sys_附件";
		public static final String AttachmentID = "附件ID";
		public static final String UploadDate = "上传时间";
		public static final String UploaderID = "上传人";
		public static final String AttahcmentDesc ="内容描述";
		public static final String AttachmentName = "文件名";
		public static final String AttachmentContent = "附件内容";
		public static final String AttachmentTypeID = "附件主体类型ID";
		public static final String AttachmentKey = "附件相关字段";
		public static final String REFID = "REFID";
		public static final String UploaderName = "uploader_name";
		public static final String UploadDisplayDate = "upload_date";
	}
	
	public class AttachmentType{
		public static final String TableName = "xmis.sys_附件主体类型";
		public static final String AttachmentTypeID = "附件主体类型ID";
		public static final String AttachmentTypeDesc = "附件主体描述";
		public static final String AttachmentTableName = "TableName";
		public static final String AttachmentFieldName = "FieldName";
	}
}
