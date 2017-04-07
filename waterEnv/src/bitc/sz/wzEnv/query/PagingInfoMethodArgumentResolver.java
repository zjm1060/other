package bitc.sz.wzEnv.query;

import java.util.List;
import java.util.Map;

import bitc.sz.wzEnv.util.JsonUtil;
import org.codehaus.jackson.type.TypeReference;
import org.springframework.core.MethodParameter;
import org.springframework.stereotype.Component;
import org.springframework.web.bind.support.WebArgumentResolver;
import org.springframework.web.context.request.NativeWebRequest;

public class PagingInfoMethodArgumentResolver implements WebArgumentResolver {

	public Object resolveArgument(MethodParameter methodParameter,
			NativeWebRequest webRequest) throws Exception {
		if (PagingInfo.class.isAssignableFrom(methodParameter
				.getParameterType())) {
			PagingInfo info = new PagingInfo();
			String startStr = webRequest.getParameter("start");
			if (startStr != null) {
				info.setStart(Integer.parseInt(startStr));
			}
			String limitStr = webRequest.getParameter("limit");
			if (limitStr != null) {
				info.setLimit(Integer.parseInt(limitStr));
			}
			String sort = webRequest.getParameter("sort");
			if (sort != null) {
				List<Map<String, String>> list = JsonUtil.readValue(sort,
						new TypeReference<List<Map<String, String>>>() {
						});
				if (list != null && list.size() > 0) {
					Map<String, String> map = list.get(0);
					info.setSort(map.get("property"));
					info.setOrder(map.get("direction"));
				}
			}
			return info;
		} else {
			return WebArgumentResolver.UNRESOLVED;
		}
	}
}
