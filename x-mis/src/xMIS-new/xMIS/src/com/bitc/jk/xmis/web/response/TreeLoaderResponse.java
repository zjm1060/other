package com.bitc.jk.xmis.web.response;

import java.util.ArrayList;
import java.util.List;

import net.sf.json.JSONArray;
import net.sf.json.JSONSerializer;
import net.sf.json.JsonConfig;
import net.sf.json.util.PropertyFilter;

public class TreeLoaderResponse {
	private List<TreeNode> nodes;

	public TreeLoaderResponse() {
		nodes = new ArrayList<TreeNode>();
	}

	public void addTreeNode(TreeNode node) {
		this.nodes.add(node);
	}

	public TreeNode addTreeNode(String text, String id, Boolean leaf,
			String cls, String iconCls) {
		TreeNode node = new TreeNode(text, id, leaf, cls, iconCls);
		this.nodes.add(node);
		return node;
	}
	
	public List<TreeNode> getNodes() {
		return nodes;
	}

	public void setNodes(List<TreeNode> nodes) {
		this.nodes = nodes;
	}

	@Override
	public String toString() {
		return toJsonString();
	}

	public String toString(JsonConfig config) {
		return toJsonString(config);
	}

	public String toJsonString(JsonConfig config) {
		final PropertyFilter filter = config.getJsonPropertyFilter();
		config.setJsonPropertyFilter(new PropertyFilter() {
			public boolean apply(Object arg0, String arg1, Object arg2) {
				if (arg0 instanceof TreeNode && !((TreeNode) arg0).getLeaf()
						&& "checked".equals(arg1)) {
					return true;
				}
				if (filter != null) {
					return filter.apply(arg0, arg1, arg2);
				}
				return false;
			}
		});
		return JSONSerializer.toJSON(nodes, config).toString();
	}

	public String toJsonString() {
		JSONArray array = JSONArray.fromObject(nodes);
		return array.toString();
	}
}
