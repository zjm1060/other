package com.bitc.jk.xmis.model.security;

import java.util.ArrayList;
import java.util.List;

public class NodeInfo {
	private String id;
	private String text;
	private List<NodeInfo> children;

	public NodeInfo() {
		children = new ArrayList<NodeInfo>();
	}

	public NodeInfo(String id) {
		this();
		this.id = id;
	}

	public NodeInfo(String id, String text) {
		this();
		this.id = id;
		this.text = text;
	}
	
	public int getSize(){
		if(children.size()==0){
			return 1;
		}
		int sum=0;
		for(NodeInfo node:children){
			sum+=node.getSize();
		}
		return sum;
	}

	public NodeInfo addChild(NodeInfo node) {
		int index = children.indexOf(node);
		if (index > -1) {
			return children.get(index);
		} else {
			children.add(node);
			return node;
		}
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getText() {
		return text;
	}

	public void setText(String text) {
		this.text = text;
	}

	public List<NodeInfo> getChildren() {
		return children;
	}

	public void setChildren(List<NodeInfo> children) {
		this.children = children;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((id == null) ? 0 : id.hashCode());
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
		NodeInfo other = (NodeInfo) obj;
		if (id == null) {
			if (other.id != null)
				return false;
		} else if (!id.equals(other.id))
			return false;
		return true;
	}
}
