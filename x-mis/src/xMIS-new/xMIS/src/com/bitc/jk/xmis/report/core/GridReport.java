package com.bitc.jk.xmis.report.core;

import com.bitc.jk.xmis.report.model.GridData;
import com.bitc.jk.xmis.report.model.GridMetaData;

public interface GridReport {

	public GridData getGridData();

	public GridMetaData getGridMetaData();

}
