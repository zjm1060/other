/**
 * 
 */
package com.bitc.jk.xmis.util.sql;

import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.DBSchema.Employee;

/**
 * @author Ted Li
 * 
 * 2011-5-13
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

	public static String getUserRolesOnFunctionLevel = DBSchema.select
			+ DBSchema.UserRoleRightsMapping.TableName + ".*,"
			+ DBSchema.Role.TableName + "." + DBSchema.Role.RoleName
			+ DBSchema.as + DBSchema.UserRoleRightsMapping.RoleName
			+ ", '' as " + DBSchema.UserRoleRightsMapping.UserName
			+ DBSchema.into + "#user_role" + DBSchema.from
			+ DBSchema.UserRoleRightsMapping.TableName + ","
			+ DBSchema.UserRole.TableName + "," + DBSchema.Role.TableName
			+ DBSchema.where + DBSchema.UserRole.TableName + "."
			+ DBSchema.UserRole.UserID + "=?" + DBSchema.and
			+ DBSchema.UserRole.TableName + "." + DBSchema.UserRole.RoleID
			+ "=" + DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.RoleOrUserID + DBSchema.and
			+ DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.IsRoleOrUser + "="
			+ DBSchema.UserRoleRightsMapping.RoleLevel + DBSchema.and
			+ DBSchema.UserRole.TableName + "." + DBSchema.UserRole.RoleID
			+ "=" + DBSchema.Role.TableName + "." + DBSchema.Role.RoleID

			+ DBSchema.union + DBSchema.all

			+ DBSchema.select + DBSchema.UserRoleRightsMapping.TableName
			+ ".*,''," + "RTRIM(" + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.FirstName + ")+RTRIM("
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.LastName
			+ ")" + DBSchema.from + DBSchema.UserRoleRightsMapping.TableName
			+ "," + DBSchema.Employee.TableName + DBSchema.where
			+ DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.RoleOrUserID + "=?" + DBSchema.and
			+ DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.IsRoleOrUser + "="
			+ DBSchema.UserRoleRightsMapping.UserLevel + DBSchema.and
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + "=?"

			+ DBSchema.select + "#user_role.*, " + DBSchema.BizModule.TableName
			+ "." + DBSchema.BizModule.BizModuleName + ","
			+ DBSchema.BizModule.TableName + "."
			+ DBSchema.BizModule.BizModuleURL + ","
			+ DBSchema.BizModule.TableName + "."
			+ DBSchema.BizModule.BizModuleSquenceNo + ","
			+ DBSchema.BizModule.TableName + "."
			+ DBSchema.BizModule.BizModuleIdentifies + ","
			+ DBSchema.BizModule.TableName + "."
			+ DBSchema.BizModule.BizModuleIconCls + ","

			+ DBSchema.BizSubModule.TableName + "."
			+ DBSchema.BizSubModule.BizSubModuleName + ","
			+ DBSchema.BizSubModule.TableName + "."
			+ DBSchema.BizSubModule.BizSubModuleURL + ","
			+ DBSchema.BizSubModule.TableName + "."
			+ DBSchema.BizSubModule.BizSubModuleIdentifies + ","
			+ DBSchema.BizSubModule.TableName + "."
			+ DBSchema.BizSubModule.BizSubModulesIconCls + ","
			+ DBSchema.BizModuleMapping.TableName + "."
			+ DBSchema.BizModuleMapping.BizSubModulesSquenceNo + ","

			+ DBSchema.BizFunction.TableName + "." + DBSchema.BizFunction.BizFunctionSqueID + ","
			+ DBSchema.BizFunction.TableName + "."
			+ DBSchema.BizFunction.BizFunctionName + ","
			+ DBSchema.BizFunction.TableName + "."
			+ DBSchema.BizFunction.BizFunctionURL + ","
			+ DBSchema.BizFunction.TableName + "."
			+ DBSchema.BizFunction.BizFunctionIdentifies + ","
			+ DBSchema.BizOperationRight.TableName + "."
			+ DBSchema.BizOperationRight.RightName + ","
			+ DBSchema.BizOperationRight.TableName + "."
			+ DBSchema.BizOperationRight.RightDesc

			+ DBSchema.from + "#user_role," + DBSchema.BizModule.TableName
			+ "," + DBSchema.BizSubModule.TableName + ","
			+ DBSchema.BizModuleMapping.TableName + ","
			+ DBSchema.BizFunction.TableName + ","
			+ DBSchema.BizOperationRight.TableName

			+ DBSchema.where + "#user_role."
			+ DBSchema.UserRoleRightsMapping.BizModuleID + "="
			+ DBSchema.BizModule.TableName + "."
			+ DBSchema.BizModule.BizModuleID + DBSchema.and + "#user_role."
			+ DBSchema.UserRoleRightsMapping.BizSubModuleID + "="
			+ DBSchema.BizSubModule.TableName + "."
			+ DBSchema.BizSubModule.BizSubModuleID + DBSchema.and
			+ "#user_role." + DBSchema.UserRoleRightsMapping.BizModuleID + "="
			+ DBSchema.BizModuleMapping.TableName + "."
			+ DBSchema.BizModuleMapping.BizModuleID + DBSchema.and
			+ "#user_role." + DBSchema.UserRoleRightsMapping.BizSubModuleID
			+ "=" + DBSchema.BizModuleMapping.TableName + "."
			+ DBSchema.BizModuleMapping.BizSubModuleID + DBSchema.and
			+ "#user_role." + DBSchema.UserRoleRightsMapping.BizFunctionID
			+ "=" + DBSchema.BizFunction.TableName + "."
			+ DBSchema.BizFunction.BizFunctionID + DBSchema.and + "#user_role."
			+ DBSchema.UserRoleRightsMapping.BizOperationID + "="
			+ DBSchema.BizOperationRight.TableName + "."
			+ DBSchema.BizOperationRight.RightID + DBSchema.orderBy
			+ DBSchema.BizModule.TableName + "."
			+ DBSchema.BizModule.BizModuleSquenceNo + ","
			+ DBSchema.BizModuleMapping.TableName + "."
			+ DBSchema.BizModuleMapping.BizSubModulesSquenceNo;

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

	public static String getSecrityDataScope = DBSchema.select
			+ DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.ID + ","
			+ DBSchema.UserRoleRightsMapping.BizOperationID
			+ DBSchema.into
			+ "#user_role" 
			+ DBSchema.from
			+ DBSchema.UserRoleRightsMapping.TableName + ","
			+ DBSchema.BizFunction.TableName + ","
			+ DBSchema.UserRole.TableName + "," + DBSchema.Role.TableName
			+ DBSchema.where
			+ DBSchema.BizFunction.TableName + "." + DBSchema.BizFunction.BizFunctionURL + "=?"
			+ DBSchema.and
			+ DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.BizFunctionID 
			+ "=" + DBSchema.BizFunction.TableName + "." + DBSchema.BizFunction.BizFunctionID
			+ DBSchema.and + DBSchema.UserRole.TableName + "."
			+ DBSchema.UserRole.UserID + "=?" + DBSchema.and
			+ DBSchema.UserRole.TableName + "." + DBSchema.UserRole.RoleID
			+ "=" + DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.RoleOrUserID + DBSchema.and
			+ DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.IsRoleOrUser + "="
			+ DBSchema.UserRoleRightsMapping.RoleLevel + DBSchema.and
			+ DBSchema.UserRole.TableName + "." + DBSchema.UserRole.RoleID
			+ "=" + DBSchema.Role.TableName + "." + DBSchema.Role.RoleID

			+ DBSchema.union //+ DBSchema.all

			+ DBSchema.select + DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.ID + ","
			+ DBSchema.UserRoleRightsMapping.BizOperationID
			+ DBSchema.from
			+ DBSchema.UserRoleRightsMapping.TableName + ","
			+ DBSchema.BizFunction.TableName 
			+ DBSchema.where
			+ DBSchema.BizFunction.TableName + "." + DBSchema.BizFunction.BizFunctionURL + "=?"
			+ DBSchema.and
			+ DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.BizFunctionID 
			+ "=" + DBSchema.BizFunction.TableName + "." + DBSchema.BizFunction.BizFunctionID
			+ DBSchema.and + DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.RoleOrUserID + "=?" + DBSchema.and
			+ DBSchema.UserRoleRightsMapping.TableName + "."
			+ DBSchema.UserRoleRightsMapping.IsRoleOrUser + "="
			+ DBSchema.UserRoleRightsMapping.UserLevel
			
			+ DBSchema.select 
			+ DBSchema.BizDataScopeRight.TableName + ".*,"
			+ "#user_role." + DBSchema.UserRoleRightsMapping.BizOperationID + ","
			+ DBSchema.RelationalOperateDefiniens.TableName + "." + DBSchema.RelationalOperateDefiniens.OpName + ","
			+ DBSchema.BizDataDefiniens.TableName + "." + DBSchema.BizDataDefiniens.DataName + ","
			+ DBSchema.BizDataDefiniens.TableName + "." + DBSchema.BizDataDefiniens.DataDefaultValue + ","
			+ DBSchema.BizDataDefiniens.TableName + "." + DBSchema.BizDataDefiniens.DataTypeID + ","
			+ DBSchema.BizDataDefiniens.TableName + "." + DBSchema.BizDataDefiniens.DataBizlogicID + ","
			+ DBSchema.DataTypeDefiniens.TableName + "." + DBSchema.DataTypeDefiniens.DataTypeName
			+ DBSchema.from
			+ "#user_role, " 
			+ DBSchema.BizDataScopeRight.TableName + ","
			+ DBSchema.RelationalOperateDefiniens.TableName + ","
			+ DBSchema.BizDataDefiniens.TableName + ","
			+ DBSchema.DataTypeDefiniens.TableName
			+ DBSchema.where
			+ DBSchema.BizDataScopeRight.TableName + "." + DBSchema.BizDataScopeRight.UserRightID 
			+ "= #user_role.ID"
			+ DBSchema.and
			+ DBSchema.BizDataScopeRight.TableName + "." + DBSchema.BizDataScopeRight.DataID 
			+ "=" + DBSchema.BizDataDefiniens.TableName + "." + DBSchema.BizDataDefiniens.DataID
			+ DBSchema.and
			+ DBSchema.BizDataScopeRight.TableName + "." + DBSchema.BizDataScopeRight.RelationOperateID
			+ "=" + DBSchema.RelationalOperateDefiniens.TableName + "." + DBSchema.RelationalOperateDefiniens.OpID
			+ DBSchema.and
			+ DBSchema.BizDataDefiniens.TableName + "." + DBSchema.BizDataDefiniens.DataTypeID
			+ "=" + DBSchema.DataTypeDefiniens.TableName + "." + DBSchema.DataTypeDefiniens.DataTypeID
			+ DBSchema.and
			+ DBSchema.BizDataScopeRight.TableName + "." + DBSchema.BizDataScopeRight.IsSensitive + "=0"
			;
	
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
	
	
	
		

}
