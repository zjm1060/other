package com.bitc.jk.xmis.model;

import java.util.ArrayList;
import java.util.List;

public class Node {
	private String id;
	private String text;
	private List<Node> children;

	public Node() {
		children = new ArrayList<Node>();
	}

	public Node(String id) {
		this();
		this.id = id;
	}

	public Node(String id, String text) {
		this();
		this.id = id;
		this.text = text;
	}
	
	public int getSize(){
		if(children.size()==0){
			return 1;
		}
		int sum=0;
		for(Node node:children){
			sum+=node.getSize();
		}
		return sum;
	}

	public boolean contains(Node node) {
		return children.indexOf(node) > -1;
	}

	public Node addChild(Node node) {
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

	public List<Node> getChildren() {
		return children;
	}

	public void setChildren(List<Node> children) {
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
		Node other = (Node) obj;
		if (id == null) {
			if (other.id != null)
				return false;
		} else if (!id.equals(other.id))
			return false;
		return true;
	}
}
