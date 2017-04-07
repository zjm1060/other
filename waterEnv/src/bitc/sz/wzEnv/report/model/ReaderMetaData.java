package bitc.sz.wzEnv.report.model;

import java.io.Serializable;
import java.sql.ResultSet;
import java.util.List;

public class ReaderMetaData implements Serializable {

	private static final long serialVersionUID = -4687834751331962112L;

	private String root = "rows";
	private String totalProperty = "totalCount";
	private String successProperty = "success";
	private List<ReaderField> fields;

	public ReaderMetaData() {
	}

	public static ReaderMetaData buildReaderMetaData(ResultSet resultSet) {
		return new ReaderMetaData();
	}

	public String getRoot() {
		return root;
	}

	public void setRoot(String root) {
		this.root = root;
	}

	public String getTotalProperty() {
		return totalProperty;
	}

	public void setTotalProperty(String totalProperty) {
		this.totalProperty = totalProperty;
	}

	public String getSuccessProperty() {
		return successProperty;
	}

	public void setSuccessProperty(String successProperty) {
		this.successProperty = successProperty;
	}

	public List<ReaderField> getFields() {
		return fields;
	}

	public void setFields(List<ReaderField> fields) {
		this.fields = fields;
	}
}
