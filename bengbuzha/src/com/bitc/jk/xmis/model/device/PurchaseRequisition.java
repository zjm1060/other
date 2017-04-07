package com.bitc.jk.xmis.model.device;

import java.io.Serializable;
import java.lang.reflect.InvocationTargetException;
import java.sql.Date;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.collections.CollectionUtils;
import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.SimpleEmployee;

/**
 * 采购申请清单
 * <p>
 * 由项目物资清单产生的采购申请
 * </p>
 * 
 * @author Bom Wu
 * 
 */
public class PurchaseRequisition implements Serializable {

	private static final long serialVersionUID = -4393319206209965251L;

	/**
	 * ID
	 */
	private String id;
	/**
	 * 项目ID
	 */
	private String projectId;

	/**
	 * 项目编码
	 */
	private String projectCode;

	/**
	 * 项目名称
	 */
	private String projectName;

	/**
	 * 备注
	 */
	private String remark;

	/**
	 * 填表人
	 */
	private SimpleEmployee entryEmp;
	/**
	 * 填表时间
	 */
	private Date entryDate;

	/**
	 * 申请部门
	 */
	private SimpleDeptObj dept;

	public static String STATUS_1_NEW = "新建";
	/**
	 * 状态
	 */
	private String status = STATUS_1_NEW;
	/**
	 * 明细
	 */
	private Set<PurchaseRequisitionDetail> details;

	// 流程实例ID
	private String processInstanceId;

	public String getProcessInstanceId() {
		return processInstanceId;
	}

	public void setProcessInstanceId(String processInstanceId) {
		this.processInstanceId = processInstanceId;
	}

	public PurchaseRequisition() {
		details = new HashSet<PurchaseRequisitionDetail>();
	}

	public void addDetail(PurchaseRequisitionDetail detail) {
		this.details.add(detail);
		detail.setPurchaseRequisition(this);
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		if (StringUtils.isBlank(id)) {
			this.id = null;
		} else {
			this.id = id;
		}
	}

	public String getProjectId() {
		return projectId;
	}

	public void setProjectId(String projectId) {
		this.projectId = projectId;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public SimpleEmployee getEntryEmp() {
		return entryEmp;
	}

	public void setEntryEmp(SimpleEmployee entryEmp) {
		this.entryEmp = entryEmp;
	}

	public Date getEntryDate() {
		return entryDate;
	}

	public void setEntryDate(Date entryDate) {
		this.entryDate = entryDate;
	}

	public Set<PurchaseRequisitionDetail> getDetails() {
		return details;
	}

	public void setDetails(Set<PurchaseRequisitionDetail> details) {
		this.details = details;
	}

	public String getProjectCode() {
		return projectCode;
	}

	public void setProjectCode(String projectCode) {
		this.projectCode = projectCode;
	}

	public String getProjectName() {
		return projectName;
	}

	public void setProjectName(String projectName) {
		this.projectName = projectName;
	}

	public void generateDetails(List<DeviceObj> projectDevices,
			String seldeviceids) {
		Set<PurchaseRequisitionDetail> list = new HashSet<PurchaseRequisitionDetail>();
		Set<String> set = new HashSet<String>();
		if (!StringUtils.isEmpty(seldeviceids)) {
			String[] str = StringUtils.split(seldeviceids, ',');
			CollectionUtils.addAll(set, str);
		}
		for (Iterator<DeviceObj> it = projectDevices.iterator(); it.hasNext();) {
			DeviceObj deviceObj = it.next();
			if (set.size() > 0 && !set.contains(deviceObj.getId())) {
				continue;
			}
			PurchaseRequisitionDetail detail = new PurchaseRequisitionDetail();
			try {
				BeanUtils.copyProperties(detail, deviceObj);
				detail.setProjectDeviceId(detail.getId());
				detail.setId(null);
				detail.setNumber(deviceObj.getNumber()
						- deviceObj.getStatusCount()[3]);
				list.add(detail);
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				e.printStackTrace();
			}
		}
		this.setDetails(list);
	}

	public String getStatus() {
		return status;
	}

	public void setStatus(String status) {
		this.status = status;
	}

	public SimpleDeptObj getDept() {
		return dept;
	}

	public void setDept(SimpleDeptObj dept) {
		this.dept = dept;
	}

}
