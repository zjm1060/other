/**
 * 
 */
package com.bitc.jk.xmis.util.sql;

import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.DBSchema.Employee;

/**
 * @author Ted Li
 * 
 *         2011-5-13
 */
public class UserSecuritySQL {

	public String getSecurityByModuleID = DBSchema.select
			+ DBSchema.BizModule.TableName + ".*,"
			+ DBSchema.BizSubModule.TableName + ".*,"
			+ DBSchema.BizFunction.TableName + ".*"

			+ DBSchema.from + DBSchema.BizModule.TableName + ","
			+ DBSchema.BizSubModule.TableName + ","
			+ DBSchema.BizFunction.TableName + ","
			+ DBSchema.BizModuleMapping.TableName + ","
			+ DBSchema.BizModuleFuncMapping.TableName

			+ DBSchema.where + DBSchema.BizModule.TableName + "."
			+ DBSchema.BizModule.BizModuleID + "=?" + DBSchema.and
			+ DBSchema.BizModule.TableName + "."
			+ DBSchema.BizModule.BizModuleID + "="
			+ DBSchema.BizModuleMapping.TableName + "."
			+ DBSchema.BizModuleMapping.BizModuleID + DBSchema.and
			+ DBSchema.BizSubModule.TableName + "."
			+ DBSchema.BizSubModule.BizSubModuleID + "="
			+ DBSchema.BizModuleMapping.TableName + "."
			+ DBSchema.BizModuleMapping.BizSubModuleID + DBSchema.and
			+ DBSchema.BizSubModule.TableName + "."
			+ DBSchema.BizSubModule.BizSubModuleID + "="
			+ DBSchema.BizModuleFuncMapping.TableName + "."
			+ DBSchema.BizModuleFuncMapping.BizSubModuleID + DBSchema.and
			+ DBSchema.BizFunction.TableName + "."
			+ DBSchema.BizFunction.BizFunctionID + "="
			+ DBSchema.BizModuleFuncMapping.TableName + "."
			+ DBSchema.BizModuleFuncMapping.BizFunctionID
	// + DBSchema.orderBy
	// + DBSchema.BizModule.TableName + "." +
	// DBSchema.BizModule.BizModulesSquenceNo
	// + "," + DBSchema.BizModuleMapping.TableName + "." +
	// DBSchema.BizModuleMapping.BizSubModulesSquenceNo
	;

	public static String getUserRolesOnFunctionLevel =
	// DBSchema.select
	// + DBSchema.UserRoleRightsMapping.TableName + ".*,"
	// + DBSchema.Role.TableName + "." + DBSchema.Role.RoleName
	// + DBSchema.as + DBSchema.UserRoleRightsMapping.RoleName
	// + ", '' as " + DBSchema.UserRoleRightsMapping.UserName
	// + DBSchema.into + "#user_role" + DBSchema.from
	// + DBSchema.UserRoleRightsMapping.TableName + ","
	// + DBSchema.UserRole.TableName + "," + DBSchema.Role.TableName
	// + DBSchema.where + DBSchema.UserRole.TableName + "."
	// + DBSchema.UserRole.UserID + "=?" + DBSchema.and
	// + DBSchema.UserRole.TableName + "." + DBSchema.UserRole.RoleID
	// + "=" + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.RoleOrUserID + DBSchema.and
	// + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.IsRoleOrUser + "="
	// + DBSchema.UserRoleRightsMapping.RoleLevel + DBSchema.and
	// + DBSchema.UserRole.TableName + "." + DBSchema.UserRole.RoleID
	// + "=" + DBSchema.Role.TableName + "." + DBSchema.Role.RoleID
	//
	// + DBSchema.union + DBSchema.all
	//
	// + DBSchema.select + DBSchema.UserRoleRightsMapping.TableName
	// + ".*,''," + "RTRIM(" + DBSchema.Employee.TableName + "."
	// + DBSchema.Employee.FirstName + ")+RTRIM("
	// + DBSchema.Employee.TableName + "." + DBSchema.Employee.LastName
	// + ")" + DBSchema.from + DBSchema.UserRoleRightsMapping.TableName
	// + "," + DBSchema.Employee.TableName + DBSchema.where
	// + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.RoleOrUserID + "=?" + DBSchema.and
	// + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.IsRoleOrUser + "="
	// + DBSchema.UserRoleRightsMapping.UserLevel + DBSchema.and
	// + DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + "=?"
	//
	// + DBSchema.select + "#user_role.*, " + DBSchema.BizModule.TableName
	// + "." + DBSchema.BizModule.BizModuleName + ","
	// + DBSchema.BizModule.TableName + "."
	// + DBSchema.BizModule.BizModuleURL + ","
	// + DBSchema.BizModule.TableName + "."
	// + DBSchema.BizModule.BizModuleSquenceNo + ","
	// + DBSchema.BizModule.TableName + "."
	// + DBSchema.BizModule.BizModuleIdentifies + ","
	// + DBSchema.BizModule.TableName + "."
	// + DBSchema.BizModule.BizModuleIconCls + ","
	//
	// + DBSchema.BizSubModule.TableName + "."
	// + DBSchema.BizSubModule.BizSubModuleName + ","
	// + DBSchema.BizSubModule.TableName + "."
	// + DBSchema.BizSubModule.BizSubModuleURL + ","
	// + DBSchema.BizSubModule.TableName + "."
	// + DBSchema.BizSubModule.BizSubModuleIdentifies + ","
	// + DBSchema.BizSubModule.TableName + "."
	// + DBSchema.BizSubModule.BizSubModulesIconCls + ","
	// + DBSchema.BizModuleMapping.TableName + "."
	// + DBSchema.BizModuleMapping.BizSubModulesSquenceNo + ","
	//
	// + DBSchema.BizFunction.TableName + "." +
	// DBSchema.BizFunction.BizFunctionSqueID + ","
	// + DBSchema.BizFunction.TableName + "."
	// + DBSchema.BizFunction.BizFunctionName + ","
	// + DBSchema.BizFunction.TableName + "."
	// + DBSchema.BizFunction.BizFunctionURL + ","
	// + DBSchema.BizFunction.TableName + "."
	// + DBSchema.BizFunction.BizFunctionIdentifies + ","
	// + DBSchema.BizOperationRight.TableName + "."
	// + DBSchema.BizOperationRight.RightName + ","
	// + DBSchema.BizOperationRight.TableName + "."
	// + DBSchema.BizOperationRight.RightDesc
	//
	// + DBSchema.from + "#user_role," + DBSchema.BizModule.TableName
	// + "," + DBSchema.BizSubModule.TableName + ","
	// + DBSchema.BizModuleMapping.TableName + ","
	// + DBSchema.BizFunction.TableName + ","
	// + DBSchema.BizOperationRight.TableName
	//
	// + DBSchema.where + "#user_role."
	// + DBSchema.UserRoleRightsMapping.BizModuleID + "="
	// + DBSchema.BizModule.TableName + "."
	// + DBSchema.BizModule.BizModuleID + DBSchema.and + "#user_role."
	// + DBSchema.UserRoleRightsMapping.BizSubModuleID + "="
	// + DBSchema.BizSubModule.TableName + "."
	// + DBSchema.BizSubModule.BizSubModuleID + DBSchema.and
	// + "#user_role." + DBSchema.UserRoleRightsMapping.BizModuleID + "="
	// + DBSchema.BizModuleMapping.TableName + "."
	// + DBSchema.BizModuleMapping.BizModuleID + DBSchema.and
	// + "#user_role." + DBSchema.UserRoleRightsMapping.BizSubModuleID
	// + "=" + DBSchema.BizModuleMapping.TableName + "."
	// + DBSchema.BizModuleMapping.BizSubModuleID + DBSchema.and
	// + "#user_role." + DBSchema.UserRoleRightsMapping.BizFunctionID
	// + "=" + DBSchema.BizFunction.TableName + "."
	// + DBSchema.BizFunction.BizFunctionID + DBSchema.and + "#user_role."
	// + DBSchema.UserRoleRightsMapping.BizOperationID + "="
	// + DBSchema.BizOperationRight.TableName + "."
	// + DBSchema.BizOperationRight.RightID + DBSchema.orderBy
	// + DBSchema.BizModule.TableName + "."
	// + DBSchema.BizModule.BizModuleSquenceNo + ","
	// + DBSchema.BizModuleMapping.TableName + "."
	// + DBSchema.BizModuleMapping.BizSubModulesSquenceNo;
	// bbz mysql
	"SELECT\n"
			+ "    user_role . *,\n"
			+ "    xmis.sec_业务模块表.业务模块名称,\n"
			+ "    xmis.sec_业务模块表.业务模块url,\n"
			+ "    xmis.sec_业务模块表.业务模块排位序号,\n"
			+ "    xmis.sec_业务模块表.业务模块标识,\n"
			+ "    xmis.sec_业务模块表.业务模块图标说明,\n"
			+ "    xmis.sec_业务子模块表.业务子模块名称,\n"
			+ "    xmis.sec_业务子模块表.业务子模块url,\n"
			+ "    xmis.sec_业务子模块表.业务子模块标识,\n"
			+ "    xmis.sec_业务子模块表.业务子模块图标说明,\n"
			+ "    xmis.sec_业务模块对应表.业务子模块排列顺序,\n"
			+ "    xmis.sec_业务功能表.bizFunctionID,\n"
			+ "    xmis.sec_业务功能表.业务功能名称,\n"
			+ "    xmis.sec_业务功能表.业务功能url,\n"
			+ "    xmis.sec_业务功能表.业务功能标识,\n"
			+ "    xmis.sec_操作权限表.操作权限名称,\n"
			+ "    xmis.sec_操作权限表.操作权限描述\n"
			+ "FROM\n"
			+ "    (SELECT\n"
			+ "        xmis . sec_角色用户操作权限定义表 . *,\n"
			+ "            xmis.sec_角色.角色名称 AS role_name,\n"
			+ "            '' AS user_name\n"
			+ "    FROM\n"
			+ "        xmis.sec_角色用户操作权限定义表, xmis.sec_人员角色, xmis.sec_角色\n"
			+ "    WHERE\n"
			+ "        xmis.sec_人员角色.人员ID = ?\n"
			+ "            AND xmis.sec_人员角色.角色ID = xmis.sec_角色用户操作权限定义表.角色或人员ID\n"
			+ "            AND xmis.sec_角色用户操作权限定义表.是否是角色定义 = 1\n"
			+ "            AND xmis.sec_人员角色.角色ID = xmis.sec_角色.角色ID UNION ALL SELECT\n"
			+ "        xmis . sec_角色用户操作权限定义表 . *,\n" + "            '',\n"
			+ "            RTRIM(xmis.hr_人员.姓氏) + RTRIM(xmis.hr_人员.名字)\n"
			+ "    FROM\n" + "        xmis.sec_角色用户操作权限定义表, xmis.hr_人员\n"
			+ "    WHERE\n" + "        xmis.sec_角色用户操作权限定义表.角色或人员ID = ?\n"
			+ "            AND xmis.sec_角色用户操作权限定义表.是否是角色定义 = 0\n"
			+ "            AND xmis.hr_人员.人员ID = ?) user_role,\n"
			+ "    xmis.sec_业务模块表,\n" + "    xmis.sec_业务子模块表,\n"
			+ "    xmis.sec_业务模块对应表,\n" + "    xmis.sec_业务功能表,\n"
			+ "    xmis.sec_操作权限表\n" + "WHERE\n"
			+ "    user_role.业务模块ID = xmis.sec_业务模块表.业务模块ID\n"
			+ "        AND user_role.业务子模块ID = xmis.sec_业务子模块表.业务子模块ID\n"
			+ "        AND user_role.业务模块ID = xmis.sec_业务模块对应表.业务模块ID\n"
			+ "        AND user_role.业务子模块ID = xmis.sec_业务模块对应表.业务子模块ID\n"
			+ "        AND user_role.业务功能ID = xmis.sec_业务功能表.业务功能ID\n"
			+ "        AND user_role.操作权限ID = xmis.sec_操作权限表.操作权限ID\n"
			+ "ORDER BY xmis.sec_业务模块表.业务模块排位序号 , xmis.sec_业务模块对应表.业务子模块排列顺序;";

	public static String getUserRightByDefault = DBSchema.select
			+ DBSchema.distinct + " 0 AS id, "
			+ DBSchema.BizModule.BizModuleName + " as name,"
			+ DBSchema.BizModule.BizModuleURL + " as url " + DBSchema.from
			+ DBSchema.BizModule.TableName + DBSchema.where
			+ DBSchema.BizModule.BizModuleURL + DBSchema.isNot + " NULL "
			+ DBSchema.and + DBSchema.BizModule.BizModuleURL + " <> ''"

			+ DBSchema.union + DBSchema.select + DBSchema.distinct + " 0,"
			+ DBSchema.BizSubModule.BizSubModuleName + ","
			+ DBSchema.BizSubModule.BizSubModuleURL + DBSchema.from
			+ DBSchema.BizSubModule.TableName + DBSchema.where
			+ DBSchema.BizSubModule.BizSubModuleURL + DBSchema.isNot + " NULL "
			+ DBSchema.and + DBSchema.BizSubModule.BizSubModuleURL + " <> ''"

			+ DBSchema.union + DBSchema.select + DBSchema.distinct
			+ DBSchema.BizFunction.BizFunctionID + ","
			+ DBSchema.BizFunction.BizFunctionName + ","
			+ DBSchema.BizFunction.BizFunctionURL + DBSchema.from
			+ DBSchema.BizFunction.TableName + DBSchema.where
			+ DBSchema.BizFunction.BizFunctionURL + DBSchema.isNot + " NULL "
			+ DBSchema.and + DBSchema.BizFunction.BizFunctionURL + " <> ''";

	public static String getSecrityDataScope =
	// DBSchema.select
	// + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.ID + ","
	// + DBSchema.UserRoleRightsMapping.BizOperationID
	// + DBSchema.into
	// + "#user_role"
	// + DBSchema.from
	// + DBSchema.UserRoleRightsMapping.TableName + ","
	// + DBSchema.BizFunction.TableName + ","
	// + DBSchema.UserRole.TableName + "," + DBSchema.Role.TableName
	// + DBSchema.where
	// + DBSchema.BizFunction.TableName + "." +
	// DBSchema.BizFunction.BizFunctionURL + "=?"
	// + DBSchema.and
	// + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.BizFunctionID
	// + "=" + DBSchema.BizFunction.TableName + "." +
	// DBSchema.BizFunction.BizFunctionID
	// + DBSchema.and + DBSchema.UserRole.TableName + "."
	// + DBSchema.UserRole.UserID + "=?" + DBSchema.and
	// + DBSchema.UserRole.TableName + "." + DBSchema.UserRole.RoleID
	// + "=" + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.RoleOrUserID + DBSchema.and
	// + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.IsRoleOrUser + "="
	// + DBSchema.UserRoleRightsMapping.RoleLevel + DBSchema.and
	// + DBSchema.UserRole.TableName + "." + DBSchema.UserRole.RoleID
	// + "=" + DBSchema.Role.TableName + "." + DBSchema.Role.RoleID
	//
	// + DBSchema.union //+ DBSchema.all
	//
	// + DBSchema.select + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.ID + ","
	// + DBSchema.UserRoleRightsMapping.BizOperationID
	// + DBSchema.from
	// + DBSchema.UserRoleRightsMapping.TableName + ","
	// + DBSchema.BizFunction.TableName
	// + DBSchema.where
	// + DBSchema.BizFunction.TableName + "." +
	// DBSchema.BizFunction.BizFunctionURL + "=?"
	// + DBSchema.and
	// + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.BizFunctionID
	// + "=" + DBSchema.BizFunction.TableName + "." +
	// DBSchema.BizFunction.BizFunctionID
	// + DBSchema.and + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.RoleOrUserID + "=?" + DBSchema.and
	// + DBSchema.UserRoleRightsMapping.TableName + "."
	// + DBSchema.UserRoleRightsMapping.IsRoleOrUser + "="
	// + DBSchema.UserRoleRightsMapping.UserLevel
	//
	// + DBSchema.select
	// + DBSchema.BizDataScopeRight.TableName + ".*,"
	// + "#user_role." + DBSchema.UserRoleRightsMapping.BizOperationID + ","
	// + DBSchema.RelationalOperateDefiniens.TableName + "." +
	// DBSchema.RelationalOperateDefiniens.OpName + ","
	// + DBSchema.BizDataDefiniens.TableName + "." +
	// DBSchema.BizDataDefiniens.DataName + ","
	// + DBSchema.BizDataDefiniens.TableName + "." +
	// DBSchema.BizDataDefiniens.DataDefaultValue + ","
	// + DBSchema.BizDataDefiniens.TableName + "." +
	// DBSchema.BizDataDefiniens.DataTypeID + ","
	// + DBSchema.BizDataDefiniens.TableName + "." +
	// DBSchema.BizDataDefiniens.DataBizlogicID + ","
	// + DBSchema.DataTypeDefiniens.TableName + "." +
	// DBSchema.DataTypeDefiniens.DataTypeName
	// + DBSchema.from
	// + "#user_role, "
	// + DBSchema.BizDataScopeRight.TableName + ","
	// + DBSchema.RelationalOperateDefiniens.TableName + ","
	// + DBSchema.BizDataDefiniens.TableName + ","
	// + DBSchema.DataTypeDefiniens.TableName
	// + DBSchema.where
	// + DBSchema.BizDataScopeRight.TableName + "." +
	// DBSchema.BizDataScopeRight.UserRightID
	// + "= #user_role.ID"
	// + DBSchema.and
	// + DBSchema.BizDataScopeRight.TableName + "." +
	// DBSchema.BizDataScopeRight.DataID
	// + "=" + DBSchema.BizDataDefiniens.TableName + "." +
	// DBSchema.BizDataDefiniens.DataID
	// + DBSchema.and
	// + DBSchema.BizDataScopeRight.TableName + "." +
	// DBSchema.BizDataScopeRight.RelationOperateID
	// + "=" + DBSchema.RelationalOperateDefiniens.TableName + "." +
	// DBSchema.RelationalOperateDefiniens.OpID
	// + DBSchema.and
	// + DBSchema.BizDataDefiniens.TableName + "." +
	// DBSchema.BizDataDefiniens.DataTypeID
	// + "=" + DBSchema.DataTypeDefiniens.TableName + "." +
	// DBSchema.DataTypeDefiniens.DataTypeID
	// + DBSchema.and
	// + DBSchema.BizDataScopeRight.TableName + "." +
	// DBSchema.BizDataScopeRight.IsSensitive + "=0"
	// ;
		
	// bbz mysql
	"SELECT  xmis.sec_数据权限定义表.* ,\n"
			+ "        user_role.操作权限ID ,\n"
			+ "        xmis.sys_关系运算符.运算符 ,\n"
			+ "        xmis.sec_业务数据字段表.数据字段名称 ,\n"
			+ "        xmis.sec_业务数据字段表.缺省值描述 ,\n"
			+ "        xmis.sec_业务数据字段表.数据字段类型ID ,\n"
			+ "        xmis.sec_业务数据字段表.业务逻辑判断 ,\n"
			+ "        xmis.sys_数据字段类型定义表.数据字段类型名称\n"
			+ "FROM    ( SELECT    xmis.sec_角色用户操作权限定义表.ID ,\n"
			+ "                    操作权限ID\n"
			+ "          FROM      xmis.sec_角色用户操作权限定义表 ,\n"
			+ "                    xmis.sec_业务功能表 ,\n"
			+ "                    xmis.sec_人员角色 ,\n"
			+ "                    xmis.sec_角色\n"
			+ "          WHERE     xmis.sec_业务功能表.业务功能url = ?\n"
			+ "                    AND xmis.sec_角色用户操作权限定义表.业务功能ID = xmis.sec_业务功能表.业务功能ID\n"
			+ "                    AND xmis.sec_人员角色.人员ID = ?\n"
			+ "                    AND xmis.sec_人员角色.角色ID = xmis.sec_角色用户操作权限定义表.角色或人员ID\n"
			+ "                    AND xmis.sec_角色用户操作权限定义表.是否是角色定义 = 1\n"
			+ "                    AND xmis.sec_人员角色.角色ID = xmis.sec_角色.角色ID\n"
			+ "          UNION\n"
			+ "          SELECT    xmis.sec_角色用户操作权限定义表.ID ,\n"
			+ "                    操作权限ID\n"
			+ "          FROM      xmis.sec_角色用户操作权限定义表 ,\n"
			+ "                    xmis.sec_业务功能表\n"
			+ "          WHERE     xmis.sec_业务功能表.业务功能url = ?\n"
			+ "                    AND xmis.sec_角色用户操作权限定义表.业务功能ID = xmis.sec_业务功能表.业务功能ID\n"
			+ "                    AND xmis.sec_角色用户操作权限定义表.角色或人员ID = ?\n"
			+ "                    AND xmis.sec_角色用户操作权限定义表.是否是角色定义 = 0\n"
			+ "        ) user_role ,\n"
			+ "        xmis.sec_数据权限定义表 ,\n"
			+ "        xmis.sys_关系运算符 ,\n"
			+ "        xmis.sec_业务数据字段表 ,\n"
			+ "        xmis.sys_数据字段类型定义表\n"
			+ "WHERE   xmis.sec_数据权限定义表.权限ID = user_role.ID\n"
			+ "        AND xmis.sec_数据权限定义表.数据字段ID = xmis.sec_业务数据字段表.数据字段ID\n"
			+ "        AND xmis.sec_数据权限定义表.运算符ID = xmis.sys_关系运算符.运算符ID\n"
			+ "        AND xmis.sec_业务数据字段表.数据字段类型ID = xmis.sys_数据字段类型定义表.数据字段类型ID\n"
			+ "        AND xmis.sec_数据权限定义表.是否是敏感字段 = 0";

	public static String getBizlogicDeptList = DBSchema.setNoCountOn +
	"DECLARE @para int " + " set @para = ? " +
	
	DBSchema.select 
	+ DBSchema.Department.TableName + "." + DBSchema.Department.DepartmentID
	+ DBSchema.into + "#dept "
	+ DBSchema.from + DBSchema.Department.TableName 
	+ DBSchema.where + DBSchema.Department.DepartmentID 
	+ DBSchema.in + "()"
	
	+ DBSchema.select + DBSchema.distinct 
	+ DBSchema.DepartmentBelongToCompany.TabelName + "." + DBSchema.DepartmentBelongToCompany.DepartmentID + " as div_id,"
	+ DBSchema.DeptBelongToDept.TableName + "." + DBSchema.DeptBelongToDept.DeptID
	+ DBSchema.into + "#div_dept_t "
	+ DBSchema.from + DBSchema.DepartmentBelongToCompany.TabelName + ", #dept," 
	+ DBSchema.DeptBelongToDept.TableName
	+ DBSchema.where 
	+ DBSchema.DepartmentBelongToCompany.TabelName + "." + DBSchema.DepartmentBelongToCompany.DirectlyFlag + "=1"
	+ DBSchema.and + DBSchema.DepartmentBelongToCompany.TabelName + "." + DBSchema.DepartmentBelongToCompany.DepartmentID
	+ "=#dept." + DBSchema.Department.DepartmentID
	+ DBSchema.and + DBSchema.DeptBelongToDept.TableName + "." + DBSchema.DeptBelongToDept.ParentsID
	+ "=*" + DBSchema.DepartmentBelongToCompany.TabelName + "." + DBSchema.DepartmentBelongToCompany.DepartmentID
	
	+ DBSchema.union
	
	+ DBSchema.select + DBSchema.DeptBelongToDept.ParentsID + "," + DBSchema.DeptBelongToDept.DeptID
	+ DBSchema.from + DBSchema.DeptBelongToDept.TableName 
	+ DBSchema.where + DBSchema.DeptBelongToDept.ParentsID 
	+ DBSchema.in + "("
	+ DBSchema.select + DBSchema.distinct 
	+ DBSchema.DeptBelongToDept.ParentsID 
	+ DBSchema.from + "#dept, " + DBSchema.DeptBelongToDept.TableName
	+ DBSchema.where + "#dept." + DBSchema.Department.DepartmentID
	+ "=" + DBSchema.DeptBelongToDept.TableName + "." + DBSchema.DeptBelongToDept.DeptID + ")"
	

	
	+ DBSchema.update + "#div_dept_t" + DBSchema.set + DBSchema.Department.DepartmentID + "= NULL "
	+ DBSchema.where + DBSchema.Department.DepartmentID + DBSchema.notIn + "("
	+ DBSchema.select + DBSchema.Department.DepartmentID + DBSchema.from + "#dept )"
	
	
	+ DBSchema.select + DBSchema.distinct + "*"
	+ DBSchema.into + "#div_dept"
	+ DBSchema.from + "#div_dept_t"
	
		
	+ DBSchema.select + "div_id, COUNT(*) AS qty"
	+ DBSchema.into + "#div_count" 
	+ DBSchema.from + "#div_dept" 
	+ DBSchema.groupBy + "div_id"
	+ DBSchema.union + DBSchema.all
	+ DBSchema.select + "div_id, -COUNT(*)" 
	+ DBSchema.from + "#div_dept"
	+ DBSchema.where + DBSchema.Department.DepartmentID 
	+ " IS NOT NULL " + DBSchema.groupBy + "div_id"
	
	+ DBSchema.select + "div_id, SUM(qty) result" 
	+ DBSchema.into + "#div_result"
	+ DBSchema.from + "#div_count" 
	+ DBSchema.groupBy + "div_id"
	
	+ DBSchema.select + "div_id"
	+ DBSchema.into + "#result"
	+ DBSchema.from + "#div_dept" 
	+ DBSchema.groupBy + "div_id" 
	+ DBSchema.having + "COUNT(*)=1"
	+ DBSchema.union
	+ DBSchema.select + "#div_dept." + DBSchema.Department.DepartmentID 
	+ DBSchema.from + "#div_dept, #div_result"
	+ DBSchema.where + "result > 0" 
	+ DBSchema.and + "#div_dept.div_id=#div_result.div_id"
	+ DBSchema.and + DBSchema.Department.DepartmentID + " IS NOT NULL "
	+ DBSchema.union
	+ DBSchema.select + "#div_dept.div_id" 
	+ DBSchema.from + "#div_dept, #div_result"
	+ DBSchema.where + "result = 0"
	+ DBSchema.and + "#div_dept.div_id=#div_result.div_id"
	
	
	+ DBSchema.select + "-1 as " + DBSchema.Department.DepartmentID 
	+ ", '所有' as " +DBSchema.Department.DepartmentName
	+ ", '-1' as paramValue" 
	+ DBSchema.union 
	+ DBSchema.select + DBSchema.Department.DepartmentID + "," + DBSchema.Department.DepartmentName
	+ ", xmis.f_Convert_VertToHorz(" + DBSchema.Department.DepartmentID +" , 'DeptBelongToDept')"
	+ DBSchema.from + "#result, " + DBSchema.Department.TableName 
	+ DBSchema.where + "div_id = " + DBSchema.Department.DepartmentID
	
	; 

//		"DECLARE @para INT\n" + 
//		"SET @para = ?;\n" + 
//		"SELECT  DISTINCT\n" + 
//		"       xmis.org_部门归属公司表.部门ID AS div_id ,\n" + 
//		"       xmis.org_部门归属部门表.部门ID\n" + 
//		"INTO   #div_dept_t\n" + 
//		"FROM   xmis.org_部门归属部门表\n" + 
//		"       RIGHT JOIN xmis.org_部门归属公司表 ON xmis.org_部门归属部门表.父部门ID = xmis.org_部门归属公司表.部门ID\n" + 
//		"       JOIN xmis.org_部门 ON xmis.org_部门归属公司表.部门ID = xmis.org_部门.部门ID\n" + 
//		"WHERE  xmis.org_部门归属公司表.是否直属 = 1\n" + 
//		"       AND xmis.org_部门.部门ID IN ()\n" + 
//		"UNION\n" + 
//		"SELECT 父部门ID ,\n" + 
//		"       部门ID\n" + 
//		"FROM   xmis.org_部门归属部门表\n" + 
//		"WHERE  父部门ID IN (\n" + 
//		"       SELECT  DISTINCT\n" + 
//		"               父部门ID\n" + 
//		"       FROM    xmis.org_部门\n" + 
//		"               JOIN xmis.org_部门归属部门表 ON xmis.org_部门.部门ID = xmis.org_部门归属部门表.部门ID\n" + 
//		"       WHERE   xmis.org_部门.部门ID IN ())\n" + 
//		"\n" + 
//		"\n" + 
//		"UPDATE #div_dept_t\n" + 
//		"SET    部门ID = NULL\n" + 
//		"WHERE  部门ID NOT IN ( SELECT    部门ID\n" + 
//		"                     FROM      #dept )\n" + 
//		"\n" + 
//		"\n" + 
//		"\n" + 
//		"SELECT  DISTINCT\n" + 
//		"       *\n" + 
//		"INTO   #div_dept\n" + 
//		"FROM   #div_dept_t\n" + 
//		"SELECT div_id ,\n" + 
//		"       COUNT(*) AS qty\n" + 
//		"INTO   #div_count\n" + 
//		"FROM   #div_dept\n" + 
//		"GROUP BY div_id\n" + 
//		"UNION  ALL\n" + 
//		"SELECT div_id ,\n" + 
//		"       -COUNT(*)\n" + 
//		"FROM   #div_dept\n" + 
//		"WHERE  部门ID IS NOT NULL\n" + 
//		"GROUP BY div_id\n" + 
//		"SELECT div_id ,\n" + 
//		"       SUM(qty) result\n" + 
//		"INTO   #div_result\n" + 
//		"FROM   #div_count\n" + 
//		"GROUP BY div_id\n" + 
//		"SELECT div_id\n" + 
//		"INTO   #result\n" + 
//		"FROM   #div_dept\n" + 
//		"GROUP BY div_id\n" + 
//		"HAVING COUNT(*) = 1\n" + 
//		"UNION\n" + 
//		"SELECT #div_dept.部门ID\n" + 
//		"FROM   #div_dept ,\n" + 
//		"       #div_result\n" + 
//		"WHERE  result > 0\n" + 
//		"       AND #div_dept.div_id = #div_result.div_id\n" + 
//		"       AND 部门ID IS NOT NULL\n" + 
//		"UNION\n" + 
//		"SELECT #div_dept.div_id\n" + 
//		"FROM   #div_dept ,\n" + 
//		"       #div_result\n" + 
//		"WHERE  result = 0\n" + 
//		"       AND #div_dept.div_id = #div_result.div_id\n" + 
//		"SELECT -1 AS 部门ID ,\n" + 
//		"       '所有' AS 部门名称 ,\n" + 
//		"       '-1' AS paramValue\n" + 
//		"UNION\n" + 
//		"SELECT 部门ID ,\n" + 
//		"       部门名称 ,\n" + 
//		"       xmis.f_Convert_VertToHorz(部门ID, 'DeptBelongToDept')\n" + 
//		"FROM   #result ,\n" + 
//		"       xmis.org_部门\n" + 
//		"WHERE  div_id = 部门ID";

}
