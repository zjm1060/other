package util;

public class DataField {
	private String name;
	private String mapping;
	private String type;
	

	public DataField() {
	}

	public DataField(String name, String mapping, String type) {
		super();
		this.name = name;
		this.mapping = mapping;
		this.type = type;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getMapping() {
		return mapping;
	}

	public void setMapping(String mapping) {
		this.mapping = mapping;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}
}