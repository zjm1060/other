package com.bitc.jk.xmis.util;

import com.bitc.jk.xmis.util.DBSchema.ChargedDetail;
import com.bitc.jk.xmis.util.DBSchema.Contract;
import com.bitc.jk.xmis.util.DBSchema.Department;
import com.bitc.jk.xmis.util.DBSchema.DeptBelongToDept;
import com.bitc.jk.xmis.util.DBSchema.Employee;
import com.bitc.jk.xmis.util.DBSchema.EmployeeBelongToDept;
import com.bitc.jk.xmis.util.DBSchema.PaymentDetail;
import com.bitc.jk.xmis.util.DBSchema.Project;
import com.bitc.jk.xmis.util.DBSchema.ProjectTask;

public class SQLScript {

	private static String select = "\n\n SELECT ";

	private static String as = " AS ";

	private static String into = "\n INTO ";

	private static String from = "\n FROM ";

	private static String where = "\n WHERE ";

	private static String and = "\n AND ";

	private static String or = " OR ";

	private static String union = "\n UNION ";

	private static String all = " ALL ";

	private static String groupBy = "\n GROUP BY ";

	private static String having = " HAVING ";

	private static String distinct = " DISTINCT ";

	private static String orderBy = "\n ORDER BY ";

	private static String update = "\n\n UPDATE ";

	private static String set = " SET ";

	private static String delete = "\n\n DELETE ";

	private static String values = "\n VALUES ";

	private static String insert = "\n\n INSERT ";

	private static String ifExists = "\n\n IF EXISTS ";

	private static String elseExists = "\n\n ELSE ";

	private static String like = " LIKE ";
	
	private static String intoResult = into + "#result";
	
	private static String dropTempTable(String tabelName) {

		String sql = "\n\n if exists (select * from dbo.sysobjects where id = object_id(N'"
				+ tabelName
				+ "') and OBJECTPROPERTY(id, N'IsUserTable') = 1)"
				+ "\n drop table " + tabelName + "\n GO ";

		return sql;
	}

	public static String getProjectListByDivision = select
			+ DeptBelongToDept.TableName
			+ "."
			+ DeptBelongToDept.DeptID
			+ " dept_id, "
			+ Department.DepartmentName // " ,dept.部门名称"
			+ ",1 AS isChiild "
			+ into
			+ "#dept"
			+ from
			+ DeptBelongToDept.TableName
			+ ","
			+ Department.TableName
			+ where
			+ DeptBelongToDept.ParentsID
			+ " = ? "
			+ and // " deptDept.父部门ID = ?"
			+ Department.TableName
			+ "."
			+ Department.DepartmentID
			+ " = "
			+ DeptBelongToDept.TableName
			+ "."
			+ DeptBelongToDept.DeptID // " AND dept.部门ID = deptDept.部门ID"
			+ union
			+ select
			+ "?," // " @divisionID"
			+ Department.DepartmentName // " ,dept.部门名称"
			+ "	,0 "
			+ from
			+ Department.TableName // xmis.org_部门 dept"
			+ where
			+ Department.TableName
			+ "."
			+ Department.DepartmentID
			+ " = ? " // " dept.部门ID = @divisionID"

			+ select
			+ distinct // " SELECT DISTINCT"
			+ "	dept_id, "
			+ ProjectTask.TableName
			+ "."
			+ ProjectTask.ProjectID
			+ " proj_id "// " , projTask.项目ID proj_id"
			+ "	, 0 isFinished"
			+ into
			+ "	#deptProj"
			+ from
			+ "	#dept, "
			+ ProjectTask.TableName // " , xmis.prj_项目任务表 projTask"
			+ where
			+ "	#dept.dept_id = "
			+ ProjectTask.AssigedToDeptID // projTask.承接部门ID"

			+ select
			+ "proj_id," // " SELECT proj_id"
			+ Project.TableName
			+ "."
			+ Project.ContractID
			+ " con_id,"// ",proj.项目合同ID con_id"
			+ Contract.ContractAmt
			+ " con_amt " // " ,con.合同总额 con_amt"
			+ into
			+ "#projCon"
			+ from
			+ " #deptProj, "
			+ Project.TableName
			+ ","// " ,xmis.prj_项目 proj"
			+ Contract.TableName // " ,xmis.con_合同 con"
			+ where
			+ " #deptProj.proj_id = "
			+ Project.TableName
			+ "."
			+ Project.ProjectID // proj.项目ID"
			+ and
			+ Project.TableName
			+ "."
			+ Project.ContractID
			+ "="
			+ Contract.TableName
			+ "."
			+ Contract.ContractID // " AND proj.项目合同ID = con.合同ID"

			+ union
			+ all // " UNION ALL"

			+ select
			+ " proj_id,"
			+ Project.TableName
			+ "."
			+ Project.ContractID// " ,proj.项目合同ID "
			+ ",-1*"
			+ PaymentDetail.PaymentAmt// " ,-1*con_pay.付款金额"
			+ from
			+ "#deptProj,"
			+ Project.TableName
			+ ","// " ,xmis.prj_项目 proj"
			+ PaymentDetail.TableName // " ,xmis.con_付款明细表 con_pay"
			+ where
			+ "	#deptProj.proj_id = "
			+ Project.TableName
			+ "."
			+ Project.ProjectID // proj.项目ID"
			+ and
			+ Project.TableName
			+ "."
			+ Project.ContractID
			+ " = "
			+ PaymentDetail.TableName
			+ "."
			+ PaymentDetail.ContractID // con_pay.合同ID"

			+ union
			+ all

			+ select
			+ "proj_id,"
			+ Project.TableName
			+ "."
			+ Project.ContractID // " ,proj.项目合同ID "
			+ ",-1*"
			+ ChargedDetail.ChargedAmt
			+ from
			+ "#deptProj, "
			+ Project.TableName
			+ "," // ",xmis.prj_项目 proj"
			+ ChargedDetail.TableName // " ,xmis.con_收款明细表 con_charge"
			+ where
			+ "#deptProj.proj_id = "
			+ Project.TableName
			+ "."
			+ Project.ProjectID // proj.项目ID"
			+ and
			+ Project.TableName
			+ "."
			+ Project.ContractID
			+ " = "
			+ ChargedDetail.TableName
			+ "."
			+ ChargedDetail.ContractID // con_charge.合同ID"

			+ "\n\n SELECT	proj_id,SUM(#projCon.con_amt) sum_amt"
			+ "	INTO 	#projSum"
			+ "	FROM	#projCon"
			+ "	GROUP BY proj_id"

			+ "\n\n UPDATE #deptProj SET isFinished = 1 "
			+ "	FROM #deptProj, #projSum"
			+ "	WHERE #deptProj.proj_id = #projSum.proj_id AND #projSum.sum_amt = 0"

			+ "\n\n SELECT dept_id,COUNT(*) AS finished_count, 0 AS unfinished_count INTO #deptProjSum "
			+ "FROM #deptProj where isFinished = 1 GROUP BY dept_id"
			+ "	UNION"
			+ "	SELECT dept_id,0 , COUNT(*) FROM #deptProj where isFinished = 0 GROUP BY dept_id"
			
			+ "\n\n SELECT dept_id, SUM(finished_count) AS finished_count, SUM(unfinished_count) AS unfinished_count" +
					"	INTO #b	FROM #deptProjSum GROUP BY dept_id"

			+ "\n\n SELECT identity(int,1,1) as 'row_no', dept_id, 部门名称 dept_name, finished_count, unfinished_count, "
			+ "finished_count+unfinished_count AS total_count"
			+ "	INTO #a	FROM	#b, xmis.org_部门 dept"
			+ "	WHERE dept_id = 部门ID"

			+ "\n\n SELECT dept_id,dept_name,finished_count,unfinished_count,total_count FROM #a order by row_no";

	public static String getDepartmentTree = // "\nIF OBJECT_ID('#deptTemp')
	// IS NOT
	// NULL drop table #deptTemp "
	// + "\nIF OBJECT_ID('#deptTree') IS NOT NULL drop table #deptTree "
	// +

	// dropTempTable("#deptTemp")
	// + dropTempTable("#deptTree")
	// +
		select
		+ DeptBelongToDept.TableName
		+ "."
		+ DeptBelongToDept.ParentsID
		+ as
		+ "dept_id, " // "SELECT xmis.org_部门归属部门表.部门ID dept_id
		+ Department.DepartmentName
		+ as
		+ "dept_name," // ,xmis.org_部门.部门名称 dept_name
		+ DBSchema.Department.TableName
		+ "."
		+ DBSchema.Department.DisabledFlag
		+ as
		+ "dept_disabled "
		+ into
		+ "#deptTemp " // INTO #deptTemp
		+ from
		+ DeptBelongToDept.TableName
		+ " , "
		+ Department.TableName // FROM xmis.org_部门归属部门表, xmis.org_部门
		+ where
		+ DeptBelongToDept.ParentsID
		+ " = ?" // WHERE xmis.org_部门归属部门表.父部门ID = @divisionID
		+ and
		+ DeptBelongToDept.TableName
		+ "."
		+ DeptBelongToDept.ParentsID
		+ "="
		+ Department.TableName
		+ "."
		+ Department.DepartmentID // AND xmis.org_部门归属部门表.部门ID =
		// xmis.org_部门.部门ID
		
		+ union
		+
		
	select
			+ DeptBelongToDept.TableName
			+ "."
			+ DeptBelongToDept.DeptID
			+ as
			+ "dept_id, " // "SELECT xmis.org_部门归属部门表.部门ID dept_id
			+ Department.DepartmentName
			+ as
			+ "dept_name," // ,xmis.org_部门.部门名称 dept_name
			+ DBSchema.Department.TableName
			+ "."
			+ DBSchema.Department.DisabledFlag
			+ as
			+ "dept_disabled "
			//+ into
			//+ "#deptTemp " // INTO #deptTemp
			+ from
			+ DeptBelongToDept.TableName
			+ " , "
			+ Department.TableName // FROM xmis.org_部门归属部门表, xmis.org_部门
			+ where
			+ DeptBelongToDept.ParentsID
			+ " = ?" // WHERE xmis.org_部门归属部门表.父部门ID = @divisionID
			+ and
			+ DeptBelongToDept.TableName
			+ "."
			+ DeptBelongToDept.DeptID
			+ "="
			+ Department.TableName
			+ "."
			+ Department.DepartmentID // AND xmis.org_部门归属部门表.部门ID =
			// xmis.org_部门.部门ID

			+ select
			+ "#deptTemp.*, "
			+ EmployeeBelongToDept.TableName
			+ "."
			+ EmployeeBelongToDept.EmployeeID
			+ as
			+ "user_id, "
			+ EmployeeBelongToDept.Position
			+ as
			+ "position "
			+ into
			+ "#deptTree" // SELECT #deptTemp.*,xmis.hr_人员部门关系表.人员ID user_id
			// INTO #deptTree
			+ from
			+ "#deptTemp,"
			+ EmployeeBelongToDept.TableName // FROM #deptTemp,
			// xmis.hr_人员部门关系表
			+ where
			+ EmployeeBelongToDept.TableName
			+ "."
			+ EmployeeBelongToDept.DepartmentID
			+ " =* #deptTemp.dept_id"
			+ and
			+ EmployeeBelongToDept.TableName
			+ "."
			+ EmployeeBelongToDept.EmployeeID
			+ " >0 " // WHERE xmis.hr_人员部门关系表.部门ID =* #deptTemp.dept_id AND
			// 人员ID>0

			+ select
			+ "#deptTree.*, RTRIM("
			+ Employee.FirstName
			+ ")+RTRIM("
			+ Employee.LastName
			+ ") user_name,"// SELECT #deptTree.*,
			// RTRIM(xmis.hr_人员.姓氏)+RTRIM(xmis.hr_人员.名字)
			// user_name FROM
			+ DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.DisabledFlag + as + "emp_disabled " + from
			+ "#deptTree,"
			+ Employee.TableName // #deptTree, xmis.hr_人员
			+ where + "#deptTree.user_id *= " + Employee.ID + orderBy
			+ "dept_disabled, dept_id ,emp_disabled, user_id" // WHERE
			// #deptTree.user_id
	// *=
	// xmis.hr_人员.人员ID
	// ORDER BY
	// dept_id

	;

	public static String getEmpTotalSummaryStr = select // SELECT
			// member.项目成员ID
			+ DBSchema.MemberOfProjectTask.TableName
			+ "."
			+ DBSchema.MemberOfProjectTask.MemberID
			+ as
			+ "user_id," // user_id,
			+ DBSchema.MemberOfProjectTask.TableName
			+ "."
			+ DBSchema.MemberOfProjectTask.ProjectTaskID
			+ as
			+ "task_id," // member.项目任务ID task_id,
			+ DBSchema.ProjectTask.TableName
			+ "."
			+ DBSchema.ProjectTask.ProjectID
			+ as
			+ "proj_id," // task.项目ID proj_id,
			+ DBSchema.PlanOfProjectTask.TableName
			+ "."
			+ DBSchema.PlanOfProjectTask.BengineDate
			+ as
			+ "beg_date," // taskPlan.开始日期 beg_date,
			+ DBSchema.PlanOfProjectTask.TableName
			+ "."
			+ DBSchema.PlanOfProjectTask.EndDate
			+ as
			+ "end_date " // taskPlan.结束日期 end_date
			+ into // INTO
			+ "#task " // #task
			+ from // FROM
			+ DBSchema.ProjectTask.TableName
			+ "," // xmis.prj_项目任务表 task,
			+ DBSchema.MemberOfProjectTask.TableName
			+ "," // xmis.prj_项目任务成员 member,
			+ DBSchema.PlanOfProjectTask.TableName // xmis.prj_任务进度计划 taskPlan
			+ where // WHERE
			+ DBSchema.ProjectTask.AssigedToDeptID
			+ " = ? " // task.承接部门ID = 2
			+ and
			+ DBSchema.ProjectTask.TableName
			+ "."
			+ DBSchema.ProjectTask.ProjectTaskID
			+ "="
			+ DBSchema.MemberOfProjectTask.TableName
			+ "."
			+ DBSchema.MemberOfProjectTask.ProjectTaskID // AND task.项目任务ID =
			// member.项目任务ID
			+ and
			+ DBSchema.PlanOfProjectTask.TableName
			+ "."
			+ DBSchema.ProjectTask.ProjectTaskID
			+ "="
			+ DBSchema.ProjectTask.TableName
			+ "."
			+ DBSchema.ProjectTask.ProjectTaskID // AND taskPlan.项目任务ID =
			// task.项目任务ID

			+ "\n\nSELECT user_id,	 proj_id,	 COUNT(task_id) finished,	 0 unfinished,	 0 not_start	 "
			+ "\nINTO	 #taskCount	 FROM	 #task	 WHERE	 end_date < getdate()	 GROUP BY	 user_id, proj_id "
			+ "\nUNION ALL"
			+ "\nSELECT user_id,	 proj_id,	 0,	 COUNT(task_id),	 0	 FROM	 #task	 WHERE	 beg_date <= GETDATE() "
			+ "AND getdate() < end_date	 GROUP BY	 user_id, proj_id"
			+ "\nUNION ALL"
			+ "\nSELECT user_id,	 proj_id,	 0,	 0,	 COUNT(task_id)	 FROM	 #task	 WHERE	 getdate() < beg_date "
			+ "GROUP BY	 user_id, proj_id"

			+ "\n\nSELECT	user_id,	 proj_id,	 SUM(finished) finished, "
			+ "SUM(unfinished) unfinished,	 SUM(not_start) not_start"
			+ "\nINTO	 #taskSUM "
			+ "\nFROM	 #taskCount "
			+ "GROUP BY	 user_id, proj_id "
			// + "\nORDER BY user_id, proj_id, not_start, unfinished, finished "

			+ "\n\nSELECT 	user_id,"
			+ "RTRIM("
			+ DBSchema.Employee.FirstName
			+ ") + RTRIM("
			+ DBSchema.Employee.LastName
			+ ") user_name, "
			+ "proj_id, RTRIM("
			+ DBSchema.Project.ProjectCode
			+ ") proj_code,"
			+ "RTRIM("
			+ DBSchema.Project.ProjectName
			+ ") proj_name,"
			+ "CONVERT(varchar(10),"
			+ DBSchema.Project.StartDate
			+ ",101)"
			+ as
			+ "start_date,"
			+ "finished finished_task,		unfinished unfinished_task,		not_start not_start_task"
			+ into + "#result" + from
			+ "#taskSUM,"
			+ DBSchema.Employee.TableName + ","
			+ DBSchema.Project.TableName
			+ where + "user_id = " + DBSchema.Employee.ID + and
			+ "proj_id = "
			+ DBSchema.Project.ProjectID;

	public static String resultStrByBiz = select 
			+ "#result.*" 
			+ into + "#resultC"
			+ from + "#result"
			+ where + "1=2"
			
			+ insert + into + "#resultC"
			+ select + "*"
			+ from + "#result"
			;

	
	public static String getResultStrWithPaging = "\n\nselect identity(int,1,1) as row_no, #resultC.* "
			+ "\n INTO #resultA "
			+ "FROM #resultC "

			+ "\n\nSELECT * from #resultA WHERE row_no = (select max(row_no) from #resultA ) UNION ALL "
			+ "\nSELECT * from #resultA "
			+ "WHERE row_no >= ? "
			+ " AND row_no < ?";
	
	public static String getResultStrWithoutPaging = "\nSELECT * from #resultC "
		;
	
	public static String positionStr = "\n\nselect identity(int,1,1) as row_no, #resultC.* "
			+ "\n INTO #resultA "
			+ "FROM #resultC "
			
			+ "\n\nSelect max(row_no) as row_no from #resultA  UNION ALL "
			+ select 
			+ "row_no"
			+ from + "#resultA"
			;

	public static String getEmployeeInof1 = select + DBSchema.Address.District
			+ "," + DBSchema.Address.ZipCode + ","
			+ DBSchema.Address.AddressInfo + "," + DBSchema.City.CityName + ","
			+ DBSchema.City.AreaCode + "," + DBSchema.Province.ProvinceName
			+ "," + DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + ","
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.Position + " position, "
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentName + ","
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.ParentsID + " division_id,"
			+ "division." + DBSchema.Department.DepartmentName
			+ " division_name, " + DBSchema.DepartmentBelongToCompany.TabelName
			+ "." + DBSchema.DepartmentBelongToCompany.CompanyID + ","
			+ DBSchema.Company.TableName + "." + DBSchema.Company.CompanyName
			+ ",DATEDIFF(yyyy," + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.Birthday + ",GETDATE()) age, "
			+ "Convert(varchar(10)," + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.JoinDate + ",1) as join_date,"
			+ "Convert(varchar(10)," + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.Birthday + ",1) as birthday,"
			+ DBSchema.EducationLevel.TableName + "."
			+ DBSchema.EducationLevel.EducationLevelName + ","
			+ DBSchema.Employee.TableName + ".* " + from
			+ DBSchema.Employee.TableName + "," + DBSchema.Address.TabelName
			+ "," + DBSchema.City.TableName + "," + DBSchema.Province.TableName
			+ "," + DBSchema.EmployeeBelongToDept.TableName + ","
			+ DBSchema.Department.TableName + ","
			+ DBSchema.DeptBelongToDept.TableName + ","
			+ DBSchema.DepartmentBelongToCompany.TabelName + ","
			+ DBSchema.Company.TableName + ","
			+ DBSchema.EducationLevel.TableName + ","
			+ DBSchema.Department.TableName + " division " + where
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID
			+ " = ? " + and + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.OfficeAddressID + " = "
			+ DBSchema.Address.TabelName
			+ "."
			+ DBSchema.Address.AddressID
			+ and
			+ DBSchema.Address.TabelName
			+ "."
			+ DBSchema.Address.CityID
			+ " = "
			+ DBSchema.City.TableName
			+ "."
			+ DBSchema.City.CityID
			+ and
			+ DBSchema.City.TableName
			+ "."
			+ DBSchema.City.ProvinceID
			+ " = "
			+ DBSchema.Province.TableName
			+ "."
			+ DBSchema.Province.ProvinceID
			+ and
			+ DBSchema.Employee.TableName
			+ "."
			+ DBSchema.Employee.ID
			+ " = "
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID
			+ and
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.IsPartTime
			+ "=1" // 全职
			+ and + DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + " = "
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + and
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.DeptID + " = "
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + and
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.ParentsID + " = " + "division."
			+ DBSchema.Department.DepartmentID + and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + " = "
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.ParentsID + and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID + " = "
			+ DBSchema.Company.TableName + "." + DBSchema.Company.CompanyID
			+ and + DBSchema.EducationLevel.TableName + "."
			+ DBSchema.EducationLevel.EducationLevelID + "=*"
			+ DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.EduicationLevelID;

	public static String getEmployeeInof = select
			+ "DATEDIFF(yyyy,"
			+ DBSchema.Employee.TableName
			+ "."
			+ DBSchema.Employee.Birthday
			+ ",GETDATE()) age, "
			+ "Convert(varchar(10),"
			+ DBSchema.Employee.TableName
			+ "."
			+ DBSchema.Employee.JoinDate
			+ ",1) as join_date,"
			+ "Convert(varchar(10),"
			+ DBSchema.Employee.TableName
			+ "."
			+ DBSchema.Employee.Birthday
			+ ",1) as birthday,"
			+ DBSchema.Employee.TableName
			+ ".*,"
			+ DBSchema.EducationLevel.EducationLevelName
			+ ","
			+ DBSchema.EducationLevel.SchoolName
			+ ","
			+ "Convert(varchar(10),"
			+ DBSchema.EducationLevel.GraduationDate
			+ ",1) as "
			+ DBSchema.EducationLevel.GraduationDate
			+ ","
			+ DBSchema.EducationLevel.Major
			+ ","
			+ DBSchema.EducationLevel.TableName
			+ '.'
			+ DBSchema.EducationLevel.Remark
			+ " educationRemark "
			+ into
			+ " #hr_edu "
			+ from
			+ DBSchema.Employee.TableName
			+ ","
			+ DBSchema.EducationLevel.TableName
			+ where
			+ DBSchema.Employee.ID
			+ " = ? "
			+ and
			+ DBSchema.Employee.TableName
			+ "."
			+ DBSchema.Employee.EduicationLevelID
			+ " *= "
			+ DBSchema.EducationLevel.TableName
			+ "."
			+ DBSchema.EducationLevel.EducationLevelID

			+ select
			+ "#hr_edu.*, "
			+ DBSchema.Address.District
			+ ","
			+ DBSchema.Address.ZipCode
			+ ","
			+ DBSchema.Address.AddressInfo
			+ ","
			+ DBSchema.Address.CityID
			+ into
			+ " #hr_add "
			+ from
			+ "#hr_edu, "
			+ DBSchema.Address.TabelName
			+ where
			+ "#hr_edu."
			+ DBSchema.Employee.OfficeAddressID
			+ " *= "
			+ DBSchema.Address.TabelName
			+ "."
			+ DBSchema.Address.AddressID

			+ select
			+ "#hr_add.*,"
			+ DBSchema.City.CityName
			+ ","
			+ DBSchema.City.AreaCode
			+ ","
			+ DBSchema.City.ProvinceID
			+ into
			+ "#hr_city "
			+ from
			+ "#hr_add,"
			+ DBSchema.City.TableName
			+ where
			+ "#hr_add."
			+ DBSchema.Address.CityID
			+ " *= "
			+ DBSchema.City.TableName
			+ "."
			+ DBSchema.City.CityID

			+ select
			+ "#hr_city.*,"
			+ DBSchema.Province.ProvinceName
			+ into
			+ "#hr_pro "
			+ from
			+ "#hr_city,"
			+ DBSchema.Province.TableName
			+ where
			+ "#hr_city."
			+ DBSchema.City.ProvinceID
			+ " *= "
			+ DBSchema.Province.TableName
			+ "."
			+ DBSchema.Province.ProvinceID

			+ select
			+ "#hr_pro.*, "
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID
			+ ","
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.Position
			+ " position "
			// + DBSchema.Department.DepartmentName
			+ into
			+ "#hr_dept "
			+ from
			+ "#hr_pro,"
			+ DBSchema.EmployeeBelongToDept.TableName // +","+DBSchema.Department.TableName
			+ where
			+ "#hr_pro."
			+ DBSchema.Employee.ID
			+ " *= "
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID
			+ and
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.IsPartTime
			+ "=1" // 全职
			// + and
			// +
			// DBSchema.EmployeeBelongToDept.TableName+"."+DBSchema.EmployeeBelongToDept.DepartmentID
			// + " = " +
			// DBSchema.Department.TableName+"."+DBSchema.Department.DepartmentID

			+ select
			+ "#hr_dept.*,"
			+ DBSchema.DeptBelongToDept.ParentsID
			+ " as division_id "
			// +
			// DBSchema.Department.TableName+"."+DBSchema.Department.DepartmentName
			// + " as division_name "
			+ into
			+ "#hr_div "
			+ from
			+ "#hr_dept, "
			+ DBSchema.DeptBelongToDept.TableName // +","+DBSchema.Department.TableName
			+ where
			+ "#hr_dept."
			+ DBSchema.EmployeeBelongToDept.DepartmentID
			+ " *= "
			+ DBSchema.DeptBelongToDept.TableName
			+ "."
			+ DBSchema.DeptBelongToDept.DeptID
			// + and
			// + DBSchema.DeptBelongToDept.ParentsID
			// + " = " +
			// DBSchema.Department.TableName+"."+DBSchema.Department.DepartmentID

			+ select
			+ "#hr_div.*, "
			+ DBSchema.DepartmentBelongToCompany.CompanyID
			// +","+ DBSchema.Company.CompanyName
			+ into
			+ "#hr_comp"
			+ from
			+ "#hr_div,"
			+ DBSchema.DepartmentBelongToCompany.TabelName
			// + DBSchema.Company.TableName
			+ where
			+ "#hr_div.division_id *= "
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID
			+ and
			+ DBSchema.DepartmentBelongToCompany.DirectlyFlag
			+ " = 1 " // 直属的部门
			// + and
			// +
			// DBSchema.DepartmentBelongToCompany.TabelName+"."+DBSchema.DepartmentBelongToCompany.CompanyID
			// + " = " +
			// DBSchema.Company.TableName+"."+DBSchema.Company.CompanyID

			+ select + "MAX(" + DBSchema.EmployeeLunchAddress.UpdateDate
			+ ") update_date, " + DBSchema.EmployeeLunchAddress.EmployeeID
			+ " as user_id" + into + "#max_lunch" + from
			+ DBSchema.EmployeeLunchAddress.TableName + groupBy
			+ DBSchema.EmployeeLunchAddress.EmployeeID

			+ select + DBSchema.EmployeeLunchAddress.TableName + "."
			+ DBSchema.EmployeeLunchAddress.UpdateDate + " as update_date ,"
			+ DBSchema.EmployeeLunchAddress.TableName + "."
			+ DBSchema.EmployeeLunchAddress.EmployeeID + " as user_id , "
			+ DBSchema.EmployeeLunchAddress.TableName + "."
			+ DBSchema.EmployeeLunchAddress.LunchAddressID + " as lunch_id "
			+ into + "#lunch" + from + DBSchema.EmployeeLunchAddress.TableName
			+ ", #max_lunch " + where
			+ DBSchema.EmployeeLunchAddress.UpdateDate
			+ "= #max_lunch.update_date " + and
			+ DBSchema.EmployeeLunchAddress.EmployeeID
			+ "= #max_lunch.user_id "

			+ select + "#hr_comp.*, #lunch.lunch_id " + into + " #hr_lunch "
			+ from + "#hr_comp, #lunch " + where
			+ "#hr_comp.人员ID *= #lunch.user_id "

			+ select + "#hr_lunch.*," + DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentName + ",division."
			+ DBSchema.Department.DepartmentName + " as division_name, "
			+ DBSchema.Company.CompanyName + ","
			+ DBSchema.LunchAddress.TableName + "."
			+ DBSchema.LunchAddress.LunchAddressName + from + "#hr_lunch, "
			+ DBSchema.Department.TableName + ","
			+ DBSchema.Department.TableName + " division, "
			+ DBSchema.Company.TableName + ","
			+ DBSchema.LunchAddress.TableName + where + "#hr_lunch."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + " *= "
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + and
			+ "#hr_lunch.division_id *= division."
			+ DBSchema.Department.DepartmentID + and + "#hr_lunch."
			+ DBSchema.DepartmentBelongToCompany.CompanyID + " *= "
			+ DBSchema.Company.TableName + "." + DBSchema.Company.CompanyID
			+ and + "#hr_lunch.lunch_id *= " + DBSchema.LunchAddress.TableName
			+ "." + DBSchema.LunchAddress.LunchAddressID;

	public static String changePassword = update + DBSchema.Employee.TableName
			+ set + DBSchema.Employee.Password + " = ?, "
			+ DBSchema.Employee.UpdateID + "= ?, "
			+ DBSchema.Employee.UpdateDate + "=getdate() " + where
			+ DBSchema.Employee.ID + " = ? ";

	public static String saveUserPhotoName = update
			+ DBSchema.Employee.TableName + set + DBSchema.Employee.imgName
			+ " = ?, " + DBSchema.Employee.UpdateID + "= ?,"
			+ DBSchema.Employee.UpdateDate + "= getdate()" + where
			+ DBSchema.Employee.ID + " = ? ";

	public static String getPoliticalStatusList = select + "*" + from
			+ DBSchema.PoliticalStatus.TableName;

	public static String getLunchAddressList = select + "*" + from
			+ DBSchema.LunchAddress.TableName;

	public static String updateEdu = update + DBSchema.EducationLevel.TableName
			+ set + DBSchema.EducationLevel.SchoolName + "= ?,"
			+ DBSchema.EducationLevel.GraduationDate + "= ?,"
			+ DBSchema.EducationLevel.Major + "= ?,"
			+ DBSchema.EducationLevel.Remark + "= ?" + where
			+ DBSchema.EducationLevel.EducationLevelID + "= ?";

	public static String createNewEdu = insert + into
			+ DBSchema.EducationLevel.TableName + "("
			+ DBSchema.EducationLevel.EducationLevelID + ","
			+ DBSchema.EducationLevel.EducationLevelName + ","
			+ DBSchema.EducationLevel.SchoolName + ","
			+ DBSchema.EducationLevel.GraduationDate + ","
			+ DBSchema.EducationLevel.Major + ","
			+ DBSchema.EducationLevel.Remark + ")" + values + "(?,?,?,?,?,?)";

	public static String getMaxIDFromTab(String columnName, String tableName){
		//select + "count(*)+1001" + from;
		
		
//		return select + "Max(CONVERT(INT," + columnName + "))" + from + tableName;
		return select + "Max(" + columnName + ")" + from + tableName;
	}
	
	public static String getMaxIDFromTab(String columnName, String tableName, String whereStr){
		
		System.out.println(getMaxIDFromTab(columnName, tableName) + where + whereStr);
		return getMaxIDFromTab(columnName, tableName) + where + whereStr;
	}

	public static String changeEduForEmployee = update
			+ DBSchema.Employee.TableName + set
			+ DBSchema.Employee.EduicationLevelID + "= ?, "
			+ DBSchema.Employee.UpdateID + "= ?,"
			+ DBSchema.Employee.UpdateDate + "= getdate()" + where
			+ DBSchema.Employee.ID + "= ?";

	public static String createNewDept = insert + into
			+ DBSchema.Department.TableName + "("
			+ DBSchema.Department.DepartmentID + ","
			+ DBSchema.Department.DepartmentName + ","
			+ DBSchema.Department.Abbreviation + ","
			+ DBSchema.Department.DisabledFlag + ")" + values + "(?,?,?,0)";

	public static String setDeptBelongToDept = insert + into
			+ DBSchema.DeptBelongToDept.TableName + "("
			+ DBSchema.DeptBelongToDept.DeptID + ","
			+ DBSchema.DeptBelongToDept.ParentsID + ")" + values + "(?,?)";

	public static String createNewUser = insert + into
			+ DBSchema.Employee.TableName + "(" 
			+ DBSchema.Employee.ID + ","			//1
			+ DBSchema.Employee.FirstName + "," 	//2
			+ DBSchema.Employee.LastName + "," 		//3
			+ DBSchema.Employee.LoginName + ","		//4
			+ DBSchema.Employee.Password + "," 		
			+ DBSchema.Employee.SexID + ","			
			+ DBSchema.Employee.UpdateID + "," 		//5
			+ DBSchema.Employee.UpdateDate + "," 	
			+ DBSchema.Employee.DisabledFlag + ")"	 
			+ values
			+ "(?,?,?,?,'000000',1,?,getdate(),0)";

	public static String setEmpBelongToDept = insert + into
			+ DBSchema.EmployeeBelongToDept.TableName + "("
			+ DBSchema.EmployeeBelongToDept.EmployeeID + ","
			+ DBSchema.EmployeeBelongToDept.DepartmentID + ","
			+ DBSchema.EmployeeBelongToDept.Position + ","
			+ DBSchema.EmployeeBelongToDept.IsPartTime + ")" + values
			+ "(?,?,'',1)";

	public static String setDefaultAddressID = select + distinct
			+ DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.OfficeAddressID + as + "add_id, 1 as level "
			+ into + "#address" + from + DBSchema.Employee.TableName + ","
			+ DBSchema.EmployeeBelongToDept.TableName + where
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + "=?" + and
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "="
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + and
			+ DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.OfficeAddressID + " is not null "

			+ union

			+ select + distinct + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.OfficeAddressID + as + "add_id, 2 as level "
			+ from + DBSchema.Employee.TableName + ","
			+ DBSchema.EmployeeBelongToDept.TableName + ","
			+ DBSchema.DeptBelongToDept.TableName + where
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.ParentsID + "=(" + select
			+ DBSchema.DeptBelongToDept.ParentsID + from
			+ DBSchema.DeptBelongToDept.TableName + where
			+ DBSchema.DeptBelongToDept.DeptID + "=?" + ")" + and
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + "="
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.DeptID + and
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "="
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + and
			+ DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.OfficeAddressID + " is not null "

			+ union

			+ select + DBSchema.Company.TableName + "."
			+ DBSchema.Company.AddressID + ", 3 " + from
			+ DBSchema.Company.TableName + ","
			+ DBSchema.DeptBelongToDept.TableName + ","
			+ DBSchema.DepartmentBelongToCompany.TabelName + where
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.DeptID + "=?" + and
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.ParentsID + "="
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DirectlyFlag + "=1" + and
			+ DBSchema.Company.TableName + "." + DBSchema.Company.CompanyID
			+ "=" + DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID + and
			+ DBSchema.Company.TableName + "." + DBSchema.Company.AddressID
			+ " is not null "

			+ "\nDECLARE @addID INT" + "\nSET ROWCOUNT 1" + select
			+ " @addID = add_id FROM #address ORDER BY level, add_id"
			+ "\nSET ROWCOUNT 0"

			+ update + DBSchema.Employee.TableName + set
			+ DBSchema.Employee.OfficeAddressID + "=@addID" + where
			+ DBSchema.Employee.ID + "=?"

	;

	public static String setDefaultLunchAddress = insert + into
			+ DBSchema.EmployeeLunchAddress.TableName + "("
			+ DBSchema.EmployeeLunchAddress.UpdateDate + ","
			+ DBSchema.EmployeeLunchAddress.EmployeeID + ","
			+ DBSchema.EmployeeLunchAddress.LunchAddressID + ","
			+ DBSchema.EmployeeLunchAddress.BookedFlag + ","
			+ DBSchema.EmployeeLunchAddress.UpdateID + ")" + values
			+ "(getdate(),?,1,1, ?)";

	public static String updateLunchAddress = update
			+ DBSchema.EmployeeLunchAddress.TableName + set
			+ DBSchema.EmployeeLunchAddress.LunchAddressID + "=?,"
			+ DBSchema.EmployeeLunchAddress.UpdateDate + "=getdate(),"
			+ DBSchema.EmployeeLunchAddress.UpdateID + "=?" + where
			+ DBSchema.EmployeeLunchAddress.EmployeeID + "=?";

	public static String inertLunchAddress = insert + into
			+ DBSchema.EmployeeLunchAddress.TableName + "("
			+ DBSchema.EmployeeLunchAddress.UpdateDate + ","
			+ DBSchema.EmployeeLunchAddress.EmployeeID + ","
			+ DBSchema.EmployeeLunchAddress.LunchAddressID + ","
			+ DBSchema.EmployeeLunchAddress.BookedFlag + ","
			+ DBSchema.EmployeeLunchAddress.UpdateID + ")" + values
			+ "(getdate(),?,?,0,?)";

	public static String updateEmployeeInfo = update
			+ DBSchema.Employee.TableName + set
			+ DBSchema.Employee.FirstName
			+ "=?," // 1
			+ DBSchema.Employee.LastName
			+ "=?," // 2
			+ DBSchema.Employee.LoginName
			+ "=?," // 3
			+ DBSchema.Employee.SexID
			+ "=?," // 4
			+ DBSchema.Employee.JoinDate
			+ "=?," // 5
			+ DBSchema.Employee.Birthday
			+ "=?," // 6
			+ DBSchema.Employee.IDCard
			+ "=?," // 7
			+ DBSchema.Employee.MobileNo
			+ "=?," // 8
			+ DBSchema.Employee.Email
			+ "=?," // 9
			+ DBSchema.Employee.WebSite
			+ "=?," // 10
			+ DBSchema.Employee.Position
			+ "=?," // 11
			+ DBSchema.Employee.OfficeTelNo
			+ "=?," // 12
			+ DBSchema.Employee.ExtNo
			+ "=?," // 13
			+ DBSchema.Employee.FaxNo
			+ "=?," // 14
			+ DBSchema.Employee.HomeAddress
			+ "=?," // 15
			+ DBSchema.Employee.ZipCode
			+ "=?," // 16
			+ DBSchema.Employee.HomeTelNo
			+ "=?," // 17
			+ DBSchema.Employee.PoliticalStatus
			+ "=?," // 18
			+ DBSchema.Employee.NativePlace
			+ "=?," // 19
			+ DBSchema.Employee.MaritalStatus
			+ "=?," // 20
			+ DBSchema.Employee.SpouseName
			+ "=?," // 21
			+ DBSchema.Employee.IssueName
			+ "=?," // 22
			+ DBSchema.Employee.IssueSex
			+ "=?," // 23
			+ DBSchema.Employee.Hobby
			+ "=?," // 24
			+ DBSchema.Employee.Specialty
			+ "=?," // 25
			+ DBSchema.Employee.DisabledFlag
			+ "=?," // 26
			+ DBSchema.Employee.Remark
			+ "=?," // 27
			+ DBSchema.Employee.UpdateID
			+ "= ?," // 28
			+ DBSchema.Employee.UpdateDate + "= getdate()" + where
			+ DBSchema.Employee.ID + "=?" // 29
	;

	public static String disableDeparment = update
			+ DBSchema.Department.TableName + set
			+ DBSchema.Department.DisabledFlag + "=1" + where
			+ DBSchema.Department.DepartmentID + "=?";

	public static String disableEmployee = update + DBSchema.Employee.TableName
			+ set + DBSchema.Employee.DisabledFlag + "=1,"
			+ DBSchema.Employee.UpdateID
			+ "= ?," // 28
			+ DBSchema.Employee.UpdateDate + "= getdate()" + where
			+ DBSchema.Employee.ID + "=?";

	public static String getLunchAddressListByDiv = select
			+ "MAX("
			+ DBSchema.EmployeeLunchAddress.TableName
			+ "."
			+ DBSchema.EmployeeLunchAddress.UpdateDate
			+ ") update_date,"
			+ DBSchema.EmployeeLunchAddress.EmployeeID
			+ " emp_id "
			+ into
			+ "#lastLunchAdd"
			+ from
			+ DBSchema.EmployeeLunchAddress.TableName
			+ groupBy
			+ DBSchema.EmployeeLunchAddress.EmployeeID

			+ select
			+ DBSchema.EmployeeLunchAddress.TableName
			+ ".*,"
			+ DBSchema.EmployeeLunchAddress.TableName
			+ "."
			+ DBSchema.EmployeeLunchAddress.LunchAddressID
			+ " as oldID"
			+ into
			+ "#lunch"
			+ from
			+ "#lastLunchAdd, "
			+ DBSchema.EmployeeLunchAddress.TableName
			+ where
			+ "emp_id="
			+ DBSchema.EmployeeLunchAddress.TableName
			+ "."
			+ DBSchema.EmployeeLunchAddress.EmployeeID
			+ and
			+ "update_date="
			+ DBSchema.EmployeeLunchAddress.TableName
			+ "."
			+ DBSchema.EmployeeLunchAddress.UpdateDate

			// + select + "MAX("
			// + DBSchema.EmployeeLunchAddress.TableName + "."
			// + DBSchema.EmployeeLunchAddress.UpdateDate + ") update_date,"
			// + DBSchema.EmployeeLunchAddress.EmployeeID + " emp_id "
			// + into + "#unbooked"
			// + from + DBSchema.EmployeeLunchAddress.TableName
			// + where + DBSchema.EmployeeLunchAddress.BookedFlag + "=0"
			// + groupBy + DBSchema.EmployeeLunchAddress.EmployeeID
			//
			// + select + DBSchema.EmployeeLunchAddress.TableName + ".*"
			// + into + "#lunch_unbooked"
			// + from + "#unbooked, " + DBSchema.EmployeeLunchAddress.TableName
			// + where + "emp_id=" + DBSchema.EmployeeLunchAddress.TableName +
			// "." + DBSchema.EmployeeLunchAddress.EmployeeID
			// + and + "update_date=" + DBSchema.EmployeeLunchAddress.TableName
			// + "." + DBSchema.EmployeeLunchAddress.UpdateDate

			+ select
			+ "MAX("
			+ DBSchema.EmployeeLunchAddress.TableName
			+ "."
			+ DBSchema.EmployeeLunchAddress.UpdateDate
			+ ") update_date,"
			+ DBSchema.EmployeeLunchAddress.EmployeeID
			+ " emp_id "
			+ into
			+ "#booked"
			+ from
			+ DBSchema.EmployeeLunchAddress.TableName
			+ where
			+ DBSchema.EmployeeLunchAddress.BookedFlag
			+ "=1"
			+ groupBy
			+ DBSchema.EmployeeLunchAddress.EmployeeID

			+ select
			+ DBSchema.EmployeeLunchAddress.TableName
			+ ".*"
			+ into
			+ "#lunch_booked"
			+ from
			+ "#booked, "
			+ DBSchema.EmployeeLunchAddress.TableName
			+ where
			+ "emp_id="
			+ DBSchema.EmployeeLunchAddress.TableName
			+ "."
			+ DBSchema.EmployeeLunchAddress.EmployeeID
			+ and
			+ "update_date="
			+ DBSchema.EmployeeLunchAddress.TableName
			+ "."
			+ DBSchema.EmployeeLunchAddress.UpdateDate

			+ update
			+ "#lunch"
			+ set
			+ DBSchema.EmployeeLunchAddress.BookedFlag
			+ "=1"
			+ from
			+ "#lunch, #lunch_booked"
			+ where
			+ "#lunch."
			+ DBSchema.EmployeeLunchAddress.EmployeeID
			+ "=#lunch_booked."
			+ DBSchema.EmployeeLunchAddress.EmployeeID
			+ and
			+ "#lunch."
			+ DBSchema.EmployeeLunchAddress.LunchAddressID
			+ "=#lunch_booked."
			+ DBSchema.EmployeeLunchAddress.LunchAddressID
			+ and
			+ "#lunch."
			+ DBSchema.EmployeeLunchAddress.BookedFlag
			+ "=0"

			+ update
			+ "#lunch"
			+ set
			+ "oldID=#lunch_booked."
			+ DBSchema.EmployeeLunchAddress.LunchAddressID
			+ from
			+ "#lunch, #lunch_booked"
			+ where
			+ "#lunch."
			+ DBSchema.EmployeeLunchAddress.EmployeeID
			+ "=#lunch_booked."
			+ DBSchema.EmployeeLunchAddress.EmployeeID

			// + update + "#lunch" + set + "oldID=#lunch_unbooked." +
			// DBSchema.EmployeeLunchAddress.LunchAddressID
			// + from + "#lunch, #lunch_unbooked"
			// + where + "#lunch." + DBSchema.EmployeeLunchAddress.EmployeeID +
			// "=#lunch_unbooked." + DBSchema.EmployeeLunchAddress.EmployeeID

			+ select
			+ "#lunch.*,"
			// + DBSchema.EmployeeLunchAddress.TableName + "." +
			// DBSchema.EmployeeLunchAddress.EmployeeID + ","
			// + DBSchema.EmployeeLunchAddress.TableName + "." +
			// DBSchema.EmployeeLunchAddress.LunchAddressID + ","
			// + "DATEDIFF(m, update_date, GETDATE()) chaged_flag, "
			+ "RTRIM("
			+ DBSchema.Employee.TableName
			+ "."
			+ DBSchema.Employee.FirstName
			+ ")+"
			+ "RTRIM("
			+ DBSchema.Employee.TableName
			+ "."
			+ DBSchema.Employee.LastName
			+ ") user_name, "
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID
			+ ","
			+ DBSchema.Department.TableName
			+ "."
			+ DBSchema.Department.DepartmentName
			+ ","
			+ DBSchema.LunchAddress.TableName
			+ "."
			+ DBSchema.LunchAddress.LunchAddressName
			+ ","
			+ "lunch."
			+ DBSchema.LunchAddress.LunchAddressName
			+ " as old_address_name "
			+ from
			+ "#lunch,"
			// + DBSchema.EmployeeLunchAddress.TableName + ","
			+ DBSchema.Employee.TableName
			+ ","
			+ DBSchema.EmployeeBelongToDept.TableName
			+ ","
			+ DBSchema.Department.TableName
			+ ","
			+ DBSchema.DeptBelongToDept.TableName
			+ ","
			+ DBSchema.LunchAddress.TableName
			+ ","
			+ DBSchema.LunchAddress.TableName
			+ " as lunch "
			+ where
			// + "emp_id = " +
			// DBSchema.EmployeeLunchAddress.TableName+"."+DBSchema.EmployeeLunchAddress.EmployeeID
			// + and + "update_date = " +
			// DBSchema.EmployeeLunchAddress.TableName+"."+DBSchema.EmployeeLunchAddress.UpdateDate
			// + and
			+ "#lunch." + DBSchema.EmployeeLunchAddress.EmployeeID + "="
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + and
			+ DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.DisabledFlag + " = 0 " + and + "#lunch."
			+ DBSchema.EmployeeLunchAddress.EmployeeID + "="
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + and
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.IsPartTime + " = 1 " + and
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + "="
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + and
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + "="
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.DeptID + and
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.ParentsID + " = ? " + and + "#lunch."
			+ DBSchema.EmployeeLunchAddress.LunchAddressID + "="
			+ DBSchema.LunchAddress.TableName + "."
			+ DBSchema.LunchAddress.LunchAddressID + and
			+ "#lunch.oldID = lunch." + DBSchema.LunchAddress.LunchAddressID;

	public static String lunchAddressBooked = select + "MAX("
			+ DBSchema.EmployeeLunchAddress.TableName + "."
			+ DBSchema.EmployeeLunchAddress.UpdateDate + ") update_date,"
			+ DBSchema.EmployeeLunchAddress.EmployeeID + " emp_id " + into
			+ "#lastLunchAdd" + from + DBSchema.EmployeeLunchAddress.TableName
			+ groupBy + DBSchema.EmployeeLunchAddress.EmployeeID

			+ update + DBSchema.EmployeeLunchAddress.TableName + set
			+ DBSchema.EmployeeLunchAddress.BookedFlag + "=1" + from
			+ DBSchema.EmployeeLunchAddress.TableName + ", #lastLunchAdd"
			+ where + DBSchema.EmployeeLunchAddress.TableName + "."
			+ DBSchema.EmployeeLunchAddress.UpdateDate + "=update_date" + and
			+ DBSchema.EmployeeLunchAddress.TableName + "."
			+ DBSchema.EmployeeLunchAddress.EmployeeID + "=emp_id";

	public static String getContractInfo = select + DBSchema.Contract.TableName
			+ ".*," + "Convert(varchar(10)," + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.WarrantyStartDate + ",1) as start_date, "
			+ "Convert(varchar(10)," + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.WarrantyEndDate + ",1) as end_date, "
			+ "Convert(varchar(10)," + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.UpdateDate + ",1) as update_date, "
			+ DBSchema.Project.TableName + "." + DBSchema.Project.ProjectID
			+ "," + DBSchema.Project.TableName + "."
			+ DBSchema.Project.ProjectCode + "," + DBSchema.Project.TableName
			+ "." + DBSchema.Project.ProjectName + "," 
			+ "contract." + DBSchema.Contract.ContractNo + " contractNo, " 
			+ "contract." + DBSchema.Contract.ContractName + " contractName,"
			+ "contract." + DBSchema.Contract.Abbreviation + " abbreviation, " 
			
			+ "aCompany."
			+ DBSchema.Company.CompanyName + " aCompany_name, " + "bCompany."
			+ DBSchema.Company.CompanyName + " bCompany_name, " + "aDept."
			+ DBSchema.Department.DepartmentName + " aDept_name, " + "bDept."
			+ DBSchema.Department.DepartmentName + " bDept_name, "
			+ "RTRIM(aEmp." + DBSchema.Employee.FirstName + ")+RTRIM(aEmp."
			+ DBSchema.Employee.LastName + ") aEmp_name, " + "RTRIM(bEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(bEmp."
			+ DBSchema.Employee.LastName + ") bEmp_name, " + "RTRIM(cEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(cEmp."
			+ DBSchema.Employee.LastName + ") cEmp_name, "
			+ DBSchema.GovernmentContractSource.TableName + "."
			+ DBSchema.GovernmentContractSource.GovermentContractSourceName + ","
//			+ "RTRIM("+DBSchema.ContactPerson.TableName + "." + DBSchema.ContactPerson.FirstName + ")+RTRIM("
//			+ DBSchema.ContactPerson.TableName + "." + DBSchema.ContactPerson.LastName + ") as " 
//			+ DBSchema.Contract.DeliveryName
			+ "RTRIM(dEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(dEmp."
			+ DBSchema.Employee.LastName + ") as "
			+ DBSchema.Contract.DeliveryName
			
			+ from + DBSchema.Contract.TableName + ","
			+ DBSchema.Project.TableName + "," + DBSchema.Contract.TableName
			+ as + "contract, " + DBSchema.Company.TableName + as
			+ "aCompany, " + DBSchema.Company.TableName + as + "bCompany, "
			+ DBSchema.Department.TableName + as + "aDept, "
			+ DBSchema.Department.TableName + as + "bDept, "
			+ DBSchema.Employee.TableName + as + "aEmp,"
			+ DBSchema.Employee.TableName + as + "bEmp,"
			+ DBSchema.Employee.TableName + as + "cEmp,"
			+ DBSchema.GovernmentContractSource.TableName + ","
			//+ DBSchema.ContactPerson.TableName
			+ DBSchema.Employee.TableName + as + "dEmp"

			+ where + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.ContractID + "=?" + and
			+ DBSchema.Project.TableName + "." + DBSchema.Project.ContractID
			+ " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.ContractID + and + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.ParentID + " *= contract."
			+ DBSchema.Contract.ContractID + and + "aCompany."
			+ DBSchema.Company.CompanyID + " =* " + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.BuyerID + and + "bCompany."
			+ DBSchema.Company.CompanyID + " =* " + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.VendorID + and + "aDept."
			+ DBSchema.Department.DepartmentID + " =* "
			+ DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.DeptIDOfBuyer + and + "bDept."
			+ DBSchema.Department.DepartmentID + " =* "
			+ DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.DeptIDOfVendor + and + "aEmp."
			+ DBSchema.Employee.ID + " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.BuyerRepresentativeID + and + "bEmp."
			+ DBSchema.Employee.ID + " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.VendorRepresentativeID + and + "cEmp."
			+ DBSchema.Employee.ID + " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.RegisterID + and
			+ DBSchema.GovernmentContractSource.TableName + "."
			+ DBSchema.GovernmentContractSource.GovermentContractSourceID
			+ " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.GovermentContractSourceID
			+ and
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.DeliveryID 
			+ "*=" + "dEmp." + DBSchema.Employee.ID
			;
	
	public static String isHadChildContractListByID = select 
			+ "count(*)"
			+ from 
			+ DBSchema.Contract.TableName
			+ where
			+ DBSchema.Contract.ParentID + "=?"
			;
	
	public static String warningString = 
//			"SELECT xmis.con_收款明细表.合同ID, SUM(xmis.con_收款明细表.收款金额) AS total_amt" +
//			"INTO #total" +
//			"FROM xmis.con_收款明细表" +
//			"GROUP BY 合同ID" +
		
			select 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID + ","
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.PaymentDate
			+ into + "#warning_date"
			+ from 
			+ DBSchema.PaymentCondition.TableName
			
			+ select 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID + ","
			+ "#warning_date." + DBSchema.PaymentCondition.PaymentDate + " AS warning_date,"
			+ "SUM(" + DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.PaymentAmt + ") as warning_amt,"
			+ "0 AS warninged_flag, null AS warning_days"
			+ into + "#warning_detail"
			+ from 
			+ DBSchema.PaymentCondition.TableName + ", #warning_date"
			+ where
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID 
			+ " = #warning_date." + DBSchema.PaymentCondition.ContractID
			+ and 
			+ "("+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.PaymentDate 
			+ " <= #warning_date." + DBSchema.PaymentCondition.PaymentDate
			
			+ or
			
			+ "#warning_date." + DBSchema.PaymentCondition.PaymentDate + " is null )"
			
			+ groupBy
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID 
			+ ",#warning_date." + DBSchema.PaymentCondition.PaymentDate
			
			+ update 
			+ "#warning_detail SET " 
			+ "warning_amt = warning_amt - " + DBSchema.ChargedDetail.TotalAmtByID 
			+ ", warninged_flag = 1"
//			+ " warninged_flag = 1"
			+ from
			+ "#charged, #warning_detail" 
			+ where 
			+ "#charged."+ DBSchema.PaymentCondition.ContractID + "= #warning_detail." + DBSchema.PaymentCondition.ContractID
			+ and 
			+ DBSchema.ChargedDetail.TotalAmtByID +">=warning_amt" 
		
//		select + "合同ID,收款日期" +
//			" INTO #warning_date" +
//			" FROM xmis.con_收款条件表" +

//			" select xmis.con_收款条件表.合同ID,#warning_date.收款日期 AS warning_date, SUM(xmis.con_收款条件表.收款金额) as warning_amt," +
//			"0 AS warninged_flag, 0 AS warning_days" +
//			" INTO #warning_detail" +
//			" FROM xmis.con_收款条件表, #warning_date" +
//			" WHERE xmis.con_收款条件表.合同ID = #warning_date.合同ID AND xmis.con_收款条件表.收款日期<= #warning_date.收款日期" +
//			" GROUP BY xmis.con_收款条件表.合同ID,#warning_date.收款日期" +
			
			
//			" UPDATE #warning_detail SET warning_amt = warning_amt - " + DBSchema.ChargedDetail.TotalAmtByID +", warninged_flag = 1" +
//			" FROM #charged, #warning_detail" +
//			" WHERE #charged.合同ID = #warning_detail.合同ID AND "+DBSchema.ChargedDetail.TotalAmtByID +">=warning_amt" +
			
			+ "\nUPDATE #warning_detail SET warning_days = " +
			" \nCASE" +
			"	\nWHEN DATEDIFF(d,GETDATE(),warning_date)<-30 THEN -30" +
			"	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>=-30 AND DATEDIFF(d,GETDATE(),warning_date)<-15 THEN -15" +
			"	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>=-15 AND DATEDIFF(d,GETDATE(),warning_date)<-7 THEN -7" +
			"	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>=-7 AND DATEDIFF(d,GETDATE(),warning_date)<0 THEN -1" +
			"	\nWHEN DATEDIFF(d,GETDATE(),warning_date)=0 THEN 1" +
			"	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>0 AND DATEDIFF(d,GETDATE(),warning_date)<=7 THEN 7" +
			"	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>7 AND DATEDIFF(d,GETDATE(),warning_date)<=15 THEN 15" +
			"	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>15 AND DATEDIFF(d,GETDATE(),warning_date)<=30 THEN 30" +
			"	\nWHEN warning_date is null then NULL" + 
			"	\nELSE DATEDIFF(d,GETDATE(),warning_date)" +
			"\nEND	" +
			"WHERE warninged_flag = 0" +
			 			
			"			SELECT "+DBSchema.PaymentCondition.ContractID
			+ ", MAX(warning_date) as warning_date, MAX(warning_amt) as warning_amt, MAX(warning_days) as warning_days" +
			"			INTO #warning" +
			"			FROM #warning_detail" +
			"			WHERE warning_days <= 30 AND warninged_flag = 0" +
			"			GROUP BY " + DBSchema.PaymentCondition.ContractID
			
			
			//warning info
			+ select 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID
			+ ", min("+DBSchema.PaymentCondition.TableName+"."+DBSchema.PaymentCondition.PaymentDate+") as warning_date,"
			+ "0 AS warning_amt, 31 AS warning_days "
			+ into
			+ "#warning_info"
			+ from 
			+ DBSchema.PaymentCondition.TableName + ", #warning"
			+ where 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID 
			+ " NOT IN (select " +  DBSchema.PaymentCondition.ContractID + " FROM #warning )"
			+ and 
			+ DBSchema.PaymentCondition.PaymentDate + ">=GETDATE() "
			+ groupBy 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID
			
			+ " UPDATE #warning_info SET warning_amt = " 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.PaymentAmt
			+ from
			+ DBSchema.PaymentCondition.TableName + ", #warning_info "
			+ where
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID 
			+ " = #warning_info." + DBSchema.PaymentCondition.ContractID
			+ and 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.PaymentDate
			+ " = #warning_info.warning_date "
			+ " INSERT INTO #warning SELECT * FROM #warning_info "
			
//			+ " SELECT xmis.con_收款条件表.合同ID, min(收款日期) AS warning_date, 0 AS warning_amt, 31 AS warning_days "
//			+ " INTO #warning_info "
//			+ "	FROM xmis.con_收款条件表, #warning "
//			+ "	WHERE xmis.con_收款条件表.合同ID NOT IN (SELECT 合同ID FROM #warning)  AND 收款日期>=GETDATE() "
//			+ "	GROUP BY xmis.con_收款条件表.合同ID " 

//			+ " UPDATE #warning_info SET warning_amt = xmis.con_收款条件表.收款金额  FROM xmis.con_收款条件表, #warning_info "
//			+ " WHERE xmis.con_收款条件表.合同ID = #warning_info.合同ID AND xmis.con_收款条件表.收款日期 = #warning_info.warning_date "
//			+ " INSERT INTO #warning SELECT * FROM #warning_info "
			
			;
		
	private static String getSumInfoByContractID = 			
			//get total_amt by contractID
			select 
			+ DBSchema.ChargedDetail.ContractID + ","
			+ "SUM(" + DBSchema.ChargedDetail.ChargedAmt + ") as " + DBSchema.ChargedDetail.TotalAmtByID
			+ into + "#charged"
			+ from + DBSchema.ChargedDetail.TableName
			+ groupBy + DBSchema.ChargedDetail.ContractID
			
			//get invoice_amt by contractID
			+ select 
			+ DBSchema.InvoiceDetail.ContractID + ","
			+ "SUM(" + DBSchema.InvoiceDetail.InvoiceAmt + ") as " + DBSchema.InvoiceDetail.TotalAmtByID
			+ into + "#invoice"
			+ from + DBSchema.InvoiceDetail.TableName
			+ groupBy + DBSchema.InvoiceDetail.ContractID
			
			+ warningString
			
			+ select 
			+ "#contractList.*, "
			
			+ "\nCASE WHEN #charged." + DBSchema.ChargedDetail.TotalAmtByID + " is NULL THEN 0 "
			+ "ELSE #charged." + DBSchema.ChargedDetail.TotalAmtByID + " END AS " 
			+ DBSchema.ChargedDetail.TotalAmtByID + ","
			
			+ "\n#contractList." + DBSchema.Contract.ContractAmt + "-" 
			+ "CASE WHEN #charged." + DBSchema.ChargedDetail.TotalAmtByID + " is NULL THEN 0 "
			+ "ELSE #charged." + DBSchema.ChargedDetail.TotalAmtByID + " END As " 
			+ DBSchema.ChargedDetail.UnChargedAmtTotal + ","
			
			
			+ "\nCASE WHEN #invoice." + DBSchema.InvoiceDetail.TotalAmtByID + " is NULL THEN 0 "
			+ "ELSE #invoice." + DBSchema.InvoiceDetail.TotalAmtByID + " END AS "
			+ DBSchema.InvoiceDetail.TotalAmtByID + ","
			
			
//			+ "\nCASE WHEN #invoice." + DBSchema.InvoiceDetail.TotalAmtByID + " is NULL THEN 0 "
//			+ "ELSE #invoice." + DBSchema.InvoiceDetail.TotalAmtByID + " END - " 
//			+ "CASE WHEN #charged." + DBSchema.ChargedDetail.TotalAmtByID + " is NULL THEN 0 "
//			+ "ELSE #charged." + DBSchema.ChargedDetail.TotalAmtByID + " END AS " 
//			+ DBSchema.InvoiceDetail.UnAccountAmtTotalSum + ", "
			

			+ "\nCASE WHEN #invoice." + DBSchema.InvoiceDetail.TotalAmtByID + " is NULL THEN 0 "
			+ "WHEN #invoice." + DBSchema.InvoiceDetail.TotalAmtByID + " = 0 THEN 0 " 
			+ "ELSE #invoice." + DBSchema.InvoiceDetail.TotalAmtByID + " - "
			+ "CASE	WHEN #charged." + DBSchema.ChargedDetail.TotalAmtByID + " is NULL THEN 0 "
			+ "ELSE #charged." + DBSchema.ChargedDetail.TotalAmtByID + " END " 
			+ "END AS " + DBSchema.InvoiceDetail.UnAccountAmtTotalSum + ", "	
			
			+ "\n#warning.warning_date, "
			+ "#warning.warning_amt - " 
			+ "CASE WHEN #charged." + DBSchema.ChargedDetail.TotalAmtByID + " is NULL THEN 0 "
			+ "ELSE #charged." + DBSchema.ChargedDetail.TotalAmtByID + " END As warning_amt" 
			+ ","			
			
			+
					"\ncase when #warning.warning_days is null then -31 else #warning.warning_days end as  warning_days"
			
			+ into + "#result"
			+ from + "#contractList, #charged, #invoice, #warning"
			+ where 
			+ "#contractList." + DBSchema.Contract.ContractID 
			+ "*=" + "#charged." + DBSchema.ChargedDetail.ContractID
			+ and 
			+ "#contractList." + DBSchema.Contract.ContractID 
			+ "*=" + "#invoice." + DBSchema.InvoiceDetail.ContractID
			+ and
			+ "#contractList." + DBSchema.Contract.ContractID 
			+ "*= " + "#warning." + DBSchema.ChargedDetail.ContractID
			;		

	public static String getChildContractListByID = select 
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ContractID + "," 
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ContractNo + ","
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ContractName + ","
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.SignatureDate + ","
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.Abbreviation + ","
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ContractAmt + ","
			//+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ContractAmt + "*"
			+ DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.ExchangeRate 
			//+ " as " + DBSchema.CurrencyType.ExchangedAmt 
			+ ","
			+ DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.CurrenyGraph
			+ into 
			+ "#contractList"
			+ from 
			+ DBSchema.Contract.TableName + ","
			+ DBSchema.CurrencyType.TableName
			+ where
			+ DBSchema.Contract.ParentID 
			+ "=?"
			+ and 
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID 
			+ "*=" + DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.CurrencyID
			
			+ getSumInfoByContractID	
			;
	
	


	public static String getContractType = select + "*" + from
			+ DBSchema.ContractType.TableName;

	public static String getContractSourceType = select + "*" + from
			+ DBSchema.ContractSourceType.TableName;

	public static String getCurrencyType = select + "*" + from
			+ DBSchema.CurrencyType.TableName 
			+ where 
			+ DBSchema.CurrencyType.CurrencyID + ">0"
			;

	public static String getCompanyList = select + DBSchema.Company.TableName
			+ ".*, " + DBSchema.Address.TabelName + "."
			+ DBSchema.Address.District + "," + DBSchema.Address.TabelName
			+ "." + DBSchema.Address.ZipCode + "," + DBSchema.Address.TabelName
			+ "." + DBSchema.Address.AddressInfo + ","
			+ DBSchema.Address.TabelName + "." + DBSchema.Address.CityID + ","
			+ DBSchema.BusinessRelation.TableName + "."
			+ DBSchema.BusinessRelation.BusinessRelationName + ","
			+ DBSchema.BusinessRelation.TableName + "."
			+ DBSchema.BusinessRelation.BusinessRelationDefinition + ","
			+ DBSchema.CompanyScale.TableName + "."
			+ DBSchema.CompanyScale.CompanyScaleName + " as scale_name, "
			+ DBSchema.CompanyScale.TableName + "."
			+ DBSchema.CompanyScale.CompanyScaleDefinition + " as scale_def, "
			+ "RTRIM(" + DBSchema.ContactPerson.TableName + "."
			+ DBSchema.ContactPerson.FirstName + ")+RTRIM("
			+ DBSchema.ContactPerson.TableName + "."
			+ DBSchema.ContactPerson.LastName + ") as chief_person, "
			+ "RTRIM(contact." + DBSchema.ContactPerson.FirstName
			+ ")+RTRIM(contact." + DBSchema.ContactPerson.LastName
			+ ") as contact_person," + DBSchema.CustomerLevel.TableName + "."
			+ DBSchema.CustomerLevel.CustomerLevelName
			+ " as customer_level_name, " + DBSchema.CustomerLevel.TableName
			+ "." + DBSchema.CustomerLevel.CustomerLevelDefinition
			+ " as customer_level_def, " + DBSchema.CreditLevel.TableName + "."
			+ DBSchema.CreditLevel.CreditLevelName + " as credit_level_name, "
			+ DBSchema.CreditLevel.TableName + "."
			+ DBSchema.CreditLevel.CreditLevelDefinition
			+ " as credit_level_def, " + "RTRIM(" + DBSchema.Employee.TableName
			+ "." + DBSchema.Employee.FirstName + ")+RTRIM("
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.LastName
			+ ") as update_name " + into + "#company_add" + from
			+ DBSchema.Company.TableName + "," + DBSchema.Address.TabelName
			+ "," + DBSchema.BusinessRelation.TableName + ","
			+ DBSchema.CompanyScale.TableName + ","
			+ DBSchema.ContactPerson.TableName + ","
			+ DBSchema.ContactPerson.TableName + " as contact,"
			+ DBSchema.CustomerLevel.TableName + ","
			+ DBSchema.CreditLevel.TableName + ","
			+ DBSchema.Employee.TableName + where + DBSchema.Company.TableName
			+ "." + DBSchema.Company.AddressID + "*="
			+ DBSchema.Address.TabelName + "." + DBSchema.Address.AddressID
			+ and + DBSchema.Company.TableName + "."
			+ DBSchema.Company.BusinessRelationID + "*="
			+ DBSchema.BusinessRelation.TableName + "."
			+ DBSchema.BusinessRelation.BusinessRelationID + and
			+ DBSchema.Company.TableName + "."
			+ DBSchema.Company.CompanyScaleID + "*="
			+ DBSchema.CompanyScale.TableName + "."
			+ DBSchema.CompanyScale.CompanyScaleID + and
			+ DBSchema.Company.TableName + "."
			+ DBSchema.Company.LegalRepresentativeID + "*="
			+ DBSchema.ContactPerson.TableName + "."
			+ DBSchema.ContactPerson.ContactPersonID + and
			+ DBSchema.Company.TableName + "."
			+ DBSchema.Company.ContactPersonID + "*= contact."
			+ DBSchema.ContactPerson.ContactPersonID + and
			+ DBSchema.Company.TableName + "."
			+ DBSchema.Company.CustomerLevelID + "*="
			+ DBSchema.CustomerLevel.TableName + "."
			+ DBSchema.CustomerLevel.CustomerLevelID + and
			+ DBSchema.Company.TableName + "." + DBSchema.Company.CreditLevelID
			+ "*=" + DBSchema.CreditLevel.TableName + "."
			+ DBSchema.CreditLevel.CreditLevelID + and
			+ DBSchema.Company.TableName + "." + DBSchema.Company.UpdateID
			+ "*=" + DBSchema.Employee.TableName + "." + DBSchema.Employee.ID

			+ select + "#company_add.*," + DBSchema.City.TableName + "."
			+ DBSchema.City.CityName + "," + DBSchema.City.TableName + "."
			+ DBSchema.City.AreaCode + "," + DBSchema.City.TableName + "."
			+ DBSchema.City.ProvinceID + into + "#company_city" + from
			+ "#company_add, " + DBSchema.City.TableName + where
			+ "#company_add." + DBSchema.Address.CityID + "*="
			+ DBSchema.City.TableName + "." + DBSchema.City.CityID

			+ select + "#company_city.*," + DBSchema.Province.TableName + "."
			+ DBSchema.Province.ProvinceName + from + "#company_city,"
			+ DBSchema.Province.TableName + where + "#company_city."
			+ DBSchema.City.ProvinceID + "*=" + DBSchema.Province.TableName
			+ "." + DBSchema.Province.ProvinceID;

	public static String getCompanyListWithoutSelf = select
			+ DBSchema.Company.TableName + ".*, " + DBSchema.Address.TabelName
			+ "." + DBSchema.Address.District + ","
			+ DBSchema.Address.TabelName + "." + DBSchema.Address.ZipCode + ","
			+ DBSchema.Address.TabelName + "." + DBSchema.Address.AddressInfo
			+ "," + DBSchema.Address.TabelName + "." + DBSchema.Address.CityID
			+ "," + DBSchema.BusinessRelation.TableName + "."
			+ DBSchema.BusinessRelation.BusinessRelationName + ","
			+ DBSchema.BusinessRelation.TableName + "."
			+ DBSchema.BusinessRelation.BusinessRelationDefinition + ","
			+ DBSchema.CompanyScale.TableName + "."
			+ DBSchema.CompanyScale.CompanyScaleName + " as scale_name, "
			+ DBSchema.CompanyScale.TableName + "."
			+ DBSchema.CompanyScale.CompanyScaleDefinition + " as scale_def, "
			+ "RTRIM(" + DBSchema.ContactPerson.TableName + "."
			+ DBSchema.ContactPerson.FirstName + ")+RTRIM("
			+ DBSchema.ContactPerson.TableName + "."
			+ DBSchema.ContactPerson.LastName + ") as chief_person, "
			+ "RTRIM(contact." + DBSchema.ContactPerson.FirstName
			+ ")+RTRIM(contact." + DBSchema.ContactPerson.LastName
			+ ") as contact_person," + DBSchema.CustomerLevel.TableName + "."
			+ DBSchema.CustomerLevel.CustomerLevelName
			+ " as customer_level_name, " + DBSchema.CustomerLevel.TableName
			+ "." + DBSchema.CustomerLevel.CustomerLevelDefinition
			+ " as customer_level_def, " + DBSchema.CreditLevel.TableName + "."
			+ DBSchema.CreditLevel.CreditLevelName + " as credit_level_name, "
			+ DBSchema.CreditLevel.TableName + "."
			+ DBSchema.CreditLevel.CreditLevelDefinition
			+ " as credit_level_def, " + "RTRIM(" + DBSchema.Employee.TableName
			+ "." + DBSchema.Employee.FirstName + ")+RTRIM("
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.LastName
			+ ") as update_name " + into + "#company_add" + from
			+ DBSchema.Company.TableName + "," + DBSchema.Address.TabelName
			+ "," + DBSchema.BusinessRelation.TableName + ","
			+ DBSchema.CompanyScale.TableName + ","
			+ DBSchema.ContactPerson.TableName + ","
			+ DBSchema.ContactPerson.TableName + " as contact,"
			+ DBSchema.CustomerLevel.TableName + ","
			+ DBSchema.CreditLevel.TableName + ","
			+ DBSchema.Employee.TableName + where + DBSchema.Company.TableName
			+ "." + DBSchema.Company.CompanyID + "<>?" + and
			+ DBSchema.Company.TableName + "." + DBSchema.Company.AddressID
			+ "*=" + DBSchema.Address.TabelName + "."
			+ DBSchema.Address.AddressID + and + DBSchema.Company.TableName
			+ "." + DBSchema.Company.BusinessRelationID + "*="
			+ DBSchema.BusinessRelation.TableName + "."
			+ DBSchema.BusinessRelation.BusinessRelationID + and
			+ DBSchema.Company.TableName + "."
			+ DBSchema.Company.CompanyScaleID + "*="
			+ DBSchema.CompanyScale.TableName + "."
			+ DBSchema.CompanyScale.CompanyScaleID + and
			+ DBSchema.Company.TableName + "."
			+ DBSchema.Company.LegalRepresentativeID + "*="
			+ DBSchema.ContactPerson.TableName + "."
			+ DBSchema.ContactPerson.ContactPersonID + and
			+ DBSchema.Company.TableName + "."
			+ DBSchema.Company.ContactPersonID + "*= contact."
			+ DBSchema.ContactPerson.ContactPersonID + and
			+ DBSchema.Company.TableName + "."
			+ DBSchema.Company.CustomerLevelID + "*="
			+ DBSchema.CustomerLevel.TableName + "."
			+ DBSchema.CustomerLevel.CustomerLevelID + and
			+ DBSchema.Company.TableName + "." + DBSchema.Company.CreditLevelID
			+ "*=" + DBSchema.CreditLevel.TableName + "."
			+ DBSchema.CreditLevel.CreditLevelID + and
			+ DBSchema.Company.TableName + "." + DBSchema.Company.UpdateID
			+ "*=" + DBSchema.Employee.TableName + "." + DBSchema.Employee.ID

			+ select + "#company_add.*," + DBSchema.City.TableName + "."
			+ DBSchema.City.CityName + "," + DBSchema.City.TableName + "."
			+ DBSchema.City.AreaCode + "," + DBSchema.City.TableName + "."
			+ DBSchema.City.ProvinceID + into + "#company_city" + from
			+ "#company_add, " + DBSchema.City.TableName + where
			+ "#company_add." + DBSchema.Address.CityID + "*="
			+ DBSchema.City.TableName + "." + DBSchema.City.CityID

			+ select + "#company_city.*," + DBSchema.Province.TableName + "."
			+ DBSchema.Province.ProvinceName 
			+ intoResult
			+ from + "#company_city,"
			+ DBSchema.Province.TableName + where + "#company_city."
			+ DBSchema.City.ProvinceID + "*=" + DBSchema.Province.TableName
			+ "." + DBSchema.Province.ProvinceID
			
			;

	public static String getGovernmentContractSourceList = select
			+ DBSchema.GovernmentContractSource.TableName + ".*,"
			+ DBSchema.Company.TableName + "." + DBSchema.Company.CompanyName
			+ "," + DBSchema.Company.TableName + "."
			+ DBSchema.Company.Abbreviation + " as company,"
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentName + ","
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.Abbreviation + " as dept,"
			+ DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrencyName 
			+ intoResult
			+ from
			+ DBSchema.GovernmentContractSource.TableName + ","
			+ DBSchema.Company.TableName + "," + DBSchema.Department.TableName
			+ "," + DBSchema.CurrencyType.TableName + where
			+ DBSchema.GovernmentContractSource.TableName + "."
			+ DBSchema.GovernmentContractSource.Actived + "=0" + and
			+ DBSchema.GovernmentContractSource.TableName + "."
			+ DBSchema.GovernmentContractSource.CompanyID + "*="
			+ DBSchema.Company.TableName + "." + DBSchema.Company.CompanyID
			+ and + DBSchema.GovernmentContractSource.TableName + "."
			+ DBSchema.GovernmentContractSource.DepartmentID + "*="
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + and
			+ DBSchema.GovernmentContractSource.TableName + "."
			+ DBSchema.GovernmentContractSource.CurrencyTypeID + "*="
			+ DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrencyID;

	public static String intsertGContractSource = insert + into
			+ DBSchema.GovernmentContractSource.TableName + "("
			+ DBSchema.GovernmentContractSource.GovermentContractSourceID + ","
			+ DBSchema.GovernmentContractSource.GovermentContractSourceName
			+ "," + DBSchema.GovernmentContractSource.CompanyID + ","
			+ DBSchema.GovernmentContractSource.DepartmentID + ","
			+ DBSchema.GovernmentContractSource.FundingLimit + ","
			+ DBSchema.GovernmentContractSource.CurrencyTypeID + ","
			+ DBSchema.GovernmentContractSource.Specialty + ","
			+ DBSchema.GovernmentContractSource.UpdateDate + ","
			+ DBSchema.GovernmentContractSource.UpdateID + ","
			+ DBSchema.GovernmentContractSource.Actived + ")" + values
			+ "(?,?,?,?,?,?,?,getdate(),?,0)";

	public static String updateGContractSource = update
			+ DBSchema.GovernmentContractSource.TableName + set
			+ DBSchema.GovernmentContractSource.GovermentContractSourceName
			+ "=?," + DBSchema.GovernmentContractSource.CompanyID + "=?,"
			+ DBSchema.GovernmentContractSource.DepartmentID + "=?,"
			+ DBSchema.GovernmentContractSource.FundingLimit + "=?,"
			+ DBSchema.GovernmentContractSource.CurrencyTypeID + "=?,"
			+ DBSchema.GovernmentContractSource.Specialty + "=?,"
			+ DBSchema.GovernmentContractSource.UpdateDate + "=getdate(),"
			+ DBSchema.GovernmentContractSource.UpdateID + "=?" + where
			+ DBSchema.GovernmentContractSource.GovermentContractSourceID
			+ "=?";

	public static String setSTonContract = update + DBSchema.Contract.TableName
			+ set + DBSchema.Contract.GovermentContractSourceID + "=?,"
			+ DBSchema.Contract.RegisterID + "=?,"
			+ DBSchema.Contract.UpdateDate + "=getdate()" + where
			+ DBSchema.Contract.ContractID + "=?";

	public static String existsContractByGST = ifExists + "(" + select + "*"
			+ from + DBSchema.Contract.TableName + where
			+ DBSchema.Contract.GovermentContractSourceID + " =?)" + select
			+ "1" + elseExists + select + "2";

	public static String deleteGContractSource = update
			+ DBSchema.GovernmentContractSource.TableName + set
			+ DBSchema.GovernmentContractSource.Actived + "=1,"
			+ DBSchema.GovernmentContractSource.UpdateDate + "=getdate(),"
			+ DBSchema.GovernmentContractSource.UpdateID + "=?" + where
			+ DBSchema.GovernmentContractSource.GovermentContractSourceID
			+ "=?";

	public static String selectSubStrOnContractList = select
			+ DBSchema.Contract.TableName + ".*";

	public static String fromSubStrOnContractList = into + "#result" + from
			+ DBSchema.Contract.TableName;

	public static String whereSubStrOnContractListForParentID = where
			+ DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.ContractTypeID + "=?";


	public static String getContractListByType = select
			+ DBSchema.Contract.TableName + ".*," + "Convert(varchar(10),"
			+ DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.WarrantyStartDate + ",1) as start_date, "
			+ "Convert(varchar(10)," + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.WarrantyEndDate + ",1) as end_date, "
			+ "Convert(varchar(10)," + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.UpdateDate + ",1) as update_date, "
			+ DBSchema.Project.TableName + "." + DBSchema.Project.ProjectID
			+ "," + DBSchema.Project.TableName + "."
			+ DBSchema.Project.ProjectCode + "," + DBSchema.Project.TableName
			+ "." + DBSchema.Project.ProjectName + "," + "contract."
			+ DBSchema.Contract.ContractNo + " contractNo, " + "contract."
			+ DBSchema.Contract.Abbreviation + " abbreviation, " + "aCompany."
			+ DBSchema.Company.CompanyName + " aCompany_name, " + "aCompany."
			+ DBSchema.Company.Abbreviation + " aCompany_abb," + "bCompany."
			+ DBSchema.Company.CompanyName + " bCompany_name, " + "aDept."
			+ DBSchema.Department.DepartmentName + " aDept_name, " + "bDept."
			+ DBSchema.Department.DepartmentName + " bDept_name, "
			+ "RTRIM(aEmp." + DBSchema.Employee.FirstName + ")+RTRIM(aEmp."
			+ DBSchema.Employee.LastName + ") aEmp_name, " + "RTRIM(bEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(bEmp."
			+ DBSchema.Employee.LastName + ") bEmp_name, " + "RTRIM(cEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(cEmp."
			+ DBSchema.Employee.LastName + ") cEmp_name, "
			+ DBSchema.GovernmentContractSource.TableName + "."
			+ DBSchema.GovernmentContractSource.GovermentContractSourceName + " as " +DBSchema.Contract.GovermentContractSourceName + ","
//			+ DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.CurrenyGraph
			
			+ "CASE	WHEN " 
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID + " IS NULL THEN '￥'"
			+ " WHEN " 
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID + " = -1 THEN '￥'"
			+ " ELSE " + DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.CurrenyGraph
			+ " END as "
			+ DBSchema.CurrencyType.CurrenyGraph
			//+ into + "#result"
			
			+ into + "#contractList"
			
			
			+ from + DBSchema.Contract.TableName + ","
			+ DBSchema.Project.TableName + "," + DBSchema.Contract.TableName
			+ as + "contract, " + DBSchema.Company.TableName + as
			+ "aCompany, " + DBSchema.Company.TableName + as + "bCompany, "
			+ DBSchema.Department.TableName + as + "aDept, "
			+ DBSchema.Department.TableName + as + "bDept, "
			+ DBSchema.Employee.TableName + as + "aEmp,"
			+ DBSchema.Employee.TableName + as + "bEmp,"
			+ DBSchema.Employee.TableName + as + "cEmp,"
			+ DBSchema.GovernmentContractSource.TableName + ","
			+ DBSchema.CurrencyType.TableName

			+ where + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.ContractTypeID + "=?" + and
			+ DBSchema.Project.TableName + "." + DBSchema.Project.ContractID
			+ " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.ContractID + and + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.ParentID + " *= contract."
			+ DBSchema.Contract.ContractID + and + "aCompany."
			+ DBSchema.Company.CompanyID + " =* " + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.BuyerID + and + "bCompany."
			+ DBSchema.Company.CompanyID + " =* " + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.VendorID + and + "aDept."
			+ DBSchema.Department.DepartmentID + " =* "
			+ DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.DeptIDOfBuyer + and + "bDept."
			+ DBSchema.Department.DepartmentID + " =* "
			+ DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.DeptIDOfVendor + and + "aEmp."
			+ DBSchema.Employee.ID + " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.BuyerRepresentativeID + and + "bEmp."
			+ DBSchema.Employee.ID + " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.VendorRepresentativeID + and + "cEmp."
			+ DBSchema.Employee.ID + " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.RegisterID + and
			+ DBSchema.GovernmentContractSource.TableName + "."
			+ DBSchema.GovernmentContractSource.GovermentContractSourceID
			+ " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.GovermentContractSourceID
			+ and 
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID 
			+ "*=" + DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.CurrencyID
			
			+ getSumInfoByContractID
			;
	
	public static String getPaymentListByID = select 
			+ DBSchema.ChargedDetail.TableName + ".*,"
			
			+ "Convert(varchar(10)," + DBSchema.ChargedDetail.TableName
			+ "." + DBSchema.ChargedDetail.ChargedDate + ",101) as " 
			+ DBSchema.ChargedDetail.ChargedDateDisplayed +", "

			+ "Convert(varchar(10)," + DBSchema.ChargedDetail.TableName
			+ "." + DBSchema.ChargedDetail.UpdateDate + ",101) as " 
			+ DBSchema.ChargedDetail.UpdateDateDisplay +", "

			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID + ","
			+ DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.CurrenyGraph + ","
			+ DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.ExchangeRate + ","
 			+ "RTRIM(aEmp." + DBSchema.Employee.FirstName + ")+RTRIM(aEmp."	+ DBSchema.Employee.LastName + ") aEmp_name, "
			+ "RTRIM(bEmp." + DBSchema.Employee.FirstName + ")+RTRIM(bEmp."	+ DBSchema.Employee.LastName + ") bEmp_name "
			+ intoResult
			+ from 
			+ DBSchema.ChargedDetail.TableName + ","
			+ DBSchema.Contract.TableName + ","
			+ DBSchema.CurrencyType.TableName + ","
			+ DBSchema.Employee.TableName + " as aEmp,"
			+ DBSchema.Employee.TableName + " as bEmp "
			+ where
			+ DBSchema.ChargedDetail.TableName + "." + DBSchema.ChargedDetail.ContractID + "=?"
			+ and 
			+ DBSchema.ChargedDetail.TableName + "." + DBSchema.ChargedDetail.ContractID 
			+ "=" + DBSchema.Contract.TableName + "." + DBSchema.Contract.ContractID
			+ and 
			+ DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.CurrencyID 
			+ "=" + DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID
			+ and 
			+ DBSchema.ChargedDetail.TableName + "." + DBSchema.ChargedDetail.TransactorID 
			+ "*=" + "aEmp." + DBSchema.Employee.ID
			+ and 
			+ DBSchema.ChargedDetail.TableName + "." + DBSchema.ChargedDetail.UpdateID
			+ "*=" + "bEmp." + DBSchema.Employee.ID
			;
	
	public static String getTotalPaymentByID = select 
			+ "SUM(" + DBSchema.ChargedDetail.ChargedAmt + ") as " + DBSchema.ChargedDetail.TotalAmtByID
			+ from + DBSchema.ChargedDetail.TableName
			+ where 
			+ DBSchema.ChargedDetail.ContractID + "=?"
			;
	
	public static String getTotalInvoicedByID = select 
			+ "SUM(" + DBSchema.InvoiceDetail.InvoiceAmt + ") as " + DBSchema.InvoiceDetail.TotalAmtByID
			+ from + DBSchema.InvoiceDetail.TableName
			+ where 
			+ DBSchema.InvoiceDetail.ContractID + "=?"
			;
	
	public static String getInvoiceListByID = select
			+ DBSchema.InvoiceDetail.TableName + ".*,"
			
			+ "Convert(varchar(10)," + DBSchema.InvoiceDetail.TableName
			+ "." + DBSchema.InvoiceDetail.InvoiceDate + ",101) as " 
			+ DBSchema.InvoiceDetail.InvoiceDateDisplayed +", "

			+ "Convert(varchar(10)," + DBSchema.InvoiceDetail.TableName
			+ "." + DBSchema.InvoiceDetail.UpdateDate + ",101) as " 
			+ DBSchema.InvoiceDetail.UpdateDateDisplayed +", "
			
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID
			+ "," + DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrenyGraph + ","
			+ DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.ExchangeRate + "," + "RTRIM(aEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(aEmp."
			+ DBSchema.Employee.LastName + ") aEmp_name, " + "RTRIM(bEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(bEmp."
			+ DBSchema.Employee.LastName + ") bEmp_name " 
			+ intoResult 
			+ from
			+ DBSchema.InvoiceDetail.TableName + ","
			+ DBSchema.Contract.TableName + ","
			+ DBSchema.CurrencyType.TableName + ","
			+ DBSchema.Employee.TableName + " as aEmp,"
			+ DBSchema.Employee.TableName + " as bEmp " 
			+ where
			+ DBSchema.InvoiceDetail.TableName + "." + DBSchema.InvoiceDetail.ContractID + "=?" 
			+ and
			+ DBSchema.InvoiceDetail.TableName + "." + DBSchema.InvoiceDetail.ContractID 
			+ "=" + DBSchema.Contract.TableName + "." + DBSchema.Contract.ContractID
			+ and + DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrencyID + "="
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID
			+ and + DBSchema.InvoiceDetail.TableName + "."
			+ DBSchema.InvoiceDetail.TransactorID + "*=" + "aEmp."
			+ DBSchema.Employee.ID + and + DBSchema.InvoiceDetail.TableName
			+ "." + DBSchema.InvoiceDetail.UpdateID + "*=" + "bEmp."
			+ DBSchema.Employee.ID;	
	
	public static String getContractListByTypeForSearch = getContractListByType.replace("#result", "#contract")
			+ select 
			+ "*"
			+ into + "#result"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.ContractNo + like + "?"
			+ union
			+ select 
			+ "*"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.Abbreviation + like + "?"
			+ union
			+ select 
			+ "*"
			+ from + "#contract"
			+ where 
			+ DBSchema.Project.ProjectCode + like + "?"
			;
	
	public static String getContractListByTypeForCurrency = getContractListByType.replace("#result", "#contract")
			+ select 
			+ "*"
			+ into + "#result"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.ContractNo + like + "?"
			+ and 
			+ DBSchema.CurrencyType.CurrenyGraph  + "= ?"
			+ union
			+ select 
			+ "*"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.Abbreviation + like + "?"
			+ and 
			+ DBSchema.CurrencyType.CurrenyGraph  + "= ?"
			+ union
			+ select 
			+ "*"
			+ from + "#contract"
			+ where 
			+ DBSchema.Project.ProjectCode + like + "?"
			+ and 
			+ DBSchema.CurrencyType.CurrenyGraph  + "= ?"
			;	
	
	public static String getSumInfoByIntegrationSimpleType = getContractListByType.replace("#result", "#contract")
			+ select 
//			+ DBSchema.CurrencyType.CurrenyGraph 
//			+ ", sum(" + DBSchema.Contract.ContractAmt + ") as " + DBSchema.Contract.ContractAmt 
//			+ ", count(*) as " + DBSchema.Contract.ContractAmtCount 
//			
//			+ ", sum(" + DBSchema.ChargedDetail.TotalAmtByID + ") as " + DBSchema.ChargedDetail.TotalAmtByID
//			+ ", sum(" + DBSchema.ChargedDetail.UnChargedAmtTotal + ") as " + DBSchema.ChargedDetail.UnChargedAmtTotal
//			+ ", sum(" + DBSchema.InvoiceDetail.TotalAmtByID + ") as " + DBSchema.InvoiceDetail.TotalAmtByID
//			+ ", sum(" + DBSchema.InvoiceDetail.UnAccountAmtTotalSum + ") as " + DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			+ "*"
			+ into + "#sum_result"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.ContractNo + like + "?"
//			+ groupBy 
//			+ DBSchema.CurrencyType.CurrenyGraph 
			+ union
			+ select 
//			+ DBSchema.CurrencyType.CurrenyGraph 
//			+ ", sum(" + DBSchema.Contract.ContractAmt + ") as " + DBSchema.Contract.ContractAmt 
//			+ ", count(*) as " + DBSchema.Contract.ContractAmtCount 
//			
//			+ ", sum(" + DBSchema.ChargedDetail.TotalAmtByID + ") as " + DBSchema.ChargedDetail.TotalAmtByID
//			+ ", sum(" + DBSchema.ChargedDetail.UnChargedAmtTotal + ") as " + DBSchema.ChargedDetail.UnChargedAmtTotal
//			+ ", sum(" + DBSchema.InvoiceDetail.TotalAmtByID + ") as " + DBSchema.InvoiceDetail.TotalAmtByID
//			+ ", sum(" + DBSchema.InvoiceDetail.UnAccountAmtTotalSum + ") as " + DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			+ "*"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.Abbreviation + like + "?"
//			+ groupBy 
//			+ DBSchema.CurrencyType.CurrenyGraph 
			+ union
			+ select 
			+ "*"
			+ from + "#contract"
			+ where 
			+ DBSchema.Project.ProjectCode + like + "?"

			+ select 
			+ DBSchema.CurrencyType.CurrenyGraph 
			+ ", sum(" + DBSchema.Contract.ContractAmt + ") as " + DBSchema.Contract.ContractAmt 
			+ ", count(" + DBSchema.Contract.ContractID + ") as " + DBSchema.Contract.ContractAmtCount 
			
			+ ", sum(" + DBSchema.ChargedDetail.TotalAmtByID + ") as " + DBSchema.ChargedDetail.TotalAmtByID
			+ ", sum(" + DBSchema.ChargedDetail.UnChargedAmtTotal + ") as " + DBSchema.ChargedDetail.UnChargedAmtTotal
			+ ", sum(" + DBSchema.InvoiceDetail.TotalAmtByID + ") as " + DBSchema.InvoiceDetail.TotalAmtByID
			+ ", sum(" + DBSchema.InvoiceDetail.UnAccountAmtTotalSum + ") as " + DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			+ into + "#result"
			+ from + "#sum_result"
			+ groupBy 
			+ DBSchema.CurrencyType.CurrenyGraph 
			;
	
	public static String getWarningTotalCountForIntegrationSimpleType = getContractListByType.replace("#result", "#contract")
			+ select 
			+ "*" 
			+ into + "#count_result"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.ContractNo + like + "?"
			+ union
			+ select
			+ "*"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.Abbreviation + like + "?"
			+ union
			+ select
			+ "*"
			+ from + "#contract"
			+ where 
			+ DBSchema.Project.ProjectCode + like + "?"
			+ select 
			+ "warning_days, COUNT(*) as warning_count" 
			+ into + "#result"
			+ from + "#count_result"
			+ groupBy + "warning_days"
			;	
	
	public static String getContractListByIntegrationNewType = getContractListByType.replace("#result", "#contract")
			+ select 
			+ "*"
			+ into + "#result"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.SignatureDate + ">=? " 
			+ and 
			+ DBSchema.Contract.SignatureDate + "<=? " 
			;	
	
	public static String getContractListByIntegrationNewTypeForCurrency = getContractListByType.replace("#result", "#contract")
			+ select 
			+ "*"
			+ into + "#result"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.SignatureDate + ">=? " 
			+ and 
			+ DBSchema.Contract.SignatureDate + "<=? " 
			+ and 
			+ DBSchema.CurrencyType.CurrenyGraph + "=?"
			;		
	
	public static String getSumInfoByIntegrationNewType = getContractListByType.replace("#result", "#contract")
			+ select 
//			+ DBSchema.CurrencyType.CurrenyGraph 
//			+ ", sum("+DBSchema.Contract.ContractAmt+") as " + DBSchema.Contract.ContractAmt
			+ DBSchema.CurrencyType.CurrenyGraph 
			+ ", sum(" + DBSchema.Contract.ContractAmt + ") as " + DBSchema.Contract.ContractAmt 
			+ ", count(*) as " + DBSchema.Contract.ContractAmtCount
			+ ", sum(" + DBSchema.ChargedDetail.TotalAmtByID + ") as " + DBSchema.ChargedDetail.TotalAmtByID
			+ ", sum(" + DBSchema.ChargedDetail.UnChargedAmtTotal + ") as " + DBSchema.ChargedDetail.UnChargedAmtTotal
			+ ", sum(" + DBSchema.InvoiceDetail.TotalAmtByID + ") as " + DBSchema.InvoiceDetail.TotalAmtByID
			+ ", sum(" + DBSchema.InvoiceDetail.UnAccountAmtTotalSum + ") as " + DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			+ into + "#result"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.SignatureDate + ">=? " 
			+ and 
			+ DBSchema.Contract.SignatureDate + "<=? " 
			+ groupBy + DBSchema.CurrencyType.CurrenyGraph 

			;		
	

	public static String getContractListByTypeFilterWarning = getContractListByType.replace("#result", "#contract")
			+ select 
			+ "*"
			+ into + "#result"
			+ from + "#contract"
			+ where 
			+ "warning_days = ?"
			;
	
	public static String getContractListForParentBySerch = getContractListByType.replace("#result", "#contract")
			+ select 
			+ "*"
			+ into + "#result"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.ParentID + " IS NULL " 
			+ and 
			+ DBSchema.Contract.ContractNo + like + "?"
			+ union
			+ select 
			+ "*"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.ParentID + " IS NULL " 
			+ and 
			+ DBSchema.Contract.Abbreviation + like + "?"
			;

	public static String getChildContractList = select
			+ DBSchema.Contract.ContractID + "," 
			+ DBSchema.Contract.ContractNo + ","
			+ DBSchema.Contract.ContractName + ","
			+ DBSchema.Contract.Abbreviation + ","
			+ DBSchema.Contract.ContractAmt + ","
			+ DBSchema.CurrencyType.CurrenyGraph + ","
			+ DBSchema.Contract.ParentID
			+ from
			+ DBSchema.Contract.TableName + ","
			+ DBSchema.CurrencyType.TableName
			+ where
			+ DBSchema.Contract.ParentID + "=?"
			+ and 
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ContractTypeID 
			+ "*=" + DBSchema.CurrencyType.TableName + "." + DBSchema.CurrencyType.CurrencyID 
			;
	
	public static String getAttachmentListByRefID = select
			+ DBSchema.Attachment.TableName + ".*, "
			+ DBSchema.AttachmentType.TableName + "."
			+ DBSchema.AttachmentType.AttachmentTypeDesc + "," 

			+ "Convert(varchar(10)," + DBSchema.Attachment.TableName
			+ "." + DBSchema.Attachment.UploadDate + ",101) as " 
			+ DBSchema.Attachment.UploadDisplayDate +", "
			
			+ "RTRIM("
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.FirstName
			+ ")+RTRIM(" + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.LastName + ") as " + DBSchema.Attachment.UploaderName 
			+ intoResult
			+ from
			+ DBSchema.Attachment.TableName + ","
			+ DBSchema.AttachmentType.TableName + ","
			+ DBSchema.Employee.TableName + where
			+ DBSchema.Attachment.TableName + "." + DBSchema.Attachment.REFID
			+ "=?" + and + DBSchema.Attachment.TableName + "."
			+ DBSchema.Attachment.AttachmentTypeID + "=?" + and
			+ DBSchema.Attachment.TableName + "."
			+ DBSchema.Attachment.UploaderID + "*="
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + and
			+ DBSchema.Attachment.TableName + "."
			+ DBSchema.Attachment.AttachmentTypeID + "*="
			+ DBSchema.AttachmentType.TableName + "."
			+ DBSchema.AttachmentType.AttachmentTypeID;

	public static String getAttachmentTypeID = select
			+ DBSchema.AttachmentType.AttachmentTypeID + from
			+ DBSchema.AttachmentType.TableName + where
			+ DBSchema.AttachmentType.AttachmentTableName + "=?" + and
			+ DBSchema.AttachmentType.AttachmentFieldName + "=?";

	public static String createNewAttachmentType = insert + into
			+ DBSchema.AttachmentType.TableName + "("
			+ DBSchema.AttachmentType.AttachmentTypeID + ","
			+ DBSchema.AttachmentType.AttachmentTypeDesc + ","
			+ DBSchema.AttachmentType.AttachmentTableName + ","
			+ DBSchema.AttachmentType.AttachmentFieldName
			+ ") values( ?,?,?,? )";

	public static String createNewAttched = insert + into
			+ DBSchema.Attachment.TableName + "("
			+ DBSchema.Attachment.AttachmentID + ","
			+ DBSchema.Attachment.UploadDate + ","
			+ DBSchema.Attachment.UploaderID
			+ ","
			// + DBSchema.Attachment.AttahcmentDesc + ","
			+ DBSchema.Attachment.AttachmentName + ","
			+ DBSchema.Attachment.AttachmentContent + ","
			+ DBSchema.Attachment.AttachmentTypeID + ","
			// + DBSchema.Attachment.AttachmentKey + ","
			+ DBSchema.Attachment.REFID + ") values (?,getdate(),?,?,?,?,?)";

	public static String getAttachmentContent = select + " * " + from
			+ DBSchema.Attachment.TableName + where
			+ DBSchema.Attachment.AttachmentID + "=?";

	public static String createNullContract = insert + into
			+ DBSchema.Contract.TableName + "(" + DBSchema.Contract.ContractID
			+ "," + DBSchema.Contract.ContractNo + ","
			+ DBSchema.Contract.ContractName + ","
			+ DBSchema.Contract.UpdateDate + "," + DBSchema.Contract.RegisterID
			+ ") values(?,'','',getdate(),?)";

	private static String getDeptListByCompanyID1 = select
			// + DBSchema.DepartmentBelongToCompany.TabelName + "." +
			// DBSchema.DepartmentBelongToCompany.DepartmentID + " as parentID,"
			// + DBSchema.Department.TableName + "." +
			// DBSchema.Department.DepartmentName + " as parentName,"
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID + " as parentID,"
			+ DBSchema.Company.TableName + "." + DBSchema.Company.CompanyName
			+ " as parentName," + DBSchema.DepartmentBelongToCompany.TabelName
			+ "." + DBSchema.DepartmentBelongToCompany.DepartmentID
			+ " as deptID," + DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentName + " as deptName";

	private static String getDeptListByCompanyID2 = from
			+ DBSchema.DepartmentBelongToCompany.TabelName + ","
			+ DBSchema.Company.TableName + "," + DBSchema.Department.TableName
			+ where + DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID + "=?" + and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID
			+ "="
			+ DBSchema.Company.TableName
			+ "."
			+ DBSchema.Company.CompanyID
			+ and
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ "."
			+ DBSchema.DepartmentBelongToCompany.DirectlyFlag
			+ "=1" // 公司直属
			+ and + DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + "="
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + and + "("
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DisabledFlag
			+ "<>1" // 非禁用
			+ or + DBSchema.Department.TableName + "."
			+ DBSchema.Department.DisabledFlag + " is null )";

	private static String getDeptListByCompanyID() {
		return getDeptListByCompanyID1 + getDeptListByCompanyID2;
	}

	public static String getDeptListByCompanyID = getDeptListByCompanyID();
	
	public static String getDefaultDeptByCompany = select 
			+ DBSchema.Department.TableName + "." + DBSchema.Department.DepartmentID
			
			+ getDeptListByCompanyID2
			+ and 
			+ DBSchema.Department.TableName + "." + DBSchema.Department.DepartmentName 
			+ "='"
			+ WebKeys.New_Company_Simple_For_Dept_Other
			+ "'"
			;
			

	private static String getDeptListBySelfCompanyID1 = select
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + " as parentID,"
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentName + " as parentName,"
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + " as deptID,"
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentName + " as deptName";

	private static String getDeptListBySelfCompanyID2 = from
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ ","
			+ DBSchema.Department.TableName
			+ where
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID
			+ "=?"
			+ and
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ "."
			+ DBSchema.DepartmentBelongToCompany.DirectlyFlag
			+ "=1" // 公司直属
			+ and
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID
			+ "="
			+ DBSchema.Department.TableName
			+ "."
			+ DBSchema.Department.DepartmentID
			+ and
			+ "("
			+ DBSchema.Department.TableName
			+ "."
			+ DBSchema.Department.DisabledFlag
			+ "<>1" // 非禁用
			+ or + DBSchema.Department.TableName + "."
			+ DBSchema.Department.DisabledFlag + " is null )"

			+ union + select + DBSchema.DepartmentBelongToCompany.TabelName
			+ "." + DBSchema.DepartmentBelongToCompany.DepartmentID
			+ " as parentID," + DBSchema.Department.TableName
			+ "."
			+ DBSchema.Department.DepartmentName
			+ " as parentName,"
			+ DBSchema.DeptBelongToDept.TableName
			+ "."
			+ DBSchema.DeptBelongToDept.DeptID
			+ " as deptID, deptTab."
			+ DBSchema.Department.DepartmentName
			+ " as deptName"
			+ from
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ ","
			+ DBSchema.Department.TableName
			+ ","
			+ DBSchema.DeptBelongToDept.TableName
			+ ","
			+ DBSchema.Department.TableName
			+ " as deptTab "
			+ where
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID
			+ "=?"
			+ and
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ "."
			+ DBSchema.DepartmentBelongToCompany.DirectlyFlag
			+ "=1" // 公司直属
			+ and
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID
			+ "="
			+ DBSchema.Department.TableName
			+ "."
			+ DBSchema.Department.DepartmentID
			+ and
			+ "("
			+ DBSchema.Department.TableName
			+ "."
			+ DBSchema.Department.DisabledFlag
			+ "<>1" // 非禁用
			+ or + DBSchema.Department.TableName + "."
			+ DBSchema.Department.DisabledFlag + " is null )" + and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + "="
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.ParentsID + and
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.DeptID + "= deptTab."
			+ DBSchema.Department.DepartmentID + and + "(deptTab."
			+ DBSchema.Department.DisabledFlag + "<>1" + or + "deptTab."
			+ DBSchema.Department.DisabledFlag + " is null )";

	private static String getDeptListBySelfCompanyID() {
		return getDeptListBySelfCompanyID1 + intoResult + getDeptListBySelfCompanyID2;
	}

	public static String getDeptListBySelfCompanyID = getDeptListBySelfCompanyID();
	
	private static String getPeasonListByCompanyIntoTempTable = into + "#dept ";

	private static String getPeasonListBySelfFromTempTable = select
			+ "#dept.*, " + DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "," + "RTRIM("
			+ Employee.FirstName + ")+RTRIM(" + Employee.LastName
			+ ") user_name, " + DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.Position + from + "#dept, "
			+ DBSchema.EmployeeBelongToDept.TableName + ","
			+ DBSchema.Employee.TableName + where + "#dept.deptID="
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + and
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "="
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + and
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.IsPartTime
			+ "=1" // 全职
			+ and + "(" + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.DisabledFlag
			+ "=0" // 非禁用
			+ or 
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.DisabledFlag 
			+ " is NULL)"
			+ and + DBSchema.Employee.TableName + "." + DBSchema.Employee.ID
			+ ">0"
			;
	
	private static String getPeasonListByCompanyFromTempTable = select
			+ "#dept.*, " + DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "," + "RTRIM("
			+ Employee.FirstName + ")+RTRIM(" + Employee.LastName
			+ ") user_name, " + DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.Position 
			+ from + "#dept, "
			+ DBSchema.EmployeeBelongToDept.TableName + ","
			+ DBSchema.Employee.TableName + where + "#dept.deptID="
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + and
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "="
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + and
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.IsPartTime
			+ "=1" // 全职
			+ and + "(" + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.DisabledFlag
			+ "=0" // 非禁用
			+ or 
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.DisabledFlag 
			+ " is NULL)"
			+ and + DBSchema.Employee.TableName + "." + DBSchema.Employee.ID
			+ ">0"
			;


	private static String subGetPeasonListBySearch = 
			and 
			+ "RTRIM(" + Employee.FirstName + ")+RTRIM(" + Employee.LastName + ")" 
			+ like	+ "?"
			;
	
	private static String subOrderByStr = orderBy + "deptID";
	
	private static String getPeasonListBySelfCompany() {

		return getDeptListBySelfCompanyID1
				+ getPeasonListByCompanyIntoTempTable
				+ getDeptListBySelfCompanyID2
				+ getPeasonListBySelfFromTempTable
				;

	}

	public static String getPeasonListBySelfCompany = getPeasonListBySelfCompany() 
			+ subOrderByStr;

	public static String getPeasonListBySelf = getPeasonListBySelfCompany()
			+ subGetPeasonListBySearch + subOrderByStr;

	private static String getPeasonListByCompany() {

		return getDeptListByCompanyID1 + getPeasonListByCompanyIntoTempTable
				+ getDeptListByCompanyID2 + getPeasonListByCompanyFromTempTable;
	}

	public static String getPeasonListByCompany = getPeasonListByCompany() + subOrderByStr;

	public static String getPeasonListBySearch = getPeasonListByCompany()
			+ subGetPeasonListBySearch + subOrderByStr;

	public static String createNewSimpleCompany = insert + into
			+ DBSchema.Company.TableName + "(" + DBSchema.Company.CompanyID
			+ "," + DBSchema.Company.CompanyName + ","
			+ DBSchema.Company.Abbreviation + ","
			+ DBSchema.Company.BusinessRelationID + ","
			+ DBSchema.Company.UpdateDate + "," + DBSchema.Company.UpdateID
			+ ") values(?,?,?,-1,getdate(),?)";

	public static String setDeptBelongToCompany = insert + into
			+ DBSchema.DepartmentBelongToCompany.TabelName + "("
			+ DBSchema.DepartmentBelongToCompany.CompanyID + ","
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + ","
			+ DBSchema.DepartmentBelongToCompany.DirectlyFlag
			+ ") values(?,?,?)";

	public static String createNewSimpleCRM = insert + into
			+ DBSchema.ContactPerson.TableName + "("
			+ DBSchema.ContactPerson.ContactPersonID + ","
			+ DBSchema.ContactPerson.FirstName + ","
			+ DBSchema.ContactPerson.CompanyID + ","
			+ DBSchema.ContactPerson.DepartmentID + ","
			+ DBSchema.ContactPerson.UpdateDate + ","
			+ DBSchema.ContactPerson.UpdateID + ") values(?,?,?,?,getdate(),?)";

	public static String getCompanyID = select
			+ DBSchema.Company.CompanyID 
			+ from 
			+ DBSchema.Company.TableName 
			+ where 
			+ DBSchema.Company.CompanyName 
			+ "=?"
			;
	
	public static String getDepartmentIDByCompany = select
			+ DBSchema.Department.TableName + "."+ DBSchema.Department.DepartmentID
			+ from 
			+ DBSchema.Department.TableName + ","
			+ DBSchema.DepartmentBelongToCompany.TabelName 
			+ where 
			+ DBSchema.Department.TableName + "." + DBSchema.Department.DepartmentName + "=?"
			+ and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "." + DBSchema.DepartmentBelongToCompany.CompanyID + "=?"
			+ and 
			+ DBSchema.Department.TableName + "." + DBSchema.Department.DepartmentID + "="
			+ DBSchema.DepartmentBelongToCompany.TabelName +"."+ DBSchema.DepartmentBelongToCompany.DepartmentID
			;
	
	public static String getContactPeasonIDByCompany = select
			+ DBSchema.ContactPerson.ContactPersonID 
			+ from 
			+ DBSchema.ContactPerson.TableName
			+ where
			+ "TRIM(" + DBSchema.ContactPerson.FirstName + ")+TRIM("
			+ DBSchema.ContactPerson.LastName + ")=?"
			+ and 
			+ DBSchema.ContactPerson.CompanyID + "=?"
			;
	
	public static String getUserIDByCompany = select 
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID
			+ from 
			+ DBSchema.Employee.TableName + ","
			+ DBSchema.EmployeeBelongToDept.TableName + ","
			+ DBSchema.DepartmentBelongToCompany.TabelName
			+ where
			+ DBSchema.DepartmentBelongToCompany.TabelName + "." + DBSchema.DepartmentBelongToCompany.CompanyID + "=?"
			+ and 
			+ DBSchema.DepartmentBelongToCompany.TabelName + "." + DBSchema.DepartmentBelongToCompany.DepartmentID + "=?"
			+ and 
			+ DBSchema.DepartmentBelongToCompany.TabelName + "." + DBSchema.DepartmentBelongToCompany.DepartmentID 
			+ "=" + DBSchema.EmployeeBelongToDept.TableName + "." + DBSchema.EmployeeBelongToDept.DepartmentID 
			+ and 
			+ "RTRIM(" + DBSchema.Employee.TableName + "." + DBSchema.Employee.FirstName + ")+RTRIM("
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.LastName + ")=?"
			;
	
	
	
	public static String updateContractInfo = update 
			+ DBSchema.Contract.TableName 
			+ set
			+ DBSchema.Contract.ContractTypeID + "=?,"		//1
			+ DBSchema.Contract.SourceTypeID + "=?,"		//2
			+ DBSchema.Contract.GovermentContractSourceID + "=?,"		//3
			+ DBSchema.Contract.ContractNo + "=?,"			//4
			+ DBSchema.Contract.ContractName + "=?,"		//5
			+ DBSchema.Contract.Abbreviation + "=?,"		//6
			+ DBSchema.Contract.ContractSummary + "=?,"		//7
			+ DBSchema.Contract.ParentID + "=?,"		//8
			+ DBSchema.Contract.WarrantyDefinition + "=?,"		//9
			+ DBSchema.Contract.WarrantyStartDate + "=?,"		//10
			+ DBSchema.Contract.WarrantyEndDate + "=?,"		//11
			+ DBSchema.Contract.ContractAmt + "=?,"		//12
			+ DBSchema.Contract.CurrencyID + "=?,"		//13
			+ DBSchema.Contract.SignatureDate + "=?,"		//14
			+ DBSchema.Contract.BuyerID + "=?,"		//15
			+ DBSchema.Contract.DeptIDOfBuyer + "=?,"		//16
			+ DBSchema.Contract.BuyerRepresentativeID + "=?,"		//17
			+ DBSchema.Contract.VendorID + "=?,"		//18
			+ DBSchema.Contract.DeptIDOfVendor + "=?,"		//19
			+ DBSchema.Contract.VendorRepresentativeID + "=?,"		//20
			+ DBSchema.Contract.Remark + "=?,"		//21
			+ DBSchema.Contract.DeliveryID + "=?,"		//22
			+ DBSchema.Contract.UpdateDate + "=getdate(),"		
			+ DBSchema.Contract.RegisterID + "=?,"		//23
			+ DBSchema.Contract.WarrantyLimit + "=?,"	//24
			+ DBSchema.Contract.WarrantyUnit + "=?"		//25
			+ where 
			+ DBSchema.Contract.ContractID + "=?" 		//26
			;
			
	public static String getPaymentConditionByContract = select 
			+ DBSchema.PaymentCondition.TableName + ".*" + ","
			+ "Convert(varchar(10)," + DBSchema.PaymentCondition.TableName
			+ "." + DBSchema.PaymentCondition.PaymentDate + ",101) as " 
			+ DBSchema.PaymentCondition.PaymentDateDisplayed +", "
			+ DBSchema.ProjectStatus.TableName + "." + DBSchema.ProjectStatus.ProjectStatusName 
			+ " as " + DBSchema.PaymentCondition.ProjectStatusName
			+ from 
			+ DBSchema.PaymentCondition.TableName + "," 
			+ DBSchema.ProjectStatus.TableName
			+ where
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID + "=?"
			+ and
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ProjectStatusID + "*="
			+ DBSchema.ProjectStatus.TableName + "." + DBSchema.ProjectStatus.ProjectStatusID
			;
	
	public static String getAllProjectStatus = select 
			+ DBSchema.ProjectStatus.TableName + ".*"
			+ from 
			+ DBSchema.ProjectStatus.TableName
			+ where
			+ DBSchema.ProjectStatus.ProjectStatusName
			+ like + "?"
			;
			
	
	public static String intsertProjectStatus = insert + into
			+ DBSchema.ProjectStatus.TableName + "("
			+ DBSchema.ProjectStatus.ProjectStatusID + ","
			+ DBSchema.ProjectStatus.ProjectStatusName + ","
			+ DBSchema.ProjectStatus.ProjectStatusDescription + ","
			+ DBSchema.ProjectStatus.SelfDefinitionFlag
			+ ")" + values
			+ "(?,?,?,1)";	
	
	public static String insertPaymentCondition = insert + into
			+ DBSchema.PaymentCondition.TableName + "("
			+ DBSchema.PaymentCondition.ContractID + ","					//1
			+ DBSchema.PaymentCondition.PaymentConditionID + ","			//2
			+ DBSchema.PaymentCondition.PaymentConditionDescription + ","	//3
			+ DBSchema.PaymentCondition.ProjectStatusID + ","				//4
			+ DBSchema.PaymentCondition.DelayDays + ","						//5
//			+ DBSchema.PaymentCondition.PaymentDate + ","					//6
			+ DBSchema.PaymentCondition.ManualFlag //+ ","					//7
//			+ DBSchema.PaymentCondition.PaymentPercent + ","				//8
//			+ DBSchema.PaymentCondition.PaymentAmt							//9
			+ ") values(?,?,'',-1,0,1)" //,?,?,?,?,?,?,?)"
			;
	
	public static String savePaymentCondition = update 
			+ DBSchema.PaymentCondition.TableName
			+ set 
			+ DBSchema.PaymentCondition.PaymentConditionDescription + "=?,"	//1
			+ DBSchema.PaymentCondition.ProjectStatusID + "=?,"				//2
			+ DBSchema.PaymentCondition.DelayDays + "=?,"					//3
			+ DBSchema.PaymentCondition.PaymentDate + "=?,"					//4
			+ DBSchema.PaymentCondition.ManualFlag + "=?,"					//5
			+ DBSchema.PaymentCondition.PaymentPercent + "=?,"				//6
			+ DBSchema.PaymentCondition.PaymentAmt + "=?,"					//7
			+ DBSchema.PaymentCondition.ManualReason + "=?"					//8
			+ where
			+ DBSchema.PaymentCondition.ContractID + "=?"					//9
			+ and 
			+ DBSchema.PaymentCondition.PaymentConditionID + "=?"			//10
			;
	
	public static String insertPaymentConditionLog = insert + into
			+ DBSchema.PaymentConditionManualLog.TableName + "("
			+ DBSchema.PaymentConditionManualLog.LogID + ","			//1
			+ DBSchema.PaymentConditionManualLog.ContractID + ","		//2
			+ DBSchema.PaymentConditionManualLog.PaymentConditionID + ","	//3
			+ DBSchema.PaymentConditionManualLog.PaymentDate + ","		//4
			+ DBSchema.PaymentConditionManualLog.PaymentPercent + ","	//5
			+ DBSchema.PaymentConditionManualLog.PaymentAmt + ","		//6
			+ DBSchema.PaymentConditionManualLog.ChangedReason + ","	//7
			+ DBSchema.PaymentConditionManualLog.ChangedDate + ","		
			+ DBSchema.PaymentConditionManualLog.ChangedID 				//8
			+ ") values(?,?,?,?,?,?,?,getdate(),?)"
			
			;
	
	
	
	public static String deletePaymentCondition = delete
			+ DBSchema.PaymentCondition.TableName 
			+ where 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID
			+ "=?"
			+ and 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.PaymentConditionID
			+ "=?"
			;
	

	public static String getCountByIDFromTab(String columnName, String tableName){
		
		return select + "count(*)" + from + tableName + where + columnName + "=?";
	}
	
	public static String updatePaymnetConditionFortributed = update 
			+ DBSchema.PaymentCondition.TableName
			+ set
			+ DBSchema.PaymentCondition.PaymentPercent 
			+ "=cast(" + DBSchema.PaymentCondition.PaymentAmt + "/?*100 as decimal(15,0))" 
			+ where
			+ DBSchema.PaymentCondition.ContractID + "=?"
			;

	
	public static String insertChargedDetail = insert + into
			+ DBSchema.ChargedDetail.TableName + "("
			+ DBSchema.ChargedDetail.ContractID + ","			//1
			+ DBSchema.ChargedDetail.ChargedID + ","			//2
			+ DBSchema.ChargedDetail.ChargedAmt + ","			//3
			+ DBSchema.ChargedDetail.ChargedDate + ","			//4
			+ DBSchema.ChargedDetail.TransactorID				//5
			+ ") values(?,?,0,getdate(),?)" 
			;	
	
	public static String saveChargedDetail = update 
			+ DBSchema.ChargedDetail.TableName
			+ set 
			+ DBSchema.ChargedDetail.ChargedAmt + "=?,"			//1
			+ DBSchema.ChargedDetail.ChargedDate + "=?,"		//2
			+ DBSchema.ChargedDetail.TransactorID + "=?,"		//3
			+ DBSchema.ChargedDetail.UpdateID + "=?,"			//4
			+ DBSchema.ChargedDetail.UpdateDate + "=getdate(),"	
			+ DBSchema.ChargedDetail.ChargedRemark + "=?"		//5
			+ where
			+ DBSchema.ChargedDetail.ContractID + "=?"			//6
			+ and 
			+ DBSchema.ChargedDetail.ChargedID + "=?"			//7
			;	
	
	public static String deleteChargedDetail = delete
			+ DBSchema.ChargedDetail.TableName 
			+ where 
			+ DBSchema.ChargedDetail.ContractID
			+ "=?"
			+ and 
			+ DBSchema.ChargedDetail.ChargedID
			+ "=?"
			;
/**	
 * saveInvoiceDetail
 */
	public static String insertInvoiceDetail = insert + into
			+ DBSchema.InvoiceDetail.TableName + "("
			+ DBSchema.InvoiceDetail.ContractID + ","			//1
			+ DBSchema.InvoiceDetail.InvoiceNo + ","			//2
			+ DBSchema.InvoiceDetail.TransactorID + ","			//3
			+ DBSchema.InvoiceDetail.InvoiceAmt + ","			//4
			+ DBSchema.InvoiceDetail.InvoiceDate + ","			//5
			+ DBSchema.InvoiceDetail.UpdateID				//6
			+ ") values(?,?,0,0,getdate(),?)" 
			;		
	
	public static String saveInvoiceDetail = update 
			+ DBSchema.InvoiceDetail.TableName
			+ set 
			+ DBSchema.InvoiceDetail.InvoiceAmt + "=?,"			//1
			+ DBSchema.InvoiceDetail.InvoiceDate + "=?,"		//2
			+ DBSchema.InvoiceDetail.TransactorID + "=?,"		//3
			+ DBSchema.InvoiceDetail.UpdateID + "=?,"			//4
			+ DBSchema.InvoiceDetail.UpdateDate + "=getdate(),"	
			+ DBSchema.InvoiceDetail.Remark + "=?,"		//5
			+ DBSchema.InvoiceDetail.InvoiceCode + "=?"		//6
			
			+ where
			+ DBSchema.InvoiceDetail.ContractID + "=?"			//7
			+ and 
			+ DBSchema.InvoiceDetail.InvoiceNo + "=?"			//8
	;	
	

	public static String deleteInvoiceDetail = delete
			+ DBSchema.InvoiceDetail.TableName 
			+ where 
			+ DBSchema.InvoiceDetail.ContractID
			+ "=?"
			+ and 
			+ DBSchema.InvoiceDetail.InvoiceNo
			+ "=?"
			;	
	
	public static String getRunContractInTimeScope = getContractListByType.replace("#result", "#contract") 
			+ select 
			+ DBSchema.ChargedDetail.TableName + "." + DBSchema.ChargedDetail.ContractID 
			+ ", sum(" + DBSchema.ChargedDetail.TableName + "." + DBSchema.ChargedDetail.ChargedAmt 
			+ ") as contract_payed_amt, 0 as contract_invoice_amt, 0 as contract_estimate_amt " 
			+ into
			+ "#contract_pay_invoice"
			+ from 
			+ DBSchema.ChargedDetail.TableName 
			+ where 
			+ DBSchema.ChargedDetail.TableName + "." + DBSchema.ChargedDetail.ChargedDate + ">=?"
			+ and
			+ DBSchema.ChargedDetail.TableName + "." + DBSchema.ChargedDetail.ChargedDate + "<=?"
			+ groupBy
			+ DBSchema.ChargedDetail.TableName + "." + DBSchema.ChargedDetail.ContractID
			+ union
			+ select 
			+ DBSchema.InvoiceDetail.TableName + "." + DBSchema.InvoiceDetail.ContractID 
			+ ", 0, sum(" + DBSchema.InvoiceDetail.TableName + "." + DBSchema.InvoiceDetail.InvoiceAmt + "), 0 "
			+ from 
			+ DBSchema.InvoiceDetail.TableName 
			+ where 
			+ DBSchema.InvoiceDetail.TableName + "." + DBSchema.InvoiceDetail.InvoiceDate + ">=?"
			+ and
			+ DBSchema.InvoiceDetail.TableName + "." + DBSchema.InvoiceDetail.InvoiceDate + "<=?"
			+ groupBy
			+ DBSchema.InvoiceDetail.TableName + "." + DBSchema.InvoiceDetail.ContractID
			+ union
			+ select 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID 
			+ ", 0, 0, sum(" + DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.PaymentAmt + ") "
			+ from 
			+ DBSchema.PaymentCondition.TableName 
			+ where 
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.PaymentDate + ">=?"
			+ and
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.PaymentDate + "<=?"
			+ groupBy
			+ DBSchema.PaymentCondition.TableName + "." + DBSchema.PaymentCondition.ContractID
			
			+ select 
			+ DBSchema.ChargedDetail.ContractID 
			+ ", sum(contract_payed_amt) as contract_payed_amt" 
			+ ", sum(contract_invoice_amt) as contract_invoice_amt" 
			+ ", sum(contract_estimate_amt) as contract_estimate_amt"
			+ into 
			+ "#contract_pay_invoice_sum"
			+ from 
			+ "#contract_pay_invoice"
			+ groupBy
			+ DBSchema.ChargedDetail.ContractID
			
			+ select 
			+ "#contract.*, contract_payed_amt, contract_invoice_amt,contract_estimate_amt"
			+ into 
			+ "#result"
			+ from 
			+ "#contract,#contract_pay_invoice_sum "
			+ where 
			+ "#contract." + DBSchema.Contract.ContractID 
			+ "= #contract_pay_invoice_sum." + DBSchema.ChargedDetail.ContractID
			
			;

	
	public static String getSumInfoByIntegrationPerformed = getRunContractInTimeScope.replace("#result", "#contract_timescope")
			+ select 
			+ DBSchema.CurrencyType.CurrenyGraph 
//			+ ", sum(" + DBSchema.Contract.ContractAmt + ") as " + DBSchema.Contract.ContractAmt 
//			+ ", count(*) as " + DBSchema.Contract.ContractAmtCount
//			+ ", sum(" + DBSchema.ChargedDetail.TotalAmtByID + ") as " + DBSchema.ChargedDetail.TotalAmtByID
//			+ ", sum(" + DBSchema.ChargedDetail.UnChargedAmtTotal + ") as " + DBSchema.ChargedDetail.UnChargedAmtTotal
//			+ ", sum(" + DBSchema.InvoiceDetail.TotalAmtByID + ") as " + DBSchema.InvoiceDetail.TotalAmtByID
//			+ ", sum(" + DBSchema.InvoiceDetail.UnAccountAmtTotalSum + ") as " + DBSchema.InvoiceDetail.UnAccountAmtTotalSum

			+ ", sum(contract_payed_amt) as contract_payed_amt, count(*) as contract_payed_count " 
			+ ", 0 as contract_invoice_amt, 0 as contract_invoice_count " 
			+ ", 0 as contract_estimate_amt, 0 as contract_estimate_count "
			
			+ into + "#contract_count"
			+ from + "#contract_timescope"
			+ where 
			+ " contract_payed_amt > 0 "
			+ groupBy + DBSchema.CurrencyType.CurrenyGraph 
			
			+ union 
			+ select 
			+ DBSchema.CurrencyType.CurrenyGraph 
			+ ", 0 , 0 , sum(contract_invoice_amt), count(*), 0, 0 " 
			
			+ from + "#contract_timescope"
			+ where 
			+ " contract_invoice_amt > 0 "
			+ groupBy + DBSchema.CurrencyType.CurrenyGraph 
			
			+ union 
			+ select 
			+ DBSchema.CurrencyType.CurrenyGraph 
			+ ", 0 , 0 , 0, 0, sum(contract_estimate_amt), count(*) " 
			
			+ from + "#contract_timescope"
			+ where 
			+ " contract_estimate_amt > 0 "
			+ groupBy + DBSchema.CurrencyType.CurrenyGraph 
			
			+ select 
			+ DBSchema.CurrencyType.CurrenyGraph 
			+ ", sum(contract_payed_amt) as contract_payed_amt, sum(contract_payed_count) as contract_payed_count"
			+ ", sum(contract_invoice_amt) as contract_invoice_amt, sum(contract_invoice_count) as contract_invoice_count" 
			+ ", sum(contract_estimate_amt) as contract_estimate_amt, sum(contract_estimate_count) as contract_estimate_count" 
			
			+ into 
			+ "#result"
			+ from 
			+ "#contract_count"
			+ groupBy + DBSchema.CurrencyType.CurrenyGraph 
			;			

	public static String getPerformedSubIntegrationForPayed = getRunContractInTimeScope.replace("#result", "#contract_timescope")
			+ select + "*"
			+ into 
			+ "#result"
			+ from 
			+ "#contract_timescope"
			+ where 
			+ DBSchema.CurrencyType.CurrenyGraph + "=?"
			+ and 
			+ "contract_payed_amt > 0"
			;

	public static String getPerformedSubIntegrationForInvoiced = getRunContractInTimeScope.replace("#result", "#contract_timescope")
			+ select + "*"
			+ into 
			+ "#result"
			+ from 
			+ "#contract_timescope"
			+ where 
			+ DBSchema.CurrencyType.CurrenyGraph + "=?"
			+ and 
			+ "contract_invoice_amt > 0"
			;

	public static String getPerformedSubIntegrationForEstimate = getRunContractInTimeScope.replace("#result", "#contract_timescope")
			+ select + "*"
			+ into 
			+ "#result"
			+ from 
			+ "#contract_timescope"
			+ where 
			+ DBSchema.CurrencyType.CurrenyGraph + "=?"
			+ and 
			+ "contract_estimate_amt > 0"
			;

	public static String getContractListByIntegrationWarranty = getContractListByType.replace("#result", "#contract")
			+ select 
			+ "*"
			+ into + "#result"
			+ from + "#contract"
			+ where 
			+ DBSchema.Contract.WarrantyEndDate + ">=? " 
			+ and 
			+ DBSchema.Contract.WarrantyEndDate + "<=? " 
			;	
}
















