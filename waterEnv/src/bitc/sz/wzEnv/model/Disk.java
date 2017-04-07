package bitc.sz.wzEnv.model;

import org.springframework.stereotype.Component;


public class Disk {
	private String diskName;
	private String free_disk;
	private String use_disk;
	public String getDiskName() {
		return diskName;
	}
	public void setDiskName(String diskName) {
		this.diskName = diskName;
	}
	public String getFree_disk() {
		return free_disk;
	}
	public void setFree_disk(String free_disk) {
		this.free_disk = free_disk;
	}
	public String getUse_disk() {
		return use_disk;
	}
	public void setUse_disk(String use_disk) {
		this.use_disk = use_disk;
	}
}
