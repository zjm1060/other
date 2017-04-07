package bitc.sz.wzEnv.util;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.OutputStream;
import java.sql.Blob;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

/**
 * Created by zsz on 2016/5/9.
 */
public class DBUtil {

    public static boolean updateBlob(File f,Connection connection,String tableName,String idName,String id,String blobName) throws Exception {
        PreparedStatement ps = null;
        try {
            if(connection == null){
                throw new Exception("Connection is null ！");
            }

            //update SDLM_ATT_计算结果文件 set 附件_obj = ? where step_id = ?
            ps=connection.prepareStatement("select  "+blobName+"  from "+ tableName+"  where  "+idName+" = ? for update");
            ps.setObject(1,id);
            ResultSet rs=ps.executeQuery();
            OutputStream out = null;
            while (rs.next()){
                Blob blob=rs.getBlob(1);
                out=blob.setBinaryStream(0);
                IOUtils.copy(new FileInputStream(f), out);
                out.flush();
                out.close();
            };

            rs.close();
        }catch (Exception e){
            e.printStackTrace();
            throw e;
        }
        return true;
    }
}
