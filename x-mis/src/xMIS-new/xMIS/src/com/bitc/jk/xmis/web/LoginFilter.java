package com.bitc.jk.xmis.web;

import java.io.IOException;
import java.net.URLEncoder;
import java.util.HashSet;
import java.util.Set;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.util.CryptUtil;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.response.LoginTimeoutResponse;

/**
 * Servlet Filter implementation class LoginFilter
 */
public class LoginFilter implements Filter {

	private static Logger logger = Logger.getLogger(LoginFilter.class);

	private ServletContext sc;

	Set<String> filterRes;

	/**
	 * Default constructor.
	 */
	public LoginFilter() {
	}

	/**
	 * @see Filter#destroy()
	 */
	public void destroy() {
		// NULL
	}

	/**
	 * @see Filter#doFilter(ServletRequest, ServletResponse, FilterChain)
	 */
	public void doFilter(ServletRequest request, ServletResponse response,
			FilterChain chain) throws IOException, ServletException {
		HttpServletRequest httpreq = (HttpServletRequest) request;
		if (!httpreq.isRequestedSessionIdValid()
				|| httpreq.getSession().getAttribute(
						WebKeys.Session_Attribute_Name_UserDNA) == null) {
			boolean foundx = false;
			String pagex = httpreq.getRequestURI();
			if (pagex != null) {
				for (String entryPage : filterRes) {
					if (pagex.toLowerCase().endsWith(entryPage.toLowerCase())) {
						foundx = true;
						break;
					}
				}
				if (!foundx) {
					EmployeeObj emp = getCookieUserState(httpreq);
					if (emp == null || emp.isLocked()) {
						String uri = pagex.substring(httpreq.getContextPath()
								.length());
						uri = URLEncoder.encode(uri, "utf-8");
						String queryString = httpreq.getQueryString();
						queryString = queryString == null ? "" : URLEncoder
								.encode(queryString, "utf-8");
						logger.info("URI = " + uri);
						logger.info("queryString = " + queryString);
						HttpServletResponse httpResponse = (HttpServletResponse) response;
						String param = WebKeys.RequestKey_Original_URI + "="
								+ uri + "&"
								+ WebKeys.RequestKey_Original_QueryString + "="
								+ queryString;
						String redirectUri = httpreq.getContextPath()
								+ "/login.jsp?" + param;
						if (httpreq.getHeader("x-requested-with") == null) {
							httpResponse.sendRedirect(redirectUri);
						} else {
							String refUrl = httpreq.getHeader("Referer");
							WebUtil.returnResponse(httpResponse,
									new LoginTimeoutResponse(refUrl)
											.toJsonString());
						}
						return;
					} else {
						WebUtil.setCurrentEmployee(emp);
					}
				}
			}
		} else {
			WebUtil.setCurrentEmployee((EmployeeObj) httpreq.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA));
		}
		chain.doFilter(request, response);
	}

	private EmployeeObj getCookieUserState(HttpServletRequest request) {
		Cookie[] cookies = request.getCookies();
		if (cookies == null || cookies.length == 0) {
			return null;
		}
		for (Cookie cookie : cookies) {
			if (cookie.getName().equals(WebKeys.Session_Attribute_Name_UserDNA)) {
				String value = cookie.getValue();
				try {
					String state = CryptUtil.decryptBase64(value);
					String[] ss = StringUtils.split(state, "|");
					ApplicationContext app = WebApplicationContextUtils
							.getWebApplicationContext(this.sc);
					LoginService loginService = (LoginService) app
							.getBean("loginServiceImpl");
					EmployeeObj userObj = loginService.doUserValidete(ss[0],
							ss[1], request);
					return userObj;
				} catch (Exception e) {
					logger.error(e.getMessage());
				}
				break;
			}
		}
		return null;
	}

	/**
	 * @see Filter#init(FilterConfig)
	 */
	public void init(FilterConfig fConfig) throws ServletException {
		this.sc = fConfig.getServletContext();
		String paramValue = fConfig.getInitParameter("filterRes");
		filterRes = new HashSet<String>();
		if (paramValue != null) {
			String[] ress = StringUtils.split(paramValue, "|");
			for (String res : ress) {
				filterRes.add(res);
			}
		}
	}

}
