package bitc.sz.wzEnv.sys.exception;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.springframework.web.servlet.HandlerExceptionResolver;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.view.json.MappingJackson2JsonView;

import bitc.sz.wzEnv.util.WebUtil;

public class ExceptionHandler implements HandlerExceptionResolver {

	public ModelAndView resolveException(HttpServletRequest request,
			HttpServletResponse response, Object handler, Exception ex) {
		Logger logger = Logger.getLogger(handler.getClass());
		String errorMsg = ex.getMessage();
		logger.error(errorMsg, ex);
		if (WebUtil.isAjaxRequest(request)) {
			return jsonView(errorMsg);
		}
		return new ModelAndView("error");
	}

	private ModelAndView jsonView(String errorMsg) {
		MappingJackson2JsonView view = new MappingJackson2JsonView();
		Map<String, Object> attributes = new HashMap<String, Object>();
		attributes.put("success", false);
		attributes.put("message", errorMsg);
		attributes.put("errorMsg", errorMsg);
		attributes.put("showMessage", true);
		view.setAttributesMap(attributes);
		view.setContentType("text/html;charset=UTF-8");
		return new ModelAndView(view);
	}

}