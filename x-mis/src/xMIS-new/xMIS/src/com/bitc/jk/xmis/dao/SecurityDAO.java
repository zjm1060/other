/**
 * 
 */
package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.security.BizFunctionObj;
import com.bitc.jk.xmis.model.security.BizMappingObj;
import com.bitc.jk.xmis.model.security.BizModuleObj;
import com.bitc.jk.xmis.model.security.BizSubModuleObj;
import com.bitc.jk.xmis.web.action.param.SecurityTreeParam;

/**
 * @author Ted Li
 *
 * 2011-5-31
 */
public interface SecurityDAO {

	
	public List getSecurityTree(SecurityTreeParam param);
	
	//通过模块ID，查找模块
	public BizModuleObj getBizModuleById(int id);
	
	//通过模块ID，查找子模块
	public List getBizSubModuleByModuleId(int id);
	
	//查询所有状态列表
	public List getAllStatusList();
	
	//查询除当前模块的子模块外所有子模块
	public List getOtherBizModuleList(int id);
	
	
	//保存模块
	public int doSaveModule(BizModuleObj obj);
	
	//保存子模块
	public int doSaveSubModule(BizSubModuleObj obj);
	
	//保存业务功能
	public int doSaveFunction(BizFunctionObj obj);
	
	//删除模块
	public void doDeleteModule(BizModuleObj obj);
	
	//删除子模块
	public void doDeleteSubModule(BizSubModuleObj obj);
	
	//删除功能
	public void doDeleteFunction(BizFunctionObj obj);
	
	//保存子模块功能对应关系
	public void doSaveSubModuleFuncMappping(BizMappingObj obj);
	
	//删除子模块功能对应关系
	public void doDeleteSubModuleFuncMappping(int bizFuncID);
	
	//删除子模块功能对应关系
	public void doDeleteSubModuleFuncMappping(BizMappingObj obj);
	
	//删除模块子模块对应关系
	public void doDeleteModuleSubModuleMappping(BizMappingObj obj);
	
	//修改子模块功能对应关系
	public void doUpdateSubModuleFuncMappping(BizMappingObj oldObj,BizMappingObj newObj);
	
	//修改模块名称
	public void doUpdateModuleName(BizModuleObj obj);
	
	//修改子模块名称
	public void doUpdateSubModuleName(BizSubModuleObj obj);
	
	//修改功能名称
	public void doUpdateFuncName(BizFunctionObj obj);
	
	//通过bizfunctionID 查询功能ID
	public int getFuncIdByBizFunctionID(int bizFunctionID);
	
	//保存模块子模块对应关系
	public void doSaveModuleSubModuleMappping(BizMappingObj obj);
	
	//修改模块子模块对应关系
	public void doUpdateModuleSubModuleMappping(BizMappingObj oldObj,BizMappingObj newObj);
	
	public List getFuncCountsByBizFunctionID(int bizFunctionID);

	public List getSecurityInfoList();
	
}
