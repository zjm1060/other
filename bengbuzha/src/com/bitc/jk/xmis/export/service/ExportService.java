package com.bitc.jk.xmis.export.service;

import java.io.File;
import java.io.IOException;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;

import jxl.write.WriteException;
import net.sf.json.JSONArray;

public interface ExportService {
	void writeBook(JSONArray columnModeArray, JSONArray rowsArray,
			File targetFile, Map<String, String> formaterMap,PageInfo pageInfo,String webRook)
			throws IOException, WriteException, Exception;
}
