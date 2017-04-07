Ext.define('Ext.ux.TreeCheckAll', {
			init : function(tree) {
				/* 向上遍历父结点 */
				var nodep = function(node) {
					var bnode = true;
					Ext.Array.each(node.childNodes, function(v) {
								if (!v.data.checked) {
									bnode = false;
									return;
								}
							});
					return bnode;
				};
				var parentnode = function(node) {
					if (node.parentNode != null) {
						if (nodep(node.parentNode)) {
							node.parentNode.set('checked', true);
						} else {
							node.parentNode.set('checked', false);
						}
						parentnode(node.parentNode);
					}
				};
				/* 遍历子结点 选中 与取消选中操作 */
				var chd = function(node, check) {
					node.set('checked', check);
					if (node.isNode) {
						node.eachChild(function(child) {
									chd(child, check);
								});
					}
				};
				tree.on('checkchange', function(node, checked) {
							if (checked) {
								node.eachChild(function(child) {
											chd(child, true);
										});
							} else {
								node.eachChild(function(child) {
											chd(child, false);
										});
							}
							parentnode(node); // 进行父级选中操作
						}, tree);
			}
		});