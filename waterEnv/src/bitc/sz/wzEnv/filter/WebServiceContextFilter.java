package bitc.sz.wzEnv.filter;

import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

/**
 * Created by zsz on 2016/3/10.
 */
public class WebServiceContextFilter implements Filter {
    public static ThreadLocal<HttpServletRequest> request = new ThreadLocal<HttpServletRequest>();
    public static ThreadLocal<HttpServletResponse> response = new ThreadLocal<HttpServletResponse>();

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {

    }

    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        request.set((HttpServletRequest) servletRequest);
        response.set((HttpServletResponse)servletResponse);
        filterChain.doFilter(servletRequest,servletResponse);
    }

    @Override
    public void destroy() {
        request.remove();
    }
}
