package com.bitc.jk.xmis.web.response;

import java.util.HashMap;
import java.util.Map;

public class TreeNode {
	private String text;
	private String id;
	private Boolean leaf;
	private String cls;
	private Boolean checked;
	private String iconCls;
	private Map<String, String> attributes;

	public Map<String, String> getAttributes() {
		return attributes;
	}

	public void setAttributes(Map<String, String> attributes) {
		this.attributes = attributes;
	}

	public void addAttribute(String key, String value) {
		this.attributes.put(key, value);
	}

	public TreeNode() {
		super();
		attributes = new HashMap<String, String>();
	}

	public TreeNode(String text, String id, Boolean leaf, String cls,
			String iconCls) {
		this();
		this.text = text;
		this.id = id;
		this.leaf = leaf;
		this.cls = cls;
		this.iconCls = iconCls;
	}

	public String getText() {
		return text;
	}

	public void setText(String text) {
		this.text = text;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public Boolean getLeaf() {
		return leaf;
	}

	public void setLeaf(Boolean leaf) {
		this.leaf = leaf;
	}

	public Boolean getChecked() {
		return checked;
	}

	public void setChecked(Boolean checked) {
		this.checked = checked;
	}

	public String getCls() {
		return cls;
	}

	public void setCls(String cls) {
		this.cls = cls;
	}

	public String getIconCls() {
		return iconCls;
	}

	public void setIconCls(String iconCls) {
		this.iconCls = iconCls;
	}

}