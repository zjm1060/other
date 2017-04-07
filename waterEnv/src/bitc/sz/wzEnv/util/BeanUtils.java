package bitc.sz.wzEnv.util;

import java.beans.PropertyDescriptor;
import java.lang.reflect.InvocationTargetException;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.commons.beanutils.BeanUtilsBean;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.DynaProperty;
import org.apache.log4j.Logger;

public class BeanUtils {
	private static Logger log = Logger.getLogger(BeanUtils.class);

	/**
	 * 拷贝一个bean中的非空属性于另一个bean中
	 * 
	 * @param dest
	 * @param orig
	 * @throws IllegalAccessException
	 * @throws InvocationTargetException
	 */
	@SuppressWarnings("rawtypes")
	public static void copyNotNullProperties(Object dest, Object orig) {
		BeanUtilsBean beanUtils = BeanUtilsBean.getInstance();
		// Validate existence of the specified beans
		if (dest == null) {
			throw new RuntimeException("No destination bean specified");
		}
		if (orig == null) {
			throw new RuntimeException("No origin bean specified");
		}
		if (log.isDebugEnabled()) {
			log.debug("BeanUtils.copyProperties(" + dest + ", " + orig + ")");
		}

		try {
			// Copy the properties, converting as necessary
			if (orig instanceof DynaBean) {
				DynaProperty[] origDescriptors = ((DynaBean) orig)
						.getDynaClass().getDynaProperties();
				for (int i = 0; i < origDescriptors.length; i++) {
					String name = origDescriptors[i].getName();
					// Need to check isReadable() for WrapDynaBean
					// (see Jira issue# BEANUTILS-61)
					if (beanUtils.getPropertyUtils().isReadable(orig, name)
							&& beanUtils.getPropertyUtils().isWriteable(dest,
									name)) {
						Object value = ((DynaBean) orig).get(name);
						beanUtils.copyProperty(dest, name, value);
					}
				}
			} else if (orig instanceof Map) {
				Iterator entries = ((Map) orig).entrySet().iterator();
				while (entries.hasNext()) {
					Map.Entry entry = (Map.Entry) entries.next();
					String name = (String) entry.getKey();
					if (beanUtils.getPropertyUtils().isWriteable(dest, name)) {
						beanUtils.copyProperty(dest, name, entry.getValue());
					}
				}
			} else /* if (orig is a standard JavaBean) */{
				PropertyDescriptor[] origDescriptors = beanUtils
						.getPropertyUtils().getPropertyDescriptors(orig);
				for (int i = 0; i < origDescriptors.length; i++) {
					String name = origDescriptors[i].getName();
					if ("class".equals(name)) {
						continue; // No point in trying to set an object's class
					}
					if (beanUtils.getPropertyUtils().isReadable(orig, name)
							&& beanUtils.getPropertyUtils().isWriteable(dest,
									name)) {
						try {
							Object value = beanUtils.getPropertyUtils()
									.getSimpleProperty(orig, name);
							if (value == null) {
								continue;
							}
							if (value instanceof List
									&& ((List) value).size() == 0) {
								continue;
							}
							if (value != null) {
								beanUtils.copyProperty(dest, name, value);
							}
						} catch (NoSuchMethodException e) {
							// Should not happen
						}
					}
				}
			}
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}

}
