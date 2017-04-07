/**
 * 
 */
package com.bitc.jk.xmis.service.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import net.sf.json.JSONArray;
import net.sf.json.JsonConfig;
import net.sf.json.util.CycleDetectionStrategy;

import com.bitc.jk.xmis.dao.SecurityDAO;
import com.bitc.jk.xmis.model.Node;
import com.bitc.jk.xmis.model.security.BizFunctionObj;
import com.bitc.jk.xmis.model.security.BizMappingObj;
import com.bitc.jk.xmis.model.security.BizModuleObj;
import com.bitc.jk.xmis.model.security.BizSubModuleObj;
import com.bitc.jk.xmis.model.security.NodeInfo;
import com.bitc.jk.xmis.service.SecurityService;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.action.param.SecurityTreeParam;
import com.bitc.jk.xmis.web.response.TreeLoaderResponse;

/**
 * @author Ted Li
 * 
 *         2011-5-31
 */
public class SecurityServiceImpl implements SecurityService {

	private SecurityDAO securityDao;

	public SecurityDAO getSecurityDao() {
		return securityDao;
	}

	public void setSecurityDao(SecurityDAO securityDao) {
		this.securityDao = securityDao;
	}

	public String getSecurityTree(SecurityTreeParam param) {
		TreeLoaderResponse loaderResponse = new TreeLoaderResponse();
		List<Map> list = securityDao.getSecurityTree(param);
		for (Iterator<Map> it = list.iterator(); it.hasNext();) {
			Map objs = it.next();
			String id = objs.get("level").toString() + "-1-"
					+ objs.get("ID").toString();

			String name = objs.get("name").toString();
			boolean leaf = false;
			if (param.getLeafLevel() == Integer.parseInt(objs.get("level")
					.toString())) {
				leaf = true;
			} else {
				if (Integer.parseInt(objs.get("subNum").toString()) == 0) {
					leaf = true;
				}
			}
			loaderResponse.addTreeNode(name, id, leaf, "file", "");
		}
		return loaderResponse.toJsonString(param.getJsonConfig());
	}

	public String getBizObjById(String node) {
		String[] args = node.split("-");
		String store = "{}";
		switch (Integer.parseInt(args[0])) {
		case 2:
			BizModuleObj obj = securityDao.getBizModuleById(Type
					.GetInt(args[2]));
			store = obj.toJsonString();
			break;
		case 3:

			break;
		}
		// BizModuleObj b = (BizModuleObj) obj;
		// JsonConfig config = new JsonConfig();
		// config.setIgnoreDefaultExcludes(false);
		// config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return store;// JSONObject.fromObject(obj,config).toString();
	}

	public String getAllStatusList() {
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(securityDao.getAllStatusList(), config)
				.toString();
	}

	public String getOtherBizModuleList(int id) {
		List list = securityDao.getOtherBizModuleList(id);
		return WebUtil.buildJsonStoreByList(list);
	}

	/**
	 * 1.判断是否为新数据，新数据则直接新建
	 */
	public void doSaveNodesMsg(String[] nodesMsgs) {
		List<Node> nodes = this.getNodes(nodesMsgs);
		Map<String, Integer> newDataIdMappingMap = new HashMap<String, Integer>();
		final int MODULE = 2;
		final int SUBMODULE = 3;
		final int FUNC = 4;

		for (Node node : nodes) { // 此次遍历目的：增加和修改数据
			if (!node.isOld) { // 新数据
				// 新建数据
				if (MODULE == node.level) {
					int newId = securityDao.doSaveModule(new BizModuleObj(
							node.text));
					newDataIdMappingMap.put(node.id, newId);
					node.id = newId + "";
				}
				if (SUBMODULE == node.level) {
					int newId = securityDao
							.doSaveSubModule(new BizSubModuleObj(node.text));
					newDataIdMappingMap.put(node.id, newId);
					node.id = newId + "";
				}
				if (FUNC == node.level) {
					int newId = securityDao.doSaveFunction(new BizFunctionObj(
							node.text));
					newDataIdMappingMap.put(node.id, newId);
					node.id = newId + "";
				}
			} else { // 已存在数据
				if (node.isChangeName) { // 改名
					// 修改已有数据名称
					int nodeId = Type.GetInt(node.id);
					if (MODULE == node.level) {
						securityDao.doUpdateModuleName(new BizModuleObj(nodeId,
								node.text));
					}
					if (SUBMODULE == node.level) {
						securityDao.doUpdateSubModuleName(new BizSubModuleObj(
								nodeId, node.text));
					}
					if (FUNC == node.level) {
						securityDao.doUpdateFuncName(new BizFunctionObj(nodeId,
								node.text));
					}
				}
			}
		}

		// 此次循环修改关系
		for (Node node : nodes) {
			if (node.isOld) { // 已有数据修改关系
				if (node.isDrag) {
					int nodeId = Type.GetInt(node.id);
					String[] args = node.newParentId.split("-");
					int newParentId;
					int oldParentId = Type
							.GetInt(node.oldParentId.split("-")[2]);
					if ("0".equals(args[1])) {
						newParentId = newDataIdMappingMap.get(node.newParentId
								.split("-")[2]);
					} else {
						newParentId = Type
								.GetInt(node.newParentId.split("-")[2]);
					}
					if (SUBMODULE == node.level) {
						securityDao.doUpdateModuleSubModuleMappping(
								new BizMappingObj(oldParentId, nodeId),
								new BizMappingObj(newParentId, nodeId));
					}
					if (FUNC == node.level) {
						int bizfuncId = securityDao
								.getFuncIdByBizFunctionID(nodeId);
						securityDao.doUpdateSubModuleFuncMappping(
								new BizMappingObj(oldParentId, bizfuncId),
								new BizMappingObj(newParentId, bizfuncId));
					}
				}
			} else { // 新数据增加关系
				int nodeId = Type.GetInt(node.id);
				String[] args = node.newParentId.split("-");
				int newParentId;
				if ("0".equals(args[1])) {
					newParentId = newDataIdMappingMap.get(node.newParentId
							.split("-")[2]);
				} else {
					newParentId = Type.GetInt(node.newParentId.split("-")[2]);
				}
				if (SUBMODULE == node.level) {
					securityDao
							.doSaveModuleSubModuleMappping(new BizMappingObj(
									newParentId, nodeId));
				}
				if (FUNC == node.level) {
					int funcId = securityDao.getFuncIdByBizFunctionID(nodeId);
					securityDao.doSaveSubModuleFuncMappping(new BizMappingObj(
							newParentId, funcId));
				}
			}
		}

		for (Node n : nodes) {
			System.out.println(n.level);
			System.out.println(n.isOld);
			System.out.println(n.id);
			System.out.println(n.text);
			System.out.println(n.newParentId);
			System.out.println(n.oldParentId);
			System.out.println(n.isChangeName);
			System.out.println(n.isDrag);
			System.out.println("------------------");
		}
		for (String s : newDataIdMappingMap.keySet()) {
			System.out.println(s + ":" + newDataIdMappingMap.get(s));
		}

	}

	private List<Node> getNodes(String[] nodesMsgs) {
		List<Node> list = null;
		if (nodesMsgs != null && nodesMsgs.length > 0) {
			list = new ArrayList<Node>();
			for (int i = 0; i < nodesMsgs.length; i++) {
				String[] nodeMsg = nodesMsgs[i].split(":");
				Node node = new Node();
				String[] idMsg = nodeMsg[0].split("-");
				node.level = Type.GetInt(idMsg[0]);
				node.isOld = "1".equals(idMsg[1]) ? true : false;
				node.id = idMsg[2];
				node.text = nodeMsg[1];
				node.newParentId = nodeMsg[2];
				node.oldParentId = nodeMsg[3];
				node.isChangeName = "true".equals(nodeMsg[4]) ? true : false;
				node.isDrag = "true".equals(nodeMsg[5]) ? true : false;
				list.add(node);
			}
		}
		return list;
	}

	public void deleteOperation(String nodeId) {
		// TODO Auto-generated method stub
		String[] args = nodeId.split("-");
		int id = Type.GetInt(args[2]);
		if ("2".equals(args[0])) {
			securityDao.doDeleteModule(new BizModuleObj(id));
		}
		if ("3".equals(args[0])) {
			BizMappingObj mapping = new BizMappingObj();
			mapping.setSubID(id);
			securityDao.doDeleteSubModule(new BizSubModuleObj(id));
			securityDao.doDeleteModuleSubModuleMappping(mapping);
		}
		if ("4".equals(args[0])) {
			// BizFunctionObj func = new BizFunctionObj(id);
			List list = securityDao.getFuncCountsByBizFunctionID(id);
			if (list != null && list.size() > 0) {
				Map map = (Map) list.get(0);
				int bizFuncID = Type.GetInt(map.get("业务功能ID"));
				int counts = Type.GetInt(map.get("counts"));
				securityDao.doDeleteFunction(new BizFunctionObj(id));
				if (counts == 1) { // 删除子模块功能对应表
					securityDao.doDeleteSubModuleFuncMappping(bizFuncID);
				}
			}

		}
	}

	private class Node {
		public int level;
		public boolean isOld; // true:已有数据
		public String id;
		public String text;
		public String newParentId;
		public String oldParentId;
		public boolean isChangeName;
		public boolean isDrag;
	}

	/**
	 * 得到一个数组，保存角色信息的list和map
	 */
	public List getSecurityInfoList() {
		List list = securityDao.getSecurityInfoList();
		return list;
	}

	public NodeInfo getSecurityNodes(){
		List list = securityDao.getSecurityInfoList();
		NodeInfo root = new NodeInfo();
		for (Iterator it = list.iterator(); it.hasNext();) {
			NodeInfo[] nodes = (NodeInfo[]) it.next();
			root.addChild(nodes[0]).addChild(nodes[1]).addChild(nodes[2]);
		}
		return root;
	}
}
