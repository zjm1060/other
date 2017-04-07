package util;

public class DateField extends DataField {
	private String dateFormat;

	public DateField() {
		super();
		// TODO Auto-generated constructor stub
	}

	public DateField(String name, String mapping, String type) {
		super(name, mapping, type);
		// TODO Auto-generated constructor stub
	}

	public String getDateFormat() {
		return dateFormat;
	}

	public void setDateFormat(String dateFormat) {
		this.dateFormat = dateFormat;
	}

}
