package com.bitc.jk.xmis.web.action.param;

import net.sf.json.JsonConfig;
import net.sf.json.util.CycleDetectionStrategy;

public class OrgTreeParam {

	public static final int LEVEL_ROOT = 0;
	public static final int LEVEL_COMP = 1;
	public static final int LEVEL_DEPT1 = 2;
	public static final int LEVEL_DEPT2 = 4;
	public static final int LEVEL_EMP = 8;

	public static final String ROOT = "root";
	public static final String PREFIX_COMP = "comp";
	public static final String PREFIX_DEPT1 = "dept1";
	public static final String PREFIX_DEPT2 = "dept2";
	public static final String PREFIX_EMP = "emp";
	/**
	 * 组织树要显示的最高级别
	 */
	private int rootLevel;
	/**
	 * 组织树要显示的最低级别
	 */
	private int leafLevel;
	/**
	 * 对最高级别节点的过滤
	 */
	private String rootFilter;
	/**
	 * 是否在叶子结点上显示复选框
	 */
	private boolean checkable = false;
	/**
	 * 是否为单选
	 */
	private boolean singleCheck = false;

	/**
	 * 当前被点击的节点ID
	 */
	private String node;

	/**
	 * 对名称的过滤，模糊匹配
	 */
	private String nameFilter;
	
	private String attributor;

	public String getAttributor() {
		return attributor;
	}

	public void setAttributor(String attributor) {
		this.attributor = attributor;
	}

	public OrgTreeParam() {
	}

	public int getRootLevel() {
		return rootLevel;
	}

	public void setRootLevel(int rootLevel) {
		this.rootLevel = rootLevel;
	}

	public int getLeafLevel() {
		return leafLevel;
	}

	public void setLeafLevel(int leafLevel) {
		this.leafLevel = leafLevel;
	}

	public boolean isCheckable() {
		return checkable;
	}

	public void setCheckable(boolean checkable) {
		this.checkable = checkable;
	}

	public boolean isSingleCheck() {
		return singleCheck;
	}

	public void setSingleCheck(boolean singleCheck) {
		this.singleCheck = singleCheck;
	}

	public String getNode() {
		return node;
	}

	public void setNode(String node) {
		this.node = node;
	}

	public String getRootFilter() {
		return rootFilter;
	}

	public void setRootFilter(String rootFitler) {
		this.rootFilter = rootFitler;
	}

	public JsonConfig getJsonConfig() {
		JsonConfig config = new JsonConfig();
		if (!checkable) {
			config.setExcludes(new String[] { "checked" });
		}
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return config;
	}

	/**
	 * @return 当前请求node的Level，例如点击了公司级别的节点，将返回LEVEL_COMP，实际为查询该公司下的部门
	 */
	public int getNodeLevel() {
		if (node.startsWith(ROOT)) {
			// 第一次请求为ROOT，返回比指定的最高级别更高一级别
			return rootLevel >> 1;
		}
		if (node.startsWith(PREFIX_COMP)) {
			return LEVEL_COMP;
		} else if (node.startsWith(PREFIX_DEPT1)) {
			return LEVEL_DEPT1;
		} else if (node.startsWith(PREFIX_DEPT2)) {
			return LEVEL_DEPT2;
		} else if (node.startsWith(PREFIX_EMP)) {
			return LEVEL_EMP;
		}
		return -1;
	}

	public String getNodeId() {
		return node.lastIndexOf('-') >= 0 ? node.substring(node
				.lastIndexOf('-') + 1) : node;
	}

	public static String getNodeId(int level, String id) {
		String nodeName = "%s-%s";
		String prefix = null;
		switch (level) {
		case LEVEL_COMP:
			prefix = PREFIX_COMP;
			break;
		case LEVEL_DEPT1:
			prefix = PREFIX_DEPT1;
			break;
		case LEVEL_DEPT2:
			prefix = PREFIX_DEPT2;
			break;
		case LEVEL_EMP:
			prefix = PREFIX_EMP;
			break;
		default:
			break;
		}
		return String.format(nodeName, prefix, id);
	}

}
