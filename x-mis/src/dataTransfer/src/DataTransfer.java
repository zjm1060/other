package src;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.Types;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class DataTransfer {

	private static Connection jkglConn, xmisConn;

	private static int maxContractID;

	private static int maxAttachmentID;

	private static int pageSize = 100;

	private static Map companyOtherDeptMap = new LinkedHashMap();

	private static int maxEmpID;

	private static Connection getConnection(String type) throws Exception {

		Connection conn = null;

		String xmisDBType = "prod";

		String xmisProd = "192.169.11.6:4297";
		String xmisUAT = "192.169.11.140:1433";

		String urlStr = "jdbc:microsoft:sqlserver://"
				+ (type.equals("jkgl") ? xmisProd
						: (xmisDBType.equals("prod") ? xmisProd : xmisUAT))
				+ ";SelectMethod=Cursor";

		String user = ";User="
				+ (type.equals("jkgl") ? "ASPNET_USER" : (xmisDBType
						.equals("prod") ? "xmis" : "xmis-uat"));
		String pwd = ";Password=" + (type.equals("jkgl") ? "manager" : "xmis");
		String dbName = ";DatabaseName="
				+ (type.equals("jkgl") ? "JKGL" : "xmis");

		try {
			Class.forName("com.microsoft.jdbc.sqlserver.SQLServerDriver");

			conn = DriverManager.getConnection(urlStr + user + pwd + dbName);

			if (conn != null)
				System.out.println("++++++ " + type + " connection Successful!"
						+ "++++++ ");

		} catch (Exception e) {

			e.printStackTrace();
			throw new Exception("++++++ " + type + " connection failed"
					+ "++++++ ");
		}
		return conn;
	}

	private static List getDataListFromJKGL(String type) {

		ResultSet rs = null;
		List aList = new ArrayList();
		String sql = "";

		if (type.equals("emp"))
			sql = "select * from 员工";

		else if (type.equals("crm"))
			sql = "select distinct(甲方) from 收入合同"
					+ " union select distinct(供货商ID) from 支出合同";

		else if (type.equals("省份"))
			sql = "select * from 省份";

		else if (type.equals("收入合同"))
			sql = "select * from 收入合同 order by 自动编号";

		else if (type.equals("甲方代表"))
			sql = "select DISTINCT 甲方,甲方代表 from 收入合同 where 甲方代表 <> ''";

		else if (type.equals("收入合同追加合同"))
			sql = "select *  from 收入合同追加合同";

		else if (type.equals("收入合同付款条件"))
			sql = "select *  from 收入合同付款条件 ORDER BY 项目编号,自动编号";

		else if (type.equals("收入合同付款明细"))
			sql = "select *  FROM dbo.收入合同来款 ORDER BY 项目编号, 自动编号";

		else if (type.equals("收入合同项目人员"))
			sql = "select *  from 收入合同项目人员";

		else if (type.equals("收入合同收据"))
			sql = "select *  from 收入合同收据  ORDER BY 项目编号, 自动编号";

		else if (type.equals("收入合同发票"))
			sql = "select *  from 收入合同发票  ORDER BY 项目编号, 自动编号";

		else if (type.equals("收入合同关联文件"))
			sql = "select *  from 收入合同关联文件 ";

		else if (type.equals("支出合同"))
			sql = "select * from 支出合同 order by 自动编号";

		else if (type.equals("支出合同对应收入合同金额"))
			sql = "select * from 支出合同对应收入合同金额";

		else if (type.equals("支出合同付款"))
			sql = "select *  FROM 支出合同付款 ORDER BY 支出合同编号, 自动编号";

		else if (type.equals("支出合同报销情况"))
			sql = "select *  from 支出合同报销情况 ORDER BY 支出合同编号,自动编号";

		else if (type.equals("支出合同关联文件"))
			sql = "SELECT count(*) aCount FROM  支出合同关联文件";

		else if (type.indexOf("支出合同关联文件_") == 0) {

			System.out.println(Type.GetInt(type.replace("支出合同关联文件_", ""))
					* pageSize);

			// sql = "SELECT TOP 50 * FROM 支出合同关联文件" +
			// " WHERE (自动编号 > (SELECT MAX(自动编号) FROM " +
			// "(SELECT TOP 50 * "+type.replace("支出合同关联文件_", "")+" 自动编号 FROM
			// 支出合同关联文件 ORDER BY 自动编号) AS T)) " +
			// " ORDER BY 自动编号";

			sql = " SELECT TOP " + pageSize + " * FROM 支出合同关联文件 WHERE 自动编号 > ("
					+ " SELECT MAX(自动编号) FROM (SELECT TOP " + pageSize
					* Type.GetInt(type.replace("支出合同关联文件_", ""))
					+ " 自动编号 FROM 支出合同关联文件 ORDER BY 自动编号) AS T) "
					+ " ORDER BY 自动编号";
		}

		// System.out.println(sql);

		try {
			rs = jkglConn.prepareStatement(sql).executeQuery();

			ResultSetMetaData rsmd = rs.getMetaData();

			while (rs.next()) {
				Map map = new HashMap();
				for (int i = 0; i < rsmd.getColumnCount(); i++) {
					map.put(rsmd.getColumnName(i + 1), rs.getObject(i + 1));
				}

				aList.add(map);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				rs.close();
			} catch (Exception e) {
				e.printStackTrace();
			}

		}

		return aList;

	}

	private static int getDeptIDMapping(String dept) {

		int deptID = 0;
		// if(dept.equals("一部") || dept.equals("工程一部"))
		if (dept.indexOf("一部") > -1)
			deptID = 2;

		// else if(dept.equals("二部") || dept.equals("工程二部"))
		else if (dept.indexOf("二部") > -1)
			deptID = 3;

		// else if(dept.equals("三部") || dept.equals("工程三部") ||
		// dept.equals("三峡部") || dept.equals("三峡工程部"))
		else if (dept.indexOf("三部") > -1 || dept.equals("三峡部")
				|| dept.equals("三峡工程部"))
			deptID = 4;

		// else if(dept.equals("四部") || dept.equals("工程四部"))
		else if (dept.indexOf("四部") > -1)
			deptID = 5;

		else if (dept.equals("开发部"))
			deptID = 6;

		else if (dept.equals("项目管理部") || dept.equals("监控事业部")
				|| dept.equals("事业部"))
			deptID = 7;

		else if (dept.equals("生产部"))
			deptID = 8;

		else if (dept.equals("市场部"))
			deptID = 9;

		else if (dept.equals("质量管理部"))
			deptID = 10;

		return deptID;
	}

	private static Map doDataTransferForEmp() throws Exception {

		Map empMap = new LinkedHashMap();

		try {
			List empListFromJKGL = getDataListFromJKGL("emp");
			List empListForXMIS = new ArrayList();

			// logger.info(empListFromJKGL);
			int iCount = 0;
			for (Iterator i = empListFromJKGL.listIterator(); i.hasNext();) {

				Map map = (HashMap) i.next();
				EmployeeObj emp = new EmployeeObj();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();

					// if (iCount == 0)
					// System.out.println(entry.getKey() + "=" +
					// entry.getValue());

					if (entry.getKey().equals("自动编号")) {
						emp.setUserID(Type.GetInt(entry.getValue()));

						maxEmpID = maxEmpID > Type.GetInt(entry.getValue()) ? maxEmpID
								: Type.GetInt(entry.getValue());

					}

					else if (entry.getKey().equals("ID"))
						emp.setLoginName(Type.getString(entry.getValue()));

					else if (entry.getKey().equals("名称")) {
						emp.setFirstName(Type.getString(entry.getValue())
								.substring(0, 1));
						emp.setLastName(Type.getString(entry.getValue())
								.substring(
										1,
										Type.getString(entry.getValue())
												.length()));

						// logger.info(emp.getFirstName() + emp.getLastName() +
						// "//");
					} else if (entry.getKey().equals("密码"))
						emp.setPassword(Type.getString(entry.getValue()));
					else if (entry.getKey().equals("性别"))
						emp.setSex(entry.getValue().equals("男") ? 1 : 0);
					else if (entry.getKey().equals("部门ID"))
						emp.setDepartmentID(getDeptIDMapping(Type.getString(
								entry.getValue()).trim()));

				}

				// empMap.put(emp.getLoginName(), emp.getUserID());
				empMap.put(emp.getLoginName(), emp);
				empListForXMIS.add(emp);

				iCount++;
			}

			doDataTransferForEmp(empListForXMIS);
		} catch (Exception e) {
			e.printStackTrace();
			throw new Exception("=========== 人员数据转换失败");

		}

		System.out.println("=========== 人员数据转换成功");
		return empMap;

	}

	private static void doDataTransferForEmp(List aList) {

		String delSql = " DELETE xmis.hr_人员 WHERE 人员ID > 0 "
				+ " DELETE xmis.hr_人员部门关系表 WHERE 人员ID > 0 ";
		String insertSql = " insert into xmis.hr_人员(人员ID,登录名,名字,姓氏,性别ID,修改人ID,修改时间) values (?,?,?,?,?,-1,GETDATE())"
				+ " insert into xmis.hr_人员部门关系表 (人员ID,部门ID,是否兼职) values (?,?,1)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				EmployeeObj emp = (EmployeeObj) aList.get(i);

				// System.out.println(emp.getFirstName() + emp.getLastName() +
				// "//");

				ps.setInt(1, emp.getUserID());
				ps.setString(2, emp.getLoginName());
				ps.setString(3, emp.getLastName());
				ps.setString(4, emp.getFirstName());
				ps.setInt(5, emp.getSex());

				ps.setInt(6, emp.getUserID());
				ps.setInt(7, emp.getDepartmentID());

				ps.addBatch();
			}

			ps.executeBatch();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	private static void doDataTransferForSupplementalContract(List aList) {

		String insertSql = "insert  into  xmis.con_合同 (合同ID,合同编号,合同名称,简称,签订日期,乙方公司ID,乙方部门ID,甲方公司ID,合同总额,"
				+ // 9
				"货币ID,合同类型ID,乙方代表ID,合同概要,备注,登记人ID,更新时间, 父合同ID)  values " + // 17
				" (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				ContractObj con = (ContractObj) aList.get(i);

				ps.setInt(1, con.getContractID());
				ps.setString(2, con.getContractNo());
				ps.setString(3, con.getContractName());
				ps.setString(4, con.getAbbreviation());
				ps.setDate(5, con.getSignatureDate());

				ps.setInt(6, con.getVendorID());
				ps.setInt(7, con.getDeptIDOfVendor());
				ps.setInt(8, con.getBuyerID());
				ps.setDouble(9, con.getContractAmt());
				ps.setInt(10, con.getCurrencyID());
				ps.setInt(11, con.getContractTypeID());
				ps.setInt(12, con.getVendorRepresentativeID());
				ps.setString(13, con.getContractSummary());
				ps.setString(14, con.getRemark());
				ps.setInt(15, con.getRegisterID());
				ps.setDate(16, con.getUpdateDate());

				ps.setInt(17, con.getParentID());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 收入合同追加合同转换成功");

		} catch (Exception e) {
			System.out.println("=========== 收入合同追加合同转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	// doDataTransferForPaymentContract
	private static void doDataTransferForPaymentContract(List aList) {

		String delSql = " DELETE xmis.con_合同 where 合同类型ID =2 ";
		String insertSql = "insert  into  xmis.con_合同 (合同ID,合同编号,合同名称,简称,签订日期,乙方公司ID,乙方部门ID,甲方公司ID,合同总额,"
				+ "货币ID,合同类型ID,乙方代表ID,合同概要,备注,登记人ID,质保期起,质保期定义,更新时间)  values  (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"
		// 18

		;

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				ContractObj con = (ContractObj) aList.get(i);

				ps.setInt(1, con.getContractID());
				ps.setString(2, con.getContractNo());
				ps.setString(3, con.getContractName());
				ps.setString(4, con.getAbbreviation());
				ps.setDate(5, con.getSignatureDate());

				ps.setInt(6, con.getVendorID());
				ps.setNull(7, Types.INTEGER);
				ps.setInt(8, con.getBuyerID());
				ps.setDouble(9, con.getContractAmt());
				ps.setInt(10, con.getCurrencyID());
				ps.setInt(11, con.getContractTypeID());
				ps.setNull(12, Types.INTEGER);
				ps.setString(13, con.getContractSummary());
				ps.setString(14, con.getRemark());
				ps.setInt(15, con.getRegisterID());
				ps.setNull(16, Types.DATE);
				ps.setNull(17, Types.VARCHAR);
				ps.setDate(18, con.getUpdateDate());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 支出合同数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 支出合同数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	private static void doDataTransferForIncomeContract(List aList) {

		String delSql = " DELETE xmis.con_合同 " + "Delete  xmis.prj_项目 ";
		String insertSql = "insert  into  xmis.con_合同 (合同ID,合同编号,合同名称,简称,签订日期,乙方公司ID,乙方部门ID,甲方公司ID,合同总额,"
				+ "货币ID,合同类型ID,乙方代表ID,合同概要,备注,登记人ID,质保期起,质保期定义,更新时间,甲方代表ID)  values  (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"
				+ // 19

				" insert into xmis.prj_项目(项目ID , 项目编码 , 项目名称 , 项目合同ID , 启动日期, 项目状态ID ) values (?,?,?,?,?,-1)" // 20-24

		;

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				ContractObj con = (ContractObj) aList.get(i);

				ps.setInt(1, con.getContractID());
				ps.setString(2, con.getContractNo());
				ps.setString(3, con.getContractName());
				ps.setString(4, con.getAbbreviation());
				ps.setDate(5, con.getSignatureDate());

				ps.setInt(6, con.getVendorID());
				ps.setInt(7, con.getDeptIDOfVendor());
				ps.setInt(8, con.getBuyerID());
				ps.setDouble(9, con.getContractAmt());
				ps.setInt(10, con.getCurrencyID());
				ps.setInt(11, con.getContractTypeID());
				ps.setInt(12, con.getVendorRepresentativeID());
				ps.setString(13, con.getContractSummary());
				ps.setString(14, con.getRemark());
				ps.setInt(15, con.getRegisterID());
				ps.setDate(16, con.getWarrantyStartDate());
				ps.setString(17, con.getWarrantyDefinition());
				ps.setDate(18, con.getUpdateDate());
				ps.setInt(19, con.getBuyerRepresentativeID());


				ps.setInt(20, con.getProjectOBJ().getProjectID());
				ps.setString(21, con.getProjectOBJ().getProjectCode());
				ps.setString(22, con.getProjectOBJ().getProjectName());
				ps.setInt(23, con.getProjectOBJ().getContractID());
				ps.setDate(24, con.getProjectOBJ().getStartDate());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 合同&项目数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 合同&项目数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	// doDataTransferBuyerRepresentativeName(name id)
	private static Map doDataTransferBuyerRepresentativeName(List aList) {

		String insertSql = " insert into xmis.hr_人员(人员ID,登录名,名字,姓氏,性别ID,修改人ID,修改时间) values (?,?,?,?,?,-1,GETDATE())" // 5
				+ " insert into xmis.hr_人员部门关系表 (人员ID,部门ID,是否兼职) values (?,?,1)" // 6,7
//				+ "   UPDATE xmis.con_合同 SET 甲方代表ID = ? WHERE xmis.con_合同.合同ID = ? " // 8,9
		; //

		Map newBuyerMap = new LinkedHashMap(); 

		PreparedStatement ps = null;

		
		try {
			ps = xmisConn.prepareStatement(insertSql);

			for (int i = 0; i < aList.size(); i++) {
				String[] conMapping = (String[]) aList.get(i);
				
				ps.setInt(1, maxEmpID + i + 1);
				ps.setString(2, conMapping[0].length() > 3 ? conMapping[0]
						.substring(0, 3) : conMapping[0]);
				ps.setString(3, conMapping[0].substring(1, conMapping[0]
						.length()));

				ps.setString(4, conMapping[0].substring(0, 1));
				ps.setInt(5, 1);

				ps.setInt(6, maxEmpID + i + 1);
				ps.setInt(7, Type.GetInt(conMapping[1]));

//				ps.setInt(8, maxEmpID + i + 1);
//				ps.setInt(9, Type.GetInt(conMapping[2]));

				ps.addBatch();
				
				newBuyerMap.put(conMapping[2]+ "~" + conMapping[0], Type.getString(maxEmpID + i + 1));
			}

			ps.executeBatch();
			
			System.out.println("=========== 合同甲方人员 数据转换成功");
			

		} catch (Exception e) {
			System.out.println("=========== 合同甲方人员 数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return newBuyerMap;

	}

	private static void doDataTransferForCompany(Map aMap) {

		String delSql = " DELETE xmis.org_公司 "
				+ "  DELETE xmis.org_部门 WHERE 部门ID > 10 "
				+ "DELETE xmis.xmis.org_部门归属公司表 WHERE 部门ID > 10 ";
		String insertSql = " INSERT INTO xmis.org_公司 "
				+ "( 公司ID , 公司名称 , 名称缩写 , 业务关系ID , 修改时间 , 修改人ID)"
				+ " VALUES  (?,?,?,-1,getdate(),-1)" // 1-3

				+ " INSERT INTO xmis.org_部门"
				+ " ( 部门ID ,  部门名称 )"
				+ " VALUES  ( ?, '其它')" // 4

				+ " INSERT INTO xmis.org_部门归属公司表"
				+ " ( 公司ID, 部门ID, 是否直属 ) values (?,?,1) " // 5，6

		;

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);

			Iterator compaynIT = aMap.entrySet().iterator();
			for (int i = 1; i < aMap.size(); i++) {
				Map.Entry entry = (Map.Entry) compaynIT.next();
				// System.out.println(entry.getKey() + "==" + entry.getValue());

				ps.setInt(1, Type.GetInt(entry.getValue()));
				ps.setString(2, Type.getString(entry.getKey()));
				ps.setString(3, Type.getString(entry.getKey()));

				ps.setInt(4, 20 + i);

				ps.setInt(5, Type.GetInt(entry.getValue()));
				ps.setInt(6, 20 + i);

				ps.addBatch();

				// if(Type.getString(entry.getKey()).equals("湖南潇湘综合开发有限公司"))
				// System.out.println("the other dept id = " + (20 +i));

				companyOtherDeptMap.put(Type.getString(entry.getKey()), 20 + i);

			}

			ps.executeBatch();
			System.out.println("=========== 公司&公司~‘其它’部门 数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 公司&公司~‘其它’部门 数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	// doDataTransferForPaymentCondition
	private static void doDataTransferForPaymentCondition(List aList) {

		String delSql = " DELETE xmis.con_收款条件表 ";
		String insertSql = "INSERT INTO xmis.con_收款条件表 ( " + "			合同ID ,"
				+ "          收款条件ID ," + "          收款条件描述 ," + "			收款百分比 ,"
				+ "          收款金额,手动设置,延迟天数,对应项目状态) values (?,?,?,?,?,0,0,0)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				PaymentConditionObj pay = (PaymentConditionObj) aList.get(i);

				ps.setInt(1, pay.getContractID());
				ps.setInt(2, pay.getPaymentConditionID());
				ps.setString(3, pay.getDescription());
				ps.setInt(4, pay.getPaymentPercent());
				ps.setDouble(5, pay.getPamymentAmt());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 收款条件数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 收款条件数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	// PayoutContractPaymentDetail
	private static void doDataTransferForPayoutContractPaymentDetail(List aList) {

		// String delSql = " DELETE xmis.con_收款明细表 ";
		String insertSql = "INSERT INTO xmis.con_收款明细表" + "        ( 合同ID ,"
				+ "          收款ID ," + "          收款金额 ," + "          收款日期 ,"
				+ "          经办人 ," + "          票据扫描 ," + "          登记人 ,"
				+ "          登记日期 ," + "          备注"
				+ "        ) values (?,?,?,?,?,null,?,?,?)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			// xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				PaymentDetailObj pay = (PaymentDetailObj) aList.get(i);

				ps.setInt(1, pay.getContractID());
				ps.setInt(2, pay.getPaymentID());
				ps.setDouble(3, pay.getPaymentAmt());
				ps.setDate(4, pay.getPaymentDate());
				ps.setInt(5, pay.getTransactorID());
				ps.setInt(6, pay.getTransactorID());
				ps.setDate(7, pay.getPaymentDate());
				ps.setString(8, pay.getRemark());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 收款明细数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 收款明细数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	// doDataTransferForPaymentDetail
	private static void doDataTransferForPaymentDetail(List aList) {

		String delSql = " DELETE xmis.con_收款明细表 ";
		String insertSql = "INSERT INTO xmis.con_收款明细表" + "        ( 合同ID ,"
				+ "          收款ID ," + "          收款金额 ," + "          收款日期 ,"
				+ "          经办人 ," + "          票据扫描 ," + "          登记人 ,"
				+ "          登记日期 ," + "          备注"
				+ "        ) values (?,?,?,?,?,null,?,?,?)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				PaymentDetailObj pay = (PaymentDetailObj) aList.get(i);

				ps.setInt(1, pay.getContractID());
				ps.setInt(2, pay.getPaymentID());
				ps.setDouble(3, pay.getPaymentAmt());
				ps.setDate(4, pay.getPaymentDate());
				ps.setInt(5, pay.getTransactorID());
				ps.setInt(6, pay.getTransactorID());
				ps.setDate(7, pay.getPaymentDate());
				ps.setString(8, pay.getRemark());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 收款明细数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 收款明细数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	// doDataTransferForReceipt
	private static void doDataTransferForReceipt(List aList) {

		String delSql = " DELETE xmis.con_收据明细表 ";
		String insertSql = "INSERT INTO xmis.con_收据明细表" + "        ( 合同ID ,"
				+ "          收据ID ," + "          收据金额 ," + "          收据日期 ,"
				+ "          经办人 ," + "          登记人 ," + "          登记日期 ,"
				+ "          备注" + "        ) values (?,?,?,?,?,?,?,?)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				InvoiceObj inv = (InvoiceObj) aList.get(i);

				ps.setInt(1, inv.getContractID());
				ps.setInt(2, inv.getInvoiceID());
				ps.setDouble(3, inv.getInvoiceAMT());
				ps.setDate(4, inv.getInvoiceDate());
				ps.setInt(5, inv.getTransactorID());
				ps.setInt(6, inv.getTransactorID());
				ps.setDate(7, inv.getInvoiceDate());
				ps.setString(8, inv.getInvoiecRemark());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 收据明细数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 收据明细数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	private static void doDataTransferForPayoutContractInvoiceDetail(List aList) {

		// String delSql = " DELETE xmis.con_发票开具表 ";
		String insertSql = "INSERT INTO xmis.con_发票开具表" + "        ( 合同ID ,"
				+ "          发票号 ," + "          发票金额 ," + "          开具日期 ,"
				+ "          经办人 ," + "          扫描件 ," + "          登记人 ,"
				+ "          登记日期 ," + "          备注"
				+ "        ) values (?,?,?,?,?,null,?,?,?)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			// xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				InvoiceObj inv = (InvoiceObj) aList.get(i);

				ps.setInt(1, inv.getContractID());
				ps.setInt(2, inv.getInvoiceID());
				ps.setDouble(3, inv.getInvoiceAMT());
				ps.setDate(4, inv.getInvoiceDate());
				ps.setInt(5, inv.getTransactorID());
				ps.setInt(6, inv.getTransactorID());
				ps.setDate(7, inv.getInvoiceDate());
				ps.setString(8, inv.getInvoiecRemark());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 支出合同发票明细数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 支出合同发票明细数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	// doDataTransferForInvoiceDetail
	private static void doDataTransferForInvoiceDetail(List aList) {

		String delSql = " DELETE xmis.con_发票开具表 ";
		String insertSql = "INSERT INTO xmis.con_发票开具表" + "        ( 合同ID ,"
				+ "          发票号 ," + "          发票金额 ," + "          开具日期 ,"
				+ "          经办人 ," + "          扫描件 ," + "          登记人 ,"
				+ "          登记日期 ," + "          备注"
				+ "        ) values (?,?,?,?,?,null,?,?,?)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				InvoiceObj inv = (InvoiceObj) aList.get(i);

				ps.setInt(1, inv.getContractID());
				ps.setInt(2, inv.getInvoiceID());
				ps.setDouble(3, inv.getInvoiceAMT());
				ps.setDate(4, inv.getInvoiceDate());
				ps.setInt(5, inv.getTransactorID());
				ps.setInt(6, inv.getTransactorID());
				ps.setDate(7, inv.getInvoiceDate());
				ps.setString(8, inv.getInvoiecRemark());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 发票明细数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 发票明细数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	// doDataTransferForContractAttachemtn
	private static void doDataTransferForContractAttachment(List aList) {

		String delSql = " DELETE xmis.sys_附件 "
				+ "DELETE xmis.sys_附件主体类型 "
				+ " insert into xmis.sys_附件主体类型 ( 附件主体类型ID ,"
				+ "          附件主体描述 , "
				+ "          TableName,  FieldName) values(1,'xmis.con_合同.','xmis.con_合同','')";

		String insertSql = "INSERT INTO xmis.sys_附件" + "        ( 附件ID ,"
				+ "          上传时间 ," + "          上传人 ," + "          文件名 ,"
				+ "          附件内容 ," + "          附件主体类型ID ,"
				+ "          REFID        )" + " values(?,?,?,?,?,?,?) ";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				AttachmentFile aFile = (AttachmentFile) aList.get(i);

				ps.setInt(1, i + 1);
				ps.setDate(2, aFile.getUploadDate());
				ps.setInt(3, aFile.getUploadID());
				ps.setString(4, aFile.getFileName());
				ps.setBytes(5, aFile.getContent());
				ps.setInt(6, 1);
				ps.setInt(7, aFile.getRefID());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 收入合同附件转换成功");

		} catch (Exception e) {
			System.out.println("=========== 收入合同附件转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	// doDataTransferForPayoutContractAttachemtn
	private static void doDataTransferForPayoutContractAttachment(List aList) {

		// String delSql = " DELETE xmis.sys_附件 DELETE xmis.sys_附件主体类型 " +
		// " insert into xmis.sys_附件主体类型 ( 附件主体类型ID ," +
		// " 附件主体描述 , " +
		// " TableName, FieldName) values(1,'xmis.con_合同.','xmis.con_合同','')";

		String insertSql = "INSERT INTO xmis.sys_附件" + "        ( 附件ID ,"
				+ "          上传时间 ," + "          上传人 ," + "          文件名 ,"
				+ "          附件内容 ," + "          附件主体类型ID ,"
				+ "          REFID        )" + " values(?,?,?,?,?,?,?) ";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			// xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				AttachmentFile aFile = (AttachmentFile) aList.get(i);

				ps.setInt(1, maxAttachmentID + 1);
				ps.setDate(2, aFile.getUploadDate());
				ps.setInt(3, aFile.getUploadID());
				ps.setString(4, aFile.getFileName());
				ps.setBytes(5, aFile.getContent());
				ps.setInt(6, 1);
				ps.setInt(7, aFile.getRefID());

				ps.addBatch();
				maxAttachmentID++;

			}

			ps.executeBatch();
			System.out.println("=========== 支出合同附件转换成功");

		} catch (Exception e) {
			System.out.println("=========== 支出合同附件转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	// doDataTransferForProjectMember
	private static void doDataTransferForProjectMember(List aList) {

		String delSql = " DELETE xmis.prj_项目任务表  " + " delete xmis.prj_项目任务成员 "
				+ " delete xmis.prj_任务进度计划";
		String insertSql = "INSERT INTO xmis.prj_项目任务表"
				+ "        ( 项目任务ID ,"
				+ "          项目ID ,"
				+ "          布置人ID ,"
				+ "          承接部门ID "
				+ "        )VALUES  (?,?,-1,?)"
				+ // 1,2,3
				"      INSERT INTO xmis.xmis.prj_项目任务成员"
				+ "		    ( 项目任务ID, 项目成员ID, 是否负责人, 备注 )"
				+ "		VALUES  (?,?,1,?  )" // 4,5,6
				+ " INSERT INTO xmis.prj_任务进度计划"
				+ "        ( 项目任务ID, 开始日期, 结束日期 )" + "VALUES  ( ?,?,getdate())" // 7,8,9
		;

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				ProjectTaskObj task = (ProjectTaskObj) aList.get(i);

				ps.setInt(1, task.getTaskID());
				ps.setInt(2, task.getProjectID());
				ps.setInt(3, task.getAssigedToDeptID());

				ps.setInt(4, task.getTaskID());
				ps.setInt(5, task.getAssigedToEmpID());
				ps.setString(6, task.getRemark());

				ps.setInt(7, task.getTaskID());
				ps.setDate(8, task.getBeginDate());

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 项目成员据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 项目成员数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	private static void doDataTransferForProvince(Map aMap) {

		String delSql = " DELETE xmis.add_省 ";
		String insertSql = " INSERT INTO xmis.add_省 "
				+ "( 省ID, 省编码, 省名称, 省简称, 地区ID )" + " VALUES  (?,?,?,?,-1)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);

			Iterator compaynIT = aMap.entrySet().iterator();
			for (int i = 1; i < aMap.size(); i++) {
				Map.Entry entry = (Map.Entry) compaynIT.next();
				// System.out.println(entry.getKey() + "==" + entry.getValue());

				ps.setInt(1, Type.GetInt(entry.getValue()));
				ps.setString(2, Type.getString(entry.getKey()));
				ps.setString(3, Type.getString(entry.getKey()));
				ps.setString(4, Type.getString(entry.getKey()));

				ps.addBatch();

			}

			ps.executeBatch();
			System.out.println("=========== 省份数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 省份数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	// doDataTransferContractMapping
	private static void doDataTransferForContractMapping(List aList) {

		String delSql = " DELETE xmis.con_收入支出合同对应表 ";
		String insertSql = "INSERT INTO xmis.con_收入支出合同对应表"
				+ "        ( 对应ID ," + "          收入合同ID ,"
				+ "          支出合同ID ," + "          金额 ," + "          备注"
				+ "        ) values (?,?,?,?,?)";

		// logger.info(aList.size());

		PreparedStatement ps = null;

		try {
			xmisConn.prepareStatement(delSql).execute();

			ps = xmisConn.prepareStatement(insertSql);
			// System.out.println(aList.size());

			for (int i = 0; i < aList.size(); i++) {
				String[] conMapping = (String[]) aList.get(i);

				ps.setObject(1, Type.GetInt(conMapping[0]));
				ps.setObject(2, Type.GetInt(conMapping[1]));
				ps.setObject(3, Type.GetInt(conMapping[2]));
				ps.setObject(4, Type.GetDouble(conMapping[3]));
				ps.setObject(5, conMapping[4]);

				ps.addBatch();
			}

			ps.executeBatch();
			System.out.println("=========== 收入支出合同对应数据转换成功");

		} catch (Exception e) {
			System.out.println("=========== 收入支出合同对应数据转换失败");
			e.printStackTrace();
		} finally {
			try {
				ps.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	private static Map doDataTransferForCompay() {

		Map companyMap = new LinkedHashMap();

		List companyListFromJKGL = getDataListFromJKGL("crm");

		int iCount = 10;
		for (Iterator i = companyListFromJKGL.listIterator(); i.hasNext();) {

			Map map = (HashMap) i.next();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();
			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				companyMap.put(entry.getValue(), iCount + 1);

				// logger.info(entry.getValue() + "=" +
				// companyMap.get(entry.getValue()));
			}
			iCount++;

		}

		companyMap.put("中水科技", 1);
		companyMap.put("中水科自动化", 2);
		companyMap.put("水科院", 3);
		// companyMap.put("监控事业部", 1);

		doDataTransferForCompany(companyMap);

		return companyMap;

	}

	private static void doDataTransferForProvince() {

		Map provinceMap = new LinkedHashMap();

		List provinceListFromJKGL = getDataListFromJKGL("省份");

		for (Iterator i = provinceListFromJKGL.listIterator(); i.hasNext();) {

			Map map = (HashMap) i.next();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();
			int id = 0;
			String name = null;
			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				if (entry.getKey().equals("自动编号"))
					id = Type.GetInt(entry.getValue());
				else if (entry.getKey().equals("名称"))
					name = Type.getString(entry.getValue());

			}

			// System.out.println(id + " = " + name);

			provinceMap.put(name, id);

		}

		doDataTransferForProvince(provinceMap);

	}

	// doDataTransferForPayoutContractProject
	private static Map doDataTransferForPayoutContractProject(Map empMap,
			Map companyMap) {

		List incomeContractListFromJKGL = getDataListFromJKGL("支出合同");
		List contractListForXMIS = new ArrayList();

		Map conProjMap = new LinkedHashMap();
		Map conMap = new LinkedHashMap();
		int iCount = 0;
		for (Iterator i = incomeContractListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			ContractObj con = new ContractObj();
			ProjectObj proj = new ProjectObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				con.setContractTypeID(2);
				con.setSourceTypeID(1);

				if (entry.getKey().equals("自动编号")) {

					con.setContractID(maxContractID + 1);
				}

				else if (entry.getKey().equals("编号"))
					con.setContractNo(Type.getString(entry.getValue()));

				// else if (entry.getKey().equals("项目编号")) {
				// try{
				// conProjMap.get(Type.getString(entry.getValue()));
				// proj = null;
				// }catch(Exception e){
				// proj.setProjectID(conProjMap.size() + 1);
				// proj.setProjectCode(Type.getString(entry.getValue()));
				// }
				//
				//
				// }
				else if (entry.getKey().equals("记录用户名称")) {
					try {
						con.setRegisterID(((EmployeeObj) empMap.get(Type
								.getString(entry.getValue()))).getUserID());
					} catch (Exception e) {
						con.setRegisterID(0);
					}

				} else if (entry.getKey().equals("签订日期")) {
					con.setSignatureDate(Type.getDate(entry.getValue()));

				} else if (entry.getKey().equals("全称")) {
					con.setContractName(Type.getString(entry.getValue()));
					con.setAbbreviation(Type.getString(entry.getValue()));

				} else if (entry.getKey().equals("公司ID"))
					con.setBuyerID(Type.GetInt(companyMap.get(Type
							.getString(entry.getValue()))));
				else if (entry.getKey().equals("供货商ID"))
					con.setVendorID(Type.GetInt(companyMap.get(Type
							.getString(entry.getValue()))));
				else if (entry.getKey().equals("货币单位"))
					con.setCurrencyID(1);
				else if (entry.getKey().equals("合同额"))
					con.setContractAmt(Type.GetDouble(entry.getValue()));

				else if (entry.getKey().equals("合同简要"))
					con.setContractSummary(Type.getString(entry.getValue()));
				else if (entry.getKey().equals("备注"))
					con.setRemark(Type.getString(entry.getValue()));
				else if (entry.getKey().equals("最新修改"))
					con.setUpdateDate(Type.getDate(entry.getValue()));

			}

			conProjMap.put(con.getContractNo(), con);
			maxContractID = con.getContractID();
			contractListForXMIS.add(con);
			iCount++;
		}

		doDataTransferForPaymentContract(contractListForXMIS);

		return conProjMap;

	}

	private static Map doDataTransferForContractProject(Map empMap,
			Map companyMap, Map newBuyerMap) {

		List incomeContractListFromJKGL = getDataListFromJKGL("收入合同");
		List contractListForXMIS = new ArrayList();

		Map conProjMap = new LinkedHashMap();
		Map conMap = new LinkedHashMap();
		int iCount = 0;
		for (Iterator i = incomeContractListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			ContractObj con = new ContractObj();
			ProjectObj proj = new ProjectObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				con.setContractTypeID(1);
				con.setSourceTypeID(1);

				if (entry.getKey().equals("自动编号")) {

					con.setContractID(Type.GetInt(entry.getValue()));
					proj.setContractID(con.getContractID());
				} else if (entry.getKey().equals("项目编号")) {
					proj.setProjectID(iCount + 1);
					proj.setProjectCode(Type.getString(entry.getValue()));

				} else if (entry.getKey().equals("记录用户名称")) {
					// con
					// .setRegisterID(Type.GetInt(empMap.get(entry
					// .getValue())));

					try {
						con.setRegisterID(((EmployeeObj) empMap.get(Type
								.getString(entry.getValue()))).getUserID());
					} catch (Exception e) {
						con.setRegisterID(0);
					}

				} else if (entry.getKey().equals("合同编号"))
					con.setContractNo(Type.getString(entry.getValue()));
				else if (entry.getKey().equals("签订日期")) {
					con.setSignatureDate(Type.getDate(entry.getValue()));
					proj.setStartDate(con.getSignatureDate());
				} else if (entry.getKey().equals("合同名称")) {
					con.setContractName(Type.getString(entry.getValue()));
					proj.setProjectName(con.getContractName());
					con.setProjectOBJ(proj);
				} else if (entry.getKey().equals("简称"))
					con.setAbbreviation(Type.getString(entry.getValue()));
				else if (entry.getKey().equals("甲方")){
					con.setBuyer(Type.getString(entry.getValue()));
					con.setBuyerID(Type.GetInt(companyMap.get(Type
							.getString(entry.getValue()))));
				}
				else if (entry.getKey().equals("乙方"))
					con.setVendorID(Type.GetInt(companyMap.get(Type
							.getString(entry.getValue()))));
				else if (entry.getKey().equals("货币单位"))
					con.setCurrencyID(1);
				else if (entry.getKey().equals("合同总额"))
					con.setContractAmt(Type.GetDouble(entry.getValue()));
				else if (entry.getKey().equals("所属部门"))
					con.setDeptIDOfVendor(getDeptIDMapping(Type.getString(entry
							.getValue())));

				 else if (entry.getKey().equals("甲方代表")) {
					 con.setBuyerRepresentativeName(Type.getString(entry.getValue()));
				 }

				else if (entry.getKey().equals("乙方代表")) {

					// con.setVendorRepresentativeID(Type.GetInt(empMap.get(Type
					// .getString(entry.getValue()))));

					try {
						con.setVendorRepresentativeID(((EmployeeObj) empMap
								.get(Type.getString(entry.getValue())))
								.getUserID());
					} catch (Exception e) {
						con.setVendorRepresentativeID(0);
					}

				} else if (entry.getKey().equals("质保期"))
					con.setWarrantyDefinition(Type.getString(entry.getValue()));
				else if (entry.getKey().equals("质保开始时间"))
					con.setWarrantyStartDate(Type.getDate(entry.getValue()));
				else if (entry.getKey().equals("合同摘要"))
					con.setContractSummary(Type.getString(entry.getValue()));
				else if (entry.getKey().equals("备注"))
					con.setRemark(Type.getString(entry.getValue()));
				else if (entry.getKey().equals("最新修改时间"))
					con.setUpdateDate(Type.getDate(entry.getValue()));

			}
			// System.out.println(con.getProjectOBJ().getProjectID() + "//" +
			// con.getContractID());
			
			try{
				con.setBuyerRepresentativeID(Type.GetInt(newBuyerMap.get(con.getBuyer()
						+ "~" + con.getBuyerRepresentativeName())));
				
			} catch(Exception e){
				e.printStackTrace();
				con.setBuyerRepresentativeID(0);
			}
			
			conProjMap.put(con.getProjectOBJ().getProjectCode(), con);
			// conMap.put(con.getContractID(), con);
			maxContractID = con.getContractID();

			contractListForXMIS.add(con);
			iCount++;
		}

		doDataTransferForIncomeContract(contractListForXMIS);

		return conProjMap;

	}

	/* 收入的追加合同 */
	private static void doDataTransferForSupplementalContract(Map conProjMap) {

		List incomeContractListFromJKGL = getDataListFromJKGL("收入合同追加合同");

		List contractListForXMIS = new ArrayList();

		int iCount = 0;
		for (Iterator i = incomeContractListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			ContractObj con = new ContractObj();
			ContractObj conParent = new ContractObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				con.setContractTypeID(1);
				con.setSourceTypeID(1);

				if (entry.getKey().equals("自动编号")) {
					// con.setContractID(maxContractID + j + 1);
					con.setContractID(maxContractID + 1);
				} else if (entry.getKey().equals("项目编号")) {

					conParent = (ContractObj) conProjMap.get(Type
							.getString(entry.getValue()));

					con.setContractNo(conParent.getContractNo().equals("") ? ""
							: conParent.getContractNo() + "-追加");
					con.setParentID(conParent.getContractID());
					con.setRegisterID(conParent.getRegisterID());
					con
							.setContractName(conParent.getContractName()
									.equals("") ? "" : conParent
									.getContractName()
									+ " 追加");
					con
							.setAbbreviation(conParent.getAbbreviation()
									.equals("") ? "" : conParent
									.getAbbreviation()
									+ " 追加");
					con.setBuyerID(conParent.getBuyerID());
					con.setVendorID(conParent.getVendorID());
					con.setCurrencyID(1);
					con.setDeptIDOfVendor(conParent.getDeptIDOfVendor());
					con.setVendorRepresentativeID(conParent
							.getVendorRepresentativeID());
					con.setUpdateDate(conParent.getUpdateDate());

				}

				else if (entry.getKey().equals("签订日期"))
					con.setSignatureDate(Type.getDate(entry.getValue()));

				else if (entry.getKey().equals("金额"))
					con.setContractAmt(Type.GetDouble(entry.getValue()));

				else if (entry.getKey().equals("描述"))
					con.setContractSummary(Type.getString(entry.getValue()));
				else if (entry.getKey().equals("备注"))
					con.setRemark(Type.getString(entry.getValue()));

			}

			contractListForXMIS.add(con);
			maxContractID = con.getContractID();
			iCount++;
		}

		doDataTransferForSupplementalContract(contractListForXMIS);

	}

	private static void doDataTransferForPaymentCondition(Map conProjMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("收入合同付款条件");

		List contractListForXMIS = new ArrayList();

		int iCount = 1;
		int contractID = 0;
		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			PaymentConditionObj pay = new PaymentConditionObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				if (entry.getKey().equals("自动编号")) {
					// pay.setPaymentConditionID(Type.GetInt(entry.getValue()));
					pay.setPaymentConditionID(iCount + 1);

				} else if (entry.getKey().equals("项目编号")) {

					pay.setContractID(((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getContractID());
					if (contractID == ((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getContractID())
						iCount++;
					else {
						iCount = 1;
						pay.setPaymentConditionID(iCount);
					}
				}

				else if (entry.getKey().equals("付款条件"))
					pay.setDescription(Type.getString(entry.getValue()));

				else if (entry.getKey().equals("百分比"))

					// System.out.println(entry.getValue());
					pay.setPaymentPercent((int) Type
							.GetDouble(entry.getValue()));

				else if (entry.getKey().equals("金额"))
					pay.setPamymentAmt(Type.GetDouble(entry.getValue()));

			}
			contractID = pay.getContractID();
			contractListForXMIS.add(pay);
			// iCount++;
		}

		doDataTransferForPaymentCondition(contractListForXMIS);
	}

	// doDataTransferForPayoutContractPaymentDetail
	private static void doDataTransferForPayoutContractPaymentDetail(
			Map payoutConProjMap, Map empMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("支出合同付款");

		List contractListForXMIS = new ArrayList();

		int iCount = 1;
		int contractID = 0;
		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			PaymentDetailObj pay = new PaymentDetailObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				if (entry.getKey().equals("自动编号")) {
					pay.setPaymentID(iCount + 1);

				} else if (entry.getKey().equals("支出合同编号")) {

					pay.setContractID(((ContractObj) payoutConProjMap.get(Type
							.getString(entry.getValue()))).getContractID());
					if (contractID == ((ContractObj) payoutConProjMap.get(Type
							.getString(entry.getValue()))).getContractID())
						iCount++;
					else {
						iCount = 1;
						pay.setPaymentID(iCount);
					}
				}

				else if (entry.getKey().equals("金额"))
					pay.setPaymentAmt(Type.GetDouble(entry.getValue()));

				else if (entry.getKey().equals("时间"))
					pay.setPaymentDate(Type.getDate(entry.getValue()));

				else if (entry.getKey().equals("经办人")) {
					// pay.setTransactorID(Type.GetInt(empMap.get(entry
					// .getValue())));

					try {
						pay.setTransactorID(((EmployeeObj) empMap.get(Type
								.getString(entry.getValue()))).getUserID());
					} catch (Exception e) {
						pay.setTransactorID(0);
					}

				} else if (entry.getKey().equals("备注"))
					pay.setRemark(Type.getString(entry.getValue()));

			}
			contractID = pay.getContractID();
			contractListForXMIS.add(pay);
			// iCount++;
		}

		doDataTransferForPayoutContractPaymentDetail(contractListForXMIS);
	}

	private static void doDataTransferForPaymentDetail(Map conProjMap,
			Map empMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("收入合同付款明细");

		List contractListForXMIS = new ArrayList();

		int iCount = 1;
		int contractID = 0;
		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			PaymentDetailObj pay = new PaymentDetailObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				if (entry.getKey().equals("自动编号")) {
					pay.setPaymentID(iCount + 1);

				} else if (entry.getKey().equals("项目编号")) {

					pay.setContractID(((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getContractID());
					if (contractID == ((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getContractID())
						iCount++;
					else {
						iCount = 1;
						pay.setPaymentID(iCount);
					}
				}

				else if (entry.getKey().equals("金额"))
					pay.setPaymentAmt(Type.GetDouble(entry.getValue()));

				else if (entry.getKey().equals("日期"))
					pay.setPaymentDate(Type.getDate(entry.getValue()));

				else if (entry.getKey().equals("经办人")) {
					// pay.setTransactorID(Type.GetInt(empMap.get(entry
					// .getValue())));

					try {
						pay.setTransactorID(((EmployeeObj) empMap.get(Type
								.getString(entry.getValue()))).getUserID());
					} catch (Exception e) {
						pay.setTransactorID(0);
					}

				} else if (entry.getKey().equals("备注"))
					pay.setRemark(Type.getString(entry.getValue()));

			}
			contractID = pay.getContractID();
			contractListForXMIS.add(pay);
			// iCount++;
		}

		doDataTransferForPaymentDetail(contractListForXMIS);
	}

	private static void doDataTransferForProjectMember(Map conProjMap,
			Map empMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("收入合同项目人员");

		List contractListForXMIS = new ArrayList();

		int iCount = 1;
		int contractID = 0;
		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			ProjectTaskObj task = new ProjectTaskObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				if (entry.getKey().equals("自动编号")) {
					task.setTaskID(Type.GetInt(entry.getValue()));

				} else if (entry.getKey().equals("项目编号")) {

					task.setProjectID(((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getProjectOBJ()
							.getProjectID());
					task.setBeginDate(((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getSignatureDate());
				}

				else if (entry.getKey().equals("员工ID")) {
					try {
						task.setAssigedToEmpID(((EmployeeObj) empMap.get(Type
								.getString(entry.getValue()))).getUserID());
						task.setAssigedToDeptID(((EmployeeObj) empMap.get(Type
								.getString(entry.getValue())))
								.getDepartmentID());
					} catch (Exception e) {
						task.setAssigedToEmpID(0);
						task.setAssigedToDeptID(0);
					}
				} else if (entry.getKey().equals("备注"))
					task.setRemark(Type.getString(entry.getValue()));

			}
			contractListForXMIS.add(task);
			// iCount++;
		}

		doDataTransferForProjectMember(contractListForXMIS);
	}

	// doDataTransferForReceipt
	private static void doDataTransferForReceipt(Map conProjMap, Map empMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("收入合同收据");

		List contractListForXMIS = new ArrayList();

		int iCount = 1;
		int contractID = 0;
		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			InvoiceObj inv = new InvoiceObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				if (entry.getKey().equals("自动编号")) {
					inv.setInvoiceID(iCount + 1);

				} else if (entry.getKey().equals("项目编号")) {

					inv.setContractID(((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getContractID());
					if (contractID == ((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getContractID())
						iCount++;
					else {
						iCount = 1;
						inv.setInvoiceID(iCount);
					}
				}

				else if (entry.getKey().equals("金额"))
					inv.setInvoiceAMT(Type.GetDouble(entry.getValue()));

				else if (entry.getKey().equals("日期"))
					inv.setInvoiceDate(Type.getDate(entry.getValue()));

				else if (entry.getKey().equals("经办人")) {
					// pay.setTransactorID(Type.GetInt(empMap.get(entry
					// .getValue())));

					try {
						inv.setTransactorID(((EmployeeObj) empMap.get(Type
								.getString(entry.getValue()))).getUserID());
					} catch (Exception e) {
						inv.setTransactorID(0);
					}

				} else if (entry.getKey().equals("备注"))
					inv.setInvoiecRemark(Type.getString(entry.getValue()));

			}
			contractID = inv.getContractID();
			contractListForXMIS.add(inv);
			// iCount++;
		}

		doDataTransferForReceipt(contractListForXMIS);
	}

	// doDataTransferForPayoutContractInvoice
	private static void doDataTransferForPayoutContractInvoice(
			Map payoutConProjMap, Map empMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("支出合同报销情况");

		List contractListForXMIS = new ArrayList();

		int iCount = 1;
		int contractID = 0;
		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			InvoiceObj inv = new InvoiceObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				if (entry.getKey().equals("自动编号")) {
					inv.setInvoiceID(iCount + 1);

				} else if (entry.getKey().equals("支出合同编号")) {

					inv.setContractID(((ContractObj) payoutConProjMap.get(Type
							.getString(entry.getValue()))).getContractID());
					if (contractID == ((ContractObj) payoutConProjMap.get(Type
							.getString(entry.getValue()))).getContractID())
						iCount++;
					else {
						iCount = 1;
						inv.setInvoiceID(iCount);
					}
				}

				else if (entry.getKey().equals("金额")) {
					inv.setInvoiceAMT(Type.GetDouble(Type.getString(
							entry.getValue()).replaceAll(",", "")));

				} else if (entry.getKey().equals("时间"))
					inv.setInvoiceDate(Type.getDate(entry.getValue()));

				else if (entry.getKey().equals("经办人")) {
					// pay.setTransactorID(Type.GetInt(empMap.get(entry
					// .getValue())));

					try {
						inv.setTransactorID(((EmployeeObj) empMap.get(Type
								.getString(entry.getValue()))).getUserID());
					} catch (Exception e) {
						inv.setTransactorID(0);
					}

				} else if (entry.getKey().equals("备注"))
					inv.setInvoiecRemark(Type.getString(entry.getValue()));

			}
			contractID = inv.getContractID();
			contractListForXMIS.add(inv);
			// iCount++;
		}

		doDataTransferForPayoutContractInvoiceDetail(contractListForXMIS);
	}

	// doDataTransferForContractInvoice
	private static void doDataTransferForContractInvoice(Map conProjMap,
			Map empMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("收入合同发票");

		List contractListForXMIS = new ArrayList();

		int iCount = 1;
		int contractID = 0;
		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			InvoiceObj inv = new InvoiceObj();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				if (entry.getKey().equals("自动编号")) {
					inv.setInvoiceID(iCount + 1);

				} else if (entry.getKey().equals("项目编号")) {

					inv.setContractID(((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getContractID());
					if (contractID == ((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getContractID())
						iCount++;
					else {
						iCount = 1;
						inv.setInvoiceID(iCount);
					}
				}

				else if (entry.getKey().equals("金额"))
					inv.setInvoiceAMT(Type.GetDouble(entry.getValue()));

				else if (entry.getKey().equals("日期"))
					inv.setInvoiceDate(Type.getDate(entry.getValue()));

				else if (entry.getKey().equals("经办人")) {
					// pay.setTransactorID(Type.GetInt(empMap.get(entry
					// .getValue())));

					try {
						inv.setTransactorID(((EmployeeObj) empMap.get(Type
								.getString(entry.getValue()))).getUserID());
					} catch (Exception e) {
						inv.setTransactorID(0);
					}

				} else if (entry.getKey().equals("备注"))
					inv.setInvoiecRemark(Type.getString(entry.getValue()));

			}
			contractID = inv.getContractID();
			contractListForXMIS.add(inv);
			// iCount++;
		}

		doDataTransferForInvoiceDetail(contractListForXMIS);
	}

	// doDataTransferForPayoutContractAttachment
	private static void doDataTransferForPayoutContractAttachment(Map conProjMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("支出合同关联文件");

		List contractListForXMIS = new ArrayList();
		int aCount = 0;

		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				if (entry.getKey().equals("aCount")) {

					if (Type.GetInt(entry.getValue()) / pageSize * pageSize == Type
							.GetInt(entry.getValue()))
						aCount = Type.GetInt(entry.getValue()) / pageSize;
					else
						aCount = Type.GetInt(entry.getValue()) / pageSize + 1;

				}
			}
		}

		for (int z = 1; z <= aCount; z++) {

			incomePaymentConditionListFromJKGL = getDataListFromJKGL("支出合同关联文件_"
					+ z);

			contractListForXMIS = new ArrayList();

			for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
					.hasNext();) {

				Map map = (HashMap) i.next();
				AttachmentFile aFile = new AttachmentFile();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();

					// if (iCount == 0)
					// System.out.println(entry.getKey() + "=" +
					// entry.getValue());

					if (entry.getKey().equals("自动编号")) {

					} else if (entry.getKey().equals("支出合同编号")) {

						aFile.setRefID(((ContractObj) conProjMap.get(Type
								.getString(entry.getValue()))).getContractID());
						aFile.setUploadDate(((ContractObj) conProjMap.get(Type
								.getString(entry.getValue()))).getUpdateDate());
						aFile.setUploadID(((ContractObj) conProjMap.get(Type
								.getString(entry.getValue()))).getRegisterID());

					}

					else if (entry.getKey().equals("文件名称"))
						aFile.setFileName(Type.getString(entry.getValue()));

					else if (entry.getKey().equals("文件内容"))
						aFile.setContent((byte[]) entry.getValue());

				}
				contractListForXMIS.add(aFile);
				// iCount++;
			}

			doDataTransferForPayoutContractAttachment(contractListForXMIS);
		}

	}

	// doDataTransferForContractAttachment
	private static void doDataTransferForContractAttachment(Map conProjMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("收入合同关联文件");

		List contractListForXMIS = new ArrayList();

		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			AttachmentFile aFile = new AttachmentFile();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				if (entry.getKey().equals("自动编号")) {

				} else if (entry.getKey().equals("项目编号")) {

					aFile.setRefID(((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getContractID());
					aFile.setUploadDate(((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getUpdateDate());
					aFile.setUploadID(((ContractObj) conProjMap.get(Type
							.getString(entry.getValue()))).getRegisterID());

				}

				else if (entry.getKey().equals("文件名称"))
					aFile.setFileName(Type.getString(entry.getValue()));

				else if (entry.getKey().equals("文件内容"))
					aFile.setContent((byte[]) entry.getValue());

			}
			contractListForXMIS.add(aFile);
			// iCount++;
		}

		maxAttachmentID = contractListForXMIS.size();
		doDataTransferForContractAttachment(contractListForXMIS);
	}

	private static void doDataTransferForContractMapping(Map conProjMap,
			Map payoutConProjMap) {

		List incomePaymentConditionListFromJKGL = getDataListFromJKGL("支出合同对应收入合同金额");

		List contractListForXMIS = new ArrayList();

		for (Iterator i = incomePaymentConditionListFromJKGL.listIterator(); i
				.hasNext();) {

			Map map = (HashMap) i.next();
			String[] conMapping = new String[5];

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();

			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				// if (iCount == 0)
				// System.out.println(entry.getKey() + "=" + entry.getValue());

				if (entry.getKey().equals("自动编号")) {
					conMapping[0] = Type.getString(entry.getValue());

				}

				else if (entry.getKey().equals("收入合同编号"))
					try {
						conMapping[1] = Type
								.getString(((ContractObj) conProjMap.get(Type
										.getString(entry.getValue())))
										.getContractID());
					} catch (Exception e) {
						conMapping[1] = "-1";
					}

				else if (entry.getKey().equals("支出合同编号"))

					conMapping[2] = Type
							.getString(((ContractObj) payoutConProjMap.get(Type
									.getString(entry.getValue())))
									.getContractID());

				else if (entry.getKey().equals("金额"))
					conMapping[3] = Type.getString(entry.getValue());

				else if (entry.getKey().equals("备注"))
					conMapping[4] = Type.getString(entry.getValue());

			}
			contractListForXMIS.add(conMapping);
			// iCount++;
		}

		doDataTransferForContractMapping(contractListForXMIS);

	}

	private static Map doDataTransferForBuyerRepresentativeName() {

		// Map crmMap = new HashMap();

		List crmMapList = new ArrayList();

		List provinceListFromJKGL = getDataListFromJKGL("甲方代表");

		for (Iterator i = provinceListFromJKGL.listIterator(); i.hasNext();) {

			String[] crmMapStr = new String[3];
			Map map = (HashMap) i.next();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();
			int id = 0;
			String name = null;
			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();

				if (entry.getKey().equals("甲方")) {
					// if(Type.getString(entry.getValue()).equals("湖南潇湘综合开发有限公司"))
					// System.out.println("the other dept id = " +
					// Type.GetInt(companyOtherDeptMap.get(Type.getString(entry.getValue()))));

					// id =
					// Type.GetInt(companyOtherDeptMap.get(Type.getString(entry.getValue())));
					crmMapStr[1] = Type.getString(companyOtherDeptMap.get(Type
							.getString(entry.getValue())));
					crmMapStr[2] = Type.getString(entry.getValue());
					
					
				} else if (entry.getKey().equals("甲方代表"))
					// name = Type.getString(entry.getValue());
					crmMapStr[0] = Type.getString(entry.getValue());

//				else if (entry.getKey().equals("项目编号"))
//					crmMapStr[2] = Type.getString(((ContractObj) conProjMap
//							.get(Type.getString(entry.getValue())))
//							.getContractID());

			}

			// System.out.println(crmMapStr[1]+ " = " + crmMapStr[0]);
			
			crmMapList.add(crmMapStr);

			// crmMap.put(name, id);

		}

		return doDataTransferBuyerRepresentativeName(crmMapList);

	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		System.out.println("Data Transfer start .......");
		try {
			jkglConn = getConnection("jkgl");
			xmisConn = getConnection("xmis");

			long l = new Date().getTime();
			xmisConn.setAutoCommit(false);

			/* 1  人员 & 人员部门对应 */
			Map empMap = (LinkedHashMap) doDataTransferForEmp();

			/* 2  公司 Data */
			Map companyMap = (LinkedHashMap) doDataTransferForCompay();

			/* 3  省份 */
			doDataTransferForProvince();

			/* ==== 收入合同 系列数据 ===== */

			/* 4  甲方代表 */
			Map newBuyerMap = doDataTransferForBuyerRepresentativeName();

			/* 5  收入合同 & 项目名称 */
			Map conProjMap = (LinkedHashMap) doDataTransferForContractProject(
					empMap, companyMap, newBuyerMap);

			/* 6  收入追加合同 */
			doDataTransferForSupplementalContract(conProjMap);

			/* 7  收入合同付款条件 */
			doDataTransferForPaymentCondition(conProjMap);

			/* 8  收入合同付款明细 */
			doDataTransferForPaymentDetail(conProjMap, empMap);

			/* 9  收入合同项目人员 */
			doDataTransferForProjectMember(conProjMap, empMap);

			/* 10  收入合同发票 */
			doDataTransferForContractInvoice(conProjMap, empMap);

			/* 11  收入合同收据 */
			doDataTransferForReceipt(conProjMap, empMap);

			/* 12  收入合同关联文件 */
			doDataTransferForContractAttachment(conProjMap);

			// /* 收入合同设备 */
			// doDataTransferForDevice(conProjMap);

			/* ==== 支出合同 系列数据 ===== */

			/* 13  支出合同 合同ID = 最大收入合同ID +１ */
			Map payoutConProjMap = (LinkedHashMap) doDataTransferForPayoutContractProject(
					empMap, companyMap);

			/* 14  收入合同 支出合同 对应 */
			doDataTransferForContractMapping(conProjMap, payoutConProjMap);

			/* 15  支出合同付款 */
			doDataTransferForPayoutContractPaymentDetail(payoutConProjMap,
					empMap);

			/* 16  支出合同报销情况 */
			doDataTransferForPayoutContractInvoice(payoutConProjMap, empMap);

			/* 17  支出合同关联文件 */
			doDataTransferForPayoutContractAttachment(payoutConProjMap);

			// /* 支出合同到货 */
			// doDataTransferForPayoutContractDevice(conProjMap,
			// payoutConProjMap);

			xmisConn.commit();
			System.out.println(new Date().getTime() - l);
			System.out.println("DataTransfer OK!!");

		} catch (Exception e) {
			try {
				xmisConn.rollback();
			} catch (Exception ei) {
				ei.printStackTrace();
			}
			e.printStackTrace();

		} finally {
			try {
				if (jkglConn != null)
					jkglConn.close();
				if (xmisConn != null)
					xmisConn.close();

			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

}
