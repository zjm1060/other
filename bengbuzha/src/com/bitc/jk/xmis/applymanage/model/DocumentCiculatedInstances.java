package com.bitc.jk.xmis.applymanage.model;

import java.io.Serializable;
import java.util.Date;

public class DocumentCiculatedInstances implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 5651124619362370470L;
	
	
	private String instanc_id;
	private String form_id;
	private int from_id;
	private Date from_datetime;
	private int to_id;
	private String comments;
	
	public String getInstanc_id() {
		return instanc_id;
	}
	public void setInstanc_id(String instanc_id) {
		this.instanc_id = instanc_id;
	}
	public String getForm_id() {
		return form_id;
	}
	public void setForm_id(String form_id) {
		this.form_id = form_id;
	}
	public int getFrom_id() {
		return from_id;
	}
	public void setFrom_id(int from_id) {
		this.from_id = from_id;
	}
	public Date getFrom_datetime() {
		return from_datetime;
	}
	public void setFrom_datetime(Date from_datetime) {
		this.from_datetime = from_datetime;
	}
	public int getTo_id() {
		return to_id;
	}
	public void setTo_id(int to_id) {
		this.to_id = to_id;
	}
	public String getComments() {
		return comments;
	}
	public void setComments(String comments) {
		this.comments = comments;
	}
	
	
}
