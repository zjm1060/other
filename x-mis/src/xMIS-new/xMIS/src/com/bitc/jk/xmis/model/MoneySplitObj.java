package com.bitc.jk.xmis.model;

import java.sql.Date;

public class MoneySplitObj {
	
	public final int hardwarePurCost = 1;//硬件购置费
	public final int softwarePurCost = 2;//软件购置费
	public final int softwareSerCost = 3;//软件服务费
	public final int elseCost = 4;//其他费用
	public final int selfHard = 101;//自制硬件费
	public final int homemadeHard = 102;//国产硬件费
	public final int importHard = 103;//进口硬件费
	public final int homemadeSoft = 201;//国产软件费
	public final int importSoft = 202;//进口软件费
	public final int softwareDevCost = 301;//软件开发费用
	public final int systemDesCost = 302;//咨询设计费（系统设计费用）
	public final int systemIntCost = 303;//系统集成费用
	public final int technicalCost = 304;//运营服务费（技术服务费用）
	public final int constructCost = 401;//建筑工程费用
	public final int managementCost = 402;//管理费
	public final int transportCost = 403;//运费
	public final int taxes = 404;//税金
	
	private int id; 
	private int contractID; //合同ID
	private int splitTypeID;//分解类型ID
	private double money;
	private double percent;//百分比
	private String name;
	private int type;//类型（按模板、按合同文本）
	private String description;//描述
	public int lastModificatorID;
	public Date lastModificationDate;
	
	
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public int getContractID() {
		return contractID;
	}
	public void setContractID(int contractID) {
		this.contractID = contractID;
	}
	public int getSplitTypeID() {
		return splitTypeID;
	}
	public void setSplitTypeID(int splitTypeID) {
		this.splitTypeID = splitTypeID;
	}
	public double getMoney() {
		return money;
	}
	public void setMoney(double money) {
		this.money = money;
	}
	public double getPercent() {
		return percent;
	}
	public void setPercent(double percent) {
		this.percent = percent;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getType() {
		return type;
	}
	public void setType(int type) {
		this.type = type;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public int getLastModificatorID() {
		return lastModificatorID;
	}
	public void setLastModificatorID(int lastModificatorID) {
		this.lastModificatorID = lastModificatorID;
	}
	public Date getLastModificationDate() {
		return lastModificationDate;
	}
	public void setLastModificationDate(Date lastModificationDate) {
		this.lastModificationDate = lastModificationDate;
	}
	
}
