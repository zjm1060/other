package bitc.sz.wzEnv.service;

import bitc.sz.wzEnv.model.Cpu;
import bitc.sz.wzEnv.model.Memory;
import bitc.sz.wzEnv.query.PagingInfo;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;


public interface DataService {
	public Cpu getCpuInfo() ;
	public Memory getPhysicalMemory() ;
	public List getDiskData();

	Map<String,Object> queryUserMessage(String keyword, PagingInfo info) throws SQLException;

	List<String> getUserRelatedWaterBody(String userId) throws SQLException;

	void saveUserRelatedWaterBody(String userId,String waterBodyIds) throws SQLException;

	void deleteUserRelatedWaterBody(String userId) throws SQLException;
}
