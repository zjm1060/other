String.prototype.trim = function() {
	var re = /^\s+|\s+$/g;
	return function() {
		return this.replace(re, "");
	};
}();

/**
 * 合同摘要Format工具（合同列表，合同列表导出）
 * 
 * @param contractSummary
 *            合同摘要
 * @param summarystencil
 *            摘要模板
 * @param color
 *            颜色 例:'red'
 */
SummaryFormatUtil = function(contractSummary, summarystencil, color) {
	var vals = contractSummary.split('&^$>,<$^&');
	if (vals != null && vals.length > 0) {
		if (vals.length != 1) {
			vals[0] = vals[0].substring(4);
			var l = vals.length - 1;
			vals[l] = vals[l].substring(0, vals[l].length - 4);
		}
	}
	var result = '';
	var isEmpty = true;
	if (vals.length != 1) {
		for (var i = 0; i < vals.length; i++) {
			if (vals[i].trim() != '') {
				isEmpty = false;
			}
		}
	}
	if (!isEmpty) {
		var content = summarystencil;
		for (var i = 0; i < vals.length; i++) {
			if (content.indexOf('{}')) {
				if (color) {
					result += content.substring(0, content.indexOf('{}'))
							+ ' <font color = ' + color + '>'
							+ (vals[i].trim() == '' ? '__' : vals[i].trim())
							+ '</font>';
				} else {
					result += content.substring(0, content.indexOf('{}'))
							+ (vals[i].trim() == '' ? '__' : vals[i].trim());
				}
				content = content.substring(content.indexOf('{}') + 2);
			} else {
				result += content;
			}
		}
	}
	if (vals.length == 1 && vals[0].trim() != '') {
		result = vals[0];
		existOldSummary = result;
	}
	return result;
};