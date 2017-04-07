/**
 * 
 */
package com.bitc.jk.xmis.model.security;

import java.util.List;

/**
 * @author Ted Li
 * 
 *         2011-5-12
 */
public class BizModuleObj extends BizModelObj {

	private List<BizSubModuleObj> bizSubModuleObjList;

	public BizModuleObj() {
	}

	public BizModuleObj(int id) {
		this.id = id;
	}

	public BizModuleObj(int id, String name) {
		this.id = id;
		this.name = name;
	}
	
	public BizModuleObj(String name) {
		this.name = name;
	}

	public List<BizSubModuleObj> getBizSubModuleObjList() {
		return bizSubModuleObjList;
	}

	public void setBizSubModuleObjList(List<BizSubModuleObj> bizSubModuleObjList) {
		this.bizSubModuleObjList = bizSubModuleObjList;
	}

	public void addBizSubModuleObjList(BizSubModuleObj obj) {
		bizSubModuleObjList.add(obj);
	}

	public String toJsonString() {
		StringBuilder store = new StringBuilder("[{");
		store.append("'bizModule-id':").append(this.id).append(",").append(
				"'bizModule-name':'").append(this.getName()).append("',")
				.append("'bizModule-url':'").append(this.getUrl()).append("',")
				.append("'bizModule-description':'").append(this.getDesc())
				.append("',").append("'bizModule-iconDeclare':'").append(
						this.getIconCls()).append("',").append(
						"'bizModule-identification':'").append(
						this.getIdentifies()).append("',").append(
						"'bizModule-commitPersonId':").append(
						this.getCommitPerson()).append(",").append(
						"'bizModule-commitPersonName':'").append(
						this.getCommitPersonName()).append("',").append(
						"'bizModule-commitDate':").append(this.getCommitDate())
				.append(",").append("'bizModule-commitStatus':").append(
						this.getCommitStatus()).append(",").append(
						"'bizModule-commitStatusName':'").append(
						this.getCommitStatusName()).append("',").append(
						"'bizModule-remark':'").append(this.getRemark())
				.append("',").append("'bizModule-px':").append(
						this.getSequenceNo());
		if (this.getBizSubModuleObjList() != null
				&& this.getBizSubModuleObjList().size() > 0) {
			store.append(",");
			store.append("[");
			for (int i = 0; i < this.getBizSubModuleObjList().size(); i++) {
				store.append("{");
				BizSubModuleObj obj = this.getBizSubModuleObjList().get(i);
				store.append("'bizSubModule-id':").append(obj.getId()).append(
						",").append("'bizSubModule-parentid':").append(
						this.getId()).append(",")
						.append("'bizSubModule-name':").append(obj.getName())
						.append(",").append("'bizSubModule-px':").append(
								obj.getSequenceNo());
			}
			store.append("]");
		}
		store.append("}]");
		return store.toString();
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + id;
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
		BizModelObj other = (BizModelObj) obj;
		if (id != other.id)
			return false;
		return true;
	}
}
