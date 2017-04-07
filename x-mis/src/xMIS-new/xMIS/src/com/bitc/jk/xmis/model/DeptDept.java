package com.bitc.jk.xmis.model;

import java.io.Serializable;

public class DeptDept implements Serializable {

	private static final long serialVersionUID = -8972778079380685699L;

	private int parentId;
	private int childId;
	private DepartmentObj child;
	private DepartmentObj parent;
	private Integer index;

	public DepartmentObj getChild() {
		return child;
	}

	public void setChild(DepartmentObj child) {
		this.child = child;
	}

	public DepartmentObj getParent() {
		return parent;
	}

	public void setParent(DepartmentObj parent) {
		this.parent = parent;
	}

	public Integer getIndex() {
		return index;
	}

	public void setIndex(Integer index) {
		this.index = index;
	}

	public int getParentId() {
		return parentId;
	}

	public void setParentId(int parentId) {
		this.parentId = parentId;
	}

	public int getChildId() {
		return childId;
	}

	public void setChildId(int childId) {
		this.childId = childId;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + childId;
		result = prime * result + parentId;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		DeptDept other = (DeptDept) obj;
		if (childId != other.childId)
			return false;
		if (parentId != other.parentId)
			return false;
		return true;
	}

}
