package com.bitc.jk.xmis.service;

import java.util.List;

import com.bitc.jk.xmis.model.security.NodeInfo;
import com.bitc.jk.xmis.web.action.param.SecurityTreeParam;

public interface SecurityService {
	public String getSecurityTree(SecurityTreeParam param);
	public String getBizObjById(String node);
	public String getAllStatusList();
	public String getOtherBizModuleList(int id);
	
	
	public void doSaveNodesMsg(String[] nodes);
	
	public void deleteOperation(String nodeId);
	public List getSecurityInfoList();
	public NodeInfo getSecurityNodes();
}
