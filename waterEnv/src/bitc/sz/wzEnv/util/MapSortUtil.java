package bitc.sz.wzEnv.util;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/4/7.
 */
public class MapSortUtil {
    //降序排列
    public static List<Map<Object,Object>> desc (List<Map<Object,Object>> list, final String key, final String type){
        Collections.sort(list, new Comparator<Map<Object, Object>>() {
            public int compare(Map<Object, Object> o1, Map<Object, Object> o2) {
//                int map1value = (Integer) o1.get(key);
//                int map2value = (Integer) o2.get(key);
                int result = 0;
                if("double".equals(type)){
                    if((double)o1.get(key)>(double)o2.get(key)){
                        result = -1;
                    }else if((double)o1.get(key)==(double)o2.get(key)){
                        result = 0;
                    }else{
                        result = 1;
                    }
                }else if("int".equals(type)){
                    if((int)o1.get(key)>(int)o2.get(key)){
                        result =  -1;
                    }else if((int)o1.get(key)==(int)o2.get(key)){
                        result = 0;
                    }else{
                        result = 1;
                    }
                }
                return result;
//                return map2value - map1value;
            }
        });
        return list;
    }

    public static List<Map<String,Object>> asc2 (List<Map<String,Object>> list, final String key, final String type){
        Collections.sort(list, new Comparator<Map<String, Object>>() {
            public int compare(Map<String, Object> o1, Map<String, Object> o2) {
//                int map1value = (Integer) o1.get(key);
//                int map2value = (Integer) o2.get(key);
                int result = 0;
                if("double".equals(type)){
                    if((double)o1.get(key)>(double)o2.get(key)){
                        result = 1;
                    }else if((double)o1.get(key)==(double)o2.get(key)){
                        result = 0;
                    }else{
                        result = -1;
                    }
                }else if("int".equals(type)){
                    if((int)o1.get(key)>(int)o2.get(key)){
                        result =  1;
                    }else if((int)o1.get(key)==(int)o2.get(key)){
                        result = 0;
                    }else{
                        result = -1;
                    }
                }
                return result;
//                return map2value - map1value;
            }
        });
        return list;
    }

    //升序排列
    public static List<Map<Object,Object>> asc (List<Map<Object,Object>> list, final String key){
        Collections.sort(list, new Comparator<Map<Object, Object>>() {
            public int compare(Map<Object, Object> o1, Map<Object, Object> o2) {
                int map1value = (Integer) o1.get(key);
                int map2value = (Integer) o2.get(key);
                return map1value - map2value;
            }
        });
        return list;
    }
}
