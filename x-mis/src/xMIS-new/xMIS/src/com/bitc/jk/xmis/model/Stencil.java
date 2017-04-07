package com.bitc.jk.xmis.model;

import java.io.Serializable;

public class Stencil implements Serializable {

	private static final long serialVersionUID = -4542884795990036758L;

	private int stencilID;
	private int deptID;
	private String content;
	private String keyWords;

	public Stencil(int stencilID, int deptID, String content, String keyWords) {
		super();
		this.stencilID = stencilID;
		this.deptID = deptID;
		this.content = content;
		this.keyWords = keyWords;
	}

	public Stencil(int stencilID) {
		super();
		this.stencilID = stencilID;
	}

	public int getStencilID() {
		return stencilID;
	}

	public void setStencilID(int stencilID) {
		this.stencilID = stencilID;
	}

	public int getDeptID() {
		return deptID;
	}

	public void setDeptID(int deptID) {
		this.deptID = deptID;
	}

	public String getKeyWords() {
		return keyWords;
	}

	public void setKeyWords(String keyWords) {
		this.keyWords = keyWords;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}

	public String getContentResult(String r) {
		if(r == null || "".equals(r))
			return "";
		String[] values = r.split(">,<");
		if(values!=null&&values.length>0){
			values[0] = values[0].substring(1);
			values[values.length-1] = values[values.length-1].substring(0,values[values.length-1].length()-1);
		}
		
		boolean isEmpty = true;
		for(int i = 0;i<values.length;i++){
			if(!"".equals(values[i].trim())){
				isEmpty = false;
				break;
			}
		}
		if(isEmpty)
			return "";
		
		String content = this.content;
		StringBuilder result = new StringBuilder("");
		for(int i = 0;i<values.length;i++){
			if(content.indexOf("{}")>-1){
				result.append(content.substring(0, content.indexOf("{}")));
				result.append(" " + values[i] + " ");
				content = content.substring(content.indexOf("{}")+2);
			}else{
				result.append(content);
			}
			
		}
		return result.toString();
	}
}
