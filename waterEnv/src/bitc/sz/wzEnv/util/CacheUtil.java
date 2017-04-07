package bitc.sz.wzEnv.util;


import net.sf.ehcache.Cache;
import net.sf.ehcache.CacheManager;
import net.sf.ehcache.Element;

/**
 * 缓存工具类
 * @author mayong 设计模式：单例模式 2015.7.21
 * @version 1.1 Gavin 重构部分方法 2015/7/27
 */

public class CacheUtil {
	private static final String path = "/ehcache.xml";
	private static CacheManager manager;
	private CacheUtil(){}
	
	static{
		manager = CacheManager.create(CacheUtil.class.getClassLoader().getResourceAsStream(path));
		if (null == manager) {
			throw new IllegalArgumentException(
					"初始化缓存失败，请确认配置文件位置！");
		}
	}

	/**
	 * 向指定缓存中插入数据
	 * 
	 * @param cacheName	缓存名称
	 * @param key		键
	 * @param value		值
	 * @return 			是否成功，如果为false代表已存在
	 */
	public static boolean save(String cacheName, Object key, Object value) {
		Cache cache = manager.getCache(cacheName);
		if (cache.isElementInMemory(key)) {
			return false;
		} else {
			Element element = new Element(key, value);
			cache.put(element);
			return true;
		}

	}

	/**
	 * 增加或更新缓存
	 * @param cacheName	缓存名称s
	 * @param key 		键
	 * @param value		值
	 * @return			是否成功
	 */
	public static boolean saveOrUpdate(String cacheName, Object key, Object value) {
		Cache cache = manager.getCache(cacheName);
		Element element = new Element(key, value);
		cache.put(element);
		return true;
	}
	
	/**
	 * 查询指定缓存名称中对应的key值
	 * 
	 * @param cacheName	缓存名称
	 * @param key		键
	 * @return 			是否成功
	 */
	public static Object get(String cacheName, Object key) {
		Cache cache = manager.getCache(cacheName);
		if(cache.isElementInMemory(key)){
			return cache.get(key).getObjectValue();
		}else{
			return null;
		}
	}

	/**
	 * 获取指定缓存
	 * @param cacheName
	 * @return
	 */
	public static Cache getCache(String cacheName) {
		return manager.getCache(cacheName);
	}

	/**
	 * 移除指定缓存的指定值
	 * @param cacheName
	 * @param key
	 */
	public static boolean delete(String cacheName, Object key) {
		Cache cache = manager.getCache(cacheName);
		return cache.remove(key);
	}

	/**
	 * 清空缓存
	 */
	public static void clear(){
		manager.clearAll();
	}
}
