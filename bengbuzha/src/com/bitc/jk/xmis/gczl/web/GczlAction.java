package com.bitc.jk.xmis.gczl.web;
import java.lang.reflect.InvocationTargetException;
import java.math.BigDecimal;
import java.util.Date;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.gczl.model.ConcSeam;
import com.bitc.jk.xmis.gczl.model.ConcreteCarb;
import com.bitc.jk.xmis.gczl.model.FlexSeam;
import com.bitc.jk.xmis.gczl.model.HorizontalShift;
import com.bitc.jk.xmis.gczl.model.OphContent;
import com.bitc.jk.xmis.gczl.model.Sink;
import com.bitc.jk.xmis.gczl.model.SteelSCT;
import com.bitc.jk.xmis.gczl.model.riverDm;
import com.bitc.jk.xmis.gczl.model.weather;
import com.bitc.jk.xmis.gczl.service.GczlService;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class GczlAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(GczlAction.class);

	private GczlService gczlService;

	public static Logger getLogger() {
		return logger;
	}

	public static void setLogger(Logger logger) {
		GczlAction.logger = logger;
	}

	public GczlService getGczlService() {
		return gczlService;
	}

	public void setGczlService(GczlService gczlService) {
		this.gczlService = gczlService;
	}
/**
 * 伸缩缝
 * @param mapping
 * @param form
 * @param request
 * @param response
 */
	public void getFsCodeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getFsCodeList();
		WebUtil.returnResponse(response, store);
	}
	public void doSavefsInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		FlexSeam info = new FlexSeam();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setSeam_width(new BigDecimal(request.getParameter("seam_width")));
		info.setWeather(new BigDecimal(request.getParameter("weather")));
		info.setFs_code(request.getParameter("fs_code"));
		info.setMemo(request.getParameter("memo"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = gczlService.doSavefsInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeletefsInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			gczlService.doDeletefsInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getFsInfoListWithGroupQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = gczlService.getFsInfoListWithGroupQuery(pageInfo, queryStr,query);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getFsColumnNameCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = gczlService.getFsColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);

	}
	/**
	 * 沉陷资料
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getSpCodeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getSpCodeList();
		WebUtil.returnResponse(response, store);
	}
	public void getS_point_codeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String sp_code = request.getParameter("sp_code");
		String store = gczlService.getS_point_codeList(sp_code);
		WebUtil.returnResponse(response, store);
	}
	public void doSaveSinkInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		Sink info = new Sink();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setAp_w_level(new BigDecimal(request.getParameter("ap_w_level")));
		info.setBp_w_level(new BigDecimal(request.getParameter("bp_w_level")));
		info.setHigh_degree(new BigDecimal(request.getParameter("high_degree")));
		info.setSp_code(request.getParameter("sp_code"));
		info.setS_point_code(request.getParameter("s_point_code"));
		info.setMemo(request.getParameter("memo"));
		info.setS_point_name(request.getParameter("s_point_name"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = gczlService.doSaveSinkInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeleteSinkInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			gczlService.doDeleteSinkInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getSinkInfoListWithGroupQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = gczlService.getSinkInfoListWithGroupQuery(pageInfo, queryStr,query);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getSinkColumnNameCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = gczlService.getSinkColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);

	}
	/**
	 * 气温资料
	 */
	public void getWeatherList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getWeatherCodeList();
		WebUtil.returnResponse(response, store);
	}
	public void doSaveWeatherInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		weather info = new weather();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setSky_status(request.getParameter("sky_status"));
		info.setWeather(new BigDecimal(request.getParameter("weather")));
		info.setMemo(request.getParameter("memo"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = gczlService.doSaveWeatherInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeleteWeatherInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			gczlService.doDeleteWeatherInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getWeatherInfoListWithGroupQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = gczlService.getWeatherInfoListWithGroupQuery(pageInfo, queryStr,query);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getWeatherColumnNameCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = gczlService.getWeatherColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);

	}
	/**
	 * 河道断面
	 */
	public void getRiverDmList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getRiverDmList();
		WebUtil.returnResponse(response, store);
	}
	public void doSaveRiverDmInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		riverDm info = new riverDm();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setPillar_code(request.getParameter("pillar_code"));
		info.setDistance(new BigDecimal(request.getParameter("distance")));
		info.setHigh_degree(new BigDecimal(request.getParameter("high_degree")));
		info.setMemo(request.getParameter("memo"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = gczlService.doSaveRiverDmInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeleteRiverDmInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			gczlService.doDeleteRiverDmInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getRiverDmInfoListWithGroupQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = gczlService.getRiverDmInfoListWithGroupQuery(pageInfo, queryStr,query);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getRiverDmColumnNameCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = gczlService.getRiverDmColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);

	}
	/**
	 * 混凝土碳化
	 */
	public void getConcreteCarbList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getConcreteCarbList();
		WebUtil.returnResponse(response, store);
	}
	public void doSaveConcreteCarbInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		ConcreteCarb info = new ConcreteCarb();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setPlace_code(request.getParameter("place_code"));
		info.setCarbonize_depth(new BigDecimal(request.getParameter("carbonize_depth")));
		info.setAllow_carb_depth(new BigDecimal(request.getParameter("allow_carb_depth")));
		info.setMemo(request.getParameter("memo"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = gczlService.doSaveConcreteCarbInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeleteConcreteCarbInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			gczlService.doDeleteConcreteCarbInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getConcreteCarbInfoListWithGroupQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = gczlService.getConcreteCarbInfoListWithGroupQuery(pageInfo, queryStr,query);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getConcreteCarbColumnNameCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = gczlService.getConcreteCarbColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);

	}
	/**
	 * 钢结构锈蚀
	 */
	public void getSteelSCTList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getSteelSCTList();
		WebUtil.returnResponse(response, store);
	}
	public void doSaveSteelSCTInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		SteelSCT info = new SteelSCT();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setPlace_code(request.getParameter("place_code"));
		info.setTarnish_depth(new BigDecimal(request.getParameter("tarnish_depth")));
		info.setSteel_stru_thick(new BigDecimal(request.getParameter("steel_stru_thick")));
		info.setMemo(request.getParameter("memo"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = gczlService.doSaveSteelSCTInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeleteSteelSCTInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			gczlService.doDeleteSteelSCTInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getSteelSCTInfoListWithGroupQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = gczlService.getSteelSCTInfoListWithGroupQuery(pageInfo, queryStr,query);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getSteelSCTColumnNameCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = gczlService.getSteelSCTColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);

	}
	/**
	 * 水平位移
	 */
	public void getHorizontalShiftList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getHorizontalShiftList();
		WebUtil.returnResponse(response, store);
	}
	public void doSaveHorizontalShiftInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		HorizontalShift info = new HorizontalShift();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setPlace_code(request.getParameter("place_code"));
		info.setStart_formula(new BigDecimal(request.getParameter("start_formula")));
		info.setMeasured_value(new BigDecimal(request.getParameter("measured_value")));
		info.setMemo(request.getParameter("memo"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = gczlService.doSaveHorizontalShiftInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeleteHorizontalShiftInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			gczlService.doDeleteHorizontalShiftInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getHorizontalShiftInfoListWithGroupQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = gczlService.getHorizontalShiftInfoListWithGroupQuery(pageInfo, queryStr,query);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getHorizontalShiftColumnNameCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = gczlService.getHorizontalShiftColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getStart_formula(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String hori_code = request.getParameter("hori_code");

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			BigDecimal start_formula = gczlService.getStart_formula(hori_code);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage(start_formula+"");
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	/**
	 * 泥沙含量
	 */
	public void getRiverCodeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getRiverCodeList();
		WebUtil.returnResponse(response, store);
	}
	public void getPlaceCodeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getPlaceCodeList();
		WebUtil.returnResponse(response, store);
	}
	public void doSaveOphContentInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		OphContent info = new OphContent();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setRiver_code(request.getParameter("river_code"));
		info.setPlace_code(request.getParameter("place_code"));
		info.setOph_content_qty(new BigDecimal(request.getParameter("oph_content_qty")));
		info.setMemo(request.getParameter("memo"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = gczlService.doSaveOphContentInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeleteOphContentInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			gczlService.doDeleteOphContentInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getOphContentInfoListWithGroupQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = gczlService.getOphContentInfoListWithGroupQuery(pageInfo, queryStr,query);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getOphContentColumnNameCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = gczlService.getOphContentColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);
	}
	/**
	 * 混凝土裂缝
	 */
	public void getConcSeamCodeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = gczlService.getConcSeamCodeList();
		WebUtil.returnResponse(response, store);
	}
	public void getConcMpCodeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String concSeamCode = request.getParameter("conc_seam_code");
		String store = gczlService.getConcMpCodeList(concSeamCode);
		WebUtil.returnResponse(response, store);
	}
	public void doSaveConcSeamInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		ConcSeam info = new ConcSeam();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(Type.getString(request.getParameter("unit_code")));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setConc_seam_code(Type.getString(request.getParameter("conc_seam_code")));
		info.setConc_mp_code(Type.getString(request.getParameter("conc_mp_code")));
		info.setWeather(new BigDecimal(request.getParameter("weather")));
		info.setSeam_long(new BigDecimal(request.getParameter("seam_long")));
		info.setSeam_width(new BigDecimal(request.getParameter("seam_width")));
		info.setXzms(Type.getString(request.getParameter("xzms")));
		info.setMemo(Type.getString(request.getParameter("memo")));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = gczlService.doSaveConcSeamInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeleteConcSeamInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			gczlService.doDeleteConcSeamInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getConcSeamInfoListWithGroupQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = gczlService.getConcSeamInfoListWithGroupQuery(pageInfo, queryStr,query);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getConcSeamColumnNameCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = gczlService.getConcSeamColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);
	}
	/**
	 * 沉陷资料统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getHightDegree(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String last_date = request.getParameter("last_date");
		String the_date = request.getParameter("the_date");
		String sp_code = request.getParameter("sp_code");
		String jsonstr = gczlService.getHightDegree(pageInfo,last_date,the_date,sp_code);
		WebUtil.returnResponse(response, jsonstr);
	}
	/**
	 * 伸缩缝统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getFlexSeam(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String start_date = request.getParameter("start_date");
		String end_date = request.getParameter("end_date");
		String fs_code = request.getParameter("fs_code");
		String jsonstr = gczlService.getFlexSeam(pageInfo,start_date,end_date,fs_code);
		WebUtil.returnResponse(response, jsonstr);
	}
	/**
	 * 获取伸缩缝最大值、最小值
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getMaxAndMinFlexSeam(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String start_date = request.getParameter("start_date");
		String end_date = request.getParameter("end_date");
		String fs_code = request.getParameter("fs_code");
		String jsonstr = gczlService.getMaxAndMinFlexSeam(start_date,end_date,fs_code);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getCeYaStatistics(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String start_date = request.getParameter("start_date");
		String end_date = request.getParameter("end_date");
		String mptp_code = request.getParameter("mptp_code");
		String mpt_name = request.getParameter("mpt_name");
		String jsonstr = gczlService.getCeYaStatistics(pageInfo,start_date,end_date,mptp_code,mpt_name);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getSWJWaterLevel(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String start_date = request.getParameter("start_date");
		String end_date = request.getParameter("end_date");
		String jsonstr = gczlService.getSWJWaterLevel(start_date,end_date);
		WebUtil.returnResponse(response, jsonstr);
	}
	/**
	 * 获取测压水位最大值、最小值
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getMaxAndMinCeYa(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String start_date = request.getParameter("start_date");
		String end_date = request.getParameter("end_date");
		String mptp_code = request.getParameter("mptp_code");
		String mpt_name = request.getParameter("mpt_name");
		String jsonstr = gczlService.getMaxAndMinCeYa(start_date,end_date,mptp_code,mpt_name);
		WebUtil.returnResponse(response, jsonstr);
	}
	/**
	 * 流量公式
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getLLValue(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String start_date = request.getParameter("begDate");
		String end_date = request.getParameter("endDate");
		String type = request.getParameter("type");
		String jsonstr = gczlService.getLLValue(pageInfo,start_date,end_date,type);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getWaterLevel(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String start_date = request.getParameter("start_date");
		String end_date = request.getParameter("end_date");
		String jsonstr = gczlService.getWaterLevel(start_date,end_date);
		WebUtil.returnResponse(response, jsonstr);
	}
	public void getLiuLiang(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		
		String gate_open_num_1 = Type.getString( request.getParameter("gate_open_num_1"));
		String gate_open_degree_1 = Type.getString(request.getParameter("gate_open_degree_1"));
		String gate_open_num_2 = Type.getString(request.getParameter("gate_open_num_2"));
		String gate_open_degree_2 =Type.getString( request.getParameter("gate_open_degree_2"));
		String ap = Type.getString(request.getParameter("ap"));
		String bp = Type.getString(request.getParameter("bp"));
		String elec_machine = Type.getString(request.getParameter("elec_machine"));
		if("".equals(gate_open_num_1)){
			gate_open_num_1=0+"";
		}
		if("".equals(gate_open_degree_1)){
			gate_open_degree_1=0+"";
		}
		if("".equals(gate_open_num_2)){
			gate_open_num_2=0+"";
		}
		if("".equals(gate_open_degree_2)){
			gate_open_degree_2=0+"";
		}
		if("".equals(elec_machine)){
			elec_machine=0+"";
		}
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			List list = gczlService.getLiuLiang(gate_open_num_1,gate_open_degree_1,gate_open_num_2,gate_open_degree_2,ap,bp,elec_machine);
			if(list.size()>0){
				ajaxResponse.setSuccess(true);
				ajaxResponse.addAttribute("liuliang", ((Map)list.get(0)).get("流量"));
				ajaxResponse.setMessage("查询成功！");
			}
		} catch (Exception e) {
			logger.error("信息的查询出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("查询错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
}
