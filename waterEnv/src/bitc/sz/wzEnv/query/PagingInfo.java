package bitc.sz.wzEnv.query;

import org.hibernate.criterion.Order;

public class PagingInfo {
	private int start;
	private int limit;
	private String sort;
	private String order;

	public String getSort() {
		return sort;
	}

	/**
	 * 设置排序属性
	 * 
	 * @param sort
	 */
	public void setSort(String sort) {
		this.sort = sort;
	}

	public String getOrder() {
		return order;
	}

	/**
	 * 设置排序规则
	 * 
	 * @param order
	 *            desc or asc
	 */
	public void setOrder(String order) {
		this.order = order;
	}

	public boolean isPaging() {
		return this.limit > 0;
	}

	public Order buildOrder() {
		if ("desc".equalsIgnoreCase(order)) {
			return Order.desc(sort);
		} else {
			return Order.asc(sort);
		}
	}

	public int getStart() {
		return start;
	}

	public void setStart(int start) {
		this.start = start;
	}

	public int getLimit() {
		return limit;
	}

	public void setLimit(int limit) {
		this.limit = limit;
	}

	public int getFirst() {
		return this.start;
	}

	public int getRows() {
		return this.limit;
	}
}
