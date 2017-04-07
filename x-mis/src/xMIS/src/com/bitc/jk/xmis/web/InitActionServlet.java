package com.bitc.jk.xmis.web;

import java.io.IOException;
import java.net.URLEncoder;
import java.util.Iterator;
import java.util.Map;
import java.util.Stack;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionServlet;

import com.bitc.jk.xmis.util.WebKeys;



public class InitActionServlet extends ActionServlet {
    /**
	 * 
	 */
	private static final long serialVersionUID = 1L;
    private static Logger logger = Logger.getLogger(InitActionServlet.class);

    public static String VERSION_NUMBER_KEY="version";
    public static String BUILD_NUMBER_KEY="builddate";

    public static String RELEASE_NUMBER;
    public static String VERSION_NUMBER;
    public static String BUILD_NUMBER;
    
    public static String Layout;
    
	
    public void init(ServletConfig config) throws ServletException {
    	
    	logger.info("---> init");
        super.init(config);
    
        VERSION_NUMBER =getInitParameter(VERSION_NUMBER_KEY); 
        BUILD_NUMBER =getInitParameter(BUILD_NUMBER_KEY);
        RELEASE_NUMBER = VERSION_NUMBER + '.' + BUILD_NUMBER;
        
        Layout = getInitParameter(WebKeys.Session_Attribute_Name_Layout);
        
    }

    protected void process(HttpServletRequest request,
            HttpServletResponse response) throws IOException, ServletException {
           	
       // WebUtil.putParameterToAttribute(request);       
    	 putParameterToAttribute(request);
        putTargetServletPath(request);    
        
        request.getSession().setAttribute(WebKeys.Session_Attribute_Name_Layout, Layout);

        logger.info(request.getRequestURI());
        
        // used for grid tag, DON'T remove it
        if (request.getRequestURI().indexOf("loginAction")<0) {
            request.setAttribute(WebKeys.RequestKey_Original_URI, request.getRequestURI().substring(5));
            request.setAttribute(WebKeys.RequestKey_Original_URL,
                    URLEncoder.encode(request.getRequestURL().toString(),"UTF-8"));
//            if (request.getQueryString() != null )
//                request.setAttribute(WebKeys.RequestKey_Original_QueryString, 
//                		URLEncoder.encode(request.getQueryString(), "UTF-8"));
            
              request.setAttribute(WebKeys.RequestKey_Original_QueryString, 
        		request.getQueryString());

            
            logger.info("URI = "+ request.getAttribute(WebKeys.RequestKey_Original_URI));
            logger.info("Url = " + request.getRequestURL().toString());
            logger.info("queryString = " + request.getAttribute(WebKeys.RequestKey_Original_QueryString));
            
            request.getSession().setAttribute(WebKeys.RequestKey_Original_URI,
					request.getAttribute(WebKeys.RequestKey_Original_URI));
			request.getSession().setAttribute(WebKeys.RequestKey_Original_URL,
					request.getAttribute(WebKeys.RequestKey_Original_URL));
			request
					.getSession()
					.setAttribute(
							WebKeys.RequestKey_Original_QueryString,
							request
									.getAttribute(WebKeys.RequestKey_Original_QueryString));
            
        }
        try {
            super.process(request, response);
        }finally {
//            UrlAccessLogger.log(request);
        }
    }

    
    public static void putParameterToAttribute(HttpServletRequest request) {
        Map paramMap = request.getParameterMap();
        if(paramMap != null)
            for (Iterator it = paramMap.entrySet().iterator(); it.hasNext(); ) {
                Map.Entry entry = (Map.Entry) it.next();
                request.setAttribute((String) entry.getKey(),entry.getValue());
            }
    }
    
    public static void putTargetServletPath(HttpServletRequest request) {
        Stack stack =
            (Stack) request.getAttribute(WebKeys.TARGET_URL_SESSION_KEY);

        if (stack == null)
            stack = new Stack();
        String uri = request.getRequestURL().toString();
        String queryString = request.getQueryString();
        if (queryString != null)
            uri = uri + "?" + queryString;
        if (uri == null)
            uri = request.getServletPath();
        stack.push(uri);
        
        logger.info("uri="+uri +" // stack=" + stack );
        
        request.setAttribute(WebKeys.TARGET_URL_SESSION_KEY, stack);
    }
   


   
}
