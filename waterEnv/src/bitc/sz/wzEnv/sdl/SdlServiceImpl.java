package bitc.sz.wzEnv.sdl;

import bitc.sz.wzEnv.util.Md5Util;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * Created by Administrator on 2016/5/19.
 */
@Service
public class SdlServiceImpl implements SdlService {
    @Autowired
    private DataSource basicDataSource;

    public DataSource getBasicDataSource() {
        return basicDataSource;
    }

    public void setBasicDataSource(DataSource basicDataSource) {
        this.basicDataSource = basicDataSource;
    }
    @Override
    public void insertdmpm(String objid, String x, String y, String distance, String elevation, String sign, String remark) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            int snid = 0;
            connection = basicDataSource.getConnection();
            String sql1 = "select sn_id from sdlm_sys_断面剖面数据表 order by sn_id desc";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            if(rs.next()){
                snid = rs.getInt(1)+1;
            }
            String sql2 = "insert into sdlm_sys_断面剖面数据表 (OBJ_ID,SN_ID,经度,纬度,距离,高程,左右标记,备注) values(?,?,?,?,?,?,?,?)";
            ps = connection.prepareStatement(sql2);
            ps.setString(1,objid);
            ps.setInt(2, snid);
            ps.setString(3, x);
            ps.setString(4, y);
            ps.setString(5, distance);
            ps.setString(6, elevation);
            ps.setString(7, sign);
            ps.setString(8, remark);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs!=null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
    }

    @Override
    public void updatedmpm(String objid, String x, String y, String distance, String elevation, String sign, String remark, String snid) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "update sdlm_sys_断面剖面数据表 set 经度 = ?,纬度 = ?,距离 = ?,高程 = ?,左右标记 = ?,备注 = ? where OBJ_ID = ? and SN_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,x);
            ps.setString(2,y);
            ps.setString(3, distance);
            ps.setString(4, elevation);
            ps.setString(5, sign);
            ps.setString(6, remark);
            ps.setString(7,objid);
            ps.setString(8,snid);
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

    @Override
    public void deletedmpm(String snid, String objid) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "delete from sdlm_sys_断面剖面数据表 where OBJ_ID = ? and SN_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,objid);
            ps.setString(2,snid);
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

    @Override
    public void deleteDm(String objid) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "delete from sdlm_sys_断面剖面数据表 where OBJ_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,objid);
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

    @Override
    public void insertdmc(String jisuanid, String hdName, String dmNumber, String cNumber, String cGao, String cWidth, String cArea, String cRadius) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        String uuid = Md5Util.getUUID();
        ResultSet rs = null;
        int num = 0;
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select 层号 from sdlm_sys_断面分层水力参数 where 计算目标_ID = ? and 河段名称 = ? and 计算断面号 = ? order by 层号 desc";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,jisuanid);
            ps.setString(2,hdName);
            ps.setInt(3, Integer.valueOf(dmNumber));
            rs = ps.executeQuery();
            if(rs.next()){
                num = rs.getInt(1)+1;
            }
            String sql = "insert into sdlm_sys_断面分层水力参数 (SN_ID,计算目标_ID,河段名称,计算断面号,层号,层高程,层河宽,层面积,层水力半径) values(?,?,?,?,?,?,?,?,?)";
            ps = connection.prepareStatement(sql);
            ps.setString(1,uuid);
            ps.setString(2,jisuanid);
            ps.setString(3, hdName);
            ps.setInt(4, Integer.valueOf(dmNumber));
            ps.setInt(5, num);
            ps.setString(6, cGao);
            ps.setString(7, cWidth);
            ps.setString(8, cArea);
            ps.setString(9, cRadius);
            ps.execute();
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
    }

    @Override
    public void updatedmc(String snid,String cNumber, String cGao, String cWidth, String cArea, String cRadius) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "update sdlm_sys_断面分层水力参数 set 层高程 = ?,层河宽 = ?,层面积 = ?,层水力半径 = ? where SN_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1, cGao);
            ps.setString(2, cWidth);
            ps.setString(3, cArea);
            ps.setString(4,cRadius);
            ps.setString(5, snid);
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

    @Override
    public void deletedmc(String snid) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "delete from sdlm_sys_断面分层水力参数 where SN_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,snid);
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

    @Override
    public void deleteTotalDmc(String jisuanid, String hdName, String dmNumber) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "delete from sdlm_sys_断面分层水力参数 where 计算目标_ID = ? and 河段名称 = ? and 计算断面号 = ? and 层号 > 0";
            ps = connection.prepareStatement(sql);
            ps.setString(1,jisuanid);
            ps.setString(2,hdName);
            ps.setInt(3, Integer.valueOf(dmNumber));
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

    @Override
    public void updateTotaldmc(String jisuanid, String hdName, String dmNumber, String newjisuanid, String newHdName, String newDmNumber) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "update sdlm_sys_断面分层水力参数 set 计算目标_ID = ?,河段名称 = ?,计算断面号 = ? where 计算目标_ID = ? and 河段名称 = ? and 计算断面号 = ? and 层号 > 0";
            ps = connection.prepareStatement(sql);
            ps.setString(1,newjisuanid);
            ps.setString(2,newHdName);
            ps.setInt(3, Integer.valueOf(newDmNumber));
            ps.setString(4, jisuanid);
            ps.setString(5,hdName);
            ps.setInt(6, Integer.valueOf(dmNumber));
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
