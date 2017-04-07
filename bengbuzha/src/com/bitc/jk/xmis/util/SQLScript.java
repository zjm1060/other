package com.bitc.jk.xmis.util;

import org.apache.commons.lang.StringUtils;

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

	private static String setNocountOn = "\n\n SET NOCOUNT ON \n";

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

			+ "\n\n SELECT identity(int,1,1) as 'row_no', dept_id, 部门名称 dept_name, finished_count, unfinished_count, "
			+ "finished_count+unfinished_count AS total_count"
			+ "	INTO #a	FROM	#deptProjSum, xmis.org_部门 dept"
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
			// + into
			// + "#deptTemp " // INTO #deptTemp
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
			+ "#deptTree.*,"
			+ " (RTRIM("
			+ Employee.FirstName
			+ ")+RTRIM("
			+ Employee.LastName
			+ ")) user_name,"// SELECT #deptTree.*,
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
			+ "\nINTO	 #taskCount	 FROM	 #task	 WHERE	 end_date < GETDATE()	 GROUP BY	 user_id, proj_id "
			+ "\nUNION ALL"
			+ "\nSELECT user_id,	 proj_id,	 0,	 COUNT(task_id),	 0	 FROM	 #task	 WHERE	 beg_date <= GETDATE() "
			+ "AND GETDATE() < end_date	 GROUP BY	 user_id, proj_id"
			+ "\nUNION ALL"
			+ "\nSELECT user_id,	 proj_id,	 0,	 0,	 COUNT(task_id)	 FROM	 #task	 WHERE	 GETDATE() < beg_date "
			+ "GROUP BY	 user_id, proj_id"

			+ "\n\nSELECT	user_id,	 proj_id,	 SUM(finished) finished, "
			+ "SUM(unfinished) unfinished,	 SUM(not_start) not_start"
			+ "\nINTO	 #taskSUM "
			+ "\nFROM	 #taskCount "
			+ "GROUP BY	 user_id, proj_id "
			// + "\nORDER BY user_id, proj_id, not_start, unfinished, finished "

			+ "\n\nSELECT 	user_id,"
			+ "(RTRIM("
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM("
			+ DBSchema.Employee.LastName
			+ ")) user_name, "
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

	public static String resultStrByBiz = select + "#result.*" + into
			+ "#resultC" + from + "#result" + where + "1=2"

			+ insert + into + "#resultC" + select + "*" + from + "#result";
	public static String getResultStringWithPagingDefault = "\n\nSELECT * from #resultA WHERE row_no = (select max(row_no) from #resultA ) UNION ALL "
			+ "\nSELECT * from #resultA "
			+ "WHERE row_no >= ? "
			+ " AND row_no < ?";
	public static String getResultStringWithPagingOfContract = "\n\nSELECT * from #resultA "
			+ "WHERE row_no >= ? "
			+ " AND row_no < ?  UNION ALL "
			+ "\nSELECT MAX(row_no) as row_no,null as 合同ID,null as 合同编号,null as 合同名称,null as 简称,null as 父合同ID,null as 签订日期,null as 甲方公司ID,null as 甲方部门ID,null as 乙方公司ID,null as 乙方部门ID,SUM(合同总额) as 合同总额,null as 货币ID,null as 合同类型ID,null as 甲方代表ID,null as 乙方代表ID,null as 合同概要,null as 备注,null as 登记人ID,null as 更新时间,null as 合同来源类型ID,null as 横向合同来源ID,null as 质保期起,null as 质保期止,null as 质保期定义,null as 发货联系人ID,null as 质保期限,null as 质保期计量单位,null as 项目ID,null as 合同负责人,null as 最后修改日期,null as 最后修改人ID,null,null,null,null,null as start_date,null as end_date,null as update_date,null as contractNo,null as abbreviation,null as aCompany_name,null as aCompany_abb,null as bCompany_name,null as aDept_name,null as bDept_name,null as aEmp_name,null as bEmp_name,null as cEmp_name, null as 负责人姓名,null as 横向合同来源,null as 货币符号,  null as execu_id,null as execu_name,  null as summarystencil,null as keywords,null as manager_id,null as manager_name,null as 对应收入合同部门ID,null as 对应收入合同负责人,null as 项目编码,null as 项目状态ID,null as 项目状态,null as 项目名称,sum(收款合计) as 收款合计,SUM(未收款金额合计) as 未收款金额合计,SUM(已开发票金额合计) as 已开发票金额合计,SUM(已开收据金额合计) as 已开收据金额合计,SUM(已开票未到账金额合计) as 已开票未到账金额合计,SUM(预付账款金额合计) as 预付账款金额合计,null as warning_date,sum(warning_amt) as warning_amt,null as warning_amtPlan,sum(warning_bjamt) as warning_bjamt,null as warning_days,null as 原因,null as 经办人,null as 登记日期,null as 登记人,sum(MB)as MB,sum(elseMB) as elseMB,NULL AS bcloseflag,SUM(difMB) AS difMB,sum(unMB) as unMB,sum(pay_amt) as pay_amt,sum(按进度已开票未到款) as 按进度已开票未到款,sum(按进度未开票应收款) as 按进度未开票应收款   from #resultA ";

	public static String getResultStringWithPagingOfContractByPayInfo = "\n\nSELECT * from #resultA "
			+ "WHERE row_no >= ? "
			+ " AND row_no < ?  UNION ALL "
			+ "\nSELECT MAX(row_no) as row_no,null as 合同ID,null as 合同编号,null as 合同名称,null as 简称,null as 父合同ID,null as 签订日期,null as 甲方公司ID,null as 甲方部门ID,null as 乙方公司ID,null as 乙方部门ID,SUM(合同总额) as 合同总额,null as 货币ID,null as 合同类型ID,null as 甲方代表ID,null as 乙方代表ID,null as 合同概要,null as 备注,null as 登记人ID,null as 更新时间,null as 合同来源类型ID,null as 横向合同来源ID,null as 质保期起,null as 质保期止,null as 质保期定义,null as 发货联系人ID,null as 质保期限,null as 质保期计量单位,null as 项目ID,null as 合同负责人,null as 最后修改日期,null as 最后修改人ID,null,null,null,null,null as start_date,null as end_date,null as update_date,null as contractNo,null as abbreviation,null as aCompany_name,null as aCompany_abb,null as bCompany_name,null as aDept_name,null as bDept_name,null as aEmp_name,null as bEmp_name,null as cEmp_name, null as 负责人姓名,null as 横向合同来源,null as 货币符号,  null as execu_id,null as execu_name,  null as summarystencil,null as keywords,null as manager_id,null as manager_name,null as 对应收入合同部门ID,null as 对应收入合同负责人,null as 项目编码,null as 项目状态ID,null as 项目状态,null as 项目名称,sum(收款合计) as 收款合计,SUM(未收款金额合计) as 未收款金额合计,SUM(已开发票金额合计) as 已开发票金额合计,SUM(已开收据金额合计) as 已开收据金额合计,SUM(已开票未到账金额合计) as 已开票未到账金额合计,SUM(预付账款金额合计) as 预付账款金额合计,null as warning_date,sum(warning_amt) as warning_amt,null as warning_amtPlan,sum(warning_bjamt) as warning_bjamt,null as warning_days,null as 原因,null as 经办人,null as 登记日期,null as 登记人,sum(MB)as MB,sum(elseMB) as elseMB,NULL AS bcloseflag,SUM(difMB) AS difMB,sum(unMB) as unMB,sum(pay_amt) as pay_amt,sum(按进度已开票未到款) as 按进度已开票未到款,sum(按进度未开票应收款) as 按进度未开票应收款 ,sum(contract_payed_amt) as contract_payed_amt,sum(contract_invoice_amt) as contract_invoice_amt,sum(contract_estimate_amt) as contract_estimate_amt,sum(officialReceiptsMB) as officialReceiptsMB,sum(officialReceiptsPayAmt) as officialReceiptsPayAmt from #resultA ";

	public static String getResultStrWithPaging = "\n\nselect identity(int,1,1) as row_no, #result.* "
			+ "\n INTO #resultA " + "FROM #result "

	// + getResultStringWithPagingDefault
	;

	public static String getResultStrWithoutPaging = "\nSELECT * from #result ";

	public static String positionStr = "\n\nselect identity(int,1,1) as row_no, #resultC.* "
			+ "\n INTO #resultA "
			+ "FROM #resultC "

			+ "\n\nSelect max(row_no) as row_no from #resultA  UNION ALL "
			+ select + "row_no" + from + "#resultA";

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
			+ DBSchema.Employee.UpdateDate + "=GETDATE() " + where
			+ DBSchema.Employee.ID + " = ? ";

	public static String saveUserPhotoName = update
			+ DBSchema.Employee.TableName + set + DBSchema.Employee.imgName
			+ " = ?, " + DBSchema.Employee.UpdateID + "= ?,"
			+ DBSchema.Employee.UpdateDate + "= GETDATE()" + where
			+ DBSchema.Employee.ID + " = ? ";

	public static String getPoliticalStatusList = select + "*" + from
			+ DBSchema.PoliticalStatus.TableName;

	public static String getLunchAddressList = select + "*" + from
			+ DBSchema.LunchAddress.TableName;

	public static String getParentDeptIDByDeptID = select
			+ DBSchema.DeptBelongToDept.ParentsID + from
			+ DBSchema.DeptBelongToDept.TableName + where
			+ DBSchema.DeptBelongToDept.DeptID + "=?";

	public static String updateEdu = update + DBSchema.EducationLevel.TableName
			+ set + DBSchema.EducationLevel.EducationLevelName + "= ?,"
			+ DBSchema.EducationLevel.SchoolName + "= ?,"
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

	public static String getMaxIDFromTab(String columnName, String tableName) {
		// select + "count(*)+1001" + from;

		// return select + "Max(CONVERT(INT," + columnName + "))" + from +
		// tableName;
		return select + "Max(" + columnName + ")" + from + tableName;
	}

	public static String getMaxIDFromTab(String columnName, String tableName,
			String whereStr) {

		System.out.println(getMaxIDFromTab(columnName, tableName) + where
				+ whereStr);
		return getMaxIDFromTab(columnName, tableName) + where + whereStr;
	}

	public static String changeEduForEmployee = update
			+ DBSchema.Employee.TableName + set
			+ DBSchema.Employee.EduicationLevelID + "= ?, "
			+ DBSchema.Employee.UpdateID + "= ?,"
			+ DBSchema.Employee.UpdateDate + "= GETDATE()" + where
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

	public static String changeEmpBelongToDept = update
			+ DBSchema.EmployeeBelongToDept.TableName + set
			+ DBSchema.EmployeeBelongToDept.DepartmentID + "= ? " + where
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "= ?";

	public static String changeDeptBelongToCompany = update
			+ DBSchema.DeptBelongToCompany.TableName + set
			+ DBSchema.DeptBelongToCompany.companyID + "=? " + where
			+ DBSchema.DeptBelongToCompany.DepartmentID + "=?";

	public static String changeDeptBelongToDept = update
			+ DBSchema.DeptBelongToDept.TableName + set
			+ DBSchema.DeptBelongToDept.ParentsID + "= ? " + where
			+ DBSchema.DeptBelongToDept.DeptID + "= ?";

	public static String updateEmployeeName = update
			+ DBSchema.Employee.TableName + set + DBSchema.Employee.LastName
			+ "= ?," + DBSchema.Employee.FirstName + "= ?" + where
			+ DBSchema.Employee.ID + "=?";

	public static String updateDeptName = update
			+ DBSchema.Department.TableName + set
			+ DBSchema.Department.DepartmentName + "= ?" + where
			+ DBSchema.Department.DepartmentID + "= ?";

	public static String createNewUser = insert
			+ into
			+ DBSchema.Employee.TableName
			+ "("
			+ DBSchema.Employee.ID
			+ "," // 1
			+ DBSchema.Employee.FirstName
			+ "," // 2
			+ DBSchema.Employee.LastName
			+ "," // 3
			+ DBSchema.Employee.LoginName
			+ "," // 4
			+ DBSchema.Employee.Password + "," + DBSchema.Employee.SexID
			+ ","
			+ DBSchema.Employee.CreateID
			+ "," // 5
			+ DBSchema.Employee.CreateDate + ","
			+ DBSchema.Employee.DisabledFlag + ")" + values
			+ "(?,?,?,?,'000000',1,?,GETDATE(),0)";

	public static String setEmpBelongToDept =
	// "declare @rid integer" +
	// "\n select @rid=max(ID)+1 from "+ DBSchema.EmployeeBelongToDept.TableName
	// +
	// @rid
	insert + into + DBSchema.EmployeeBelongToDept.TableName + "("
			+ DBSchema.EmployeeBelongToDept.ID + ","
			+ DBSchema.EmployeeBelongToDept.EmployeeID + ","
			+ DBSchema.EmployeeBelongToDept.DepartmentID + ","
			+ DBSchema.EmployeeBelongToDept.Position + ","
			+ DBSchema.EmployeeBelongToDept.IsPartTime + ")" + values
			+ "(?,?,?,null,1)";

	public static String setEmpRole = insert + into
			+ DBSchema.UserRole.TableName + "(" + DBSchema.UserRole.ID + ","
			+ DBSchema.UserRole.UserID + "," + DBSchema.UserRole.RoleID + ")"
			+ values + "(?,?,?)";

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

			+ "\nDECLARE @addID varchar(50)" + "\nSET ROWCOUNT 1" + select
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
			+ "(GETDATE(),?,1,1, ?)";

	public static String updateLunchAddress = update
			+ DBSchema.EmployeeLunchAddress.TableName + set
			+ DBSchema.EmployeeLunchAddress.LunchAddressID + "=?,"
			+ DBSchema.EmployeeLunchAddress.UpdateDate + "=GETDATE(),"
			+ DBSchema.EmployeeLunchAddress.UpdateID + "=?" + where
			+ DBSchema.EmployeeLunchAddress.EmployeeID + "=?";

	public static String inertLunchAddress = insert + into
			+ DBSchema.EmployeeLunchAddress.TableName + "("
			+ DBSchema.EmployeeLunchAddress.UpdateDate + ","
			+ DBSchema.EmployeeLunchAddress.EmployeeID + ","
			+ DBSchema.EmployeeLunchAddress.LunchAddressID + ","
			+ DBSchema.EmployeeLunchAddress.BookedFlag + ","
			+ DBSchema.EmployeeLunchAddress.UpdateID + ")" + values
			+ "(GETDATE(),?,?,0,?)";

	public static String updateEmployeeInfo = update
			+ DBSchema.Employee.TableName
			+ set
			+ DBSchema.Employee.LastName
			+ "=?," // 1
			+ DBSchema.Employee.FirstName
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
			+ DBSchema.Employee.Locked
			+ "= ?," // 29
			+ DBSchema.Employee.OfficeAddressID
			+ "= ?," // 30
			+ DBSchema.Employee.Urgentinfo
			+ "= ?," // 31
			+ DBSchema.Employee.UpdateDate
			+ "= GETDATE(),\n"
			+
			// "        --32 开始\n" +
			"        在职类型 = ?,\n" + "        民族 = ? ,\n"
			+ "        资格取得时间 = ? ,\n" + "        字段1 = ? ,\n"
			+ "        工资级别 = ? ,\n" + "        参加工作年月 = ? ,\n"
			+ "        入党年月 = ? ,\n" + "        入团年月 = ? ,\n"
			+ "        何时何校何专业毕业 = ? ,\n" + "        备注1 = ? ,\n"
			+ "        备注2 = ? ,\n" + "        职务级别 = ? ,\n"
			+ "        技术职务 = ? ,\n" + "        专业技术级别 = ? ,\n"
			+ "       现有文化程度 = ? ,\n" + "       工种 = ? ,\n"
			+ "       备注3 = ? ,\n" + "       身份 = ? ,\n" + "       退休时间 = ?\n"
			+ where + DBSchema.Employee.ID + "=?" // 51
	;

	public static String disableDeparment = update
			+ DBSchema.Department.TableName + set
			+ DBSchema.Department.DisabledFlag + "=1" + where
			+ DBSchema.Department.DepartmentID + "=?";

	public static String disableEmployee = update + DBSchema.Employee.TableName
			+ set + DBSchema.Employee.DisabledFlag + "=1,"
			+ DBSchema.Employee.UpdateID
			+ "= ?," // 28
			+ DBSchema.Employee.UpdateDate + "= GETDATE()" + where
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

	public static String getContractInfo = "" // setNocountOn
	// + select
	// + DBSchema.Contract.TableName
	// + ".*,"
	// + DBSchema.CurrencyType.CurrencyName
	// + ","
	// + "Convert(varchar(10),"
	// + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.WarrantyStartDate
	// + ",1) as start_date, "
	// + "Convert(varchar(10),"
	// + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.WarrantyEndDate
	// + ",1) as end_date, "
	// + "Convert(varchar(10),"
	// + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.ValidityStartDate
	// + ",1) as validity_start_date, "
	// + "Convert(varchar(10),"
	// + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.ValidityEndDate
	// + ",1) as validity_end_date, "
	// + "Convert(varchar(10),"
	// + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.UpdateDate
	// + ",1) as update_date, "
	// + DBSchema.Project.TableName
	// + "."
	// + DBSchema.Project.ProjectID
	// + ","
	// + DBSchema.Project.TableName
	// + "."
	// + DBSchema.Project.ProjectCode
	// + ","
	// + DBSchema.Project.TableName
	// + "."
	// + DBSchema.Project.ProjectName
	// + ","
	// + "contract."
	// + DBSchema.Contract.ContractNo
	// + " contractNo, "
	// + "contract."
	// + DBSchema.Contract.ContractName
	// + " contractName,"
	// + "contract."
	// + DBSchema.Contract.Abbreviation
	// + " abbreviation, "
	//
	// + "aCompany."
	// + DBSchema.Company.CompanyName
	// + " aCompany_name, "
	// + "bCompany."
	// + DBSchema.Company.CompanyName
	// + " bCompany_name, "
	// + "aDept."
	// + DBSchema.Department.DepartmentName
	// + " aDept_name, "
	// + "bDept."
	// + DBSchema.Department.DepartmentName
	// + " bDept_name, "
	// + "RTRIM(aEmp."
	// + DBSchema.Employee.FirstName
	// + ")+RTRIM(aEmp."
	// + DBSchema.Employee.LastName
	// + ") aEmp_name, "
	// + "RTRIM(bEmp."
	// + DBSchema.Employee.FirstName
	// + ")+RTRIM(bEmp."
	// + DBSchema.Employee.LastName
	// + ") bEmp_name, "
	// + "RTRIM(cEmp."
	// + DBSchema.Employee.FirstName
	// + ")+RTRIM(cEmp."
	// + DBSchema.Employee.LastName
	// + ") cEmp_name, "
	// + DBSchema.GovernmentContractSource.TableName
	// + "."
	// + DBSchema.GovernmentContractSource.GovermentContractSourceName
	// + ","
	// // + "RTRIM("+DBSchema.ContactPerson.TableName + "." +
	// // DBSchema.ContactPerson.FirstName + ")+RTRIM("
	// // + DBSchema.ContactPerson.TableName + "." +
	// // DBSchema.ContactPerson.LastName + ") as "
	// // + DBSchema.Contract.DeliveryName
	// + "RTRIM(dEmp."
	// + DBSchema.Employee.FirstName
	// + ")+RTRIM(dEmp."
	// + DBSchema.Employee.LastName
	// + ") as "
	// + DBSchema.Contract.DeliveryName
	// + ","
	// + "RTRIM(eEmp."
	// + DBSchema.Employee.FirstName
	// + ")+RTRIM(eEmp."
	// + DBSchema.Employee.LastName
	// + ") as "
	// + DBSchema.Contract.DirectorName
	//
	// +
	// ",dEmp.家庭地址 as delivery_addr,dEmp.单位电话 as delivery_phone,dEmp.家庭邮编 as delivery_postCode,dEmp.职务 as delivery_post,dEmp.移动电话 as delivery_mobilePhone,dEmp.电子邮件 as delivery_email,RTRIM(fEmp.姓氏)+RTRIM(fEmp.名字) as lastModificator "
	//
	// + from
	// + DBSchema.Contract.TableName
	// + ","
	// + DBSchema.CurrencyType.TableName
	// + ","
	// + DBSchema.Project.TableName
	// + ","
	// + DBSchema.Contract.TableName
	// + as
	// + "contract, "
	// + DBSchema.Company.TableName
	// + as
	// + "aCompany, "
	// + DBSchema.Company.TableName
	// + as
	// + "bCompany, "
	// + DBSchema.Department.TableName
	// + as
	// + "aDept, "
	// + DBSchema.Department.TableName
	// + as
	// + "bDept, "
	// + DBSchema.Employee.TableName
	// + as
	// + "aEmp,"
	// + DBSchema.Employee.TableName
	// + as
	// + "bEmp,"
	// + DBSchema.Employee.TableName
	// + as
	// + "cEmp,"
	// + DBSchema.GovernmentContractSource.TableName
	// + ","
	// // + DBSchema.ContactPerson.TableName
	// + DBSchema.Employee.TableName + as
	// + "dEmp,"
	// + DBSchema.Employee.TableName + as + "eEmp,xmis.hr_人员 AS fEmp"
	//
	// + where + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.ContractID + "=?"
	// + and
	// + DBSchema.Contract.TableName + "."
	// + DBSchema.Contract.CurrencyID
	// + "*=" + DBSchema.CurrencyType.TableName
	// + "."
	// + DBSchema.CurrencyType.CurrencyID
	// + and
	// + DBSchema.Project.TableName + "."
	// + DBSchema.Project.ProjectID
	// + " =* " + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.ProjectID + and
	// + DBSchema.Contract.TableName
	// + "." + DBSchema.Contract.ParentID
	// + " *= contract."
	// + DBSchema.Contract.ContractID + and
	// + "aCompany."
	// + DBSchema.Company.CompanyID + " =* "
	// + DBSchema.Contract.TableName
	// + "." + DBSchema.Contract.BuyerID + and
	// + "bCompany."
	// + DBSchema.Company.CompanyID + " =* "
	// + DBSchema.Contract.TableName
	// + "." + DBSchema.Contract.VendorID + and
	// + "aDept."
	// + DBSchema.Department.DepartmentID
	// + " =* "
	// + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.DeptIDOfBuyer + and
	// + "bDept."
	// + DBSchema.Department.DepartmentID
	// + " =* "
	// + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.DeptIDOfVendor + and
	// + "aEmp."
	// + DBSchema.Employee.ID + " =* " + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.BuyerRepresentativeID + and
	// + "bEmp."
	// + DBSchema.Employee.ID + " =* " + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.VendorRepresentativeID + and
	// + "cEmp."
	// + DBSchema.Employee.ID + " =* "
	// + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.RegisterID
	// + and
	// + DBSchema.GovernmentContractSource.TableName
	// + "."
	// + DBSchema.GovernmentContractSource.GovermentContractSourceID
	// + " =* " + DBSchema.Contract.TableName
	// + "."
	// + DBSchema.Contract.GovermentContractSourceID
	// + and
	// + DBSchema.Contract.TableName + "."
	// + DBSchema.Contract.DeliveryID
	// + "*=" + "dEmp." + DBSchema.Employee.ID
	// + and
	// + DBSchema.Contract.TableName + "."
	// + DBSchema.Contract.DirectorID
	// + "*=" + "eEmp."
	// + DBSchema.Employee.ID
	// + " AND xmis.con_合同.最后修改人ID*=fEmp.人员ID";
			+ setNocountOn
			+ " select * into #temp from xmis.con_合同  \n"
			+ " where 合同ID = ? \n"
			+ " update #temp set 合同负责人= xmis.prj_项目.项目经理ID from #temp,xmis.prj_项目 where #temp.项目ID = xmis.prj_项目.项目ID \n"
			+ " "
			+ " SELECT #temp.*,货币名称,Convert(varchar(10),#temp.质保期起,1) as start_date, Convert(varchar(10),#temp.质保期止,1) as end_date, \n"
			+ " Convert(varchar(10),#temp.有效期起,1) as validity_start_date, Convert(varchar(10),#temp.有效期止,1) as validity_end_date, \n"
			+ " Convert(varchar(10),#temp.更新时间,1) as update_date, \n"
			+ " xmis.prj_项目.项目ID,xmis.prj_项目.项目编码,xmis.prj_项目.项目名称, \n"
			+ " contract.合同编号 contractNo, contract.合同名称 contractName,contract.简称 abbreviation,  \n"
			+ " aCompany.公司名称 aCompany_name, bCompany.公司名称 bCompany_name, \n"
			+ " aDept.部门名称 aDept_name, bDept.部门名称 bDept_name, RTRIM(aEmp.姓氏)+RTRIM(aEmp.名字) aEmp_name,  \n"
			+ " RTRIM(bEmp.姓氏)+RTRIM(bEmp.名字) bEmp_name, RTRIM(cEmp.姓氏)+RTRIM(cEmp.名字) cEmp_name, \n"
			+ " xmis.con_横向合同来源.名称, \n"
			+ " RTRIM(dEmp.姓氏)+RTRIM(dEmp.名字) as delivery_name,RTRIM(eEmp.姓氏)+RTRIM(eEmp.名字) as director_name, \n"
			+ " dEmp.家庭地址 as delivery_addr,dEmp.单位电话 as delivery_phone,dEmp.家庭邮编 as delivery_postCode,dEmp.职务 as delivery_post, \n"
			+ " dEmp.移动电话 as delivery_mobilePhone,dEmp.电子邮件 as delivery_email,RTRIM(fEmp.姓氏)+RTRIM(fEmp.名字) as lastModificator  \n"
			+ " FROM #temp,xmis.sys_货币,xmis.prj_项目,xmis.con_合同 AS contract, xmis.org_公司 AS aCompany, xmis.org_公司 AS bCompany, \n"
			+ " xmis.org_部门 AS aDept, xmis.org_部门 AS bDept, xmis.hr_人员 AS aEmp,xmis.hr_人员 AS bEmp,xmis.hr_人员 AS cEmp, \n"
			+ " xmis.con_横向合同来源,xmis.hr_人员 AS dEmp,xmis.hr_人员 AS eEmp,xmis.hr_人员 AS fEmp \n"
			+ " WHERE \n"
			+ " #temp.货币ID*=xmis.sys_货币.货币ID \n"
			+ " AND xmis.prj_项目.项目ID =* #temp.项目ID \n"
			+ " AND #temp.父合同ID *= contract.合同ID \n"
			+ " AND aCompany.公司ID =* #temp.甲方公司ID \n"
			+ " AND bCompany.公司ID =* #temp.乙方公司ID \n"
			+ " AND aDept.部门ID =* #temp.甲方部门ID \n"
			+ " AND bDept.部门ID =* #temp.乙方部门ID \n"
			+ " AND aEmp.人员ID =* #temp.甲方代表ID \n"
			+ " AND bEmp.人员ID =* #temp.乙方代表ID \n"
			+ " AND cEmp.人员ID =* #temp.登记人ID \n"
			+ " AND xmis.con_横向合同来源.横向合同来源ID =* #temp.横向合同来源ID \n"
			+ " AND #temp.发货联系人ID*=dEmp.人员ID \n"
			+ " AND #temp.合同负责人*=eEmp.人员ID  \n"
			+ " AND #temp.最后修改人ID*=fEmp.人员ID \n";

	public static String isHadChildContractListByID = select + "count(*)"
			+ from + DBSchema.Contract.TableName + where
			+ DBSchema.Contract.ParentID + "=?";

	public static String warningString =
	/*
	 * old //
	 * "SELECT xmis.con_收款明细表.合同ID, SUM(xmis.con_收款明细表.收款金额) AS total_amt" + //
	 * "INTO #total" + // "FROM xmis.con_收款明细表" + // "GROUP BY 合同ID" +
	 */
	/*
	 * new select + DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.ContractID + "," +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.PaymentDate + into + "#warning_date" + from +
	 * DBSchema.PaymentCondition.TableName
	 * 
	 * + select + DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.ContractID + "," + "#warning_date." +
	 * DBSchema.PaymentCondition.PaymentDate + " AS warning_date," + "SUM(" +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.PaymentAmt + ") as warning_amt," +
	 * "0 AS warninged_flag, null AS warning_days" + into + "#warning_detail" +
	 * from + DBSchema.PaymentCondition.TableName + ", #warning_date" + where +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.ContractID + " = #warning_date." +
	 * DBSchema.PaymentCondition.ContractID + and + "(" +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.PaymentDate + " <= #warning_date." +
	 * DBSchema.PaymentCondition.PaymentDate
	 * 
	 * + or
	 * 
	 * + "#warning_date." + DBSchema.PaymentCondition.PaymentDate + " is null )"
	 * 
	 * + groupBy + DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.ContractID + ",#warning_date." +
	 * DBSchema.PaymentCondition.PaymentDate
	 * 
	 * + update + "#warning_detail SET " + "warning_amt = warning_amt - " +
	 * DBSchema.ChargedDetail.TotalAmtByID + ", warninged_flag = 1" + from +
	 * "#charged, #warning_detail" + where + "#charged." +
	 * DBSchema.PaymentCondition.ContractID + "= #warning_detail." +
	 * DBSchema.PaymentCondition.ContractID + and +
	 * DBSchema.ChargedDetail.TotalAmtByID + ">=warning_amt"
	 */

	/*
	 * old // select + "合同ID,收款日期" + // " INTO #warning_date" + //
	 * " FROM xmis.con_收款条件表" +
	 * 
	 * // " select xmis.con_收款条件表.合同ID,#warning_date.收款日期 AS warning_date, //
	 * SUM(xmis.con_收款条件表.收款金额) as warning_amt," // + //
	 * "0 AS warninged_flag, 0 AS warning_days" + // " INTO #warning_detail" +
	 * // " FROM xmis.con_收款条件表, #warning_date" + // " WHERE xmis.con_收款条件表.合同ID
	 * = #warning_date.合同ID AND // xmis.con_收款条件表.收款日期<= #warning_date.收款日期" //
	 * + // " GROUP BY xmis.con_收款条件表.合同ID,#warning_date.收款日期" +
	 */
	// " UPDATE #warning_detail SET warning_amt = warning_amt - " +
	// DBSchema.ChargedDetail.TotalAmtByID +", warninged_flag = 1" +
	// " FROM #charged, #warning_detail" +
	// " WHERE #charged.合同ID = #warning_detail.合同ID AND "
	// + DBSchema.ChargedDetail.TotalAmtByID
	// +">=warning_amt"

	" select \n"
			+ " xmis.con_合同.合同ID, \n"
			+ " xmis.fas_glInfo.* \n"
			+ " into #income  \n"
			+ " from  \n"
			+ " xmis.fas_glInfo \n"
			+ " ,xmis.con_合同 \n"
			+ " ,xmis.prj_项目 \n"
			+ " where xmis.fas_glInfo.project_code = xmis.prj_项目.项目编码 \n"
			+ " and xmis.prj_项目.项目ID = xmis.con_合同.项目ID \n"
			+ " UPDATE #warning_detail SET warning_amt = warning_amt - MB, warninged_flag = 1  \n"
			+ " FROM #income, #warning_detail WHERE #income.合同ID = #warning_detail.合同ID AND MB>=warning_amt \n"

			+ "\nUPDATE #warning_detail SET warning_days = "
			+ " \nCASE"
			+ "	\nWHEN DATEDIFF(d,GETDATE(),warning_date)<-30 THEN -30"
			+ "	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>=-30 AND DATEDIFF(d,GETDATE(),warning_date)<-15 THEN -15"
			+ "	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>=-15 AND DATEDIFF(d,GETDATE(),warning_date)<-7 THEN -7"
			+ "	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>=-7 AND DATEDIFF(d,GETDATE(),warning_date)<0 THEN -1"
			+ "	\nWHEN DATEDIFF(d,GETDATE(),warning_date)=0 THEN 1"
			+ "	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>0 AND DATEDIFF(d,GETDATE(),warning_date)<=7 THEN 7"
			+ "	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>7 AND DATEDIFF(d,GETDATE(),warning_date)<=15 THEN 15"
			+ "	\nWHEN DATEDIFF(d,GETDATE(),warning_date)>15 AND DATEDIFF(d,GETDATE(),warning_date)<=30 THEN 30"
			+ "	\nWHEN warning_date is null then NULL"
			+ "	\nELSE DATEDIFF(d,GETDATE(),warning_date)"
			+ " \nEND \n WHERE warninged_flag = 0 \n"

			+ "	INSERT  INTO #warning \n"
			+ "	SELECT  a.合同ID , \n"
			+ "	a.warning_date , \n"
			+ "	a.warning_amt , \n"
			+ "	a.warning_days ,0,0 \n"
			+ "	FROM #warning_detail AS a , \n"
			+ "	( SELECT    合同ID , \n"
			+ "	MAX(warning_date) AS warning_date \n"
			+ "	FROM #warning_detail \n"
			+ " where  warning_days <=30 \n AND"
			+ "  warninged_flag = 0 \n"
			+ "	GROUP BY  合同ID \n"
			+ "	) AS b \n"
			+ "	WHERE a.合同ID = b.合同ID \n"
			+ "	AND a.warning_date = b.warning_date \n"

			+ " SELECT  a.合同ID , \n"
			+ " a.warning_amt as warning_amt \n"
			+ " into #warnall  \n"
			+ " FROM #warning_detail AS a ,  \n"
			+ " ( SELECT    合同ID ,  \n"
			+ " MAX(warning_date) AS warning_date  \n"
			+ " FROM #warning_detail  \n"
			+ " where  \n"
			+ " warninged_flag = 0  \n"
			+ " GROUP BY  合同ID  \n"
			+ " ) AS b  \n"
			+ " WHERE a.合同ID = b.合同ID  \n"
			+ " AND a.warning_date = b.warning_date  \n"

			+ " update #warning set warning_amtPlan=#warnall.warning_amt,warning_bjamt=#warnall.warning_amt - #warning.warning_amt from #warnall where #warning.合同ID = #warnall.合同ID \n"

	/*
	 * + "WHERE warninged_flag = 0" +
	 * 
	 * "			SELECT " + DBSchema.PaymentCondition.ContractID +
	 * ", MAX(warning_date) as warning_date, MAX(warning_amt) as warning_amt, MAX(warning_days) as warning_days"
	 * + "			INTO #warning" + "			FROM #warning_detail" +
	 * "			WHERE warning_days <= 30 AND warninged_flag = 0" + "			GROUP BY " +
	 * DBSchema.PaymentCondition.ContractID
	 * 
	 * // warning info + select + DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.ContractID + ", min(" +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.PaymentDate + ") as warning_date," +
	 * "0 AS warning_amt, 31 AS warning_days " + into + "#warning_info" + from +
	 * DBSchema.PaymentCondition.TableName + ", #warning" + where +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.ContractID + " NOT IN (select " +
	 * DBSchema.PaymentCondition.ContractID + " FROM #warning )" + and +
	 * DBSchema.PaymentCondition.PaymentDate + ">=GETDATE() " + groupBy +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.ContractID
	 * 
	 * + " UPDATE #warning_info SET warning_amt = " +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.PaymentAmt + from +
	 * DBSchema.PaymentCondition.TableName + ", #warning_info " + where +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.ContractID + " = #warning_info." +
	 * DBSchema.PaymentCondition.ContractID + and +
	 * DBSchema.PaymentCondition.TableName + "." +
	 * DBSchema.PaymentCondition.PaymentDate + " = #warning_info.warning_date "
	 * + " INSERT INTO #warning SELECT * FROM #warning_info "
	 */
	/*
	 * old // + // " SELECT xmis.con_收款条件表.合同ID, min(收款日期) AS warning_date, 0 AS
	 * // warning_amt, 31 AS warning_days " // + " INTO #warning_info " // +
	 * " FROM xmis.con_收款条件表, #warning " // + // " WHERE xmis.con_收款条件表.合同ID NOT
	 * IN (SELECT 合同ID FROM #warning) AND // 收款日期>=GETDATE() " // +
	 * " GROUP BY xmis.con_收款条件表.合同ID " // + // " UPDATE #warning_info SET
	 * warning_amt = xmis.con_收款条件表.收款金额 FROM // xmis.con_收款条件表, #warning_info "
	 * // + // " WHERE xmis.con_收款条件表.合同ID = #warning_info.合同ID AND
	 * xmis.con_收款条件表.收款日期 // = #warning_info.warning_date " // +
	 * " INSERT INTO #warning SELECT * FROM #warning_info "
	 */
	;

	// public static String getGLaccvouchBeforeOptimization =
	// // 发生额
	// //
	// " SELECT  * INTO    #glaccvouchAll FROM    xmis.fas_GL_accvouch WHERE xmis.fas_GL_accvouch.iperiod > 0 AND xmis.fas_GL_accvouch.iperiod <= 12"
	// // + " UNION	SELECT  * FROM    xmis.fas_GL_accvouch_v "
	//
	// // " SELECT  DISTINCT        year ,        iperiod ,        ino_id "
	// // + " INTO    #glaccvouch FROM    xmis.fas_GL_accvouch "
	// // +
	// //
	// " WHERE  xmis.fas_GL_accvouch.iperiod > 0 AND xmis.fas_GL_accvouch.iperiod <= 12 and "
	// // + " not (year = 2011 AND iperiod = 8 AND ino_id IN (327,466)) "
	//
	// " SELECT  DISTINCT        xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id "
	// + " INTO    #income_vouch FROM    xmis.fas_GL_accvouch "// ,
	// // #glaccvouch
	// // "
	// // +
	// //
	// " WHERE   xmis.fas_GL_accvouch.year = #glaccvouch.year         AND xmis.fas_GL_accvouch.iperiod = #glaccvouch.iperiod "
	// // + " AND xmis.fas_GL_accvouch.ino_id = #glaccvouch.ino_id "
	// +
	// " WHERE  xmis.fas_GL_accvouch.iperiod > 0 AND xmis.fas_GL_accvouch.iperiod <= 12 and "
	// + " not (year = 2011 AND iperiod = 8 AND ino_id IN (327,466)) "
	// + " CREATE INDEX i_t_incom_yii ON #income_vouch(year,iperiod,ino_id) "
	//
	// +
	// " SELECT  xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id , "
	// +
	// " SUM(mc) - SUM(md) income_amt ,        project_code ,        citem_id  "
	// +
	// " INTO    #income  FROM    xmis.fas_GL_accvouch ,        #income_vouch ,        xmis.fas_fitemss00 ,        xmis.fas_codeType "
	// // +
	// //
	// " WHERE   xmis.fas_GL_accvouch.year <> 2011 and xmis.fas_GL_accvouch.iperiod <> 8 and xmis.fas_GL_accvouch.ino_id not in( 466 , 327) and "
	// +
	// " where xmis.fas_GL_accvouch.year = xmis.fas_fitemss00.year        AND xmis.fas_GL_accvouch.citem_id = xmis.fas_fitemss00.citemcode "
	// +
	// " AND xmis.fas_fitemss00.bclose = 0        AND xmis.fas_GL_accvouch.year = #income_vouch.year "
	// +
	// " AND xmis.fas_GL_accvouch.iperiod = #income_vouch.iperiod        AND xmis.fas_GL_accvouch.ino_id = #income_vouch.ino_id "
	// +
	// " AND xmis.fas_codeType.type = 0        AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode "
	// + " AND project_code IS NOT NULL "
	// +
	// " GROUP BY xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id ,"
	// + " project_code ,        citem_id 			"
	//
	// // 来款期初余额
	// +
	// " SELECT  citem_id ,        CASE WHEN cbegind_c LIKE '借' THEN -1 * mb             ELSE 0        END AS income_mb "
	// + " INTO    #qch	FROM    xmis.fas_GL_accass ,        xmis.fas_codeType "
	// +
	// " WHERE   year = 2008        AND iperiod = 1        AND citem_class = 00        AND xmis.fas_codeType.type = 0 "
	// + " AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode "
	// + " UNION ALL "
	// +
	// " SELECT  citem_id ,        CASE WHEN cbegind_c LIKE '贷' THEN mb             ELSE 0        END "
	// + " FROM    xmis.fas_GL_accass ,        xmis.fas_codeType "
	// +
	// " WHERE   year = 2008        AND iperiod = 1        AND citem_class = 00        AND xmis.fas_codeType.type = 0 "
	// + " AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode "
	// + " UNION ALL "
	// + " SELECT  citem_id ,        SUM(mc) "
	// + " FROM    xmis.fas_GL_accvouch "
	// +
	// " WHERE   xmis.fas_GL_accvouch.iperiod > 0 AND xmis.fas_GL_accvouch.iperiod <= 12 and "
	// + " ccode = '51010102' or (year = 2011 and iperiod = 8 and ino_id = 466)"
	// + " group by citem_id "
	//
	// // 采购支出期初余额
	// +
	// " SELECT  citem_id ,        CASE WHEN cbegind_c LIKE '借' THEN  mb             ELSE 0        END AS payout_mb "
	// +
	// " INTO    #qcjcb		FROM    xmis.fas_GL_accass ,        xmis.fas_codeType "
	// +
	// " WHERE   year = 2008        AND iperiod = 1        AND citem_class = 00        AND xmis.fas_codeType.type = 1 "
	// + " AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode "
	// + " UNION ALL "
	// +
	// " SELECT  citem_id ,        CASE WHEN cbegind_c LIKE '贷' THEN -1 * mb             ELSE 0        END "
	// + " FROM    xmis.fas_GL_accass ,        xmis.fas_codeType "
	// +
	// " WHERE   year = 2008        AND iperiod = 1        AND citem_class = 00        AND xmis.fas_codeType.type = 1 "
	// + " AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode "
	// + " UNION ALL "
	// +
	// " select citem_id, sum(md) from xmis.fas_GL_accvouch where xmis.fas_GL_accvouch.iperiod > 0 AND xmis.fas_GL_accvouch.iperiod <= 12 and "
	// + " ccode in( '41010187','4101012033') or "
	// + " (year = 2011 and iperiod = 8 and ino_id = 327) group by citem_id"
	//
	// ;

	// public static String getGLaccvouch =
	//
	// // 发生额
	//
	// "\n select * into #income from xmis.fas_income "
	//
	// // "\n--发生额"
	// // +
	// "\n SELECT  xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id , "
	// // + "citem_id,   SUM(mc) - SUM(md) income_amt "
	// // +
	// " INTO    #income1  FROM    xmis.fas_GL_accvouch( INDEX=i_GLaccvouch_yiicc ) , xmis.fas_codeType "
	// // +
	// " WHERE  xmis.fas_GL_accvouch.iperiod > 0 AND xmis.fas_GL_accvouch.iperiod <= 12 "
	// // +
	// " AND NOT ( year = 2011 AND iperiod = 8 AND ino_id IN ( 327, 466 )  ) "
	// // +
	// " AND xmis.fas_codeType.type = 0        AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode "
	// // +
	// " GROUP BY xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id ,"
	// // + " citem_id 			"
	// //
	// // +
	// " SELECT  #income1.* ,                xmis.fas_fitemss00.project_code        INTO    #income "
	// // +
	// " FROM    #income1 ,                xmis.fas_fitemss00        WHERE   #income1.year = xmis.fas_fitemss00.year "
	// // +
	// " AND #income1.citem_id = xmis.fas_fitemss00.citemcode                AND xmis.fas_fitemss00.bclose = 0 "
	// // + " AND project_code IS NOT NULL "
	//
	// +
	// " CREATE INDEX i_t_income_yiic ON #income(year,iperiod,ino_id,citem_id, project_code) "
	//
	// // 来款期初余额
	// + "\n select * into #qch from xmis.fas_qch "
	//
	// // + "\n--来款期初余额"
	// // +
	// "\n SELECT  citem_id ,        CASE WHEN cbegind_c LIKE '借' THEN -1 * mb             ELSE 0        END AS income_mb "
	// // +
	// " INTO    #qch	FROM    xmis.fas_GL_accass ,        xmis.fas_codeType "
	// // +
	// " WHERE   year = 2008        AND iperiod = 1        AND citem_class = 00        AND xmis.fas_codeType.type = 0 "
	// // + " AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode "
	// // + " UNION ALL "
	// // +
	// " SELECT  citem_id ,        CASE WHEN cbegind_c LIKE '贷' THEN mb             ELSE 0        END "
	// // + " FROM    xmis.fas_GL_accass ,        xmis.fas_codeType "
	// // +
	// " WHERE   year = 2008        AND iperiod = 1        AND citem_class = 00        AND xmis.fas_codeType.type = 0 "
	// // + " AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode "
	// // + " UNION ALL "
	// // + " SELECT  citem_id ,        SUM(mc) "
	// // + " FROM    xmis.fas_GL_accvouch "
	// // +
	// " WHERE   xmis.fas_GL_accvouch.iperiod > 0 AND xmis.fas_GL_accvouch.iperiod <= 12 and "
	// // +
	// " ccode = '51010102' or (year = 2011 and iperiod = 8 and ino_id = 466)"
	// // + " group by citem_id "
	//
	// // 采购支出期初余额
	// + "\n select * into #qcjcb from xmis.fas_qcjcb"
	//
	// // + "\n--采购支出期初余额"
	// // +
	// "\n SELECT  citem_id ,        CASE WHEN cbegind_c LIKE '借' THEN  mb             ELSE 0        END AS payout_mb "
	// // +
	// " INTO    #qcjcb		FROM    xmis.fas_GL_accass ,        xmis.fas_codeType "
	// // +
	// " WHERE   year = 2008        AND iperiod = 1        AND citem_class = 00        AND xmis.fas_codeType.type = 1 "
	// // + " AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode "
	// // + " UNION ALL "
	// // +
	// " SELECT  citem_id ,        CASE WHEN cbegind_c LIKE '贷' THEN -1 * mb             ELSE 0        END "
	// // + " FROM    xmis.fas_GL_accass ,        xmis.fas_codeType "
	// // +
	// " WHERE   year = 2008        AND iperiod = 1        AND citem_class = 00        AND xmis.fas_codeType.type = 1 "
	// // + " AND xmis.fas_GL_accass.ccode = xmis.fas_codeType.ccode "
	// // + " UNION ALL "
	// // +
	// " select citem_id, sum(md) from xmis.fas_GL_accvouch( INDEX=i_GLaccvouch_cc ) "
	// // +
	// " where xmis.fas_GL_accvouch.iperiod > 0 AND xmis.fas_GL_accvouch.iperiod <= 12 and ccode in( '41010187','4101012033') "
	// // + " GROUP BY citem_id "
	// // + " UNION ALL "
	// // +
	// " SELECT  citem_id ,                SUM(md)         FROM    xmis.fas_GL_accvouch ( INDEX=i_GLaccvouch_yii ) "
	// // +
	// " WHERE   ( year = 2011  AND iperiod = 8  AND ino_id = 327  )   GROUP BY citem_id "
	//
	// ;

	// public static String getGLIncomeByProjectCode =
	//
	// // getGLaccvouch
	//
	// " \nSELECT  2008 as year, convert(int, null) as iperiod, convert(int, null) as ino_id, SUM(income_mb) AS MB, "
	// +
	// " '2008期初余额' as cdigest, convert(datetime, null) as dbill_date , project_code  "
	// + intoResult
	// // + " FROM    #qch ,        xmis.fas_fitemss00 ,"
	// + " from xmis.fas_qch, xmis.fas_fitemss00, "
	// + DBSchema.Project.TableName
	// +
	// " WHERE  xmis.fas_fitemss00.year = 2008 and  xmis.fas_qch.citem_id = xmis.fas_fitemss00.citemcode and project_code = "
	// + DBSchema.Project.ProjectCode
	// + " and "
	// + DBSchema.Project.ProjectID
	// + " = ? "
	// + " GROUP BY project_code "
	// + " UNION ALL "
	// +
	// " SELECT  xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id , "
	// +
	// " SUM(mc) - SUM(md) ,        xmis.fas_GL_accvouch.cdigest ,        xmis.fas_GL_accvouch.dbill_date ,        xmis.fas_income.project_code "
	// // +
	// //
	// " FROM    xmis.fas_GL_accvouch ,        #income ,        xmis.fas_codeType ,"
	// +
	// " FROM    xmis.fas_GL_accvouch ,        xmis.fas_income ,        xmis.fas_codeType ,"
	// + DBSchema.Project.TableName
	// +
	// " WHERE   xmis.fas_GL_accvouch.year = xmis.fas_income.year        AND xmis.fas_GL_accvouch.iperiod = xmis.fas_income.iperiod "
	// +
	// " AND xmis.fas_GL_accvouch.ino_id = xmis.fas_income.ino_id        AND xmis.fas_GL_accvouch.citem_id = xmis.fas_income.citem_id "
	// +
	// " AND xmis.fas_codeType.type = 0        AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode  AND xmis.fas_GL_accvouch.year = xmis.fas_codeType.year"
	// + " and xmis.fas_income.project_code = "
	// + DBSchema.Project.ProjectCode
	// + " and "
	// + DBSchema.Project.ProjectID
	// + " = ? "
	// +
	// " GROUP BY xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id , "
	// +
	// " xmis.fas_GL_accvouch.dbill_date ,        cdigest ,        xmis.fas_income.project_code "
	// +
	// " HAVING  SUM(mc) - SUM(md) <> 0 ORDER BY xmis.fas_GL_accvouch.dbill_date ";

	public static String getGLIncomeByProjectCode =

	// getGLaccvouch
	" DECLARE @prjectID NVARCHAR(100) "
			+ "SELECT @prjectID = ?, @prjectID = ? "

			+ "        SELECT  CONVERT(INT, null) AS year ,                CONVERT(INT, NULL) AS iperiod ,"
			+ "                CONVERT(INT, NULL) AS ino_id ,                CONVERT(DECIMAL(11,2),NULL) AS MB ,"
			+ "                CONVERT(NVARCHAR(100),NULL) AS cdigest ,                CONVERT(DATETIME, NULL) AS dbill_date ,"
			+ "                CONVERT(NVARCHAR(100),NULL)project_code"
			+ intoResult
			+ "        WHERE 1=2"

			+ "\nIF EXISTS ( SELECT  DISTINCT project_code FROM    xmis.fas_项目金额调整表 adjusted , xmis.fas_fitemss00 , xmis.prj_项目  "
			+ "\nWHERE   xmis.fas_fitemss00.year = 2010                   AND adjusted.citem_id = citemcode                    AND project_code = xmis.prj_项目.项目编码"
			+ "\nAND 项目ID = @prjectID ) "
			+ "\nBEGIN"
			+ "\n insert into #result "
			+ "SELECT  2010 as year, convert(int, null) as iperiod, convert(int, null) as ino_id, adjusted_mb AS MB, "
			+ " '2010.09项目清理后,结算余额' as cdigest, convert(datetime, null) as dbill_date , project_code  "
			+ " from xmis.fas_项目金额调整表, xmis.fas_fitemss00, "
			+ DBSchema.Project.TableName
			+ " WHERE  xmis.fas_fitemss00.year = 2010 and  xmis.fas_项目金额调整表.citem_id = xmis.fas_fitemss00.citemcode and project_code = "
			+ DBSchema.Project.ProjectCode
			+ " and "
			+ DBSchema.Project.ProjectID
			+ " = @prjectID "
			+ "\nEND"
			+ "\nELSE"
			+ "\n    BEGIN"
			+

			"\n insert into #result "
			+ "SELECT  2008 as year, convert(int, null) as iperiod, convert(int, null) as ino_id, SUM(income_mb) AS MB, "
			+ " '2008期初余额' as cdigest, convert(datetime, null) as dbill_date , project_code  "
			+ " from xmis.fas_qch, xmis.fas_fitemss00, "
			+ DBSchema.Project.TableName
			+ " WHERE  xmis.fas_fitemss00.year = 2008 and  xmis.fas_qch.citem_id = xmis.fas_fitemss00.citemcode and project_code = "
			+ DBSchema.Project.ProjectCode
			+ " and "
			+ DBSchema.Project.ProjectID
			+ " = @prjectID "
			+ " GROUP BY project_code "
			+ " UNION ALL "
			+ " SELECT  xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id , "
			+ " SUM(mc) - SUM(md) ,        xmis.fas_GL_accvouch.cdigest ,        xmis.fas_GL_accvouch.dbill_date ,        xmis.fas_income.project_code "
			+ " FROM    xmis.fas_GL_accvouch ,        xmis.fas_income ,        xmis.fas_codeType ,"
			+ DBSchema.Project.TableName
			+ " WHERE   xmis.fas_GL_accvouch.year = xmis.fas_income.year        AND xmis.fas_GL_accvouch.iperiod = xmis.fas_income.iperiod "
			+ " AND xmis.fas_GL_accvouch.ino_id = xmis.fas_income.ino_id        AND xmis.fas_GL_accvouch.citem_id = xmis.fas_income.citem_id "
			+ " AND xmis.fas_codeType.type in(0,102)        AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode  AND xmis.fas_GL_accvouch.year = xmis.fas_codeType.year"
			+ " and xmis.fas_income.project_code = "
			+ DBSchema.Project.ProjectCode
			+ " and "
			+ DBSchema.Project.ProjectID
			+ " = @prjectID "
			+ " GROUP BY xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id , "
			+ " xmis.fas_GL_accvouch.dbill_date ,        cdigest ,        xmis.fas_income.project_code "
			+ " HAVING  SUM(mc) - SUM(md) <> 0 ORDER BY xmis.fas_GL_accvouch.dbill_date "
			+ "\n    END";

	public static String getGLElseIncomeAmtByProjectID = "DECLARE @prjectID NVARCHAR(100) SELECT @prjectID = ?, @prjectID = ? \n"

			+ " SELECT xmis.fas_GL_accvouch.year , \n"
			+ " xmis.fas_GL_accvouch.iperiod , \n"
			+ " xmis.fas_GL_accvouch.ino_id , \n"
			+ " SUM(mc) - SUM(md) AS MB , \n"
			+ " xmis.fas_GL_accvouch.cdigest , \n"
			+ " xmis.fas_GL_accvouch.dbill_date , \n"
			+ " xmis.fas_income.project_code \n"
			+ " into #result \n"
			+ " FROM   xmis.fas_GL_accvouch , \n"
			+ " xmis.fas_income , \n"
			+ " xmis.fas_codeType , \n"
			+ " xmis.prj_项目 \n"
			+ " WHERE  xmis.fas_GL_accvouch.year = xmis.fas_income.year \n"
			+ " AND xmis.fas_GL_accvouch.iperiod = xmis.fas_income.iperiod \n"
			+ " AND xmis.fas_GL_accvouch.ino_id = xmis.fas_income.ino_id \n"
			+ " AND xmis.fas_GL_accvouch.citem_id = xmis.fas_income.citem_id \n"
			+ " AND xmis.fas_codeType.type = ? \n"
			+ " AND xmis.fas_income.type = xmis.fas_codeType.type \n"
			+ " AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode \n"
			+ " AND xmis.fas_GL_accvouch.year = xmis.fas_codeType.year \n"
			+ " AND xmis.fas_income.project_code = 项目编码 \n"
			+ " AND 项目ID = @prjectID \n"
			+ " GROUP BY xmis.fas_GL_accvouch.year , \n"
			+ " xmis.fas_GL_accvouch.iperiod , \n"
			+ " xmis.fas_GL_accvouch.ino_id , \n"
			+ " xmis.fas_GL_accvouch.dbill_date , \n"
			+ " cdigest , \n"
			+ " xmis.fas_income.project_code \n"
			+ " HAVING SUM(mc) - SUM(md) <> 0 \n"
			+ " ORDER BY xmis.fas_GL_accvouch.dbill_date  \n";

	public static String getGLPurchaseByProjectCode =

	// getGLaccvouch

	" \nSELECT  2008 as year, convert(int, null) as iperiod, convert(int, null) as ino_id, SUM(payout_mb) AS MB, "
			+ " '2008期初余额' as cdigest, convert(datetime, null) as dbill_date , project_code  "
			+ intoResult
			// + " FROM    #qcjcb ,        xmis.fas_fitemss00 ,"
			+ " FROM    xmis.fas_qcjcb ,        xmis.fas_fitemss00 ,"
			+ DBSchema.Project.TableName
			+ " WHERE   xmis.fas_fitemss00.year = 2008 and  xmis.fas_qcjcb.citem_id = xmis.fas_fitemss00.citemcode and project_code = "
			+ DBSchema.Project.ProjectCode
			+ " and "
			+ DBSchema.Project.ProjectID
			+ " = ? "
			+ " GROUP BY project_code "
			+ " UNION ALL "
			+ " \nSELECT  xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id ,         SUM(md - mc) pay_amt , "
			+ " xmis.fas_GL_accvouch.cdigest ,        xmis.fas_GL_accvouch.dbill_date ,        project_code  "
			// + " INTO #purchaseDetail "
			// +
			// " FROM    xmis.fas_GL_accvouch ,        #income_vouch ,        xmis.fas_fitemss00 ,        xmis.fas_codeType ,"
			+ " FROM    xmis.fas_GL_accvouch ,        xmis.fas_fitemss00 ,        xmis.fas_codeType ,"
			+ DBSchema.Project.TableName
			+ " WHERE   xmis.fas_GL_accvouch.year = xmis.fas_fitemss00.year        AND xmis.fas_GL_accvouch.citem_id = xmis.fas_fitemss00.citemcode "
			+ " AND xmis.fas_fitemss00.bclose = 0        "

			+ " AND xmis.fas_GL_accvouch.iperiod > 0                AND xmis.fas_GL_accvouch.iperiod <= 12 "
			+ " AND NOT ( fas_GL_accvouch.year = 2011  AND fas_GL_accvouch.iperiod = 8  AND fas_GL_accvouch.ino_id IN ( 327, 466 ) )"

			// + " AND xmis.fas_GL_accvouch.year = #income_vouch.year "
			// +
			// " AND xmis.fas_GL_accvouch.iperiod = #income_vouch.iperiod        AND xmis.fas_GL_accvouch.ino_id = #income_vouch.ino_id "
			+ " AND xmis.fas_codeType.type = 1        AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode AND xmis.fas_GL_accvouch.year = xmis.fas_codeType.year"
			+ " AND project_code = "
			+ DBSchema.Project.ProjectCode
			+ " and "
			+ DBSchema.Project.ProjectID
			+ " = ? "
			+ " GROUP BY xmis.fas_GL_accvouch.year ,        xmis.fas_GL_accvouch.iperiod ,        xmis.fas_GL_accvouch.ino_id ,        project_code , "
			+ " citem_id ,        xmis.fas_GL_accvouch.cdigest ,        xmis.fas_GL_accvouch.dbill_date "
			+ " HAVING  SUM(md - mc) <> 0	ORDER BY xmis.fas_GL_accvouch.dbill_date ";

	// public static String getGLInfoByContractIDBeforeOptimization =
	// " select #sumInfo.*, CONVERT(DECIMAL(18,2),0) AS MB, CONVERT(DECIMAL(18,2),0) AS unMB, CONVERT(DECIMAL(18,2),0) AS pay_amt "
	// + " into #result " + " from #sumInfo where 1=2"
	//
	// + " if (@contractType = 1) begin "
	// + getGLaccvouch
	//
	// // 来款合计
	// + " SELECT  project_code ,        SUM(income_mb) AS MB "
	// + " into #gl"
	// + " FROM    #qch ,        xmis.fas_fitemss00 "
	// +
	// " WHERE   xmis.fas_fitemss00.year = 2008 and #qch.citem_id = xmis.fas_fitemss00.citemcode "
	// + " GROUP BY project_code "
	// + " UNION ALL "
	// + " SELECT  #income.project_code, SUM(mc) - SUM(md) "
	// +
	// " FROM    xmis.fas_GL_accvouch ,        #income ,        xmis.fas_codeType "
	// +
	// " WHERE   xmis.fas_GL_accvouch.year = #income.year        AND xmis.fas_GL_accvouch.iperiod = #income.iperiod "
	// +
	// " AND xmis.fas_GL_accvouch.ino_id = #income.ino_id        AND xmis.fas_GL_accvouch.citem_id = #income.citem_id "
	// +
	// " AND xmis.fas_codeType.type = 0        AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode "
	// + " GROUP BY #income.project_code			HAVING  SUM(income_amt) <> 0 "
	//
	// +
	// " select project_code, sum(MB) MB into #glTotal from #gl group by project_code "
	//
	// // 采购支出合计
	// + " SELECT  project_code ,        SUM(payout_mb) AS pay_amt "
	// + " INTO #purchase "
	// + " FROM    #qcjcb ,        xmis.fas_fitemss00 "
	// +
	// " WHERE   xmis.fas_fitemss00.year = 2008 and #qcjcb.citem_id = xmis.fas_fitemss00.citemcode "
	// + " GROUP BY project_code "
	// + " UNION ALL "
	// + " SELECT  project_code , SUM(md - mc) "
	// +
	// " FROM    xmis.fas_GL_accvouch ,        #income_vouch ,        xmis.fas_fitemss00 ,        xmis.fas_codeType "
	// +
	// " WHERE   xmis.fas_GL_accvouch.year = xmis.fas_fitemss00.year        AND xmis.fas_GL_accvouch.citem_id = xmis.fas_fitemss00.citemcode "
	// +
	// " AND xmis.fas_fitemss00.bclose = 0        AND xmis.fas_GL_accvouch.year = #income_vouch.year "
	// +
	// " AND xmis.fas_GL_accvouch.iperiod = #income_vouch.iperiod        AND xmis.fas_GL_accvouch.ino_id = #income_vouch.ino_id "
	// +
	// " AND xmis.fas_codeType.type = 1        AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode "
	// + " AND project_code IS NOT NULL "
	// + " GROUP BY project_code HAVING  SUM(md - mc) <> 0 "
	//
	// +
	// " select project_code, sum(pay_amt) pay_amt into #purchaseTotal from #purchase group by project_code "
	//
	// +
	// " select #glTotal.project_code, MB, pay_amt into #glInfo from #glTotal, #purchaseTotal "
	// + " where #glTotal.project_code *= #purchaseTotal.project_code "
	//
	// + " insert into #result "
	//
	// + " select #sumInfo.*, case when MB is null then 0 else MB end as MB, "
	// + DBSchema.Contract.ContractAmt
	// + " - case when MB is null then 0 else MB end as unMB, "
	// + " case when pay_amt is null then 0 else pay_amt end as pay_amt "
	// // + " into #result "
	// + " from #sumInfo, #glInfo "
	// + " where #sumInfo."
	// + DBSchema.Project.ProjectCode
	// + " *= #glInfo.project_code"
	//
	// + " end else begin "
	// + " insert into #result "
	// +
	// " select #sumInfo.*, CONVERT(DECIMAL(9,2),0) AS MB, CONVERT(DECIMAL(9,2),0) AS unMB, CONVERT(DECIMAL(9,2),0) AS pay_amt  "
	// // + " into #result "
	// + " from #sumInfo " + " end ";

	// used now
	public static String getGLInfoByContractID = " select #sumInfo.*, CONVERT(DECIMAL(18,2),0) AS MB,  CONVERT (DECIMAL(18,2),0) AS elseMB,CONVERT(INT, NULL) AS bcloseflag ,CONVERT(DECIMAL(18,2),0) AS difMB,CONVERT(DECIMAL(18,2),0) AS unMB, CONVERT(DECIMAL(18,2),0) AS pay_amt, CONVERT(DECIMAL(18, 2), 0) AS 按进度已开票未到款 ,CONVERT(DECIMAL(18, 2), 0) AS 按进度未开票应收款  \n"
			+ " into #result  \n"
			+ " from #sumInfo where 1=2  \n"

			+ " if (@contractType = 1) begin  \n"

			// + getGLaccvouch

			// //来款合计 "
			// + "\n-- 来款合计 "
			// + "\n SELECT  project_code ,        SUM(income_mb) AS MB "
			// + " into #gl"
			// + " FROM    #qch ,        xmis.fas_fitemss00 "
			// +
			// " WHERE   xmis.fas_fitemss00.year = 2008 and #qch.citem_id = xmis.fas_fitemss00.citemcode "
			// + " GROUP BY project_code "
			// + " UNION ALL "
			// + " SELECT  #income.project_code, SUM(#income.income_amt) "
			// + " FROM    #income "
			// +
			// " GROUP BY #income.project_code			HAVING  SUM(income_amt) <> 0 "
			//
			// +
			// " select project_code, sum(MB) MB into #glTotal from #gl group by project_code "
			//
			// //采购支出合计 "
			// + "\n--  采购支出合计 "
			// +
			// "\n SELECT                  year ,                citem_id ,                SUM(md - mc) AS pay_amt "
			// + " INTO    #purchase1        "
			// +
			// " FROM    xmis.fas_GL_accvouch ( INDEX=i_GLaccvouch_yii ) ,                xmis.fas_codeType "
			// +
			// " WHERE   xmis.fas_GL_accvouch.iperiod > 0                AND xmis.fas_GL_accvouch.iperiod <= 12 "
			// +
			// " AND NOT ( year = 2011 AND iperiod = 8 AND ino_id IN ( 327, 466 ) ) "
			// +
			// " AND xmis.fas_codeType.type = 1                AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode "
			// +
			// " GROUP BY YEAR ,                citem_id         HAVING  SUM(md - mc) <> 0 "
			//
			// + " SELECT  project_code ,  SUM(payout_mb) AS pay_amt "
			// + " INTO #purchase "
			// + " FROM    #qcjcb ,        xmis.fas_fitemss00 "
			// +
			// " WHERE   xmis.fas_fitemss00.year = 2008 and #qcjcb.citem_id = xmis.fas_fitemss00.citemcode "
			// + " GROUP BY project_code "
			// + " UNION ALL "
			// + " SELECT  project_code ,                SUM(pay_amt) "
			// + " FROM    #purchase1 ,                xmis.fas_fitemss00 "
			// +
			// " WHERE   #purchase1.year = xmis.fas_fitemss00.year                AND #purchase1.citem_id = xmis.fas_fitemss00.citemcode "
			// +
			// " AND xmis.fas_fitemss00.bclose = 0                AND project_code IS NOT NULL        GROUP BY project_code "
			//
			// +
			// " select project_code, sum(pay_amt) pay_amt into #purchaseTotal from #purchase group by project_code "
			//
			// +
			// " select #glTotal.project_code, MB, pay_amt into #glInfo from #glTotal, #purchaseTotal "
			// + " where #glTotal.project_code *= #purchaseTotal.project_code "

			// + "\n select * into #glInfo from xmis.fas_glInfo "

			// + "\n----------------"

			+ "\n insert into #result  \n"
			+ " select #sumInfo.*, case when MB is null then 0 else MB end as MB,  \n"
			+ " 其它财务来款   as elseMB, \n"
			+ " bcloseflag,  \n"
			+ " case when MB is null then 0 else MB end - 收款合计  as difMB, \n"
			+ DBSchema.Contract.ContractAmt
			+ " - case when MB is null then 0 else MB end as unMB,  \n"
			+ " case when pay_amt is null then 0 else pay_amt end as pay_amt ,0 ,0  \n"
			// + " into #result "
			+ " from #sumInfo, xmis.fas_glInfo  \n"
			+ " where #sumInfo."
			+ DBSchema.Project.ProjectCode
			+ " *= xmis.fas_glInfo.project_code  \n"

			+ " --根据财务实际到款，计算报警金额, \n"
			+ " UPDATE #result \n"
			+ " SET 按进度已开票未到款 = CASE WHEN ( warning_amtPlan - MB ) >= 已开票未到账金额合计 \n"
			+ " THEN 已开票未到账金额合计 \n"
			+ " ELSE CASE WHEN warning_amtPlan > MB \n"
			+ " THEN warning_amtPlan - MB \n"
			+ " ELSE 0 \n"
			+ " END \n"
			+ " END , \n"
			+ " 按进度未开票应收款 = CASE WHEN ( warning_amtPlan - MB ) >= 已开票未到账金额合计 \n"
			+ " THEN ( CASE WHEN warning_amtPlan > MB \n"
			+ " THEN warning_amtPlan - MB \n"
			+ " ELSE 0 \n"
			+ " END ) - 已开票未到账金额合计 \n"
			+ " ELSE 0 \n"
			+ " END , \n"
			+ " warning_amt = CASE WHEN warning_amtPlan > MB THEN warning_amtPlan - MB \n"
			+ " ELSE 0 \n"
			+ " END ,\n"
			+ " warning_bjamt = CASE WHEN warning_amtPlan-MB > warning_bjamt THEN warning_amtPlan - MB - warning_bjamt \n"
			+ " ELSE 0 \n"
			+ " END \n"

			// 合同没有业务数据，以财务数据进行初始化
			// + "\n-- 合同没有业务数据，以财务数据进行初始化"
			// + " \n ALTER TABLE #result ADD bizFlg INT"
			// +
			// " \n UPDATE #result SET 收款合计 = MB, 未收款金额合计 = unMB WHERE 收款合计 = 0 "
			// // ,
			// bizFlg
			// =
			// (
			// CASE
			// WHEN
			// MB
			// >
			// 收款合计
			// THEN
			// 1
			// ELSE
			// 0
			// END
			// )
			// "
			// + " WHERE 收款合计 = 0 "

			+ " end else begin  \n"
			+ " insert into #result  \n"
			+ " select #sumInfo.*, CONVERT(DECIMAL(9,2),0) AS MB,   "
			+ "CONVERT(DECIMAL(9,2),0) AS elseMB,"
			+ "CONVERT(INT, NULL) AS bcloseflag ,"
			+ "CONVERT(DECIMAL(18,2),0) AS difMB,"
			+ "CONVERT(DECIMAL(9,2),0) AS unMB, "
			+ "CONVERT(DECIMAL(9,2),0) AS pay_amt,0,0  \n"
			// + " into #result "
			+ " from #sumInfo  \n"

			// + "--支出合同物资状态--20140317"
			// + "UPDATE  #sumInfo\n"
			// + "SET     bcloseflag = b.bflag\n"
			// + "FROM    #sumInfo ,\n"
			// + "        ( SELECT    a.支出合同ID ,\n"
			// + "                    SUM(a.flag) AS bflag\n"
			// + "          FROM      ( SELECT    xmis.dev_采购清单明细.支出合同ID ,\n"
			// + "                                项目物资ID ,\n"
			// + "                                SUM(数量) - SUM(到货数量) qty ,\n"
			// +
			// "                                CASE WHEN ( SUM(数量) <= SUM(到货数量) )\n"
			// + "                                     THEN 0\n"
			// + "                                     ELSE 1\n"
			// + "                                END AS flag\n"
			// + "                      FROM      xmis.dev_采购清单明细 ,\n"
			// + "                                xmis.dev_到货清单\n"
			// +
			// "                      WHERE     xmis.dev_到货清单.支出合同ID = xmis.dev_采购清单明细.支出合同ID\n"
			// + "                                AND 物资ID = 项目物资ID\n"
			// + "                      GROUP BY  xmis.dev_采购清单明细.支出合同ID ,\n"
			// + "                                项目物资ID\n"
			// + "                    ) AS a\n"
			// + "          GROUP BY  a.支出合同ID\n"
			// + "        ) AS b\n" + "WHERE   #sumInfo.合同ID = b.支出合同ID"

			+ " end  \n";

	private static String getSumInfoByContractID =
	// get total_amt by contractID
	select
			+ DBSchema.ChargedDetail.ContractID
			+ ","
			+ "SUM("
			+ DBSchema.ChargedDetail.ChargedAmt
			+ ") as "
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ into
			+ "#charged"
			+ from
			+ DBSchema.ChargedDetail.TableName
			+ groupBy
			+ DBSchema.ChargedDetail.ContractID

			// get invoice_amt by contractID
			+ select
			+ DBSchema.InvoiceDetail.ContractID
			+ ","
			+ "SUM("
			+ DBSchema.InvoiceDetail.InvoiceAmt
			+ ") as "
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ into
			+ "#invoice"
			+ from
			+ DBSchema.InvoiceDetail.TableName
			+ groupBy
			+ DBSchema.InvoiceDetail.ContractID

			// get Receipt_amt by contractID
			+ select
			+ DBSchema.Receipt.ContractID
			+ ","
			+ "SUM("
			+ DBSchema.Receipt.ReceiptAmt
			+ ") as "
			+ DBSchema.Receipt.TotalAmtByID
			+ into
			+ "#receipt"
			+ from
			+ DBSchema.Receipt.TableName
			+ groupBy
			+ DBSchema.Receipt.ContractID

			+ " select reason.*,RTRIM(a.姓氏)+RTRIM(a.名字) as 经办人,RTRIM(b.姓氏)+RTRIM(b.名字) as 登记人 INTO #delay from [xmis].[con_付款推迟原因] as reason \n"
			+ " left outer join xmis.hr_人员 as a on reason.经办人ID = a.人员ID \n"
			+ " left outer join xmis.hr_人员 as b on reason.登记人ID = b.人员ID  \n"
			+ " where ID in ( \n"
			+ " SELECT MAX(ID) \n"
			+ " FROM [xmis].[con_付款推迟原因] group by 合同id) \n"

			+ "\n -- get prject info & paymentcondition base On ProjectPlan \n"
			+ " IF ( @contractType = 1 )  \n"
			+ " BEGIN \n"
			+ " INSERT  INTO #contractList \n"
			+ " SELECT  contract.* , \n"
			+ " null, \n"
			+ " null, \n"
			+ " prject.项目编码 , \n"
			+ " prject.项目状态ID , \n"
			+ " prjStatus.项目状态 ,\n"
			+ " prject.项目名称"
			+ " FROM xmis.prj_项目 AS prject \n"
			+ " RIGHT JOIN #contractList_1 AS contract ON prject.项目ID = contract.项目ID \n"
			+ " LEFT JOIN xmis.prj_项目状态 AS prjStatus ON prject.项目状态ID = prjStatus.项目状态ID \n"
			+ " AND prject.项目状态ID = prjStatus.项目状态ID \n"

			+ " SELECT  c.合同ID , \n"
			+ " cc.对应项目状态 , \n"
			+ " cc.收款金额 , \n"
			+ " cc.延迟天数 , \n"
			+ " pl.预计开始日期 as 预计开始日期, \n"
			+ " DATEADD(d, cc.延迟天数, pl.实际开始日期) AS 按进度应收款日期 ,\n"
			+ " case when pl.实际开始日期 is null then 1 else 0 end as isnullflag"
			+ " INTO #paymentConditionBaseOnProjectPlan \n"
			+ " FROM xmis.con_合同 AS c \n"
			+ " JOIN xmis.con_收款条件表 AS cc ON c.合同ID = cc.合同ID \n"
			+ " LEFT JOIN xmis.prj_项目进度计划 AS pl ON c.项目ID = pl.项目ID \n"
			+ " AND cc.对应项目状态 = pl.项目状态ID \n"
			+ " WHERE   c.合同类型ID = @contractType  \n"
			// + " AND pl.实际开始日期 IS NOT NULL \n"
			+ " AND pl.预计开始日期 IS NOT NULL \n"

			+ " select 合同ID,min(按进度应收款日期) as 最小应收款日期,max(预计开始日期) as 最大预计开始日期 into #mmtemp \n"
			+ " from #paymentConditionBaseOnProjectPlan where 按进度应收款日期 is not null \n"
			+ " group by 合同ID  \n"

			+ " update #paymentConditionBaseOnProjectPlan set 按进度应收款日期 = #mmtemp.最小应收款日期 from #mmtemp \n"
			+ " where #paymentConditionBaseOnProjectPlan.合同ID = #mmtemp.合同ID \n"
			+ " and isnullflag = 1  \n"
			+ " and #paymentConditionBaseOnProjectPlan.预计开始日期 < = #mmtemp.最大预计开始日期 \n"

			+ " SELECT  c.合同ID , \n"
			+ " warning_date.按进度应收款日期 AS warning_date , \n"
			+ " SUM(c.收款金额) AS warning_amt , \n"
			+ " 0 AS warninged_flag , \n"
			+ " NULL AS warning_days \n"
			+ " INTO #warning_detail \n"
			+ " FROM #paymentConditionBaseOnProjectPlan c , \n"
			+ " ( SELECT    DISTINCT \n"
			+ " c.合同ID , \n"
			+ " c.按进度应收款日期 \n"
			+ " FROM #paymentConditionBaseOnProjectPlan c \n"
			+ " ) AS warning_date \n"
			+ " WHERE c.合同ID = warning_date.合同ID \n"
			+ " AND ( c.按进度应收款日期 <= warning_date.按进度应收款日期 \n"
			+ " ) \n"
			+ " GROUP BY c.合同ID , \n"
			+ " warning_date.按进度应收款日期 \n"

			+ warningString

			+ " END \n"
			/*
			 * + " else begin \n" +
			 * " SELECT xmis.con_收入支出合同对应表.支出子合同ID,xmis.con_合同.合同ID,xmis.con_合同.乙方部门ID,xmis.con_合同.合同负责人 \n"
			 * + " INTO #incomeContractTemp  \n" +
			 * " FROM xmis.con_收入支出合同对应表,xmis.con_合同 \n" +
			 * " WHERE xmis.con_收入支出合同对应表.收入合同ID = xmis.con_合同.合同ID \n" +
			 * " INSERT  INTO #contractList \n" +
			 * " select #contractList_1.* , \n" +
			 * " case when #incomeContractTemp.合同ID is not null then #incomeContractTemp.乙方部门ID else #contractList_1.甲方部门ID end, \n"
			 * +
			 * " case when #incomeContractTemp.合同ID is not null then #incomeContractTemp.合同负责人 else #contractList_1.合同负责人 end, \n"
			 * + " CONVERT(NVARCHAR(50), NULL) AS 项目编码 , \n" +
			 * " CONVERT(INT, NULL) AS 项目状态ID , \n" +
			 * " CONVERT(NVARCHAR(100), NULL) 项目状态 , \n" +
			 * " CONVERT(NVARCHAR(200), NULL) 项目名称" +
			 * " FROM #contractList_1,#incomeContractTemp \n" +
			 * " where #contractList_1.合同ID *= #incomeContractTemp.支出子合同ID  \n"
			 * + " end \n"
			 */

			+ select
			+ "#contractList.*, "

			+ "\nCASE WHEN #charged."
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ " is NULL THEN 0 "
			+ "ELSE #charged."
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ " END AS "
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ ","

			+ "\n#contractList."
			+ DBSchema.Contract.ContractAmt
			+ "-"
			+ "CASE WHEN #charged."
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ " is NULL THEN 0 "
			+ "ELSE #charged."
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ " END As "
			+ DBSchema.ChargedDetail.UnChargedAmtTotal
			+ ","

			+ "\nCASE WHEN #invoice."
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ " is NULL THEN 0 "
			+ "ELSE #invoice."
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ " END AS "
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ ","

			+ "\nCASE WHEN #receipt."
			+ DBSchema.Receipt.TotalAmtByID
			+ " is NULL THEN 0 "
			+ "ELSE #receipt."
			+ DBSchema.Receipt.TotalAmtByID
			+ " END AS "
			+ DBSchema.Receipt.TotalAmtByID
			+ ","

			+ "\nCASE WHEN #invoice."
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ " is NULL THEN 0 "
			+ "WHEN #invoice."
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ " = 0 THEN 0 "
			+ "ELSE #invoice."
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ " - "
			+ "CASE	WHEN #charged."
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ " is NULL THEN 0 "
			+ "ELSE #charged."
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ " END "
			+ "END AS "
			+ DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			+ ", "

			+ "\nCASE WHEN #charged."
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ " is NULL THEN 0 "
			+ "WHEN #charged."
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ " = 0 THEN 0 "
			+ "ELSE #charged."
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ " - "
			+ "CASE	WHEN #invoice."
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ " is NULL THEN 0 "
			+ "ELSE #invoice."
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ " END "
			+ "END AS "
			+ DBSchema.InvoiceDetail.PaymentIndvanceAmtTotalSum
			+ ",#warning.warning_date, "

			+ "warning_amt,warning_amtPlan,warning_bjamt,"

			+ "\n case when #warning.warning_days is null then -31 else #warning.warning_days end as  warning_days \n"
			+ ",#delay.原因 as 原因,#delay.经办人 as 经办人,#delay.登记日期,#delay.登记人 as 登记人  \n"
			+ into + "#sumInfo" + from
			+ "#contractList, #charged, #invoice, #warning,#receipt,#delay  \n"
			+ where + "#contractList." + DBSchema.Contract.ContractID + "*="
			+ "#charged." + DBSchema.ChargedDetail.ContractID + and
			+ "#contractList." + DBSchema.Contract.ContractID + "*="
			+ "#invoice." + DBSchema.InvoiceDetail.ContractID + and
			+ "#contractList." + DBSchema.Contract.ContractID + "*= "
			+ "#warning." + DBSchema.ChargedDetail.ContractID + and
			+ "#contractList." + DBSchema.Contract.ContractID + "*= "
			+ "#receipt." + DBSchema.Receipt.ContractID
			+ " AND #contractList.合同ID*= #delay.合同ID"

			+ "\nUPDATE #sumInfo SET "
			+ DBSchema.InvoiceDetail.UnAccountAmtTotalSum + " = 0 WHERE "
			+ DBSchema.InvoiceDetail.UnAccountAmtTotalSum + " < 0 "
			+ "\nUPDATE #sumInfo SET "
			+ DBSchema.InvoiceDetail.PaymentIndvanceAmtTotalSum + " = 0 WHERE "
			+ DBSchema.InvoiceDetail.PaymentIndvanceAmtTotalSum + " < 0 "

			// 合同财务数据
			+ getGLInfoByContractID;

	public static String getChildContractListByID = " DECLARE @contractType int  set @contractType = 1 \n "
			+ select
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.ContractID
			+ ","
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.ContractNo
			+ ","
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.ContractName
			+ ","
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.SignatureDate
			+ ","
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.Abbreviation
			+ ","
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.ContractAmt
			+ ","
			+ DBSchema.Project.TableName
			+ "."
			+ DBSchema.Project.ProjectCode
			+ ","
			// + DBSchema.Contract.TableName + "." +
			// DBSchema.Contract.ContractAmt + "*"
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.ExchangeRate
			// + " as " + DBSchema.CurrencyType.ExchangedAmt
			+ ","
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.CurrenyGraph
			+ into
			+ "#contractList"
			+ from
			+ DBSchema.Contract.TableName
			+ ","
			+ DBSchema.Project.TableName
			+ ","
			+ DBSchema.CurrencyType.TableName
			+ where
			+ DBSchema.Contract.ParentID
			+ "=?"
			+ and
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.ProjectID
			+ "*="
			+ DBSchema.Project.TableName
			+ "."
			+ DBSchema.Project.ProjectID
			+ and
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.CurrencyID
			+ "*="
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.CurrencyID

			+ getSumInfoByContractID;

	public static String getContractType = select + "*" + from
			+ DBSchema.ContractType.TableName;

	public static String getContractSourceType = select + "*" + from
			+ DBSchema.ContractSourceType.TableName;

	public static String getCurrencyType = select + "*" + from
			+ DBSchema.CurrencyType.TableName + where
			+ DBSchema.CurrencyType.CurrencyID + ">0";

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
			+ DBSchema.Province.ProvinceName + intoResult + from
			+ "#company_city," + DBSchema.Province.TableName + where
			+ "#company_city." + DBSchema.City.ProvinceID + "*="
			+ DBSchema.Province.TableName + "." + DBSchema.Province.ProvinceID;

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
			+ DBSchema.Province.ProvinceName + intoResult + from
			+ "#company_city," + DBSchema.Province.TableName + where
			+ "#company_city." + DBSchema.City.ProvinceID + "*="
			+ DBSchema.Province.TableName + "." + DBSchema.Province.ProvinceID

	;

	public static String getGovernmentContractSourceList = select
			+ DBSchema.GovernmentContractSource.TableName
			+ ".*,"
			+ DBSchema.Company.TableName
			+ "."
			+ DBSchema.Company.CompanyName
			+ ","
			+ DBSchema.Company.TableName
			+ "."
			+ DBSchema.Company.Abbreviation
			+ " as company,"
			+ DBSchema.Department.TableName
			+ "."
			+ DBSchema.Department.DepartmentName
			+ ","
			+ DBSchema.Department.TableName
			+ "."
			+ DBSchema.Department.Abbreviation
			+ " as dept,"
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.CurrencyName
			// + intoResult
			+ from + DBSchema.GovernmentContractSource.TableName + ","
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
			+ "(?,?,?,?,?,?,?,GETDATE(),?,0)";

	public static String updateGContractSource = update
			+ DBSchema.GovernmentContractSource.TableName + set
			+ DBSchema.GovernmentContractSource.GovermentContractSourceName
			+ "=?," + DBSchema.GovernmentContractSource.CompanyID + "=?,"
			+ DBSchema.GovernmentContractSource.DepartmentID + "=?,"
			+ DBSchema.GovernmentContractSource.FundingLimit + "=?,"
			+ DBSchema.GovernmentContractSource.CurrencyTypeID + "=?,"
			+ DBSchema.GovernmentContractSource.Specialty + "=?,"
			+ DBSchema.GovernmentContractSource.UpdateDate + "=GETDATE(),"
			+ DBSchema.GovernmentContractSource.UpdateID + "=?" + where
			+ DBSchema.GovernmentContractSource.GovermentContractSourceID
			+ "=?";

	public static String setSTonContract = update + DBSchema.Contract.TableName
			+ set + DBSchema.Contract.GovermentContractSourceID + "=?,"
			+ DBSchema.Contract.RegisterID + "=?,"
			+ DBSchema.Contract.UpdateDate + "=GETDATE()" + where
			+ DBSchema.Contract.ContractID + "=?";

	public static String existsContractByGST = ifExists + "(" + select + "*"
			+ from + DBSchema.Contract.TableName + where
			+ DBSchema.Contract.GovermentContractSourceID + " =?)" + select
			+ "1" + elseExists + select + "2";

	public static String deleteGContractSource = update
			+ DBSchema.GovernmentContractSource.TableName + set
			+ DBSchema.GovernmentContractSource.Actived + "=1,"
			+ DBSchema.GovernmentContractSource.UpdateDate + "=GETDATE(),"
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

	private static String contractColumn = "xmis.con_合同.合同ID ,"
			+ " xmis.con_合同.合同编号 ," + " xmis.con_合同.合同名称 ,"
			+ " xmis.con_合同.简称 ," + " xmis.con_合同.父合同ID ,"
			+ " xmis.con_合同.签订日期 ," + " xmis.con_合同.甲方公司ID ,"
			+ " xmis.con_合同.甲方部门ID ," + " xmis.con_合同.乙方公司ID ,"
			+ " xmis.con_合同.乙方部门ID ," + " xmis.con_合同.合同总额 ,"
			+ " xmis.con_合同.货币ID ," + " xmis.con_合同.合同类型ID ,"
			+ " xmis.con_合同.甲方代表ID ," + " xmis.con_合同.乙方代表ID ,"
			+ " xmis.con_合同.合同概要 ," + " xmis.con_合同.备注 ,"
			+ " xmis.con_合同.登记人ID ," + " xmis.con_合同.更新时间 ,"
			+ " xmis.con_合同.合同来源类型ID ," + " xmis.con_合同.横向合同来源ID ,"
			+ " xmis.con_合同.质保期起 ," + " xmis.con_合同.质保期止 ,"
			+ " xmis.con_合同.质保期定义 ," + " xmis.con_合同.发货联系人ID ,"
			+ " xmis.con_合同.质保期限 ," + " xmis.con_合同.质保期计量单位 ,"
			+ " xmis.con_合同.项目ID ," + " xmis.con_合同.合同负责人 ,"
			+ " xmis.con_合同.最后修改日期 ," + " xmis.con_合同.最后修改人ID ,"
			+ " xmis.con_合同.有效期限 ," + " xmis.con_合同.有效期计量单位 ,"
			+ " xmis.con_合同.有效期起 ," + " xmis.con_合同.有效期止";

	public static String getContractListByTypeWithoutAccount = "DECLARE @contractType int \n set @contractType = ? \n"
			+ select
			+ contractColumn
			+ ","
			+ "Convert(varchar(10),"
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.WarrantyStartDate
			+ ",1) as start_date, "
			+ "Convert(varchar(10),"
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.WarrantyEndDate
			+ ",1) as end_date, "
			+ "Convert(varchar(10),"
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.UpdateDate
			+ ",1) as update_date, "
			+ "contract."
			+ DBSchema.Contract.ContractNo
			+ " contractNo, "
			+ "contract."
			+ DBSchema.Contract.Abbreviation
			+ " abbreviation, "
			+ "aCompany."
			+ DBSchema.Company.CompanyName
			+ " aCompany_name, "
			+ "aCompany."
			+ DBSchema.Company.Abbreviation
			+ " aCompany_abb,"
			+ "bCompany."
			+ DBSchema.Company.CompanyName
			+ " bCompany_name, "
			+ "aDept."
			+ DBSchema.Department.DepartmentName
			+ " aDept_name, "
			+ "bDept."
			+ DBSchema.Department.DepartmentName
			+ " bDept_name, "
			+ "RTRIM(aEmp."
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM(aEmp."
			+ DBSchema.Employee.LastName
			+ ") aEmp_name, "
			+ "RTRIM(bEmp."
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM(bEmp."
			+ DBSchema.Employee.LastName
			+ ") bEmp_name, "
			+ "RTRIM(cEmp."
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM(cEmp."
			+ DBSchema.Employee.LastName
			+ ") cEmp_name, "
			+ "RTRIM(principal."
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM(principal."
			+ DBSchema.Employee.LastName
			+ ") 负责人姓名, "
			+ DBSchema.GovernmentContractSource.TableName
			+ "."
			+ DBSchema.GovernmentContractSource.GovermentContractSourceName
			+ " as "
			+ DBSchema.Contract.GovermentContractSourceName
			+ ","
			+ "CASE	WHEN "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.CurrencyID
			+ " IS NULL THEN '￥'"
			+ " WHEN "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.CurrencyID
			+ " = -1 THEN '￥'"
			+ " ELSE "
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.CurrenyGraph
			+ " END as "
			+ DBSchema.CurrencyType.CurrenyGraph
			+ into
			// + "#contractList_1"
			+ "#contractList_0"

			+ from
			+ DBSchema.Contract.TableName
			+ ","
			+ DBSchema.Contract.TableName
			+ as
			+ "contract, "
			+ DBSchema.Company.TableName
			+ as
			+ "aCompany, "
			+ DBSchema.Company.TableName
			+ as
			+ "bCompany, "
			+ DBSchema.Department.TableName
			+ as
			+ "aDept, "
			+ DBSchema.Department.TableName
			+ as
			+ "bDept, "
			+ DBSchema.Employee.TableName
			+ as
			+ "aEmp,"
			+ DBSchema.Employee.TableName
			+ as
			+ "bEmp,"
			+ DBSchema.Employee.TableName
			+ as
			+ "cEmp,"
			+ DBSchema.Employee.TableName
			+ as
			+ "principal,"
			+ DBSchema.GovernmentContractSource.TableName
			+ ","
			+ DBSchema.CurrencyType.TableName

			+ where
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.ContractTypeID
			+ "=@contractType"
			+ and
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.ParentID
			+ " *= contract."
			+ DBSchema.Contract.ContractID
			+ and
			+ "aCompany."
			+ DBSchema.Company.CompanyID
			+ " =* "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.BuyerID
			+ and
			+ "bCompany."
			+ DBSchema.Company.CompanyID
			+ " =* "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.VendorID
			+ and
			+ "aDept."
			+ DBSchema.Department.DepartmentID
			+ " =* "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.DeptIDOfBuyer
			+ and
			+ "bDept."
			+ DBSchema.Department.DepartmentID
			+ " =* "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.DeptIDOfVendor
			+ and
			+ "aEmp."
			+ DBSchema.Employee.ID
			+ " =* "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.BuyerRepresentativeID
			+ and
			+ "bEmp."
			+ DBSchema.Employee.ID
			+ " =* "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.VendorRepresentativeID
			+ and
			+ "cEmp."
			+ DBSchema.Employee.ID
			+ " =* "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.RegisterID
			+ and
			+ "principal."
			+ DBSchema.Employee.ID
			+ " =* "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.DirectorID
			+ and
			+ DBSchema.GovernmentContractSource.TableName
			+ "."
			+ DBSchema.GovernmentContractSource.GovermentContractSourceID
			+ " =* "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.GovermentContractSourceID
			+ and
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.CurrencyID
			+ "*="
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.CurrencyID

			// + "\n\n select #contractList_0.*,xmis.org_部门.部门名称 execu_name,"
			// + " xmis.prj_项目.执行部门 execu_id"
			// + " into #contractList_1"
			// +
			// " from #contractList_0 left join xmis.prj_项目 on #contractList_0.项目ID = xmis.prj_项目.项目ID "
			// + " left join xmis.org_部门 on xmis.prj_项目.执行部门 = xmis.org_部门.部门ID"

			/*
			 * lx 针对老自动化合同数据处理 +
			 * " \n\nselect #contractList_0.*,xmis.org_部门.部门名称 execu_name, xmis.prj_项目.执行部门 execu_id, xmis.con_摘要模版.模板内容 as summarystencil ,\n"
			 * +
			 * " 关键字 as keywords,xmis.prj_项目.项目经理ID manager_id,RTRIM(xmis.hr_人员.姓氏)+RTRIM(xmis.hr_人员.名字) manager_name \n"
			 * + " into #contractList_1 \n" +
			 * " from #contractList_0 left join xmis.prj_项目 on #contractList_0.项目ID = xmis.prj_项目.项目ID \n "
			 * +
			 * " left join xmis.org_部门 on xmis.prj_项目.执行部门 = xmis.org_部门.部门ID \n"
			 * +
			 * " left join xmis.con_摘要模版 on #contractList_0.乙方部门ID = xmis.con_摘要模版.部门ID \n"
			 * +
			 * " left join xmis.hr_人员 on xmis.prj_项目.项目经理ID = xmis.hr_人员.人员ID \n"
			 */

			/*
			 * +
			 * "\n\nSELECT #contractList_0.* ,        xmis.org_部门.部门名称 execu_name ,        xmis.prj_项目.执行部门 execu_id ,"
			 * +
			 * "        xmis.con_摘要模版.模板内容 AS summarystencil ,        关键字 AS keywords ,        xmis.prj_项目.项目经理ID manager_id ,"
			 * +
			 * "        RTRIM(xmis.hr_人员.姓氏) + RTRIM(xmis.hr_人员.名字) manager_name"
			 * +
			 * " INTO   #contractList_1 FROM   #contractList_0 ,        xmis.prj_项目 ,        xmis.con_摘要模版,"
			 * + "        xmis.org_部门,        xmis.hr_人员" +
			 * " WHERE  #contractList_0.项目ID = xmis.prj_项目.项目ID        AND #contractList_0.乙方部门ID *= xmis.con_摘要模版.部门ID"
			 * +
			 * "        AND xmis.prj_项目.执行部门 *= xmis.org_部门.部门ID        AND xmis.prj_项目.项目经理ID *= xmis.hr_人员.人员ID "
			 */

			+ " SELECT    #contractList_0.* , \n"
			+ " xmis.prj_项目.执行部门 execu_id , \n"
			+ " xmis.con_摘要模版.模板内容 AS summarystencil , \n"
			+ " 关键字 AS keywords , \n"
			+ " xmis.prj_项目.项目经理ID manager_id \n"
			+ " INTO      #contractList_11 \n"
			+ " FROM      #contractList_0 , \n"
			+ " xmis.prj_项目 , \n"
			+ " xmis.con_摘要模版 \n"
			+ " WHERE     #contractList_0.项目ID *= xmis.prj_项目.项目ID \n"
			+ " AND #contractList_0.乙方部门ID *= xmis.con_摘要模版.部门ID \n"

			+ " SELECT    #contractList_11.* , \n"
			+ " xmis.org_部门.部门名称 execu_name , \n"
			+ " RTRIM(xmis.hr_人员.姓氏) + RTRIM(xmis.hr_人员.名字) manager_name \n"
			+ " INTO      #contractList_1 \n"
			+ " FROM      #contractList_11 , \n"
			+ " xmis.org_部门 , \n"
			+ " xmis.hr_人员 \n"
			+ " WHERE     #contractList_11.execu_id *= xmis.org_部门.部门ID \n"
			+ " AND #contractList_11.manager_id *= xmis.hr_人员.人员ID  \n"

			// 创建临时表
			+ "\n\n --create new temp table \n"
			+ " SELECT  #contractList_1.* , \n"
			+ " CONVERT(SMALLINT, NULL) AS 对应收入合同部门ID , \n"
			+ " CONVERT(SMALLINT, NULL) AS 对应收入合同负责人, \n"
			+ " CONVERT(NVARCHAR(50), NULL) AS 项目编码 , \n"
			+ " CONVERT(INT, NULL) AS 项目状态ID , \n"
			+ " CONVERT(NVARCHAR(100), NULL) 项目状态  , \n"
			+ " CONVERT(NVARCHAR(200), NULL) 项目名称 \n"
			+ " INTO    #contractList \n"
			+ " FROM    #contractList_1 \n"
			+ " WHERE   1 = 2 \n"
			+ " SELECT  #contractList.合同ID , \n"
			+ " CONVERT(DATETIME, NULL) AS warning_date , \n"
			+ " CONVERT(DECIMAL(18, 2), 0) AS warning_amt , \n"
			+ " CONVERT(INT, NULL) AS warning_days , \n"
			+ " CONVERT(DECIMAL(18, 2), NULL) AS warning_amtPlan , \n"
			+ " CONVERT(DECIMAL(18, 2), NULL) AS warning_bjamt \n"
			+ " INTO    #warning \n"
			+ " FROM    #contractList \n"
			+ " WHERE   1 = 2 \n"

			// 删除父合同
			+ "\n\n --收入合同删除父合同，支出合同删除有子合同的父合同，保留无子合同的‘父’合同及支出子合同 \n"
			+ " IF ( @contractType = 1 )  \n"
			+ " BEGIN  \n"
			+ " DELETE  #contractList_1  \n"
			+ " WHERE   父合同ID IS NOT NULL  AND 乙方部门ID >0\n"

			+ " END  \n"
			+ " ELSE  \n"
			+ " BEGIN  \n"

			+ delete
			+ "#contractList_1"
			+ where
			+ DBSchema.Contract.ContractTypeID
			+ " = 2"
			+ and
			+ DBSchema.Contract.ContractID
			+ " in (select distinct "
			+ DBSchema.Contract.ParentID
			+ " from "
			+ DBSchema.Contract.TableName
			+ " where "
			+ DBSchema.Contract.ContractTypeID
			+ " = 2 AND 父合同ID IS NOT NULL)  \n "

			+ " SELECT xmis.con_收入支出合同对应表.支出子合同ID,xmis.con_合同.合同ID,xmis.con_合同.乙方部门ID,xmis.con_合同.合同负责人 \n"
			+ " INTO #incomeContractTemp  \n"
			+ " FROM xmis.con_收入支出合同对应表,xmis.con_合同 \n"
			+ " WHERE xmis.con_收入支出合同对应表.收入合同ID = xmis.con_合同.合同ID \n"
			+ " INSERT  INTO #contractList \n"
			+ " select #contractList_1.* , \n"
			+ " case when #incomeContractTemp.合同ID is not null then #incomeContractTemp.乙方部门ID else #contractList_1.甲方部门ID end, \n"
			+ " case when #incomeContractTemp.合同ID is not null then #incomeContractTemp.合同负责人 else #contractList_1.合同负责人 end, \n"
			+ " CONVERT(NVARCHAR(50), NULL) AS 项目编码 , \n"
			+ " CONVERT(INT, NULL) AS 项目状态ID , \n"
			+ " CONVERT(NVARCHAR(100), NULL) 项目状态 , \n"
			+ " CONVERT(NVARCHAR(200), NULL) 项目名称"
			+ " FROM #contractList_1,#incomeContractTemp \n"
			+ " where #contractList_1.合同ID *= #incomeContractTemp.支出子合同ID  \n"

			+ " end"

	// 此处特殊处理原因，见ContractDAOImpl.hasFullRightsOnConcrat()
	;

	public static String getContractListByType = getContractListByTypeWithoutAccount
			// 合同报警信息、及财务信息
			+ getSumInfoByContractID;

	public static String getContractListByProjectID = select
			+ DBSchema.Contract.TableName
			+ ".*,"
			+ "rtrim(director."
			+ DBSchema.Employee.FirstName
			+ ") + rtrim(director."
			+ DBSchema.Employee.LastName
			+ ") as director_name,"
			+ "Convert(varchar(10),"
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.WarrantyStartDate
			+ ",1) as start_date, "
			+ "Convert(varchar(10),"
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.WarrantyEndDate
			+ ",1) as end_date, "
			+ "Convert(varchar(10),"
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.UpdateDate
			+ ",1) as update_date, "
			// + DBSchema.Project.TableName
			// + "."
			// + DBSchema.Project.ProjectCode
			// + ","
			// + DBSchema.Project.TableName
			// + "."
			// + DBSchema.Project.ProjectName
			// + ","
			+ "contract."
			+ DBSchema.Contract.ContractNo
			+ " contractNo, "
			+ "contract."
			+ DBSchema.Contract.Abbreviation
			+ " abbreviation, "
			+ "aCompany."
			+ DBSchema.Company.CompanyName
			+ " aCompany_name, "
			+ "aCompany."
			+ DBSchema.Company.Abbreviation
			+ " aCompany_abb,"
			+ "bCompany."
			+ DBSchema.Company.CompanyName
			+ " bCompany_name, "
			+ "aDept."
			+ DBSchema.Department.DepartmentName
			+ " aDept_name, "
			+ "bDept."
			+ DBSchema.Department.DepartmentName
			+ " bDept_name, "
			+ "RTRIM(aEmp."
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM(aEmp."
			+ DBSchema.Employee.LastName
			+ ") aEmp_name, "
			+ "RTRIM(bEmp."
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM(bEmp."
			+ DBSchema.Employee.LastName
			+ ") bEmp_name, "
			+ "RTRIM(cEmp."
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM(cEmp."
			+ DBSchema.Employee.LastName
			+ ") cEmp_name, "
			+ DBSchema.GovernmentContractSource.TableName
			+ "."
			+ DBSchema.GovernmentContractSource.GovermentContractSourceName
			+ " as "
			+ DBSchema.Contract.GovermentContractSourceName
			+ ",CASE	WHEN "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.CurrencyID
			+ " IS NULL THEN '￥'"
			+ " WHEN "
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.CurrencyID
			+ " = -1 THEN '￥'"
			+ " ELSE "
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.CurrenyGraph
			+ " END as "
			+ DBSchema.CurrencyType.CurrenyGraph
			// + into + "#result"

			+ into + "#contractList_1"

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
			+ DBSchema.CurrencyType.TableName + ","
			+ DBSchema.Employee.TableName + " as director "

			+ where + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.ParentID + " IS NULL " + and
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ProjectID
			+ "=?" + and + DBSchema.Project.TableName + "."
			+ DBSchema.Project.ProjectID + " =* " + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.ProjectID + and
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ParentID
			+ " *= contract." + DBSchema.Contract.ContractID + and
			+ "aCompany." + DBSchema.Company.CompanyID + " =* "
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.BuyerID
			+ and + "bCompany." + DBSchema.Company.CompanyID + " =* "
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.VendorID
			+ and + "aDept." + DBSchema.Department.DepartmentID + " =* "
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
			+ DBSchema.Contract.GovermentContractSourceID + and
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID
			+ "*=" + DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrencyID + and + " director."
			+ DBSchema.Employee.ID + " =* " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.DirectorID + orderBy
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ProjectID

			+ "\n\n --create new temp table \n"
			+ " SELECT  #contractList_1.* , \n"
			+ " CONVERT(SMALLINT, NULL) AS 对应收入合同部门ID , \n"
			+ " CONVERT(SMALLINT, NULL) AS 对应收入合同负责人, \n"
			+ " CONVERT(NVARCHAR(50), NULL) AS 项目编码 , \n"
			+ " CONVERT(INT, NULL) AS 项目状态ID , \n"
			+ " CONVERT(NVARCHAR(100), NULL) 项目状态  , \n"
			+ " CONVERT(NVARCHAR(200), NULL) 项目名称"
			+ " INTO    #contractList \n" + " FROM    #contractList_1 \n"
			+ " WHERE   1 = 2 \n" + " SELECT  #contractList.合同ID , \n"
			+ " CONVERT(DATETIME, NULL) AS warning_date , \n"
			+ " CONVERT(DECIMAL(18, 2), 0) AS warning_amt , \n"
			+ " CONVERT(INT, NULL) AS warning_days ,\n"
			+ " CONVERT(DECIMAL(18, 2), NULL) AS warning_amtPlan , \n"
			+ " CONVERT(DECIMAL(18, 2), NULL) AS warning_bjamt \n"
			+ " INTO    #warning \n" + " FROM    #contractList \n"
			+ " WHERE   1 = 2 \n"

			+ getSumInfoByContractID;

	public static String getOutContractByProjectID = select + " c2."
			+ DBSchema.Contract.ContractID + ",c2."
			+ DBSchema.Contract.ContractNo + ",c2."
			+ DBSchema.Contract.ContractName + ",c2."
			+ DBSchema.Contract.Abbreviation + ",c2."
			+ DBSchema.Contract.SignatureDate + ",m."
			+ DBSchema.ContractRelated.Amt + ",m."
			+ DBSchema.ContractRelated.Remark + from
			+ DBSchema.Contract.TableName + " c1 left join "
			+ DBSchema.ContractRelated.TableName + " m on m."
			+ DBSchema.ContractRelated.IncomeContractID + " = c1."
			+ DBSchema.Contract.ContractID + " left join "
			+ DBSchema.Contract.TableName + " c2 on c2."
			+ DBSchema.Contract.ContractID + "=m."
			+ DBSchema.ContractRelated.OutContractID + where + " c1."
			+ DBSchema.Contract.ProjectID + "=?" + and + " m.对应ID is not null";

	public static String getPaymentListByID = select
			+ DBSchema.ChargedDetail.TableName + ".*,"

			+ "Convert(varchar(10)," + DBSchema.ChargedDetail.TableName + "."
			+ DBSchema.ChargedDetail.ChargedDate + ",101) as "
			+ DBSchema.ChargedDetail.ChargedDateDisplayed + ", "

			+ "Convert(varchar(10)," + DBSchema.ChargedDetail.TableName + "."
			+ DBSchema.ChargedDetail.UpdateDate + ",101) as "
			+ DBSchema.ChargedDetail.UpdateDateDisplay + ", "

			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID
			+ "," + DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrenyGraph + ","
			+ DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.ExchangeRate + "," + "RTRIM(aEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(aEmp."
			+ DBSchema.Employee.LastName + ") aEmp_name, " + "RTRIM(bEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(bEmp."
			+ DBSchema.Employee.LastName + ") bEmp_name " + intoResult + from
			+ DBSchema.ChargedDetail.TableName + ","
			+ DBSchema.Contract.TableName + ","
			+ DBSchema.CurrencyType.TableName + ","
			+ DBSchema.Employee.TableName + " as aEmp,"
			+ DBSchema.Employee.TableName + " as bEmp " + where
			+ DBSchema.ChargedDetail.TableName + "."
			+ DBSchema.ChargedDetail.ContractID + "=?" + and
			+ DBSchema.ChargedDetail.TableName + "."
			+ DBSchema.ChargedDetail.ContractID + "="
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ContractID
			+ and + DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrencyID + "="
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID
			+ and + DBSchema.ChargedDetail.TableName + "."
			+ DBSchema.ChargedDetail.TransactorID + "*=" + "aEmp."
			+ DBSchema.Employee.ID + and + DBSchema.ChargedDetail.TableName
			+ "." + DBSchema.ChargedDetail.UpdateID + "*=" + "bEmp."
			+ DBSchema.Employee.ID;

	/*
	 * 
	 */

	/**
	 * select SUM(pay.收款金额) from xmis.con_合同 p left join xmis.con_合同 s on
	 * p.合同ID=s.父合同ID left join xmis.con_收款明细表 pay on pay.合同ID=p.合同ID or
	 * pay.合同ID=s.合同ID WHERE p.合同ID=?
	 */
	public static String getTotalPaymentByID = select + "SUM(pay."
			+ DBSchema.ChargedDetail.ChargedAmt + ") as "
			+ DBSchema.ChargedDetail.TotalAmtByID + from
			+ DBSchema.Contract.TableName + " p left join "
			+ DBSchema.Contract.TableName + " s on p."
			+ DBSchema.Contract.ContractID + " = s."
			+ DBSchema.Contract.ParentID + " left join "
			+ DBSchema.ChargedDetail.TableName + " pay on pay."
			+ DBSchema.ChargedDetail.ContractID + " = p."
			+ DBSchema.Contract.ContractID + " or pay."
			+ DBSchema.ChargedDetail.ContractID + " = s."
			+ DBSchema.Contract.ContractID + where + " p."
			+ DBSchema.ChargedDetail.ContractID + "=?";

	public static String getTotalInvoicedByID = select + "SUM(pay."
			+ DBSchema.InvoiceDetail.InvoiceAmt + ") as "
			+ DBSchema.InvoiceDetail.TotalAmtByID + from
			+ DBSchema.Contract.TableName + " p left join "
			+ DBSchema.Contract.TableName + " s on p."
			+ DBSchema.Contract.ContractID + " = s."
			+ DBSchema.Contract.ParentID + " left join "
			+ DBSchema.InvoiceDetail.TableName + " pay on pay."
			+ DBSchema.InvoiceDetail.ContractID + " = p."
			+ DBSchema.Contract.ContractID + " or pay."
			+ DBSchema.InvoiceDetail.ContractID + " = s."
			+ DBSchema.Contract.ContractID + where + " p."
			+ DBSchema.ChargedDetail.ContractID + "=?";

	public static String getDelayReasonListByContractID = "select m.ID,m.合同ID,m.原因,m.经办人ID,m.登记人ID,CONVERT(CHAR(10), m.登记日期, 23) as 登记日期,rtrim(a.姓氏)+rtrim(a.名字) 经办人,rtrim(b.姓氏)+rtrim(b.名字) 登记人 from [xmis].[con_付款推迟原因] m"
			+ " left outer join xmis.hr_人员 a on m.经办人ID = a.人员ID"
			+ " left outer join xmis.hr_人员 b on m.登记人ID = b.人员ID"
			+ " where m.合同ID = ? order by m.ID desc";

	public static String getInvoiceListByID = "DECLARE @contractType int  set @contractType = ? "
			+ select
			+ DBSchema.InvoiceDetail.TableName
			+ ".*,"

			+ "Convert(varchar(10),"
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.InvoiceDate
			+ ",101) as "
			+ DBSchema.InvoiceDetail.InvoiceDateDisplayed
			+ ", "

			+ "Convert(varchar(10),"
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.UpdateDate
			+ ",101) as "
			+ DBSchema.InvoiceDetail.UpdateDateDisplayed
			+ ", "

			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.CurrencyID
			+ ","
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.CurrenyGraph
			+ ","
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.ExchangeRate
			+ ","
			+ "RTRIM(aEmp."
			+ DBSchema.Employee.FirstName
			+ ")"
			+ "+RTRIM(aEmp."
			+ DBSchema.Employee.LastName
			+ ") aEmp_name, "
			+ "RTRIM(bEmp."
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM(bEmp."
			+ DBSchema.Employee.LastName
			+ ") bEmp_name, "
			+ "RTRIM(cEmp."
			+ DBSchema.Employee.FirstName
			+ ")+RTRIM(cEmp."
			+ DBSchema.Employee.LastName
			+ ") cEmp_name "
			+ ",0 as "
			+ DBSchema.InvoiceDetail.paidAmount
			+ ","
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.InvoiceAmt
			+ " as "
			+ DBSchema.InvoiceDetail.unPaidAmount
			+ intoResult
			+ from
			+ DBSchema.InvoiceDetail.TableName
			+ ","
			+ DBSchema.Contract.TableName
			+ ","
			+ DBSchema.CurrencyType.TableName
			+ ","
			+ DBSchema.Employee.TableName
			+ " as aEmp,"
			+ DBSchema.Employee.TableName
			+ " as bEmp, "
			+ DBSchema.Employee.TableName
			+ " as cEmp "
			+ where
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.ContractID
			+ "=@contractType"
			+ and
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.ContractID
			+ "="
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.ContractID
			+ and
			+ DBSchema.CurrencyType.TableName
			+ "."
			+ DBSchema.CurrencyType.CurrencyID
			+ "="
			+ DBSchema.Contract.TableName
			+ "."
			+ DBSchema.Contract.CurrencyID
			+ and
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.TransactorID
			+ "*="
			+ "aEmp."
			+ DBSchema.Employee.ID
			+ and
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.UpdateID
			+ "*="
			+ "bEmp."
			+ DBSchema.Employee.ID
			+ and
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.ReceiverID
			+ "*="
			+ "cEmp."
			+ DBSchema.Employee.ID

			+ select
			+ "a."
			+ DBSchema.InvoiceDetail.ContractID
			+ ",a."
			+ DBSchema.InvoiceDetail.InvoiceNo
			+ ",SUM(拆分金额) as 已付款"
			+ into
			+ " #r"
			+ from
			+ DBSchema.InvoiceDetail.TableName
			+ " as a"
			+ " left outer join xmis.con_发票金额拆分 as b on a.合同ID = b.合同ID and a.发票号=b.发票号"
			+ " where a.合同ID =@contractType and b.是否已付款=1 group by a.合同ID,a.发票号,a.发票金额"

			+ " update #result set #result.已付款=#r.已付款,#result.未付款=#result.未付款-#r.已付款 from #r where #result.合同ID=#r.合同ID and #result.发票号=#r.发票号";

	public static String getReceiptListAsInvoiceByID = select
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.ReceiptID
			+ " as " + DBSchema.InvoiceDetail.InvoiceNo + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.ContractID
			+ " as " + DBSchema.InvoiceDetail.ContractID + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.ReceiptAmt
			+ " as " + DBSchema.InvoiceDetail.InvoiceAmt + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.ReceiptDate
			+ " as " + DBSchema.InvoiceDetail.InvoiceDate + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.TransactorID
			+ " as " + DBSchema.InvoiceDetail.TransactorID + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.ReceiptScan
			+ " as " + DBSchema.InvoiceDetail.ScanAttachment + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.UpdateID
			+ " as " + DBSchema.InvoiceDetail.UpdateID + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.UpdateDate
			+ " as " + DBSchema.InvoiceDetail.UpdateDate + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.Remark
			+ " as " + DBSchema.InvoiceDetail.Remark + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.ReceiptCode
			+ " as " + DBSchema.InvoiceDetail.InvoiceCode + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.ReceiverID
			+ " as " + DBSchema.InvoiceDetail.ReceiverID + ","
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.MailCode
			+ " as " + DBSchema.InvoiceDetail.MailCode + ","

			+ "Convert(varchar(10)," + DBSchema.Receipt.TableName + "."
			+ DBSchema.Receipt.ReceiptDate + ",101) as "
			+ DBSchema.InvoiceDetail.InvoiceDateDisplayed + ", "

			+ "Convert(varchar(10)," + DBSchema.Receipt.TableName + "."
			+ DBSchema.Receipt.UpdateDate + ",101) as "
			+ DBSchema.InvoiceDetail.UpdateDateDisplayed + ", "

			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID
			+ "," + DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrenyGraph + ","
			+ DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.ExchangeRate + "," + "RTRIM(aEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(aEmp."
			+ DBSchema.Employee.LastName + ") aEmp_name, " + "RTRIM(bEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(bEmp."
			+ DBSchema.Employee.LastName + ") bEmp_name, " + "RTRIM(cEmp."
			+ DBSchema.Employee.FirstName + ")+RTRIM(cEmp."
			+ DBSchema.Employee.LastName + ") cEmp_name " + intoResult + from
			+ DBSchema.Receipt.TableName + "," + DBSchema.Contract.TableName
			+ "," + DBSchema.CurrencyType.TableName + ","
			+ DBSchema.Employee.TableName + " as aEmp,"
			+ DBSchema.Employee.TableName + " as bEmp, "
			+ DBSchema.Employee.TableName + " as cEmp " + where
			+ DBSchema.Receipt.TableName + "." + DBSchema.Receipt.ContractID
			+ "=?" + and + DBSchema.Receipt.TableName + "."
			+ DBSchema.Receipt.ContractID + "=" + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.ContractID + and
			+ DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrencyID + "="
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.CurrencyID
			+ and + DBSchema.Receipt.TableName + "."
			+ DBSchema.Receipt.TransactorID + "*=" + "aEmp."
			+ DBSchema.Employee.ID + and + DBSchema.Receipt.TableName + "."
			+ DBSchema.Receipt.UpdateID + "*=" + "bEmp." + DBSchema.Employee.ID
			+ and + DBSchema.Receipt.TableName + "."
			+ DBSchema.Receipt.ReceiverID + "*=" + "cEmp."
			+ DBSchema.Employee.ID;

	public static String getContractListByTypeForSearch = getContractListByType
			.replace("#result", "#contract")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.ContractNo
			+ like
			+ "?"
			+ union
			+ select
			+ "*"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.Abbreviation + like + "?";

	public static String getContractListByQueryForSearch(String paramString) {
		String str = getContractListByType.replace("#result", "#contract")
				+ select + "*" + into + "#result" + from + "#contract";
		return StringUtils.isEmpty(paramString) ? str : str + where
				+ paramString;
	}

	public static final String getRelatedContractListIncomeWithoutTotalInfo = " SELECT r.*,payout.合同编号 AS 支出合同编码,"
			+ " income.合同名称 AS 收入合同名称, payout.合同名称 AS 支出合同名称,sum(pay.收款金额) as 已付合同款"
			+ " FROM xmis.con_收入支出合同对应表 AS r "
			+ " LEFT OUTER JOIN xmis.con_合同 AS income ON r.收入合同ID = income.合同ID"
			+ " LEFT OUTER JOIN xmis.con_合同 AS payout ON r.支出合同ID = payout.合同ID"
			+ " left join xmis.con_合同 s on r.支出合同ID=s.父合同ID"
			+ " left join xmis.con_收款明细表 pay on pay.合同ID=r.支出合同ID or pay.合同ID=s.合同ID "
			+ " where r.收入合同ID = ? group by r.收入合同ID,r.支出合同ID,r.对应ID,r.金额,r.备注,r.支出子合同ID,payout.合同编号,income.合同名称,payout.合同名称";

	public static final String getRelatedContractListIncome = " select list.*,sum(p.收款金额) as 已付合同款 "
			+ " into #result"
			+ " from "
			+ " (SELECT "
			+ " distinct"
			+ " r.*,payout.合同编号  as 支出合同编码, income.合同名称 AS 收入合同名称, payout.合同名称 AS 支出合同名称,payout.签订日期  AS 支出合同签订日期"
			+ " FROM xmis.con_收入支出合同对应表 AS r  "
			+ " LEFT OUTER JOIN xmis.con_合同 AS income ON r.收入合同ID = income.合同ID "
			+ " LEFT OUTER JOIN xmis.con_合同 AS payout ON r.支出合同ID = payout.合同ID "
			+ " LEFT OUTER JOIN xmis.con_合同 as s on r.支出合同ID=s.父合同ID "
			+ " where r.收入合同ID = ?) AS list"
			+ " LEFT OUTER JOIN xmis.con_收款明细表 as p on list.支出子合同ID = p.合同ID"
			+ " group by list.收入合同ID,list.支出合同ID,list.对应ID,list.金额,list.备注,list.支出子合同ID,list.支出合同编码,list.收入合同名称,list.支出合同名称,list.支出合同签订日期,list.最后修改日期,list.最后修改人ID";

	public static final String getRelatedContractListIncomeWithTotalInfoResults = " select null as 收入合同ID,null as 支出合同ID,null as 对应ID,SUM(金额) as 金额,null as 备注,null as 支出子合同ID,null as 最后修改日期,null as 最后修改人ID,null as 支出合同编码,null as 收入合同名称,null as 支出合同名称,NULL AS 支出合同签订日期,sum(已付合同款) as 已付合同款 from #result "
			+ " union all  " + " select * from #result";

	// public static final String getRelatedContractListPayout =
	// " select list.收入合同ID,list.支出合同ID,list.对应ID,list.金额,list.支出子合同ID,list.备注,list.收入合同名称,list.支出合同名称,list.支出子合同编号,list.已付合同款,SUM(list.发票金额) as 已报销金额 "
	// + " into #result"
	// + " from"
	// +
	// " (SELECT r.*,case when income.简称 is null then income.合同名称 else income.简称 end 收入合同名称, payout.合同名称 AS 支出合同名称,payoutsub.合同编号 AS 支出子合同编号"
	// + " ,sum(proceeds.收款金额) as 已付合同款,invoice.发票金额"
	// + " FROM xmis.con_收入支出合同对应表 AS r "
	// + " LEFT OUTER JOIN xmis.con_合同 AS income ON r.收入合同ID = income.合同ID "
	// + " LEFT OUTER JOIN xmis.con_合同 AS payout ON r.支出合同ID = payout.合同ID "
	// +
	// " LEFT OUTER JOIN xmis.con_合同 AS payoutsub ON r.支出子合同ID =payoutsub.合同ID"
	// +
	// " LEFT OUTER JOIN xmis.con_收款明细表 as proceeds on r.支出子合同ID = proceeds.合同ID"
	// +
	// " LEFT OUTER JOIN xmis.con_发票开具表 as invoice on r.支出子合同ID = invoice.合同ID"
	// + " WHERE  r.支出合同ID=?"
	// +
	// " group by 收入合同ID,支出合同ID,对应ID,金额,r.备注,支出子合同ID,income.简称,income.合同名称,payout.合同名称,payoutsub.合同编号,invoice.发票金额,r.最后修改日期,r.最后修改人ID) as list"
	// +
	// " group by list.收入合同ID,list.支出合同ID,list.对应ID,list.金额,list.备注,支出子合同ID,list.收入合同名称,list.支出合同名称,list.支出子合同编号,list.已付合同款,list.最后修改日期,list.最后修改人ID";

	public static final String getRelatedContractListPayout = " SELECT r.支出子合同ID , \n"
			+ " SUM(proceeds.收款金额) AS 已付合同款 \n"
			+ " INTO   #proceeds \n"
			+ " FROM   xmis.con_收入支出合同对应表 AS r \n"
			+ " JOIN xmis.con_收款明细表 AS proceeds ON r.支出子合同ID = proceeds.合同ID \n"
			+ " GROUP BY 支出子合同ID \n"

			+ " SELECT r.支出子合同ID , \n"
			+ " SUM(invoice.发票金额) AS 已报销金额 \n"
			+ " INTO   #invoice \n"
			+ " FROM   xmis.con_收入支出合同对应表 AS r \n"
			+ " JOIN xmis.con_发票开具表 AS invoice ON r.支出子合同ID = invoice.合同ID \n"
			+ " GROUP BY r.支出子合同ID \n"

			+ " SELECT r.收入合同ID , \n"
			+ " r.支出合同ID , \n"
			+ " r.对应ID , \n"
			+ " r.金额 , \n"
			+ " r.支出子合同ID , \n"
			+ " r.备注 , \n"
			+ " CASE WHEN income.简称 IS NULL THEN income.合同名称 \n"
			+ " ELSE income.简称 \n"
			+ " END 收入合同名称 , \n"
			+ " payout.合同名称 AS 支出合同名称 , \n"
			+ " payoutsub.合同编号 AS 支出子合同编号 , \n"
			+ " #proceeds.已付合同款 , \n"
			+ " #invoice.已报销金额 \n"
			+ " INTO   #result \n"
			+ " FROM   xmis.con_收入支出合同对应表 AS r \n"
			+ " JOIN xmis.con_合同 AS income ON r.收入合同ID = income.合同ID \n"
			+ " JOIN xmis.con_合同 AS payout ON r.支出合同ID = payout.合同ID \n"
			+ " JOIN xmis.con_合同 AS payoutsub ON r.支出子合同ID = payoutsub.合同ID \n"
			+ " LEFT JOIN #proceeds ON r.支出子合同ID = #proceeds.支出子合同ID \n"
			+ " LEFT JOIN #invoice ON r.支出子合同ID = #invoice.支出子合同ID \n"
			+ " WHERE  r.支出合同ID = ?";

	public static final String getRelatedContractListPayoutWithTotalInfoResults = " select"
			+ " null as 收入合同ID, null as 支出合同ID,null as 对应ID,"
			+ "SUM(金额) as 金额,null as 支出子合同ID,null as 备注,"
			+ "null as 收入合同名称,null as 支出合同名称,null as 支出子合同编号, sum(已付合同款) as 已付合同款,SUM(已报销金额 ) as 已报销金额 "
			+ " from #result" + " union all " + " select * from #result";

	public static String getContractListByTypeForCurrency = getContractListByType
			.replace("#result", "#contract")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.ContractNo
			+ like
			+ "?"
			+ and
			+ DBSchema.CurrencyType.CurrenyGraph
			+ "= ?"
			+ union
			+ select
			+ "*"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.Abbreviation
			+ like
			+ "?"
			+ and
			+ DBSchema.CurrencyType.CurrenyGraph + "= ?";

	public static String getSumInfoByIntegrationSimpleType(String contractSQL) {
		StringBuilder builder = new StringBuilder(contractSQL.replace(
				"#result", "#sum_result"));

		builder.append(select).append(DBSchema.CurrencyType.CurrenyGraph)
				.append(", sum(").append(DBSchema.Contract.ContractAmt)
				.append(") as ").append(DBSchema.Contract.ContractAmt)
				.append(", count(").append(DBSchema.Contract.ContractID)
				.append(") as ").append(DBSchema.Contract.ContractAmtCount)
				.append(", sum(").append(DBSchema.ChargedDetail.TotalAmtByID)
				.append(") as ").append(DBSchema.ChargedDetail.TotalAmtByID)
				.append(", sum(")
				.append(DBSchema.ChargedDetail.UnChargedAmtTotal)
				.append(") as ")
				.append(DBSchema.ChargedDetail.UnChargedAmtTotal)
				.append(", sum(").append(DBSchema.InvoiceDetail.TotalAmtByID)
				.append(") as ").append(DBSchema.InvoiceDetail.TotalAmtByID)
				.append(", sum(")
				.append(DBSchema.InvoiceDetail.UnAccountAmtTotalSum)
				.append(") as ")
				.append(DBSchema.InvoiceDetail.UnAccountAmtTotalSum)
				.append(into).append("#result" + from).append("#sum_result")
				.append(groupBy).append(DBSchema.CurrencyType.CurrenyGraph);
		return builder.toString();
	}

	public static String getWarningTotalCountForIntegrationSimpleType = getContractListByType
			.replace("#result", "#contract")
			+ select
			+ "*"
			+ into
			+ "#count_result"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.ContractNo
			+ like
			+ "?"
			+ union
			+ select
			+ "*"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.Abbreviation
			+ like
			+ "?"
			+ select
			+ "warning_days, COUNT(*) as warning_count"
			+ into
			+ "#result"
			+ from
			+ "#count_result"
			+ groupBy
			+ "warning_days";

	public static String getContractListByIntegrationNewType = getContractListByType
			.replace("#result", "#contract")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.SignatureDate
			+ ">=? " + and + DBSchema.Contract.SignatureDate + "<=? ";

	public static String getContractListByIntegrationNewTypeForCurrency = getContractListByType
			.replace("#result", "#contract")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.SignatureDate
			+ ">=? "
			+ and
			+ DBSchema.Contract.SignatureDate
			+ "<=? "
			+ and
			+ DBSchema.CurrencyType.CurrenyGraph + "=?";

	public static String getSumInfoByIntegrationNewType = getContractListByType
			.replace("#result", "#contract")
			+ select
			// + DBSchema.CurrencyType.CurrenyGraph
			// + ", sum("+DBSchema.Contract.ContractAmt+") as " +
			// DBSchema.Contract.ContractAmt
			+ DBSchema.CurrencyType.CurrenyGraph
			+ ", sum("
			+ DBSchema.Contract.ContractAmt
			+ ") as "
			+ DBSchema.Contract.ContractAmt
			+ ", count(*) as "
			+ DBSchema.Contract.ContractAmtCount
			+ ", sum("
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ ") as "
			+ DBSchema.ChargedDetail.TotalAmtByID
			+ ", sum("
			+ DBSchema.ChargedDetail.UnChargedAmtTotal
			+ ") as "
			+ DBSchema.ChargedDetail.UnChargedAmtTotal
			+ ", sum("
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ ") as "
			+ DBSchema.InvoiceDetail.TotalAmtByID
			+ ", sum("
			+ DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			+ ") as "
			+ DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			+ into
			+ "#result"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.SignatureDate
			+ ">=? "
			+ and
			+ DBSchema.Contract.SignatureDate
			+ "<=? "
			+ groupBy
			+ DBSchema.CurrencyType.CurrenyGraph

	;

	public static String getContractListByTypeFilterWarning = getContractListByType
			.replace("#result", "#contract")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from + "#contract" + where + "warning_days = ?";

	public static String getContractListForParentBySerch = getContractListByType
			.replace("#result", "#contract")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.ParentID
			+ " IS NULL "
			+ and
			+ DBSchema.Contract.ContractNo
			+ like
			+ "?"
			+ union
			+ select
			+ "*"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.ParentID
			+ " IS NULL "
			+ and
			+ DBSchema.Contract.Abbreviation + like + "?";

	public static String getChildContractList = select
			+ DBSchema.Contract.ContractID + "," + DBSchema.Contract.ContractNo
			+ "," + DBSchema.Contract.ContractName + ","
			+ DBSchema.Contract.Abbreviation + ","
			+ DBSchema.Contract.ContractAmt + ","
			+ DBSchema.CurrencyType.CurrenyGraph + ","
			+ DBSchema.Contract.ParentID + from + DBSchema.Contract.TableName
			+ "," + DBSchema.CurrencyType.TableName + where
			+ DBSchema.Contract.ParentID + "=?" + and
			+ DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.ContractTypeID + "*="
			+ DBSchema.CurrencyType.TableName + "."
			+ DBSchema.CurrencyType.CurrencyID;

	public static String getAttachmentListByRefID = select
			+ DBSchema.Attachment.TableName + "."
			+ DBSchema.Attachment.AttachmentID + ","
			+ DBSchema.Attachment.TableName + "."
			+ DBSchema.Attachment.AttachmentName + ", "
			+ DBSchema.AttachmentType.TableName + "."
			+ DBSchema.AttachmentType.AttachmentTypeDesc + ","

			+ "Convert(varchar(10)," + DBSchema.Attachment.TableName + "."
			+ DBSchema.Attachment.UploadDate + ",101) as " + " upload_date "
			+ ", " + "(RTRIM(" + DBSchema.Employee.FirstName + ")+RTRIM("
			+ DBSchema.Employee.LastName + "))  " + "  as " + " uploader_name "
			+ intoResult + from + DBSchema.Attachment.TableName + ","
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
			+ DBSchema.Attachment.TableName
			+ "("
			+ DBSchema.Attachment.AttachmentID
			+ ","// 1
			+ DBSchema.Attachment.UploadDate + ","
			+ DBSchema.Attachment.UploaderID
			+ ","// 2
			+ DBSchema.Attachment.AttahcmentDesc
			+ ","// 3
			+ DBSchema.Attachment.AttachmentName
			+ ","// 4
			+ DBSchema.Attachment.AttachmentContent
			+ ","// 5
			+ DBSchema.Attachment.AttachmentTypeID
			+ ","// 6
			+ DBSchema.Attachment.REFID
			+ ","// 7
			+ DBSchema.Attachment.LastUpdateDate + ","
			+ DBSchema.Attachment.LastUpdateEmpId + ","// 8
			+ DBSchema.Attachment.KeyWords + ","// 9
			+ DBSchema.Attachment.FilePath + ","// 10
			+ DBSchema.Attachment.DeleteFlag// 11
			+ ")\n values (?,GETDATE(),?,?,?,?,?,?,GETDATE(),?,?,?,?)";

	public static String getAttachmentContent = select + " * " + from
			+ DBSchema.Attachment.TableName + where
			+ DBSchema.Attachment.AttachmentID + "=?";

	public static String createNullContract = insert + into
			+ DBSchema.Contract.TableName + "(" + DBSchema.Contract.ContractID
			+ "," + DBSchema.Contract.ContractNo + ","
			+ DBSchema.Contract.ContractName + ","
			+ DBSchema.Contract.UpdateDate + "," + DBSchema.Contract.RegisterID
			+ ") values(?,'','',GETDATE(),?)";

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
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID

			+ getDeptListByCompanyID2 + and + DBSchema.Department.TableName
			+ "." + DBSchema.Department.DepartmentName + "='"
			+ WebKeys.New_Company_Simple_For_Dept_Other + "'";

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
		return getDeptListBySelfCompanyID1 + intoResult
				+ getDeptListBySelfCompanyID2;
	}

	public static String getDeptListBySelfCompanyID = getDeptListBySelfCompanyID();

	public static String getDeptListByDivID = select + "*" + from
			+ DBSchema.DeptBelongToDept.TableName + where
			+ DBSchema.DeptBelongToDept.TableName + "."
			+ DBSchema.DeptBelongToDept.ParentsID + "=?";

	public static String getDeptListByCompID = select + "*" + from
			+ DBSchema.DepartmentBelongToCompany.TabelName + where
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID + "=?";

	private static String getPeasonListByCompanyIntoTempTable = into + "#dept ";

	private static String getPeasonListBySelfFromTempTable = select
			+ "#dept.*, " + DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "," + "(RTRIM("
			+ DBSchema.Employee.FirstName + ")+RTRIM("
			+ DBSchema.Employee.LastName + ")) user_name, "
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.Position + from + "#dept, "
			+ DBSchema.EmployeeBelongToDept.TableName + ","
			+ DBSchema.Employee.TableName + where + "#dept.deptID="
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + and
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "="
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID
			+ and
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.IsPartTime
			+ "=1" // 全职
			+ and + "(" + DBSchema.Employee.TableName
			+ "."
			+ DBSchema.Employee.DisabledFlag
			+ "=0" // 非禁用
			+ or + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.DisabledFlag + " is NULL)" + and
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + ">0";

	private static String getPeasonListByCompanyFromTempTable = select
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
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID
			+ and
			+ DBSchema.EmployeeBelongToDept.TableName
			+ "."
			+ DBSchema.EmployeeBelongToDept.IsPartTime
			+ "=1" // 全职
			+ and + "(" + DBSchema.Employee.TableName
			+ "."
			+ DBSchema.Employee.DisabledFlag
			+ "=0" // 非禁用
			+ or + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.DisabledFlag + " is NULL)" + and
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + ">0";

	private static String subGetPeasonListBySearch = and + "RTRIM("
			+ Employee.FirstName + ")+RTRIM(" + Employee.LastName + ")" + like
			+ "?";

	private static String subOrderByStr = orderBy + "deptID";

	private static String getPeasonListBySelfCompany() {

		return getDeptListBySelfCompanyID1
				+ getPeasonListByCompanyIntoTempTable
				+ getDeptListBySelfCompanyID2
				+ getPeasonListBySelfFromTempTable;

	}

	public static String getPeasonListBySelfCompany = getPeasonListBySelfCompany()
			+ subOrderByStr;

	public static String getPeasonListBySelf = getPeasonListBySelfCompany()
			+ subGetPeasonListBySearch + subOrderByStr;

	private static String getPeasonListByCompany() {

		return getDeptListByCompanyID1 + getPeasonListByCompanyIntoTempTable
				+ getDeptListByCompanyID2 + getPeasonListByCompanyFromTempTable;
	}

	public static String getPeasonListByCompany = getPeasonListByCompany()
			+ subOrderByStr;

	public static String getPeasonListBySearch = getPeasonListByCompany()
			+ subGetPeasonListBySearch + subOrderByStr;

	public static String createNewSimpleCompany = insert + into
			+ DBSchema.Company.TableName + "(" + DBSchema.Company.CompanyID
			+ "," + DBSchema.Company.CompanyName + ","
			+ DBSchema.Company.Abbreviation + ","
			+ DBSchema.Company.BusinessRelationID + ","
			+ DBSchema.Company.UpdateDate + "," + DBSchema.Company.UpdateID
			+ ") values(?,?,?,-1,GETDATE(),?)";

	public static String setDeptBelongToCompany = insert + into
			+ DBSchema.DepartmentBelongToCompany.TabelName + "("
			+ DBSchema.DepartmentBelongToCompany.CompanyID + ","
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + ","
			+ DBSchema.DepartmentBelongToCompany.DirectlyFlag
			+ ") values(?,?,?)";

	public static String getCompIDByDeptID = select
			+ DBSchema.DeptBelongToCompany.companyID + from
			+ DBSchema.DeptBelongToCompany.TableName + where
			+ DBSchema.DeptBelongToCompany.DepartmentID + "=?";

	public static String createNewSimpleCRM = insert + into
			+ DBSchema.ContactPerson.TableName + "("
			+ DBSchema.ContactPerson.ContactPersonID + ","
			+ DBSchema.ContactPerson.FirstName + ","
			+ DBSchema.ContactPerson.CompanyID + ","
			+ DBSchema.ContactPerson.DepartmentID + ","
			+ DBSchema.ContactPerson.UpdateDate + ","
			+ DBSchema.ContactPerson.UpdateID + ") values(?,?,?,?,GETDATE(),?)";

	public static String getCompanyID = select + DBSchema.Company.CompanyID
			+ from + DBSchema.Company.TableName + where
			+ DBSchema.Company.CompanyName + "=?";

	public static String getDepartmentIDByCompany = select
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + from
			+ DBSchema.Department.TableName + ","
			+ DBSchema.DepartmentBelongToCompany.TabelName + where
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentName + "=?" + and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID + "=?" + and
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + "="
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID;

	public static String getContactPeasonIDByCompany = select
			+ DBSchema.ContactPerson.ContactPersonID + from
			+ DBSchema.ContactPerson.TableName + where + "rtrim("
			+ DBSchema.ContactPerson.FirstName + ")+rtrim("
			+ DBSchema.ContactPerson.LastName + ")=?" + and
			+ DBSchema.ContactPerson.CompanyID + "=?";

	public static String getUserIDByCompany = select
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID + from
			+ DBSchema.Employee.TableName + ","
			+ DBSchema.EmployeeBelongToDept.TableName + ","
			+ DBSchema.DepartmentBelongToCompany.TabelName + where
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID + "=?" + and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + "=?" + and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + "="
			+ DBSchema.EmployeeBelongToDept.TableName + "."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + and + "RTRIM("
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.FirstName
			+ ")+RTRIM(" + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.LastName + ")=?";

	public static String updateContractInfo = update
			+ DBSchema.Contract.TableName + set
			+ DBSchema.Contract.ContractTypeID
			+ "=?," // 1
			+ DBSchema.Contract.SourceTypeID
			+ "=?," // 2
			+ DBSchema.Contract.GovermentContractSourceID
			+ "=?," // 3
			+ DBSchema.Contract.ContractNo
			+ "=?," // 4
			+ DBSchema.Contract.ContractName
			+ "=?," // 5
			+ DBSchema.Contract.Abbreviation
			+ "=?," // 6
			+ DBSchema.Contract.ContractSummary
			+ "=?," // 7
			+ DBSchema.Contract.ParentID
			+ "=?," // 8
			+ DBSchema.Contract.WarrantyDefinition
			+ "=?," // 9
			+ DBSchema.Contract.WarrantyStartDate
			+ "=?," // 10
			+ DBSchema.Contract.WarrantyEndDate
			+ "=?," // 11
			+ DBSchema.Contract.ContractAmt
			+ "=?," // 12
			+ DBSchema.Contract.CurrencyID
			+ "=?," // 13
			+ DBSchema.Contract.SignatureDate
			+ "=?," // 14
			+ DBSchema.Contract.BuyerID
			+ "=?," // 15
			+ DBSchema.Contract.DeptIDOfBuyer
			+ "=?," // 16
			+ DBSchema.Contract.BuyerRepresentativeID
			+ "=?," // 17
			+ DBSchema.Contract.VendorID
			+ "=?," // 18
			+ DBSchema.Contract.DeptIDOfVendor
			+ "=?," // 19
			+ DBSchema.Contract.VendorRepresentativeID
			+ "=?," // 20
			+ DBSchema.Contract.Remark
			+ "=?," // 21
			+ DBSchema.Contract.DeliveryID
			+ "=?," // 22
			+ DBSchema.Contract.LastModificationDate + "=GETDATE(),"
			+ DBSchema.Contract.LastModificatorID + "=?," // 23
			+ DBSchema.Contract.WarrantyLimit + "=?," // 24
			+ DBSchema.Contract.WarrantyUnit + "=?," // 25
			+ DBSchema.Contract.ProjectID + "=?," // 26
			+ DBSchema.Contract.DirectorID + "=?" // 27
			+ where + DBSchema.Contract.ContractID + "=?" // 28
	;
	public static String updateContractInfoNew = update
			+ DBSchema.Contract.TableName + set
			+ DBSchema.Contract.ContractTypeID
			+ "=?," // 1
			+ DBSchema.Contract.SourceTypeID
			+ "=?," // 2
			+ DBSchema.Contract.GovermentContractSourceID
			+ "=?," // 3
			+ DBSchema.Contract.ContractNo
			+ "=?," // 4
			+ DBSchema.Contract.ContractName
			+ "=?," // 5
			+ DBSchema.Contract.Abbreviation
			+ "=?," // 6
			+ DBSchema.Contract.ContractSummary
			+ "=?," // 7
			+ DBSchema.Contract.ParentID
			+ "=?," // 8
			+ DBSchema.Contract.WarrantyDefinition
			+ "=?," // 9
			+ DBSchema.Contract.WarrantyStartDate
			+ "=?," // 10
			+ DBSchema.Contract.WarrantyEndDate
			+ "=?," // 11
			+ DBSchema.Contract.ContractAmt
			+ "=?," // 12
			+ DBSchema.Contract.CurrencyID
			+ "=?," // 13
			+ DBSchema.Contract.SignatureDate
			+ "=?," // 14
			+ DBSchema.Contract.BuyerID
			+ "=?," // 15
			+ DBSchema.Contract.DeptIDOfBuyer
			+ "=?," // 16
			+ DBSchema.Contract.BuyerRepresentativeID
			+ "=?," // 17
			+ DBSchema.Contract.VendorID
			+ "=?," // 18
			+ DBSchema.Contract.DeptIDOfVendor
			+ "=?," // 19
			+ DBSchema.Contract.VendorRepresentativeID
			+ "=?," // 20
			+ DBSchema.Contract.Remark
			+ "=?," // 21
			+ DBSchema.Contract.DeliveryID
			+ "=?," // 22
			+ DBSchema.Contract.LastModificationDate + "=GETDATE(),"
			+ DBSchema.Contract.LastModificatorID + "=?," // 23
			+ DBSchema.Contract.WarrantyLimit + "=?," // 24
			+ DBSchema.Contract.WarrantyUnit + "=?," // 25
			+ DBSchema.Contract.ProjectID + "=?," // 26
			+ DBSchema.Contract.DirectorID + "=?," // 27
			+ DBSchema.Contract.ValidityStartDate + "=?,"// 28
			+ DBSchema.Contract.ValidityEndDate + "=?,"// 29
			+ DBSchema.Contract.ValidityLimit + "=?,"// 30
			+ DBSchema.Contract.ValidityUnit + "=?,"// 31
			+ DBSchema.Contract.newflag + "=?"// 32
			+ where + DBSchema.Contract.ContractID + "=?" // 33
	;

	public static String getPaymentConditionByContract = "\nSET NOCOUNT ON \n declare @totalImcome DECIMAL(18,2) \n"
			+ " declare @count int \n"
			+ " declare @i int \n"
			+ " declare @contractID int  \n"
			+ " declare @projectID nvarchar(500)\n"
			+ " set @contractID = ?  \n"
			+ " set @i = 0  \n"
			+ " select @projectID = c.项目ID from xmis.con_合同 as c where 合同ID = @contractID\n"

			+ " select * into #plan from xmis.prj_项目进度计划 where 项目ID = @projectID\n"

			+ getGLIncomeByProjectCode.replace("?", "@projectID")

			+ " SELECT @totalImcome = sum(MB) from #result " // "group by MB"

			+ " select @count = count(*) \n"
			+ " FROM xmis.con_收款条件表,xmis.prj_项目状态 \n"
			+ " WHERE xmis.con_收款条件表.合同ID=@contractID \n"
			+ " AND xmis.con_收款条件表.对应项目状态*=xmis.prj_项目状态.项目状态ID \n"

			+ " SELECT identity(int,0,1) as row_no1  \n"
			+ " ,xmis.con_收款条件表.合同ID  \n"
			+ " ,xmis.con_收款条件表.收款条件ID  \n"
			+ " ,xmis.con_收款条件表.收款条件描述  \n"
			+ " ,xmis.con_收款条件表.对应项目状态  \n"
			+ " ,xmis.con_收款条件表.收款日期  \n"
			+ " ,xmis.con_收款条件表.延迟天数  \n"
			+ " ,xmis.con_收款条件表.收款百分比  \n"
			+ " ,xmis.con_收款条件表.收款金额  \n"
			+ " ,xmis.con_收款条件表.收款节点名称  \n"
			+ " , xmis.prj_项目状态.项目状态 as 项目状态  \n"
			+ " ,CONVERT(DECIMAL(18,2),NULL) as realIncome   \n"
			+ " into #temp2   \n"
			+ " FROM xmis.con_收款条件表,xmis.prj_项目状态   \n"
			+ " WHERE xmis.con_收款条件表.合同ID=@contractID   \n"
			+ " AND xmis.con_收款条件表.对应项目状态*=xmis.prj_项目状态.项目状态ID  \n"
			+ " select #temp2.*  \n"
			+ " ,#plan.预计开始日期 as 预计开始日期  \n"
			+ " ,dateadd(d,#temp2.延迟天数,#plan.预计开始日期) as 预计到款日期  \n"
			+ " ,Convert(varchar(10),#plan.预计开始日期,101) as start_date \n"
			+ " ,Convert(varchar(10),dateadd(d,#temp2.延迟天数,#plan.预计开始日期),101) as payment_date  \n"
			+ " ,Convert(varchar(10),#plan.实际开始日期,101) as 实际开始日期  \n"
			+ " ,Convert(varchar(10),dateadd(d,#temp2.延迟天数,#plan.实际开始日期),101) as 应收款日期  \n"
			+ " ,Convert(varchar(10),#plan.实际开始日期,101) as realDate   \n"
			+ " ,#plan.实际开始日期 as td \n"
			+ " into #temp1   \n"
			+ " from #temp2,#plan   \n"
			+ " where #temp2.对应项目状态 *= #plan.项目状态ID  \n"
			+ " order by #plan.预计开始日期  \n"
			+ " select identity(int,0,1) as row_no,#temp1.* into #temp from #temp1 order by 预计到款日期,td \n"

			+ "\n update #temp set payment_date = Convert(varchar(10),t.收款日期,101) from #temp as t where payment_date is null \n"

			+ " while(@i < @count and @totalImcome > 0) begin \n"
			+ " declare @total1 DECIMAL(18,2) \n"
			+ " declare @total2 DECIMAL(18,2) \n"
			+ " select @total1 = 收款金额 from #temp where row_no = @i \n"
			+ " if(@total1<=@totalImcome) begin \n"
			+ " set @total2 = @total1 \n"
			+ " set @totalImcome = @totalImcome - @total1 \n"
			+ " end \n"
			+ " else begin \n"
			+ " set @total2 = @totalImcome \n"
			+ " set @totalImcome = 0 \n"
			+ " end \n"
			+ " update #temp set realIncome=@total2 where row_no = @i \n"
			+ " set @i = @i + 1 \n"
			+ " end \n"
			+ " select * from #temp order by 预计到款日期,td";

	public static String getAllProjectStatus = select
			+ DBSchema.ProjectStatus.TableName + ".*" + from
			+ DBSchema.ProjectStatus.TableName + where
			+ DBSchema.ProjectStatus.ProjectStatusName + like + "?";

	public static String intsertProjectStatus = insert + into
			+ DBSchema.ProjectStatus.TableName + "("
			+ DBSchema.ProjectStatus.ProjectStatusID + ","
			+ DBSchema.ProjectStatus.ProjectStatusName + ","
			+ DBSchema.ProjectStatus.ProjectID + ","
			+ DBSchema.ProjectStatus.ProjectStatusDescription + ","
			+ DBSchema.ProjectStatus.SelfDefinitionFlag + ")" + values
			+ "(?,?,?,?,1)";

	public static String insertPaymentCondition = insert + into
			+ DBSchema.PaymentCondition.TableName + "("
			+ DBSchema.PaymentCondition.ContractID + "," // 1
			+ DBSchema.PaymentCondition.PaymentConditionID + "," // 2
			+ DBSchema.PaymentCondition.PaymentConditionDescription + "," // 3
			+ DBSchema.PaymentCondition.ProjectStatusID + "," // 4
			+ DBSchema.PaymentCondition.DelayDays + "," // 5
			// + DBSchema.PaymentCondition.PaymentDate + "," //6
			+ DBSchema.PaymentCondition.ManualFlag // + "," //7
			// + DBSchema.PaymentCondition.PaymentPercent + "," //8
			// + DBSchema.PaymentCondition.PaymentAmt //9
			+ ") values(?,?,'',-1,0,1)" // ,?,?,?,?,?,?,?)"
	;

	public static String savePaymentCondition = update
			+ DBSchema.PaymentCondition.TableName + set
			+ DBSchema.PaymentCondition.PaymentConditionDescription + "=?," // 1
			+ DBSchema.PaymentCondition.ProjectStatusID + "=?," // 2
			+ DBSchema.PaymentCondition.DelayDays + "=?," // 3
			+ DBSchema.PaymentCondition.PaymentPercent + "=?," // 4
			+ DBSchema.PaymentCondition.PaymentNodeName + "=?," // 5
			+ DBSchema.PaymentCondition.PaymentAmt + "=?," // 6
			+ DBSchema.PaymentCondition.LastModificationDate + "=?," // 7
			+ DBSchema.PaymentCondition.LastModificatorID + "=?" // 8
			+ where + DBSchema.PaymentCondition.ContractID + "=?" // 9
			+ and + DBSchema.PaymentCondition.PaymentConditionID + "=?" // 10
	;

	public static String insertPaymentConditionLog = insert + into
			+ DBSchema.PaymentConditionManualLog.TableName + "("
			+ DBSchema.PaymentConditionManualLog.LogID
			+ "," // 1
			+ DBSchema.PaymentConditionManualLog.ContractID
			+ "," // 2
			+ DBSchema.PaymentConditionManualLog.PaymentConditionID
			+ "," // 3
			+ DBSchema.PaymentConditionManualLog.PaymentDate
			+ "," // 4
			+ DBSchema.PaymentConditionManualLog.PaymentPercent
			+ "," // 5
			+ DBSchema.PaymentConditionManualLog.PaymentAmt
			+ "," // 6
			+ DBSchema.PaymentConditionManualLog.ChangedReason
			+ "," // 7
			+ DBSchema.PaymentConditionManualLog.ChangedDate + ","
			+ DBSchema.PaymentConditionManualLog.ChangedID // 8
			+ ") values(?,?,?,?,?,?,?,GETDATE(),?)"

	;

	public static String deletePaymentCondition = delete
			+ DBSchema.PaymentCondition.TableName + where
			+ DBSchema.PaymentCondition.TableName + "."
			+ DBSchema.PaymentCondition.ContractID + "=?" + and
			+ DBSchema.PaymentCondition.TableName + "."
			+ DBSchema.PaymentCondition.PaymentConditionID + "=?";

	public static String getCountByIDFromTab(String columnName, String tableName) {

		return select + "count(*)" + from + tableName + where + columnName
				+ "=?";
	}

	public static String updatePaymnetConditionFortributed = update
			+ DBSchema.PaymentCondition.TableName + set
			+ DBSchema.PaymentCondition.PaymentPercent + "=cast("
			+ DBSchema.PaymentCondition.PaymentAmt + "/?*100 as decimal(15,0))"
			+ where + DBSchema.PaymentCondition.ContractID + "=?";

	public static String insertChargedDetail = insert + into
			+ DBSchema.ChargedDetail.TableName + "("
			+ DBSchema.ChargedDetail.ContractID + "," // 1
			+ DBSchema.ChargedDetail.ChargedID + "," // 2
			+ DBSchema.ChargedDetail.ChargedAmt + "," // 3
			+ DBSchema.ChargedDetail.ChargedDate + "," // 4
			+ DBSchema.ChargedDetail.TransactorID // 5
			+ ") values(?,?,0,GETDATE(),?)";

	public static String saveChargedDetail = update
			+ DBSchema.ChargedDetail.TableName + set
			+ DBSchema.ChargedDetail.ChargedAmt
			+ "=?," // 1
			+ DBSchema.ChargedDetail.ChargedDate
			+ "=?," // 2
			+ DBSchema.ChargedDetail.TransactorID
			+ "=?," // 3
			+ DBSchema.ChargedDetail.UpdateID
			+ "=?," // 4
			+ DBSchema.ChargedDetail.UpdateDate + "=GETDATE(),"
			+ DBSchema.ChargedDetail.ChargedRemark + "=?" // 5
			+ where + DBSchema.ChargedDetail.ContractID + "=?" // 6
			+ and + DBSchema.ChargedDetail.ChargedID + "=?" // 7
	;

	public static String deleteChargedDetail = delete
			+ DBSchema.ChargedDetail.TableName + where
			+ DBSchema.ChargedDetail.ContractID + "=?" + and
			+ DBSchema.ChargedDetail.ChargedID + "=?";

	/**
	 * saveInvoiceDetail
	 */
	public static String insertInvoiceDetail = insert + into
			+ DBSchema.InvoiceDetail.TableName + "("
			+ DBSchema.InvoiceDetail.ContractID + "," // 1
			+ DBSchema.InvoiceDetail.InvoiceNo + "," // 2
			+ DBSchema.InvoiceDetail.TransactorID + "," // 3
			+ DBSchema.InvoiceDetail.InvoiceAmt + "," // 4
			+ DBSchema.InvoiceDetail.InvoiceDate + "," // 5
			+ DBSchema.InvoiceDetail.UpdateID // 6
			+ ") values(?,?,0,0,GETDATE(),?)";

	public static String insertReceiptDetail = insert + into
			+ DBSchema.Receipt.TableName + "(" + DBSchema.Receipt.ContractID
			+ "," // 1
			+ DBSchema.Receipt.ReceiptID + "," // 2
			+ DBSchema.Receipt.TransactorID + "," // 3
			+ DBSchema.Receipt.ReceiptAmt + "," // 4
			+ DBSchema.Receipt.ReceiptDate + "," // 5
			+ DBSchema.Receipt.UpdateID // 6
			+ ") values(?,?,0,0,GETDATE(),?)";

	public static String saveInvoiceDetail = update
			+ DBSchema.InvoiceDetail.TableName + set
			+ DBSchema.InvoiceDetail.InvoiceAmt
			+ "=?," // 1
			+ DBSchema.InvoiceDetail.InvoiceDate
			+ "=?," // 2
			+ DBSchema.InvoiceDetail.TransactorID
			+ "=?," // 3
			+ DBSchema.InvoiceDetail.UpdateID
			+ "=?," // 4
			+ DBSchema.InvoiceDetail.UpdateDate + "=GETDATE(),"
			+ DBSchema.InvoiceDetail.Remark + "=?," // 5
			+ DBSchema.InvoiceDetail.InvoiceCode + "=?," // 6
			+ DBSchema.InvoiceDetail.ReceiverID + "=?,"// 7
			+ DBSchema.InvoiceDetail.MailCode + "=?"// 8
			+ where + DBSchema.InvoiceDetail.ContractID + "=?" // 9
			+ and + DBSchema.InvoiceDetail.InvoiceNo + "=?" // 10
	;

	public static String saveReceiptDetail = update
			+ DBSchema.Receipt.TableName + set
			+ DBSchema.Receipt.ReceiptAmt
			+ "=?," // 1
			+ DBSchema.Receipt.ReceiptDate
			+ "=?," // 2
			+ DBSchema.Receipt.TransactorID
			+ "=?," // 3
			+ DBSchema.Receipt.UpdateID
			+ "=?," // 4
			+ DBSchema.Receipt.UpdateDate + "=GETDATE(),"
			+ DBSchema.Receipt.Remark + "=?," // 5
			+ DBSchema.Receipt.ReceiptCode + "=?," // 6
			+ DBSchema.Receipt.ReceiverID + "=?,"// 7
			+ DBSchema.Receipt.MailCode + "=?"// 8
			+ where + DBSchema.Receipt.ContractID + "=?" // 9
			+ and + DBSchema.Receipt.ReceiptID + "=?" // 10
	;

	public static String deleteInvoiceDetail = delete
			+ DBSchema.InvoiceDetail.TableName + where
			+ DBSchema.InvoiceDetail.ContractID + "=?" + and
			+ DBSchema.InvoiceDetail.InvoiceNo + "=?";

	public static String deleteReceiptDetail = delete
			+ DBSchema.Receipt.TableName + where + DBSchema.Receipt.ContractID
			+ "=?" + and + DBSchema.Receipt.ReceiptID + "=?";

	public static String deleteDeptBelongToCompany = delete + from
			+ DBSchema.DeptBelongToCompany.TableName + where
			+ DBSchema.DeptBelongToCompany.DepartmentID + "=? " + and
			+ DBSchema.DeptBelongToCompany.companyID + "=?";

	public static String deleteDeptBelongToDept = delete + from
			+ DBSchema.DeptBelongToDept.TableName + where
			+ DBSchema.DeptBelongToDept.DeptID + "=? " + and
			+ DBSchema.DeptBelongToDept.ParentsID + "=?";

	public static String getRunContractInTimeScope1 = getContractListByType
			.replace("#result", "#contract")

			+ " DECLARE @beginDate datetime  set @beginDate = ? DECLARE @endDate datetime  set @endDate = ? "
			+ " SELECT xmis.con_收款明细表.合同ID, sum(xmis.con_收款明细表.收款金额) as contract_payed_amt, 0 as contract_invoice_amt, 0 as contract_estimate_amt, CONVERT(DECIMAL(18, 2), 0) AS MB,CONVERT(DECIMAL(18, 2), 0) as payAmt"
			+ " INTO #contract_pay_invoice"
			+ " FROM xmis.con_收款明细表"
			+ " WHERE xmis.con_收款明细表.收款日期>=@beginDate" // 1
			+ " AND xmis.con_收款明细表.收款日期<=@endDate" // 2
			+ " GROUP BY xmis.con_收款明细表.合同ID"
			+ " UNION "

			+ " SELECT xmis.con_发票开具表.合同ID, 0, sum(xmis.con_发票开具表.发票金额), 0 ,0,0"
			+ " FROM xmis.con_发票开具表"
			+ " WHERE xmis.con_发票开具表.开具日期>=@beginDate" // 3
			+ " AND xmis.con_发票开具表.开具日期<=@endDate" // 4
			+ " GROUP BY xmis.con_发票开具表.合同ID"

			+ " if(@contractType = 1) begin"
			+ " insert into #contract_pay_invoice"
			+ " SELECT xmis.con_收款条件表.合同ID, 0, 0, sum(xmis.con_收款条件表.收款金额) ,0,0"
			+ " FROM xmis.con_收款条件表"
			+ " WHERE xmis.con_收款条件表.收款日期>= @beginDate" // 5
			+ " AND xmis.con_收款条件表.收款日期<=@endDate" // 6
			+ " GROUP BY xmis.con_收款条件表.合同ID"
			+ " insert into #contract_pay_invoice"
			+ " select #contract.合同ID,0,0,0,SUM(mc) - SUM(md),0"
			+ " FROM xmis.fas_GL_accvouch,xmis.fas_income,xmis.fas_codeType,"
			+ " #contract"
			+ " WHERE xmis.fas_GL_accvouch.year = xmis.fas_income.year        "
			+ " AND xmis.fas_GL_accvouch.iperiod = xmis.fas_income.iperiod "
			+ " AND xmis.fas_GL_accvouch.ino_id = xmis.fas_income.ino_id        "
			+ " AND xmis.fas_GL_accvouch.citem_id = xmis.fas_income.citem_id "
			+ " AND xmis.fas_codeType.type = 0        "
			+ " AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode "
			+ " AND xmis.fas_GL_accvouch.year = xmis.fas_codeType.year "
			+ " and xmis.fas_income.project_code = #contract.项目编码"
			+ " and "
			+ " xmis.fas_GL_accvouch.dbill_date >=@beginDate" // 7
			+ " and "
			+ " xmis.fas_GL_accvouch.dbill_date <=@endDate" // 8
			+ " GROUP BY xmis.fas_GL_accvouch.year , xmis.fas_GL_accvouch.iperiod , xmis.fas_GL_accvouch.ino_id , "
			+ " xmis.fas_GL_accvouch.dbill_date , cdigest , xmis.fas_income.project_code  ,#contract.合同ID"
			+ " HAVING  SUM(mc) - SUM(md) <> 0 ORDER BY xmis.fas_GL_accvouch.dbill_date "
			+ " if(@beginDate<'2008-01-01') begin" // 9
			+ " insert into #contract_pay_invoice"
			+ " select #contract.合同ID,0,0,0,SUM(income_mb),0"
			+ " FROM xmis.fas_qch , xmis.fas_fitemss00,"
			+ " #contract"
			+ " WHERE  xmis.fas_fitemss00.year = 2008 and  xmis.fas_qch.citem_id = xmis.fas_fitemss00.citemcode and project_code = "
			+ " #contract.项目编码"
			+ " GROUP BY project_code,#contract.合同ID"

			+ " insert into #contract_pay_invoice"
			+ " select #contract.合同ID ,0,0,0, 0 , SUM(payout_mb)"
			+ " FROM xmis.fas_qcjcb , xmis.fas_fitemss00 ,"
			+ " #contract"
			+ " WHERE xmis.fas_fitemss00.year = 2008 "
			+ " and xmis.fas_qcjcb.citem_id = xmis.fas_fitemss00.citemcode "
			+ " and project_code = #contract.项目编码"
			+ " GROUP BY project_code ,#contract.合同ID"
			+ " end "

			+ " insert into #contract_pay_invoice"
			+ " select 合同ID,0,0,0,0,SUM(md - mc) from ("
			+ " select #contract.合同ID,xmis.fas_GL_accvouch.md,xmis.fas_GL_accvouch.mc"
			+ " FROM xmis.fas_GL_accvouch , xmis.fas_fitemss00 , xmis.fas_codeType ,#contract"
			+ " WHERE xmis.fas_GL_accvouch.year = xmis.fas_fitemss00.year        "
			+ " AND xmis.fas_GL_accvouch.citem_id = xmis.fas_fitemss00.citemcode "
			+ " AND xmis.fas_fitemss00.bclose = 0        "
			+ " AND xmis.fas_GL_accvouch.iperiod > 0    "
			+ " AND xmis.fas_GL_accvouch.iperiod <= 12 "
			+ " AND NOT ( fas_GL_accvouch.year = 2011"
			+ " AND fas_GL_accvouch.iperiod = 8  "
			+ " AND fas_GL_accvouch.ino_id IN ( 327, 466 ))"
			+ " AND xmis.fas_codeType.type = 1"
			+ " AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode "
			+ " AND project_code = #contract.项目编码"
			+ " and "
			+ " xmis.fas_GL_accvouch.dbill_date >=@beginDate" // 10
			+ " and "
			+ " xmis.fas_GL_accvouch.dbill_date <=@endDate" // 11
			+ " GROUP BY xmis.fas_GL_accvouch.year , xmis.fas_GL_accvouch.iperiod , xmis.fas_GL_accvouch.ino_id , project_code , "
			+ " citem_id , xmis.fas_GL_accvouch.cdigest , xmis.fas_GL_accvouch.dbill_date ,#contract.合同ID,xmis.fas_GL_accvouch.md,xmis.fas_GL_accvouch.mc"
			+ " ) as a GROUP BY 合同ID,md,mc  HAVING  SUM(md - mc) <> 0"
			+ " end"

			+ " SELECT 合同ID, sum(contract_payed_amt) as contract_payed_amt, sum(contract_invoice_amt) as contract_invoice_amt, sum(contract_estimate_amt) as contract_estimate_amt,SUM(MB) as officialReceiptsMB,SUM(payAmt) as officialReceiptsPayAmt"
			+ " INTO #contract_pay_invoice_sum"
			+ " FROM #contract_pay_invoice"
			+ " GROUP BY 合同ID"

			+ " SELECT #contract.*, contract_payed_amt, contract_invoice_amt,contract_estimate_amt,officialReceiptsMB,officialReceiptsPayAmt "
			+ " INTO #result"
			+ " FROM #contract,#contract_pay_invoice_sum "
			+ " WHERE #contract.合同ID= #contract_pay_invoice_sum.合同ID ";

	public static String getRunContractInTimeScope = getContractListByType
			.replace("#result", "#contract")
			+ select
			+ DBSchema.ChargedDetail.TableName
			+ "."
			+ DBSchema.ChargedDetail.ContractID
			+ ", sum("
			+ DBSchema.ChargedDetail.TableName
			+ "."
			+ DBSchema.ChargedDetail.ChargedAmt
			+ ") as contract_payed_amt, 0 as contract_invoice_amt, 0 as contract_estimate_amt "
			+ into
			+ "#contract_pay_invoice"
			+ from
			+ DBSchema.ChargedDetail.TableName
			+ where
			+ DBSchema.ChargedDetail.TableName
			+ "."
			+ DBSchema.ChargedDetail.ChargedDate
			+ ">=?"
			+ and
			+ DBSchema.ChargedDetail.TableName
			+ "."
			+ DBSchema.ChargedDetail.ChargedDate
			+ "<=?"
			+ groupBy
			+ DBSchema.ChargedDetail.TableName
			+ "."
			+ DBSchema.ChargedDetail.ContractID
			+ union
			+ select
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.ContractID
			+ ", 0, sum("
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.InvoiceAmt
			+ "), 0 "
			+ from
			+ DBSchema.InvoiceDetail.TableName
			+ where
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.InvoiceDate
			+ ">=?"
			+ and
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.InvoiceDate
			+ "<=?"
			+ groupBy
			+ DBSchema.InvoiceDetail.TableName
			+ "."
			+ DBSchema.InvoiceDetail.ContractID
			+ union
			+ select
			+ DBSchema.PaymentCondition.TableName
			+ "."
			+ DBSchema.PaymentCondition.ContractID
			+ ", 0, 0, sum("
			+ DBSchema.PaymentCondition.TableName
			+ "."
			+ DBSchema.PaymentCondition.PaymentAmt
			+ ") "
			+ from
			+ DBSchema.PaymentCondition.TableName
			+ where
			+ DBSchema.PaymentCondition.TableName
			+ "."
			+ DBSchema.PaymentCondition.PaymentDate
			+ ">=?"
			+ and
			+ DBSchema.PaymentCondition.TableName
			+ "."
			+ DBSchema.PaymentCondition.PaymentDate
			+ "<=?"
			+ groupBy
			+ DBSchema.PaymentCondition.TableName
			+ "."
			+ DBSchema.PaymentCondition.ContractID

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
			+ "#contract."
			+ DBSchema.Contract.ContractID
			+ "= #contract_pay_invoice_sum."
			+ DBSchema.ChargedDetail.ContractID

	;

	public static String getSumInfoByIntegrationPerformed = getRunContractInTimeScope
			.replace("#result", "#contract_timescope")
			+ select
			+ DBSchema.CurrencyType.CurrenyGraph
			// + ", sum(" + DBSchema.Contract.ContractAmt + ") as " +
			// DBSchema.Contract.ContractAmt
			// + ", count(*) as " + DBSchema.Contract.ContractAmtCount
			// + ", sum(" + DBSchema.ChargedDetail.TotalAmtByID + ") as " +
			// DBSchema.ChargedDetail.TotalAmtByID
			// + ", sum(" + DBSchema.ChargedDetail.UnChargedAmtTotal + ") as " +
			// DBSchema.ChargedDetail.UnChargedAmtTotal
			// + ", sum(" + DBSchema.InvoiceDetail.TotalAmtByID + ") as " +
			// DBSchema.InvoiceDetail.TotalAmtByID
			// + ", sum(" + DBSchema.InvoiceDetail.UnAccountAmtTotalSum +
			// ") as " + DBSchema.InvoiceDetail.UnAccountAmtTotalSum

			+ ", sum(contract_payed_amt) as contract_payed_amt, count(*) as contract_payed_count "
			+ ", 0 as contract_invoice_amt, 0 as contract_invoice_count "
			+ ", 0 as contract_estimate_amt, 0 as contract_estimate_count "

			+ into
			+ "#contract_count"
			+ from
			+ "#contract_timescope"
			+ where
			+ " contract_payed_amt > 0 "
			+ groupBy
			+ DBSchema.CurrencyType.CurrenyGraph

			+ union
			+ select
			+ DBSchema.CurrencyType.CurrenyGraph
			+ ", 0 , 0 , sum(contract_invoice_amt), count(*), 0, 0 "

			+ from
			+ "#contract_timescope"
			+ where
			+ " contract_invoice_amt > 0 "
			+ groupBy
			+ DBSchema.CurrencyType.CurrenyGraph

			+ union
			+ select
			+ DBSchema.CurrencyType.CurrenyGraph
			+ ", 0 , 0 , 0, 0, sum(contract_estimate_amt), count(*) "

			+ from
			+ "#contract_timescope"
			+ where
			+ " contract_estimate_amt > 0 "
			+ groupBy
			+ DBSchema.CurrencyType.CurrenyGraph

			+ select
			+ DBSchema.CurrencyType.CurrenyGraph
			+ ", sum(contract_payed_amt) as contract_payed_amt, sum(contract_payed_count) as contract_payed_count"
			+ ", sum(contract_invoice_amt) as contract_invoice_amt, sum(contract_invoice_count) as contract_invoice_count"
			+ ", sum(contract_estimate_amt) as contract_estimate_amt, sum(contract_estimate_count) as contract_estimate_count"

			+ into
			+ "#result"
			+ from
			+ "#contract_count"
			+ groupBy
			+ DBSchema.CurrencyType.CurrenyGraph;

	public static String getPerformedSubIntegrationForPayed = getRunContractInTimeScope
			.replace("#result", "#contract_timescope")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from
			+ "#contract_timescope"
			+ where
			+ DBSchema.CurrencyType.CurrenyGraph
			+ "=?"
			+ and
			+ "contract_payed_amt > 0";

	public static String getPerformedSubIntegrationForInvoiced = getRunContractInTimeScope
			.replace("#result", "#contract_timescope")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from
			+ "#contract_timescope"
			+ where
			+ DBSchema.CurrencyType.CurrenyGraph
			+ "=?"
			+ and
			+ "contract_invoice_amt > 0";

	public static String getPerformedSubIntegrationForEstimate = getRunContractInTimeScope
			.replace("#result", "#contract_timescope")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from
			+ "#contract_timescope"
			+ where
			+ DBSchema.CurrencyType.CurrenyGraph
			+ "=?"
			+ and
			+ "contract_estimate_amt > 0";

	public static String getContractListByIntegrationWarranty = getContractListByType
			.replace("#result", "#contract")
			+ select
			+ "*"
			+ into
			+ "#result"
			+ from
			+ "#contract"
			+ where
			+ DBSchema.Contract.WarrantyEndDate
			+ ">=? " + and + DBSchema.Contract.WarrantyEndDate + "<=? ";

	public static String getContractListByTypeForPaymentCondition(
			String paramString) {
		StringBuilder builder = new StringBuilder(
				getContractListByType.replace("#result", "#contract"));
		builder.append(select).append(" #contract.* ").append(into)
				.append("#result").append(from)
				.append("#contract  LEFT OUTER JOIN ")
				.append(DBSchema.PaymentCondition.TableName).append(" as pay ")
				.append(" on #contract.").append(DBSchema.Contract.ContractID)
				.append(" = ").append("pay.")
				.append(DBSchema.PaymentCondition.ContractID).append(where)
				.append("pay.").append(DBSchema.PaymentCondition.PaymentAmt)
				.append(" = ?");
		if (StringUtils.isEmpty(paramString)) {
		} else {
			builder.append(" and ").append(paramString);
		}
		return builder.toString();
	}

	public static String saveContractRelated = "declare @rid integer"
			+ "\n select @rid=max(对应ID)+1 from con_收入支出合同对应表"
			+ "\n insert into " + DBSchema.ContractRelated.TableName + "("
			+ DBSchema.ContractRelated.RelatedID + ","
			+ DBSchema.ContractRelated.IncomeContractID + ","
			+ DBSchema.ContractRelated.OutContractID + ","
			+ DBSchema.ContractRelated.Amt + ","
			+ DBSchema.ContractRelated.Remark + ","
			+ DBSchema.ContractRelated.OutSubContractID
			+ ")\n values(@rid,?,?,?,?,?)";

	public static String getContractInfoByProjectNo = select + "*" + from
			+ DBSchema.Contract.TableName + " as c left join "
			+ DBSchema.Project.TableName + " as p on p."
			+ DBSchema.Project.ProjectID + " = c."
			+ DBSchema.Contract.ProjectID + where
			+ DBSchema.Project.ProjectCode + like + "?";

	public static String getEmpNumOfCompany = select + "count(*)" + from
			+ DBSchema.Company.TableName + " a left outer join "
			+ DBSchema.DeptBelongToCompany.TableName + " b on a."
			+ DBSchema.Company.CompanyID + "=b."
			+ DBSchema.DeptBelongToCompany.companyID + " left outer join "
			+ DBSchema.Department.TableName + " c on b."
			+ DBSchema.DeptBelongToCompany.DepartmentID + "= c."
			+ DBSchema.Department.DepartmentID + " left outer join "
			+ DBSchema.EmployeeBelongToDept.TableName + " d on c."
			+ DBSchema.Department.DepartmentID + "= d."
			+ DBSchema.EmployeeBelongToDept.DepartmentID + " left outer join "
			+ DBSchema.Employee.TableName + " e on d."
			+ DBSchema.EmployeeBelongToDept.EmployeeID + "= e."
			+ DBSchema.Employee.ID + where + " a." + DBSchema.Company.CompanyID
			+ "=? and e." + DBSchema.Employee.ID + ">0" + and + "(e."
			+ DBSchema.Employee.DisabledFlag + " is null" + or + "e."
			+ DBSchema.Employee.DisabledFlag + "=0)";

	public static String getWaringDays = "SET NOCOUNT ON SELECT xmis.con_收款条件表.合同ID,xmis.con_收款条件表.收款日期"
			+ " INTO #warning_date"
			+ " FROM xmis.con_收款条件表 where 合同ID = ?"
			+ " SELECT xmis.con_收款条件表.合同ID,#warning_date.收款日期 AS warning_date,0 AS warninged_flag, null AS warning_days"
			+ " INTO #warning_detail"
			+ " FROM xmis.con_收款条件表, #warning_date"
			+ " WHERE xmis.con_收款条件表.合同ID = #warning_date.合同ID"
			+ " AND (xmis.con_收款条件表.收款日期 <= #warning_date.收款日期 OR #warning_date.收款日期 is null )"
			+ " GROUP BY xmis.con_收款条件表.合同ID,#warning_date.收款日期"
			+ " UPDATE #warning_detail SET warning_days =  "
			+ " CASE	"
			+ " WHEN DATEDIFF(d,GETDATE(),warning_date)<-30 THEN -30	"
			+ " WHEN DATEDIFF(d,GETDATE(),warning_date)>=-30 AND DATEDIFF(d,GETDATE(),warning_date)<-15 THEN -15	"
			+ " WHEN DATEDIFF(d,GETDATE(),warning_date)>=-15 AND DATEDIFF(d,GETDATE(),warning_date)<-7 THEN -7	"
			+ " WHEN DATEDIFF(d,GETDATE(),warning_date)>=-7 AND DATEDIFF(d,GETDATE(),warning_date)<0 THEN -1	"
			+ " WHEN DATEDIFF(d,GETDATE(),warning_date)=0 THEN 1	"
			+ " WHEN DATEDIFF(d,GETDATE(),warning_date)>0 AND DATEDIFF(d,GETDATE(),warning_date)<=7 THEN 7	"
			+ " WHEN DATEDIFF(d,GETDATE(),warning_date)>7 AND DATEDIFF(d,GETDATE(),warning_date)<=15 THEN 15	"
			+ " WHEN DATEDIFF(d,GETDATE(),warning_date)>15 AND DATEDIFF(d,GETDATE(),warning_date)<=30 THEN 30	"
			+ " WHEN warning_date is null then NULL	"
			+ " ELSE DATEDIFF(d,GETDATE(),warning_date)"
			+ " END	WHERE warninged_flag = 0 "
			+ " SELECT MAX(warning_days)"
			+ " FROM #warning_detail"
			+ " WHERE warning_days <= 30 AND warninged_flag = 0";

	public static String insertDelayReasonInfo = insert + into
			+ DBSchema.DelayReason.TableName + values
			+ " (?,?,?,?,GETDATE(),?)";

	public static String updateDelayReasonInfo = update
			+ DBSchema.DelayReason.TableName + set
			+ DBSchema.DelayReason.Reason + "=?,"
			+ DBSchema.DelayReason.OperatorID + "=?" + where
			+ DBSchema.DelayReason.ID + "=?";

	public static String updateDelayReasonInfoWithoutOper = update
			+ DBSchema.DelayReason.TableName + set
			+ DBSchema.DelayReason.Reason + "=?" + where
			+ DBSchema.DelayReason.ID + "=?";

	public static String delDelayReasonInfo = delete + from
			+ DBSchema.DelayReason.TableName + where + DBSchema.DelayReason.ID
			+ "=?";

	public static String invoiceSplitDetailBySimpleInvoice = select
			+ " invoice.*,RTRIM(hr." + DBSchema.Employee.FirstName
			+ ")+RTRIM(hr." + DBSchema.Employee.LastName + ") as 登记人 " + from
			+ DBSchema.InvoiceSplit.TableName + " as invoice left outer join "
			+ DBSchema.Employee.TableName + " as hr on invoice."
			+ DBSchema.InvoiceSplit.Registrant + " = hr."
			+ DBSchema.Employee.ID + " where "
			+ DBSchema.InvoiceSplit.ContractID + " = ? " + and
			+ DBSchema.InvoiceSplit.InvoiceNumber + " = ? order by "
			+ DBSchema.InvoiceSplit.ID;

	public static String invoiceSplitInfoBySimpleInvoice = select + "invoice."
			+ DBSchema.InvoiceSplit.AnticipatedTime
			+ " as anticipatedTime,invoice." + DBSchema.InvoiceSplit.SplitSum
			+ " as splitSum,invoice." + DBSchema.InvoiceSplit.PayFlag
			+ " as payFlag,invoice." + DBSchema.InvoiceSplit.Remark
			+ " as remark" + from + DBSchema.InvoiceSplit.TableName
			+ " as invoice" + where + DBSchema.InvoiceSplit.ContractID + " =? "
			+ and + DBSchema.InvoiceSplit.InvoiceNumber + " =? " + " order by "
			+ DBSchema.InvoiceSplit.ID;

	public static String deleteInvoiceSplitInfo = delete + from
			+ DBSchema.InvoiceSplit.TableName + where
			+ DBSchema.InvoiceSplit.ID + " =?";

	public static String saveInvoiceSplitInfo = insert + into
			+ DBSchema.InvoiceSplit.TableName + values
			+ "(?,?,?,?,?,?,?,GETDATE(),?)";

	public static String updateInvoiceSplitInfo = update
			+ DBSchema.InvoiceSplit.TableName + set
			+ DBSchema.InvoiceSplit.SplitSum + " =?,"
			+ DBSchema.InvoiceSplit.PayFlag + " =?,"
			+ DBSchema.InvoiceSplit.AnticipatedTime + " =?,"
			+ DBSchema.InvoiceSplit.Remark + " =? " + where
			+ DBSchema.InvoiceSplit.ID + " =?";

	public static String updateInvoiceSplitIncomeStatus = update
			+ DBSchema.InvoiceSplit.TableName + set
			+ DBSchema.InvoiceSplit.PayFlag + " =? " + where
			+ DBSchema.InvoiceSplit.ID + " =?";

	public static String getIncomeAssistList = select
			+ "payed."
			+ DBSchema.Contract.ContractID
			+ ",sum("
			+ DBSchema.ChargedDetail.ChargedAmt
			+ ") as 已收金额 "
			+ into
			+ "#payedAmt"
			+ from
			+ DBSchema.ChargedDetail.TableName
			+ " as payed group by "
			+ DBSchema.Contract.ContractID
			+ select
			+ "#payedAmt."
			+ DBSchema.Contract.ContractID
			+ into
			+ "#deleteList"
			+ from
			+ DBSchema.Contract.TableName
			+ " as contract,#payedAmt "
			+ where
			+ "contract."
			+ DBSchema.Contract.ContractTypeID
			+ " = 1 "
			+ and
			+ "contract."
			+ DBSchema.Contract.ContractID
			+ " = #payedAmt."
			+ DBSchema.Contract.ContractID
			+ and
			+ "contract."
			+ DBSchema.Contract.ContractAmt
			+ " <= #payedAmt.已收金额"
			+ select
			+ "*"
			+ into
			+ "#totalContract"
			+ from
			+ DBSchema.Contract.TableName
			+ " as contract "
			+ where
			+ "contract."
			+ DBSchema.Contract.ContractTypeID
			+ " = 1 "
			+ and
			+ DBSchema.Contract.ContractID
			+ " not in ("
			+ select
			+ "*"
			+ from
			+ "#deleteList)"
			+ select
			+ "contract."
			+ DBSchema.Contract.ContractID
			+ ",contract."
			+ DBSchema.Contract.Abbreviation
			+ " as "
			+ DBSchema.Contract.ContractName
			+ ",contract."
			+ DBSchema.Contract.DeptIDOfVendor
			+ ",invoice."
			+ DBSchema.InvoiceDetail.InvoiceAmt
			+ " as "
			+ DBSchema.InvoiceDetail.InvoiceAmt
			+ ",company."
			+ DBSchema.Company.Abbreviation
			+ " as "
			+ DBSchema.Company.CompanyName
			+ ",company."
			+ DBSchema.Company.AccountNo
			+ " as 付款人帐号,project."
			+ DBSchema.Project.ProjectCode
			+ " as "
			+ DBSchema.Project.ProjectCode
			+ ",invoice."
			+ DBSchema.InvoiceDetail.InvoiceDate
			+ ",invoice."
			+ DBSchema.InvoiceDetail.InvoiceNo
			+ "\n,'<font color=red>' + (case when contract."
			+ DBSchema.Contract.Abbreviation
			+ " is not null then RTRIM(contract."
			+ DBSchema.Contract.Abbreviation
			+ ") else '' end) + '&nbsp;</font><font color=green>[' + (case when project."
			+ DBSchema.Project.ProjectCode + " is not null then RTRIM(project."
			+ DBSchema.Project.ProjectCode
			+ ") else '' end)  + ']</font><font color=blue>&nbsp;&nbsp;"
			+ "' + (case when company." + DBSchema.Company.Abbreviation
			+ " is not null then RTRIM(company."
			+ DBSchema.Company.Abbreviation + ") else '' end  + "
			+ " + (case when company." + DBSchema.Company.AccountNo
			+ " is not null then +'&nbsp;('+ RTRIM(company."
			+ DBSchema.Company.AccountNo + ") + ')' else '' end) + "
			+ "'</font>&nbsp;&nbsp;' ) as contractMsg"
			+ ", project.项目经理ID manager_id,project.执行部门   execu_id "
			+ " into #temp" + from + "#totalContract as contract,"
			+ DBSchema.Company.TableName + " as company,"
			+ DBSchema.InvoiceDetail.TableName + " as invoice,"
			+ DBSchema.Project.TableName + " as project where contract."
			+ DBSchema.Contract.ContractID + " = invoice."
			+ DBSchema.Contract.ContractID + and + "contract."
			+ DBSchema.Contract.BuyerID + " = company."
			+ DBSchema.Company.CompanyID + and + "contract."
			+ DBSchema.Contract.ProjectID + " = project."
			+ DBSchema.Project.ProjectID + select + "split."
			+ DBSchema.InvoiceSplit.ID + " as " + DBSchema.InvoiceSplit.ID
			+ ",split." + DBSchema.InvoiceSplit.ContractID + " as "
			+ DBSchema.Contract.ContractID + ",#temp."
			+ DBSchema.Project.ProjectCode + ",#temp."
			+ DBSchema.Contract.ContractName + ",#temp."
			+ DBSchema.InvoiceDetail.InvoiceAmt + ",split."
			+ DBSchema.InvoiceSplit.SplitSum + " as "
			+ DBSchema.InvoiceSplit.AnticipatedAmt + ",split."
			+ DBSchema.InvoiceSplit.AnticipatedTime + ",#temp.付款人帐号,#temp."
			+ DBSchema.Company.CompanyName + ",split."
			+ DBSchema.InvoiceSplit.PayFlag + ",split."
			+ DBSchema.InvoiceSplit.Remark + ",#temp."
			+ DBSchema.Contract.DeptIDOfVendor + ",#temp.contractMsg,#temp."
			+ DBSchema.InvoiceDetail.InvoiceDate + " as 开票日期,#temp."
			+ DBSchema.InvoiceDetail.InvoiceNo + " as "
			+ DBSchema.InvoiceDetail.InvoiceNo
			+ " ,#temp.manager_id ,#temp.execu_id " + into + "#result" + from
			+ DBSchema.InvoiceSplit.TableName + " as split,#temp where split."
			+ DBSchema.InvoiceSplit.ContractID + " = #temp."
			+ DBSchema.Contract.ContractID + and + "split."
			+ DBSchema.InvoiceSplit.InvoiceNumber + " = #temp."
			+ DBSchema.InvoiceDetail.InvoiceNo + and + "split."
			+ DBSchema.InvoiceSplit.PayFlag + " = 0" + insert + into
			+ "#result" + select + " -1 as " + DBSchema.InvoiceSplit.ID
			+ ",#temp." + DBSchema.Contract.ContractID + " as "
			+ DBSchema.Contract.ContractID + ",#temp."
			+ DBSchema.Project.ProjectCode + " as 项目编号,#temp."
			+ DBSchema.Contract.ContractName + ",#temp."
			+ DBSchema.InvoiceDetail.InvoiceAmt + ",0 as "
			+ DBSchema.InvoiceSplit.AnticipatedAmt + ",null as "
			+ DBSchema.InvoiceSplit.AnticipatedTime + ",#temp.付款人帐号,#temp."
			+ DBSchema.Company.CompanyName + ",0 as "
			+ DBSchema.InvoiceSplit.PayFlag + ",'' as "
			+ DBSchema.InvoiceSplit.Remark + ",#temp."
			+ DBSchema.Contract.DeptIDOfVendor
			+ ",#temp.contractMsg as contractMsg,#temp."
			+ DBSchema.InvoiceDetail.InvoiceDate + " as 开票日期,#temp."
			+ DBSchema.InvoiceDetail.InvoiceNo + " as "
			+ DBSchema.InvoiceSplit.InvoiceNumber
			+ " ,#temp.manager_id ,#temp.execu_id " + from + "#temp" + where
			+ DBSchema.Contract.ContractID + " not in (" + select + "distinct "
			+ DBSchema.Contract.ContractID + from
			+ DBSchema.InvoiceSplit.TableName + ")";

	public static String getProjectStatusByProjectId = "select p."
			+ DBSchema.ProjectStatus.ProjectStatusID + ",p."
			+ DBSchema.ProjectStatus.ProjectStatusName + ",p."
			+ DBSchema.ProjectStatus.ProjectID + ",p."
			+ DBSchema.ProjectStatus.ProjectStatusDescription + ",p."
			+ DBSchema.ProjectStatus.SelfDefinitionFlag + " from ( "
			+ "select * from prj_项目状态 p1 where p1.是否自定义=0 and p1.项目状态ID>0 "
			+ " union " + "select p2.* from  " + DBSchema.Contract.TableName
			+ " c " + " left join " + DBSchema.PaymentCondition.TableName
			+ " pc on pc." + DBSchema.PaymentCondition.ContractID + "=c."
			+ DBSchema.Contract.ContractID + " left join "
			+ DBSchema.ProjectStatus.TableName + " p2 on p2."
			+ DBSchema.ProjectStatus.ProjectStatusID + "=pc."
			+ DBSchema.PaymentCondition.ProjectStatusID + " where c."
			+ DBSchema.Contract.ProjectID + "=? and p2.是否自定义=1) as p";

	public static String getPaymentListByIDAndFas = "SET NOCOUNT ON "
			+ " DECLARE @contractID int set @contractID = ?"
			+ " DECLARE @projectCode nvarchar(100)"
			+ " DECLARE @totalCount int"
			+ " DECLARE @maxYear datetime"
			+ " DECLARE @totalImcome DECIMAL(18,2)"
			+ " SELECT @projectCode = 项目ID"
			+ " FROM [xmis].[con_合同] where 合同ID = @contractID"
			+ getPaymentListByID.replace("?", "@contractID")
			+ " select @totalCount = COUNT(*) from #result"
			+ " if(@totalCount=0) begin "
			+ getGLIncomeByProjectCode.replace("?", "@projectCode").replace(
					"#result", "#resultTemp")
			+ " select @totalImcome = SUM(MB) from #resultTemp"
			+ " if(@totalImcome<>0) begin"
			+ " select @maxYear = case when max(dbill_date) IS null then CONVERT(datetime,'2007-12-31',101) else max(dbill_date) end from #resultTemp"
			+ " DECLARE @currencyID int "
			+ " DECLARE @currencySymbol varchar(10)"
			+ " DECLARE @paritiesForRMB DECIMAL(18,2)"
			+ " DECLARE @person varchar(50)"
			+ " SELECT @currencyID=xmis.sys_货币.货币ID,@currencySymbol=xmis.sys_货币.货币符号,@paritiesForRMB=xmis.sys_货币.对人民币汇率"
			+ " FROM xmis.con_合同,xmis.sys_货币 where 合同ID=@contractID and xmis.con_合同.货币ID =xmis.sys_货币.货币ID"
			+ " SELECT @person=rtrim(姓氏)+RTRIM(名字)"
			+ " FROM xmis.hr_人员 where 人员ID =-1"
			+ " insert into #result "
			+ " values(@contractID,-1,@totalImcome,@maxYear,-1,null,null,null,'此数据为:截止至 '+CONVERT(varchar, @maxYear, 120 )+"
			+ "' 财务来款金额为:'+CONVERT(varchar, @totalImcome)+' 的初始化数据',null,null,Convert(varchar(10),@maxYear,101),null,@currencyID,@currencySymbol,"
			+ "@paritiesForRMB,@person,null)" + " end" + " end";

	public static String deleteSubModuleFuncMappping = delete + from
			+ DBSchema.BizModuleFuncMapping.TableName + where
			+ DBSchema.BizModuleFuncMapping.BizFunctionID + "=?";

	public static String getFuncCountsByBizFunctionID = select + " COUNT("
			+ DBSchema.BizFunction.FunctionID + ") as counts,"
			+ DBSchema.BizFunction.FunctionID + from
			+ DBSchema.BizFunction.TableName + where
			+ DBSchema.BizFunction.FunctionID + " = (" + select
			+ DBSchema.BizFunction.FunctionID + from
			+ DBSchema.BizFunction.TableName + where
			+ DBSchema.BizFunction.BizFunctionID + " = ?) group by "
			+ DBSchema.BizFunction.FunctionID;

	public static String deleteModuleSubModuleMappping = delete + from
			+ DBSchema.BizModuleMapping.TableName + where
			+ DBSchema.BizModuleMapping.BizSubModuleID + " =? ";

	public static String updateModuleSubModuleMappping = update
			+ DBSchema.BizModuleMapping.TableName + set
			+ DBSchema.BizModuleMapping.BizModuleID + " =? " + where
			+ DBSchema.BizModuleMapping.BizModuleID + " = ? " + and
			+ DBSchema.BizModuleMapping.BizSubModuleID + " = ? ";

	public static String saveModuleSubModuleMappping = insert + into
			+ DBSchema.BizModuleMapping.TableName + "("
			+ DBSchema.BizModuleMapping.BizModuleID + ","
			+ DBSchema.BizModuleMapping.BizSubModuleID + ") values(?,?)";

	public static String getFuncIdByBizFunctionID = select
			+ DBSchema.BizFunction.FunctionID + from
			+ DBSchema.BizFunction.TableName + where
			+ DBSchema.BizFunction.BizFunctionID + " = ?";

	public static String updateSubModuleName = update
			+ DBSchema.BizSubModule.TableName + set
			+ DBSchema.BizSubModule.BizSubModuleName + " = ? " + where
			+ DBSchema.BizSubModule.BizSubModuleID + " = ?";

	public static String updateModuleName = update
			+ DBSchema.BizModule.TableName + set
			+ DBSchema.BizModule.BizModuleName + "  = ? " + where
			+ DBSchema.BizModule.BizModuleID + " = ?";

	public static String updateFuncName = update
			+ DBSchema.BizFunction.TableName + set
			+ DBSchema.BizFunction.BizFunctionName + " = ? " + where
			+ DBSchema.BizFunction.BizFunctionID + " = ?";

	public static String updateSubModuleFuncMappping = update
			+ DBSchema.BizModuleFuncMapping.TableName + set
			+ DBSchema.BizModuleFuncMapping.BizSubModuleID + " = ? " + where
			+ DBSchema.BizModuleFuncMapping.BizSubModuleID + " = ? " + and
			+ DBSchema.BizModuleFuncMapping.BizFunctionID + " = ? ";

	public static String deleteSubModuleFunctionMappping = delete + from
			+ DBSchema.BizModuleFuncMapping.TableName + where
			+ DBSchema.BizModuleFuncMapping.BizSubModuleID + " = ? " + and
			+ DBSchema.BizModuleFuncMapping.BizFunctionID + " = ? ";

	public static String saveSubModuleFuncMappping = insert + into
			+ DBSchema.BizModuleFuncMapping.TableName + " values(?,?)";

	public static String deleteSubModule = delete + from
			+ DBSchema.BizSubModule.TableName + where
			+ DBSchema.BizSubModule.BizSubModuleID + " = ? ";

	public static String deleteModule = delete + from
			+ DBSchema.BizModule.TableName + where
			+ DBSchema.BizModule.BizModuleID + " = ? ";

	public static String deleteFunction = delete + from
			+ DBSchema.BizFunction.TableName + where
			+ DBSchema.BizFunction.BizFunctionID + " = ? ";

	public static String saveSubModule = insert + into
			+ DBSchema.BizSubModule.TableName + "("
			+ DBSchema.BizSubModule.BizSubModuleID + ","
			+ DBSchema.BizSubModule.BizSubModuleName + ") values(?,?)";

	public static String saveModule = insert + into
			+ DBSchema.BizModule.TableName + "("
			+ DBSchema.BizModule.BizModuleID + ","
			+ DBSchema.BizModule.BizModuleName + ")  values(?,?)";

	public static String saveFunction = insert + into
			+ DBSchema.BizFunction.TableName + "("
			+ DBSchema.BizFunction.FunctionID + ","
			+ DBSchema.BizFunction.BizFunctionName + ","
			+ DBSchema.BizFunction.BizFunctionID + ") values(?,?,?)";

	public static String getAllStatusList = select + " * " + from
			+ DBSchema.Status.TableName + " order by "
			+ DBSchema.Status.StatusID + " asc";

	public static String getBizModuleById = select + " model.*,status."
			+ DBSchema.Status.StatusName + from + DBSchema.BizModule.TableName
			+ " as model" + " left outer join " + DBSchema.Status.TableName
			+ " as status on model." + DBSchema.BizModule.BizModuleCommitStatus
			+ " = status." + DBSchema.Status.StatusID + where + " model."
			+ DBSchema.BizModule.BizModuleID + " = ?";

	public static String getBizSubModuleByModuleId = select + " mapping."
			+ DBSchema.BizModuleMapping.BizSubModuleID + ",mapping."
			+ DBSchema.BizModuleMapping.BizModuleID + ",submodel."
			+ DBSchema.BizSubModule.BizSubModuleName + ",mapping."
			+ DBSchema.BizModuleMapping.BizSubModulesSquenceNo + from
			+ DBSchema.BizModuleMapping.TableName + " as mapping,"
			+ DBSchema.BizSubModule.TableName + " as submodel" + where
			+ " mapping." + DBSchema.BizModuleMapping.BizModuleID + " = ? "
			+ and + " mapping." + DBSchema.BizModuleMapping.BizSubModuleID
			+ " = submodel." + DBSchema.BizSubModule.BizSubModuleID
			+ " order by mapping."
			+ DBSchema.BizModuleMapping.BizSubModulesSquenceNo;

	public static String getPaymentConditionTotalAmtById = select
			+ " sum(收款金额) as amt,sum(收款百分比) as percentage " + from
			+ DBSchema.PaymentCondition.TableName + where
			+ DBSchema.PaymentCondition.ContractID + " = ? group by "
			+ DBSchema.PaymentCondition.ContractID;

	public static String getEmpInfoById = select + DBSchema.Employee.ID + ","
			+ DBSchema.Employee.LastName + "," + DBSchema.Employee.FirstName
			+ "," + DBSchema.Employee.HomeAddress + ","
			+ DBSchema.Employee.Position + "," + DBSchema.Employee.MobileNo
			+ "," + DBSchema.Employee.OfficeTelNo + ","
			+ DBSchema.Employee.Email + "," + DBSchema.Employee.ZipCode + from
			+ DBSchema.Employee.TableName + where + DBSchema.Employee.ID
			+ " = ?";

	public static String updateEmpInfo = update + DBSchema.Employee.TableName
			+ set + DBSchema.Employee.LastName + "=?,"
			+ DBSchema.Employee.FirstName + " =?,"
			+ DBSchema.Employee.HomeAddress + "=?,"
			+ DBSchema.Employee.Position + "=?," + DBSchema.Employee.MobileNo
			+ "=?," + DBSchema.Employee.OfficeTelNo + "=?,"
			+ DBSchema.Employee.Email + "=?," + DBSchema.Employee.ZipCode
			+ "=?," + DBSchema.Employee.UpdateID + "=?,"
			+ DBSchema.Employee.UpdateDate + "=GETDATE() " + where
			+ DBSchema.Employee.ID + "=?";

	public static String updateRelatedContract = " SET NOCOUNT ON " + update
			+ DBSchema.ContractRelated.TableName + set
			+ DBSchema.ContractRelated.IncomeContractID + "=?,"
			+ DBSchema.ContractRelated.Amt + "=?,"
			+ DBSchema.ContractRelated.Remark + "=? " + where
			+ DBSchema.ContractRelated.RelatedID + "=? "
			+ " DECLARE @outCID int " + select + " @outCID = "
			+ DBSchema.ContractRelated.OutSubContractID + from
			+ DBSchema.ContractRelated.TableName + where
			+ DBSchema.ContractRelated.RelatedID + "=? and 支出合同ID=?" + update
			+ DBSchema.Contract.TableName + set + DBSchema.Contract.ContractNo
			+ "=?," + DBSchema.Contract.ContractAmt + "=? " + where
			+ DBSchema.Contract.ContractID + "=@outCID";

	public static String getContractRelationInfo = " SET NOCOUNT ON "
			+ " DECLARE @pcontractID int  set @pcontractID = ? "
			+ " DECLARE @rID int  set @rID = ? " + " DECLARE @count int "
			+ select
			+ "contract."
			+ DBSchema.Contract.ContractID
			+ " as pconid,contract."
			+ DBSchema.Contract.ContractAmt
			+ " as totalAmt "
			+ into
			+ " #totalAmt"
			+ from
			+ DBSchema.Contract.TableName
			+ " as contract "
			+ where
			+ DBSchema.Contract.ContractID
			+ " = @pcontractID "
			+ select
			+ " * "
			+ into
			+ " #temp "
			+ from
			+ DBSchema.ContractRelated.TableName
			+ where
			+ DBSchema.ContractRelated.OutContractID
			+ " = @pcontractID "
			+ and
			+ DBSchema.ContractRelated.RelatedID
			+ " <> @rID "
			+ select
			+ " @count = COUNT(*) "
			+ from
			+ " #temp "
			+ select
			+ DBSchema.ContractRelated.OutContractID
			+ " as pconid,sum("
			+ DBSchema.ContractRelated.Amt
			+ ") as amt "
			+ into
			+ " #currentAmt "
			+ from
			+ " #temp group by "
			+ DBSchema.ContractRelated.OutContractID
			+ " having "
			+ DBSchema.ContractRelated.OutContractID
			+ " = @pcontractID "
			+ " if(@count = 0) begin "
			+ insert
			+ into
			+ " #currentAmt values(@pcontractID,0) end "
			+ select
			+ " #totalAmt.pconid as pcontractID,#totalAmt.totalAmt - #currentAmt.amt as amt "
			+ into
			+ " #amt "
			+ from
			+ " #totalAmt,#currentAmt "
			+ where
			+ " #totalAmt.pconid = #currentAmt.pconid "
			+ select
			+ " relate."
			+ DBSchema.ContractRelated.IncomeContractID
			+ " as inContractID,relate."
			+ DBSchema.ContractRelated.OutContractID
			+ " as pContractID,relate."
			+ DBSchema.ContractRelated.RelatedID
			+ " as relatedID,relate."
			+ DBSchema.ContractRelated.Amt
			+ " as relateAmt,relate."
			+ DBSchema.ContractRelated.Remark
			+ " as remark,project."
			+ DBSchema.Project.ProjectCode
			+ " as inContractNo,outcontract."
			+ DBSchema.Contract.ContractNo
			+ " as contractNo,#amt.amt as samt "
			+ from
			+ DBSchema.ContractRelated.TableName
			+ " as relate,"
			+ DBSchema.Contract.TableName
			+ " as incontract,"
			+ DBSchema.Contract.TableName
			+ " as outcontract,#amt,"
			+ DBSchema.Project.TableName
			+ " as project "
			+ where
			+ DBSchema.ContractRelated.RelatedID
			+ " = @rID "
			+ and
			+ " relate."
			+ DBSchema.ContractRelated.IncomeContractID
			+ " = incontract."
			+ DBSchema.Contract.ContractID
			+ and
			+ " relate."
			+ DBSchema.ContractRelated.OutSubContractID
			+ " = outcontract."
			+ DBSchema.Contract.ContractID
			+ and
			+ " relate."
			+ DBSchema.ContractRelated.OutContractID
			+ " = #amt.pcontractID and  incontract."
			+ DBSchema.Project.ProjectID
			+ " = project."
			+ DBSchema.Project.ProjectID;

	public static String getContractSummaryByConId = select
			+ DBSchema.Contract.ContractSummary + " as summary" + from
			+ DBSchema.Contract.TableName + where
			+ DBSchema.Contract.ContractID + " =?";

	public static String getStencilByDeptId = "set nocount on "
			+ " declare @counts int" + " declare @deptNo int "
			+ " set @counts = 0" + " set @deptNo = ?" + select + " @counts = "
			+ DBSchema.DeptBelongToDept.ParentsID + from
			+ DBSchema.DeptBelongToDept.TableName + where
			+ DBSchema.DeptBelongToDept.DeptID + " = @deptNo"
			+ " if(@counts <> 0)begin" + " set @deptNo = @counts" + " end"
			+ select + " * " + from + DBSchema.Stencil.TableName + where
			+ DBSchema.Stencil.DeptID + " = @deptNo";

	public static String updateProjectStatusName = update
			+ DBSchema.ProjectStatus.TableName + set
			+ DBSchema.ProjectStatus.ProjectStatusName + " =? " + where
			+ DBSchema.ProjectStatus.ProjectStatusID + " =?";

	// 电话本sql
	// 通过部门ID查找
	public static String getEmpInfoByDeptID = "\n declare @maxCP int, @maxBP int \n"
			+ " select @maxCP = MAX(排序)+1 from xmis.sys_职务 \n"
			+ " select @maxBP = MAX(排序)+1 from xmis.hr_人员部门关系表 \n"
			+ " SELECT  a.人员ID empId,  b.部门ID tdeptId,d.部门名称 tdeptName, (RTRIM(姓氏)+RTRIM(名字))  userName ,   \n"
			+ " c.职务名称 position, 移动电话 mobileNo, 单位电话 switchboardNo, 单位分机 extNo, 传真号码 faxNo, \n"
			+ " 电子邮件 emailAddress, 详细地址 officeAddress,  \n"
			+ " case when  g.部门名称 is null then  d.部门名称 else case when  c.排序 < 102 then  g.部门名称 else  g.部门名称 +  d.部门名称 end end  AS  departmentName  , \n"
			+ " case when e.是否直属 =1  then b.部门ID else  f.父部门ID end deptId , \n"
			+ " CASE WHEN e.是否直属 = 1 THEN d.部门名称 ELSE g.部门名称  END deptName , \n"
			+ " case when c.排序 is NULL then @maxCP else c.排序 end cp,  case when b.排序 is NULL then @maxBP else b.排序 end  bp \n"
			+ " ,e.公司ID compId ,e.公司名称 compName \n"
			+ " FROM xmis.hr_人员部门关系表  b   \n"
			+ " join xmis.hr_人员 a  on  a.人员ID =  b.人员ID \n"
			+ " left join xmis.sys_职务 c  on  b.职务 =  c.职务ID \n"
			+ " left join xmis.org_部门 d  on  b.部门ID =  d.部门ID \n"
			+ " left join  ( SELECT dcomp.部门ID,dcomp.公司ID,comp.公司名称,dcomp.是否直属,dcomp.排序 \n"
			+ " FROM xmis.org_部门归属公司表 dcomp  \n"
			+ " JOIN xmis.org_公司 comp ON  dcomp.公司ID = comp.公司ID )  e \n"
			+ " on  b.部门ID =  e.部门ID \n"
			+ " left join xmis.org_部门归属部门表 f  on  b.部门ID =  f.部门ID \n"
			+ " left join xmis.org_部门 g  on  g.部门ID =  f.父部门ID left join xmis.add_地址 ad on a.办公地址ID = ad.地址ID    \n"
			+ " WHERE  a.人员ID > 0  and  b.部门ID = ? \n"
			+ " and  (  a.禁用 = 0  or  a.禁用 is NULL  )  \n"
			+ " ORDER BY  cp  , bp ,userName \n";

	public static String getAllEmpInfo = "\n declare @maxCP int, @maxBP int \n"
		+ " select @maxCP = MAX(排序)+1 from xmis.sys_职务 \n"
		+ " select @maxBP = MAX(排序)+1 from xmis.hr_人员部门关系表 \n"
		+ " SELECT  a.人员ID empId,  b.部门ID tdeptId,d.部门名称 tdeptName, (RTRIM(姓氏)+RTRIM(名字))  userName ,   \n"
		+ " c.职务名称 position, 移动电话 mobileNo, 单位电话 switchboardNo, 单位分机 extNo, 传真号码 faxNo, \n"
		+ " 电子邮件 emailAddress, 详细地址 officeAddress,  \n"
		+ " case when  g.部门名称 is null then  d.部门名称 else case when  c.排序 < 102 then  g.部门名称 else  g.部门名称 +  d.部门名称 end end  AS  departmentName  , \n"
		+ " case when e.是否直属 =1  then b.部门ID else  f.父部门ID end deptId , \n"
		+ " CASE WHEN e.是否直属 = 1 THEN d.部门名称 ELSE g.部门名称  END deptName , \n"
		+ " case when c.排序 is NULL then @maxCP else c.排序 end cp,  case when b.排序 is NULL then @maxBP else b.排序 end  bp \n"
		+ " ,e.公司ID compId ,e.公司名称 compName \n"
		+ " FROM xmis.hr_人员部门关系表  b   \n"
		+ " join xmis.hr_人员 a  on  a.人员ID =  b.人员ID \n"
		+ " left join xmis.sys_职务 c  on  b.职务 =  c.职务ID \n"
		+ " left join xmis.org_部门 d  on  b.部门ID =  d.部门ID \n"
		+ " left join  ( SELECT dcomp.部门ID,dcomp.公司ID,comp.公司名称,dcomp.是否直属,dcomp.排序 \n"
		+ " FROM xmis.org_部门归属公司表 dcomp  \n"
		+ " JOIN xmis.org_公司 comp ON  dcomp.公司ID = comp.公司ID )  e \n"
		+ " on  b.部门ID =  e.部门ID \n"
		+ " left join xmis.org_部门归属部门表 f  on  b.部门ID =  f.部门ID \n"
		+ " left join xmis.org_部门 g  on  g.部门ID =  f.父部门ID left join xmis.add_地址 ad on a.办公地址ID = ad.地址ID    \n"
		+ " WHERE  a.人员ID > 0 " +
			"  AND b.部门ID in (SELECT 部门ID   FROM    xmis.org_部门归属公司表 WHERE 公司ID=1) \n"
		+ " and  (  a.禁用 = 0  or  a.禁用 is NULL  )  \n"
		+ " ORDER BY  cp  , bp ,userName \n";

	// 获取公司领导及事业部领导信息
	public static String getLeaderInfo = " \ndeclare @maxCP int, @maxBP int \n"
			+ " select @maxCP = MAX(排序)+1 from xmis.sys_职务 \n"
			+ " select @maxBP = MAX(排序)+1 from xmis.hr_人员部门关系表 \n"
			+ " SELECT new.empId empId, new.事业部级部门ID deptId, new.deptId tdeptId, new.userName userName,  \n"
			+ " new.departmentName departmentName, new.position position, new.mobileNo mobileNo,  \n"
			+ " new.switchboardNo switchboardNo, new.extNo extNo, new.faxNo faxNo, new.emailAddress emailAddress, new.officeAddress officeAddress,h.排序 , new.cp,new.bp \n"
			+ " FROM ( \n"
			+ " SELECT a.人员ID empId,  b.部门ID deptId,  (RTRIM(姓氏)+RTRIM(名字)) userName,  \n"
			+ " c.职务名称 position, 移动电话 mobileNo, 单位电话 switchboardNo,  \n"
			+ " 单位分机 extNo, 传真号码 faxNo, 电子邮件 emailAddress, 详细地址 officeAddress,  \n"
			+ " e.排序 e排序,  d.部门ID d部门ID,  \n"
			+ " case when e.是否直属 =1  then b.部门ID else  f.父部门ID end 事业部级部门ID, \n"
			+ " case when  g.部门名称 is null then  d.部门名称 else case when  c.排序 < 102 then  g.部门名称 else  g.部门名称 +  d.部门名称 end end  AS  departmentName, \n"
			+ " case when c.排序 is NULL then @maxCP else c.排序 end cp,  case when b.排序 is NULL then @maxBP else b.排序 end  bp \n"
			+ " FROM xmis.hr_人员部门关系表  b   \n"
			+ " join xmis.hr_人员 a  on  a.人员ID =  b.人员ID \n"
			+ " left join xmis.sys_职务 c  on  b.职务 =  c.职务ID \n"
			+ " left join xmis.org_部门 d  on  b.部门ID =  d.部门ID \n"
			+ " left join xmis.org_部门归属公司表 e  on  b.部门ID =  e.部门ID \n"
			+ " left join xmis.org_部门归属部门表 f  on  b.部门ID =  f.部门ID \n"
			+ " left join xmis.org_部门 g  on  g.部门ID =  f.父部门ID left join xmis.add_地址 ad on a.办公地址ID = ad.地址ID    \n"
			+ " WHERE  a.人员ID >0  and  e.公司ID = 1 and  c.排序<102  and  (  a.禁用 = 0  or  a.禁用 is NULL  ) AND b.职务  IN (1,2,3,4,5)  ) AS  new  \n"
			+ " left join xmis.org_部门归属公司表 h  on  new.事业部级部门ID = h.部门ID \n"
			+ " ORDER BY  h.排序 , cp,bp,userName \n";

	// 获取公司全部员工信息
	public static String getEmps = " declare @maxCP int, @maxBP int \n"
			+ " select @maxCP = MAX(排序)+1 from xmis.sys_职务  \n"
			+ " select @maxBP = MAX(排序)+1 from xmis.hr_人员部门关系表 \n"
			+ " SELECT new.empId empId, new.事业部级部门ID deptId, new.deptId tdeptId, \n"
			+ " new.userName userName, new.departmentName departmentName, new.position position,  \n"
			+ " new.mobileNo mobileNo, new.switchboardNo switchboardNo, new.extNo extNo,  \n"
			+ " new.faxNo faxNo, new.emailAddress emailAddress, new.officeAddress officeAddress, \n"
			+ " h.排序 ,  new.f排序,  new.cp , bp \n"
			+ " into #result  \n"
			+ " FROM  (  \n"
			+ " SELECT a.人员ID empId,  b.部门ID deptId,  (RTRIM(姓氏)+RTRIM(名字)) userName,  c.职务名称 position, \n"
			+ " 移动电话 mobileNo, 单位电话 switchboardNo, 单位分机 extNo, 传真号码 faxNo, 电子邮件 emailAddress, \n"
			+ " 详细地址 officeAddress,  e.排序 e排序,  d.部门ID d部门ID,  c.排序 c排序,  \n"
			+ " case when b.排序 is NULL then @maxBP else b.排序 end  bp, \n"
			+ " case when e.是否直属 =1  then b.部门ID else  f.父部门ID end 事业部级部门ID,  \n"
			+ " case when  g.部门名称 is null then  d.部门名称 else case when  c.排序 < 102 then  g.部门名称 else  g.部门名称 +  d.部门名称 end end  AS  departmentName , \n"
			+ " f.排序 f排序, \n"
			+ " case when c.排序 is NULL then @maxCP else c.排序 end cp \n"
			+ " FROM xmis.hr_人员部门关系表  b   \n"
			+ " join xmis.hr_人员 a  on  a.人员ID =  b.人员ID \n"
			+ " left join xmis.sys_职务 c  on  b.职务 =  c.职务ID \n"
			+ " left join xmis.org_部门 d  on  b.部门ID =  d.部门ID \n"
			+ " left join xmis.org_部门归属公司表 e  on  b.部门ID =  e.部门ID \n"
			+ " left join xmis.org_部门归属部门表 f  on  b.部门ID =  f.部门ID \n"
			+ " left join xmis.org_部门 g  on  g.部门ID =  f.父部门ID left join xmis.add_地址 ad on a.办公地址ID = ad.地址ID  \n"
			+ " WHERE  a.人员ID >0  and  e.公司ID=1 and  (  a.禁用 = 0  or  a.禁用 is NULL  )  )  AS  new  \n"
			+ " left join xmis.org_部门归属公司表 h  on  new.事业部级部门ID = h.部门ID \n"
			+ " ORDER BY  h.排序 ,  new.f排序,  new.cp , new.bp ,new.userName \n";

	// 删除附件
	public static String DeleteAttachmentByID = delete + " from "
			+ DBSchema.Attachment.TableName + " where "
			+ DBSchema.Attachment.AttachmentID + " =? ";

	// 提示上传文件已存在时（覆盖/取消）
	public static String existValidate = select + " count(*) " + " from "
			+ DBSchema.Attachment.TableName + " where "
			+ DBSchema.Attachment.AttachmentTypeID + " = ?" + " and "
			+ DBSchema.Attachment.REFID + " = ?" + " and "
			+ DBSchema.Attachment.AttachmentName + " = " + "?";

	public static String existValidateReturnAttID = select + " 附件ID "
			+ " from " + DBSchema.Attachment.TableName + " where "
			+ DBSchema.Attachment.AttachmentTypeID + " = ?" + " and "
			+ DBSchema.Attachment.REFID + " = ?" + " and "
			+ DBSchema.Attachment.AttachmentName + " = " + "?";

	// 覆盖(上传文件已存在时删除原有的)
	public static String deleteByConIDFileName = delete + " from "
			+ DBSchema.Attachment.TableName + " where "
			+ DBSchema.Attachment.AttachmentTypeID + " = ?" + " and "
			+ DBSchema.Attachment.REFID + " = ? " + " and "
			+ DBSchema.Attachment.AttachmentName + " = ?";

	// 获取附件列表 dmj
	public static String getAttachmentList = select
			+ DBSchema.Attachment.TableName + "."
			+ DBSchema.Attachment.AttachmentID + ","
			+ DBSchema.Attachment.TableName + "."
			+ DBSchema.Attachment.AttachmentName + ", "
			+ DBSchema.AttachmentType.TableName + "."
			+ DBSchema.AttachmentType.AttachmentTypeDesc + ","

			+ "Convert(varchar(10)," + DBSchema.Attachment.TableName + "."
			+ DBSchema.Attachment.UploadDate + ",23) as " + " upload_date "
			+ ", "

			+ "RTRIM(" + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.FirstName + ")+RTRIM("
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.LastName
			+ ") as " + " uploader_name " + from
			+ DBSchema.Attachment.TableName + ","
			+ DBSchema.AttachmentType.TableName + ","
			+ DBSchema.Employee.TableName + where
			+ "  "+DBSchema.Attachment.AttachmentID +">0 and "
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

	public static String getFinanceContractAmtContractInTimeScope = getContractListByType
			.replace("#result", "#contract")
			+ "\n DECLARE @beginDate datetime  set @beginDate = ? \n DECLARE @endDate datetime  set @endDate = ? "

			+ "\n SELECT #contract.合同ID, 0 as contract_payed_amt, 0 as contract_invoice_amt, 0 as contract_estimate_amt, CONVERT(DECIMAL(18, 2), 0) AS MB,CONVERT(DECIMAL(18, 2), 0) as PayAmt"
			+ "\n INTO #contract_pay_invoice"
			+ "\n FROM #contract where 1=2"
			+ "\n if(@contractType = 1) begin"
			+ "\n insert into #contract_pay_invoice"
			+ "\n select #contract.合同ID,0,0,0,SUM(mc) - SUM(md),0"
			+ "\n FROM xmis.fas_GL_accvouch,xmis.fas_income,xmis.fas_codeType,"
			+ "\n #contract"
			+ "\n WHERE xmis.fas_GL_accvouch.year = xmis.fas_income.year        "
			+ "\n AND xmis.fas_GL_accvouch.iperiod = xmis.fas_income.iperiod "
			+ "\n AND xmis.fas_GL_accvouch.ino_id = xmis.fas_income.ino_id        "
			+ "\n AND xmis.fas_GL_accvouch.citem_id = xmis.fas_income.citem_id "
			+ "\n AND xmis.fas_codeType.type IN(0,101)        "
			+ "\n AND xmis.fas_codeType.type = xmis.fas_income.type "
			+ "\n AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode "
			+ "\n AND xmis.fas_GL_accvouch.year = xmis.fas_codeType.year "
			+ "\n and xmis.fas_income.project_code = #contract.项目编码"
			+ "\n and "
			+ "\n xmis.fas_GL_accvouch.dbill_date >=@beginDate" // 7
			+ "\n and "
			+ "\n xmis.fas_GL_accvouch.dbill_date <=@endDate" // 8
			+ "\n GROUP BY xmis.fas_GL_accvouch.year , xmis.fas_GL_accvouch.iperiod , xmis.fas_GL_accvouch.ino_id , "
			+ "\n xmis.fas_GL_accvouch.dbill_date , cdigest , xmis.fas_income.project_code  ,#contract.合同ID"
			+ "\n HAVING  SUM(mc) - SUM(md) <> 0 ORDER BY xmis.fas_GL_accvouch.dbill_date "
			+ "\n if(@beginDate<'2008-01-01') begin" // 9
			+ "\n insert into #contract_pay_invoice"
			+ "\n select #contract.合同ID,0,0,0,SUM(income_mb),0"
			+ "\n FROM xmis.fas_qch , xmis.fas_fitemss00,"
			+ "\n #contract"
			+ "\n WHERE  xmis.fas_fitemss00.year = 2008 and  xmis.fas_qch.citem_id = xmis.fas_fitemss00.citemcode and project_code = "
			+ "\n #contract.项目编码"
			+ "\n GROUP BY project_code,#contract.合同ID"

			+ "\n insert into #contract_pay_invoice"
			+ "\n select #contract.合同ID ,0,0,0, 0 , SUM(payout_mb)"
			+ "\n FROM xmis.fas_qcjcb , xmis.fas_fitemss00 ,"
			+ "\n #contract"
			+ "\n WHERE xmis.fas_fitemss00.year = 2008 "
			+ "\n and xmis.fas_qcjcb.citem_id = xmis.fas_fitemss00.citemcode "
			+ "\n and project_code = #contract.项目编码"
			+ "\n GROUP BY project_code ,#contract.合同ID"
			+ "\n end "

			+ "\n insert into #contract_pay_invoice"
			+ "\n select #contract.合同ID, 0,0,0,0 , SUM(md - mc)"
			+ "\n FROM xmis.fas_GL_accvouch , xmis.fas_fitemss00 , xmis.fas_codeType ,#contract"
			+ "\n WHERE xmis.fas_GL_accvouch.year = xmis.fas_fitemss00.year        "
			+ "\n AND xmis.fas_GL_accvouch.citem_id = xmis.fas_fitemss00.citemcode "
			+ "\n AND xmis.fas_fitemss00.bclose = 0        "
			+ "\n AND xmis.fas_GL_accvouch.iperiod > 0    "
			+ "\n AND xmis.fas_GL_accvouch.iperiod <= 12 "
			+ "\n AND NOT ( fas_GL_accvouch.year = 2011"
			+ "\n AND fas_GL_accvouch.iperiod = 8  "
			+ "\n AND fas_GL_accvouch.ino_id IN ( 327, 466 ))"
			+ "\n AND xmis.fas_GL_accvouch.year = xmis.fas_codeType.year"
			+ "\n AND xmis.fas_codeType.type = 1"
			+ "\n AND xmis.fas_GL_accvouch.ccode = xmis.fas_codeType.ccode "
			+ "\n AND project_code = #contract.项目编码"
			+ "\n and "
			+ "\n xmis.fas_GL_accvouch.dbill_date >=@beginDate" // 10
			+ "\n and "
			+ "\n xmis.fas_GL_accvouch.dbill_date <=@endDate" // 11
			+ "\n GROUP BY xmis.fas_GL_accvouch.year , xmis.fas_GL_accvouch.iperiod , xmis.fas_GL_accvouch.ino_id , project_code , "
			+ "\n citem_id , xmis.fas_GL_accvouch.cdigest , xmis.fas_GL_accvouch.dbill_date ,#contract.合同ID"
			+ "\n HAVING  SUM(md - mc) <> 0"
			+ "\n ORDER BY xmis.fas_GL_accvouch.dbill_date end"

			+ "\n SELECT 合同ID, sum(contract_payed_amt) as contract_payed_amt, sum(contract_invoice_amt) as contract_invoice_amt, sum(contract_estimate_amt) as contract_estimate_amt,SUM(MB) as officialReceiptsMB,SUM(payAmt) as officialReceiptsPayAmt"
			+ "\n INTO #contract_pay_invoice_sum"
			+ "\n FROM #contract_pay_invoice"
			+ "\n GROUP BY 合同ID"

			+ "\n SELECT #contract.*, contract_payed_amt, contract_invoice_amt,contract_estimate_amt,officialReceiptsMB,officialReceiptsPayAmt "
			+ "\n INTO #result"
			+ "\n FROM #contract,#contract_pay_invoice_sum "
			+ "\n WHERE #contract.合同ID= #contract_pay_invoice_sum.合同ID ";

	public static String getPerformedContractInTimeScope = getContractListByType
			.replace("#result", "#contract")
			+ "\n SELECT xmis.con_收款明细表.合同ID, sum(xmis.con_收款明细表.收款金额) as contract_payed_amt, 0 as contract_invoice_amt, 0 as contract_estimate_amt, CONVERT(DECIMAL(18, 2), 0) AS officialReceiptsMB,CONVERT(DECIMAL(18, 2), 0) as officialReceiptsPayAmt"
			+ "\n INTO #contract_pay_invoice"
			+ "\n FROM xmis.con_收款明细表"
			+ "\n WHERE xmis.con_收款明细表.收款日期>=?"
			+ "\n AND xmis.con_收款明细表.收款日期<=?"
			+ "\n GROUP BY xmis.con_收款明细表.合同ID"
			+ "\n SELECT #contract.*, contract_payed_amt, contract_invoice_amt,contract_estimate_amt,officialReceiptsMB,officialReceiptsPayAmt "
			+ "\n INTO #result"
			+ "\n FROM #contract,#contract_pay_invoice "
			+ "\n WHERE #contract.合同ID= #contract_pay_invoice.合同ID ";

	public static String getInvoiceContractInTimeScope = getContractListByType
			.replace("#result", "#contract")
			+ "\n SELECT xmis.con_发票开具表.合同ID, 0 as contract_payed_amt, sum(xmis.con_发票开具表.发票金额) as contract_invoice_amt, 0 as contract_estimate_amt, CONVERT(DECIMAL(18, 2), 0) AS officialReceiptsMB,CONVERT(DECIMAL(18, 2), 0) as officialReceiptsPayAmt"
			+ "\n INTO #contract_pay_invoice"
			+ "\n FROM xmis.con_发票开具表"
			+ "\n WHERE xmis.con_发票开具表.开具日期>=?"
			+ "\n AND xmis.con_发票开具表.开具日期<=?"
			+ "\n GROUP BY xmis.con_发票开具表.合同ID"
			+ "\n SELECT #contract.*, contract_payed_amt, contract_invoice_amt,contract_estimate_amt,officialReceiptsMB,officialReceiptsPayAmt "
			+ "\n INTO #result"
			+ "\n FROM #contract,#contract_pay_invoice "
			+ "\n WHERE #contract.合同ID= #contract_pay_invoice.合同ID ";

	public static String getEstimateContractAmtContractInTimeScope = getContractListByType
			.replace("#result", "#contract")
			+ "\n SELECT xmis.con_收款条件表.合同ID, 0 as contract_payed_amt, 0 as contract_invoice_amt, sum(xmis.con_收款条件表.收款金额) as contract_estimate_amt, CONVERT(DECIMAL(18, 2), 0) AS officialReceiptsMB,CONVERT(DECIMAL(18, 2), 0) as officialReceiptsPayAmt"
			+ "\n INTO #contract_pay_invoice"
			+ "\n FROM xmis.con_收款条件表"
			+ "\n WHERE xmis.con_收款条件表.收款日期>=?"
			+ "\n AND xmis.con_收款条件表.收款日期<=?"
			+ "\n GROUP BY xmis.con_收款条件表.合同ID"
			+ "\n SELECT #contract.*, contract_payed_amt, contract_invoice_amt,contract_estimate_amt,officialReceiptsMB,officialReceiptsPayAmt "
			+ "\n INTO #result"
			+ "\n FROM #contract,#contract_pay_invoice "
			+ "\n WHERE #contract.合同ID= #contract_pay_invoice.合同ID ";

	public static String getIncomeAssistListByAccurateSearch = " SET NOCOUNT ON  \n"
			+ " select  \n"
			+ "  contract.合同ID  \n"
			+ " ,contract.合同名称  \n"
			+ " ,contract.简称  \n"
			+ " ,contract.甲方公司ID  \n"
			+ " ,contract.乙方部门ID  \n"
			+ " ,project.项目ID  \n"
			+ " ,project.项目编码  \n"
			+ " ,project.项目状态ID   \n"
			+ " ,case when gl.MB is null then 0 else gl.MB end as MB  \n"
			+ " ,contract.合同总额  \n"
			+ " into #totalContract  \n"
			+ " from xmis.con_合同 as contract  \n"
			+ " join xmis.prj_项目 as project on project.项目ID = contract.项目ID  \n"
			+ " left join xmis.fas_glInfo as gl on project.项目编码 = gl.project_code  \n"
			+ " where contract.合同类型ID = 1  \n"
			+ " and contract.父合同ID is null  \n"
			+ " and contract.乙方部门ID is not null  \n"
			+ " and contract.合同总额> (case when gl.MB is null then 0 else gl.MB end)  \n"
			+ " select paymentCondition.*,#totalContract.项目ID  \n"
			+ " into #paymentCondition  \n"
			+ " from xmis.con_收款条件表 as paymentCondition  \n"
			+ " ,#totalContract  \n"
			+ " where paymentCondition.合同ID = #totalContract.合同ID  \n"
			+ " SELECT  \n"
			+ "  #paymentCondition.合同ID as 合同ID  \n"
			+ " ,#paymentCondition.收款条件ID as 收款条件ID  \n"
			+ " ,#paymentCondition.对应项目状态 as 项目状态ID  \n"
			+ " ,#paymentCondition.延迟天数 as  延迟天数  \n"
			+ " ,#paymentCondition.收款金额 as  收款金额  \n"
			+ " ,xmis.prj_项目状态.项目状态 as 项目状态名称  \n"
			+ " ,#paymentCondition.项目ID as 项目ID  \n"
			+ " ,CONVERT(DECIMAL(18,2),0.00) as realIncome  \n"
			+ " ,#paymentCondition.是否已到款 as 是否已到款  \n"
			+ " into #temp2     \n"
			+ " FROM #paymentCondition  \n"
			+ " ,xmis.prj_项目状态  \n"
			+ " WHERE #paymentCondition.对应项目状态 *= xmis.prj_项目状态.项目状态ID  \n"
			+ " select #temp2.*  \n"
			+ " ,xmis.prj_项目进度计划.预计开始日期 as 预计开始日期    \n"
			+ " ,dateadd(d,#temp2.延迟天数,xmis.prj_项目进度计划.预计开始日期) as 预计到款日期  \n"
			+ " ,xmis.prj_项目进度计划.实际开始日期 as td   \n"
			+ " into #temp1     \n"
			+ " from #temp2,xmis.prj_项目进度计划     \n"
			+ " where #temp2.项目状态ID = xmis.prj_项目进度计划.项目状态ID  \n"
			+ " and #temp2.项目ID = xmis.prj_项目进度计划.项目ID  \n"
			+ " order by xmis.prj_项目进度计划.预计开始日期  \n"
			+ " select identity(int,0,1) as row_no,#temp1.* into #temp from #temp1   \n"
			+ " order by 合同ID,预计到款日期,td  \n"
			+ " SELECT 项目ID  \n"
			+ " 	  ,project_code  \n"
			+ "      ,MB  \n"
			+ " into #incomelist  \n"
			+ " FROM xmis.fas_glInfo,xmis.prj_项目  \n"
			+ " where project_code = 项目编码  \n"
			+ " declare @totalImcome DECIMAL(18,2)   \n"
			+ " declare @allContractCount int  \n"
			+ " declare @countInContract int  \n"
			+ " declare @i int  \n"
			+ " set @i = 0 \n"
			+ " select identity(int,0,1) as row_no,合同ID,count(合同ID) as num,项目ID,min(row_no) as start into #allContract from #temp group by 合同ID,项目ID order by 合同ID  \n"
			+ " select @allContractCount = count(*) from #allContract  \n"
			+ " while(@i < @allContractCount) begin   \n"
			+ " 	declare @projectID nvarchar(100)  \n"
			+ " 	declare @j int  \n"
			+ " 	select @countInContract = num + start,@projectID=项目ID,@j=start from #allContract where row_no = @i  \n"
			+ " 	select @totalImcome = MB from #incomelist where 项目ID = @projectID  \n"
			+ " 	while(@j < @countInContract and @totalImcome > 0) begin  \n"
			+ " 		 declare @total1 DECIMAL(18,2)   \n"
			+ " 		 declare @total2 DECIMAL(18,2)   \n"
			+ " 		 select @total1 = 收款金额 from #temp where row_no = @j   \n"
			+ " 		 if(@total1<=@totalImcome) begin   \n"
			+ " 		 set @total2 = @total1   \n"
			+ " 		 set @totalImcome = @totalImcome - @total1   \n"
			+ " 		 end   \n"
			+ " 		 else begin   \n"
			+ " 		 set @total2 = @totalImcome  \n"
			+ " 		 set @totalImcome = 0   \n"
			+ "  		 end   \n"
			+ "  		 update #temp set realIncome=@total2 where row_no = @j  \n"
			+ " 		 set @j = @j + 1   \n"
			+ "  	end   \n"
			+ "  	set @i = @i + 1   \n"
			+ " end \n"
			+ " select *   \n"
			+ " into #paymentResult  \n"
			+ " from #temp where 收款金额 > realIncome  \n"
			+ " update #paymentResult set 收款金额=收款金额-realIncome,realIncome=0 where realIncome>0 \n"
			+ " --发票 \n"
			+ " select identity(int,0,1) as row_no \n"
			+ " ,#totalContract.合同ID as 合同ID \n"
			+ " ,#totalContract.项目ID as 项目ID \n"
			+ " ,xmis.con_发票开具表.发票号 as 发票号 \n"
			+ " ,xmis.con_发票开具表.发票金额 as 发票金额 \n"
			+ " ,CONVERT(DECIMAL(18,2),0.00) as realIncome  \n"
			+ " ,xmis.con_发票开具表.开具日期 as 开具日期 \n"
			+ " ,xmis.con_发票开具表.是否已到款 as 是否已到款 \n"
			+ " into #invoice \n"
			+ " from #totalContract,xmis.con_发票开具表  \n"
			+ " where #totalContract.合同ID = xmis.con_发票开具表.合同ID  \n"
			+ " order by #totalContract.合同ID,xmis.con_发票开具表.开具日期 \n"
			+ " select identity(int,0,1) as row_no,合同ID,count(合同ID) as num,项目ID,min(row_no) as start into #allInvoiceContract from #invoice group by 合同ID,项目ID order by 合同ID \n"
			+ " select @allContractCount = count(*) from #allInvoiceContract \n"
			+ " set @i = 0 \n"
			+ " while(@i < @allContractCount) begin \n"
			+ " 	select @countInContract = num + start,@projectID=项目ID,@j=start from #allInvoiceContract where row_no = @i  \n"
			+ " 	select @totalImcome = MB from #incomelist where 项目ID = @projectID  \n"
			+ " 	while(@j < @countInContract and @totalImcome > 0) begin   \n"
			+ " 		 select @total1 = 发票金额 from #invoice where row_no = @j   \n"
			+ " 		 if(@total1<=@totalImcome) begin   \n"
			+ " 		 set @total2 = @total1   \n"
			+ " 		 set @totalImcome = @totalImcome - @total1   \n"
			+ " 		 end   \n"
			+ " 		 else begin   \n"
			+ " 		 set @total2 = @totalImcome  \n"
			+ " 		 set @totalImcome = 0   \n"
			+ "  		 end   \n"
			+ "  		 update #invoice set realIncome=@total2 where row_no = @j  \n"
			+ " 		 set @j = @j + 1   \n"
			+ "  	end   \n"
			+ "  	set @i = @i + 1   \n"
			+ " end \n"
			+ " select *   \n"
			+ " into #invoiceResult  \n"
			+ " from #invoice where 发票金额 > realIncome  \n"
			+ " update #invoiceResult set 发票金额=发票金额-realIncome,realIncome=0 where realIncome>0 \n"
			+ " --收据 \n"
			+ "  select identity(int,0,1) as row_no \n"
			+ " ,#totalContract.合同ID as 合同ID \n"
			+ " ,#totalContract.项目ID as 项目ID \n"
			+ " ,xmis.con_收据明细表.收据ID as 收据ID \n"
			+ " ,xmis.con_收据明细表.收据金额 as 收据金额 \n"
			+ " ,CONVERT(DECIMAL(18,2),0.00) as realIncome  \n"
			+ " ,xmis.con_收据明细表.收据日期 as 收据日期 \n"
			+ " ,xmis.con_收据明细表.是否已到款 as 是否已到款 \n"
			+ " into #receipt \n"
			+ " from #totalContract,xmis.con_收据明细表 \n"
			+ " where #totalContract.合同ID = xmis.con_收据明细表.合同ID  \n"
			+ " order by #totalContract.合同ID,xmis.con_收据明细表.收据日期 \n"
			+ " select identity(int,0,1) as row_no,合同ID,count(合同ID) as num,项目ID,min(row_no) as start into #allReceiptContract from #receipt group by 合同ID,项目ID order by 合同ID \n"
			+ " select @allContractCount = count(*) from #allReceiptContract \n"
			+ " set @i = 0 \n"
			+ " while(@i < @allContractCount) begin \n"
			+ " 	select @countInContract = num + start,@projectID=项目ID,@j=start from #allReceiptContract where row_no = @i  \n"
			+ " 	select @totalImcome = MB from #incomelist where 项目ID = @projectID  \n"
			+ " 	while(@j < @countInContract and @totalImcome > 0) begin   \n"
			+ " 		 select @total1 = 收据金额 from #receipt where row_no = @j   \n"
			+ " 		 if(@total1<=@totalImcome) begin   \n"
			+ " 		 set @total2 = @total1   \n"
			+ " 		 set @totalImcome = @totalImcome - @total1   \n"
			+ " 		 end   \n"
			+ " 		 else begin   \n"
			+ " 		 set @total2 = @totalImcome  \n"
			+ " 		 set @totalImcome = 0   \n"
			+ "  		 end   \n"
			+ "   		 update #receipt set realIncome=@total2 where row_no = @j  \n"
			+ " 		 set @j = @j + 1   \n"
			+ "  	end   \n"
			+ "  	set @i = @i + 1   \n"
			+ " end \n"
			+ " select *   \n"
			+ " into #receiptResult  \n"
			+ " from #receipt where 收据金额 > realIncome  \n"
			+ " update #receiptResult set 收据金额=收据金额-realIncome,realIncome=0 where realIncome>0 \n"
			+ " SELECT contract.合同ID as 合同ID  \n"
			+ " ,contract.简称 as 合同名称  \n"
			+ " ,contract.乙方部门ID as 乙方部门ID  \n"
			+ " ,#paymentResult.收款金额 as 金额  \n"
			+ " ,project.项目编码 as 项目编码  \n"
			+ " ,#paymentResult.预计到款日期 as 日期  \n"
			+ " ,Convert(varchar(50),收款条件ID) as 票据号  \n"
			+ " ,company.公司名称 as 公司名称  \n"
			+ " ,company.名称缩写 as 公司简称  \n"
			+ " ,company.公司ID as 公司ID  \n"
			+ " ,1 as 来源  \n"
			+ " ,project.项目ID as 项目ID \n"
			+ "  into #prev  \n"
			+ " from #totalContract as contract  \n"
			+ " ,#paymentResult  \n"
			+ " ,xmis.org_公司 as company  \n"
			+ " ,xmis.prj_项目 as project   \n"
			+ " where contract.合同ID = #paymentResult.合同ID  \n"
			+ " AND contract.甲方公司ID = company.公司ID  \n"
			+ " AND #paymentResult.项目ID = project.项目ID  \n"
			+ " and (#paymentResult.是否已到款 = 0 or #paymentResult.是否已到款 is null) \n"
			+ " union all  \n"
			+ " --手工设置  \n"
			+ " SELECT contract.合同ID as 合同ID  \n"
			+ " ,contract.简称 as 合同名称  \n"
			+ " ,contract.乙方部门ID as 乙方部门ID  \n"
			+ " ,estimatedIncome.预计金额 as 金额  \n"
			+ " ,project.项目编码 as 项目编码  \n"
			+ " ,estimatedIncome.预计到款时间 as 日期  \n"
			+ " ,Convert(varchar(50),estimatedIncome.ID)  \n"
			+ "  ,company.公司名称 as 公司名称  \n"
			+ "  ,company.名称缩写 as 公司简称  \n"
			+ " ,company.公司ID as 公司ID  \n"
			+ " ,2 as 来源  \n"
			+ " ,project.项目ID \n"
			+ " FROM #totalContract as contract  \n"
			+ " ,xmis.org_公司 as company  \n"
			+ " ,xmis.con_预计来款明细表 as estimatedIncome  \n"
			+ " ,xmis.prj_项目 as project   \n"
			+ " where contract.项目ID = estimatedIncome.项目ID  \n"
			+ " and estimatedIncome.是否已到款 = 0  \n"
			+ " AND contract.甲方公司ID = company.公司ID  \n"
			+ " AND contract.项目ID = project.项目ID  \n"
			+ " union all  \n"
			+ " --发票  \n"
			+ " SELECT contract.合同ID as 合同ID  \n"
			+ " ,contract.简称 as 合同名称  \n"
			+ "  ,contract.乙方部门ID as 乙方部门ID  \n"
			+ " ,invoice.发票金额 as 金额  \n"
			+ " ,project.项目编码 as 项目编码  \n"
			// + " ,invoice.开具日期 as 日期  \n"
			+ " ,null as 日期  \n"
			+ " ,Convert(varchar(50),invoice.发票号)  \n"
			+ " ,company.公司名称 as 公司名称  \n"
			+ " ,company.名称缩写 as 公司简称  \n"
			+ " ,company.公司ID as 公司ID  \n"
			+ " ,3 as 来源  \n"
			+ " ,project.项目ID \n"
			+ " FROM #totalContract as contract  \n"
			+ " ,xmis.org_公司 as company  \n"
			+ " ,#invoiceResult as invoice  \n"
			+ " ,xmis.prj_项目 as project   \n"
			+ " where contract.合同ID = invoice.合同ID  \n"
			+ " AND contract.甲方公司ID = company.公司ID  \n"
			+ " AND contract.项目ID = project.项目ID  \n"
			+ " and (invoice.是否已到款 = 0 or invoice.是否已到款 is null) \n"
			+ " union all  \n"
			+ " --收据  \n"
			+ " SELECT contract.合同ID as 合同ID  \n"
			+ " ,contract.简称 as 合同名称  \n"
			+ " ,contract.乙方部门ID as 乙方部门ID  \n"
			+ " ,receipt.收据金额 as 金额  \n"
			+ " ,project.项目编码 as 项目编码  \n"
			// + " ,receipt.收据日期 as 日期  \n"
			+ " ,null as 日期  \n"
			+ " ,Convert(varchar(50),receipt.收据ID)  \n"
			+ " ,company.公司名称 as 公司名称  \n"
			+ " ,company.名称缩写 as 公司简称  \n"
			+ " ,company.公司ID as 公司ID  \n"
			+ " ,4 as 来源  \n"
			+ " ,project.项目ID \n"
			+ " FROM #totalContract as contract  \n"
			+ " ,xmis.org_公司 as company  \n"
			+ " ,#receiptResult as receipt  \n"
			+ " ,xmis.prj_项目 as project   \n"
			+ " where contract.合同ID = receipt.合同ID  \n"
			+ " AND contract.甲方公司ID = company.公司ID  \n"
			+ " AND contract.项目ID = project.项目ID  \n"
			+ " and (receipt.是否已到款 = 0 or receipt.是否已到款 is null) \n";

	public static String getIncomeAssistListByUnIncomeSearch = " --得到所有未执行完的合同 \n"
			+ "  select  \n"
			+ "  contract.合同ID \n"
			+ " ,contract.合同名称 \n"
			+ " ,contract.简称 \n"
			+ " ,contract.甲方公司ID \n"
			+ " ,contract.乙方部门ID \n"
			+ " ,project.项目ID \n"
			+ " ,project.项目编码 \n"
			+ " ,project.项目状态ID  \n"
			+ " ,case when gl.MB is null then 0 else gl.MB end as MB \n"
			+ " ,contract.合同总额 \n"
			+ " into #totalContract \n"
			+ " from xmis.con_合同 as contract \n"
			+ " join xmis.prj_项目 as project on project.项目ID = contract.项目ID \n"
			+ " left join xmis.fas_glInfo as gl on project.项目编码 = gl.project_code \n"
			+ " where contract.合同类型ID = 1 \n"
			+ " and contract.父合同ID is null \n"
			+ " and contract.乙方部门ID is not null \n"
			+ " and contract.合同总额> (case when gl.MB is null then 0 else gl.MB end) \n"
			+ " select paymentCondition.*,#totalContract.项目ID  \n"
			+ " into #paymentCondition \n"
			+ " from xmis.con_收款条件表 as paymentCondition \n"
			+ " ,#totalContract  \n"
			+ " where paymentCondition.合同ID = #totalContract.合同ID \n"
			+ " SELECT \n"
			+ " #paymentCondition.合同ID as 合同ID  \n"
			+ " ,#paymentCondition.收款条件ID as 收款条件ID \n"
			+ " ,#paymentCondition.对应项目状态 as 项目状态ID \n"
			+ " ,#paymentCondition.延迟天数 as  延迟天数 \n"
			+ " ,#paymentCondition.收款金额 as  收款金额 \n"
			+ " ,xmis.prj_项目状态.项目状态 as 项目状态名称 \n"
			+ " ,#paymentCondition.项目ID as 项目ID \n"
			+ " ,CONVERT(DECIMAL(18,2),0.00) as realIncome \n"
			+ " ,#paymentCondition.是否已到款 as 是否已到款 \n"
			+ " into #temp2    \n"
			+ " FROM #paymentCondition \n"
			+ " ,xmis.prj_项目状态 \n"
			+ " WHERE #paymentCondition.对应项目状态 *= xmis.prj_项目状态.项目状态ID \n"

			+ " select #temp2.*   \n"
			+ " ,xmis.prj_项目进度计划.预计开始日期 as 预计开始日期   \n"
			+ " ,dateadd(d,#temp2.延迟天数,xmis.prj_项目进度计划.预计开始日期) as 预计到款日期 \n"
			+ " ,xmis.prj_项目进度计划.实际开始日期 as td  \n"
			+ " into #temp1    \n"
			+ " from #temp2,xmis.prj_项目进度计划    \n"
			+ " where #temp2.项目状态ID = xmis.prj_项目进度计划.项目状态ID \n"
			+ " and #temp2.项目ID = xmis.prj_项目进度计划.项目ID \n"
			+ " order by xmis.prj_项目进度计划.预计开始日期  \n"

			+ " select identity(int,0,1) as row_no,#temp1.* into #temp from #temp1  \n"

			+ " order by 合同ID,预计到款日期,td  \n"

			+ " SELECT 项目ID \n"
			+ " 	  ,project_code \n"
			+ "     ,MB \n"
			+ "      into #incomelist \n"
			+ " FROM xmis.fas_glInfo,xmis.prj_项目 \n"
			+ "  where project_code = 项目编码 \n"

			+ " declare @totalImcome DECIMAL(18,2)  \n"
			+ " declare @allContractCount int \n"
			+ " declare @countInContract int \n"
			+ " declare @i int \n"
			+ " set @i = 0   \n"

			+ " select identity(int,0,1) as row_no,合同ID,count(合同ID) as num,项目ID,min(row_no) as start into #allContract from #temp group by 合同ID,项目ID order by 合同ID \n"
			+ " select @allContractCount = count(*) from #allContract \n"
			+ " while(@i < @allContractCount) begin  \n"
			+ " 	declare @projectID nvarchar(100) \n"
			+ " 	declare @j int \n"
			+ " 	select @countInContract = num + start,@projectID=项目ID,@j=start from #allContract where row_no = @i \n"
			+ " 	select @totalImcome = MB from #incomelist where 项目ID = @projectID \n"
			+ "  	while(@j < @countInContract and @totalImcome > 0) begin  \n"
			+ " 		 declare @total1 DECIMAL(18,2)  \n"
			+ " 		 declare @total2 DECIMAL(18,2)  \n"
			+ " 		 select @total1 = 收款金额 from #temp where row_no = @j  \n"
			+ " 		 if(@total1<=@totalImcome) begin  \n"
			+ " 		 set @total2 = @total1  \n"
			+ " 		 set @totalImcome = @totalImcome - @total1  \n"
			+ " 		 end  \n"
			+ " 		 else begin  \n"
			+ " 		 set @total2 = @totalImcome  \n"
			+ " 		 set @totalImcome = 0  \n"
			+ " 		 end  \n"
			+ " 		 update #temp set realIncome=@total2 where row_no = @j \n"
			+ " 		 set @j = @j + 1  \n"
			+ " 	end  \n"
			+ " 	set @i = @i + 1  \n"
			+ " end  \n"
			+ " select *  \n"
			+ " into #paymentResult \n"
			+ " from #temp where 收款金额>realIncome \n"

			+ " update #paymentResult set 收款金额=收款金额-realIncome,realIncome=0 where realIncome>0 \n"

			+ " SELECT contract.合同ID as 合同ID \n"
			+ " ,contract.简称 as 合同名称 \n"
			+ " ,contract.乙方部门ID as 乙方部门ID \n"
			+ " ,#paymentResult.收款金额 as 金额 \n"
			+ " ,project.项目编码 as 项目编码 \n"
			+ " ,#paymentResult.预计到款日期 as 日期 \n"
			+ " ,Convert(varchar(50),收款条件ID) as 票据号  \n"
			+ " ,company.公司名称 as 公司名称 \n"
			+ " ,company.名称缩写 as 公司简称 \n"
			+ " ,company.公司ID as 公司ID \n"
			+ " ,1 as 来源 \n"
			+ " ,project.项目ID as 项目ID \n"
			+ "  into #result \n"
			+ " from #totalContract as contract \n"
			+ " ,#paymentResult \n"
			+ " ,xmis.org_公司 as company \n"
			+ " ,xmis.prj_项目 as project  \n"
			+ " where contract.合同ID = #paymentResult.合同ID \n"
			+ " AND contract.甲方公司ID *= company.公司ID \n"
			+ " AND #paymentResult.项目ID = project.项目ID \n"
			+ " and #paymentResult.是否已到款 = 0 \n"
			+ "  union all \n"
			+ " --手工设置 \n"
			+ " SELECT contract.合同ID as 合同ID \n"
			+ " ,contract.简称 as 合同名称 \n"
			+ " ,contract.乙方部门ID as 乙方部门ID \n"
			+ " ,estimatedIncome.预计金额 as 金额 \n"
			+ " ,project.项目编码 as 项目编码 \n"
			+ " ,estimatedIncome.预计到款时间 as 日期 \n"
			+ " ,Convert(varchar(50),estimatedIncome.ID) as 票据号 \n"
			+ " ,company.公司名称 as 公司名称 \n"
			+ " ,company.名称缩写 as 公司简称 \n"
			+ " ,company.公司ID as 公司ID \n"
			+ " ,2 as 来源 \n"
			+ " ,project.项目ID \n"
			+ " FROM #totalContract as contract \n"
			+ " ,xmis.org_公司 as company \n"
			+ " ,xmis.con_预计来款明细表 as estimatedIncome \n"
			+ " ,xmis.prj_项目 as project  \n"
			+ " where contract.项目ID = estimatedIncome.项目ID \n"
			+ " and estimatedIncome.是否已到款 = 0 \n"
			+ " AND contract.甲方公司ID = company.公司ID \n"
			+ " AND contract.项目ID = project.项目ID \n";

	// 合同资质管理基本信息（项目来源等）
	public static String getContractInfoByParentID = "\n SELECT "
			+ DBSchema.ContractManagement.ID + ","
			+ DBSchema.ContractManagement.name + from
			+ DBSchema.ContractManagement.TableName + where
			+ DBSchema.ContractManagement.parentID + "=?"
			// + " union all select -1,'- - -' "
			+ " order by " + DBSchema.ContractManagement.ID;

	public static String getContractInfoByType = "\n SELECT "
			+ DBSchema.ContractManagement.ID + ","
			+ DBSchema.ContractManagement.name + from
			+ DBSchema.ContractManagement.TableName + where
			+ DBSchema.ContractManagement.type + "=? and "
			+ DBSchema.ContractManagement.parentID + " is null"
			+ " union all select -1,'- - -' order by "
			+ DBSchema.ContractManagement.ID;

	public static String updateContractInfoObj = update
			+ DBSchema.ContractInfoCon.TableName + " set "
			+ DBSchema.ContractInfoCon.projectSourceParentID + "=?,"
			+ DBSchema.ContractInfoCon.projectSourceID + "=?,"
			+ DBSchema.ContractInfoCon.projectOperationTypeParentID + "=?,"
			+ DBSchema.ContractInfoCon.projectOperationTypeID + "=?,"
			+ DBSchema.ContractInfoCon.projectPropertyID + "=?,"
			+ DBSchema.ContractInfoCon.accountingAgencyID + "=?,"
			+ DBSchema.ContractInfoCon.executiveOrgID + "=? where "
			+ DBSchema.ContractInfoCon.contractID + "=?";

	public static String saveContractInfoObj = insert + " into "
			+ DBSchema.ContractInfoCon.TableName + "("
			+ DBSchema.ContractInfoCon.contractID + ","
			+ DBSchema.ContractInfoCon.projectSourceParentID + ","
			+ DBSchema.ContractInfoCon.projectSourceID + ","
			+ DBSchema.ContractInfoCon.projectOperationTypeParentID + ","
			+ DBSchema.ContractInfoCon.projectOperationTypeID + ","
			+ DBSchema.ContractInfoCon.projectPropertyID + ","
			+ DBSchema.ContractInfoCon.accountingAgencyID + ","
			+ DBSchema.ContractInfoCon.executiveOrgID
			+ ") values (?,?,?,?,?,?,?,?)";

	public static String existConInfo = "\n SELECT " + " count(*) from "
			+ DBSchema.ContractInfoCon.TableName + " where "
			+ DBSchema.ContractInfoCon.contractID + "=?";

	// 项目编码自动生成
	public static String getContractInfoObj = "\n SELECT " + " * " + from
			+ DBSchema.ContractInfoCon.TableName + where
			+ DBSchema.ContractInfoCon.contractID + "=?";

	public static String getProjectCodeTypeByParentID = "\n SELECT " + " * "
			+ from + DBSchema.ContractManagement.TableName + where
			+ DBSchema.ContractManagement.parentID + " = ?";

	public static String getDeptCode = "\n SELECT "
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + ","
			+ DBSchema.Department.DepartmentName + ","
			+ DBSchema.Department.code + from + DBSchema.Department.TableName
			+ " left join " + DBSchema.DepartmentBelongToCompany.TabelName
			+ " on " + DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + " = "
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DepartmentID + " left join "
			+ "(" + select + DBSchema.DeptBelongToDept.ParentsID + from
			+ DBSchema.DeptBelongToDept.TableName + " group by "
			+ DBSchema.DeptBelongToDept.ParentsID + ") a " + " on a."
			+ DBSchema.DeptBelongToDept.ParentsID + " = "
			+ DBSchema.Department.TableName + "."
			+ DBSchema.Department.DepartmentID + where
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.DirectlyFlag + " = 1" + and
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.CompanyID + " = 1 " + and
			+ DBSchema.Department.code + " is not null order by "
			+ DBSchema.DepartmentBelongToCompany.TabelName + "."
			+ DBSchema.DepartmentBelongToCompany.Sort;

	public static String getProjectNumber = "\n SELECT "
			+ DBSchema.Project.ProjectCode + from + DBSchema.Project.TableName
			+ " join " + DBSchema.Contract.TableName + " on "
			+ DBSchema.Project.TableName + "." + DBSchema.Project.ProjectID
			+ " = " + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.ProjectID + where
			+ DBSchema.Project.ProjectCode + " is not null " + and
			+ DBSchema.Project.ProjectCode + like + "?" + and
			+ DBSchema.Contract.ContractTypeID + " = 1";
	public static String existProjectNumber = "\n SELECT " + " count(*) "
			+ from + DBSchema.Project.TableName + " join "
			+ DBSchema.Contract.TableName + " on " + DBSchema.Project.TableName
			+ "." + DBSchema.Project.ProjectID + " = "
			+ DBSchema.Contract.TableName + "." + DBSchema.Contract.ProjectID
			+ where + DBSchema.Project.ProjectCode + " = ?" + and
			+ DBSchema.Contract.ContractTypeID + " = 1";

	public static String getConInfoByProjectCode = "\n SELECT "
			+ DBSchema.Project.TableName + "." + DBSchema.Project.ProjectCode
			+ "," + DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.ContractName + ","
			+ DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.LastModificationDate + "," + "RTRIM" + "("
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.FirstName
			+ ")" + "+" + "RTRIM" + "(" + DBSchema.Employee.TableName + "."
			+ DBSchema.Employee.LastName + ")" + " userName ,"
			+ DBSchema.Contract.DeptIDOfVendor + ","
			+ DBSchema.Department.DepartmentName + ","
			+ DBSchema.Contract.ContractAmt + from
			+ DBSchema.Contract.TableName + " left join "
			+ DBSchema.Project.TableName + " on " + DBSchema.Contract.TableName
			+ "." + DBSchema.Contract.ProjectID + " = "
			+ DBSchema.Project.TableName + "." + DBSchema.Project.ProjectID
			+ " left join " + DBSchema.Employee.TableName + " on "
			+ DBSchema.Contract.TableName + "."
			+ DBSchema.Contract.LastModificatorID + " = "
			+ DBSchema.Employee.TableName + "." + DBSchema.Employee.ID
			+ " left join " + DBSchema.Department.TableName + " on "
			+ DBSchema.Contract.DeptIDOfVendor + " = "
			+ DBSchema.Department.DepartmentID + where
			+ DBSchema.Project.TableName + "." + DBSchema.Project.ProjectCode
			+ "=?";

	// 合同金额分解
	public static String insertConQualifyManage = insert + into
			+ DBSchema.ContractManagement.TableName + "("
			+ DBSchema.ContractManagement.name + ","
			+ DBSchema.ContractManagement.ID + ","
			+ DBSchema.ContractManagement.type + ")" + values + " ('',?,?)";

	public static String insertConTextSplit = insert + into
			+ DBSchema.conMoneySplit.TableName + "("
			+ DBSchema.conMoneySplit.ID + ","
			+ DBSchema.conMoneySplit.contractID + ","
			+ DBSchema.conMoneySplit.splitTypeID + ","
			+ DBSchema.conMoneySplit.LastUpdateDate + ","
			+ DBSchema.conMoneySplit.LastUpdateEmpId + ")" + values
			+ "(?,?,?,GETDATE(),?)";

	public static String updateConQualifyManage = update
			+ DBSchema.ContractManagement.TableName + set
			+ DBSchema.ContractManagement.name + " = ?,"
			+ DBSchema.ContractManagement.description + " = ?" + where
			+ DBSchema.ContractManagement.ID + " = ?" + " and "
			+ DBSchema.ContractManagement.type + " = ?";

	public static String updateConMoneySplit = update
			+ DBSchema.conMoneySplit.TableName + set
			+ DBSchema.conMoneySplit.money + " = ?,"
			+ DBSchema.conMoneySplit.percent + " = ?,"
			+ DBSchema.conMoneySplit.LastUpdateDate + "=GETDATE(),"
			+ DBSchema.conMoneySplit.LastUpdateEmpId + " = ?" + where
			+ DBSchema.conMoneySplit.contractID + " = ?" + and
			+ DBSchema.conMoneySplit.splitTypeID + " = ?";

	public static String deleteConMoneySplit = delete
			+ DBSchema.conMoneySplit.TableName + where
			+ DBSchema.conMoneySplit.contractID + " = ?" + and
			+ DBSchema.conMoneySplit.splitTypeID + " = ?";

	public static String deleteQualifyManage = delete
			+ DBSchema.conMoneySplit.TableName + where
			+ DBSchema.conMoneySplit.ID + " = ?";
	public static String getConTextSplitList = "\n SELECT "
			+ DBSchema.ContractManagement.TableName + "."
			+ DBSchema.ContractManagement.ID + ","
			+ DBSchema.ContractManagement.TableName + "."
			+ DBSchema.ContractManagement.name + ","
			+ DBSchema.ContractManagement.TableName + "."
			+ DBSchema.ContractManagement.description + ","
			+ DBSchema.conMoneySplit.TableName + "."
			+ DBSchema.conMoneySplit.money + ","
			+ DBSchema.conMoneySplit.TableName + "."
			+ DBSchema.conMoneySplit.percent + from
			+ DBSchema.conMoneySplit.TableName + ","
			+ DBSchema.ContractManagement.TableName + where
			+ DBSchema.conMoneySplit.TableName + "."
			+ DBSchema.conMoneySplit.splitTypeID + "="
			+ DBSchema.ContractManagement.TableName + "."
			+ DBSchema.ContractManagement.ID + and
			+ DBSchema.conMoneySplit.TableName + "."
			+ DBSchema.conMoneySplit.contractID + " = ? " + and
			+ DBSchema.ContractManagement.TableName + "."
			+ DBSchema.ContractManagement.type + " = ?";

	public static String existMoneySplit = "\n SELECT " + "count(*)" + from
			+ DBSchema.conMoneySplit.TableName + where
			+ DBSchema.conMoneySplit.contractID + " = ? " + and
			+ DBSchema.conMoneySplit.splitTypeID + " = ?";

	public static String getContractStateList = "SET NOCOUNT ON \n"
			+ " DECLARE @i DECIMAL(18, 10) \n"
			+ " SET @i = ? \n"
			+ " SELECT  合同ID , \n"
			+ " 合同名称 , \n"
			+ " xmis.prj_项目.项目编码, \n"
			+ " '新建--收入合同' AS text , \n"
			+ " 项目经理ID AS manager_id , \n"
			+ " 乙方部门ID , \n"
			+ " xmis.org_部门.部门名称 , \n"
			+ " xmis.con_合同.最后修改日期 , \n"
			+ " xmis.con_合同.最后修改人ID , \n"
			+ " RTRIM(xmis.hr_人员.姓氏) + RTRIM(xmis.hr_人员.名字) AS lastName \n"
			+ " INTO    #result \n"
			+ " FROM    xmis.con_合同 \n"
			+ " LEFT JOIN xmis.org_部门 ON xmis.con_合同.乙方部门ID = xmis.org_部门.部门ID \n"
			+ " LEFT JOIN xmis.prj_项目 ON xmis.con_合同.项目ID = xmis.prj_项目.项目ID \n"
			+ " left JOIN xmis.hr_人员 ON xmis.con_合同.最后修改人ID = xmis.hr_人员.人员ID \n"
			+ " WHERE   新建标志 = 1 \n"
			+ " AND 合同类型ID = 1 \n"
			+ " AND 父合同ID IS NULL \n"
			+ " UNION ALL \n"
			+ " SELECT  合同ID , \n"
			+ " 合同名称 , \n"
			+ " xmis.con_合同.合同编号, \n"
			+ " '新建--支出合同' AS text , \n"
			+ " 项目经理ID AS manager_id , \n"
			+ " 甲方部门ID AS 乙方部门ID , \n"
			+ " xmis.org_部门.部门名称 , \n"
			+ " xmis.con_合同.最后修改日期 , \n"
			+ " xmis.con_合同.最后修改人ID , \n"
			+ " RTRIM(xmis.hr_人员.姓氏) + RTRIM(xmis.hr_人员.名字) AS lastName \n"
			+ " FROM    xmis.con_合同 \n"
			+ " LEFT JOIN xmis.org_部门 ON xmis.con_合同.甲方部门ID = xmis.org_部门.部门ID \n"
			+ " LEFT JOIN xmis.prj_项目 ON xmis.con_合同.项目ID = xmis.prj_项目.项目ID \n"
			+ " left JOIN xmis.hr_人员 ON xmis.con_合同.最后修改人ID = xmis.hr_人员.人员ID \n"
			+ " WHERE   新建标志 = 1 \n" + " AND 合同类型ID = 2 \n"
			+ " AND 父合同ID IS NULL \n" + " UNION ALL \n"
			+ " SELECT  a.合同ID , \n" + " 合同名称 , \n" + " xmis.prj_项目.项目编码, \n"
			+ " '80万未分解' AS text , \n" + " 项目经理ID AS manager_id , \n"
			+ " a.乙方部门ID , \n" + " xmis.org_部门.部门名称 , \n" + " a.最后修改日期 , \n"
			+ " a.最后修改人ID , \n"
			+ " RTRIM(xmis.hr_人员.姓氏) + RTRIM(xmis.hr_人员.名字) AS lastName \n"
			+ " FROM    xmis.con_合同 a \n"
			+ " LEFT JOIN xmis.org_部门 ON a.乙方部门ID = xmis.org_部门.部门ID \n"
			+ " LEFT JOIN xmis.prj_项目 ON a.项目ID = xmis.prj_项目.项目ID \n"
			+ " left JOIN xmis.hr_人员 ON a.最后修改人ID = xmis.hr_人员.人员ID \n"
			+ " WHERE   a.合同总额 > 800000.00 \n" + " AND 合同ID NOT IN ( \n"
			/*
			 * + " SELECT  合同ID \n" + " FROM    xmis.con_合同金额分解 con \n" +
			 * " LEFT JOIN xmis.sys_合同资质管理字典表 sys ON con.分解类型ID = sys.ID \n" +
			 * " WHERE   sys.type = 1 \n" + " GROUP BY 合同ID ) \n"
			 */
			+ " SELECT  合同ID  FROM    xmis.con_合同金额分解 con \n"
			+ " JOIN xmis.sys_合同资质管理字典表 sys ON con.分解类型ID = sys.ID \n"
			+ " WHERE   sys.type = 1  GROUP BY 合同ID \n"
			+ " HAVING  SUM(金额) > 0 )\n"

			+ " AND 合同类型ID = 1 \n" + " AND 父合同ID IS NULL \n"
			+ " AND xmis.f_getYearFromPrjCode(项目编码) > 2010 \n"
			+ " ORDER BY 乙方部门ID , \n" + " xmis.con_合同.最后修改日期 DESC ";

	public static String isEmpBelongDept = "select COUNT(*) from ("
			+ " SELECT a.人员ID 'empId', case when e.是否直属 = 1 then  b.部门ID else f.父部门ID end '事业部级部门ID'"
			+ " from xmis.hr_人员部门关系表 b "
			+ "  join xmis.hr_人员 a on a.人员ID = b.人员ID "
			+ " left join xmis.org_部门归属公司表 e  on  b.部门ID =  e.部门ID"
			+ "  left join xmis.org_部门归属部门表 f  on  b.部门ID =  f.部门ID"
			+ "  WHERE  a.人员ID >0  and  e.公司ID=1 and  (  a.禁用 = 0  or  a.禁用 is NULL  )  "
			+ "  ) as new where 事业部级部门ID = ? and empId = ?";

	public static String getPaymentPromptList = "SET NOCOUNT ON \n  declare @i DECIMAL(18,10) set @i=? SELECT payed.合同ID,sum(收款金额) as 已收金额  "
			+ " INTO #payedAmt "
			+ " FROM xmis.con_收款明细表 as payed  "
			+ " group by 合同ID "
			+ " select xmis.con_合同.合同ID \n"
			+ " ,case when xmis.con_合同.简称 is null or xmis.con_合同.简称 = '' then xmis.con_合同.合同名称 else xmis.con_合同.简称 end as 合同名称 \n"
			+ " ,xmis.con_合同.合同负责人 \n"
			+ " ,xmis.con_合同.乙方部门ID \n"
			+ " ,xmis.fas_glInfo.MB as 财务来款  \n"
			+ " ,xmis.con_合同.最后修改日期 as time \n"
			+ " into #payedFas \n"
			+ " from xmis.prj_项目,xmis.fas_glInfo,xmis.con_合同 \n"
			+ " where xmis.prj_项目.项目编码 = xmis.fas_glInfo.project_code \n"
			+ " and xmis.con_合同.项目ID = xmis.prj_项目.项目ID \n"
			+ " and xmis.fas_glInfo.MB>0 \n"
			+ " and xmis.con_合同.合同类型ID = 1   \n"
			+ " and xmis.con_合同.父合同ID is null --合同的财务来款 \n"
			+ " "
			+ " select #payedFas.合同ID"
			+ " ,#payedFas.合同名称"
			+ " ,#payedFas.合同负责人"
			+ " ,#payedFas.乙方部门ID"
			+ " ,case when #payedAmt.已收金额 IS null then 0.00 else #payedAmt.已收金额 end as 已收金额"
			+ " ,#payedFas.财务来款"
			+ " ,case when #payedAmt.已收金额 IS null then"
			+ " #payedFas.财务来款 else #payedFas.财务来款 - #payedAmt.已收金额 end as 未登记金额"
			+ " ,'有金额未登记' as text"
			+ " ,#payedFas.time"
			+ " from #payedFas"
			+ " left join #payedAmt on #payedFas.合同ID = #payedAmt.合同ID"
			+ " where #payedFas.财务来款 > #payedAmt.已收金额 or #payedAmt.已收金额 is null"
			+ " "
			+ " union all "
			+ " select #payedFas.合同ID"
			+ " ,#payedFas.合同名称"
			+ " ,#payedFas.合同负责人"
			+ " ,#payedFas.乙方部门ID"
			+ " ,case when #payedAmt.已收金额 IS null then 0.00 else #payedAmt.已收金额 end as 已收金额"
			+ " ,#payedFas.财务来款"
			+ " ,case when #payedFas.财务来款 is null then "
			+ " #payedAmt.已收金额 else #payedAmt.已收金额-#payedFas.财务来款 end as 登记金额超过财务来款"
			+ " ,'登记金额超过财务来款' as text"
			+ " ,#payedFas.time"
			+ " from #payedFas"
			+ " left join #payedAmt on #payedFas.合同ID = #payedAmt.合同ID"
			+ " where #payedAmt.已收金额>#payedFas.财务来款 or #payedFas.财务来款 is null"
			+ " order by time desc";

	public static String getScientificName = " select ID,name,xmis.org_部门.部门ID"
			+ " from xmis.sys_合同资质管理字典表 join xmis.org_部门 on xmis.sys_合同资质管理字典表.ID = xmis.org_部门.科研所Id "
			+ " where xmis.org_部门.部门ID = ?";

	public static String getContractCodeNumber = " select 合同编号 from xmis.con_合同 where 合同编号 like ? and 合同类型ID = 2 AND 父合同ID IS NOT NULL ";

	public static String existContractCode = "  select count(*) "
			+ "from xmis.con_合同 where 合同编号 = ? and  父合同ID is null";

	public static String getContractInfoByContractCode = "select 合同编号,合同名称,xmis.con_合同.最后修改日期, \n"
			+ " RTRIM(xmis.hr_人员.姓氏)+RTRIM(xmis.hr_人员.名字) userName \n"
			+ " from xmis.con_合同 left join xmis.hr_人员 on xmis.con_合同.最后修改人ID = xmis.hr_人员.人员ID \n"
			+ " where xmis.con_合同.合同编号 = ? and 父合同ID is null \n";

	public static String getDeptInfoByProjectCode = "\n select 合同ID,合同名称,乙方部门ID,部门名称,合同总额 from xmis.con_合同  \n"
			+ "left join xmis.prj_项目 on xmis.con_合同.项目ID =  xmis.prj_项目.项目ID  \n"
			+ "left join xmis.org_部门 on xmis.con_合同.乙方部门ID = xmis.org_部门.部门ID  \n"
			+ "where 项目编码 = ? and 合同类型ID = 1  \n";

	public static String getSumAmtByConID = "select 合同总额  as 'amt' from xmis.con_合同 where 父合同ID = ?";

	public static String getAmtByConId = "select 合同总额   as 'amt' from xmis.con_合同   where 合同ID = ?";

	public static String updateContractNewflag = "update xmis.con_合同   set 新建标志= ? where 合同ID = ?";

	public static String contractAmtSplitList = " select  con.合同ID,合同名称,null as 项目领域,合同总额,con.合同来源类型ID,con.乙方部门ID, \n"
			+ " amt.分解类型ID,amt.金额, \n"
			+ " 签订日期,有效期起,null as 项目验收时间 , \n"
			+ " sys.项目合作形式父ID \n"
			+ " ,prj.项目编码 \n"
			+ " ,CASE WHEN con.合同ID not in ( \n"
			+ " select 合同ID \n"
			+ " from xmis.con_合同金额分解 con left join xmis.sys_合同资质管理字典表 sys \n"
			+ " on con.分解类型ID = sys.ID \n"
			+ " where sys.type =1  group by 合同ID ) THEN 1 ELSE 2 END AS splitflag \n"
			+ " into #temp \n"
			+ " from xmis.con_合同 con  left join xmis.con_合同金额分解  amt on con.合同ID = amt.合同ID  \n"
			+ " left  join xmis.con_合同资质信息表 sys on con.合同ID = sys.合同ID  \n"
			+ " left join xmis.prj_项目 prj on con.项目ID = prj.项目ID  \n"
			+ " where con.父合同ID is null and con.合同总额>=800000 and con.合同类型ID = 1 and 乙方公司ID = 1 \n"
			+ " and (amt.分解类型ID in (101,102,103,1,201,202,2,301,302,303,304,3,4,401,402,403,404) or amt.分解类型ID is null) \n\n"

			+ " select #temp.合同ID \n"
			+ " ,#temp.合同名称 \n"
			+ " ,#temp.项目领域 \n"
			+ " ,#temp.合同总额 \n"
			+ " ,#temp.合同来源类型ID \n"
			+ " ,#temp.乙方部门ID \n"
			+ " ,#temp.签订日期 \n"
			+ " ,#temp.有效期起 \n"
			+ " ,#temp.项目验收时间 \n"
			+ " ,#temp.项目合作形式父ID \n"
			+ " ,#temp.项目编码 \n"
			+ " ,#temp.splitflag \n"
			+ " ,convert(decimal(18,2),0) as 'a1' \n"
			+ " ,convert(decimal(18,2),0) as 'a2' \n"
			+ " ,convert(decimal(18,2),0) as 'a3' \n"
			+ " ,convert(decimal(18,2),0) as 'a4' \n"
			+ " ,convert(decimal(18,2),0) as 'a101' \n"
			+ " ,convert(decimal(18,2),0) as 'a102' \n"
			+ " ,convert(decimal(18,2),0) as 'a103' \n"
			+ " ,convert(decimal(18,2),0) as 'a201' \n"
			+ " ,convert(decimal(18,2),0) as 'a202' \n"
			+ " ,convert(decimal(18,2),0) as 'a301' \n"
			+ " ,convert(decimal(18,2),0) as 'a302' \n"
			+ " ,convert(decimal(18,2),0) as 'a303' \n"
			+ " ,convert(decimal(18,2),0) as 'a304' \n"
			+ " ,convert(decimal(18,2),0) as 'a401' \n"
			+ " ,convert(decimal(18,2),0) as 'a402' \n"
			+ " ,convert(decimal(18,2),0) as 'a403' \n"
			+ " ,convert(decimal(18,2),0) as 'a404' \n"
			+ " into #t \n"
			+ " from #temp \n"
			+ " where 1=2  \n"

			+ " insert into #t select 合同ID \n\n"
			+ " ,合同名称  \n"
			+ " ,项目领域  \n"
			+ " ,合同总额 \n"
			+ " ,合同来源类型ID \n"
			+ " ,乙方部门ID \n"
			+ " ,签订日期 \n"
			+ " ,有效期起 \n"
			+ " ,项目验收时间 \n"
			+ " ,项目合作形式父ID \n"
			+ " ,项目编码 \n"
			+ " ,splitflag \n"
			+ " ,case when 分解类型ID = 1 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 2 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 3 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 4 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 101 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 102 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 103 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 201 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 202 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 301 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 302 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 303 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 304 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 401 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 402 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 403 then 金额 else 0 end \n"
			+ " ,case when 分解类型ID = 404 then 金额 else 0 end \n"
			+ " from #temp \n"

			+ " select 合同ID \n"
			+ " ,合同名称 \n"
			+ " ,项目领域 \n"
			+ " ,合同总额 \n"
			+ " ,合同来源类型ID \n"
			+ " ,乙方部门ID \n"
			+ " ,签订日期 \n"
			+ " ,有效期起 \n"
			+ " ,项目验收时间 \n"
			+ " ,项目合作形式父ID \n"
			+ " ,项目编码 \n"
			+ " ,splitflag \n"
			+ " ,SUM(a1) as '硬件购置费' \n"
			+ " ,SUM(a2) as '软件购置费' \n"
			+ " ,SUM(a3) as '软件和信息技术服务费' \n"
			+ " ,SUM(a4) as '其他' \n"
			+ " ,SUM(a101) as '自制硬件费' \n"
			+ " ,SUM(a102) as '国产硬件费' \n"
			+ " ,SUM(a103) as '进口硬件费' \n"
			+ " ,SUM(a201) as '国产软件费' \n"
			+ " ,SUM(a202) as '进口软件费' \n"
			+ " ,SUM(a301) as '软件开发费用' \n"
			+ " ,SUM(a302) as '咨询设计费用' \n"
			+ " ,SUM(a303) as '系统集成费用' \n"
			+ " ,SUM(a304) as '运营维护费用' \n"
			+ " ,SUM(a401) as '建筑工程费' \n"
			+ " ,SUM(a402) as '管理费' \n"
			+ " ,SUM(a403) as '运费' \n"
			+ " ,SUM(a404) as '税金' \n"
			+ " ,SUM(a3)/合同总额 as '软件和信息技术服务费用比例' \n"
			+ " into #result \n"
			+ " from #t \n"
			+ " group by 合同ID \n"
			+ " ,合同名称 \n"
			+ " ,项目领域 \n"
			+ " ,乙方部门ID \n"
			+ " ,合同来源类型ID \n"
			+ " ,合同总额 \n"
			+ " ,签订日期 \n"
			+ " ,有效期起 \n"
			+ " ,项目验收时间 \n"
			+ " ,项目合作形式父ID \n" + " ,项目编码 \n" + " ,splitflag \n";

	public static String getContractAmtSplitListPageInfo = "\n\nSELECT * from #resultA "
			+ "WHERE row_no >= ? "
			+ " AND row_no < ?  UNION ALL "
			+ "\nSELECT MAX(row_no) as row_no,"
			+ "null as 合同ID,null as 合同名称,null as 项目领域,"
			+ "sum(合同总额) as 合同总额,null as 合同来源类型ID,null as 乙方部门ID,"
			+ "null as 签订日期,null as 有效期起,null as 项目验收时间,10000 as 项目合作形式父ID,null as 项目编码, null as splitflag,"
			+ "sum(硬件购置费) as 硬件购置费,"
			+ "sum(软件购置费) as 软件购置费,"
			+ "sum(软件和信息技术服务费) as 软件和信息技术服务费,"
			+ "sum(其他) as 其他,"
			+ "sum(自制硬件费) as 自制硬件费,sum(国产硬件费) as 国产硬件费,sum(进口硬件费) as 进口硬件费,"
			+ "sum(国产软件费) as 国产软件费,sum(进口软件费) as 进口软件费,"
			+ "sum(软件开发费用) as 软件开发费用,sum(咨询设计费用) as 咨询设计费用,"
			+ "sum(系统集成费用) as 系统集成费用,sum(运营维护费用) as 运营维护费用,"
			+ "sum(建筑工程费) as 建筑工程费,sum(管理费) as 管理费,sum(运费) as 运费,sum(税金) as 税金,"
			+ "2 as 软件和信息技术服务费用比例  from #resultA ";

	public static String getOutContractDelimiter = "select name as delimiter from xmis.sys_合同资质管理字典表 where ID = -1";

	public static String getIncomeContractPrefix = "SELECT name AS prefix FROM xmis.sys_合同资质管理字典表 WHERE ID = -2";

	public static String doSavePayoutContractInfo = update
			+ DBSchema.Contract.TableName + set
			+ DBSchema.Contract.SourceTypeID
			+ "=?," // 1
			+ DBSchema.Contract.GovermentContractSourceID
			+ "=?," // 2
			+ DBSchema.Contract.ContractName
			+ "=?," // 3
			+ DBSchema.Contract.Abbreviation
			+ "=?," // 4
			+ DBSchema.Contract.ContractSummary
			+ "=?," // 5
			+ DBSchema.Contract.WarrantyDefinition
			+ "=?," // 6
			+ DBSchema.Contract.WarrantyStartDate
			+ "=?," // 7
			+ DBSchema.Contract.WarrantyEndDate
			+ "=?," // 8
			+ DBSchema.Contract.CurrencyID
			+ "=?," // 9
			+ DBSchema.Contract.SignatureDate
			+ "=?," // 10
			+ DBSchema.Contract.BuyerID
			+ "=?," // 11
			+ DBSchema.Contract.DeptIDOfBuyer
			+ "=?," // 12
			+ DBSchema.Contract.BuyerRepresentativeID
			+ "=?," // 13
			+ DBSchema.Contract.VendorID
			+ "=?," // 14
			+ DBSchema.Contract.DeptIDOfVendor
			+ "=?," // 15
			+ DBSchema.Contract.VendorRepresentativeID
			+ "=?," // 16
			+ DBSchema.Contract.Remark
			+ "=?," // 17
			+ DBSchema.Contract.DeliveryID
			+ "=?," // 18
			+ DBSchema.Contract.LastModificationDate + "=GETDATE(),"
			+ DBSchema.Contract.LastModificatorID + "=?," // 19
			+ DBSchema.Contract.WarrantyLimit + "=?," // 20
			+ DBSchema.Contract.WarrantyUnit + "=?," // 21
			+ DBSchema.Contract.ProjectID + "=?," // 22
			+ DBSchema.Contract.DirectorID + "=?," // 23
			+ DBSchema.Contract.ValidityStartDate + "=?,"// 24
			+ DBSchema.Contract.ValidityEndDate + "=?,"// 25
			+ DBSchema.Contract.ValidityLimit + "=?,"// 26
			+ DBSchema.Contract.ValidityUnit + "=?,"// 27
			+ DBSchema.Contract.newflag + "=?"// 28
			+ where + DBSchema.Contract.ParentID + "=?" // 29
	;

	public static String getAttachmentListForKZHB = "SELECT xmis.sys_附件.附件ID , \n"
			+ "xmis.sys_附件.文件名 , \n"
			+ "xmis.sys_附件主体类型.附件主体描述 , \n"
			+ "CONVERT(VARCHAR(10), xmis.sys_附件.上传时间, 23) AS upload_date , \n"
			+ "xmis.sys_附件.上传人 AS uploader_id, \n"
			+ "(RTRIM(xmis.hr_人员.姓氏)+RTRIM(xmis.hr_人员.名字))  AS uploader_name, \n"
			+ "xmis.sys_附件.内容描述, \n"
			+ "xmis.sys_附件.关键字 \n"
			+ " into #result \n"
			+ "FROM   xmis.sys_附件 , \n"
			+ "xmis.sys_附件主体类型 , \n"
			+ "xmis.hr_人员 \n"
			+ "WHERE  xmis.sys_附件.REFID = -1 \n"
			+ "AND xmis.sys_附件.附件主体类型ID = -1 \n"
			+ "AND xmis.sys_附件.上传人=xmis.hr_人员.人员ID \n"
			+ "AND xmis.sys_附件.附件主体类型ID=xmis.sys_附件主体类型.附件主体类型ID \n"
			+ "AND xmis.sys_附件.删除标志 = 1";

	public static String DeleteKzhbFileById = "UPDATE xmis.sys_附件 SET 删除标志 = 2 WHERE 附件ID = ?";

	public static String existValidateForKzhb = select + " count(*) "
			+ " from " + DBSchema.Attachment.TableName + " where "
			+ DBSchema.Attachment.AttachmentTypeID + " = ?" + " and "
			+ DBSchema.Attachment.REFID + " = ?" + " and "
			+ DBSchema.Attachment.AttachmentName + " = " + "?"
			+ " and 删除标志 ==1";

	public static String isEmpDeptSpecial = "SELECT COUNT(*) FROM xmis.con_人员部门特殊情况对应表 WHERE 人员ID = ? and 部门ID = ? AND 合同类型ID = 2";

	public static String getCustomGroupTree = "SELECT groupid,father_groupid,type,groupname FROM xmis.org_通讯录自定义组关系表   WHERE father_groupid = ?";

	public static String deleteEmpOrDeptOrGroupOfCustomGroup = "DELETE xmis.org_通讯录自定义组关系表 WHERE groupid = ? AND father_groupid = ? AND TYPE = ?";

	public static String groupBeUsedNum = "SELECT COUNT(*) FROM xmis.org_通讯录自定义组关系表 WHERE groupid = ?";

	public static String updateCustomGroupName = "update xmis.org_通讯录自定义组关系表 SET  groupname = ? WHERE groupid = ?";

	public static String addCustomGroup = "INSERT INTO xmis.org_通讯录自定义组关系表 VALUES (?,?,?,?)";

	public static String updateGroupRelation = "UPDATE xmis.org_通讯录自定义组关系表 SET father_groupid = ? WHERE groupid = ?";

	public static String getEmpInfo = "  SELECT a.人员ID empId , \n"
			+ " CASE WHEN e.是否直属 = 1 THEN b.部门ID ELSE f.父部门ID  \n"
			+ " END deptId , \n"
			+ " CASE WHEN e.是否直属 = 1 THEN d.部门名称 ELSE g.部门名称  \n"
			+ " END deptName , \n"
			+ " b.部门ID tdeptId ,d.部门名称 tdeptName, \n"
			+ " RTRIM(姓氏)+RTRIM(名字)  userName , 姓氏 firstName , \n"
			+ " 名字 lastName , c.职务名称 position , 移动电话 mobileNo , \n"
			+ " 单位电话 switchboardNo , 单位分机 extNo , 传真号码 faxNo , \n"
			+ " 电子邮件 emailAddress , 详细地址 officeAddress , \n"
			+ " e.公司ID compId , e.公司名称 compName \n"
			+ " FROM   xmis.hr_人员部门关系表 b \n"
			+ " JOIN xmis.hr_人员 a ON a.人员ID = b.人员ID \n"
			+ " LEFT JOIN xmis.sys_职务 c ON b.职务 = c.职务ID \n"
			+ " LEFT JOIN xmis.org_部门 d ON b.部门ID = d.部门ID \n"
			+ " LEFT JOIN ( SELECT  dcomp.部门ID , dcomp.公司ID , \n"
			+ " comp.公司名称 , dcomp.是否直属 ,dcomp.排序   FROM    xmis.org_部门归属公司表 dcomp \n"
			+ " JOIN xmis.org_公司 comp ON dcomp.公司ID = comp.公司ID \n"
			+ " ) e ON b.部门ID = e.部门ID \n"
			+ " LEFT JOIN xmis.org_部门归属部门表 f ON b.部门ID = f.部门ID \n"
			+ " LEFT JOIN xmis.org_部门 g ON g.部门ID = f.父部门ID \n"
			+ " LEFT JOIN xmis.add_地址 ad ON a.办公地址ID = ad.地址ID \n"
			+ " WHERE  a.人员ID = ?\n";

	public static String getDeptListByFatherDeptID = "SELECT  部门ID from xmis.org_部门归属部门表 WHERE 父部门ID = ?";

	public static String getCompInfoByEmpName = "SELECT  DISTINCT "
			+ " dc.公司ID , comp.公司名称 ,comp.名称缩写 " + " INTO #result "
			+ " FROM  xmis.org_部门归属公司表 dc ,  xmis.org_公司 comp "
			+ " WHERE 部门ID IN ( SELECT 部门ID "
			+ " FROM xmis.hr_人员 h ,  xmis.hr_人员部门关系表 hd "
			+ " WHERE  h.人员ID = hd.人员ID  AND ( h.禁用 <> 1 "
			+ " OR h.禁用 IS NULL )  AND (RTRIM(h.姓氏)+RTRIM(h.名字))  like ? ) "
			+ " AND dc.公司ID = comp.公司ID ";

	public static String getCustomGroupList = "SELECT  groupid,father_groupid,type,groupname FROM xmis.org_通讯录自定义组关系表 WHERE father_groupid =?";

	public static String getCustomGroupListByGroupid = "SELECT  groupid,father_groupid,type,groupname FROM xmis.org_通讯录自定义组关系表 WHERE groupid = ?";

	public static String isExistCustomGroup = "SELECT count(*) FROM xmis.org_通讯录自定义组关系表 WHERE groupid = ? AND father_groupid = ? AND type = ?";

	public static String isEmpBelongDeptByEmpName = "SELECT  COUNT(*) \n"
			+ " FROM    ( SELECT    a.人员ID 'empId' , \n"
			+ " (RTRIM(a.姓氏)+RTRIM(a.名字))  AS empName , \n"
			+ " CASE WHEN e.是否直属 = 1 THEN b.部门ID ELSE f.父部门ID \n"
			+ " END 'deptId' FROM      xmis.hr_人员部门关系表 b \n"
			+ " JOIN xmis.hr_人员 a ON a.人员ID = b.人员ID \n"
			+ " LEFT JOIN xmis.org_部门归属公司表 e ON b.部门ID = e.部门ID \n"
			+ " LEFT JOIN xmis.org_部门归属部门表 f ON b.部门ID = f.部门ID \n"
			+ " WHERE     a.人员ID > 0  AND ( a.禁用 = 0 \n"
			+ " OR a.禁用 IS NULL ) UNION ALL \n"
			+ " SELECT    a.人员ID 'empId' , \n"
			+ " (RTRIM(a.姓氏)+RTRIM(a.名字))  AS empName , b.部门ID deptId \n"
			+ " FROM      xmis.hr_人员部门关系表 b \n"
			+ " JOIN xmis.hr_人员 a ON a.人员ID = b.人员ID \n"
			+ " WHERE     a.人员ID > 0  AND ( a.禁用 = 0 \n"
			+ " OR a.禁用 IS NULL  ) ) AS new \n"
			+ " WHERE   deptId = ? AND empName = ?";

	public static String getCompIDByEmpName = "SELECT  DISTINCT "
			+ " dc.公司ID , comp.公司名称 ,comp.名称缩写 "
			+ " FROM  xmis.org_部门归属公司表 dc ,  xmis.org_公司 comp "
			+ " WHERE 部门ID IN ( SELECT 部门ID "
			+ " FROM xmis.hr_人员 h ,  xmis.hr_人员部门关系表 hd "
			+ " WHERE  h.人员ID = hd.人员ID  AND ( h.禁用 <> 1 "
			+ " OR h.禁用 IS NULL )  AND  (RTRIM(h.姓氏)+RTRIM(h.名字)) = ? ) "
			+ " AND dc.公司ID = comp.公司ID ";

	public static String getDeptIDByUserID = "SELECT 部门ID FROM xmis.hr_人员部门关系表  WHERE 人员ID=?";

	public static String getDivisionDeptIDByDeptID = "SELECT 部门ID FROM xmis.org_部门归属公司表 WHERE 是否直属=1 AND 部门ID=?  UNION  SELECT  父部门ID FROM xmis.org_部门归属部门表 where 部门ID = ?";

	public static String getFdeptIDIsExistByDeptID = "select count(父部门ID) FROM org_部门归属部门表   WHERE 部门ID = ? ";
	public static String getFdeptIDByDeptID = "select 父部门ID FROM org_部门归属部门表   WHERE 部门ID = ? ";

	public static String getEmpListByName = "  select H.人员ID  AS EMPID from xmis.hr_人员   h,  xmis.hr_人员部门关系表   hd,  xmis.org_部门归属公司表  dc,  XMIS.ORG_公司 c  where  h.人员ID = hd.人员ID and  hd.部门ID = dc.部门ID and  dc.公司ID = c.公司ID "
			+ " and ( h.禁用<>1 or  h.禁用 is null)"
			+ " and  c.公司ID=? and   (RTRIM(h.姓氏)+RTRIM(h.名字)) = ?";

	public static String getEmpListByNameAdmin = "  select H.人员ID  AS EMPID from xmis.hr_人员   h,  xmis.hr_人员部门关系表   hd,  xmis.org_部门归属公司表  dc,  XMIS.ORG_公司 c  where  h.人员ID = hd.人员ID and  hd.部门ID = dc.部门ID and  dc.公司ID = c.公司ID "
			+ " and  c.公司ID=? and   (RTRIM(h.姓氏)+RTRIM(h.名字)) = ?";

	public static String getEmpPathId = "DECLARE @deptid INT "
			+ " DECLARE @divsionid INT "
			+ " DECLARE @compid INT "
			+ " SELECT @deptid=部门ID FROM xmis.hr_人员部门关系表 WHERE 人员ID=?"
			+ " SELECT @divsionid=xmis.f_getDivisionID(@deptid) "
			+ " SELECT @compid=公司ID FROM xmis.org_部门归属公司表 WHERE 部门ID= @divsionid "
			+ " SELECT CONVERT(VARCHAR(100),?)+','+xmis.f_getAFDeptPathByDeptId(@deptid)+CONVERT(VARCHAR(100),@compid) AS emppathid ";

	public static String sql_Search = "\n--模糊查询\n"
			+ "  SELECT c.公司ID ,\n"
			+ "        c.公司名称 ,\n"
			+ "RTRIM(c.电话1) AS  电话1,\n"
			+ "RTRIM(c.电话2) AS  电话2,\n"
			+ "RTRIM(c.传真1) AS  传真1,\n"
			+ "RTRIM(c.传真2) AS  传真2,\n"
			+ "RTRIM(c.网址) AS  网址,\n"
			+ "RTRIM(c.邮箱) AS  邮箱,\n"
			+ "RTRIM(c.开户银行) AS  开户银行,\n"
			+ "RTRIM(c.账号)  AS 账号,\n"
			+ "RTRIM(c.税号) AS  税号,\n"
			+ "RTRIM(c.机构代码)  AS 机构代码,\n"
			+ "RTRIM(c.注册资金)  AS 注册资金,\n"
			+ "        c.备注 ,\n"
			+ "        c.业务关系ID ,\n"
			+ "        xmis.org_业务关系.关系名称 ,\n"
			+ "        xmis.org_公司规模.名称 AS 公司规模名称 ,\n"
			+ "        xmis.org_客户级别.名称 AS 客户级别名称 ,\n"
			+ "        xmis.org_信用等级.名称 AS 信用等级名称 ,\n"
			+ "        RTRIM(contactperson.姓氏) + RTRIM(contactperson.名字) AS 联系人 ,\n"
			+ "        RTRIM(legalperson.姓氏) + RTRIM(legalperson.名字) AS 法人代表 ,\n"
			+ "        RTRIM(updateperson.姓氏) + RTRIM(updateperson.名字) AS 修改人 ,\n"
			+ "        RTRIM(lastupdateperson.姓氏) + RTRIM(lastupdateperson.名字) AS 最后修改人 ,\n"
			+ "        CONVERT(VARCHAR(100), c.修改时间, 23) AS 修改日期 ,\n"
			+ "        CONVERT(VARCHAR(100), c.最后修改日期, 23) AS 最后修改日期 ,\n"
			+ "        xmis.add_洲际表.名称 AS 洲际,\n"
			+ "        ( xmis.add_国家及地区.名称+xmis.add_地区.名称) AS 国家及地区,\n"
			+ "        xmis.add_省.省名称,\n"
			+ "        xmis.add_城市.城市名称,\n"
			+ "        xmis.add_地址.地址ID ,\n"
			+ "        xmis.add_地址.区县 ,\n"
			+ "        xmis.add_地址.邮编 ,\n"
			+ "        xmis.add_地址.详细地址,\n"
			+ "        NULL AS 收入金额,  \n"
			+ "        NULL AS 支出金额 \n"
			+ "        INTO #a\n"
			+ " FROM\n"
			+ "        xmis.org_公司 c\n"
			+ "        LEFT OUTER  JOIN ( xmis.add_地址\n"
			+ "                           JOIN xmis.add_城市 ON xmis.add_地址.城市ID = xmis.add_城市.城市ID\n"
			+ "                           JOIN xmis.add_省 ON xmis.add_城市.省ID = xmis.add_省.省ID\n"
			+ "                           JOIN xmis.add_地区 ON xmis.add_省.地区ID = xmis.add_地区.地区ID\n"
			+ "                           JOIN xmis.add_国家及地区 ON xmis.add_地区.国家ID = xmis.add_国家及地区.国家ID\n"
			+ "                           JOIN xmis.add_洲际表 ON xmis.add_国家及地区.洲际ID = xmis.add_洲际表.洲际ID\n"
			+ "                         ) ON c.地址ID = xmis.add_地址.地址ID\n"
			+ "        LEFT OUTER JOIN xmis.org_业务关系 ON c.业务关系ID = xmis.org_业务关系.业务关系ID\n"
			+ "        LEFT OUTER JOIN xmis.org_公司规模 ON c.公司规模ID = xmis.org_公司规模.公司规模ID\n"
			+ "        LEFT OUTER JOIN xmis.org_客户级别 ON c.客户级别ID = xmis.org_客户级别.客户级别ID\n"
			+ "        LEFT OUTER JOIN xmis.org_信用等级 ON c.信用等级ID = xmis.org_信用等级.信用等级ID\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 AS contactperson ON c.联系人ID = contactperson.人员ID\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 AS legalperson ON c.法人代表ID = legalperson.人员ID\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 AS updateperson ON c.修改人ID = updateperson.人员ID\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 AS lastupdateperson ON c.最后修改人ID = lastupdateperson.人员ID\n";

	public static String sql_Search_temp =

	"\n"
			+ "--收入 支出\n"
			+ " SELECT (a.合同总额 * c.对人民币汇率) AS 收入金额 ,0 AS 支出金额,\n"
			+ "        b.公司ID\n"
			+ " INTO   #contractAmt\n"
			+ " FROM   xmis.con_合同 a ,\n"
			+ "        xmis.org_公司 b ,\n"
			+ "        xmis.sys_货币 c\n"
			+ " WHERE  a.甲方公司ID = b.公司ID\n"
			+ "        AND a.货币ID = C.货币ID\n"
			+ "        AND a.合同类型ID = 1\n"
			+ "        AND a.甲方公司ID = b.公司ID\n"
			+ "\n"
			+ "UNION ALL\n"
			+ " SELECT 0 AS 收入金额 ,(a.合同总额 * c.对人民币汇率) AS 支出金额 ,\n"
			+ "        b.公司ID\n"
			+ " FROM   xmis.con_合同 a ,\n"
			+ "        xmis.org_公司 b ,\n"
			+ "        xmis.sys_货币 c\n"
			+ " WHERE  a.乙方公司ID = b.公司ID\n"
			+ "        AND a.货币ID = C.货币ID\n"
			+ "        AND a.合同类型ID = 2\n"
			+ "        AND a.父合同ID IS NULL\n"
			+ "        AND a.乙方公司ID = b.公司ID\n"
			+ "\n"
			+ "--收入 支出的总和\n"
			+ " SELECT SUM(#contractAmt.收入金额) AS 收入金额,SUM(#contractAmt.支出金额) AS 支出金额,#contractAmt.公司ID\n"
			+ " INTO #totalContactAmt\n"
			+ " FROM #contractAmt\n"
			+ " GROUP BY #contractAmt.公司ID\n"
			+ "\n"
			+ "  -- 交集\n"
			+ " SELECT\n"
			+ "        #a.公司ID ,\n"
			+ "        #a.公司名称 ,\n"
			+ "        RTRIM(#a.电话1) AS 电话1 ,\n"
			+ "        RTRIM(#a.电话2) AS 电话2 ,\n"
			+ "        RTRIM(#a.传真1) AS 传真1 ,\n"
			+ "        RTRIM(#a.传真2) AS 传真2 ,\n"
			+ "        RTRIM(#a.网址) AS 网址 ,\n"
			+ "        RTRIM(#a.邮箱) AS 邮箱 ,\n"
			+ "        RTRIM(#a.开户银行) AS 开户银行 ,\n"
			+ "        RTRIM(#a.账号) AS 账号 ,\n"
			+ "        RTRIM(#a.税号) AS 税号 ,\n"
			+ "        RTRIM(#a.机构代码) AS 机构代码 ,\n"
			+ "        RTRIM(#a.注册资金) AS 注册资金 ,\n"
			+ "        #a.备注 ,\n"
			+ "        #a.业务关系ID ,\n"
			+ "        #a.关系名称 ,\n"
			+ "        #a.公司规模名称 ,\n"
			+ "        #a.客户级别名称 ,\n"
			+ "        #a.信用等级名称 ,\n"
			+ "        #a.联系人 ,\n"
			+ "        #a.法人代表 ,\n"
			+ "        #a.修改人 ,\n"
			+ "        #a.最后修改人 ,\n"
			+ "        #a.修改日期 ,\n"
			+ "        #a.最后修改日期 ,\n"
			+ "        #a.洲际 ,\n"
			+ "        #a.国家及地区 ,\n"
			+ "        #a.省名称 ,\n"
			+ "        #a.城市名称 ,\n"
			+ "        #a.地址ID ,\n"
			+ "        #a.区县 ,\n"
			+ "        #a.邮编 ,\n"
			+ "        #a.详细地址 ,\n"
			+ "        #totalContactAmt.收入金额,\n"
			+ "        #totalContactAmt.支出金额\n"
			+ " INTO   #result\n"
			+ " FROM   #a\n"
			+ "        INNER JOIN #person ON #a.公司ID = #person.公司ID  AND #a.公司ID!=1\n"
			+ "        LEFT OUTER  JOIN #totalContactAmt ON #a.公司ID = #totalContactAmt.公司ID";

	public static String sql_SearchCompanyWithPageTotal = "  SELECT MAX(row_no) AS row_no,\n"
			+ "        null as  公司ID ,\n"
			+ "        null as  公司名称 ,\n"
			+ "        null as  电话1 ,\n"
			+ "        null as  电话2 ,\n"
			+ "        null as  传真1 ,\n"
			+ "        null as  传真2 ,\n"
			+ "        null as  网址 ,\n"
			+ "        null as  邮箱 ,\n"
			+ "        null as  开户银行 ,\n"
			+ "        null as  账号 ,\n"
			+ "        null as  税号 ,\n"
			+ "        null as  机构代码 ,\n"
			+ "        null as  注册资金 ,\n"
			+ "        null as  备注 ,\n"
			+ "        null as  业务关系ID ,\n"
			+ "        null as  关系名称 ,\n"
			+ "        null as  公司规模名称 ,\n"
			+ "        null as  客户级别名称 ,\n"
			+ "        null as  信用等级名称 ,\n"
			+ "        null as  联系人 ,\n"
			+ "        null as  法人代表 ,\n"
			+ "        null as  修改人 ,\n"
			+ "        null as  最后修改人 ,\n"
			+ "        null as  修改日期 ,\n"
			+ "        null as  最后修改日期 ,\n"
			+ "        null as  洲际,\n"
			+ "        null as  国家及地区,\n"
			+ "        null as  省名称,\n"
			+ "        null as  城市名称,\n"
			+ "        null as  地址ID ,\n"
			+ "        null as  区县 ,\n"
			+ "        null as  邮编 ,\n"
			+ "        null as  详细地址,\n"
			+ "        null as  收入金额,\n"
			+ "        null as  支出金额\n"
			+ "FROM #resultA "
			+ " UNION ALL "
			+ "\n\nSELECT * from #resultA "
			+ "WHERE row_no >= ? "
			+ " AND row_no < ? ";

	public static String sql_PersonSearch = "\n--人员模糊查询\n"
			+ "  SELECT DISTINCT  c.公司ID\n"
			+ "INTO #person\n"
			+ "FROM    xmis.org_公司 c\n"
			+ "        LEFT OUTER JOIN ( xmis.hr_人员 h\n"
			+ "                          JOIN xmis.hr_人员部门关系表 pd ON pd.人员ID = h.人员ID\n"
			+ "                          JOIN xmis.org_部门归属公司表 dc ON dc.部门ID = xmis.f_getDivisionID(pd.部门ID)\n"
			+ "                        ) ON dc.公司ID = c.公司ID\n";

	public static String getCountryAreaCodeById = "SELECT 区号 FROM xmis.add_国家及地区 WHERE 国家ID=?";
	public static String getcityAreaCodeById = "SELECT 区号 FROM xmis.add_城市 where 城市ID=?";

	public static String savePostCardFontName = update
			+ DBSchema.Employee.TableName + set + " 名片正面名字  " + " = ?, "
			+ DBSchema.Employee.UpdateID + "= ?,"
			+ DBSchema.Employee.UpdateDate + "= GETDATE()" + where
			+ DBSchema.Employee.ID + " = ? ";

	public static String savePostCardBackName = update
			+ DBSchema.Employee.TableName + set + " 名片背面名字  " + " = ?, "
			+ DBSchema.Employee.UpdateID + "= ?,"
			+ DBSchema.Employee.UpdateDate + "= GETDATE()" + where
			+ DBSchema.Employee.ID + " = ? ";
	public static String savePostMaterials = "update xmis.dev_物资信息库 set 照片名称 = ? where 物资ID = ? ";
	public static String saveUserTopicMessageHabit = "UPDATE xmis.sys_消息记录表 SET receive_flag = 1,receive_time = GETDATE() WHERE message_id = ? AND receive_id= ? ";

	public static String getMessageByUserIDAndTopicID = "SELECT  xmis.sys_消息定义表.message_id ,\n"
			+ " message_info ,\n"
			+ " from_id ,\n"
			+ " RTRIM(姓氏) + RTRIM(名字) AS from_name ,\n"
			+ " from_time ,\n"
			+ " system_msg_flag\n"
			+ " FROM xmis.sys_消息记录表\n"
			+ " JOIN xmis.sys_消息定义表 ON xmis.sys_消息记录表.message_id = xmis.sys_消息定义表.message_id\n"
			+ " LEFT JOIN xmis.hr_人员 ON from_id = xmis.hr_人员.人员ID\n"
			+ " WHERE receive_id = ?\n"
			+ " AND topic_type = ?\n"
			+ " AND receive_flag = 0 ORDER BY from_time";

	public static String insertMessageDefinition = "INSERT  INTO xmis.sys_消息定义表 VALUES  ( ?, ?, ?, ?,GETDATE(), 0 )";

	public static String insertMessageRecord = "   INSERT INTO xmis.sys_消息记录表\n"
			+ " SELECT  ? message_id ,\n"
			+ " new.empId receive_id ,\n"
			+ " NULL receive_time ,\n"
			+ " 0 receive_flag\n"
			+ " FROM    ( SELECT    a.人员ID empId\n"
			+ " FROM      xmis.hr_人员部门关系表 b\n"
			+ " JOIN xmis.hr_人员 a ON a.人员ID = b.人员ID\n"
			+ " LEFT JOIN xmis.org_部门归属公司表 e ON b.部门ID = e.部门ID\n"
			+ " WHERE     a.人员ID > 0\n"
			+ " AND e.公司ID = 1\n"
			+ " AND ( a.禁用 = 0 OR a.禁用 IS NULL ) ) new  ";

	public static String insertPersonMessageDefinition = "INSERT  INTO xmis.sys_消息定义表 VALUES  ( ?, ?, ?, ?,GETDATE(), 1 )";

	public static String insertPersonMessageRecord = "INSERT  INTO xmis.sys_消息记录表 VALUES  ( ?, ?, null, 0 )";

	// public static String gethaveReceivedMessageByUseId =
	// "SELECT  xmis.sys_消息定义表.message_id ,\n"
	// + " message_info ,\n"
	// + " from_id ,\n"
	// + " RTRIM(姓氏) + RTRIM(名字) AS from_name ,\n"
	// + " from_time , receive_time,\n"
	// + " system_msg_flag\n"
	// + " FROM    xmis.sys_消息记录表\n"
	// +
	// " JOIN xmis.sys_消息定义表 ON xmis.sys_消息记录表.message_id = xmis.sys_消息定义表.message_id\n"
	// + " LEFT JOIN xmis.hr_人员 ON from_id = xmis.hr_人员.人员ID\n"
	// + " WHERE   receive_id = ? AND receive_flag = 0";

	public static String getActorAndAssignmentStrategyList =

	"SELECT  CASE WHEN xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
			+ "                                                           task.PROCESS_ID,GETDATE()) IS NULL\n"
			+ "             THEN RTRIM(p.姓氏) + RTRIM(P.名字)\n"
			+ "             ELSE RTRIM(p.姓氏) + RTRIM(P.名字) + '('\n"
			+ "                  + xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
			+ "                                                             task.PROCESS_ID,GETDATE())\n"
			+ "                  + '代理)'\n"
			+ "        END AS 接收人,NULL AS ASSIGNMENT_STRATEGY\n"
			+ "FROM    xmis.T_FF_RT_TASKINSTANCE task ,\n"
			+ "        xmis.T_FF_RT_WORKITEM item ,\n"
			+ "        xmis.hr_人员 p\n"
			+ "WHERE   --PROCESSINSTANCE_ID = '8a8181ea4332ce5d014332d0772d0002'\n"
			+ "        task.PROCESSINSTANCE_ID = ?\n"
			+ "        AND item.ACTOR_ID = p.人员ID\n"
			+ "        AND task.ID = item.TASKINSTANCE_ID\n"
			+ "        AND ( item.state = 0\n"
			+ "              OR item.state = 1\n" + "            )\n" + "";

	// "SELECT  RTRIM(p.姓氏) + RTRIM(P.名字) AS 接收人\n" +
	// " ,task.ASSIGNMENT_STRATEGY \n"+
	// "FROM    xmis.T_FF_RT_TASKINSTANCE task ,\n" +
	// "        xmis.T_FF_RT_WORKITEM item ,\n" +
	// "        xmis.hr_人员 p\n" +
	// "WHERE   PROCESSINSTANCE_ID = ?\n" +
	// "        AND item.ACTOR_ID = p.人员ID\n" +
	// "        AND task.ID = item.TASKINSTANCE_ID\n" +
	// "        AND ( item.state = 0\n" +
	// "              OR item.state = 1\n" +
	// "            )\n" +

	public static String getProcessInstanceList = "SELECT  process.ID AS 流程实例ID,\n"
			+ "       CASE WHEN  k.关键字 IS NULL THEN  process.DISPLAY_NAME  else \n"
			+ "        k.关键字  END AS 流程名称,\n"
			+ "       PROCESS_ID,\n"
			+ "       process.CREATOR_ID,\n"
			+ "       RTRIM(p.姓氏) + RTRIM(P.名字) AS 创建人 ,\n"
			+ "       process.STATE AS 状态 ,\n"
			+ "       CONVERT(VARCHAR(100),process.CREATED_TIME,21) AS 创建时间 ,\n"
			+

			"       CASE WHEN process.STATE='7' THEN  CONVERT(VARCHAR(100), process.END_TIME, 21)\n"
			+ "        ELSE xmis.f_getTimeByProcessInstanceId( process.ID)  END AS 完成时间,\n"
			+
			// "       CONVERT(VARCHAR(100),process.END_TIME,21) AS 完成时间,\n" +
			"       process.VERSION,\n"
			+ "       NULL AS 接收人\n"
			+ "       into #result \n"
			+ "FROM   xmis.T_FF_RT_PROCESSINSTANCE process,\n"
			+ "       xmis.hr_人员 p,\n"
			+ "       xmis.wf_流程关键字表 k\n"
			+ "WHERE  process.CREATOR_ID=p.人员ID\n"
			+ "AND process.ID *= k.流程实例ID\n"
			+ "ORDER BY process.CREATED_TIME DESC";

	public static String sql_SearchProcessInstanceListWithPageTotal = "  SELECT MAX(row_no) AS row_no,\n"
			+ "        null as  流程实例ID ,\n"
			+ "        null as  流程名称 ,\n"
			+ "        NULL AS PROCESS_ID,\n"
			+ "        NULL AS CREATOR_ID,\n"
			+ "        NULL AS 创建人 ,\n"
			+ "        NULL AS 状态 ,\n"
			+ "        NULL AS 创建时间 ,\n"
			+ "        NULL AS 完成时间,\n"
			+ "        NULL AS VERSION,\n"
			+ "        NULL AS 接收人\n"
			+ "FROM #resultA "
			+ " UNION ALL "
			+ "\n\nSELECT * from #resultA "
			+ "WHERE row_no >= ? " + " AND row_no < ? ";

	public static String getProcessInstanceComb = " SELECT DISTINCT PROCESS_ID AS id,DISPLAY_NAME AS name FROM xmis.T_FF_DF_WORKFLOWDEF"
			+ " where PROCESS_ID='MissAttence'";

	public static String getTaskListByQuery = "--根据taskId 将\"我的任务\"分类\n"
			+ " SELECT  a.ID ,\n" + "        a.task_ID ,\n"
			+ "        c.ID as workItemId ,\n" + "        a.DISPLAY_NAME ,\n"
			+ "        todo.标题   ,\n" + "        todo.id as todoId,\n"
			+ "        todo.发送人 AS 发送人ID,\n" + "        pd.部门ID AS  发送人部门ID,\n"
			+ "        d.部门名称  AS  发送人部门名称,\n" + "        todo.用户ID,\n"
			+ "        RTRIM(pp.姓氏) + RTRIM(pp.名字) AS 发送人 ,\n"
			+ "        RTRIM(p.姓氏) + RTRIM(P.名字) AS 接收人 ,\n"
			+ "        a.STATE AS 状态 ,\n" + "        a.PROCESSINSTANCE_ID ,\n"
			+ "        c.COMMENTS ,\n"
			+ "        CONVERT(VARCHAR(50),a.CREATED_TIME,21) AS 创建时间\n"
			+ "        into #result \n"
			+ " FROM   xmis.T_FF_RT_TASKINSTANCE a ,\n"
			+ "        xmis.wf_待办 todo ,\n"
			+ "        xmis.T_FF_RT_WORKITEM c ,\n"
			+ "        xmis.hr_人员 p ,\n" + "        xmis.hr_人员 pp,\n"
			+ "        xmis.hr_人员部门关系表 pd ,\n" + "        xmis.org_部门 d\n"
			+ " WHERE  a.id = c.TASKINSTANCE_ID\n"
			+ "        AND todo.应用ID = c.ID\n"
			+ "        AND todo.用户ID *= p.人员ID\n"
			+ "        AND todo.发送人 = pp.人员ID\n"
			+ "         AND todo.发送人 = pd.人员ID\n"
			+ "          AND pd.部门ID =d.部门ID\n" + "        AND todo.状态 = 1\n"
			+ "        AND todo.是否显示 = 1\n" + " ORDER BY a.TASK_ID";

	public static String sql_SearchTaskListWithPageTotal = "  SELECT MAX(row_no) AS row_no,\n"
			+ "     NULL AS ID ,\n"
			+ "     NULL AS task_ID ,\n"
			+ "     NULL AS workItemId ,\n"
			+ "     NULL AS DISPLAY_NAME ,\n"
			+ "     NULL AS 标题 ,\n"
			+ "     NULL AS todoId ,\n"
			+ "     NULL AS 发送人ID ,\n"
			+ "     NULL AS 发送人部门ID,\n"
			+ "     NULL AS 发送人部门名称 ,\n"
			+ "     NULL AS 用户ID ,\n"
			+ "     NULL AS 发送人 ,\n"
			+ "     NULL AS 接收人 ,\n"
			+ "     NULL AS 状态 ,\n"
			+ "     NULL AS PROCESSINSTANCE_ID ,\n"
			+ "     NULL AS COMMENTS ,\n"
			+ "     NULL AS 创建时间 "
			+ " FROM #resultA "
			+ " UNION ALL "
			+ "\n\nSELECT * from #resultA "
			+ "WHERE row_no >= ? " + " AND row_no < ? ";
	public static String GetTileAndTaskIDByUserID = "SELECT DISTINCT\n"
			+ "          a.task_ID ,\n" + "          a.DISPLAY_NAME \n"
			+ "FROM      xmis.T_FF_RT_TASKINSTANCE a ,\n"
			+ "          xmis.wf_待办 todo ,\n"
			+ "          xmis.T_FF_RT_WORKITEM c\n"
			+ "WHERE     a.id = c.TASKINSTANCE_ID\n"
			+ "          AND todo.应用ID = c.ID\n"
			+ "          AND todo.用户ID =?\n" + "          AND todo.是否显示=1\n"
			+ "          AND a.task_ID LIKE 'MissAttence.DeptAssign%'\n";
	// "          AND ( a.task_ID LIKE 'ProjectCheck%'\n" +
	// "                OR a.task_ID LIKE 'ProjectExecutiveDeptChange%'\n" +
	// "                OR a.task_ID LIKE 'ProjectTaskAssign%'\n" +
	// "                OR a.task_ID LIKE 'PurchaseRequisition%'\n" +
	// "                OR a.task_ID LIKE 'StartProject%'\n" +
	// "              )";

	public static String sql_getTileAndTaskIDByUserID = "  SELECT MAX(row_no) AS row_no,\n"
			+ "          NULL AS task_ID ,\n"
			+ "          NULL AS DISPLAY_NAME\n"
			+ " FROM #resultA "
			+ " UNION ALL "
			+ "\n\nSELECT * from #resultA "
			+ "WHERE row_no >= ? " + " AND row_no < ? ";

	public static final String getIsExistByDeptId = "SELECT count(是否直属) FROM xmis.org_部门归属公司表 WHERE 公司ID=1 AND 部门ID=?";
	public static final String getIsDirectByDeptId = "SELECT 是否直属 FROM xmis.org_部门归属公司表 WHERE 公司ID=1 AND 部门ID=?";

	public static final String getPersonByDepId = "SELECT xmis.hr_人员.人员ID ,\n"
			+ "       RTRIM(xmis.hr_人员.姓氏) + RTRIM(xmis.hr_人员.名字) AS name\n"
			+ "FROM   xmis.hr_人员部门关系表 ,\n" + "       xmis.hr_人员\n"
			+ "WHERE  xmis.hr_人员.人员ID = xmis.hr_人员部门关系表.人员ID\n"
			+ "       AND ( xmis.hr_人员.禁用 = 0\n"
			+ "             OR xmis.hr_人员.禁用 IS NULL\n" + "           )\n"
			+ "       AND xmis.hr_人员.人员ID > 0\n" + "       AND 部门ID = ?";

	public static String getContractSourceTypeID = "SELECT  '0'  合同来源类型ID,\n"
			+ " '所有合同' 合同来源类型    UNION ALL \n"
			+ " SELECT  CAST(合同来源类型ID AS VARCHAR) 合同来源类型ID , 合同来源类型\n"
			+ " FROM    xmis.con_合同来源类型    WHERE   合同来源类型ID = 1\n"
			+ " OR 合同来源类型ID = 201   UNION ALL\n"
			+ " SELECT  CAST(合同来源类型ID AS VARCHAR) 合同来源类型ID ,\n"
			+ " '非自研' + LEFT(合同来源类型, 2) 合同来源类型\n"
			+ " FROM    xmis.con_合同来源类型    WHERE   合同来源类型ID = 101\n"
			+ " OR 合同来源类型ID = 102  ORDER BY 合同来源类型ID ";

	public static String getContractSourceTypeForEditContract = "   SELECT   合同来源类型ID ,\n"
			+ " 合同来源类型\n"
			+ " FROM     xmis.con_合同来源类型 a\n"
			+ " WHERE    a.合同来源类型ID IN (\n"
			+ " SELECT  b.合同来源类型ID\n"
			+ " FROM    xmis.con_合同来源类型 b\n"
			+ " INNER JOIN xmis.con_合同来源类型 c ON b.合同来源类型ID = LEFT(c.合同来源类型ID,\n"
			+ " LEN(b.合同来源类型ID))\n"
			+ " GROUP BY b.合同来源类型ID\n"
			+ " HAVING  LEN(b.合同来源类型ID) = MAX(LEN(c.合同来源类型ID)) )";

	public static String getExcuteDeptInfo = "SELECT  部门ID  deptID ,部门名称  deptName   from xmis.org_部门   WHERE 部门ID IN ? ORDER BY 部门ID ";

	public static String getPersonByDeptID = "\n" + "SELECT a.人员ID userID ,\n"
			+ "       RTRIM(姓氏) + RTRIM(名字) userName ,\n"
			+ "       b.部门ID tdeptId ,\n" + "       d.部门名称 tdeptName ,\n"
			+ "       c.职务名称 position ,\n"
			+ "       CASE WHEN c.排序 IS NULL THEN '1004'\n"
			+ "            ELSE c.排序\n" + "       END cp ,\n"
			+ "       CASE WHEN b.排序 IS NULL THEN '10001'\n"
			+ "            ELSE b.排序\n" + "       END bp\n"
			+ "FROM   xmis.hr_人员部门关系表 b\n"
			+ "       JOIN xmis.hr_人员 a ON a.人员ID = b.人员ID\n"
			+ "       LEFT JOIN xmis.sys_职务 c ON b.职务 = c.职务ID\n"
			+ "       LEFT JOIN xmis.org_部门 d ON b.部门ID = d.部门ID\n"
			+ "WHERE  a.人员ID > 0\n" + "       AND b.部门ID = ?\n"
			+ "       AND ( a.禁用 = 0\n" + "             OR a.禁用 IS NULL\n"
			+ "           )\n" + "ORDER BY cp ,\n" + "       bp ,\n"
			+ "       userName";

	// 去除监控职业部、水利部的登录名 和真是姓名
	// public static String getDugDataWithDept1And2 =
	// "SELECT  '000p' + CONVERT(VARCHAR(20), p.人员ID) AS 用户名 ,\n" +
	// "        RTRIM(p.姓氏) + RTRIM(p.名字) AS 真实姓名\n" +
	// "FROM    xmis.hr_人员 p ,\n" +
	// "        xmis.hr_人员部门关系表 d\n" +
	// "WHERE   p.人员ID = d.人员ID\n" +
	// "        AND ( d.部门ID = 2\n" +
	// "              OR d.部门ID IN ( SELECT deptid\n" +
	// "                             FROM   xmis.f_getDeptPath(1) )\n" +
	// "            )\n" +
	// "        AND P.人员ID > 0\n" +
	// "        AND ( p.禁用 IS NULL\n" +
	// "              OR p.禁用 = 0\n" +
	// "            )\n" +
	// "ORDER BY d.人员ID";

	public static final String getZentaoDataFromEmployee = "SELECT  RTRIM(p.姓氏) + RTRIM(p.名字) AS 真实姓名 ,\n"
			+ "        '000p' + CONVERT(VARCHAR(20), p.人员ID) AS 用户名 ,\n"
			+ "        RTRIM(p.密码) as 密码  ,\n"
			+ "        d.部门ID AS 部门,\n"
			+ "         CASE when p.性别ID=0 THEN 'f' ELSE 'm' END as  性别\n"
			+ "FROM    xmis.hr_人员 p ,\n"
			+ "        xmis.hr_人员部门关系表 d\n"
			+ "WHERE   p.人员ID = d.人员ID\n"
			+ "        AND d.部门ID = 6\n"
			+ "        AND P.人员ID > 0\n"
			+ "        AND ( p.禁用 IS NULL\n"
			+ "              OR p.禁用 = 0\n"
			+ "            )\n"
			+ "            AND p.人员ID NOT IN('1162','1068','1400')\n"
			+ "ORDER BY d.人员ID";

	public static final String getAllMissAttendanceList = "SELECT  COUNT(todo.ID)\n"
			+ "FROM    xmis.wf_待办 todo ,\n"
			+ "        xmis.T_FF_RT_WORKITEM workitem ,\n"
			+ "        xmis.T_FF_RT_TASKINSTANCE taskinstance\n"
			+ "WHERE   todo.应用ID = workitem.ID\n"
			+ "        AND workitem.TASKINSTANCE_ID = taskinstance.ID\n"
			+ "        AND todo.是否显示 = 1\n"
			+ "        AND todo.用户ID = ?\n"
			+ "        AND 模块 = ?\n"
			+ "        AND SUBSTRING(CONVERT(VARCHAR(20), todo.创建时间), 0, 11) <= ?";

	public static final String getMissAttendanceList =

	"SELECT  info.ID ,\n"
			+ "        attd.申请人ID ,\n"
			+ "        RTRIM(applicant.姓氏) + RTRIM(applicant.名字) AS applicantname ,\n"
			+ "        dept.部门名称 ,\n"
			+ "        attd.申请日期 ,\n"
			+ "        info.缺勤日期 ,\n"
			+ "        info.开始日期 ,\n"
			+ "        info.结束日期 ,\n"
			+ "        info.上下午 ,\n"
			+ "        info.事由 ,\n"
			+ "        attd.操作人ID ,\n"
			+ "        RTRIM(agent.姓氏) + RTRIM(agent.名字) AS agentname ,\n"
			+ "        todo.应用ID AS workItemId ,\n"
			+ "        attd.流程实例ID ,\n"
			+ "        attd.考勤单ID,\n"
			+ "        xmis.f_getwfapproveinfoStr(attd.流程实例ID ) AS 审批人及审批时间, todo.状态, attd.出差单标识,todo.用户ID, todo.ID AS todoid,\n"
			+ "               CASE WHEN xmis.f_getAuthorizeeByUserIDAndProcessID(workitem.ACTOR_ID,\n"
			+ "                                                           task.PROCESS_ID,GETDATE()) IS NULL\n"
			+ "             THEN RTRIM(actor.姓氏) + RTRIM(actor.名字)\n"
			+ "             ELSE RTRIM(actor.姓氏) + RTRIM(actor.名字) + '('\n"
			+ "                  + xmis.f_getAuthorizeeByUserIDAndProcessID(workitem.ACTOR_ID,\n"
			+ "                                                             task.PROCESS_ID,GETDATE())\n"
			+ "                  + '代理)'\n" + "        END AS 任务人\n"
			+ "INTO #result\n" + "FROM    xmis.wf_待办 todo ,\n"
			+ "        xmis.T_FF_RT_WORKITEM workitem ,\n"
			+ "        xmis.T_FF_RT_TASKINSTANCE task ,\n"
			+ "        xmis.attd_补报考勤单表 attd ,\n"
			+ "        xmis.attd_补报考勤内容表 info ,\n"
			+ "        xmis.hr_人员 applicant ,\n"
			+ "        xmis.hr_人员 agent ,\n" + "        xmis.hr_人员 actor,\n"
			+ "        xmis.hr_人员部门关系表 pd ,\n" + "        xmis.org_部门 dept\n"
			+ "WHERE   todo.应用ID = workitem.id\n"
			+ "        AND workitem.TASKINSTANCE_ID = task.ID\n"
			+ "        AND attd.流程实例ID = task.PROCESSINSTANCE_ID\n"
			+ "        AND attd.考勤单ID = info.所属考勤单ID\n"
			+ "        AND attd.申请人ID = applicant.人员ID\n"
			+ "        AND attd.操作人ID = agent.人员ID\n"
			+ "        AND todo.用户ID *=actor.人员ID\n"
			+ "        AND applicant.人员ID = pd.人员ID\n"
			+ "        AND pd.部门ID = dept.部门ID";

	public static final String getLastDay = "  SELECT 数值  AS value from xmis.sys_参数配置表 WHERE 名称=?";

	public static final String getEmpsByRoleAndDivisionID = "SELECT r.人员ID FROM xmis.sec_人员角色 r ,xmis.hr_人员部门关系表 d WHERE d.人员ID=r.人员ID AND r.角色ID=? AND d.部门ID IN (SELECT deptid FROM xmis.f_getDeptPath(?))";

	public static final String getIsExistRole = "SELECT  COUNT(ID) FROM  xmis.sec_人员角色 r WHERE   角色ID = ? AND 人员ID=?";

	public static final String isExistMissAttendanceListByEmpId = "SELECT  COUNT(todo.ID)\n"
			+ "FROM    xmis.wf_待办 todo\n"
			+ "WHERE   todo.用户ID = ?\n"
			+ "        AND 模块 = ?\n" + "        AND todo.创建时间 =?";

	public static final String getMissAttendanceListByEmpId =

	"SELECT  info.ID ,\n"
			+ "        attd.申请人ID ,\n"
			+ "        RTRIM(applicant.姓氏) + RTRIM(applicant.名字) AS applicantname ,\n"
			+ "        dept.部门名称 ,\n"
			+ "        attd.申请日期 ,\n"
			+ "        info.缺勤日期 ,\n"
			+ "        info.开始日期 ,\n"
			+ "        info.结束日期 ,\n"
			+ "        info.上下午 ,\n"
			+ "        info.事由 ,\n"
			+ "        attd.操作人ID ,\n"
			+ "        todo.用户ID ,\n"
			+ "        CASE WHEN xmis.f_getAuthorizeeByUserIDAndProcessID(workitem.ACTOR_ID,\n"
			+ "                                                           task.PROCESS_ID,GETDATE()) IS NULL\n"
			+ "             THEN RTRIM(actor.姓氏) + RTRIM(actor.名字)\n"
			+ "             ELSE RTRIM(actor.姓氏) + RTRIM(actor.名字) + '('\n"
			+ "                  + xmis.f_getAuthorizeeByUserIDAndProcessID(workitem.ACTOR_ID,\n"
			+ "                                                             task.PROCESS_ID,GETDATE())\n"
			+ "                  + '代理)'\n" + "        END AS 任务人 ,\n"
			+ "        RTRIM(agent.姓氏) + RTRIM(agent.名字) AS agentname ,\n"
			+ "        todo.应用ID AS workItemId ,\n" + "        attd.流程实例ID ,\n"
			+ "        attd.考勤单ID ,\n"
			+ "        xmis.f_getwfapproveinfoStr(attd.流程实例ID) AS 审批人及审批时间\n"
			+ "FROM    xmis.wf_待办 todo ,\n"
			+ "        xmis.T_FF_RT_WORKITEM workitem ,\n"
			+ "        xmis.T_FF_RT_TASKINSTANCE task ,\n"
			+ "        xmis.attd_补报考勤单表 attd ,\n"
			+ "        xmis.attd_补报考勤内容表 info ,\n"
			+ "        xmis.hr_人员 applicant ,\n"
			+ "        xmis.hr_人员 agent ,\n" + "        xmis.hr_人员 actor ,\n"
			+ "        xmis.hr_人员部门关系表 pd ,\n" + "        xmis.org_部门 dept\n"
			+ "WHERE   todo.应用ID = workitem.id\n"
			+ "        AND workitem.TASKINSTANCE_ID = task.ID\n"
			+ "        AND attd.流程实例ID = task.PROCESSINSTANCE_ID\n"
			+ "        AND attd.考勤单ID = info.所属考勤单ID\n"
			+ "        AND attd.申请人ID = applicant.人员ID\n"
			+ "        AND attd.操作人ID = agent.人员ID\n"
			+ "        AND todo.用户ID = actor.人员ID\n"
			+ "        AND applicant.人员ID = pd.人员ID\n"
			+ "        AND pd.部门ID = dept.部门ID\n"
			+ "        AND todo.是否显示 = 1\n" + "        AND todo.模块=?\n"
			+ "        AND todo.应用ID = ?\n" + "        AND todo.状态 = ?\n"
			+ "        AND attd.出差单标识= ?\n";

	public static String getMissAttendanceListByQuery = "--考勤单 审批信息\n"
			+ "SELECT\n"
			+ "  DISTINCT\n"
			+ "        attd.考勤单ID\n"
			+ "INTO    #a\n"
			+ "FROM    xmis.T_FF_RT_WORKITEM workitem ,\n"
			+ "        xmis.T_FF_RT_TASKINSTANCE task ,\n"
			+ "        xmis.attd_补报考勤单表 attd ,\n"
			+ "        xmis.T_FF_RT_PROCESSINSTANCE process\n"
			+ "WHERE   workitem.TASKINSTANCE_ID = task.ID\n"
			+ "        AND task.PROCESSINSTANCE_ID = process.ID\n"
			+ "        AND attd.流程实例ID = task.PROCESSINSTANCE_ID\n"
			+ "        AND process.STATE = 7\n"
			+ "\n"
			+ "\n"
			+ "--考勤单详细信息\n"
			+ "SELECT  info.ID ,\n"
			+ "        attd.申请人ID ,\n"
			+ "        RTRIM(applicant.姓氏) + RTRIM(applicant.名字) AS applicantname ,\n"
			+ "        dept.部门ID ,\n" + "        dept.部门名称 ,\n"
			+ "        division.部门ID AS 事业部ID ,\n"
			+ "        division.部门名称 AS 事业部 ,\n" + "        attd.申请日期 ,\n"
			+ "        info.缺勤日期 ,\n" + "        info.开始日期 ,\n"
			+ "        info.结束日期 ,\n" + "        info.上下午 ,\n"
			+ "        info.事由 ,\n" + "        attd.操作人ID ,\n"
			+ "        RTRIM(agent.姓氏) + RTRIM(agent.名字) AS agentname ,\n"
			+ "        attd.流程实例ID ,\n" + "        attd.考勤单ID ,\n"
			+ "        attd.出差单标识 ,\n"
			+ "        xmis.f_getwfapproveinfoStr(attd.流程实例ID) AS 审批人及审批时间\n"
			+ "INTO    #b\n" + "FROM    xmis.attd_补报考勤单表 attd ,\n"
			+ "        xmis.attd_补报考勤内容表 info ,\n"
			+ "        xmis.hr_人员 applicant ,\n"
			+ "        xmis.hr_人员 agent ,\n" + "        xmis.hr_人员部门关系表 pd ,\n"
			+ "        xmis.org_部门 dept ,\n" + "        xmis.org_部门 division\n"
			+ "WHERE   attd.考勤单ID = info.所属考勤单ID\n"
			+ "        AND attd.申请人ID = applicant.人员ID\n"
			+ "        AND attd.操作人ID = agent.人员ID\n"
			+ "        AND applicant.人员ID = pd.人员ID\n"
			+ "        AND pd.部门ID = dept.部门ID\n"
			+ "        AND division.部门ID = xmis.f_getDivisionID(dept.部门ID)\n"
			+ "SELECT info.ID AS infoID ,\n" + "       mt.是否被处理\n"
			+ "INTO   #d\n" + "FROM   xmis.attd_补报考勤内容表 info ,\n"
			+ "       attd_考勤月记录表 mt\n" + "WHERE  info.id*=mt.infoID\n"
			+ "SELECT #b.* ,\n" + "       #d.是否被处理\n" + "INTO   #result\n"
			+ "FROM   #a ,\n" + "       #b ,\n" + "       #d\n"
			+ "WHERE  #a.考勤单ID = #b.考勤单ID\n"
			+ "       AND #b.id = #d.infoID \n";

	public static String getMissAttendanceListByQueryWithMonth = "--考勤单 审批信息\n"
			+ "SELECT\n"
			+ "  DISTINCT\n"
			+ "        attd.考勤单ID\n"
			+ "INTO    #a\n"
			+ "FROM    xmis.T_FF_RT_WORKITEM workitem ,\n"
			+ "        xmis.T_FF_RT_TASKINSTANCE task ,\n"
			+ "        xmis.attd_补报考勤单表 attd ,\n"
			+ "        xmis.T_FF_RT_PROCESSINSTANCE process\n"
			+ "WHERE   workitem.TASKINSTANCE_ID = task.ID\n"
			+ "        AND task.PROCESSINSTANCE_ID = process.ID\n"
			+ "        AND attd.流程实例ID = task.PROCESSINSTANCE_ID\n"
			+ "        AND process.STATE = 7\n"
			+ "\n"
			+ "\n"
			+ "--考勤单详细信息\n"
			+ "SELECT  info.ID ,\n"
			+ "        attd.申请人ID ,\n"
			+ "        RTRIM(applicant.姓氏) + RTRIM(applicant.名字) AS applicantname ,\n"
			+ "        dept.部门ID ,\n"
			+ "        dept.部门名称 ,\n"
			+ "        division.部门ID AS 事业部ID ,\n"
			+ "        division.部门名称 AS 事业部 ,\n"
			+ "        attd.申请日期 ,\n"
			+ "        info.缺勤日期 ,\n"
			+ "        info.开始日期 ,\n"
			+ "        info.结束日期 ,\n"
			+ "        info.上下午 ,\n"
			+ "        info.事由 ,\n"
			+ "        attd.操作人ID ,\n"
			+ "        RTRIM(agent.姓氏) + RTRIM(agent.名字) AS agentname ,\n"
			+ "        attd.流程实例ID ,\n"
			+ "        attd.考勤单ID ,\n"
			+ "        attd.出差单标识 ,\n"
			+ "        xmis.f_getwfapproveinfoStr(attd.流程实例ID) AS 审批人及审批时间\n"
			+ "INTO    #b\n"
			+ "FROM    xmis.attd_补报考勤单表 attd ,\n"
			+ "        xmis.attd_补报考勤内容表 info ,\n"
			+ "        xmis.hr_人员 applicant ,\n"
			+ "        xmis.hr_人员 agent ,\n"
			+ "        xmis.hr_人员部门关系表 pd ,\n"
			+ "        xmis.org_部门 dept ,\n"
			+ "        xmis.org_部门 division\n"
			+ "WHERE   attd.考勤单ID = info.所属考勤单ID\n"
			+ "        AND attd.申请人ID = applicant.人员ID\n"
			+ "        AND attd.操作人ID = agent.人员ID\n"
			+ "        AND applicant.人员ID = pd.人员ID\n"
			+ "        AND pd.部门ID = dept.部门ID\n"
			+ "        AND division.部门ID = xmis.f_getDivisionID(dept.部门ID)\n"
			+ "SELECT  info.ID AS infoID ,\n"
			+ "        mt.是否被处理\n"
			+ "INTO    #d\n"
			+ "FROM    xmis.attd_补报考勤内容表 info ,\n"
			+ "        attd_考勤月记录表 mt\n"
			+ "WHERE   info.id*=mt.infoID\n"
			+ "\n"
			+ "\n"
			+ "SELECT  #b.* \n"
			+ "INTO    #c\n"
			+ "FROM    #a ,\n"
			+ "        #b \n"
			+ "WHERE   #a.考勤单ID = #b.考勤单ID\n"
			+

			"--开始日期、结束日期的跨月处理 --- 后补充\n"
			+ "SELECT  #c.ID ,\n"
			+ "        #c.申请人ID ,\n"
			+ "        #c.applicantname ,\n"
			+ "        #c.部门ID ,\n"
			+ "        #c.部门名称 ,\n"
			+ "        #c.事业部ID ,\n"
			+ "        #c.事业部 ,\n"
			+ "        #c.申请日期 ,\n"
			+ "        #c.缺勤日期 ,\n"
			+ "        CASE WHEN #c.开始日期 <= '@DATEFIRST' THEN '@DATEFIRST'\n"
			+ "             ELSE #c.开始日期\n"
			+ "        END AS 开始日期 ,\n"
			+ "        CASE WHEN #c.结束日期 >= '@DATELAST' THEN '@DATELAST'\n"
			+ "             ELSE #c.结束日期\n"
			+ "        END AS 结束日期 ,\n"
			+ "        #c.上下午 ,\n"
			+ "        #c.事由 ,\n"
			+ "        #c.操作人ID ,\n"
			+ "        #c.agentname ,\n"
			+ "        #c.流程实例ID ,\n"
			+ "        #c.考勤单ID ,\n"
			+ "        #c.出差单标识 ,\n"
			+ "        #c.审批人及审批时间,\n"
			+ "        mt.是否被处理 \n"
			+ " into #result\n"
			+ "FROM    #c, xmis.attd_考勤月记录表 mt\n"
			+ "WHERE    #c.id*=mt.infoID\n"
			+ "          AND 年份=SUBSTRING('@DATEFIRST',0,5) AND 月份=SUBSTRING('@DATEFIRST',6,2)\n"
			+ " and ( 结束日期 >= '@DATEFIRST'\n"
			+ "          AND 开始日期 <= '@DATELAST'\n" + "\n" + "        )";

	public static String getAllDivisionDept = "SELECT c.部门ID,d.部门名称 FROM xmis.org_部门归属公司表 c ,xmis.org_部门 d WHERE c.部门ID=d.部门ID and 公司ID=? AND c.是否直属=1 AND (d.禁用=0 OR d.禁用 IS null)";

	public static String isHasExistAttendanceWarnInfo = "SELECT count(message_id) FROM   xmis.sys_消息定义表  WHERE  message_info = ?  AND from_time >= ?";

	// public static String getMissAttendanceInfos=
	// "SELECT attd.考勤单ID ,是否是代理,attd.申请人ID,attd.申请日期,info.ID as id,Convert(varchar(10),info.缺勤日期,23) AS missDate,info.上下午 AS amorpm,info.事由 AS missDesc,info.所属考勤单ID,Convert(varchar(10),info.开始日期,23) AS startDate ,Convert(varchar(10),info.结束日期,23) as stopDate\n"
	// +
	// "FROM   \n" +
	// "       xmis.attd_补报考勤单表 attd ,\n" +
	// "       xmis.attd_补报考勤内容表 info\n" +
	// "WHERE  attd.考勤单ID = info.所属考勤单ID\n" +
	// "       AND attd.考勤单ID = ?";
	public static String getMissAttendanceInfos = "SELECT attd.考勤单ID ,是否是代理,attd.申请人ID,attd.申请日期,info.ID as id,Convert(varchar(10),info.缺勤日期,23) AS missDate,info.上下午 AS amorpm,info.事由 AS missDesc,info.所属考勤单ID,Convert(varchar(10),info.开始日期,23) AS startDate ,Convert(varchar(10),info.结束日期,23) as stopDate\n"
			+ "FROM   xmis.wf_历史 history ,\n"
			+ "       xmis.attd_补报考勤单表 attd ,\n"
			+ "       xmis.attd_补报考勤内容表 info\n"
			+ "WHERE  history.流程实例ID = attd.流程实例ID\n"
			+ "       AND attd.考勤单ID = info.所属考勤单ID\n"
			+ "       AND history.id = ?";

	public static String isExistMissAttendanceRecord = "SELECT COUNT(id) FROM  xmis.attd_考勤月记录表 WHERE infoID=? AND 年份=? AND 月份=?";

	public static String doUpdateMissAttendanceRecordState = "UPDATE  xmis.attd_考勤月记录表 SET 是否被处理=? ,处理时间=?  WHERE infoID=? AND 年份=? AND 月份=?";

	public static String doCreateMissAttendanceRecord = " INSERT INTO xmis.attd_考勤月记录表(infoID,年份,月份,是否被处理,处理时间)  VALUES(?,?,?,?,?)";

	public static String getApprovalInfos = "SELECT  task.DISPLAY_NAME AS 任务名称,\n"
			+ "        RTRIM(hr.姓氏) + RTRIM(hr.名字) AS 审批人 ,\n"
			+ "        appr.是否通过 ,\n"
			+ "        appr.审批意见 ,\n"
			+ "        appr.审批时间 ,\n"
			+ "        appr.审批人ID ,\n"
			+ "        appr.workItemID ,\n"
			+ "        appr.ID,\n"
			+ "        item.ACTOR_ID AS actorID,\n"
			+ "        RTRIM(actor.姓氏) + RTRIM(actor.名字) AS actorName\n"
			+ "FROM    xmis.wf_审批表 appr ,\n"
			+ "        xmis.T_FF_RT_TASKINSTANCE task ,\n"
			+ "        xmis.T_FF_RT_WORKITEM item ,\n"
			+ "        xmis.hr_人员 hr,\n"
			+ "        xmis.hr_人员 actor\n"
			+ "WHERE   item.TASKINSTANCE_ID = task.ID\n"
			+ "        AND item.ID = appr.workItemID\n"
			+ "        AND appr.审批人ID = hr.人员ID\n"
			+ "        AND item.ACTOR_ID = actor.人员ID\n"
			+ "        AND task.PROCESSINSTANCE_ID = ?\n" + "ORDER BY 审批时间";

	public static String getApprovalTaskList = "--\"考勤任务\"批量\n"
			+ "SELECT  \n"
			+ "        a.task_ID ,\n"
			+ "        c.ID AS workItemId ,\n"
			+ "        a.DISPLAY_NAME ,\n"
			+ "        todo.标题 ,\n"
			+ "        todo.id AS todoId ,\n"
			+ "        todo.发送人 AS 发送人ID ,\n"
			+ "        pd.部门ID AS 发送人部门ID ,\n"
			+ "        d.部门名称 AS 发送人部门名称 ,\n"
			+ "        todo.用户ID ,\n"
			+ "        RTRIM(pp.姓氏) + RTRIM(pp.名字) AS 发送人 ,\n"
			+ "        RTRIM(p.姓氏) + RTRIM(P.名字) AS 接收人 ,\n"
			+ "        a.STATE AS 状态 ,\n"
			+ "        a.PROCESSINSTANCE_ID ,\n"
			+ "        c.COMMENTS ,\n"
			+ "        CONVERT(VARCHAR(50), a.CREATED_TIME, 21) AS 创建时间\n"
			+ "INTO    #todoinfo\n"
			+ "FROM    xmis.T_FF_RT_TASKINSTANCE a ,\n"
			+ "        xmis.wf_待办 todo ,\n"
			+ "        xmis.T_FF_RT_WORKITEM c ,\n"
			+ "        xmis.hr_人员 p ,\n"
			+ "        xmis.hr_人员 pp ,\n"
			+ "        xmis.hr_人员部门关系表 pd ,\n"
			+ "        xmis.org_部门 d\n"
			+ "WHERE   a.id = c.TASKINSTANCE_ID\n"
			+ "        AND todo.应用ID = c.ID\n"
			+ "        AND todo.用户ID *= p.人员ID\n"
			+ "        AND todo.发送人 = pp.人员ID\n"
			+ "        AND todo.发送人 = pd.人员ID\n"
			+ "        AND pd.部门ID = d.部门ID\n"
			+ "        AND todo.状态 = 1\n"
			+ "        AND todo.是否显示 = 1\n"
			+ "ORDER BY a.TASK_ID\n"
			+ "\n"
			+ "\n"
			+ " SELECT  info.ID ,\n"
			+ "        attd.申请人ID ,\n"
			+ "        RTRIM(applicant.姓氏) + RTRIM(applicant.名字) AS applicantname ,\n"
			+ "        dept.部门ID ,\n" + "        dept.部门名称 ,\n"
			+ "        attd.申请日期 ,\n" + "        info.缺勤日期 ,\n"
			+ "        info.开始日期 ,\n" + "        info.结束日期 ,\n"
			+ "        info.上下午 ,\n" + "        info.事由 ,\n"
			+ "        attd.操作人ID ,\n"
			+ "        RTRIM(agent.姓氏) + RTRIM(agent.名字) AS agentname ,\n"
			+ "        attd.流程实例ID ,\n" + "        attd.考勤单ID ,\n"
			+ "        attd.出差单标识\n" + "INTO    #attdinfo\n"
			+ "FROM    xmis.attd_补报考勤单表 attd ,\n"
			+ "        xmis.attd_补报考勤内容表 info ,\n"
			+ "        xmis.hr_人员 applicant ,\n"
			+ "        xmis.hr_人员 agent ,\n" + "        xmis.hr_人员部门关系表 pd ,\n"
			+ "        xmis.org_部门 dept\n"
			+ "WHERE   attd.考勤单ID = info.所属考勤单ID\n"
			+ "        AND attd.申请人ID = applicant.人员ID\n"
			+ "        AND attd.操作人ID = agent.人员ID\n"
			+ "        AND applicant.人员ID = pd.人员ID\n"
			+ "        AND pd.部门ID = dept.部门ID\n" + "\n" + "\n"
			+ "SELECT  #todoinfo.*,#attdinfo.*\n" + "into #result\n"
			+ "FROM    #attdinfo ,\n" + "        #todoinfo\n"
			+ "WHERE   #attdinfo.流程实例ID = #todoinfo.PROCESSINSTANCE_ID";

	public static String getHaveDoneHistoryListStore =

		"SELECT  history.ID ,\n"
				+ "        history.任务名称 ,\n"
				+ "        RTRIM(p.姓氏) + RTRIM(P.名字) AS 任务人 ,\n"
				+ "        history.任务人ID ,\n"
				+ "        RTRIM(agent.姓氏) + RTRIM(agent.名字) AS 代理人 ,\n"
				+ "        history.代理人ID,\n"
				+ "        history.创建时间 ,\n"
				+ "        history.签收时间 ,\n"
				+ "        history.完成时间 ,\n"
				+ "        history.备注 ,\n"
				+ "        history.状态 ,\n"
				+ "        process.state AS 任务状态 ,\n"
				+ "        CASE WHEN k.关键字 IS NULL THEN process.DISPLAY_NAME\n"
				+ "             ELSE k.关键字\n"
				+ "        END AS 流程名称 ,\n"
				+ "         process.PROCESS_ID ,\n"
				+ "        process.VERSION ,\n"
				+ "        process.ID AS processInstanceId ,\n"
				+ "        history.发送人ID ,\n"
				+ "        RTRIM(sender.姓氏) + RTRIM(sender.名字) AS 发送人 ,\n"
				+ "     history.当前执行人\n"
				+ "INTO    #list\n"
				+ "FROM    xmis.wf_历史 history ,\n"
				+ "        xmis.T_FF_RT_PROCESSINSTANCE process ,\n"
				+ "        xmis.wf_流程关键字表 k ,\n"
				+ "        xmis.hr_人员 p ,\n"
				+ "        xmis.hr_人员 sender,\n"
				+ "        xmis.hr_人员 agent\n"
				+ "WHERE   history.流程实例ID = process.ID\n"
				+ "        AND process.ID *= k.流程实例ID\n"
				+ "        AND history.任务人ID = p.人员ID\n"
				+ "         AND history.代理人ID *= agent.人员ID\n"
				+ "        AND history.发送人ID *= sender.人员ID\n"
				+ "\n"
				+ "\n"
				+ "SELECT  '任务队列的预计执行日期是' + CONVERT(VARCHAR(23), systask.执行时间, 23) AS 预计执行日期 ,\n"
				+ "        history.流程实例ID\n"
				+ "INTO    #nextItemId\n"
				+ "FROM    xmis.wf_历史 history ,\n"
				+ "        #list ,\n"
				+ "        xmis.sys_任务队列表 systask\n"
				+ "WHERE   #list.任务人ID = -10000\n"
				+ "        AND history.状态 = 1\n"
				+ "        AND #list.processInstanceId = history.流程实例ID\n"
				+ "        AND SUBSTRING(systask.参数串, 0, 33) = history.ID\n"
				+ "        AND systask.状态 = 0\n"
				+ "        AND systask.命令名字 = 'missAttendanceTask'\n"
				+ "\n"
				+ "\n"
				+ "SELECT  #list.ID ,\n"
				+ "        #list.任务名称 ,\n"
				+ "        #list.任务人 ,\n"
				+ "        #list.任务人ID ,\n"
				+ "        #list.代理人,\n"
				+ "        #list.代理人ID,\n"
				+ "        #list.创建时间 ,\n"
				+ "        #list.签收时间 ,\n"
				+ "        #list.完成时间 ,\n"
				+ "        #list.状态 ,\n"
				+ "        #list.任务状态 ,\n"
				+ "CASE WHEN #nextItemId.预计执行日期 IS NULL THEN #list.备注\n"
				+ "     ELSE ( CASE WHEN #list.备注 IS NULL\n"
				+ "                      OR RTRIM(#list.备注) = '' THEN #nextItemId.预计执行日期\n"
				+ "                 ELSE #list.备注 + '；' + #nextItemId.预计执行日期\n"
				+ "            END )\n" + "        END AS 备注 ,\n"
				+ "        #list.流程名称 , #list.PROCESS_ID,\n"
				+ "        #list.VERSION ,\n"
				+ "        #list.processInstanceId ,\n" + "        #list.发送人ID ,\n"
				+ "        #list.发送人 ,\n" + "        #list.当前执行人\n"
				+ "INTO    #result\n" + "FROM    #list ,\n"
				+ "        #nextItemId\n"
				+ "WHERE   #list.processInstanceId*=#nextItemId.流程实例ID "
			 +	"and #list.完成时间 in (SELECT  MAX(#list.完成时间) FROM #list GROUP BY #list.processInstanceId,#list.任务人ID) "
				;


	public static String getActorsEmailByActorId = "SELECT RTRIM(p.电子邮件) AS email\n"
			+ "FROM   \n" + "       xmis.hr_人员 p\n" + "WHERE  p.人员ID= ?";

	public static String isWaitByWorkItemId = "SELECT COUNT(ID)FROM xmis.sys_任务队列表 systask WHERE SUBSTRING(systask.参数串, 0, 33) = ? AND systask.状态=0 ";

	public static String getContentByWorkItemId = "SELECT '任务队列的预计执行日期是' + CONVERT(VARCHAR(23), systask.执行时间, 23) AS 预计执行日期  FROM xmis.sys_任务队列表 systask WHERE SUBSTRING(systask.参数串, 0, 33) =? AND systask.命令名字 = 'missAttendanceTask'  AND systask.状态=0 ";

	public static String getSendMessageAfterProcessInstance =

	"SELECT  k.关键字 ,\n" + "        RTRIM(hr.电子邮件) AS 电子邮件\n"
			+ "FROM    xmis.T_FF_RT_PROCESSINSTANCE process ,\n"
			+ "        xmis.wf_流程关键字表 k ,\n" + "        xmis.hr_人员 hr\n"
			+ "WHERE   process.ID = k.流程实例ID\n"
			+ "        AND process.CREATOR_ID = hr.人员ID\n"
			+ "        AND process.ID = ?";

	public static String updateLastActorToNull = "UPDATE   XMIS.wf_历史 SET  当前执行人 = null where 流程实例ID=?";

	public static String getTodoListSQL =

	"SELECT  todo.ID AS id ,\n"
			+ "        todo.模块 AS model ,\n"
			+ "        todo.应用ID AS appId ,\n"
			+ "        todo.用户ID AS userID ,\n"
			+ "CASE WHEN xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
			+ "                                                   task.PROCESS_ID,GETDATE()) IS NULL\n"
			+ "     THEN RTRIM(actor.姓氏) + RTRIM(actor.名字)\n"
			+ "     ELSE RTRIM(actor.姓氏) + RTRIM(actor.名字) + '('\n"
			+ "          + xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
			+ "                                                     task.PROCESS_ID,GETDATE())\n"
			+ "          + '代理)'\n"
			+ "END AS actorName ,\n"
			+ "CASE WHEN xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
			+ "                                                   task.PROCESS_ID,GETDATE()) IS NULL\n"
			+ "     THEN todo.标题\n"
			+ "     ELSE todo.标题 + '('\n"
			+ "          + xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
			+ "                                                     task.PROCESS_ID,GETDATE())\n"
			+ "          + '代理)'\n"
			+ "END AS text ,\n"
			+ "        todo.链接地址 AS url ,\n"
			+ "        todo.脚本 AS script ,\n"
			+ "        todo.优先级 AS priority ,\n"
			+ "        todo.创建时间 AS createTime ,\n"
			+ "        todo.状态 AS state ,\n"
			+ "        todo.描述 AS description ,\n"
			+ "        todo.发送人 AS senderID ,\n"
			+ "        RTRIM(sender.姓氏) + RTRIM(sender.名字) AS senderName ,\n"
			+ "        todo.发送时间 AS sendTime ,\n"
			+ "        todo.是否显示 AS isVisible\n"
			+ "INTO    #result\n"
			+ "FROM    xmis.wf_待办 todo ,\n"
			+ "        xmis.hr_人员 sender ,\n"
			+ "        xmis.hr_人员 actor,\n"
			+ "        xmis.T_FF_RT_WORKITEM item,\n"
			+ "        xmis.T_FF_RT_TASKINSTANCE task\n"
			+ "WHERE   todo.是否显示 = 1\n"
			+ "        AND todo.发送人*=sender.人员ID\n"
			+ "        AND todo.用户ID = actor.人员ID\n"
			+ "        AND todo.应用ID = item.ID\n"
			+ "        AND item.TASKINSTANCE_ID = task.ID\n"
			+ "        AND todo.用户ID = ?\n"
			+

			"UNION\n"
			+ "SELECT  todo.ID AS id ,\n"
			+ "        todo.模块 AS model ,\n"
			+ "        todo.应用ID AS appId ,\n"
			+ "        todo.用户ID AS userID ,\n"
			+ "         RTRIM(actor.姓氏) + RTRIM(actor.名字)+'('+RTRIM(shouquan.姓氏) + RTRIM(shouquan.名字)+'代理)' AS actorName ,\n"
			+ "        todo.标题+'('+RTRIM(shouquan.姓氏) + RTRIM(shouquan.名字)+'代理)' AS text ,\n"
			+ "        todo.链接地址 AS url ,\n" + "        todo.脚本 AS script ,\n"
			+ "        todo.优先级 AS priority ,\n"
			+ "        todo.创建时间 AS createTime ,\n"
			+ "        todo.状态 AS state ,\n"
			+ "        todo.描述 AS description ,\n"
			+ "        todo.发送人 AS senderID ,\n"
			+ "        RTRIM(sender.姓氏) + RTRIM(sender.名字) AS senderName ,\n"
			+ "        todo.发送时间 AS sendTime ,\n"
			+ "        todo.是否显示 AS isVisible\n"
			+ "FROM    xmis.wf_待办 todo ,\n" + "        xmis.wf_授权记录表 b ,\n"
			+ "        xmis.T_FF_RT_TASKINSTANCE task ,\n"
			+ "        xmis.T_FF_RT_WORKITEM item ,\n"
			+ "        xmis.hr_人员 sender,\n" + "        xmis.hr_人员 actor,\n"
			+ "        xmis.hr_人员 shouquan\n" + "WHERE   todo.用户ID = b.授权人ID\n"
			+ "        AND todo.发送人*=sender.人员ID\n"
			+ "        AND task.ID = item.TASKINSTANCE_ID\n"
			+ "        AND item.ID = todo.应用ID\n"
			+ "        AND todo.是否显示 = 1\n"
			+ "        AND task.PROCESS_ID = b.process_ID\n"
			+ "        AND GETDATE() >= b.生效时间\n"
			+ "        AND b.撤销时间 IS NULL\n"
			+ "        AND todo.用户ID=actor.人员ID\n"
			+ "        AND b.被授权人ID =shouquan.人员ID\n"
			+ "        AND b.被授权人ID = ?";

	// bbz mysql
	// "SELECT\n"
	// + "    todo.ID AS id,\n"
	// + "    todo.模块 AS model,\n"
	// + "    todo.应用ID AS appId,\n"
	// + "    todo.用户ID AS userID,\n"
	// + "    CASE\n"
	// +
	// "        WHEN xmis. f_getAuthorizeeByUserIDAndProcessID (item.ACTOR_ID, task.PROCESS_ID, GETDATE()) IS NULL THEN RTRIM(actor.姓氏) + RTRIM(actor.名字)\n"
	// +
	// "        ELSE RTRIM(actor.姓氏) + RTRIM(actor.名字) + '(' + xmis. f_getAuthorizeeByUserIDAndProcessID (item.ACTOR_ID, task.PROCESS_ID, GETDATE()) + '代理)'\n"
	// + "    END AS actorName,\n"
	// + "    CASE\n"
	// +
	// "        WHEN xmis. f_getAuthorizeeByUserIDAndProcessID (item.ACTOR_ID, task.PROCESS_ID, GETDATE()) IS NULL THEN todo.标题\n"
	// +
	// "        ELSE todo.标题 + '(' + xmis. f_getAuthorizeeByUserIDAndProcessID (item.ACTOR_ID, task.PROCESS_ID, GETDATE()) + '代理)'\n"
	// + "    END AS text,\n"
	// + "    todo.链接地址 AS url,\n"
	// + "    todo.脚本 AS script,\n"
	// + "    todo.优先级 AS priority,\n"
	// + "    todo.创建时间 AS createTime,\n"
	// + "    todo.状态 AS state,\n"
	// + "    todo.描述 AS description,\n"
	// + "    todo.发送人 AS senderID,\n"
	// + "    RTRIM(sender.姓氏) + RTRIM(sender.名字) AS senderName,\n"
	// + "    todo.发送时间 AS sendTime,\n"
	// + "    todo.是否显示 AS isVisible\n"
	// + "FROM\n"
	// + "    xmis.wf_待办 todo\n"
	// + "        left join\n"
	// + "    xmis.hr_人员 sender ON todo.发送人 = sender.人员ID\n"
	// + "        join\n"
	// + "    xmis.hr_人员 actor ON todo.用户ID = actor.人员ID\n"
	// + "        join\n"
	// + "    xmis.T_FF_RT_WORKITEM item ON todo.应用ID = item.ID\n"
	// + "        join\n"
	// +
	// "    xmis.T_FF_RT_TASKINSTANCE task ON item.TASKINSTANCE_ID = task.ID\n"
	// + "WHERE\n"
	// + "    todo.是否显示 = 1\n"
	// + "        AND todo.用户ID = ?\n"
	// + "union SELECT\n"
	// + "    todo.ID AS id,\n"
	// + "    todo.模块 AS model,\n"
	// + "    todo.应用ID AS appId,\n"
	// + "    todo.用户ID AS userID,\n"
	// +
	// "    RTRIM(actor.姓氏) + RTRIM(actor.名字) + '(' + RTRIM(shouquan.姓氏) + RTRIM(shouquan.名字) + '代理)' AS actorName,\n"
	// +
	// "    todo.标题 + '(' + RTRIM(shouquan.姓氏) + RTRIM(shouquan.名字) + '代理)' AS text,\n"
	// + "    todo.链接地址 AS url,\n"
	// + "    todo.脚本 AS script,\n"
	// + "    todo.优先级 AS priority,\n"
	// + "    todo.创建时间 AS createTime,\n"
	// + "    todo.状态 AS state,\n"
	// + "    todo.描述 AS description,\n"
	// + "    todo.发送人 AS senderID,\n"
	// + "    RTRIM(sender.姓氏) + RTRIM(sender.名字) AS senderName,\n"
	// + "    todo.发送时间 AS sendTime,\n"
	// + "    todo.是否显示 AS isVisible\n"
	// + "FROM\n"
	// + "    xmis.wf_待办 todo\n"
	// + "        join\n"
	// + "    xmis.wf_授权记录表 b ON todo.用户ID = b.授权人ID\n"
	// + "        left join\n"
	// + "    xmis.hr_人员 sender ON todo.发送人 = sender.人员ID\n"
	// + "        join\n"
	// + "    xmis.T_FF_RT_WORKITEM item ON item.ID = todo.应用ID\n"
	// + "        join\n"
	// +
	// "    xmis.T_FF_RT_TASKINSTANCE task ON task.ID = item.TASKINSTANCE_ID\n"
	// + "        and task.PROCESS_ID = b.process_ID\n" + "        join\n"
	// + "    xmis.hr_人员 actor ON todo.用户ID = actor.人员ID\n"
	// + "        join\n"
	// + "    xmis.hr_人员 shouquan ON b.被授权人ID = shouquan.人员ID\n"
	// + "WHERE\n" + "    todo.是否显示 = 1\n"
	// + "        AND GETDATE() >= b.生效时间\n" + "        AND b.撤销时间 IS NULL\n"
	// + "        AND b.被授权人ID = ?\n";
	//
	public static String getAuthorizeList = "\n" + "SELECT  list.ID ,\n"
			+ "        list.process_ID ,\n"
			+ "        w.DISPLAY_NAME AS processName,\n"
			+ "        list.授权人ID  AS authorizerID,\n"
			+ "        RTRIM(f.姓氏) + RTRIM(f.名字)  AS authorizerName,\n"
			+ "        list.被授权人ID  AS authorizeeID,\n"
			+ "        RTRIM(t.姓氏) + RTRIM(t.名字)  AS authorizeeName ,\n"
			+ "        CONVERT(VARCHAR,list.创建时间,120)   AS createDate,\n"
			+ "        CONVERT(VARCHAR(11),list.生效时间,23)   AS availableDate,\n"
			+ "        CONVERT(VARCHAR(11),list.撤销时间,23)   AS cancelDate,\n"
			+ "        list.授权原因 AS results,\n"
			+ "        list.备注 AS comment\n"
			+ "        ,CASE WHEN 撤销时间 IS NULL  THEN 0 ELSE 1 end AS flag\n"
			+ "        into #result\n" + "FROM    xmis.wf_授权记录表 list ,\n"
			+ "        xmis.hr_人员 f ,\n" + "        xmis.hr_人员 t,\n"
			+ "        xmis.T_FF_DF_WORKFLOWDEF w\n"
			+ "WHERE   list.授权人ID = f.人员ID\n"
			+ "        AND list.被授权人ID = t.人员ID\n"
			+ "        AND list.process_ID=w.PROCESS_ID\n"
			+ "		 AND w.VERSION=1\n";

	public static String getCurrentActor = "SELECT 当前执行人 as name FROM xmis.wf_历史 WHERE 流程实例ID=?";

	public static String doCancelAuthorizeById = " UPDATE xmis.wf_授权记录表 SET 撤销时间=GETDATE()  WHERE id=?";

	public static String isValidateAuthorizeSQL = "SELECT  COUNT(1)\n"
			+ "FROM    xmis.wf_授权记录表\n" + "WHERE   撤销时间 IS NULL\n"
			+ "AND ID<>?\n" + "AND process_ID = ?\n"
			+ "        AND ( 授权人ID = ?\n" + "              OR 被授权人ID = ?\n"
			+ "              OR 授权人ID = ?\n" + "            )";

	public static String getOutContractDelimiterStore = "select name as delimiter from xmis.sys_合同资质管理字典表 where type = -1";

	public static String getRelationAddressCombo = "SELECT  地址ID\n"
			+ "INTO    #add\n"
			+ "FROM    xmis.org_公司\n"
			+ "WHERE   公司ID = ?\n"
			+ "UNION ALL\n"
			+ "SELECT  地址ID\n"
			+ "FROM    xmis.org_公司相关地址表\n"
			+ "WHERE   公司ID = ?\n"
			+ "\n"
			+ "\n"
			+ "SELECT  CASE WHEN pr.省名称 = city.城市名称 THEN city.城市名称 + adr.区县 + adr.详细地址\n"
			+ "             ELSE ( pr.省名称 + city.城市名称 + adr.区县 + adr.详细地址 )\n"
			+ "        END AS NAME,adr.地址ID AS id\n" + "FROM    #add ,\n"
			+ "        xmis.add_地址 adr ,\n" + "        xmis.add_城市 city ,\n"
			+ "        xmis.add_省 pr ,\n" + "        xmis.add_地区 area ,\n"
			+ "        xmis.add_国家及地区 country ,\n"
			+ "        xmis.add_洲际表 internation\n"
			+ "WHERE   #add.地址ID = adr.地址ID\n"
			+ "        AND adr.城市ID = city.城市ID\n"
			+ "        AND city.省ID = pr.省ID\n"
			+ "        AND pr.地区ID = area.地区ID\n"
			+ "        AND area.国家ID = country.国家ID\n"
			+ "        AND country.洲际ID = internation.洲际ID";

	public static String getWareHouseCombo = "SELECT w.库房ID AS id,w.库房名称 AS name,w.管理人ID,RTRIM(h.姓氏)+RTRIM(h.名字) AS personname FROM xmis.dev_库房 w ,xmis.hr_人员 h WHERE w.管理人ID*=h.人员ID AND  w.地址ID=?";
	public static String getShelfCombo = "SELECT 货架ID AS id,货架编号 AS name FROM xmis.dev_库房货架 WHERE 库房ID=?";
	public static String getPositionCombo = "SELECT 位置ID AS id,位置编号 AS name FROM xmis.dev_库存位置 WHERE 货架ID=?";

	public static String getPorjectDeviceInfoList = "\n"
			+ " declare @projectID varchar(50)\n"
			+ " set @projectID = ?  \n"
			+ "SELECT DISTINCT\n"
			+ "        w.物资名称 AS 设备名称 ,\n"
			+ "        w.物资ID ,\n"
			+ "        w.型号 ,\n"
			+ "        w.规格 ,\n"
			+ "        w.重量 ,\n"
			+ "        w.重量单位 ,\n"
			+ "        w.官方报价 AS 参考价格 ,\n"
			+ "        m.货币符号 ,\n"
			+ "        c.项目物资ID ,\n"
			+ "        w.最后修改人ID ,\n"
			+ "        RTRIM(h.姓氏) + RTRIM(h.名字) AS 最后修改人 ,\n"
			+ "        CONVERT(VARCHAR(11), w.最后修改日期, 23) AS 最后修改日期 ,\n"
			+ "        con.项目ID ,\n"
			+ "        xmis.f_getProjectDeviceState(con.项目ID, w.物资ID) AS state\n"
			+ "FROM    xmis.dev_物资信息库 w ,\n"
			+ "        xmis.dev_采购清单明细 c ,\n"
			+ "        xmis.con_收入支出合同对应表 s ,\n"
			+ "        xmis.con_合同 con ,\n"
			+ "        xmis.sys_货币 m ,\n"
			+ "        xmis.hr_人员 h\n"
			+ "WHERE   c.项目物资ID = w.物资ID\n"
			+ "        AND c.支出合同ID = s.支出子合同ID\n"
			+ "        AND s.收入合同ID = con.合同ID\n"
			+ "        AND w.货币ID = m.货币ID\n"
			+ "        AND w.最后修改人ID = h.人员ID\n"
			+ "        AND con.项目ID = @projectID\n"
			+ "UNION\n"
			+ "SELECT DISTINCT\n"
			+ "        w.物资名称 AS 设备名称 ,\n"
			+ "        w.物资ID ,\n"
			+ "        w.型号 ,\n"
			+ "        w.规格 ,\n"
			+ "        w.重量 ,\n"
			+ "        w.重量单位 ,\n"
			+ "        w.官方报价 AS 参考价格 ,\n"
			+ "        m.货币符号 ,\n"
			+ "        d.物资ID AS 项目物资ID,\n"
			+ "        w.最后修改人ID ,\n"
			+ "        RTRIM(h.姓氏) + RTRIM(h.名字) AS 最后修改人 ,\n"
			+ "        CONVERT(VARCHAR(11), w.最后修改日期, 23) AS 最后修改日期 ,\n"
			+ "        con.项目ID ,\n"
			+ "        xmis.f_getProjectDeviceState(con.项目ID, w.物资ID) AS state\n"
			+ "FROM    xmis.dev_物资信息库 w ,\n" + "        xmis.dev_到货清单 d ,\n"
			+ "        xmis.con_收入支出合同对应表 s ,\n"
			+ "        xmis.con_合同 con ,\n" + "        xmis.sys_货币 m ,\n"
			+ "        xmis.hr_人员 h\n" + "WHERE   d.物资ID = w.物资ID\n"
			+ "        AND d.收入合同ID = con.项目ID\n"
			+ "        AND s.收入合同ID = con.合同ID\n"
			+ "        AND w.货币ID = m.货币ID\n"
			+ "        AND w.最后修改人ID = h.人员ID\n"
			+ "        AND con.项目ID = @projectID\n" + "";

	public static String getDeviceInList = "SELECT\n"
			+ "DISTINCT\n"
			+ "        w.物资ID ,\n"
			+ "        con.项目ID ,\n"
			+ "        c.清单ID AS 采购清单ID ,\n"
			+ "        outcon.合同ID ,\n"
			+ "        outcon.合同编号 ,\n"
			+ "        xmis.f_getProjectDeviceArrivalState(con.项目ID, w.物资ID) AS 已到货数量 ,\n"
			+ "        xmis.f_getProjectDevicePurchaseState(con.项目ID, w.物资ID) AS 已采购数量 ,\n"
			+ "        xmis.f_getProjectDevicePurchaseState(con.项目ID, w.物资ID)-xmis.f_getProjectDeviceArrivalState(con.项目ID, w.物资ID) AS 未到货数量 ,\n"
			+ "        w.物资名称 AS 设备名称\n" + "        into #result \n"
			+ "FROM    xmis.dev_物资信息库 w ,\n" + "        xmis.dev_采购清单明细 c ,\n"
			+ "        xmis.con_收入支出合同对应表 s ,\n"
			+ "        xmis.con_合同 con ,\n" + "        xmis.con_合同 outcon\n"
			+ "WHERE   w.物资ID=c.项目物资ID\n"
			+ "        AND c.支出合同ID = s.支出子合同ID\n"
			+ "        AND s.收入合同ID = con.合同ID\n"
			+ "        AND c.支出合同ID = outcon.合同ID";

	public static String doAddWareHouse = "INSERT INTO xmis.dev_库房\n"
			+ "        ( 库房ID ,\n" + "          库房名称 ,\n"
			+ "          地址ID ,\n" + "          管理人ID ,\n"
			+ "          最后修改人ID ,\n" + "          最后修改时间\n" + "        )\n"
			+ "VALUES  ( ? , -- 库房ID - varchar(32)\n"
			+ "          ? , -- 库房名称 - varchar(50)\n"
			+ "          ? , -- 地址ID - varchar(32)\n"
			+ "          ?, -- 管理人ID - int\n"
			+ "          ?, -- 最后修改人ID - int\n" + "          GETDATE()\n"
			+ "            -- 最后修改时间 - datetime\n" + "        )";

	public static String doAddShelf = "INSERT INTO xmis.dev_库房货架\n"
			+ "        ( 货架ID ,\n" + "          货架编号 ,\n"
			+ "          货架描述 ,\n" + "          库房ID ,\n"
			+ "          最后修改人ID ,\n" + "          最后修改时间\n" + "        )\n"
			+ "VALUES  ( ? , -- 货架ID - varchar(50)\n"
			+ "          ? , -- 货架编号 - varchar(50)\n"
			+ "          ? , -- 货架描述 - varchar(100)\n"
			+ "          ? , -- 库房ID - varchar(32)\n"
			+ "          ? , -- 最后修改人ID - int\n"
			+ "          GETDATE()  -- 最后修改时间 - datetime\n" + "        )";

	public static String doAddPositon = "INSERT INTO xmis.dev_库存位置\n"
			+ "        ( 位置ID ,\n" + "          位置编号 ,\n"
			+ "          位置描述 ,\n" + "          货架ID ,\n"
			+ "          最后修改人ID ,\n" + "          最后修改时间\n" + "        )\n"
			+ "VALUES  ( ? , -- 位置ID - varchar(32)\n"
			+ "          ? , -- 位置编号 - varchar(50)\n"
			+ "          ? , -- 位置描述 - varchar(100)\n"
			+ "          ? , -- 货架ID - varchar(32)\n"
			+ "          ? , -- 最后修改人ID - int\n"
			+ "          GETDATE()  -- 最后修改时间 - datetime\n" + "        )";

	public static String getArrivalDeviceInfos = "SELECT d.ID ,\n"
			+ "       s.支出合同ID ,\n" + "       d.到货数量 ,\n"
			+ "       CONVERT(VARCHAR(11), d.到货日期, 23) AS 到货日期 ,\n"
			+ "       d.登记人ID ,\n" + "       d.登记日期\n"
			+ "FROM   xmis.dev_到货清单 d ,\n" + "       xmis.con_收入支出合同对应表 s\n"
			+ "WHERE  d.支出合同ID = s.支出子合同ID\n" + "       AND d.支出合同ID = ?\n"
			+ "       AND d.物资ID = ?";

	public static String getDeviceInfoList = "SELECT w.物资ID,w.物资名称,w.型号,w.规格 into #result FROM xmis.dev_物资信息库 w ";
	public static String getDeviceInfoListWithPurchaseList =

	"SELECT  w.物资ID ,\n" + "        w.物资名称 ,\n"
			+ "        list.ID AS 采购清单ID ,\n" + "        SUM(info.数量) AS num\n"
			+ "FROM    xmis.dev_采购清单 list ,\n"
			+ "        xmis.dev_采购清单明细 info ,\n" + "        xmis.dev_物资信息库 w\n"
			+ "WHERE   list.ID = info.清单ID\n"
			+ "        AND info.项目物资ID = w.物资ID\n"
			+ "        AND info.支出合同ID = ?\n" + "GROUP BY w.物资ID ,\n"
			+ "        w.物资名称 ,\n" + "        w.型号 ,\n" + "        list.ID ,\n"
			+ "        w.规格";

	public static String getContractCodeAndPurchaseIDByContarctID =

	"SELECT  c.合同编号 AS contractCode ,\n" + "        c.合同ID contractID ,\n"
			+ "        info.ID AS infoID,\n" + "        p.项目ID AS  收入合同ID,\n"
			+ "        p.项目编码,p.项目名称\n" + "FROM    xmis.con_合同 c ,\n"
			+ "        xmis.con_收入支出合同对应表 d ,\n"
			+ "        xmis.dev_采购清单 info,\n" + "        xmis.con_合同 pcon,\n"
			+ "        xmis.prj_项目 p\n" + "WHERE   d.支出子合同ID = c.合同ID\n"
			+ "        AND d.支出合同ID*=info.支出合同ID\n"
			+ "        AND d.收入合同ID=pcon.合同ID\n"
			+ "        AND pcon.项目ID=p.项目ID\n" + "        AND d.支出合同ID = ?";

	public static String getDeviceInListByContractId =

	"SELECT  w.物资ID ,\n"
			+ "        d.id ,\n"
			+ "        w.物资名称 ,\n"
			+ "        c.合同编号 AS 支出合同编号 ,\n"
			+ "        d.收入合同ID ,\n"
			+ "        d.支出合同ID ,\n"
			+ "        s.支出合同ID AS 合同ID ,\n"
			+ "        d.采购清单ID ,\n"
			+ "        p.项目编码 AS 收入合同编号 ,\n"
			+ "        p.执行部门 AS execu_id,\n"
			+ "        d.到货数量 ,\n"
			+ "        CONVERT(VARCHAR(11), d.到货日期, 23) AS 到货日期 ,\n"
			+ "        CONVERT(VARCHAR(11), d.登记日期, 23) AS 登记日期 ,\n"
			+ "        CONVERT(VARCHAR(11), d.出厂日期, 23) AS 出厂日期 ,\n"
			+ "        RTRIM(register.姓氏) + RTRIM(register.名字) AS registerName  ,\n"
			+ "        d.登记人ID ,\n"
			+ "        d.原厂保修天数 ,\n"
			+ "        d.原厂可退天数 ,\n"
			+ "        d.原厂可换天数 ,\n"
			+ "        CONVERT(VARCHAR(11), DATEADD(DAY, d.原厂保修天数, d.出厂日期), 23) AS 保修期限 ,\n"
			+ "        CONVERT(VARCHAR(11), DATEADD(DAY, d.原厂可退天数, d.出厂日期), 23) AS 可退期限 ,\n"
			+ "        CONVERT(VARCHAR(11), DATEADD(DAY, d.原厂可换天数, d.出厂日期), 23) AS 可换期限\n"
			+ "INTO    #result\n" + "FROM    xmis.dev_到货清单 d ,\n"
			+ "        xmis.dev_物资信息库 w ,\n" + "        xmis.con_合同 c ,\n"
			+ "        xmis.prj_项目 p ,\n" + "        xmis.con_收入支出合同对应表 s ,\n"
			+ "        xmis.hr_人员 register ,\n" + "        xmis.dev_领用级别 ut\n"
			+ "WHERE   d.支出合同ID *= c.合同ID\n" + "        AND d.物资ID = w.物资ID\n"
			+ "        AND d.领用级别ID *= ut.领用级别ID\n"
			+ "        AND d.收入合同ID = p.项目ID\n"
			+ "        AND d.支出合同ID *= s.支出子合同ID\n"
			+ "        AND d.登记人ID=register.人员ID\n" + "ORDER BY c.合同编号 ASC ,\n"
			+ "        w.物资名称 ASC\n" + "\n" + "";

	public static String getDeviceInfoObjById = "SELECT  d.ID AS id ,\n"
			+ "        d.收入合同ID AS projectId ,\n"
			+ "        p.项目编码  AS projectCode ,\n"
			+ "         p.项目名称 AS projectName ,\n"
			+ "        d.支出合同ID AS outContractId ,\n"
			+ "        con.合同编号  AS outContract ,\n"
			+ "        d.采购清单ID AS purchasingListId ,\n"
			+ "        d.物资ID AS deviceInfoId ,\n"
			+ "        w.物资名称 AS deviceInfoName ,\n"
			+ "        d.到货数量 AS inNum ,\n"
			+ "        CONVERT(VARCHAR(11), d.到货日期, 23) AS inDate ,\n"
			+ "        d.领用级别ID AS useStypeId ,\n"
			+ "        d.出厂日期 AS outFactoryDate ,\n"
			+ "        d.原厂可退天数 AS backDays ,\n"
			+ "        d.原厂可换天数 AS changeDays ,\n"
			+ "        d.原厂保修天数 AS repairDays ,\n"
			+ "        CONVERT(VARCHAR(11), DATEADD(DAY, d.原厂保修天数, d.出厂日期), 23) AS 保修期限 ,\n"
			+ "        CONVERT(VARCHAR(11), DATEADD(DAY, d.原厂可退天数, d.出厂日期), 23) AS 可退期限 ,\n"
			+ "        CONVERT(VARCHAR(11), DATEADD(DAY, d.原厂可换天数, d.出厂日期), 23) AS 可换期限 ,\n"
			+ "        d.登记人ID AS registerId ,\n"
			+ "        CONVERT(VARCHAR(11), d.登记日期, 23) AS registerDate ,\n"
			+ "        d.备注 AS remarks ,\n"
			+ "        d.最后修改人ID AS lastModifierNameId ,\n"
			+ "        CONVERT(VARCHAR(11), d.最后修改时间, 23) AS lastModified ,\n"
			+ "        RTRIM(register.姓氏) + RTRIM(register.名字) AS registerName ,\n"
			+ "        RTRIM(lastModifier.姓氏) + RTRIM(lastModifier.名字) AS lastModifierName ,\n"
			+ "        pst.位置ID as positionId ,\n"
			+ "        pst.位置编号 as positionName ,\n"
			+ "        shlf.货架ID as shelfId ,\n"
			+ "        shlf.货架编号 as shelfName ,\n"
			+ "        wh.库房ID  as wareHouseId,\n"
			+ "        wh.库房名称  as wareHouseName,\n"
			+ "        RTRIM(whperson.姓氏) + RTRIM(whperson.名字) AS whpersonname ,\n"
			+ "        addrs.地址ID as addressId,\n"
			+ "        addrs.详细地址 ,\n"
			+ "        addrs.区县 ,\n"
			+ "        city.城市ID ,\n"
			+ "        city.城市名称 ,\n"
			+ "        prv.省ID ,\n"
			+ "        prv.省名称 ,\n"
			+ "        area.地区ID ,\n"
			+ "        area.名称 AS 区域名称 ,\n"
			+ "        country.国家ID ,\n"
			+ "        country.名称 AS 国家 ,\n"
			+ "        internation.洲际ID ,\n"
			+ "        internation.名称 AS 洲际名称 ,\n"
			+ "        prv.省名称 + city.城市名称 + addrs.区县 + addrs.详细地址 AS addressname\n"
			+ "FROM    xmis.dev_到货清单 d ,\n" + "        xmis.dev_物资信息库 w ,\n"
			+ "        xmis.hr_人员 register ,\n"
			+ "        xmis.hr_人员 whperson ,\n"
			+ "        xmis.hr_人员 lastModifier ,\n"
			+ "        xmis.dev_库存位置 pst ,\n"
			+ "        xmis.dev_库房货架 shlf ,\n" + "        xmis.dev_库房 wh ,\n"
			+ "        xmis.add_地址 addrs ,\n" + "        xmis.add_城市 city ,\n"
			+ "        xmis.add_省 prv ,\n" + "        xmis.add_地区 area ,\n"
			+ "        xmis.add_国家及地区 country ,\n"
			+ "        xmis.add_洲际表 internation,\n"
			+ "        xmis.con_合同  con,\n" + "        xmis.prj_项目 p\n"
			+ "WHERE   d.物资ID = w.物资ID\n" + "        AND d.收入合同ID=p.项目ID\n"
			+ "        AND d.支出合同ID *=con.合同ID\n"
			+ "        AND d.登记人ID *= register.人员ID\n"
			+ "        AND wh.管理人ID *= whperson.人员ID\n"
			+ "        AND d.最后修改人ID *= lastModifier.人员ID\n"
			+ "        AND d.存放位置ID = pst.位置ID\n"
			+ "        AND pst.货架ID = shlf.货架ID\n"
			+ "        AND shlf.库房ID = wh.库房ID\n"
			+ "        AND wh.地址ID = addrs.地址ID\n"
			+ "        AND addrs.城市ID = city.城市ID\n"
			+ "        AND city.省ID = prv.省ID\n"
			+ "        AND prv.地区ID = area.地区ID\n"
			+ "        AND area.国家ID = country.国家ID\n"
			+ "        AND country.洲际ID = internation.洲际ID\n"
			+ "      AND d.ID=?";

	public static String getUseStypeList = " SELECT  领用级别ID AS id,领用级别 AS name FROM xmis.dev_领用级别";

	public static String getDeviceAddressList =

	"SELECT  d.收入合同ID AS 项目ID ,\n"
			+ "        w.物资ID ,\n"
			+ "        p.项目编码 ,\n"
			+ "        p.执行部门  as execu_id,\n"
			+ "        w.物资名称 ,\n"
			+ "        SUM(d.到货数量) AS 存放数量 ,\n"
			+ "        d.存放位置ID ,\n"
			+ "        '正常' AS 状态 ,\n"
			+ "        CASE WHEN prv.省名称 = city.城市名称 THEN city.城市名称 + addrs.区县 + addrs.详细地址 + wh.库房名称 + shlf.货架编号+ pst.位置编号\n"
			+ "             ELSE ( prv.省名称+ city.城市名称 + addrs.区县 + addrs.详细地址 + wh.库房名称 + shlf.货架编号+ pst.位置编号 )\n"
			+ "        END AS addressname\n"

			+

			"INTO    #result\n" + "FROM    xmis.dev_物资信息库 w ,\n"
			+ "        xmis.dev_到货清单 d ,\n" + "        xmis.prj_项目 p ,\n"
			+ "        xmis.dev_库存位置 pst ,\n"
			+ "        xmis.dev_库房货架 shlf ,\n" + "        xmis.dev_库房 wh ,\n"
			+ "        xmis.add_地址 addrs ,\n" + "        xmis.add_城市 city ,\n"
			+ "        xmis.add_省 prv ,\n" + "        xmis.add_地区 area ,\n"
			+ "        xmis.add_国家及地区 country ,\n"
			+ "        xmis.add_洲际表 internation\n"
			+ "WHERE   d.物资ID = w.物资ID\n" + "        AND d.收入合同ID = p.项目ID\n"
			+ "        AND d.存放位置ID = pst.位置ID\n"
			+ "        AND pst.货架ID = shlf.货架ID\n"
			+ "        AND shlf.库房ID = wh.库房ID\n"
			+ "        AND wh.地址ID = addrs.地址ID\n"
			+ "        AND addrs.城市ID = city.城市ID\n"
			+ "        AND city.省ID = prv.省ID\n"
			+ "        AND prv.地区ID = area.地区ID\n"
			+ "        AND area.国家ID = country.国家ID\n"
			+ "        AND country.洲际ID = internation.洲际ID\n"
			+ "GROUP BY d.收入合同ID ,\n" + "        w.物资ID ,\n"
			+ "        p.项目编码 , p.执行部门 ,\n" + "        w.物资名称 ,\n"
			+ "        d.存放位置ID ,\n" + "        prv.省名称 ,\n"
			+ "        city.城市名称 ,\n" + "        addrs.区县 ,\n"
			+ "        addrs.详细地址 ,\n" + "        wh.库房名称 ,\n"
			+ "        shlf.货架编号 ,\n" + "        pst.位置编号\n" + "";

	public static String getPurchaseSumNum = "SELECT  SUM(info.数量) AS num\n"
			+ "FROM    xmis.dev_采购清单明细 info\n" + "WHERE   info.项目物资ID =?\n"
			+ "        AND info.清单ID = ?";

	public static String getContactListByProjectIdCombo = "SELECT  s.支出子合同ID ,\n"
			+ "        c.合同编号\n"
			+ "FROM    xmis.con_收入支出合同对应表 s ,\n"
			+ "        xmis.con_合同 c,\n"
			+ "        xmis.con_合同 p\n"
			+ "\n"
			+ "WHERE   s.支出子合同ID = c.合同ID\n"
			+ "    AND s.收入合同ID=p.合同ID\n"
			+ "        AND  p.项目ID =?";
	public static String getDeviceAddressListByProjectIdAndDid废弃 = "SELECT  d.ID ,\n"
			+ "        d.到货数量,\n"
			+ "        w.物资名称 \n"
			+ "FROM    xmis.dev_到货清单 d ,\n"
			+ "        xmis.con_收入支出合同对应表 s ,\n"
			+ "        xmis.con_合同 pcon ,\n"
			+ "        xmis.dev_物资信息库 w\n"
			+ "WHERE   d.支出合同ID = s.支出子合同ID\n"
			+ "        AND s.收入合同ID = pcon.合同ID\n"
			+ "        AND d.物资ID = w.物资ID\n"
			+ "        AND pcon.项目ID = ?\n"
			+ "        AND d.物资ID = ?";

	public static String delAttachById = "DELETE  FROM xmis.sys_附件\n"
			+ "WHERE   附件ID =?";

	public static final String getImportDeviceInfoList =

	"SELECT  info.id ,\n"
			+ "        info.sheet_uuid ,\n"
			+ "        info.level_id ,\n"
			+ "        info.序号 ,\n"
			+ "        att.文件名 AS 附件名字 ,\n"
			+ "        sheet.sheet_name ,\n"
			+ "        ( att.文件名 + '．' + sheet.sheet_name ) AS groupname ,\n"
			+ "        ( att.文件名 + '．' + sheet.sheet_name + '.' + RTRIM(currency.货币名称) + '.'\n"
			+ "          + RTRIM(sheet.currency_unit) ) AS groupname2 ,\n"
			+ "        info.名称 ,\n" + "        info.规格 ,\n"
			+ "        info.数量 ,\n" + "        info.单位 ,\n"
			+ "        info.合同单价 ,\n" + "        info.合同合价 ,\n"
			+ "        info.成本单价 ,\n" + "        sheet.currency_unit ,\n"
			+ "        currency.货币名称 ,\n" + "        info.产地 ,\n"
			+ "        info.制造商 ,\n" + "        sheet.附件ID ,\n"
			+ "        att.REFID AS projectId\n" + "INTO    #result\n"
			+ "FROM    xmis.dev_设备清单导入信息 info ,\n"
			+ "        xmis.dev_设备清单导入Sheet sheet ,\n"
			+ "        xmis.sys_附件 att ,\n" + "        xmis.sys_货币 currency\n"
			+ "WHERE   info.sheet_uuid = sheet.sheet_uuid\n"
			+ "        AND sheet.附件ID *= att.附件ID\n"
			+ "        AND sheet.currency_type = currency.货币ID";

	public static final String delExistImportInfo =

	"DELETE  FROM xmis.dev_设备清单导入信息\n"
			+ "WHERE   sheet_uuid IN ( SELECT  sheet_uuid\n"
			+ "                        FROM    xmis.dev_设备清单导入Sheet\n"
			+ "                        WHERE   附件ID = ? )";

	public static final String delExistSheetInfo =

	"DELETE  FROM xmis.dev_设备清单导入Sheet\n" + "WHERE   附件ID = ?";

	public static String getApplyMeetingRoomFormById = "SELECT\n"
			+ "	f.id,\n"
			+ "	f.room_id,\n"
			+ "	f.priority,\n"
			+ "	f.sponsor_id,\n"
			+ "	sponsorperson.姓氏 AS sponsor_surname,\n"
			+ "	sponsorperson.名字 AS sponsor_name,\n"
			+ "	f.reason,\n"
			+ "	convert(varchar(50), f.app_datetime,120) as app_datetime,\n"
			+ "	convert(varchar(50), f.begin_datetime,120) as begin_datetime,\n"
			+ "	convert(varchar(50), f.end_datetime,120) as end_datetime,\n"
			+ "	f.number_people,\n"
			+ "	f.app_user_id,\n"
			+ "	appperson.姓氏 AS app_surname,\n"
			+ "	appperson.名字 AS app_name,\n"
			+ "	f.remark,\n"
			+ "	f.assigned_id,\n"
			+ "	assignedperson.姓氏 AS assigned_surname,\n"
			+ "	assignedperson.名字 AS assigned_name,\n"
			+ "	convert(varchar(50), f.assigned_datetime,120) as assigned_datetime,\n"
			+ "	f.status_id\n"
			+ "FROM\n"
			+ "	t_meetingroom_app_form f\n"
			+ "LEFT JOIN hr_人员 appperson ON f.app_user_id = appperson.人员ID\n"
			+ "LEFT JOIN hr_人员 assignedperson ON f.assigned_id = assignedperson.人员ID\n"
			+ "LEFT JOIN hr_人员 sponsorperson ON f.sponsor_id = sponsorperson.人员ID\n"
			+ "WHERE\n" + "	id = ?";

	public static String getAppMeetingRoomFormList = "SELECT\n"
			+ "	f.id,\n"
			+ "	r.room_no,\n"
			+ "	f.status_id,\n"
			+ "	statu.status_name,\n"
			+ "	f.priority,\n"
			+ "	f.reason,\n"
			+ "	f.sponsor_id,\n"
			+ "	h.姓氏 AS sponsor_surname,\n"
			+ "	h.名字 AS sponsor_name,\n"
			+ "	f.app_user_id,\n"
			+ "	f.assigned_id,\n"
			+ "	assignedperson.姓氏 AS assigned_surname,\n"
			+ "	assignedperson.名字 AS assigned_name,\n"
			+ "	convert(varchar(50), f.assigned_datetime,120) as assigned_datetime,\n"
			+ "	convert(varchar(50), f.app_datetime,120) as app_datetime,\n"
			+ "	convert(varchar(50), f.begin_datetime,120) as begin_datetime,\n"
			+ "	convert(varchar(50), f.end_datetime,120) as end_datetime\n"
			+ "FROM\n"
			+ "	t_meetingroom_app_form f\n"
			+ "LEFT JOIN t_meetingRoom_info r ON f.room_id=r.room_id \n"
			+ "LEFT JOIN hr_人员 h ON f.sponsor_id = h.人员ID\n"
			+ "LEFT JOIN hr_人员 assignedperson ON f.assigned_id = assignedperson.人员ID\n"
			+ "LEFT JOIN t_meetingroom_app_form_status statu on f.status_id=statu.status_id\n";

	public static String getMeetingRoomCombo = "select room_id,room_No from xmis.t_meetingroom_info";

	public static String doHandleAppMeetingRoomStatus = " update t_meetingroom_app_form set status_id=? where id=? ";

	public static String getMeetingRoomUsingTimeList = "select id,convert(varchar(50), f.begin_datetime,120) as begin_datetime,convert(varchar(50), f.end_datetime,120) as end_datetime from t_meetingroom_app_form where room_id=? and (status_id=3) and id<>?";

	public static String getApplyMeetingFormChangeCount =
		"/*未过期的会议室预约提醒*/\n" +
		"\n" + 
		"--预约中的会议室不存在：\n" + 
		"SELECT  6 AS id ,\n" + 
		"        '预约中的会议室不存在' AS name ,\n" + 
		"        COUNT(*) qty ,\n" + 
		"        1 AS type\n" + 
		"FROM    xmis.t_meetingroom_app_form f\n" + 
		"        LEFT JOIN t_meetingroom_info room ON f.room_id = room.room_id\n" + 
		"WHERE   f.status_id = 3\n" + 
		"        AND GETDATE() <= f.begin_datetime\n" + 
		"        AND ( room.room_No IS  NULL\n" + 
		"              OR room.room_No = ''\n" + 
		"            )\n" + 
		"UNION\n" + 
		"--新的会议室申请：\n" + 
		"SELECT  1 ,\n" + 
		"        '新的会议室申请' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 1\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		" --assigned_datetime IS null\n" + 
		"UNION\n" + 
		"--申请人申请变更已批准的会议室预约：\n" + 
		"SELECT  2 ,\n" + 
		"        '申请人申请变更已批准的会议室预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 2\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime > assigned_datetime\n" + 
		"UNION\n" + 
		"--审批人已批准（或变更）的会议室预约：\n" + 
		"SELECT  4 ,\n" + 
		"        '审批人已批准（或变更）的会议室预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 3\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime < assigned_datetime\n" + 
		"UNION\n" + 
		"--申请人申请撤销已批准的会议室预约：\n" + 
		"SELECT  3 ,\n" + 
		"        '申请人申请撤销已批准的会议室预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 4\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime > assigned_datetime\n" + 
		"UNION\n" + 
		"--审批人撤销会议室预约：\n" + 
		"SELECT  5 ,\n" + 
		"        '审批人撤销会议室预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 4\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime < assigned_datetime\n" + 
		"        /*未过期的公务车预约提醒*/\n" + 
		"\n" + 
		"--预约中的公务车不存在：\n" + 
		"\n" + 
		"union\n" + 
		"SELECT  16 AS id ,\n" + 
		"        '预约中的公务车不存在' AS name ,\n" + 
		"        COUNT(*) qty ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_carid_detail d\n" + 
		"        RIGHT OUTER JOIN t_car_app_form f ON f.form_id = d.form_id\n" + 
		"        LEFT OUTER JOIN t_car_info c ON d.car_id = c.car_id\n" + 
		"WHERE   f.status_id = 3\n" + 
		"        AND ( c.car_No IS NULL\n" + 
		"              OR c.car_No = ''\n" + 
		"            )\n" + 
		"UNION\n" + 
		"--新的公务车申请：\n" + 
		"SELECT  11 ,\n" + 
		"        '新的公务车申请' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 1\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		" --assigned_datetime IS null\n" + 
		"UNION\n" + 
		"--申请人申请变更已批准的公务车预约：\n" + 
		"SELECT  12 ,\n" + 
		"        '申请人申请变更已批准的公务车预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 2\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime > assigned_datetime\n" + 
		"UNION\n" + 
		"--审批人已批准（或变更）的公务车预约：\n" + 
		"SELECT  14 ,\n" + 
		"        '审批人已批准（或变更）的公务车预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 3\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime < assigned_datetime\n" + 
		"UNION\n" + 
		"--申请人申请撤销已批准的公务车预约：\n" + 
		"SELECT  13 ,\n" + 
		"        '申请人申请撤销已批准的公务车预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 4\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime > assigned_datetime\n" + 
		"UNION\n" + 
		"--审批人撤销公务车预约：\n" + 
		"SELECT  15 ,\n" + 
		"        '审批人撤销公务车预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 4\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime < assigned_datetime\n" + 
		"ORDER BY id\n" + 
		"\n" + 
		"";
 

	// public static String
	// getImportDeviceSheetCount="SELECT COUNT(1)+1 FROM xmis.dev_设备清单导入Sheet WHERE 附件ID=?";

	public static String getAppCarList = "SELECT\n"
			+ "	f.form_id,\n"
			+ "	c.car_type,\n"
			+ "	c.car_brand,\n"
			+ "	c.car_No,\n"
			+ "	f.status_id,\n"
			+ "	statu.status_name,\n"
			+ "	f.priority,\n"
			+ "	f.reason,\n"
			+ "	convert(varchar(50), f.begin_datetime,120) as begin_datetime,\n"
			+ "	convert(varchar(50), f.end_datetime,120) as end_datetime,\n"
			+ "	convert(varchar(50), f.app_datetime,120) as app_datetime,\n"
			+ "	f.app_user_id,\n"
			+ "	appperson.姓氏 AS app_surname,\n"
			+ "	appperson.名字 AS app_name,\n"
			+ "	f.remark,\n"
			+ "	f.assigned_id,\n"
			+ "	assignedperson.姓氏 AS assigned_surname,\n"
			+ "	assignedperson.名字 AS assigned_name\n"
			+ "FROM\n"
			+ "	t_car_app_carid_detail d\n"
			+ "RIGHT OUTER JOIN t_car_app_form f ON f.form_id = d.form_id\n"
			+ "LEFT OUTER JOIN t_car_info c ON d.car_id = c.car_id\n"
			+ "LEFT OUTER JOIN t_meetingroom_app_form_status statu ON f.status_id = statu.status_id\n"
			+ "LEFT JOIN hr_人员 appperson ON f.app_user_id = appperson.人员ID\n"
			+ "LEFT JOIN hr_人员 assignedperson ON f.assigned_id = assignedperson.人员ID";

	public static String getVehicleManagementListToSelectGrid = "select car_id,car_No,car_brand,car_type from T_Car_Info \n";

	public static String doSaveAppCarDetals = "INSERT into t_car_app_carid_detail (form_id,car_id) values (?,?)\n";

	public static String getApplyCarByFormId = "SELECT\n"
			+ "	f.form_id,\n"
			+ "	f.priority,\n"
			+ "	f.reason,\n"
			+ "	convert(varchar(50), f.begin_datetime,120) as begin_datetime,\n"
			+ "	convert(varchar(50), f.end_datetime,120) as end_datetime,\n"
			+ "	convert(varchar(50), f.app_datetime,120) as app_datetime,\n"
			+ "	f.app_user_id,\n"
			+ "	appperson.姓氏 AS app_surname,\n"
			+ "	appperson.名字 AS app_name,\n"
			+ "	f.remark,\n"
			+ "	f.assigned_id,\n"
			+ "	f.assigned_datetime,\n"
			+ "	f.status_id,\n"
			+ "	statu.status_name,\n"
			+ "	assignedperson.姓氏 AS assigned_surname,\n"
			+ "	assignedperson.名字 AS assigned_name\n"
			+ " FROM\n"
			+ "	t_car_app_form f\n"
			+ "LEFT JOIN t_meetingroom_app_form_status statu ON f.status_id = statu.status_id\n"
			+ "LEFT JOIN hr_人员 appperson ON f.app_user_id = appperson.人员ID\n"
			+ "LEFT JOIN hr_人员 assignedperson ON f.assigned_id = assignedperson.人员ID\n" 
			+ " where form_id=?";

	public static String getApplyCarIdsDetalsByFormId = "SELECT car_id FROM t_car_app_carid_detail where form_id=?";

	public static String delCarAppDetails = "DELETE from t_car_app_carid_detail  where form_id = ?";

	public static String getCarUsingTimeList = "select d.car_id,CONVERT(VARCHAR(100),f.begin_datetime,120) as begin_datetime ,CONVERT(VARCHAR(100),f.end_datetime,120) as end_datetime  from t_car_app_carid_detail d INNER JOIN t_car_app_form f on d.form_id=f.form_id where f.status_id=3 AND d.car_id=?  and d.form_id <>?\n";
	public static String getOfficeSuppliesSQL = "SELECT\n"
			+ " supplies_id as supplies_id,\n"
			+ " t1.supplies_type_id as supplies_type_id,\n"
			+ " t2.supplies_type_Name as supplies_type_name,\n"
			+ " supplies_name as supplies_name,\n"
			+ " supplies_brand as supplies_brand,\n"
			+ " supplies_model as supplies_model,\n"
			+ " CONVERT(VARCHAR,purchase_date , 120) as purchase_date,\n"
			+ " qty as qty,\n"
			+ " t1.remark as remark\n"
			+ " from\n"
			+ " T_Office_Supplies_Info t1\n"
			+ " LEFT JOIN T_office_supplies_type_info t2 ON t1.supplies_type_id = t2.supplies_type_id\n ";

	public static String getApplyOfficialSuppliesListFormId = " SELECT d.form_id ,\n"
			+ "        d.priority ,\n"
			+ "        d.supplies_id ,\n"
			+ "        info.supplies_name ,\n"
			+ "        d.reason ,\n"
			+ "        d.qty ,\n"
			+ "         CONVERT(VARCHAR(100),d.app_datetime,120) as app_datetime ,\n"
			+ "        d.app_user_id ,\n"
			+ "        RTRIM(h.姓氏)+RTRIM(h.名字) app_user_name ,\n"
			+ "        d.remark \n"
			+ " FROM   t_offices_supplies_app_form_detail d ,\n"
			+ "        hr_人员 h ,\n"
			+ "        T_Office_Supplies_Info info\n"
			+ " WHERE  d.supplies_id = info.supplies_id\n"
			+ "        AND d.app_user_id = h.人员ID\n"
			+ "        AND d.form_id = ?";

	public static String getOfficialSuppliesTypeCombo = " SELECT supplies_type_id,supplies_type_Name FROM T_Office_Supplies_Type_Info";
	public static String getOfficialSuppliesCombo = "SELECT supplies_id ,supplies_name FROM T_Office_Supplies_Info WHERE supplies_type_id = ?";

	public static String doDeleteApplyOfficialSuppliesForm = "DELETE FROM  t_offices_supplies_app_form_detail WHERE form_id=? AND supplies_id=?";

	public static String getOfficalSuppliesByCompositeIds = " SELECT d.form_id ,\n"
			+ "        d.priority ,\n"
			+ "        d.supplies_id ,\n"
			+ "        info.supplies_name ,\n"
			+ "        info.supplies_type_id,\n"
			+ "        type.supplies_type_Name,\n"
			+ "        d.reason ,\n"
			+ "        d.qty ,\n"
			+ "         CONVERT(VARCHAR(100),d.app_datetime,120) as app_datetime ,\n"
			+ "        d.app_user_id ,\n"
			+ "        d.remark \n"
			+ " FROM   t_offices_supplies_app_form_detail d ,\n"
			+ "        T_Office_Supplies_Info info,\n"
			+ "        T_Office_Supplies_Type_Info type\n"
			+ " WHERE  d.supplies_id = info.supplies_id\n"
			+ "        AND info.supplies_type_id= type.supplies_type_id\n"
			+ "        AND d.form_id = ?\n" + "        AND d.supplies_id = ?\n";

	public static String doUpdateApplyOfficialSuppliesForm = "UPDATE  t_offices_supplies_app_form_detail\n"
			+ "SET     form_id = ? ,\n"
			+ "        priority = ? ,\n"
			+ "        supplies_id = ? ,\n"
			+ "        reason = ? ,\n"
			+ "        qty = ? ,\n"
			+ "        app_datetime = ? ,\n"
			+ "        app_user_id = ? ,\n"
			+ "        remark = ? \n"
			+ "WHERE   form_id = ?\n" + "        AND supplies_id = ?";

	public static String doSaveAppFormProcessinstanceIDInfo = "INSERT INTO t_app_form_processinstanceid_info VALUES (?,?,?)";

	public static String doSaveApp = "insert into sec_人员角色 (ID,人员ID,角色ID,最后修改日期) values(newid(),?,?,GETDATE())";

	public static String doUpdateAppDateTimeByFormId = "UPDATE xmis.t_offices_supplies_app_form_detail SET app_datetime=? WHERE form_id=? ";

	public static String doUpdateAppappFormStatus = "UPDATE t_app_form_processinstanceid_info set status_id=? where processinstanceid=? ";

	public static String getApplyOfficialSuppliesListQuery = " SELECT d.form_id ,\n"
			+ "        d.priority ,\n"
			+ "        d.supplies_id ,\n"
			+ "        info.supplies_name,\n"
			+ "        info.supplies_type_id,\n"
			+ "        typeinfo.supplies_type_Name,\n"
			+ "        d.reason ,\n"
			+ "        d.qty ,\n"
			+ "        CONVERT(VARCHAR(100),d.app_datetime,120) as app_datetime  ,\n"
			+ "        d.app_user_id ,\n"
			+ "        RTRIM(h.姓氏)+RTRIM(h.名字) AS app_user_name,\n"
			+ "        d.remark ,\n"
			+ "        p.status_id\n"
			+ " FROM   xmis.t_app_form_processinstanceid_info p\n"
			+ "        LEFT JOIN xmis.t_offices_supplies_app_form_detail d ON p.form_id = d.form_id\n"
			+ "        LEFT OUTER JOIN T_Office_Supplies_Info info ON d.supplies_id = info.supplies_id\n"
			+ "        LEFT OUTER JOIN xmis.T_Office_Supplies_Type_Info typeinfo ON info.supplies_type_id = typeinfo.supplies_type_id\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 h ON d.app_user_id=h.人员ID\n";
	public static String getApplyOfficialSuppliesListSum = " SELECT d.form_id ,\n"
			+ "        p.status_id ,\n"
			+ "        CONVERT(VARCHAR(100),d.app_datetime,120) as app_datetime  ,\n"
			+ "        d.app_user_id ,\n"
			+ "        RTRIM(h.姓氏) + RTRIM(h.名字) AS app_user_name\n"
			+ " FROM   xmis.t_app_form_processinstanceid_info p\n"
			+ "        INNER JOIN xmis.t_offices_supplies_app_form_detail d ON p.form_id = d.form_id\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 h ON d.app_user_id = h.人员ID\n"
			+ " GROUP BY d.form_id ,\n"
			+ "        p.status_id ,\n"
			+ "        d.app_datetime ,\n"
			+ "        d.app_user_id ,\n"
			+ "        RTRIM(h.姓氏) + RTRIM(h.名字)  ";
	public static String getSecurityRolesSql = " select 角色ID as roleID,角色名称 as roleName,角色描述 as description from sec_角色  where 显示标志 =1 order by 排序";

	public static String doUpdateApplyOfficialSuppliesRepairForm = "UPDATE  t_offices_supplies_app_form_repair_detail\n"
			+ "SET     form_id = ? ,\n"
			+ "        priority = ? ,\n"
			+ "        supplies_id = ? ,\n"
			+ "        reason = ? ,\n"
			+ "        qty = ? ,\n"
			+ "        app_datetime = ? ,\n"
			+ "        app_user_id = ? ,\n"
			+ "        remark = ? \n"
			+ "WHERE   form_id = ?\n" + "        AND supplies_id = ?";

	public static String doDeleteApplyOfficialSuppliesRepairForm = "DELETE FROM  t_offices_supplies_app_form_repair_detail WHERE form_id=? AND supplies_id=?";

	public static String doUpdateAppDateTimeByRepairFormId = "UPDATE t_offices_supplies_app_form_repair_detail SET app_datetime=? WHERE form_id=? ";

	public static String getApplyOfficialSuppliesRepairListFormId = " SELECT d.form_id ,\n"
			+ "        d.priority ,\n"
			+ "        d.supplies_id ,\n"
			+ "        info.supplies_name ,\n"
			+ "        d.reason ,\n"
			+ "        d.qty ,\n"
			+ "         CONVERT(VARCHAR(100),d.app_datetime,120) as app_datetime ,\n"
			+ "        d.app_user_id ,\n"
			+ "        RTRIM(h.姓氏)+RTRIM(h.名字) app_user_name ,\n"
			+ "        d.remark \n"
			+ " FROM   t_offices_supplies_app_form_repair_detail d ,\n"
			+ "        hr_人员 h ,\n"
			+ "        T_Office_Supplies_Info info\n"
			+ " WHERE  d.supplies_id = info.supplies_id\n"
			+ "        AND d.app_user_id = h.人员ID\n"
			+ "        AND d.form_id = ?";

	public static String getApplyOfficialSuppliesRepairListSum = " SELECT d.form_id ,\n"
			+ "        p.status_id ,\n"
			+ "        convert(varchar(50), d.app_datetime,120) as app_datetime ,\n"
			+ "        d.app_user_id ,\n"
			+ "        RTRIM(h.姓氏) + RTRIM(h.名字) AS app_user_name\n"
			+ " FROM   xmis.t_app_form_processinstanceid_info p\n"
			+ "        INNER JOIN t_offices_supplies_app_form_repair_detail d ON p.form_id = d.form_id\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 h ON d.app_user_id = h.人员ID\n"
			+ " GROUP BY d.form_id ,\n"
			+ "        p.status_id ,\n"
			+ "        d.app_datetime ,\n"
			+ "        d.app_user_id ,\n"
			+ "        RTRIM(h.姓氏) + RTRIM(h.名字)  ";

	public static String doUpdateOfficeSuppliesDetailsNum = "UPDATE xmis.T_Office_Supplies_Info SET qty=isnull(qty,0)-? WHERE supplies_id=?";

	public static String getBusinessEntertainmentFormByFormId = "SELECT  f.form_id ,\n"
			+ "        f.visitor_company_name ,\n"
			+ "        f.visitor_qty ,\n"
			+ "        CONVERT(VARCHAR(20),f.meals_date,23) AS meals_date ,\n"
			+ "        f.meals_type ,\n"
			+ "        f.meals_fee_budget ,\n"
			+ "        f.accompany_qty ,\n"
			+ "        f.cost ,\n"
			+ "        f.profit ,\n"
			+ "        f.cigarettes_wine_amt ,\n"
			+ "        f.total_amt ,\n"
			+ "        CONVERT(VARCHAR(20),f.app_datetime,20) AS app_datetime ,\n"
			+ "        f.app_user_id ,\n"
			+ "        RTRIM(app_user.姓氏) + RTRIM(app_user.名字) AS app_user_name ,\n"
			+ "        f.app_deptment ,\n"
			+ "        app_dept.部门名称 AS app_deptment_name ,\n"
			+ "        f.sign_No ,\n"
			+ "        f.remark ,\n"
			+ "        f.is_worked\n"
			+ "FROM    xmis.T_Business_Entertainment_Expenses_Form f\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 app_user ON f.app_user_id = app_user.人员ID\n"
			+ "        LEFT OUTER JOIN xmis.org_部门 app_dept ON f.app_deptment = app_dept.部门ID\n"
			+ "WHERE   f.form_id = ?";

	public static String getBusinessEntertainmentFormList = "SELECT  f.form_id ,\n"
			+ "        f.visitor_company_name ,\n"
			+ "        f.visitor_qty ,\n"
			+ "        CONVERT(VARCHAR(20),f.meals_date,23) AS meals_date ,\n"
			+ "        f.meals_type ,\n"
			+ "        f.meals_fee_budget ,\n"
			+ "        f.accompany_qty ,\n"
			+ "        f.cost ,\n"
			+ "        f.profit ,\n"
			+ "        f.cigarettes_wine_amt ,\n"
			+ "        f.total_amt ,\n"
			+ "        CONVERT(VARCHAR(20),f.app_datetime,23) AS app_datetime ,\n"
			+ "        f.app_user_id ,\n"
			+ "        RTRIM(app_user.姓氏) + RTRIM(app_user.名字) AS app_user_name ,\n"
			+ "        f.app_deptment ,\n"
			+ "        app_dept.部门名称 AS app_deptment_name ,\n"
			+ "        f.sign_No ,\n"
			+ "        f.remark ,\n"
			+ "        f.is_worked,\n"
			+ "         info.processinstanceid as proid\n"
			+ "FROM    xmis.T_Business_Entertainment_Expenses_Form f\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 app_user ON f.app_user_id = app_user.人员ID\n"
			+ "        LEFT OUTER JOIN xmis.org_部门 app_dept ON f.app_deptment = app_dept.部门ID\n"
			+ "         LEFT OUTER JOIN xmis.t_app_form_processinstanceid_info info ON f.form_id=info.form_id\n"
			+ " WHERE f.is_worked=0\n";

	public static String doUpdateBusinessEntertainmentWorkedId = "UPDATE T_Business_Entertainment_Expenses_Form SET is_worked=? WHERE form_id=?";

	public static String doUpdateSignNoByFormId = "UPDATE T_Business_Entertainment_Expenses_Form SET sign_No=? WHERE (sign_No = '' OR sign_No IS NULL ) AND form_id=? ";

	public static String getSign_No ="SELECT MAX(sign_No) as sign_No FROM [bbz].[xmis].[T_Business_Entertainment_Expenses_Form] where SUBSTRING(sign_No,1,4) = ?";
	
	public static String getAbsentFormByFormId =

	"SELECT  form_id ,\n"
			+ "        t.absent_type_name as absent_type_name,\n"
			+ "        f.absent_type_id,\n "
			+ "        f.plan_begin_date ,\n"
			+ "        f.plan_end_date ,\n"
			+ "        f.actual_begin_date ,\n"
			+ "        f.plan_sum ,\n"
			+ "        f.actual_sum ,\n"
			+ "        f.actual_end_date ,\n"
			+ "        f.leavetime ,\n"
			+ "        f.app_datetime ,\n"
			+ "          CONVERT(VARCHAR(20),f.plan_begin_date ,20)  as plan_begin_date ,\n"
			+ "         CONVERT(VARCHAR(20),f.plan_end_date ,20)  as plan_end_date ,\n"
			+ "         CONVERT(VARCHAR(20),f.actual_begin_date ,20) as actual_begin_date ,\n"
			+ "         CONVERT(VARCHAR(20),f.actual_end_date ,20)  as actual_end_date ,\n"
			+ "         CONVERT(VARCHAR(20),f.leavetime ,20) as leavetime ,\n"
			+ "         CONVERT(VARCHAR(20),f.app_datetime ,20) as app_datetime ,\n"
			+ "        f.reason ,\n"
			+ "        RTRIM(h.姓氏)+RTRIM(h.名字) AS app_user_name,\n"
			+ "        f.app_user_id\n" + "FROM    T_Absent_Form_Detail f\n"
			+ "LEFT OUTER JOIN xmis.hr_人员 h ON f.app_user_id=h.人员ID\n "
			+ "LEFT OUTER JOIN xmis.T_Absent_STATUS t ON t.absent_type_id=f.absent_type_id\n"
			+ "WHERE f.form_id=?";
	
	public static String getMonthPlanByFormId =

			"SELECT  f.form_id ,\n"
					+ "        f.work_month,\n"
					+ "        f.plan_comments ,\n"
					+ "        f.plan_assign_comments ,\n"
					+ "        f.plan_assigned_id ,\n"
					+ "        f.app_user_id,\n"
					+ "        f.app_dept_id,\n"
					+ "        g.部门名称   as app_dept_name,\n"
					+ "        f.app_date ,\n"
					+ "          CONVERT(VARCHAR(10), f.plan_assigned_date ,20)  as plan_assigned_date ,\n"
					+ "          CONVERT(VARCHAR(10), f.app_date ,20)  as app_date ,\n"
					+ "        RTRIM(h.姓氏)+RTRIM(h.名字) AS app_user_name,\n"
					+ "        RTRIM(r.姓氏)+RTRIM(r.名字) AS plan_assigned_name,\n"
					+ "         d.excuted_comments,"
					+ "			d.unfinished_comments,"
					+ "			d.excuted_assigned_comments,"
					+ "			d.excuted_assigned_id,"
					+ "			d.excute_assigned_date, "
					+ "          CONVERT(VARCHAR(10), d.excute_assigned_date ,20)  as excuted_assigned_date ,\n"
					+ "        RTRIM(hr.姓氏)+RTRIM(hr.名字) AS excuted_assigned_name \n"
					+ "FROM    T_Workplan_Form_Detail_EarlyPlans f  "
					+ " LEFT OUTER JOIN xmis.hr_人员 h ON  f.app_user_id=h.人员ID  LEFT OUTER JOIN xmis.hr_人员 r ON f.plan_assigned_id = r.人员ID, "
					+ "T_Workplan_Form_Detail_EndOfPlanned d "
					+ "   LEFT OUTER JOIN xmis.hr_人员 hr ON d.excuted_assigned_id = hr.人员ID,  \n"
					+" org_部门  g \n"
					+ "WHERE f.form_id=? and  d.form_id = f.form_id  and  g.部门ID = f.app_dept_id";

	public static String getAbsentFormList =

	"SELECT  f.form_id ,\n"
			+" pr.processinstanceid as proId,"
			+ "        f.app_user_id ,\n"
			+ "        RTRIM(p.姓氏) + RTRIM(p.名字) AS app_user_name ,\n"
			+ "        f.absent_type_id ,\n"
			+"			leavetime,"
			+ "        plan_sum ,\n"
			+ "       actual_sum ,\n"
			+ "        f.reason ,\n"
			+"         ''  as  diffstr ,"
			+ "         CONVERT(VARCHAR(20),f.leavetime ,20) as leavetime ,\n"
			+ "         CONVERT(VARCHAR(20),f.app_datetime ,20) as app_datetime \n"
			+ "FROM    T_Absent_Form_Detail f\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 p ON f.app_user_id = p.人员ID\n"
			+ "        LEFT OUTER JOIN xmis.t_app_form_processinstanceid_info pr ON pr.form_id = f.form_id\n"
			
			;
			/* ly 原有逻辑
			+ "  WHERE f.leavetime IS  NOT NULL \n";
			*/
	
	public static String getAskForLeaveList =" SELECT  org.部门名称 AS deptName,RTRIM(hr.姓氏) + RTRIM(hr.名字) AS hrName,i.* FROM("
   +"  SELECT   a.人员ID ,"
   +"         a.带薪年假 ,"
   +"         b.探亲假 ,"
   +"         c.病假 ,"
   +"         d.事假 ,"
   +"         e.婚假 ,"
   +"         f.丧假 ,"
   +"         g.优生优育假 ,"
   +"         h.公派"
   +"   FROM     ( SELECT    p.人员ID,"
   +"                     SUM(f.actual_sum) AS 带薪年假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 1 "
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(7),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(7),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID" 
   +"         ) AS a ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 探亲假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 2"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(7),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(7),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"         GROUP BY  p.人员ID"
   +"         ) AS b ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 病假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 3"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(7),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(7),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"        ) AS c ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 事假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 4"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(7),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(7),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"         ) AS d ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 婚假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 5"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(7),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(7),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"         ) AS e ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 丧假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                          AND f.absent_type_id = 6"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(7),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(7),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"        ) AS f ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 优生优育假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 7"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(7),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(7),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"         ) AS g ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 公派"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 8"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(7),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(7),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"         ) AS h"
   +"   WHERE    a.人员ID = b.人员ID"
   +"         AND a.人员ID = c.人员ID"
   +"         AND a.人员ID = d.人员ID"
   +"         AND a.人员ID = e.人员ID"
   +"         AND a.人员ID = f.人员ID"
   +"         AND a.人员ID = g.人员ID"
   +"         AND a.人员ID = h.人员ID"
   +"		) AS i "
   +"		 LEFT OUTER JOIN xmis.hr_人员 hr  ON hr.人员ID = i.人员ID"
   +"		  LEFT OUTER JOIN xmis.hr_人员部门关系表 dp  ON dp.人员ID = i.人员ID"
   +"			   LEFT OUTER JOIN xmis.org_部门 org  ON dp.部门ID = org.部门ID";
   	

	public static String getAskForLeaveListByFullYear =" SELECT  org.部门名称 AS deptName,RTRIM(hr.姓氏) + RTRIM(hr.名字) AS hrName,i.* FROM("
   +"  SELECT   a.人员ID ,"
   +"         a.带薪年假 ,"
   +"         b.探亲假 ,"
   +"         c.病假 ,"
   +"         d.事假 ,"
   +"         e.婚假 ,"
   +"         f.丧假 ,"
   +"         g.优生优育假 ,"
   +"         h.公派"
   +"   FROM     ( SELECT    p.人员ID,"
   +"                     SUM(f.actual_sum) AS 带薪年假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 1 "
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(4),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(4),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID" 
   +"         ) AS a ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 探亲假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 2"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(4),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(4),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"         GROUP BY  p.人员ID"
   +"         ) AS b ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 病假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 3"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(4),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(4),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"        ) AS c ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 事假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 4"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(4),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(4),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"         ) AS d ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 婚假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 5"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(4),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(4),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"         ) AS e ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 丧假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                          AND f.absent_type_id = 6"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(4),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(4),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"        ) AS f ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 优生优育假"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 7"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(4),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(4),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"         ) AS g ,"
   +"         ( SELECT    p.人员ID ,"
   +"                     SUM(f.actual_sum) AS 公派"
   +"           FROM      xmis.hr_人员 p"
   +"                     LEFT OUTER JOIN xmis.T_Absent_Form_Detail f ON p.人员ID = f.app_user_id"
   +"                                                           AND f.absent_type_id = 8"
   + "															AND f.leavetime IS NOT NULL "
   + "                                                          AND  CONVERT(VARCHAR(4),actual_begin_date,20)  <= ?  "
   + "                                                          AND CONVERT(VARCHAR(4),actual_end_date,20)  >=?"
   + "                     , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1"
   +"           GROUP BY  p.人员ID"
   +"         ) AS h"
   +"   WHERE    a.人员ID = b.人员ID"
   +"         AND a.人员ID = c.人员ID"
   +"         AND a.人员ID = d.人员ID"
   +"         AND a.人员ID = e.人员ID"
   +"         AND a.人员ID = f.人员ID"
   +"         AND a.人员ID = g.人员ID"
   +"         AND a.人员ID = h.人员ID"
   +"		) AS i "
   +"		 LEFT OUTER JOIN xmis.hr_人员 hr  ON hr.人员ID = i.人员ID"
   +"		  LEFT OUTER JOIN xmis.hr_人员部门关系表 dp  ON dp.人员ID = i.人员ID"
   +"			   LEFT OUTER JOIN xmis.org_部门 org  ON dp.部门ID = org.部门ID";
	
	public static String getAttachmentListByAbsentFormId =

	"SELECT attd.附件ID ,\n"
			+ "       CONVERT(VARCHAR(20), attd.上传时间, 20) AS 上传时间 ,\n"
			+ "        RTRIM(upload_p.姓氏) + RTRIM(upload_p.名字) AS 上传人 ,\n"
			+ "       attd.文件名 ,\n"
			+ "       attd.附件主体类型ID ,\n"
			+ "       attd.REFID ,\n"
			+ "       CONVERT(VARCHAR(20), attd.最后修改日期, 20) AS 最后修改日期 ,\n"
			+ "        RTRIM(update_p.姓氏) + RTRIM(update_p.名字) AS 最后修改人 ,\n"
			+ "       attd.存放路径 ,\n"
			+ "       attd.删除标志\n"
			+ "FROM   sys_附件 attd\n"
			+ "       LEFT OUTER JOIN xmis.hr_人员 upload_p ON attd.上传人 = upload_p.人员ID\n"
			+ "       LEFT OUTER JOIN xmis.hr_人员 update_p ON attd.最后修改人ID = update_p.人员ID\n"
			+ "WHERE  attd.删除标志 = 0\n" + "       AND attd.REFID = ? and attd.附件ID >0";

	public static String getEmpsByDeptLeaderMapping = " SELECT user_id FROM xmis.T_Dept_Leader_Mapping WHERE dept_id=? AND role_flag=? ";

	public static String updateAttched = "UPDATE  xmis.sys_附件 SET  附件内容 = ? WHERE   附件ID = ?";

	public static String getDutyPlanList = " SELECT t.id,t.on_watch_id,t.job_type_id,RTRIM(p1.姓氏) + RTRIM(p1.名字) AS on_watch_name,p5.部门名称  AS department,CONVERT(VARCHAR(10), t.on_watch_date, 23) as on_watch_date,CONVERT(VARCHAR(8), t.begin_time, 108) as begin_time,CONVERT(VARCHAR(10), t.end_date, 23)  as end_date,CONVERT(VARCHAR(8), t.end_time, 108) as end_time,CONVERT(VARCHAR(10), t.scheduled_datetime, 23) as scheduled_datetime,t.scheduled_id,RTRIM(p2.姓氏) + RTRIM(p2.名字) AS scheduled_name,t.last_updated_id,RTRIM(p3.姓氏) + RTRIM(p3.名字) AS last_updated_name,CONVERT(VARCHAR(10), t.last_updated_datetime, 23) as last_updated_datetime,t.reason "
			+ "FROM xmis.T_OnWatch_schedule t "
			+ " LEFT OUTER JOIN xmis.hr_人员 p1 ON t.on_watch_id = p1.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员 p2 ON t.scheduled_id = p2.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员 p3 ON t.last_updated_id = p3.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员部门关系表   p4 ON t.on_watch_id = p4.人员ID "
			+ "LEFT OUTER JOIN xmis.org_部门   p5 ON p4.部门ID = p5.部门ID "
			+ "order by on_watch_date desc,begin_time asc ";

	public static String getDocumentTypeInfoCombo = "SELECT  document_type_id ,\n"
			+ "        document_type_Name ,\n"
			+ "        remark\n"
			+ "FROM    bbz.xmis.T_Document_Type_Info";

	public static String getDocumentAppFormDetailByFormId = "SELECT  f.form_id ,\n"
			+ "        f.document_type_id ,\n"
			+ "        dtype.document_type_Name as  document_type_name,\n"
			+ "        f.priority ,\n"
			+ "        f.file_no ,\n"
			+ "        f.file_flag ,\n"
			+ "        f.title ,\n"
			+ "        f.external_name ,\n"
			+ "        f.document_code ,\n"
			+ "        f.summary ,\n"
			+ "        CONVERT(VARCHAR(50), f.app_datetime, 120) AS app_datetime ,\n"
			+ "        f.app_user_id ,"
			+"         f.app_dept_id,\n"
			+"         dept.部门名称  as app_dept_name,"
			+ "        RTRIM(app_user.姓氏) + RTRIM(app_user.名字) app_user_name ,\n"
			+ "        f.app_dept_leader_id ,\n"
			+ "        RTRIM(app_dept_leader.姓氏) + RTRIM(app_dept_leader.名字) app_dept_leader_name ,\n"
			+ "        CONVERT(VARCHAR(50), f.app_dept_approved_datetime, 120) AS app_dept_approved_datetime ,\n"
			+ "        f.app_dept_approved_reason ,\n"
			+ "        f.app_spec_leader_id ,\n"
			+ "        RTRIM(app_spec_leader.姓氏) + RTRIM(app_spec_leader.名字) app_spec_leader_name ,\n"
			+ "        CONVERT(VARCHAR(50), f.app_spec_approved_datetime, 120) AS app_spec_approved_datetime ,\n"
			+ "        f.app_spec_approved_reason ,\n"
			+ "        f.app_direc_leader_id ,\n"
			+ "        RTRIM(app_direc_leader.姓氏) + RTRIM(app_direc_leader.名字) app_direc_leader_name ,\n"
			+ "        CONVERT(VARCHAR(50), f.app_direc_approved_datetime, 120) AS app_direc_approved_datetime ,\n"
			+ "        f.app_direc_approved_reason\n"
			+ "FROM    T_Document_App_Form_Detail f\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 app_user ON f.app_user_id = app_user.人员ID\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 app_dept_leader ON f.app_dept_leader_id = app_dept_leader.人员ID\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 app_spec_leader ON f.app_spec_leader_id = app_spec_leader.人员ID\n"
			+ "        LEFT OUTER JOIN xmis.hr_人员 app_direc_leader ON f.app_direc_leader_id = app_direc_leader.人员ID\n"
			+ "        LEFT OUTER JOIN bbz.xmis.T_Document_Type_Info dtype ON f.document_type_id = dtype.document_type_id\n" 
			+"         LEFT OUTER JOIN xmis.org_部门 dept on dept.部门ID = f.app_dept_id  "
			+ "WHERE   f.form_id = ?";

	public static String doUpdateDeptArroval = "UPDATE xmis.T_Document_App_Form_Detail SET app_dept_leader_id=?,app_dept_approved_reason=?,app_dept_approved_datetime=? WHERE form_id=?";

	public static String doUpdateSpecArroval = "UPDATE xmis.T_Document_App_Form_Detail SET app_spec_leader_id=?,app_spec_approved_reason=?,app_spec_approved_datetime=? WHERE form_id=?";

	public static String doUpdatedirecArroval = "UPDATE xmis.T_Document_App_Form_Detail SET app_direc_leader_id=?,app_direc_approved_reason=?,app_direc_approved_datetime=? WHERE form_id=?";

	public static String getDutyPlanListByPart = " SELECT t.id,t.on_watch_id,t.job_type_id,RTRIM(p1.姓氏) + RTRIM(p1.名字) AS on_watch_name,p5.部门名称  AS department,CONVERT(VARCHAR(10), t.on_watch_date, 23) as on_watch_date,CONVERT(VARCHAR(8), t.begin_time, 108) as begin_time,CONVERT(VARCHAR(10), t.end_date, 23)  as end_date,CONVERT(VARCHAR(8), t.end_time, 108) as end_time,CONVERT(VARCHAR(10), t.scheduled_datetime, 23) as scheduled_datetime,t.scheduled_id,RTRIM(p2.姓氏) + RTRIM(p2.名字) AS scheduled_name,t.last_updated_id,RTRIM(p3.姓氏) + RTRIM(p3.名字) AS last_updated_name,CONVERT(VARCHAR(10), t.last_updated_datetime, 23) as last_updated_datetime,t.reason "
			+ "FROM xmis.T_OnWatch_schedule t "
			+ " LEFT OUTER JOIN xmis.hr_人员 p1 ON t.on_watch_id = p1.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员 p2 ON t.scheduled_id = p2.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员 p3 ON t.last_updated_id = p3.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员部门关系表   p4 ON t.on_watch_id = p4.人员ID "
			+ "LEFT OUTER JOIN xmis.org_部门   p5 ON p4.部门ID = p5.部门ID where p5.部门ID = ? "
			+ "order by on_watch_date desc,begin_time asc ";

	public static String getDutyPlanListByDate = " SELECT t.id,t.on_watch_id,t.job_type_id,RTRIM(p1.姓氏) + RTRIM(p1.名字) AS on_watch_name,p5.部门名称  AS department,CONVERT(VARCHAR(10), t.on_watch_date, 23) as on_watch_date,CONVERT(VARCHAR(8), t.begin_time, 108) as begin_time,CONVERT(VARCHAR(10), t.end_date, 23)  as end_date,CONVERT(VARCHAR(8), t.end_time, 108) as end_time,CONVERT(VARCHAR(10), t.scheduled_datetime, 23) as scheduled_datetime,t.scheduled_id,RTRIM(p2.姓氏) + RTRIM(p2.名字) AS scheduled_name,t.last_updated_id,RTRIM(p3.姓氏) + RTRIM(p3.名字) AS last_updated_name,CONVERT(VARCHAR(10), t.last_updated_datetime, 23) as last_updated_datetime,t.reason "
			+ "FROM xmis.T_OnWatch_schedule t "
			+ " LEFT OUTER JOIN xmis.hr_人员 p1 ON t.on_watch_id = p1.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员 p2 ON t.scheduled_id = p2.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员 p3 ON t.last_updated_id = p3.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员部门关系表   p4 ON t.on_watch_id = p4.人员ID "
			+ "LEFT OUTER JOIN xmis.org_部门   p5 ON p4.部门ID = p5.部门ID where t.on_watch_date <= ? and t.end_date >= ?  "
			+ "order by on_watch_date desc,begin_time asc ";

	public static String getDutyPlanListByPartAndDate = " SELECT t.id,t.on_watch_id,t.job_type_id,RTRIM(p1.姓氏) + RTRIM(p1.名字) AS on_watch_name,p5.部门名称  AS department,CONVERT(VARCHAR(10), t.on_watch_date, 23) as on_watch_date,CONVERT(VARCHAR(8), t.begin_time, 108) as begin_time,CONVERT(VARCHAR(10), t.end_date, 23)  as end_date,CONVERT(VARCHAR(8), t.end_time, 108) as end_time,CONVERT(VARCHAR(10), t.scheduled_datetime, 23) as scheduled_datetime,t.scheduled_id,RTRIM(p2.姓氏) + RTRIM(p2.名字) AS scheduled_name,t.last_updated_id,RTRIM(p3.姓氏) + RTRIM(p3.名字) AS last_updated_name,CONVERT(VARCHAR(10), t.last_updated_datetime, 23) as last_updated_datetime,t.reason "
			+ "FROM xmis.T_OnWatch_schedule t "
			+ " LEFT OUTER JOIN xmis.hr_人员 p1 ON t.on_watch_id = p1.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员 p2 ON t.scheduled_id = p2.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员 p3 ON t.last_updated_id = p3.人员ID "
			+ "LEFT OUTER JOIN xmis.hr_人员部门关系表   p4 ON t.on_watch_id = p4.人员ID "
			+ "LEFT OUTER JOIN xmis.org_部门   p5 ON p4.部门ID = p5.部门ID where p5.部门ID = ? and t.on_watch_date <= ? and t.end_date >= ?  "
			+ "order by on_watch_date desc,begin_time asc ";

	public static String getDocumentCirculatedInstancesList =

	"SELECT   \n"
			+ "        f.from_id ,\n"
			+ "        RTRIM(p.姓氏) + RTRIM(p.名字) AS from_name ,\n"
			+ "        CONVERT(VARCHAR(50), f.from_datetime, 120) AS from_datetime ,\n"
			+ "        f.to_id ,\n"
			+ "        RTRIM(pt.姓氏) + RTRIM(pt.名字) AS to_name ,\n"
			+ "        f.comments\n"
			+ "FROM    T_Document_Circulated_Instances f ,\n"
			+ "        xmis.hr_人员 p,\n" + "        xmis.hr_人员 pt\n"
			+ "WHERE   f.from_id = p.人员ID\n" + "and   f.to_id = pt.人员ID\n"
			+ "and  f.form_id =?\n" + "ORDER BY from_datetime";

	public static String getTitleByFormId = "  SELECT title  FROM xmis.T_Document_App_Form_Detail WHERE form_id=?";

	public static String getMonthPlanList = " select e.work_month,o.部门名称 AS app_dept_name, "
			+ " ( SELECT TOP ( 1 )"
			+ "        task.DISPLAY_NAME "
			+ " FROM    xmis.T_FF_RT_TASKINSTANCE task ,"
			+ "       xmis.t_app_form_processinstanceid_info app"
			+ "    WHERE     "
			+ "       task.PROCESSINSTANCE_ID = app.processinstanceid "
			+ "       AND app.form_id = e.form_id "
			+ "             ORDER BY  task.CREATED_TIME DESC "
			+ "         )  status,"
			+ "e.form_id as formid,"
			+ "( select app.processinstanceid " 
			+ " FROM  xmis.t_app_form_processinstanceid_info app"
			+ "    WHERE    app.form_id = e.form_id ) as proid,"
			+ "RTRIM(hr1.姓氏) + RTRIM(hr1.名字) AS plan_assigned_name, CONVERT(VARCHAR(10), e.plan_assigned_date, 23) AS plan_assigned_date,RTRIM(hr.姓氏) + RTRIM(hr.名字) AS app_user_name,CONVERT(VARCHAR(10), e.app_date, 23) AS  app_date "
			+ " FROM    T_Workplan_Form_Detail_EarlyPlans e  "
			+ " LEFT OUTER JOIN xmis.hr_人员 hr ON e.app_user_id = hr.人员ID "
			+ " LEFT OUTER JOIN xmis.hr_人员 hr1 ON e.plan_assigned_id = hr1.人员ID "
			+ " LEFT OUTER JOIN xmis.org_部门   o ON e.app_dept_id = o.部门ID  "
			+ "  order by app_date DESC";
	
	public static String getMonthPlanListByDate = " select e.work_month,o.部门名称 AS app_dept_name,"
			+ " ( SELECT TOP ( 1 )"
			+ "        task.DISPLAY_NAME "
			+ " FROM    xmis.T_FF_RT_TASKINSTANCE task ,"
			+ "       xmis.t_app_form_processinstanceid_info app"
			+ "    WHERE     "
			+ "        task.PROCESSINSTANCE_ID = app.processinstanceid "
			+ "       AND app.form_id = e.form_id "
			+ "             ORDER BY  task.CREATED_TIME DESC "
			+ "         )  status," 
			+ "RTRIM(hr1.姓氏) + RTRIM(hr1.名字) AS plan_assigned_name, CONVERT(VARCHAR(10), e.plan_assigned_date, 23) AS plan_assigned_date,RTRIM(hr.姓氏) + RTRIM(hr.名字) AS app_user_name,CONVERT(VARCHAR(10), e.app_date, 23) AS  app_date "
			+ " FROM    T_Workplan_Form_Detail_EarlyPlans e  "
			+ " LEFT OUTER JOIN xmis.hr_人员 hr ON e.app_user_id = hr.人员ID "
			+ " LEFT OUTER JOIN xmis.hr_人员 hr1 ON e.plan_assigned_id = hr1.人员ID "
			+ " LEFT OUTER JOIN xmis.org_部门   o ON e.app_dept_id = o.部门ID "
			+ " where work_month = ?  order by app_date DESC";
	
	public static String getMonthPlanListByPart = " select e.work_month,o.部门名称 AS app_dept_name,"
			+ " ( SELECT TOP ( 1 )"
			+ "        task.DISPLAY_NAME "
			+ " FROM    xmis.T_FF_RT_TASKINSTANCE task ,"
			+ "       xmis.t_app_form_processinstanceid_info app"
			+ "    WHERE     "
			+ "        task.PROCESSINSTANCE_ID = app.processinstanceid "
			+ "       AND app.form_id = e.form_id "
			+ "             ORDER BY  task.CREATED_TIME DESC "
			+ "         )  status," 
			+ "RTRIM(hr1.姓氏) + RTRIM(hr1.名字) AS plan_assigned_name, CONVERT(VARCHAR(10), e.plan_assigned_date, 23) AS plan_assigned_date,RTRIM(hr.姓氏) + RTRIM(hr.名字) AS app_user_name,CONVERT(VARCHAR(10), e.app_date, 23) AS  app_date "
			+ " FROM    T_Workplan_Form_Detail_EarlyPlans e  "
			+ " LEFT OUTER JOIN xmis.hr_人员 hr ON e.app_user_id = hr.人员ID "
			+ " LEFT OUTER JOIN xmis.hr_人员 hr1 ON e.plan_assigned_id = hr1.人员ID "
			+ " LEFT OUTER JOIN xmis.org_部门   o ON e.app_dept_id = o.部门ID "
			+ " where app_dept_id = ?  order by app_date DESC";
	
	public static String getMonthPlanListByPartAndDate = " select e.work_month,o.部门名称 AS app_dept_name,"
			+ " ( SELECT TOP ( 1 )"
			+ "        task.DISPLAY_NAME "
			+ " FROM    xmis.T_FF_RT_TASKINSTANCE task ,"
			+ "       xmis.t_app_form_processinstanceid_info app"
			+ "    WHERE     "
			+ "      task.PROCESSINSTANCE_ID = app.processinstanceid "
			+ "       AND app.form_id = e.form_id "
			+ "             ORDER BY  task.CREATED_TIME DESC "
			+ "         )  status," 
			+ "RTRIM(hr1.姓氏) + RTRIM(hr1.名字) AS plan_assigned_name, CONVERT(VARCHAR(10), e.plan_assigned_date, 23) AS plan_assigned_date,RTRIM(hr.姓氏) + RTRIM(hr.名字) AS app_user_name,CONVERT(VARCHAR(10), e.app_date, 23) AS  app_date "
			+ " FROM    T_Workplan_Form_Detail_EarlyPlans e  "
			+ " LEFT OUTER JOIN xmis.hr_人员 hr ON e.app_user_id = hr.人员ID "
			+ " LEFT OUTER JOIN xmis.hr_人员 hr1 ON e.plan_assigned_id = hr1.人员ID "
			+ " LEFT OUTER JOIN xmis.org_部门   o ON e.app_dept_id = o.部门ID "
			+ " where app_dept_id = ?  and  work_month = ? order by app_date DESC";
	
	public static String doSaveFormIdAndProId = " insert into t_app_form_processinstanceid_info (form_id,processinstanceid,status_id) values(?,?,0)";
	
	public static String getDocumentAppFormDetailList = "SELECT  f.form_id ,\n"
		+ "        f.document_type_id ,\n"
		+ "        dtype.document_type_Name as  document_type_name,\n"
		+ "        f.priority ,\n"
		+ "        f.title ,\n"
		+ "        f.external_name ,\n"
		+ "        f.document_code ,\n"
		+ "        f.summary ,\n"
		+ "        CONVERT(VARCHAR(50), f.app_datetime, 120) AS app_datetime ,\n"
		+ "        f.app_user_id ,\n"
		+ "        RTRIM(app_user.姓氏) + RTRIM(app_user.名字) app_user_name ,\n"
		+ "        f.app_dept_leader_id ,\n"
		+ "        RTRIM(app_dept_leader.姓氏) + RTRIM(app_dept_leader.名字) app_dept_leader_name ,\n"
		+ "        CONVERT(VARCHAR(50), f.app_dept_approved_datetime, 120) AS app_dept_approved_datetime ,\n"
		+ "        f.app_dept_approved_reason ,\n"
		+ "        f.app_spec_leader_id ,\n"
		+ "        RTRIM(app_spec_leader.姓氏) + RTRIM(app_spec_leader.名字) app_spec_leader_name ,\n"
		+ "        CONVERT(VARCHAR(50), f.app_spec_approved_datetime, 120) AS app_spec_approved_datetime ,\n"
		+ "        f.app_spec_approved_reason ,\n"
		+ "        f.app_direc_leader_id ,\n"
		+ "        RTRIM(app_direc_leader.姓氏) + RTRIM(app_direc_leader.名字) app_direc_leader_name ,\n"
		+ "        CONVERT(VARCHAR(50), f.app_direc_approved_datetime, 120) AS app_direc_approved_datetime ,\n"
		+ "        f.app_direc_approved_reason\n"
		+ "FROM    T_Document_App_Form_Detail f\n"
		+ "        LEFT OUTER JOIN xmis.hr_人员 app_user ON f.app_user_id = app_user.人员ID\n"
		+ "        LEFT OUTER JOIN xmis.hr_人员 app_dept_leader ON f.app_dept_leader_id = app_dept_leader.人员ID\n"
		+ "        LEFT OUTER JOIN xmis.hr_人员 app_spec_leader ON f.app_spec_leader_id = app_spec_leader.人员ID\n"
		+ "        LEFT OUTER JOIN xmis.hr_人员 app_direc_leader ON f.app_direc_leader_id = app_direc_leader.人员ID\n"
		+ "        LEFT OUTER JOIN bbz.xmis.T_Document_Type_Info dtype ON f.document_type_id = dtype.document_type_id\n";
	
	public static String getGczlInfoList = 
		"SELECT  info.tab_seed ,\n" +
		"        info.unit_code ,\n" + 
		"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
		"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" + 
		"        info.ap_w_level ,\n" + 
		"        info.bp_w_level ,\n" + 
		"        info.gate_open_num_1 ,\n" + 
		"        info.gate_open_degree_1 ,\n" + 
		"        info.gate_open_num_2 ,\n" + 
		"        info.gate_open_degree_2 ,\n" + 
		"        info.elec_machine ,\n" + 
		"        info.flow_qty ,\n" + 
		"        info.w_level_chan ,\n" + 
		"        info.memo ,\n" + 
		"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
		"        h.人员ID AS measure_man_id\n" + 
		"FROM    bbz.xmis.gczl_water_level_info info\n" + 
		"        LEFT OUTER JOIN xmis.hr_人员 h ON info.measure_man = h.人员ID";

	public static String getMaxIDFromGczlInfo=
		"SELECT MAX(tab_seed) FROM bbz.xmis.gczl_water_level_info";

	public static String doDeleteGczlInfoById=
		"delete FROM bbz.xmis.gczl_water_level_info WHERE tab_seed=?";

	public static String getGczlInfoListWithQuery=

		" DECLARE @b_date VARCHAR(11) , @e_date VARCHAR(11) SET @b_date=? SET @e_date=?\n" +
		"\n" + 
		"SELECT  '闸上水位' AS title, '最大值' AS boundary,ap_w_level  AS VALUE,measure_date,'.'  as space \n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE ap_w_level=(SELECT   MAX(ap_w_level)\n" + 
		"FROM  gczl_water_level_info\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		"\n" + 
		"UNION\n" + 
		"\n" + 
		"SELECT '闸下水位' AS title,  '最大值' AS boundary,bp_w_level    AS value,measure_date,'.'  as space\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE bp_w_level=(SELECT   MAX(bp_w_level)\n" + 
		"FROM  gczl_water_level_info\n" + 
		
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		"\n" + 
		"\n" + 
		"UNION\n" + 
		"\n" + 
		"SELECT   '闸上水位' AS title, '最小值' AS boundary,ap_w_level  AS value,measure_date,'.'  as space\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE ap_w_level=(SELECT  MIN(ap_w_level)\n" + 
		"FROM  gczl_water_level_info\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		"\n" + 
		"UNION\n" + 
		"\n" + 
		"SELECT   '闸下水位' AS title, '最小值' AS boundary,bp_w_level  AS value,measure_date,'.'  as space\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE bp_w_level=(SELECT  MIN(bp_w_level)\n" + 
		"FROM  gczl_water_level_info\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		"\n" + 
		"\n" + 
		"UNION\n" + 
		"\n" + 
		"\n" + 
		"\n" + 
		"SELECT  '水位差' AS title, '最大值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE w_level_chan=(SELECT   MAX(w_level_chan)\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		"\n" + 
		"UNION\n" + 
		"\n" + 
		"\n" + 
		"SELECT   '水位差' AS title, '最小值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE w_level_chan=(SELECT  Min(w_level_chan)\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		"\n" + 
		"UNION\n" + 
		"\n" + 
		"SELECT   '流量' AS title, '最大值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE flow_qty=(SELECT   MAX(flow_qty)\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		"\n" + 
		"UNION\n" + 
		"\n" + 
		"SELECT   '流量' AS title, '最小值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE flow_qty=(SELECT   MIN(flow_qty)\n" + 
		"FROM gczl_water_level_info\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<@e_date\n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<@e_date";

	public static String getGczlInfoListComparison=

			" DECLARE @b_date VARCHAR(7)  SET @b_date=?\n" +
			"\n" + 
			"SELECT  CONVERT(VARCHAR(4),@b_date,20) AS year,'闸上水位' AS title, '最大值' AS boundary,ap_w_level  AS VALUE,measure_date,'.'  as space \n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE ap_w_level=(SELECT   MAX(ap_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			"WHERE  CONVERT(VARCHAR(7),measure_date,20) =  @b_date\n" + 
			")\n" + 
			"AND CONVERT(VARCHAR(7),measure_date,20) = @b_date\n" +
			"UNION\n" + 
			"\n" + 
			"SELECT   CONVERT(VARCHAR(4),@b_date,20) AS year,'闸上水位' AS title, '最小值' AS boundary,ap_w_level  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE ap_w_level=(SELECT  MIN(ap_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			"WHERE  CONVERT(VARCHAR(7),measure_date,20) =  @b_date\n" + 
			")\n" + 
			"AND CONVERT(VARCHAR(7),measure_date,20) = @b_date\n" +
			"\n" + 
			"UNION\n" + 
			"\n" + 
			"SELECT  CONVERT(VARCHAR(4),@b_date,20) AS year, '闸下水位' AS title, '最大值' AS boundary,bp_w_level  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE bp_w_level=(SELECT  MAX(bp_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			"WHERE  CONVERT(VARCHAR(7),measure_date,20) =  @b_date\n" + 
			")\n" + 
			"AND CONVERT(VARCHAR(7),measure_date,20) = @b_date\n" +
			"\n" + "UNION\n" + 
			"\n" + 
			"SELECT   CONVERT(VARCHAR(4),@b_date,20) AS year,'闸下水位' AS title, '最小值' AS boundary,bp_w_level  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE bp_w_level=(SELECT  MIN(bp_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			"WHERE  CONVERT(VARCHAR(7),measure_date,20) =  @b_date\n" + 
			")\n" + 
			"AND CONVERT(VARCHAR(7),measure_date,20) = @b_date\n" +
			"\n" 
			;
			
			
	public static String getCurrentAndNextAbsentInfo=

		"SELECT  f.form_id ,\n" +
		"        f.absent_type_id ,\n" + 
		"        s.absent_type_name ,\n" + 
		"        f.reason ,\n" + 
		"        f.app_user_id ,\n" + 
		"        RTRIM(p.姓氏) + RTRIM(p.名字) AS app_user_name ,\n" + 
		"        CONVERT(VARCHAR(50), f.plan_begin_date, 23) AS plan_begin_date ,\n" + 
		"        CONVERT(VARCHAR(50), f.plan_end_date, 23) AS plan_end_date ,\n" + 
		"        1 AS type\n" + 
		"FROM    xmis.T_Absent_Form_Detail f\n" + 
		"        LEFT OUTER JOIN T_Absent_STATUS s ON f.absent_type_id = s.absent_type_id\n" + 
		"        LEFT OUTER JOIN xmis.hr_人员 p ON f.app_user_id = p.人员ID\n ,"
		+ "      xmis.t_app_form_processinstanceid_info fp,xmis.T_FF_RT_TASKINSTANCE task " + 
		"WHERE   GETDATE() BETWEEN f.plan_begin_date\n" + 
		"                  AND     f.plan_end_date\n"
		+ "    and  f.form_id = fp.form_id  and fp.processinstanceid = task.PROCESSINSTANCE_ID and task.TASK_ID = 'AskForLeave.ApplicantReportBack.Task' " + 
		"UNION\n" + 
		"SELECT  f.form_id ,\n" + 
		"        f.absent_type_id ,\n" + 
		"        s.absent_type_name ,\n" + 
		"        f.reason ,\n" + 
		"        f.app_user_id ,\n" + 
		"        RTRIM(p.姓氏) + RTRIM(p.名字) AS app_user_name ,\n" + 
		"        CONVERT(VARCHAR(50), f.plan_begin_date, 23) AS plan_begin_date ,\n" + 
		"        CONVERT(VARCHAR(50), f.plan_end_date, 23) AS plan_end_date ,\n" + 
		"        2 AS type\n" + 
		"FROM    xmis.T_Absent_Form_Detail f\n" + 
		"        LEFT OUTER JOIN T_Absent_STATUS s ON f.absent_type_id = s.absent_type_id\n" + 
		"        LEFT OUTER JOIN xmis.hr_人员 p ON f.app_user_id = p.人员ID\n  ," + 
		"      xmis.t_app_form_processinstanceid_info fp,xmis.T_FF_RT_TASKINSTANCE task " + 
		"WHERE   GETDATE() < f.plan_begin_date\n" + 
		 "    and  f.form_id = fp.form_id  and fp.processinstanceid = task.PROCESSINSTANCE_ID and task.TASK_ID = 'AskForLeave.ApplicantReportBack.Task' " +
		"ORDER BY type ASC ,\n" + 
		"        f.plan_begin_date ASC";


	public static String getAbsentList = "SELECT absent_type_id,absent_type_name FROM T_Absent_STATUS";

	public static String getColumnNameCombo= 

		"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
//		"SELECT 3 AS id, '测量时间' AS columnname, 'measure_time' AS dbname,'time' AS datatype  UNION\n" + 
		"SELECT 4 AS id, '闸上水位' AS columnname, 'ap_w_level' AS dbname,'numeric' AS datatype  UNION\n" + 
		"SELECT 5 AS id, '闸下水位' AS columnname, 'bp_w_level' AS dbname,'numeric' AS datatype  UNION\n" + 
		"SELECT 6 AS id, '28孔闸开闸数量' AS columnname, 'gate_open_num_1' AS dbname,'numeric' AS datatype  UNION\n" + 
		"SELECT 7 AS id, '28空闸闸开度' AS columnname, 'gate_open_degree_1' AS dbname,'numeric' AS datatype  UNION\n" + 
		"      SELECT 8 AS id, '12孔闸开闸数量' AS columnname, 'gate_open_num_2' AS dbname,'numeric' AS datatype  UNION\n" + 
		"       SELECT 9 AS id, '12空闸闸开度' AS columnname, 'gate_open_degree_2' AS dbname,'numeric' AS datatype  UNION\n" + 
		"         SELECT 10 AS id, '电机' AS columnname, 'elec_machine' AS dbname,'numeric' AS datatype  UNION\n" + 
		"          SELECT 11 AS id, '流量' AS columnname, 'flow_qty' AS dbname,'numeric' AS datatype  UNION\n" + 
		"             SELECT 12 AS id, '水位差' AS columnname, 'w_level_chan' AS dbname,'numeric' AS datatype  UNION\n" + 
		"               SELECT 13 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
		"                 SELECT 14 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getCeYaColumnNameCombo= 

			"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
			"SELECT 3 AS id, '管水位' AS columnname, 'tubo_w_level' AS dbname,'numeric' AS datatype  UNION\n" + 
			"SELECT 4 AS id, '闸上水位' AS columnname, 'ap_w_level' AS dbname,'numeric' AS datatype  UNION\n" + 
			"SELECT 5 AS id, '闸下水位' AS columnname, 'bp_w_level' AS dbname,'numeric' AS datatype  UNION\n" + 
			"SELECT 6 AS id, '测压部位' AS columnname, 'mptp_name' AS dbname,'string' AS datatype  UNION\n "+
			"SELECT 7 AS id, '测压管名' AS columnname, 'mpt_name' AS dbname,'string' AS datatype  UNION\n "+
			"SELECT 8 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
			"SELECT 9 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";
	public static String getGczlInfoListWithGroupQuery=

		"SELECT  info.tab_seed ,\n" +
		"        info.unit_code ,\n" + 
		"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
		"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" + 
		"        info.ap_w_level ,\n" + 
		"        info.bp_w_level ,\n" + 
		"        info.gate_open_num_1 ,\n" + 
		"        info.gate_open_degree_1 ,\n" + 
		"        info.gate_open_num_2 ,\n" + 
		"        info.gate_open_degree_2 ,\n" + 
		"        info.elec_machine ,\n" + 
		"        info.flow_qty ,\n" + 
		"        info.w_level_chan ,\n" + 
		"        info.memo ,\n" + 
		"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
		"        h.人员ID AS measure_man_id\n" + 
		"        INTO #temp\n" + 
		"FROM    bbz.xmis.gczl_water_level_info info\n" + 
		"        LEFT OUTER JOIN xmis.hr_人员 h ON info.measure_man = h.人员ID";

	public static String getSexCount="SELECT "
			+ "case when  p.性别ID = 0 then '女性'  else  '男性'  END as sex"
			+ ", COUNT(*) as data FROM bbz.xmis.hr_人员  p"
			+ ", xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1 AND p.禁用 != 1 "
			+ " GROUP BY 性别ID ";

	public static String getSexList ="select  RTRIM(h.姓氏) + RTRIM(h.名字) AS name ,case when  h.性别ID = 0 then '女性'  else  '男性'  END as sex "
			+ "  FROM bbz.xmis.hr_人员  h  "
			+ "  , xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where h.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1  AND h.禁用 != 1 ";

	public static String getZzmmCount= " SELECT zzmm,COUNT(*) AS data FROM( "
		 +"  SELECT CASE WHEN p.政治面貌ID IS NULL THEN '普通群众' "
		 +"              ELSE CASE WHEN p.政治面貌ID = 3 THEN '普通群众' "
		 +"                        ELSE CASE WHEN p.政治面貌ID = 1 THEN '中央党员' "
		 +"                                  ELSE CASE WHEN p.政治面貌ID = 2 THEN '共青团员' "
		 +"                                       END "
		 +"                             END "
		 +"                   END "
		 +"         END AS zzmm , "
		 +"         1 AS qty "
		 +"  FROM   bbz.xmis.hr_人员 p , "
		 +"         xmis.org_部门归属公司表 org , "
		 +"         xmis.hr_人员部门关系表 dp "
		 +"  WHERE  p.人员ID = dp.人员ID "
		 +"         AND dp.部门ID = org.部门ID "
		 +"         AND org.公司ID = 1 "
		 +"         AND p.禁用 != 1 "
		 +" ) a "
		 +" GROUP BY zzmm ";

	public static String getZzmmList="SELECT  RTRIM(p.姓氏) + RTRIM(p.名字) AS name ,"
			+ "case when  p.性别ID = 0 then '女'  else  '男'  END as sex,"
			+" convert(varchar(10),p.入职日期,20)  as entryDate,"
			+ "DATEDIFF(yyyy,出生日期,GETDATE()) age "
			+ " FROM bbz.xmis.hr_人员  p"
			+ ", xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1 AND p.禁用 != 1 ";

	public static String getAgeCount="SELECT age,COUNT(*) as data FROM (select  "
			+ " case when  DATEDIFF(yyyy,出生日期,GETDATE()) <=30 then '30岁及以下'  else  "
			+ "  case when DATEDIFF(yyyy,出生日期,GETDATE()) >30 and DATEDIFF(yyyy,出生日期,GETDATE()) <= 40 then '31岁到40岁'  else  "
			+ "  case when DATEDIFF(yyyy,出生日期,GETDATE()) >40 and DATEDIFF(yyyy,出生日期,GETDATE()) <= 50 then '41岁到50岁'  else  "
			+ "  case when  DATEDIFF(yyyy,出生日期,GETDATE()) >50 then '50岁及以上' "
			+ "  else '出生日期为空'"
			+ " end end end end"
			+ " as age "
			+ ", 1 as qty FROM bbz.xmis.hr_人员  p"
			+ ", xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1 AND p.禁用 != 1 ) as a "
			+ "  GROUP BY  age ";

	public static String getAgeList ="SELECT  RTRIM(p.姓氏) + RTRIM(p.名字) AS name ,"
			+ "case when  p.性别ID = 0 then '女'  else  '男'  END as sex,"
			+" convert(varchar(10),p.出生日期 ,20) as birthday,"
			+ "DATEDIFF(yyyy,出生日期,GETDATE()) age "
			+ " FROM bbz.xmis.hr_人员  p"
			+ ", xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1 AND p.禁用 != 1 ";

	public static String getDutyCount=" SELECT duty ,COUNT(*) AS data FROM ( "
			+ " SELECT   CASE WHEN  dp.职务 = 1 OR dp.职务 =2 OR dp.职务 =3 THEN '高层' ELSE"
			+ "  CASE WHEN dp.职务 = 4 OR dp.职务 = 5 THEN '中层' ELSE "
			+ "  CASE WHEN dp.职务 = 6 THEN '班组长' ELSE  "
			+ "  CASE WHEN  dp.职务 = 7 OR dp.职务 is NULL  THEN '普通职工' "
			+ "  END END  END END AS duty,1 AS qty "
			+ "  FROM    [bbz].[xmis].[hr_人员] p , "
			+ "   xmis.org_部门归属公司表 org , "
			+ "  xmis.hr_人员部门关系表 dp "
			+ " WHERE   p.人员ID = dp.人员ID "
			+ "  AND dp.部门ID = org.部门ID "
			+ " AND org.公司ID = 1 "
			+ " AND p.禁用 != 1) AS a  GROUP BY duty";
	public static String getDutyList = " SELECT   RTRIM(p.姓氏)+RTRIM(p.名字) AS name,"
			+ " CASE WHEN  dp.职务 = 1 OR dp.职务 =2 OR dp.职务 =3 THEN '高层' ELSE"
			+ "  CASE WHEN dp.职务 = 4 OR dp.职务 = 5 THEN '中层' ELSE "
			+ "  CASE WHEN dp.职务 = 6 THEN '班组长' ELSE  "
			+ "  CASE WHEN  dp.职务 = 7 OR dp.职务 is NULL  THEN '普通职工' "
			+ "  END END  END END AS duty "
			+ "  FROM    [bbz].[xmis].[hr_人员] p , "
			+ "   xmis.org_部门归属公司表 org , "
			+ "  xmis.hr_人员部门关系表 dp "
			+ " WHERE   p.人员ID = dp.人员ID "
			+ "  AND dp.部门ID = org.部门ID "
			+ " AND org.公司ID = 1 "
			+ " AND p.禁用 != 1  ";

	public static String getTitlesCount=" SELECT titles ,COUNT(*) AS data FROM ( SELECT "
			+ " CASE WHEN  p.技术职务 = 3 OR p.技术职务 =4 OR p.技术职务 =5 THEN '高级职称' ELSE"
			+ " CASE WHEN  p.技术职务 = 6 OR p.技术职务 =12 OR p.技术职务 =15 THEN '中级职称' ELSE"
			+ "  CASE WHEN p.技术职务= 2 THEN '高级工' ELSE  "
			+ "  CASE WHEN p.技术职务= 14 THEN '中级工' ELSE  "
			+ " CASE WHEN  p.技术职务 = 8 OR p.技术职务 =7 OR p.技术职务 =13 OR p.技术职务 =11 OR p.技术职务 =10 OR p.技术职务  is null THEN '其他' ELSE"
			+"  CASE WHEN p.技术职务 = 16  OR p.技术职务 =17 OR p.技术职务 =18 OR p.技术职务 =19 OR p.技术职务 =20 OR p.技术职务 =21 OR p.技术职务 =22 OR p.技术职务 =9  OR p.技术职务 =1 "
			+ "  THEN '初级'"
			+ "  END END  END END  END  END AS titles "
			+ ", 1 as qty FROM bbz.xmis.hr_人员  p"
			+ ", xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID and org.公司ID=1 AND p.禁用 != 1 "
			+ ") AS a  GROUP BY titles";

	public static String getTitlesList = " SELECT RTRIM(p.姓氏)+RTRIM(p.名字) AS name,"
			+ " CASE WHEN  p.技术职务 = 3 OR p.技术职务 =4 OR p.技术职务 =5 THEN '高级职称' ELSE"
			+ " CASE WHEN  p.技术职务 = 6 OR p.技术职务 =12 OR p.技术职务 =15 THEN '中级职称' ELSE"
			+ "  CASE WHEN p.技术职务= 2 THEN '高级工' ELSE  "
			+ "  CASE WHEN p.技术职务= 14 THEN '中级工' ELSE  "
			+ " CASE WHEN  p.技术职务 = 8 OR p.技术职务 =7 OR p.技术职务 =13 OR p.技术职务 =11 OR p.技术职务 =10 OR p.技术职务  is null THEN '其他' ELSE"
			+"  CASE WHEN p.技术职务 = 16  OR p.技术职务 =17 OR p.技术职务 =18 OR p.技术职务 =19 OR p.技术职务 =20 OR p.技术职务 =21 OR p.技术职务 =22 OR p.技术职务 =9  OR p.技术职务 =1 "
			+ "  THEN '初级'"
			+ "  END END  END END  END  END AS titles "
			+ " FROM bbz.xmis.hr_人员  p"
			+ ", xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  "
			+ " where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID "
			+ " and org.公司ID=1 AND p.禁用 != 1 ";

	public static String getJsjbCount = "SELECT jsjb,COUNT(*) as data FROM( "
			+ " SELECT   CASE WHEN  p.专业技术级别 = 1 THEN '高级' ELSE "
			+ " CASE WHEN  p.专业技术级别 =2 THEN  '中级' ELSE  "
			+ "  '助理级及其他' END END AS jsjb,1 AS qty "
			+ " FROM    [bbz].[xmis].[hr_人员] p , "
			+ "  xmis.org_部门归属公司表 org , "
			+ " xmis.hr_人员部门关系表 dp "
			+ " WHERE   p.人员ID = dp.人员ID "
			+ "   AND dp.部门ID = org.部门ID "
			+ " AND org.公司ID = 1 "
			+ "   AND p.禁用 != 1 ) AS a GROUP  BY  jsjb ";

	public static String getJsjbList=" SELECT  RTRIM(p.姓氏) + RTRIM(p.名字) AS name , "
			+ "  CASE WHEN p.专业技术级别 = 1 THEN '高级' "
			+ "   ELSE CASE WHEN p.专业技术级别 = 2 THEN '中级'  ELSE '助理级及其他'  END END  as jsjb"
			+ " FROM    [bbz].[xmis].[hr_人员] p , "
			+ "  xmis.org_部门归属公司表 org , "
			+ " xmis.hr_人员部门关系表 dp "
			+ " WHERE   p.人员ID = dp.人员ID "
			+ "   AND dp.部门ID = org.部门ID "
			+ " AND org.公司ID = 1 "
			+ "   AND p.禁用 != 1 ";

	public static String getWhcdCount=" SELECT whcd,COUNT(*) as data FROM( "
			+ " SELECT   CASE WHEN  p.现有文化程度 = 2 OR  p.现有文化程度 =8 OR p.现有文化程度 =9 THEN '硕士及研究生' ELSE "
			+ "    CASE WHEN p.现有文化程度 =1 THEN '本科' ELSE "
			+ "   CASE WHEN p.现有文化程度 =4 OR p.现有文化程度 = 7 THEN '大专' ELSE "
			+ "   CASE WHEN p.现有文化程度 =5 OR p.现有文化程度 =6 OR p.现有文化程度 =10 THEN '中专' ELSE "
			+ "  CASE WHEN p.现有文化程度 =3 OR p.现有文化程度 IS NULL THEN '高中以下' "
			+ "  END END END END END AS whcd,1 AS qty "
			+ " FROM    [bbz].[xmis].[hr_人员] p , "
			+ "  xmis.org_部门归属公司表 org , "
			+ " xmis.hr_人员部门关系表 dp "
			+ " WHERE   p.人员ID = dp.人员ID "
			+ "   AND dp.部门ID = org.部门ID "
			+ " AND org.公司ID = 1 "
			+ "   AND p.禁用 != 1 ) AS a GROUP  BY  whcd ";

	public static String getWhcdList=" SELECT  RTRIM(p.姓氏) + RTRIM(p.名字) AS name , "
			+ " CASE WHEN  p.现有文化程度 = 2 OR  p.现有文化程度 =8 OR p.现有文化程度 =9 THEN '硕士及研究生' ELSE "
			+ "    CASE WHEN p.现有文化程度 =1 THEN '本科' ELSE "
			+ "   CASE WHEN p.现有文化程度 =4 OR p.现有文化程度 = 7 THEN '大专' ELSE "
			+ "   CASE WHEN p.现有文化程度 =5 OR p.现有文化程度 =6 OR p.现有文化程度 =10 THEN '中专' ELSE "
			+ "  CASE WHEN p.现有文化程度 =3 OR p.现有文化程度 IS NULL THEN '高中以下' "
			+ "  END END END END END AS whcd "
			+ " FROM    [bbz].[xmis].[hr_人员] p , "
			+ "  xmis.org_部门归属公司表 org , "
			+ " xmis.hr_人员部门关系表 dp "
			+ " WHERE   p.人员ID = dp.人员ID "
			+ "   AND dp.部门ID = org.部门ID "
			+ " AND org.公司ID = 1 "
			+ "   AND p.禁用 != 1 ";

	public static String getZgsfCount=" SELECT zgsf,COUNT(*) as data FROM( "
			+ " SELECT  CASE WHEN  p.身份 = 1 THEN '干部' ELSE  "
			+ "    CASE WHEN p.身份 =2 THEN '工人' ELSE  "
			+ "   CASE WHEN p.身份 =3  THEN '合同制工人' ELSE  "
			+ "   CASE WHEN p.身份 =4  THEN '聘用干部' ELSE "
			+ "   CASE WHEN p.身份 =5  THEN '离休' ELSE      "
			+ "  CASE WHEN p.身份 =6  THEN '退休干部' ELSE  "
			+ " CASE WHEN p.身份 =7 OR p.身份 IS NULL THEN '退休工人'"
			+ " END END END END END END END AS zgsf,1 AS qty "
			+ " FROM    [bbz].[xmis].[hr_人员] p , "
			+ "  xmis.org_部门归属公司表 org , "
			+ " xmis.hr_人员部门关系表 dp "
			+ " WHERE   p.人员ID = dp.人员ID "
			+ "   AND dp.部门ID = org.部门ID "
			+ " AND org.公司ID = 1 "
			+ "   AND p.禁用 != 1 ) AS a GROUP  BY  zgsf ";

	public static String getZgsfList ="SELECT  RTRIM(p.姓氏) + RTRIM(p.名字) AS name ,"
			+ "case when  p.性别ID = 0 then '女'  else  '男'  END as sex,"
			+" convert(varchar(10),p.出生日期 ,20) as birthday,"
			+ "DATEDIFF(yyyy,出生日期,GETDATE()) age "
			+ " FROM bbz.xmis.hr_人员  p"
			+ ", xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  "
			+ "where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID "
			+ "and org.公司ID=1 AND p.禁用 != 1 ";

	public static String getZzygCount=" SELECT zzyg,COUNT(*) as data FROM( "
			+ " SELECT  CASE WHEN  p.身份 = 1 or p.身份 = 2 or p.身份 = 3 or p.身份 = 4 THEN '在职' ELSE  "
			+ " CASE WHEN p.身份 =5 or p.身份 = 6 or p.身份 = 7 OR p.身份 IS NULL THEN '退休'"
			+ " END END zzyg,1 AS qty "
			+ " FROM    [bbz].[xmis].[hr_人员] p , "
			+ "  xmis.org_部门归属公司表 org , "
			+ " xmis.hr_人员部门关系表 dp "
			+ " WHERE   p.人员ID = dp.人员ID "
			+ "   AND dp.部门ID = org.部门ID "
			+ " AND org.公司ID = 1 "
			+ "   AND p.禁用 != 1 ) AS a GROUP  BY  zzyg ";

	public static String getZzygList="SELECT  RTRIM(p.姓氏) + RTRIM(p.名字) AS name ,"
			+ "case when  p.性别ID = 0 then '女'  else  '男'  END as sex,"
			+" convert(varchar(10),p.出生日期 ,20) as birthday,"
			+ "DATEDIFF(yyyy,出生日期,GETDATE()) age "
			+ " FROM bbz.xmis.hr_人员  p"
			+ ", xmis.org_部门归属公司表  org,xmis.hr_人员部门关系表 dp  "
			+ "where p.人员ID = dp.人员ID and dp.部门ID = org.部门ID "
			+ "and org.公司ID=1 AND p.禁用 != 1 ";

	public static String getGczlInfoExcretion="SELECT  measure_date AS time, flow_qty AS value  "
			+ "  FROM gczl_water_level_info "
			+ "  WHERE measure_date>=? "
			+ " AND measure_date<?  ORDER BY measure_date desc ";

	public static String getExcretionSum=" SELECT  SUM(a.value) AS  sum FROM ( "
			+ " SELECT  1 AS qty, flow_qty*60*60*24 AS value "
			+ " FROM gczl_water_level_info "
			+ " WHERE measure_date>=? "
			+ " AND measure_date<? ) AS a GROUP BY qty ";

	public static String getGczlInfoListForFloodNot= " DECLARE @year VARCHAR(11)  SET @year=?\n" +
			"\n" + 
			"SELECT  '闸上水位' AS title, '最大值' AS boundary,ap_w_level  AS VALUE,measure_date,'.'  as space \n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE   ap_w_level = ( SELECT   MAX(ap_w_level)\n" + 
			" FROM     gczl_water_level_info\n "
			+ " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-01') "
			+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
			+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
			+ "   WHERE KEY_NAME = '非汛期' ) )   "
			+ " ) "
			+ "   OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'+ ( SELECT first_value FROM gczl_syscontrol "
			+ "   WHERE KEY_NAME = '非汛期'  ) ) "
			+ " AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
			+ "  ) "
			+ "   ) "
			+ "   ) "
			+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT  @year + '-01' ) "
			+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
			+ "  WHERE  KEY_NAME = '非汛期' ) ) "
			+ " ) "
			+ "  OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'  + ( SELECT first_value  FROM gczl_syscontrol "
			+ "  WHERE  KEY_NAME = '非汛期' ) ) "
			+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
			+ " ) "
			+ " ) " + 
			"\n" + 
			"UNION\n" + 
			"\n" + 
			"SELECT '闸下水位' AS title,  '最大值' AS boundary,bp_w_level    AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE bp_w_level=(SELECT   MAX(bp_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-01') "
			+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
			+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
			+ "   WHERE KEY_NAME = '非汛期' ) )   "
			+ " ) "
			+ "   OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'+ ( SELECT first_value FROM gczl_syscontrol "
			+ "   WHERE KEY_NAME = '非汛期'  ) ) "
			+ " AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
			+ "  ) "
			+ "   ) "
			+ "   ) "
			+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT  @year + '-01' ) "
			+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
			+ "  WHERE  KEY_NAME = '非汛期' ) ) "
			+ " ) "
			+ "  OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'  + ( SELECT first_value  FROM gczl_syscontrol "
			+ "  WHERE  KEY_NAME = '非汛期' ) ) "
			+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
			+ " ) "
			+ " ) " + 
			"\n" + 
			"UNION\n" + 
			"SELECT   '闸上水位' AS title, '最小值' AS boundary,ap_w_level  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE ap_w_level=(SELECT  MIN(ap_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-01') "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期' ) )   "
				+ " ) "
				+ "   OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'+ ( SELECT first_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期'  ) ) "
				+ " AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ "  ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT  @year + '-01' ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ " ) "
				+ "  OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'  + ( SELECT first_value  FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"SELECT   '闸下水位' AS title, '最小值' AS boundary,bp_w_level  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE bp_w_level=(SELECT  MIN(bp_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-01') "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期' ) )   "
				+ " ) "
				+ "   OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'+ ( SELECT first_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期'  ) ) "
				+ " AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ "  ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT  @year + '-01' ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ " ) "
				+ "  OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'  + ( SELECT first_value  FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"\n" + 
			"\n" + 
			"SELECT  '水位差' AS title, '最大值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE w_level_chan=(SELECT   MAX(w_level_chan)\n" + 
			"FROM gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-01') "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期' ) )   "
				+ " ) "
				+ "   OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'+ ( SELECT first_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期'  ) ) "
				+ " AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ "  ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT  @year + '-01' ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ " ) "
				+ "  OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'  + ( SELECT first_value  FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"\n" + 
			"SELECT   '水位差' AS title, '最小值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE w_level_chan=(SELECT  Min(w_level_chan)\n" + 
			"FROM gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-01') "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期' ) )   "
				+ " ) "
				+ "   OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'+ ( SELECT first_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期'  ) ) "
				+ " AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ "  ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT  @year + '-01' ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ " ) "
				+ "  OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'  + ( SELECT first_value  FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"SELECT   '流量' AS title, '最大值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE flow_qty=(SELECT   MAX(flow_qty)\n" + 
			"FROM gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-01') "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期' ) )   "
				+ " ) "
				+ "   OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'+ ( SELECT first_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期'  ) ) "
				+ " AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ "  ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT  @year + '-01' ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ " ) "
				+ "  OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'  + ( SELECT first_value  FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"SELECT   '流量' AS title, '最小值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE flow_qty=(SELECT   MIN(flow_qty)\n" + 
			"FROM gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-01') "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期' ) )   "
				+ " ) "
				+ "   OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'+ ( SELECT first_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '非汛期'  ) ) "
				+ " AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ "  ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT  @year + '-01' ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ " ) "
				+ "  OR ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-'  + ( SELECT first_value  FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '非汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= @year + '-12' "
				+ " ) "
				+ " ) ";
	public static String getGczlInfoListForFlood= " DECLARE @year VARCHAR(11)  SET @year=?\n" +
			"\n" + 
			"SELECT  '闸上水位' AS title, '最大值' AS boundary,ap_w_level  AS VALUE,measure_date,'.'  as space \n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE   ap_w_level = ( SELECT   MAX(ap_w_level)\n" + 
			" FROM     gczl_water_level_info\n "
			+ " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
			+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
			+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
			+ "   WHERE KEY_NAME = '汛期' ) )   "
			+ " ) "
			+ "   ) "
			+ "   ) "
			+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
			+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
			+ "  WHERE  KEY_NAME = '汛期' ) ) "
			+ " ) "
			+ " ) " + 
			"\n" + 
			"UNION\n" + 
			"\n" + 
			"SELECT '闸下水位' AS title,  '最大值' AS boundary,bp_w_level    AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE bp_w_level=(SELECT   MAX(bp_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
			+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
			+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
			+ "   WHERE KEY_NAME = '汛期' ) )   "
			+ " ) "
			+ "   ) "
			+ "   ) "
			+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
			+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
			+ "  WHERE  KEY_NAME = '汛期' ) ) "
			+ " ) "
			+ " ) " + 
			"\n" + 
			"UNION\n" + 
			"SELECT   '闸上水位' AS title, '最小值' AS boundary,ap_w_level  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE ap_w_level=(SELECT  MIN(ap_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '汛期' ) )   "
				+ " ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '汛期' ) ) "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"SELECT   '闸下水位' AS title, '最小值' AS boundary,bp_w_level  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE bp_w_level=(SELECT  MIN(bp_w_level)\n" + 
			"FROM  gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '汛期' ) )   "
				+ " ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '汛期' ) ) "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"\n" + 
			"\n" + 
			"SELECT  '水位差' AS title, '最大值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE w_level_chan=(SELECT   MAX(w_level_chan)\n" + 
			"FROM gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '汛期' ) )   "
				+ " ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '汛期' ) ) "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"\n" + 
			"SELECT   '水位差' AS title, '最小值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE w_level_chan=(SELECT  Min(w_level_chan)\n" + 
			"FROM gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '汛期' ) )   "
				+ " ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '汛期' ) ) "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"SELECT   '流量' AS title, '最大值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE flow_qty=(SELECT   MAX(flow_qty)\n" + 
			"FROM gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '汛期' ) )   "
				+ " ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '汛期' ) ) "
				+ " ) "
				+ " ) " + 
				"\n" + 
			"UNION\n" + 
			"\n" + 
			"SELECT   '流量' AS title, '最小值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
			"FROM gczl_water_level_info\n" + 
			"WHERE flow_qty=(SELECT   MIN(flow_qty)\n" + 
			"FROM gczl_water_level_info\n" + 
			 " WHERE    ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "    AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT "
				+ "   @year + '-' + ( SELECT  secord_value FROM gczl_syscontrol "
				+ "   WHERE KEY_NAME = '汛期' ) )   "
				+ " ) "
				+ "   ) "
				+ "   ) "
				+ "  AND ( ( CONVERT(VARCHAR(7), measure_date, 20) >= ( SELECT @year + '-' + ( SELECT  first_value FROM gczl_syscontrol WHERE KEY_NAME = '汛期' ) ) "
				+ "  AND CONVERT(VARCHAR(7), measure_date, 20) <= ( SELECT @year + '-' + ( SELECT secord_value FROM gczl_syscontrol "
				+ "  WHERE  KEY_NAME = '汛期' ) ) "
				+ " ) "
				+ " ) " + 
				"\n";

	public static String getGczlInfoListForFloodGatesNot = 
			" DECLARE @b_date VARCHAR(11) , @e_date VARCHAR(11) SET @b_date=? SET @e_date=?\n" +
					"\n" + 
					"SELECT  '闸上水位' AS title, '最大值' AS boundary,ap_w_level  AS VALUE,measure_date,'.'  as space \n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE ap_w_level=(SELECT   MAX(ap_w_level)\n" + 
					"FROM  gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT '闸下水位' AS title,  '最大值' AS boundary,bp_w_level    AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE bp_w_level=(SELECT   MAX(bp_w_level)\n" + 
					"FROM  gczl_water_level_info\n" + 
					
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					"\n" + 
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT   '闸上水位' AS title, '最小值' AS boundary,ap_w_level  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE ap_w_level=(SELECT  MIN(ap_w_level)\n" + 
					"FROM  gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT   '闸下水位' AS title, '最小值' AS boundary,bp_w_level  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE bp_w_level=(SELECT  MIN(bp_w_level)\n" + 
					"FROM  gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					"\n" + 
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"\n" + 
					"\n" + 
					"SELECT  '水位差' AS title, '最大值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE w_level_chan=(SELECT   MAX(w_level_chan)\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"\n" + 
					"SELECT   '水位差' AS title, '最小值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE w_level_chan=(SELECT  Min(w_level_chan)\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT   '流量' AS title, '最大值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE flow_qty=(SELECT   MAX(flow_qty)\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT   '流量' AS title, '最小值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE flow_qty=(SELECT   MIN(flow_qty)\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)\n" + 
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 =0 OR gate_open_num_1 IS NULL) AND (gate_open_num_2 =0 OR gate_open_num_2 IS NULL)";


	public static String getGczlInfoListForFloodGates  = 
			" DECLARE @b_date VARCHAR(11) , @e_date VARCHAR(11) SET @b_date=? SET @e_date=?\n" +
					"\n" + 
					"SELECT  '闸上水位' AS title, '最大值' AS boundary,ap_w_level  AS VALUE,measure_date,'.'  as space \n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE ap_w_level=(SELECT   MAX(ap_w_level)\n" + 
					"FROM  gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" + 
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT '闸下水位' AS title,  '最大值' AS boundary,bp_w_level    AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE bp_w_level=(SELECT   MAX(bp_w_level)\n" + 
					"FROM  gczl_water_level_info\n" + 
					
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					"\n" + 
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT   '闸上水位' AS title, '最小值' AS boundary,ap_w_level  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE ap_w_level=(SELECT  MIN(ap_w_level)\n" + 
					"FROM  gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT   '闸下水位' AS title, '最小值' AS boundary,bp_w_level  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE bp_w_level=(SELECT  MIN(bp_w_level)\n" + 
					"FROM  gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					"\n" + 
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"\n" + 
					"\n" + 
					"SELECT  '水位差' AS title, '最大值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE w_level_chan=(SELECT   MAX(w_level_chan)\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"\n" + 
					"SELECT   '水位差' AS title, '最小值' AS boundary,w_level_chan  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE w_level_chan=(SELECT  Min(w_level_chan)\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT   '流量' AS title, '最大值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE flow_qty=(SELECT   MAX(flow_qty)\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					"\n" + 
					"UNION\n" + 
					"\n" + 
					"SELECT   '流量' AS title, '最小值' AS boundary,flow_qty  AS value,measure_date,'.'  as space\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE flow_qty=(SELECT   MIN(flow_qty)\n" + 
					"FROM gczl_water_level_info\n" + 
					"WHERE measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n" +
					")\n" + 
					"AND measure_date>=@b_date\n" + 
					"AND measure_date<@e_date AND (gate_open_num_1 !=0 AND gate_open_num_1 IS NOT NULL) AND (gate_open_num_2 !=0 OR gate_open_num_2 IS NOT NULL)\n";

	public static String getGczlInfoEveryMonthMaxLevel=" SET NOCOUNT ON "
			+ " CREATE TABLE #result1 "
			+ " ( "
			+ " 年份  VARCHAR(20) , "
			+ "  项目  VARCHAR(20) , "
			+ "    [1月] DECIMAL(9,2), "
			+ "  [2月] DECIMAL(9,2), "
			+ "  [3月] DECIMAL(9,2),"
			+ " [4月] DECIMAL(9,2),"
			+ " [5月] DECIMAL(9,2),"
			+ " [6月] DECIMAL(9,2),"
			+ "  [7月] DECIMAL(9,2),"
			+ " [8月] DECIMAL(9,2), "
			+ " [9月] DECIMAL(9,2),"
			+ " [10月] DECIMAL(9,2),"
			+ " [11月] DECIMAL(9,2),"
			+ "  [12月] DECIMAL(9,2)"
			+ "    )"
			+ "\n"
			+ " DECLARE @b_date VARCHAR(4) ,"
			+ "  @e_date VARCHAR(4) "
			+ " SET @b_date = ? "
			+ " SET @e_date = ? "
			+ " WHILE @e_date >= @b_date  "
			+ "   BEGIN "
			+ " INSERT  INTO #result1 "
			+ "  SELECT  DATEPART(YEAR, measure_date) AS 年份,"
			+ "  '闸上水位' AS 项目 , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 1 THEN ap_w_level "
			+ " ELSE NULL "
			+ "  END AS '1月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 2 THEN ap_w_level "
			+ " ELSE NULL "
			+ "  END AS '2月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 3 THEN ap_w_level  "
			+ "  ELSE NULL "
			+ " END AS '3月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 4 THEN ap_w_level "
			+ " ELSE NULL "
			+ "  END AS '4月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 5 THEN ap_w_level "
			+ "  ELSE NULL "
			+ "  END AS '5月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 6 THEN ap_w_level "
			+ " ELSE NULL"
			+ "   END AS '6月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 7 THEN ap_w_level "
			+ " ELSE NULL "
			+ " END AS '7月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 8 THEN ap_w_level "
			+ " ELSE NULL"
			+ "  END AS '8月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 9 THEN ap_w_level "
			+ " ELSE NULL"
			+ "  END AS '9月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 10 THEN ap_w_level"
			+ "  ELSE NULL "
			+ " END AS '10月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 11 THEN ap_w_level"
			+ "  ELSE NULL "
			+ "  END AS '11月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 12 THEN ap_w_level "
			+ " ELSE NULL "
			+ " END AS '12月' "
			+ "  FROM    xmis.gczl_water_level_info "
			+ "  WHERE   DATEPART(YEAR, measure_date) = @b_date"
			+ "  UNION ALL"
			+ " SELECT  DATEPART(YEAR, measure_date) AS 年份 , "
			+ "  '闸下水位' AS 项目 , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 1 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '1月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 2 THEN bp_w_level"
			+ "  ELSE NULL "
			+ "  END AS '2月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 3 THEN bp_w_level "
			+ "  ELSE NULL "
			+ " END AS '3月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 4 THEN bp_w_level"
			+ "  ELSE NULL "
			+ " END AS '4月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 5 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '5月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 6 THEN bp_w_level "
			+ "  ELSE NULL"
			+ "  END AS '6月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 7 THEN bp_w_level"
			+ "  ELSE NULL "
			+ " END AS '7月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 8 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '8月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 9 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '9月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 10 THEN bp_w_level"
			+ " ELSE NULL"
			+ "  END AS '10月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 11 THEN bp_w_level"
			+ " ELSE NULL"
			+ "  END AS '11月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 12 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '12月'"
			+ " FROM    xmis.gczl_water_level_info"
			+ " WHERE   DATEPART(YEAR, measure_date) = @b_date"
			+ " SET @b_date = @b_date + 1"
			+ " END "
			+ "\n  "
			+"SELECT  年份 ,项目 ,"
			+ "  MAX([1月]) AS '1月' ,"
			+ " MAX([2月]) AS '2月' ,"
			+ "  MAX([3月]) AS '3月' ,"
			+ "  MAX([4月]) AS '4月' ,"
			+ "  MAX([5月]) AS '5月' ,"
			+ "  MAX([6月]) AS '6月' ,"
			+ "  MAX([7月]) AS '7月' ,"
			+ "  MAX([8月]) AS '8月' ,"
			+ " MAX([9月]) AS '9月' ,"
			+ "  MAX([10月]) AS '10月' ,"
			+ "  MAX([11月]) AS '11月' ,"
			+ "  MAX([12月]) AS '12月'"
			+ " FROM    #result1 "
			+ " GROUP BY  年份,项目  "
			+ " ORDER BY 年份  desc,项目 "
			;
	public static String getGczlInfoEveryMonthMinLevel=" SET NOCOUNT ON "
			+ " CREATE TABLE #result1 "
			+ " ( "
			+ " 年份  VARCHAR(20) , "
			+ "  项目  VARCHAR(20) , "
			+ "    [1月] DECIMAL(9,2), "
			+ "  [2月] DECIMAL(9,2), "
			+ "  [3月] DECIMAL(9,2),"
			+ " [4月] DECIMAL(9,2),"
			+ " [5月] DECIMAL(9,2),"
			+ " [6月] DECIMAL(9,2),"
			+ "  [7月] DECIMAL(9,2),"
			+ " [8月] DECIMAL(9,2), "
			+ " [9月] DECIMAL(9,2),"
			+ " [10月] DECIMAL(9,2),"
			+ " [11月] DECIMAL(9,2),"
			+ "  [12月] DECIMAL(9,2)"
			+ "    )"
			+ "\n"
			+ " DECLARE @b_date VARCHAR(4) ,"
			+ "  @e_date VARCHAR(4) "
			+ " SET @b_date = ? "
			+ " SET @e_date = ? "
			+ " WHILE @e_date >= @b_date  "
			+ "   BEGIN "
			+ " INSERT  INTO #result1 "
			+ "  SELECT  DATEPART(YEAR, measure_date) AS 年份,"
			+ "  '闸上水位' AS 项目 , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 1 THEN ap_w_level "
			+ " ELSE NULL "
			+ "  END AS '1月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 2 THEN ap_w_level "
			+ " ELSE NULL "
			+ "  END AS '2月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 3 THEN ap_w_level  "
			+ "  ELSE NULL "
			+ " END AS '3月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 4 THEN ap_w_level "
			+ " ELSE NULL "
			+ "  END AS '4月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 5 THEN ap_w_level "
			+ "  ELSE NULL "
			+ "  END AS '5月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 6 THEN ap_w_level "
			+ " ELSE NULL"
			+ "   END AS '6月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 7 THEN ap_w_level "
			+ " ELSE NULL "
			+ " END AS '7月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 8 THEN ap_w_level "
			+ " ELSE NULL"
			+ "  END AS '8月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 9 THEN ap_w_level "
			+ " ELSE NULL"
			+ "  END AS '9月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 10 THEN ap_w_level"
			+ "  ELSE NULL "
			+ " END AS '10月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 11 THEN ap_w_level"
			+ "  ELSE NULL "
			+ "  END AS '11月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 12 THEN ap_w_level "
			+ " ELSE NULL "
			+ " END AS '12月' "
			+ "  FROM    xmis.gczl_water_level_info "
			+ "  WHERE   DATEPART(YEAR, measure_date) = @b_date"
			+ "  UNION ALL"
			+ " SELECT  DATEPART(YEAR, measure_date) AS 年份 , "
			+ "  '闸下水位' AS 项目 , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 1 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '1月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 2 THEN bp_w_level"
			+ "  ELSE NULL "
			+ "  END AS '2月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 3 THEN bp_w_level "
			+ "  ELSE NULL "
			+ " END AS '3月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 4 THEN bp_w_level"
			+ "  ELSE NULL "
			+ " END AS '4月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 5 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '5月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 6 THEN bp_w_level "
			+ "  ELSE NULL"
			+ "  END AS '6月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 7 THEN bp_w_level"
			+ "  ELSE NULL "
			+ " END AS '7月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 8 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '8月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 9 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '9月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 10 THEN bp_w_level"
			+ " ELSE NULL"
			+ "  END AS '10月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 11 THEN bp_w_level"
			+ " ELSE NULL"
			+ "  END AS '11月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 12 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '12月'"
			+ " FROM    xmis.gczl_water_level_info"
			+ " WHERE   DATEPART(YEAR, measure_date) = @b_date"
			+ " SET @b_date = @b_date + 1"
			+ " END "
			+ "\n  "
			+"SELECT  年份 ,项目 ,"
			+ "  MIN([1月]) AS '1月' ,"
			+ " MIN([2月]) AS '2月' ,"
			+ "  MIN([3月]) AS '3月' ,"
			+ "  MIN([4月]) AS '4月' ,"
			+ "  MIN([5月]) AS '5月' ,"
			+ "  MIN([6月]) AS '6月' ,"
			+ "  MIN([7月]) AS '7月' ,"
			+ "  MIN([8月]) AS '8月' ,"
			+ " MIN([9月]) AS '9月' ,"
			+ "  MIN([10月]) AS '10月' ,"
			+ "  MIN([11月]) AS '11月' ,"
			+ "  MIN([12月]) AS '12月'"
			+ " FROM    #result1 "
			+ " GROUP BY  年份,项目  "
			+ " ORDER BY 年份  desc,项目 "
			;
	public static String getGczlInfoEveryMonthAvgLevel=" SET NOCOUNT ON "
			+ " CREATE TABLE #result1 "
			+ " ( "
			+ " 年份  VARCHAR(20) , "
			+ "  项目  VARCHAR(20) , "
			+ "    [1月] DECIMAL(9,2), "
			+ "  [2月] DECIMAL(9,2), "
			+ "  [3月] DECIMAL(9,2),"
			+ " [4月] DECIMAL(9,2),"
			+ " [5月] DECIMAL(9,2),"
			+ " [6月] DECIMAL(9,2),"
			+ "  [7月] DECIMAL(9,2),"
			+ " [8月] DECIMAL(9,2), "
			+ " [9月] DECIMAL(9,2),"
			+ " [10月] DECIMAL(9,2),"
			+ " [11月] DECIMAL(9,2),"
			+ "  [12月] DECIMAL(9,2)"
			+ "    )"
			+ "\n"
			+ " DECLARE @b_date VARCHAR(4) ,"
			+ "  @e_date VARCHAR(4) "
			+ " SET @b_date = ? "
			+ " SET @e_date = ? "
			+ " WHILE @e_date >= @b_date  "
			+ "   BEGIN "
			+ " INSERT  INTO #result1 "
			+ "  SELECT  DATEPART(YEAR, measure_date) AS 年份,"
			+ "  '闸上水位' AS 项目 , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 1 THEN ap_w_level "
			+ " ELSE NULL "
			+ "  END AS '1月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 2 THEN ap_w_level "
			+ " ELSE NULL "
			+ "  END AS '2月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 3 THEN ap_w_level  "
			+ "  ELSE NULL "
			+ " END AS '3月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 4 THEN ap_w_level "
			+ " ELSE NULL "
			+ "  END AS '4月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 5 THEN ap_w_level "
			+ "  ELSE NULL "
			+ "  END AS '5月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 6 THEN ap_w_level "
			+ " ELSE NULL"
			+ "   END AS '6月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 7 THEN ap_w_level "
			+ " ELSE NULL "
			+ " END AS '7月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 8 THEN ap_w_level "
			+ " ELSE NULL"
			+ "  END AS '8月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 9 THEN ap_w_level "
			+ " ELSE NULL"
			+ "  END AS '9月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 10 THEN ap_w_level"
			+ "  ELSE NULL "
			+ " END AS '10月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 11 THEN ap_w_level"
			+ "  ELSE NULL "
			+ "  END AS '11月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 12 THEN ap_w_level "
			+ " ELSE NULL "
			+ " END AS '12月' "
			+ "  FROM    xmis.gczl_water_level_info "
			+ "  WHERE   DATEPART(YEAR, measure_date) = @b_date"
			+ "  UNION ALL"
			+ " SELECT  DATEPART(YEAR, measure_date) AS 年份 , "
			+ "  '闸下水位' AS 项目 , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 1 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '1月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 2 THEN bp_w_level"
			+ "  ELSE NULL "
			+ "  END AS '2月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 3 THEN bp_w_level "
			+ "  ELSE NULL "
			+ " END AS '3月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 4 THEN bp_w_level"
			+ "  ELSE NULL "
			+ " END AS '4月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 5 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '5月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 6 THEN bp_w_level "
			+ "  ELSE NULL"
			+ "  END AS '6月' , "
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 7 THEN bp_w_level"
			+ "  ELSE NULL "
			+ " END AS '7月' , "
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 8 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '8月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 9 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '9月' ,"
			+ "  CASE WHEN DATEPART(MONTH, measure_date) = 10 THEN bp_w_level"
			+ " ELSE NULL"
			+ "  END AS '10月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 11 THEN bp_w_level"
			+ " ELSE NULL"
			+ "  END AS '11月' ,"
			+ " CASE WHEN DATEPART(MONTH, measure_date) = 12 THEN bp_w_level"
			+ "  ELSE NULL"
			+ " END AS '12月'"
			+ " FROM    xmis.gczl_water_level_info"
			+ " WHERE   DATEPART(YEAR, measure_date) = @b_date"
			+ " SET @b_date = @b_date + 1"
			+ " END "
			+ "\n  "
			+"SELECT  年份 ,项目 ,"
			+ "  AVG([1月]) AS '1月' ,"
			+ " AVG([2月]) AS '2月' ,"
			+ "  AVG([3月]) AS '3月' ,"
			+ "  AVG([4月]) AS '4月' ,"
			+ "  AVG([5月]) AS '5月' ,"
			+ "  AVG([6月]) AS '6月' ,"
			+ "  AVG([7月]) AS '7月' ,"
			+ "  AVG([8月]) AS '8月' ,"
			+ " AVG([9月]) AS '9月' ,"
			+ "  AVG([10月]) AS '10月' ,"
			+ "  AVG([11月]) AS '11月' ,"
			+ "  AVG([12月]) AS '12月'"
			+ " FROM    #result1 "
			+ " GROUP BY  年份,项目  "
			+ " ORDER BY 年份  desc,项目 "
			;

	public static String getMptpCodeList="select mptp_code as mptp_code ,mptp_name as mptp_name from [xmis].[gczl_measure_press_tube_place_dict]";

	public static String getMptCodeList="select mpt_code,mpt_name from [xmis].[gczl_measure_press_tube_dict] where mptp_code = ?";

	public static String getMaxIDFromCyInfo="SELECT MAX(tab_seed) FROM bbz.xmis.gczl_measure_press_tube_informatiom";

	public static String getMpt_codeFromCyInfo="select mpt_code FROM bbz.xmis.gczl_measure_press_tube_dict where mpt_name =? and mptp_code = ?";

	public static String doDeleteCyInfoById = "delete FROM bbz.xmis.gczl_measure_press_tube_informatiom WHERE tab_seed=?";
	
	public static String getCyInfoListWithGroupQuery=

			"SELECT  info.tab_seed ,\n" +
			"        info.unit_code ,\n" + 
			"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
			"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" + 
			"        info.ap_w_level ,\n" + 
			"        info.bp_w_level ,\n" + 
			"        info.tubo_w_level ,\n" + 
			"        info.mptp_code ,\n" + 
			"        info.mpt_name,\n" + 
			"        info.memo ,"+ 
			" 		 case when  ap_w_level = bp_w_level then null else "
			+ "  convert(numeric(16,2), round( (tubo_w_level-bp_w_level)/(ap_w_level-bp_w_level)*100,2))  end as sensitivity,"+
			"        mptp.mptp_name as mptp_name,\n" + 
			"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
			"        h.人员ID AS measure_man_id\n" + 
			"        INTO #temp\n" + 
			"FROM    bbz.xmis.gczl_measure_press_tube_informatiom info\n" + 
			"        LEFT OUTER JOIN xmis.hr_人员 h ON info.measure_man = h.人员ID  "
			+ "      Left outer join xmis.gczl_measure_press_tube_place_dict mptp on info.mptp_code = mptp.mptp_code";

	public static String getFsInfoListWithGroupQuery = 
			"SELECT  info.tab_seed ,\n" +
					"        info.unit_code ,\n" + 
					"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
					"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" + 
					"        info.fs_code ,\n" + 
					"        info.seam_width ,\n" + 
					"        fs.fs_name,\n "+
					" 		 info.weather,"+ 
					"        info.memo ,"+ 
					"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
					"        h.人员ID AS measure_man_id\n" + 
					"        INTO #temp\n" + 
					"FROM    bbz.xmis.gczl_flex_seam_info info\n" + 
					"        LEFT OUTER JOIN xmis.hr_人员 h ON info.measure_man = h.人员ID  "
					+ "      Left outer join xmis.gczl_flex_seam_dict fs on info.fs_code = fs.fs_code";

	public static String getFsCodeList= "select fs_code,fs_name from [xmis].[gczl_flex_seam_dict]";

	public static String getMaxIDFromFsInfo="SELECT MAX(tab_seed) FROM bbz.xmis.gczl_flex_seam_info";

	public static String doDeleteFsInfoById= "delete FROM bbz.xmis.gczl_flex_seam_info WHERE tab_seed=?";

	public static String getFsColumnNameCombo= 
			"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
					"SELECT 4 AS id, '伸缩缝名称' AS columnname, 'fs_name' AS dbname,'string' AS datatype  UNION\n "+
					"SELECT 5 AS id, '缝宽值' AS columnname, 'seam_width' AS dbname,'numeric' AS datatype  UNION\n" + 
					"SELECT 6 AS id, '气温' AS columnname, 'weather' AS dbname,'numeric' AS datatype  UNION\n" +
					"SELECT 7 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
					"SELECT 8 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getSpCodeList ="select sp_code,sp_name from [bbz].[xmis].[gczl_sink_place_dict]";

	public static String getS_point_codeList="select s_point_code,s_point_name+' ['+s_point_code+']' as s_point_name from [bbz].[xmis].[gczl_sink_point_dict] where sp_code = ?";

	public static String getS_point_nameFromSinkInfo = "select s_point_name from [bbz].[xmis].[gczl_sink_point_dict] where s_point_code = ? and sp_code = ?";

	public static String getMaxIDFromSinkInfo = "SELECT MAX(tab_seed) FROM bbz.xmis.gczl_sink_informatiom";

	public static String doDeleteSinkInfoById ="delete FROM bbz.xmis.gczl_sink_informatiom WHERE tab_seed=?";

	public static String getSinkInfoListWithGroupQuery = 
			"SELECT  info.tab_seed ,\n" +
					"        info.unit_code ,\n" + 
					"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
					"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" + 
					"        info.ap_w_level ,\n" + 
					"        info.bp_w_level ,\n" + 
					"        info.high_degree ,\n" + 
					"        info.s_point_code,\n" + 
					"        info.s_point_name,\n" + 
					"        info.memo ,"+ 
					"        info.sp_code ,"+ 
					"        sp.sp_name as sp_name,\n" + 
					"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
					"        h.人员ID AS measure_man_id\n" + 
					"        INTO #temp\n" + 
					"FROM    bbz.xmis.gczl_sink_informatiom info\n" + 
					"        LEFT OUTER JOIN xmis.hr_人员 h ON info.measure_man = h.人员ID  "
					+ "      Left outer join xmis.gczl_sink_place_dict sp on info.sp_code = sp.sp_code ";

	public static String getSinkColumnNameCombo =
			"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
			"SELECT 3 AS id, '高程' AS columnname, 'high_degree' AS dbname,'numeric' AS datatype  UNION\n" + 
			"SELECT 4 AS id, '闸上水位' AS columnname, 'ap_w_level' AS dbname,'numeric' AS datatype  UNION\n" + 
			"SELECT 5 AS id, '闸下水位' AS columnname, 'bp_w_level' AS dbname,'numeric' AS datatype  UNION\n" + 
			"SELECT 6 AS id, '部位名称' AS columnname, 'sp_name' AS dbname,'string' AS datatype  UNION\n "+
			"SELECT 7 AS id, '沉陷点' AS columnname, 's_point_name' AS dbname,'string' AS datatype  UNION\n "+
			"SELECT 8 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
			"SELECT 9 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getUnitCodeFormUnitInfo = "select unit_code from [bbz].[xmis].[gczl_unit_info]";

	public static String getWeatherCodeList = "select [weather_code],[weather_state] from [bbz].[xmis].[gczl_weather_state_dict]";

	public static String getMaxIDFromWeatherInfo = "SELECT MAX(tab_seed) FROM bbz.xmis.gczl_air_temperature_info";
	
	public static String doDeleteWeatherInfoById="delete from bbz.xmis.gczl_air_temperature_info where tab_seed=?";

	public static String getWeatherInfoListWithGroupQuery = "SELECT  info.tab_seed ,\n" +
	"        info.unit_code ,\n" + 
	"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
	"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" + 
	"        case when st.weather_state is null then info.sky_status else st.weather_state end as sky_status,\n" + 
	" 		 info.weather,"+ 
	"        info.memo ,"+ 
	"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
	"        h.人员ID AS measure_man_id\n" + 
	"        INTO #temp\n" + 
	"FROM    bbz.xmis.gczl_air_temperature_info info\n" + 
	"        LEFT OUTER JOIN xmis.hr_人员 h ON info.measure_man = h.人员ID " +
	"		LEFT OUTER JOIN xmis.gczl_weather_state_dict st on st.weather_code = info.sky_status";

	public static String getWeatherColumnNameCombo= 
		"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
		"SELECT 4 AS id, '天气状况' AS columnname, 'sky_status' AS dbname,'string' AS datatype  UNION\n "+
		"SELECT 6 AS id, '气温' AS columnname, 'weather' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 7 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
		"SELECT 8 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getRiverDmList=" select pillar_code,pillar_name +' ['+pillar_code+']' as pillar_name FROM [bbz].[xmis].[gczl_pillar_code_dict]";

	public static String getMaxIDFromRiverDmInfo= "SELECT MAX(tab_seed) FROM bbz.xmis.gczl_river_dm_info";
	
	public static String doDeleteRiverDmInfoById="delete FROM [bbz].[xmis].[gczl_river_dm_info] where tab_seed=?";

	public static String getRiverDmInfoListWithGroupQuery= "SELECT  info.tab_seed ,\n" +
	"        info.unit_code ,\n" + 
	"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
	"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" +
	"		 info.pillar_code," +
	"		 st.pillar_name," + 
	" 		 info.distance," +
	"		 info.high_degree,"+ 
	"        info.memo ,"+ 
	"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
	"        h.人员ID AS measure_man_id\n" + 
	"        INTO #temp\n" + 
	"FROM    bbz.xmis.gczl_river_dm_info info\n" + 
	"        LEFT OUTER JOIN xmis.hr_人员 h ON info.measure_man = h.人员ID " +
	"		LEFT OUTER JOIN xmis.gczl_pillar_code_dict st on st.pillar_code = info.pillar_code";

	public static String getRiverDmColumnNameCombo= 
		"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
		"SELECT 4 AS id, '桩号' AS columnname, 'pillar_code' AS dbname,'string' AS datatype  UNION\n "+
		"SELECT 5 AS id, '距离' AS columnname, 'distance' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 6 AS id, '高程' AS columnname, 'high_degree' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 7 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
		"SELECT 8 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getConcreteCarbList=" select concrete_code as place_code,concrete_place FROM [bbz].[xmis].[gczl_concrete_place_dict]";

	public static String getMaxIDFromConcreteCarbInfo= "SELECT MAX(tab_seed) FROM bbz.xmis.gczl_concrete_carb_table";

	public static String doDeleteConcreteCarbInfoById="delete FROM [bbz].[xmis].[gczl_concrete_carb_table] where tab_seed=?";

	public static String getConcreteCarbInfoListWithGroupQuery= "SELECT  info.tab_seed ,\n" +
	"        info.unit_code ,\n" + 
	"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
	"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" +
	"		 info.place_code," +
	"		 st.concrete_place," + 
	" 		 info.carbonize_depth," +
	"		 info.allow_carb_depth,"+ 
	"        info.memo ,"+ 
	"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
	"        h.人员ID AS measure_man_id\n" + 
	"        INTO #temp\n" + 
	"FROM    bbz.xmis.gczl_concrete_carb_table info\n" + 
	"        LEFT OUTER JOIN xmis.hr_人员 h ON info.op_code = h.人员ID " +
	"		LEFT OUTER JOIN xmis.gczl_concrete_place_dict st on st.concrete_code = info.place_code";

	public static String getConcreteCarbColumnNameCombo= 
		"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
		"SELECT 4 AS id, '测量部位' AS columnname, 'concrete_place' AS dbname,'string' AS datatype  UNION\n "+
		"SELECT 5 AS id, '碳化深度' AS columnname, 'carbonize_depth' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 6 AS id, '允许深度' AS columnname, 'allow_carb_depth' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 7 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
		"SELECT 8 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getSteelSCTList=" select steel_stru_code as place_code,steel_stru_place FROM [bbz].[xmis].[gczl_steel_stru_place_dict]";

	public static String getMaxIDFromSteelSCTInfo= "SELECT MAX(tab_seed) FROM [bbz].[xmis].[gczl_steel_stru_corr_tarnishing]";

	public static String doDeleteSteelSCTInfoById="delete FROM [bbz].[xmis].[gczl_steel_stru_corr_tarnishing] where tab_seed=?";

	public static String getSteelSCTInfoListWithGroupQuery= "SELECT  info.tab_seed ,\n" +
	"        info.unit_code ,\n" + 
	"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
	"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" +
	"		 info.place_code," +
	"		 st.steel_stru_place," + 
	" 		 info.tarnish_depth," +
	"		 info.steel_stru_thick,"+ 
	"        info.memo ,"+ 
	"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
	"        h.人员ID AS measure_man_id\n" + 
	"        INTO #temp\n" + 
	"FROM    bbz.xmis.gczl_steel_stru_corr_tarnishing info\n" + 
	"        LEFT OUTER JOIN xmis.hr_人员 h ON info.op_code = h.人员ID " +
	"		LEFT OUTER JOIN xmis.gczl_steel_stru_place_dict st on st.steel_stru_code = info.place_code";

	public static String getSteelSCTColumnNameCombo= 
		"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
		"SELECT 4 AS id, '测量部位' AS columnname, 'steel_stru_place' AS dbname,'string' AS datatype  UNION\n "+
		"SELECT 5 AS id, '锈蚀深度' AS columnname, 'tarnish_depth' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 6 AS id, '原深度' AS columnname, 'steel_stru_thick' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 7 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
		"SELECT 8 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getHorizontalShiftList=" select hori_code as place_code,hori_place FROM [bbz].[xmis].[gczl_horizon_place_dict]";

	public static String getMaxIDFromHorizontalShiftInfo= "SELECT MAX(tab_seed) FROM [bbz].[xmis].[gczl_horizontal_shift_table]";

	public static String doDeleteHorizontalShiftInfoById="delete FROM [bbz].[xmis].[gczl_horizontal_shift_table] where tab_seed=?";

	public static String getHorizontalShiftInfoListWithGroupQuery= "SELECT  info.tab_seed ,\n" +
	"        info.unit_code ,\n" + 
	"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
	"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" +
	"		 info.place_code," +
	"		 st.hori_place," + 
	" 		 info.start_formula," +
	"		 info.measured_value,"+ 
	"        info.memo ,"+ 
	"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
	"        h.人员ID AS measure_man_id\n" + 
	"        INTO #temp\n" + 
	"FROM    bbz.xmis.gczl_horizontal_shift_table info\n" + 
	"        LEFT OUTER JOIN xmis.hr_人员 h ON info.op_code = h.人员ID " +
	"		LEFT OUTER JOIN xmis.gczl_horizon_place_dict st on st.hori_code = info.place_code";

	public static String getHorizontalShiftColumnNameCombo= 
		"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
		"SELECT 4 AS id, '测量部位' AS columnname, 'hori_place' AS dbname,'string' AS datatype  UNION\n "+
		"SELECT 5 AS id, '初始值' AS columnname, 'start_formula' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 6 AS id, '测量值' AS columnname, 'measured_value' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 7 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
		"SELECT 8 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getRiverCodeList=" select river_code,river_name FROM [bbz].[xmis].[gczl_river_dict]";

	public static String getPlaceCodeList=" select  place_code,place_name FROM [bbz].[xmis].[gczl_place_dict]";

	public static String getMaxIDFromOphContentInfo= "SELECT MAX(tab_seed) FROM [bbz].[xmis].[gczl_oph_content_table]";

	public static String doDeleteOphContentInfoById="delete FROM [bbz].[xmis].[gczl_oph_content_table] where tab_seed=?";

	public static String getOphContentInfoListWithGroupQuery= "SELECT  info.tab_seed ,\n" +
	"        info.unit_code ,\n" + 
	"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
	"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" +
	"		 info.river_code," +
	"		 info.place_code," +
	"		 river.river_name," +
	"  		 place.place_name," + 
	"		 info.oph_content_qty,"+ 
	"        info.memo ,"+ 
	"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
	"        h.人员ID AS measure_man_id\n" + 
	"        INTO #temp\n" + 
	"FROM    [bbz].[xmis].[gczl_oph_content_table] info\n" + 
	"        LEFT OUTER JOIN xmis.hr_人员 h ON info.op_code = h.人员ID " +
	"		LEFT OUTER JOIN [bbz].[xmis].[gczl_river_dict] river on river.river_code = info.river_code " +
	"       LEFT OUTER JOIN [bbz].[xmis].[gczl_place_dict] place on place.place_code = info.place_code ";

	public static String getOphContentColumnNameCombo= 
		"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
		"SELECT 4 AS id, '河流名称' AS columnname, 'river_name' AS dbname,'string' AS datatype  UNION\n "+
		"SELECT 5 AS id, '取样部位' AS columnname, 'place_name' AS dbname,'string' AS datatype  UNION\n "+
		"SELECT 6 AS id, '泥沙含量' AS columnname, 'oph_content_qty' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 7 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
		"SELECT 8 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getConcSeamCodeList="select conc_seam_code,conc_seam_name FROM [bbz].[xmis].[gczl_conc_seam_place_dict]";

	public static String getConcMpCodeList="select conc_mp_code,conc_mp_name FROM [bbz].[xmis].[gczl_conc_mp_dict] where conc_seam_code=?";

	public static String getMaxIDFromConcSteamInfo="SELECT MAX(tab_seed) FROM [bbz].[xmis].[gczl_conc_seam_info]";

	public static String doDeleteConcSeamInfoById="delete FROM [bbz].[xmis].[gczl_conc_seam_info] where tab_seed=?";

	public static String getConcSeamInfoListWithGroupQuery= "SELECT  info.tab_seed ,\n" +
	"        info.unit_code ,\n" + 
	"        CONVERT(VARCHAR(50), info.measure_date, 23) AS measure_date ,\n" + 
	"        SUBSTRING(CONVERT(VARCHAR(50), info.measure_date, 120), 12, 16) AS measure_time ,\n" +
	"		 info.conc_seam_code," +
	"		 info.conc_mp_code," +
	"		 seam.conc_seam_name," +
	"  		 mp.conc_mp_name," + 
	"		 info.weather,"+ 
	"		 info.seam_long,"+ 
	"		 info.seam_width,"+ 
	"		 info.xzms,"+ 
	"        info.remark as memo ,"+ 
	"        RTRIM(h.姓氏) + RTRIM(h.名字) AS measure_man ,\n" + 
	"        h.人员ID AS measure_man_id\n" + 
	"        INTO #temp\n" + 
	"FROM    [bbz].[xmis].[gczl_conc_seam_info] info\n" + 
	"        LEFT OUTER JOIN xmis.hr_人员 h ON info.measure_man = h.人员ID " +
	"		LEFT OUTER JOIN [bbz].[xmis].[gczl_conc_seam_place_dict] seam on seam.conc_seam_code = info.conc_seam_code " +
	"       LEFT OUTER JOIN [bbz].[xmis].[gczl_conc_mp_dict] mp on mp.conc_mp_code = info.conc_mp_code ";

	public static String getConcSeamColumnNameCombo= 
		"SELECT 2 AS id, '测量日期' AS columnname, 'measure_date' AS dbname,'date' AS datatype  UNION\n" +
		"SELECT 4 AS id, '测量部位' AS columnname, 'conc_seam_code' AS dbname,'string' AS datatype  UNION\n "+
		"SELECT 5 AS id, '测量点' AS columnname, 'conc_mp_code' AS dbname,'string' AS datatype  UNION\n "+
		"SELECT 6 AS id, '气温' AS columnname, 'oph_content_qty' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 7 AS id, '裂缝长度' AS columnname, 'seam_long' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 8 AS id, '裂缝宽度' AS columnname, 'seam_width' AS dbname,'numeric' AS datatype  UNION\n" +
		"SELECT 9 AS id, '形状描述' AS columnname, 'xzms' AS dbname,'string' AS datatype  UNION\n" +
		"SELECT 10 AS id, '备注' AS columnname, 'memo' AS dbname,'string' AS datatype  UNION\n" + 
		"SELECT 11 AS id, '测量人' AS columnname, 'measure_man' AS dbname,'string' AS datatype ORDER BY id";

	public static String getHightDegree = "SELECT DISTINCT  sp.sp_name,sink.s_point_name,last.high_degree AS last_high_degree,the.high_degree AS the_high_degree , " +
			" case when last.high_degree = 0 or the.high_degree  = 0 OR last.high_degree IS NULL  or the.high_degree  IS NULL then 0 else (last.high_degree-the.high_degree)*1000  end as qty ," +
			"  CASE WHEN last.memo IS NULL THEN '' ELSE '[上次]'+last.memo+'  ' END  + CASE WHEN the.memo IS NULL THEN '' ELSE '[本次]'+the.memo  END   as memo " +
			" FROM [bbz].[xmis].[gczl_sink_informatiom] sink " +
			"  LEFT OUTER JOIN [bbz].[xmis].[gczl_sink_informatiom] last ON last.s_point_code = sink.s_point_code AND last.measure_date = ? AND sink.sp_code = last.sp_code " +
			"   LEFT OUTER JOIN [bbz].[xmis].[gczl_sink_informatiom] the ON the.s_point_code = sink.s_point_code AND the.measure_date = ?  AND sink.sp_code = the.sp_code   " +
			"    LEFT OUTER JOIN [bbz].[xmis].[gczl_sink_place_dict] sp on sink.sp_code = sp.sp_code " +
			"    WHERE sink.sp_code = ? AND (sink.measure_date = ? OR sink.measure_date = ? ) ";

	public static String getFlexSeam="select measure_date ,seam_width , weather , memo " +
			"  FROM    [bbz].[xmis].[gczl_flex_seam_info] WHERE   measure_date >= ?    AND measure_date < =? and fs_code=? order by measure_date asc";

	public static String getMaxAndMinFlexSeam=
		" DECLARE @b_date VARCHAR(20) , @e_date VARCHAR(20),@fs_code varchar(3) SET @b_date=? SET @e_date=?  set @fs_code=?\n" +
		"\n" + 
		"SELECT  '缝宽值' AS title, '最大值' AS boundary,seam_width  AS VALUE,measure_date,'.'  as space \n" + 
		"FROM [bbz].[xmis].[gczl_flex_seam_info]\n" + 
		"WHERE seam_width=(SELECT   MAX(seam_width)\n" + 
		"FROM  [bbz].[xmis].[gczl_flex_seam_info]\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<=@e_date\n " +
		"and fs_code = @fs_code \n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<=@e_date\n " +
		"and fs_code = @fs_code \n " + 
		"\n" + 
		"UNION\n" + 
		"\n" + 
		"SELECT '缝宽值' AS title,  '最小值' AS boundary,seam_width    AS value,measure_date,'.'  as space\n" + 
		"FROM [bbz].[xmis].[gczl_flex_seam_info]\n" + 
		"WHERE seam_width=(SELECT   MIN(seam_width)\n" + 
		"FROM  [bbz].[xmis].[gczl_flex_seam_info]\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<=@e_date\n " +
		"and fs_code = @fs_code \n" + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<=@e_date\n " +
		"and fs_code = @fs_code \n " + 
		"\n";

	public static String getCeYaStatistics="SELECT  convert(varchar(10), measure_date,120) as measure_date ,\n" + 
	"        info.ap_w_level ,\n" + 
	"        info.bp_w_level ,\n" + 
	"        info.tubo_w_level ,\n" + 
	"        info.mptp_code ,\n" + 
	"        info.mpt_name,\n" + 
	"        info.memo ,"+ 
	" 		 case when  ap_w_level = bp_w_level then null else "
	+ "  convert(numeric(16,2), round( (tubo_w_level-bp_w_level)/(ap_w_level-bp_w_level)*100,2))  end as sensitivity,"+
	"        mptp.mptp_name as mptp_name \n" + 
	"FROM    bbz.xmis.gczl_measure_press_tube_informatiom info\n" + 
	"      Left outer join xmis.gczl_measure_press_tube_place_dict mptp on info.mptp_code = mptp.mptp_code  " +
	" where measure_date>=? and measure_date<= ? and info.mptp_code = ? and info.mpt_name = ? order by measure_date asc";

	public static String getMaxAndMinCeYa=
		" DECLARE @b_date VARCHAR(20) , @e_date VARCHAR(20),@mpt_name varchar(10),@mptp_code varchar(10) SET @b_date=? SET @e_date=? set @mpt_name=? set @mptp_code=? \n" +
		"\n" + 
		"SELECT  '管水位' AS title, '最大值' AS boundary,tubo_w_level  AS VALUE,measure_date,'.'  as space \n" + 
		"FROM  bbz.xmis.gczl_measure_press_tube_informatiom\n" + 
		"WHERE tubo_w_level=(SELECT   MAX(tubo_w_level)\n" + 
		"FROM  bbz.xmis.gczl_measure_press_tube_informatiom\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<=@e_date\n " +
		"and mpt_name = @mpt_name \n" +
		"and mpt_name =@mpt_name\n " + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<=@e_date\n " +
		"and mpt_name = @mpt_name \n" +
		"and mpt_name =@mpt_name\n " + 
		"\n" + 
		"UNION\n" + 
		"\n" + 
		"SELECT '管水位' AS title,  '最小值' AS boundary,tubo_w_level    AS value,measure_date,'.'  as space\n" + 
		"FROM  bbz.xmis.gczl_measure_press_tube_informatiom\n" + 
		"WHERE tubo_w_level=(SELECT   MIN(tubo_w_level)\n" + 
		"FROM  bbz.xmis.gczl_measure_press_tube_informatiom\n" + 
		"WHERE measure_date>=@b_date\n" + 
		"AND measure_date<=@e_date\n " +
		"and mpt_name = @mpt_name \n" +
		"and mpt_name =@mpt_name\n " + 
		")\n" + 
		"AND measure_date>=@b_date\n" + 
		"AND measure_date<=@e_date\n " +
		"and mpt_name = @mpt_name \n" +
		"and mpt_name =@mpt_name\n " + 
		"\n";

	public static String getWaterLevel=
		"SELECT  CONVERT(VARCHAR(10),measure_date, 120) AS measure_date ,\n" + 
		"        ap_w_level ,\n" + 
		"        bp_w_level ,\n" + 
		"        flow_qty \n" + 
		"FROM    bbz.xmis.gczl_water_level_info  " +
		" where measure_date >=? and measure_date<= ?\n" ;
	
	public static String getTodoListByApp = 
		"SELECT  todo.ID AS id ,\n"
		+ "        todo.模块 AS model ,\n"
		+ "        todo.应用ID AS appId ,\n"
		+ "        todo.用户ID AS userID ,\n"
		+ "CASE WHEN xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
		+ "                                                   task.PROCESS_ID,GETDATE()) IS NULL\n"
		+ "     THEN RTRIM(actor.姓氏) + RTRIM(actor.名字)\n"
		+ "     ELSE RTRIM(actor.姓氏) + RTRIM(actor.名字) + '('\n"
		+ "          + xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
		+ "                                                     task.PROCESS_ID,GETDATE())\n"
		+ "          + '代理)'\n"
		+ "END AS actorName ,\n"
		+ "CASE WHEN xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
		+ "                                                   task.PROCESS_ID,GETDATE()) IS NULL\n"
		+ "     THEN todo.标题\n"
		+ "     ELSE todo.标题 + '('\n"
		+ "          + xmis.f_getAuthorizeeByUserIDAndProcessID(item.ACTOR_ID,\n"
		+ "                                                     task.PROCESS_ID,GETDATE())\n"
		+ "          + '代理)'\n"
		+ "END AS text ,\n"
		+ "        todo.链接地址 AS url ,\n"
		+ "        todo.脚本 AS script ,\n"
		+ "        todo.优先级 AS priority ,\n"
		+ "        todo.创建时间 AS createTime ,\n"
		+ "        todo.状态 AS state ,\n"
		+ "        todo.描述 AS description ,\n"
		+ "        todo.发送人 AS senderID ,\n"
		+ "        RTRIM(sender.姓氏) + RTRIM(sender.名字) AS senderName ,\n"
		+ "        todo.发送时间 AS sendTime ,\n"
		+ "        todo.是否显示 AS isVisible\n"
		+ "INTO    #result\n"
		+ "FROM    xmis.wf_待办 todo ,\n"
		+ "        xmis.hr_人员 sender ,\n"
		+ "        xmis.hr_人员 actor,\n"
		+ "        xmis.T_FF_RT_WORKITEM item,\n"
		+ "        xmis.T_FF_RT_TASKINSTANCE task\n"
		+ "WHERE   todo.是否显示 = 1\n"
		+ "        AND todo.发送人*=sender.人员ID\n"
		+ "        AND todo.用户ID = actor.人员ID\n"
		+ "        AND todo.应用ID = item.ID\n"
		+ "        AND item.TASKINSTANCE_ID = task.ID\n"
		+ "        AND task.PROCESS_ID IN ('AskForLeave','ApplyDocumentCirculation','ApplyBusinessEntertainment')\n"
		+ "        AND todo.用户ID = ?\n"
		+

		"UNION\n"
		+ "SELECT  todo.ID AS id ,\n"
		+ "        todo.模块 AS model ,\n"
		+ "        todo.应用ID AS appId ,\n"
		+ "        todo.用户ID AS userID ,\n"
		+ "         RTRIM(actor.姓氏) + RTRIM(actor.名字)+'('+RTRIM(shouquan.姓氏) + RTRIM(shouquan.名字)+'代理)' AS actorName ,\n"
		+ "        todo.标题+'('+RTRIM(shouquan.姓氏) + RTRIM(shouquan.名字)+'代理)' AS text ,\n"
		+ "        todo.链接地址 AS url ,\n" + "        todo.脚本 AS script ,\n"
		+ "        todo.优先级 AS priority ,\n"
		+ "        todo.创建时间 AS createTime ,\n"
		+ "        todo.状态 AS state ,\n"
		+ "        todo.描述 AS description ,\n"
		+ "        todo.发送人 AS senderID ,\n"
		+ "        RTRIM(sender.姓氏) + RTRIM(sender.名字) AS senderName ,\n"
		+ "        todo.发送时间 AS sendTime ,\n"
		+ "        todo.是否显示 AS isVisible\n"
		+ "FROM    xmis.wf_待办 todo ,\n" + "        xmis.wf_授权记录表 b ,\n"
		+ "        xmis.T_FF_RT_TASKINSTANCE task ,\n"
		+ "        xmis.T_FF_RT_WORKITEM item ,\n"
		+ "        xmis.hr_人员 sender,\n" + "        xmis.hr_人员 actor,\n"
		+ "        xmis.hr_人员 shouquan\n" + "WHERE   todo.用户ID = b.授权人ID\n"
		+ "        AND todo.发送人*=sender.人员ID\n"
		+ "        AND task.ID = item.TASKINSTANCE_ID\n"
		+ "        AND task.PROCESS_ID IN ('AskForLeave','ApplyDocumentCirculation','ApplyBusinessEntertainment')\n"
		+ "        AND item.ID = todo.应用ID\n"
		+ "        AND todo.是否显示 = 1\n"
		+ "        AND task.PROCESS_ID = b.process_ID\n"
		+ "        AND GETDATE() >= b.生效时间\n"
		+ "        AND b.撤销时间 IS NULL\n"
		+ "        AND todo.用户ID=actor.人员ID\n"
		+ "        AND b.被授权人ID =shouquan.人员ID\n"
		+ "        AND b.被授权人ID = ?";

	public static String getSWJWaterLevel="select CONVERT(VARCHAR(20),观测时间, 120) as measure_date,上游水位    as ap_w_level, 下游水位      as bp_w_level  FROM [bbz].[xmis].[gczl_水文站上下水位数据] where 观测时间 >=? and 观测时间 <= ? " +
			"and 上游水位 is not null  and  下游水位  is not null " +
			" order by 观测时间  asc";
	public static String getDeptManagerList =  " SELECT  hr.人员ID as getDeptManager_id , "
		 +"         dept.部门ID as getDeptManagerDept_id, "
		 +" 		dept.部门名称   as getDeptManagerDept_name, "
		 +"         RTRIM(hr.姓氏) + RTRIM(hr.名字) as getDeptManager_name"
		 +" FROM    xmis.hr_人员 hr "
		 +"         LEFT OUTER JOIN hr_人员部门关系表 hrdept ON hr.人员ID = hrdept.人员ID "
		 +" 		LEFT OUTER JOIN org_部门 dept ON dept.部门ID = hrdept.部门ID "
		 +" WHERE   hr.人员ID IN ( SELECT 人员ID "
		 +"                      FROM   [bbz].[xmis].[sec_人员角色] "
		 +"                      WHERE  角色ID = -5 ) "
		 +"         AND hr.人员ID != -1 ORDER BY dept.部门ID ASC ";
	public static void main(String[] args) {
		System.out.println(getDeptManagerList);
	}
	
}