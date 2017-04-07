package bitc.sz.wzEnv.sdl;

import java.sql.SQLException;

/**
 * Created by Administrator on 2016/5/19.
 */
public interface SdlService {
    void insertdmpm(String objid,String x,String y,String distance,String elevation,String sign,String remark) throws SQLException;
    void updatedmpm(String objid,String x,String y,String distance,String elevation,String sign,String remark,String snid) throws SQLException;
    void deletedmpm(String snid,String objid) throws SQLException;
    void deleteDm(String objid) throws SQLException;
    void insertdmc(String jisuanid,String hdName,String dmNumber,String cNumber,String cGao,String cWidth,String cArea,String cRadius) throws SQLException;
    void updatedmc(String snid,String cNumber,String cGao,String cWidth,String cArea,String cRadius) throws SQLException;
    void deletedmc(String snid) throws SQLException;
    void deleteTotalDmc(String jisuanid,String hdName,String dmNumber) throws SQLException;
    void updateTotaldmc(String jisuanid,String hdName,String dmNumber,String newjisuanid,String newHdName,String newDmNumber) throws SQLException;
}
