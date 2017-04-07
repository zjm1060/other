package com.bitc.jk.xmis.web.response;

import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.DepartmentObj;
import com.bitc.jk.xmis.model.DeptDept;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.EmployeePost;
import com.bitc.jk.xmis.model.sys.Post;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.web.action.param.OrgTreeParam;

public class TreeNodeEmailAttributor implements TreeNodeAttributor {
	private OrganizationDAO organizationDAO;

	public TreeNodeEmailAttributor() {
	}

	public void opt(List<TreeNode> nodes) {
		for (int i = 0; i < nodes.size(); i++) {
			TreeNode node = nodes.get(i);
			if (String.valueOf(OrgTreeParam.LEVEL_EMP).equals(
					node.getAttributes().get("level"))) {
				EmployeeObj emp = organizationDAO.getEmployee(Integer
						.valueOf(node.getId().substring(4)));
				node.addAttribute("email", emp.getEmailAddress());
				String position = "";
				int deptId = 0;// 事业部ID
				int tdeptId = 0;// 部门ID
				String deptName = "";
				String tdeptName = "";
				for (int j = 0; j < emp.getPostList().size(); j++) {
					EmployeePost empPost = emp.getPostList().get(j);
					DepartmentObj dept = empPost.getDepartment();
					tdeptId = dept.getDepartmentID();
					tdeptName = dept.getDepartmentName();
					if (dept.getParent() == null) {
						deptId = dept.getDepartmentID();
						deptName = dept.getDepartmentName();
					} else {
						DeptDept dd = dept.getParent();
						deptId = dd.getParentId();
						deptName = dd.getParent().getDepartmentName();
					}
					Post post = empPost.getPost();
					if (post != null)
						position = position + post.getName();
				}
				node.addAttribute("position", position);
				node.addAttribute("deptId", String.valueOf(deptId));
				node.addAttribute("deptName", deptName);
				node.addAttribute("tdeptId", String.valueOf(tdeptId));
				node.addAttribute("tdeptName", tdeptName);
				node.addAttribute("mobileNo", emp.getMobileNo());

			}
		}
	}

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}

}
