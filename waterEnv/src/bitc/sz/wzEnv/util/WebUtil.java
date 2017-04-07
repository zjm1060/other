package bitc.sz.wzEnv.util;

import bitc.sz.wzEnv.query.PagingInfo;
import sun.plugin.util.UIUtil;

import java.util.Enumeration;
import java.util.UUID;

import javax.servlet.http.HttpServletRequest;

public class WebUtil {

	public static String getContextPath(HttpServletRequest request) {
		String path = request.getContextPath();
		if ("/".equals(path)) {
			path = "";
		}
		return path;
	}

	public static boolean isAjaxRequest(HttpServletRequest request) {
		Enumeration<String> e = request.getHeaderNames();
		while (e.hasMoreElements()) {
			if (e.nextElement().equalsIgnoreCase("x-requested-with")) {
				return true;
			}
		}
		return false;
	}

	public static PagingInfo generatePageInfo(HttpServletRequest request) {
		int start = new Integer(
				request.getParameter("start") != null ? request
						.getParameter("start") : "0").intValue();
		int limit = new Integer(
				request.getParameter("limit") != null ? request
						.getParameter("limit") : "0").intValue();
		String sort = request.getParameter("sort");
		String dir = request.getParameter("dir");
		PagingInfo pageInfo = new PagingInfo();
		pageInfo.setStart(start);
		pageInfo.setLimit(limit);
		pageInfo.setSort(sort);
		pageInfo.setOrder(dir);
		return pageInfo;
	}
	public static void main(String[] args){
		System.out.println(UUID.randomUUID());
	}


}
