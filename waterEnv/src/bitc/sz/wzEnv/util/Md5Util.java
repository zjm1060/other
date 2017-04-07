package bitc.sz.wzEnv.util;

import org.apache.shiro.crypto.hash.Md5Hash;

import java.util.UUID;

/**
 * Created by zsz on 2015/10/14.
 */
public class Md5Util {

    public static String getMd5CodeWithSalt(String str,String salt){
        String code = new Md5Hash(str,salt).toString();
        return code;
    }
    public static String getMd5Code(String str){
        String code = new Md5Hash(str).toString();
        return code;
    }
    public static String getSalt(){
        return  UUID.randomUUID().toString().replace("-","");
    }
    public static String getUUID(){
        return getSalt();
    }
    public static String getPasswordMd5Code(String password,String salt){
        return getMd5CodeWithSalt(password,salt);
    }
    public static void main(String agrs []){
        System.out.print(UUID.randomUUID());
    }
//    0c8852bde4f24b98bfa749f503b7b900
//    18fdf5f2e428c88a0fac8697de727824
    //200ceb26807d6bf99fd6f4f0d1ca54d4
    //200ceb26807d6bf99fd6f4f0d1ca54d4
}
