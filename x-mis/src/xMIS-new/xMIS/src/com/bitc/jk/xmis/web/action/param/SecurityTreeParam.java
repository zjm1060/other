package com.bitc.jk.xmis.web.action.param;

import net.sf.json.JsonConfig;
import net.sf.json.util.CycleDetectionStrategy;

public class SecurityTreeParam {
	
	/**
	 * 树的级别
	 */
	public static final int LEVEL_ROOT = 0;
	public static final int LEVEL_BIZMODULE = 1;
	public static final int LEVEL_BIZSUBMODULE = 2;
	public static final int LEVEL_BIZFUNCTION = 3;
	public static final int LEVEL_BIZDATA = 4;
	
	/**
	 * 数据是否已存在 
	 *  0 ：不存在 
	 *  1 ：已存在
	 */
	public static final int NEW_DATA = 0;
	public static final int OLD_DATA = 1;
	
	/**
	 * 树要显示的最低级别
	 */
	private int leafLevel;
	
	private String node;
	
	/**
	 * constructor without params
	 */
	public SecurityTreeParam() {
		super();
	}
	
	public String getNode() {
		return node;
	}

	public void setNode(String node) {
		this.node = node;
	}
	
	public int getLeafLevel() {
		return leafLevel;
	}

	public void setLeafLevel(int leafLevel) {
		this.leafLevel = leafLevel;
	}

	public int getNodeLevel(){
		String[] args = node.split("-");
		if(args.length > 0){
			int level = Integer.parseInt(args[0]);
			if(LEVEL_BIZMODULE == level){
				return LEVEL_BIZMODULE;
			}else if(LEVEL_BIZSUBMODULE == level){
				return LEVEL_BIZSUBMODULE;
			}else if(LEVEL_BIZFUNCTION == level){
				return LEVEL_BIZFUNCTION;
			}else if(LEVEL_BIZDATA == level){
				return LEVEL_BIZDATA;
			}
		}
		//默认为根节点
		return LEVEL_ROOT;
	}
	
	public String getNodeId(){
		return node.lastIndexOf('-') >= 0 ? node.substring(node
				.lastIndexOf('-') + 1) : node;
	}

	public JsonConfig getJsonConfig() {
		JsonConfig config = new JsonConfig();
		if (true) {
			config.setExcludes(new String[] { "checked" });
		}
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return config;
	}
}
