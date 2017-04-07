package bitc.sz.wzEnv.service.impl;

import bitc.sz.wzEnv.model.Cpu;
import bitc.sz.wzEnv.model.Disk;
import bitc.sz.wzEnv.model.Memory;
import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.service.DataService;
import com.sun.xml.xsom.impl.scd.Iterators;
import org.apache.commons.collections.map.HashedMap;
import org.apache.commons.lang.StringUtils;
import org.hyperic.sigar.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.DecimalFormat;
import java.util.*;


@Service
public class DataServiceImpl implements DataService {

	@Autowired
	private DataSource basicDataSource;

	public Cpu getCpuInfo() {
		Sigar sigar = new Sigar();
		CpuPerc[] cpuList = null;
		Cpu cpu = new Cpu();
		float combine_sum = 0;
		float combine_format = 0;
		float user_sum = 0;
		float user_format = 0;
		float sys_sum = 0;
		float sys_format = 0;
		try {
			cpuList = sigar.getCpuPercList();
			for (int i = 0; i < cpuList.length; i++) {
				double weight = 100/cpuList.length;
				combine_sum += cpuList[i].getCombined()*weight;
				user_sum += cpuList[i].getUser()*weight;
				sys_sum += cpuList[i].getSys()*weight;
			}
			combine_format = combine_sum/100;
			user_format = user_sum/100;
			sys_format = sys_sum/100;
			
			//cpu当前总的使用率
			cpu.setCombine(CpuPerc.format(combine_format));
			cpu.setUser(CpuPerc.format(user_format));
			cpu.setSys(CpuPerc.format(sys_format));
		} catch (SigarException e) {
			e.printStackTrace();
		}
		return cpu;
	}
	public Memory getPhysicalMemory() {
		// a)物理内存信息
		DecimalFormat df = new DecimalFormat("#0.00");
		Sigar sigar = new Sigar();
		Memory memory = new Memory();
		Mem mem;
		try {
			mem = sigar.getMem();
			// 内存总量
			memory.setTotal_mem(df.format((float) mem.getTotal() / 1024 / 1024 / 1024)
			);
			// 当前内存使用量
			memory.setUse_mem(df.format((float) mem.getUsed() / 1024 / 1024 / 1024)
			);
			// 当前内存剩余量
			memory.setFree_mem(df.format((float) mem.getFree() / 1024 / 1024 / 1024)
			);
			
		} catch (SigarException e) {
			e.printStackTrace();
		}
		return memory;
	}
	
	public List getDiskData()  {
		Sigar sigar = new Sigar();
		List<Disk> array = new ArrayList<Disk>();
		
		FileSystem fslist[] = null;
		try {
			fslist = sigar.getFileSystemList();
		} catch (SigarException e1) {
			e1.printStackTrace();
		}
		DecimalFormat df = new DecimalFormat("#0.00");
		for (int i = 0; i < fslist.length; i++) {
			Disk disk = new Disk();
			FileSystem fs = fslist[i];
			System.out.println(fs.getType()+":"+fs.getTypeName());
			
			FileSystemUsage usage = null;
			try {
				if (fs.getType() == 2){
					usage = sigar.getFileSystemUsage(fs.getDirName());
					//文件系统的盘符
					disk.setDiskName(fs.getDevName().substring(0, 1));
					// 文件系统剩余大小
					disk.setFree_disk(df.format((float) usage.getFree() / 1024 / 1024));
					// 文件系统已经使用量
					disk.setUse_disk(df.format((float) usage.getUsed() / 1024 / 1024));
					array.add(disk);
				}
			} catch (SigarException e) {
				e.printStackTrace();
			}
			
		}
		return array;
	}

	@Override
	public Map<String, Object> queryUserMessage(String keyword, PagingInfo info) throws SQLException {
		Connection connection = null;
		PreparedStatement ps1 = null;
		ResultSet rs1 = null;
		PreparedStatement ps2 = null;
		ResultSet rs2 = null;
		//存放结果
		Map<String,Object> resultMap = new HashMap<String,Object>();

		try {
			connection = basicDataSource.getConnection();
			String sql1 = "select * from syz_sys_用户表";
			String sql2 = "select t2.obj_name\n" +
					"  from syz_rel_用户水体关系 t1, V_用户对应水体 t2\n" +
					" where t1.obj_id = t2.obj_id\n" +
					"   and t1.user_id = ?\n";
			ps1 = connection.prepareStatement(sql1);
			ps2 = connection.prepareStatement(sql2);
			rs1 = ps1.executeQuery();
			List<Map<String,String>> tempList = new ArrayList<Map<String,String>>();
			while (rs1.next()){
				Map<String,String> tempMap = new LinkedHashMap<String,String>();
				tempMap.put("userId",rs1.getString(1));
				tempMap.put("userName",rs1.getString(2));
				tempMap.put("password",rs1.getString(3));
				ps2.setString(1,rs1.getString(1));
				rs2 = ps2.executeQuery();
				StringBuilder waterBodyName = new StringBuilder();
				while (rs2.next()){
					waterBodyName.append(rs2.getString(1)+",");
				}
				tempMap.put("relateWaterBody",StringUtils.isEmpty(waterBodyName.toString())?"":waterBodyName.toString().substring(0,waterBodyName.toString().lastIndexOf(",")));
				tempList.add(tempMap);
			}
			List<Map<String,String>> queryList = new ArrayList<Map<String,String>>();
			if(StringUtils.isNotEmpty(keyword)){
				for(int i = 0 ; i < tempList.size() ; i++){
					Map<String,String> tempMap = tempList.get(i);
					if(tempMap.get("userName").contains(keyword)){
						queryList.add(tempMap);
					}
				}
			}else{
				queryList.addAll(tempList);
			}
			if(info.getStart() == 0 && info.getLimit() == 0){
				resultMap.put("size",queryList.size());
				resultMap.put("result",queryList);
				return resultMap;
			}
			List<Map<String,String>> subList = new ArrayList<Map<String,String>>();
			if(queryList.size()>info.getStart() && queryList.size()>info.getStart()+info.getLimit()){
				subList = queryList.subList(info.getStart(), info.getStart() + info.getLimit());
			}else if(queryList.size()>info.getStart() && queryList.size()<= info.getStart()+info.getLimit()){
				subList = queryList.subList(info.getStart(),queryList.size());
			}
			resultMap.put("size",queryList.size());
			resultMap.put("result",subList);

		} catch (SQLException e) {
			e.printStackTrace();
			throw e;
		}finally {
			if(rs1 != null){
				rs1.close();
			}
			if(rs2 != null){
				rs2.close();
			}
			if(ps1 != null){
				ps1.close();
			}
			if(ps2 != null){
				ps2.close();
			}
			if(connection != null){
				connection.close();
			}
		}
		return resultMap;
	}

	@Override
	public List<String> getUserRelatedWaterBody(String userId) throws SQLException {
		Connection connection = null;
		PreparedStatement ps = null;
		ResultSet rs = null;
		List<String> waterBodyIdList = new ArrayList<String>();
		try {
			connection = basicDataSource.getConnection();
			String sql = "select obj_id from syz_rel_用户水体关系 where user_id = ?";
			ps = connection.prepareStatement(sql);
			ps.setString(1,userId);
			rs = ps.executeQuery();
			while (rs.next()){
				waterBodyIdList.add(rs.getString(1));
			}
		} catch (SQLException e) {
			e.printStackTrace();
			throw e;
		}finally {
			if(rs != null){
				rs.close();
			}
			if(ps != null){
				ps.close();
			}
			if(connection != null){
				connection.close();
			}
		}
		return waterBodyIdList;
	}

	@Override
	public void saveUserRelatedWaterBody(String userId, String waterBodyIds) throws SQLException {
		Connection connection = null;
		PreparedStatement ps = null;
		try {
			connection = basicDataSource.getConnection();
			connection.setAutoCommit(false);
			String sql1 = "delete from syz_rel_用户水体关系 where user_id = ?";
			String sql2 = "insert into syz_rel_用户水体关系 (user_id,obj_id) values(?,?)";
			ps = connection.prepareStatement(sql1);
			ps.setString(1,userId);
			ps.execute();

			ps = connection.prepareStatement(sql2);
			String[] waterBodyId = waterBodyIds.split(",");
			for(int i = 0 ; i < waterBodyId.length ; i++){
				ps.setString(1,userId);
				ps.setString(2,waterBodyId[i]);
				ps.execute();
			}
			connection.commit();
		} catch (SQLException e) {
			e.printStackTrace();
			connection.rollback();
			throw e;
		}finally {
			if(ps != null){
				ps.close();
			}
			if(connection != null){
				connection.close();
			}
		}
	}

	@Override
	public void deleteUserRelatedWaterBody(String userId) throws SQLException {
		Connection connection = null;
		PreparedStatement ps = null;
		try {
			connection = basicDataSource.getConnection();
			String sql = "delete from syz_rel_用户水体关系 where user_id = ?";
			ps = connection.prepareStatement(sql);
			ps.setString(1,userId);
			ps.execute();
		} catch (SQLException e) {
			e.printStackTrace();
			throw e;
		}finally {
			if(ps != null){
				ps.close();
			}
			if(connection != null){
				connection.close();
			}
		}
	}
}
