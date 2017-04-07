package com.bitc.jk.xmis.model;

import java.io.Serializable;

import com.bitc.jk.xmis.util.JSONUtils;

public class MeetingRoomInformation implements Serializable {
	private static final long serialVersionUID = 949846564180205754L;
	/**
	 * 会议室ID
	 * */
	private String room_id;
	/**
	 * 会议室编号
	 * */
	private String room_No;
	/**
	 * 会议室名称
	 * */
	private String room_name;
	/**
	 * 楼层
	 * */
	private String floor;
	/**
	 * 地址
	 * */
	private String address;
	/**
	 * 备注
	 * */
	private String remark;
	/**
	 * 最多可容纳人数
	 * */
	private int number_people;

	
	
	public String getRoom_id() {
		return room_id;
	}
	public void setRoom_id(String room_id) {
		this.room_id = room_id;
	}
	public String getRoom_No() {
		return room_No;
	}
	public void setRoom_No(String room_No) {
		this.room_No = room_No;
	}
	public String getRoom_name() {
		return room_name;
	}
	public void setRoom_name(String room_name) {
		this.room_name = room_name;
	}
	public String getFloor() {
		return floor;
	}
	public void setFloor(String floor) {
		this.floor = floor;
	}
	public String getAddress() {
		return address;
	}
	public void setAddress(String address) {
		this.address = address;
	}
	public String getRemark() {
		return remark;
	}
	public void setRemark(String remark) {
		this.remark = remark;
	}
	public int getNumber_people() {
		return number_people;
	}
	public void setNumber_people(int number_people) {
		this.number_people = number_people;
	}
	public static long getSerialversionuid() {
		return serialVersionUID;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
	
}
