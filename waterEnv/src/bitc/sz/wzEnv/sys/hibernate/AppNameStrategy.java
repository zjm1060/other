package bitc.sz.wzEnv.sys.hibernate;

import org.hibernate.cfg.DefaultNamingStrategy;

import bitc.sz.wzEnv.util.CamelCaseUtils;


public class AppNameStrategy extends DefaultNamingStrategy {

	private static final long serialVersionUID = 1893437782964893335L;
	private String prefix;

	@Override
	public String classToTableName(String className) {
		return addPrefix(super.classToTableName(className));
	}

	private String addPrefix(String tableName) {
		if (prefix != null) {
			// 大写开头
			if (tableName.charAt(0) < 97) {
				tableName = prefix + tableName;
			} else {
				tableName = prefix + "_" + tableName;
			}

		}
		return CamelCaseUtils.toUnderScoreCase(tableName);
	}

	@Override
	public String collectionTableName(String ownerEntity,
			String ownerEntityTable, String associatedEntity,
			String associatedEntityTable, String propertyName) {
		return addPrefix(super.collectionTableName(ownerEntity,
				ownerEntityTable, associatedEntity, associatedEntityTable,
				propertyName));
	}

	@Override
	public String logicalColumnName(String columnName, String propertyName) {
		return CamelCaseUtils.toUnderScoreCase(super.logicalColumnName(
				columnName, propertyName));
	}

	@Override
	public String joinKeyColumnName(String joinedColumn, String joinedTable) {
		return CamelCaseUtils.toUnderScoreCase(super.joinKeyColumnName(
				joinedColumn, joinedTable));
	}

	@Override
	public String columnName(String columnName) {
		return CamelCaseUtils.toUnderScoreCase(super.columnName(columnName));
	}

	@Override
	public String tableName(String tableName) {
		return addPrefix(super.tableName(tableName));
	}

	@Override
	public String propertyToColumnName(String propertyName) {
		return CamelCaseUtils.toUnderScoreCase(super
				.propertyToColumnName(propertyName));
	}

	@Override
	public String foreignKeyColumnName(String propertyName,
			String propertyEntityName, String propertyTableName,
			String referencedColumnName) {
		return CamelCaseUtils.toUnderScoreCase(super.foreignKeyColumnName(
				propertyName, propertyEntityName, propertyTableName,
				referencedColumnName));
	}

	public String logicalCollectionTableName(String tableName,
			String ownerEntityTable, String associatedEntityTable,
			String propertyName) {
		return addPrefix(super.logicalCollectionTableName(tableName,
				ownerEntityTable, associatedEntityTable, propertyName));
	};

	public String getPrefix() {
		return prefix;
	}

	public void setPrefix(String prefix) {
		this.prefix = prefix;
	}

}
