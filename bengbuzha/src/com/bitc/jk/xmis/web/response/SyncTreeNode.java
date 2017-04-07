package com.bitc.jk.xmis.web.response;

import java.util.ArrayList;
import java.util.List;

public class SyncTreeNode extends TreeNode {
	private List<TreeNode> children;

	public SyncTreeNode() {
		children = new ArrayList<TreeNode>();
	}

	public void addChildren(TreeNode treeNode) {
		children.add(treeNode);
	}

	public List<TreeNode> getChildren() {
		return children;
	}

	public void setChildren(List<TreeNode> children) {
		this.children = children;
	}
}
