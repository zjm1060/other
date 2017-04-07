package com.bitc.jk.xmis.util.sql;

import java.util.UUID;

import com.bitc.jk.xmis.util.UUIDUtils;

public class SQLScript_BBZFinancy {

	public static String getAttachmentList = "select 附件ID,文件名,上传人,ltrim(RTRIM(姓氏))+ltrim(RTRIM(名字)) as name, "
			+ "convert(varchar(20),上传时间 ,120) 上传时间  from "
			+ "xmis.sys_附件 left join xmis.hr_人员 on xmis.sys_附件.上传人 = xmis.hr_人员.人员ID where 附件主体类型ID = ? order by 上传时间  desc";
	
	public static String getYearAttachmentList = "select 附件ID,文件名,上传人,ltrim(RTRIM(姓氏))+ltrim(RTRIM(名字)) as name, "
			+ "convert(varchar(20),上传时间 ,120) 上传时间  from "
			+ "xmis.sys_附件 left join xmis.hr_人员 on xmis.sys_附件.上传人 = xmis.hr_人员.人员ID where 附件主体类型ID = ? order by 上传时间  desc";
	
	public static String doSaveSalryExcelInfo = "insert into xmis.T_Excel_Sheet(sheet_uuid,file_id,sheet_index,sheet_name,isImport) values(?,?,?,?,?)";

	public static String delAttachmentInfo = "delete from  xmis.sys_附件 where 附件ID =?";

	public static String delExcelSheetInfo = "delete from  xmis.T_Excel_Sheet where file_id = ?";
	
	public static String delYearExcelSheetDate = "delete from  xmis.T_Income_Info where sheet_uuid in (select sheet_uuid from xmis.T_Excel_Sheet where file_id = ?)";
	
	public static String delExcelSheetDate = "delete from  xmis.T_Salary_Info where sheet_uuid in (select sheet_uuid from xmis.T_Excel_Sheet where file_id = ?)";
	
	public static String delExcelSheetDateByYear = "delete from  xmis.T_Income_Info where iyear = ? and type = ?";
	
	public static String delExcelSheetDataBySheetUuid = "delete from  xmis.T_Income_Info where sheet_uuid = ? and type = ?";
	
	public static String findByYearOrSheetUuidAndType = "select *  from  xmis.T_Income_Info where (iyear = ? or sheet_uuid = ?) and type = ?";
	
	public static String getSalarySheetList = "select sheet_uuid,file_id,sheet_index,sheet_name,isImport from xmis.T_Excel_Sheet where file_id = ?";

	public static String updateIsImportForSheet = "update xmis.T_Excel_Sheet set  isImport = 1 where sheet_uuid= ?";
	
	public static String updateIsNotImportForSheet = "update xmis.T_Excel_Sheet set  isImport = 0 where sheet_uuid in ( select sheet_uuid from xmis.T_Income_Info where iyear = ? and type = ?) ";

	public static String updateIsNotImportForSheetBySheet_Uuid ="update xmis.T_Excel_Sheet set  isImport = 0 where sheet_uuid = ?";
	
	public static String getSalaryType = "select type_id,type_name from xmis.t_salary_type_info ";

	public static String getSheetsForSalaryType = "SELECT  xmis.t_salary_info.sheet_uuid , \n"
			+ " name_column \n"
			+ " FROM    xmis.T_Excel_Sheet \n"
			+ " LEFT JOIN xmis.t_salary_info ON xmis.T_Excel_Sheet.sheet_uuid = xmis.T_Salary_Info.sheet_uuid \n"
			+ " WHERE   iyear = ? \n"
			+ " AND imonth = ? \n"
			+ " AND type_id = ? \n"
			+ " GROUP BY xmis.t_salary_info.sheet_uuid ,\n" + " name_column  ";
	
	public static String getSheetsIncome = "SELECT  xmis.T_Income_Info.sheet_uuid , \n"
			+ " FROM    xmis.T_Excel_Sheet \n"
			+ " LEFT JOIN xmis.T_Income_Info ON xmis.T_Excel_Sheet.sheet_uuid = xmis.T_Income_Info.sheet_uuid \n"
			+ " WHERE   iyear = ? \n"
			+ " GROUP BY xmis.T_Income_Info.sheet_uuid \n";


	public static String getSheetColumn = "select  * from  xmis.t_salary_info where ?*?  = '姓名' and sheet_uuid = ?";

	public static String getSheetsForPersonOneSalaryType = "select  iyear,imonth,type_id,sheet_uuid,name_column,salary_column,titleColumn, \n"
			+ " column_1,column_2,column_3,column_4,column_5,column_6,column_7,column_8,column_9,column_10, \n"
			+ " column_11,column_12,column_13,column_14,column_15,column_16,column_17,column_18,column_19,column_20, \n"
			+ " column_21,column_22,column_23,column_24,column_25,column_26,column_27,column_28,column_29,column_30, \n"
			+ " column_31,column_32,column_33,column_34,column_35,column_36,column_37,column_38,column_39,column_40,\n"
			+ " column_41,column_42,column_43,column_44,column_45,column_46,column_47,column_48,column_49,column_50,\n"
			+ " column_51,column_52,column_53,column_54,column_55,column_56,column_57,column_58,column_59,column_60 \n"
			+ " from  xmis.t_salary_info where titleColumn = 1  and sheet_uuid = ? and iyear= ? and imonth = ?  and type_id = ? \n"
			+ " union all  \n"
			+ " select  iyear,imonth,type_id,sheet_uuid,name_column,salary_column,titleColumn, \n"
			+ " column_1,column_2,column_3,column_4,column_5,column_6,column_7,column_8,column_9,column_10, \n"
			+ " column_11,column_12,column_13,column_14,column_15,column_16,column_17,column_18,column_19,column_20, \n"
			+ " column_21,column_22,column_23,column_24,column_25,column_26,column_27,column_28,column_29,column_30, \n"
			+ " column_31,column_32,column_33,column_34,column_35,column_36,column_37,column_38,column_39,column_40, \n"
			+ " column_41,column_42,column_43,column_44,column_45,column_46,column_47,column_48,column_49,column_50, \n"
			+ " column_51,column_52,column_53,column_54,column_55,column_56,column_57,column_58,column_59,column_60 \n"
			+ " from  xmis.t_salary_info where *?*  = ? and sheet_uuid = ? and iyear= ? and imonth = ?  and type_id = ?\n";
	public static String getDateIncome="select  iyear,sheet_uuid, \n"
			+ " column_1,column_2,column_3,column_4,column_5,column_6,column_7,column_8,column_9,column_10, \n"
			+ " column_11,column_12,column_13,column_14,column_15,column_16,column_17,column_18,column_19,column_20, \n"
			+ " column_21,column_22,column_23,column_24,column_25,column_26,column_27,column_28,column_29,column_30 \n"
			+ " from  xmis.T_Income_Info where titleColumn = 1  and iyear = ? and type= ?\n"
			+ " union all  \n"
			+ " select  iyear,sheet_uuid, \n"
			+ " column_1,column_2,column_3,column_4,column_5,column_6,column_7,column_8,column_9,column_10, \n"
			+ " column_11,column_12,column_13,column_14,column_15,column_16,column_17,column_18,column_19,column_20, \n"
			+ " column_21,column_22,column_23,column_24,column_25,column_26,column_27,column_28,column_29,column_30 \n"
			+ " from  xmis.T_Income_Info where titleColumn = 0 and iyear = ? and type = ?\n";
			
	public static String getIsNotHavePersonForSheet = "select  * from  xmis.t_salary_info where *?*  = ? and sheet_uuid = ?";

    
	public static void main(String[] args) {
		System.out.println(getSheetsForPersonOneSalaryType);
	}

}
