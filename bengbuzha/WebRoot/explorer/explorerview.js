var grid2;
Ext.onReady(function() {
	var dummyData2 = [
		['Program Files', '2008-01-01 00:00', 'File Folder', 0, 'folder.png'],
		['Program Files (x86)', '2008-01-01 00:03', 'File Folder', 0,
				'folder.png'],
		['ProgramData', '2008-02-06 13:21', 'File Folder', 0, 'folder.png'],
		['temp', '2007-12-05 00:59', 'File Folder', 0, 'folder.png'],
		['Users', '2008-05-01 18:08', 'File Folder', 0, 'folder.png'],
		['Windows', '2008-01-01 04:57', 'File Folder', 0, 'folder.png'],

		['install.exe', '2008-08-17 03:42', 'Application', 561671,
				'application.png'],
		['globdata.ini', '2008-10-01 16:01', 'Configuration Settings', 3214,
				'application-settings.png'],
		['VC_RED.MSI', '2008-10-09 07:31', 'Application', 9498742,
				'application-installer.png'],
		['VC_RED.cab', '2008-10-09 07:31', 'WinRAR Archive', 65789416,
				'winrar-archive.png']];
	var reader = new Ext.data.ArrayReader({}, [{
						name : 'name'
					}, {
						name : 'modified',
						type : 'date',
						dateFormat : 'Y-m-d H:i'
					}, {
						name : 'type'
					}, {
						name : 'size'
					}, {
						name : 'icon'
					}]);

	var largeIcons = new Ext.Template('<div class="x-grid3-row ux-explorerview-item ux-explorerview-large-item" unselectable="on">'
					+ '<table class="ux-explorerview-icon" cellpadding="0" cellspacing="0">'
					+ '<tr><td align="center"><img src="'
					+ __cxtPath
					+ '/explorer/images/large-{icon}"></td></tr></table>'
					+ '<div class="ux-explorerview-text"><div class="x-grid3-cell x-grid3-td-name" unselectable="on">{name}</div></div></div>');

	var mediumIcons = new Ext.Template('<div class="x-grid3-row ux-explorerview-item ux-explorerview-medium-item">'
					+ '<table class="ux-explorerview-icon" cellpadding="0" cellspacing="0">'
					+ '<tr><td align="center"><img src="'
					+ __cxtPath
					+ '/explorer/images/medium-{icon}"></td></tr></table>'
					+ '<div class="ux-explorerview-text"><div class="x-grid3-cell x-grid3-td-name" unselectable="on">{name}</div></div></div>');

	var smallIcons = new Ext.Template('<div class="x-grid3-row ux-explorerview-item ux-explorerview-small-item">'
					+ '<div class="ux-explorerview-icon"><img src="'
					+ __cxtPath
					+ '/explorer/images/small-{icon}"></div>'
					+ '<div class="ux-explorerview-text"><div class="x-grid3-cell x-grid3-td-name" unselectable="on">{name}</div></div></div>');

	var tileIcons = new Ext.Template('<div class="x-grid3-row ux-explorerview-item ux-explorerview-tiles-item">'
					+ '<div class="ux-explorerview-icon"><img src="'
					+ __cxtPath
					+ '/explorer/images/medium-{icon}"></div>'
					+ '<div class="ux-explorerview-text"><div class="x-grid3-cell x-grid3-td-name" unselectable="on">{name}<br/><span>{type}</span></div></div></div>');

	function changeView(item, checked) {
		var tpl;
		if (checked) {
			if (item.text == 'Large Icons')
				tpl = largeIcons;
			else if (item.text == 'Medium Icons')
				tpl = mediumIcons;
			else if (item.text == 'Small Icons')
				tpl = smallIcons;
			else if (item.text == 'Tiles')
				tpl = tileIcons;
			else
				tpl = null;
			grid2.getView().changeTemplate(tpl);
		}
	}
	grid2 = new Ext.grid.GridPanel({
				store : new Ext.data.Store({
							reader : reader,
							data : dummyData2,
							sortInfo : {
								field : 'modified',
								direction : 'DESC'
							}
						}),
				columns : [{
							id : 'name',
							header : 'Name',
							sortable : true,
							dataIndex : 'name'
						}, {
							header : 'Modified',
							width : 120,
							sortable : true,
							renderer : Ext.util.Format
									.dateRenderer('Y-m-d H:i'),
							dataIndex : 'modified'
						}, {
							header : 'Type',
							width : 120,
							sortable : true,
							dataIndex : 'type'
						}, {
							header : 'Size',
							width : 120,
							sortable : true,
							dataIndex : 'size',
							align : 'right',
							renderer : Ext.util.Format.fileSize
						}],

				viewConfig : {
					// Let this grid be special :)
					rowTemplate : null
				},
				tbar : new Ext.Toolbar({
							items : [{
										text : 'Views',
										menu : [{
													group : 'view',
													checkHandler : changeView,
													checked : false,
													text : 'Extra Large Icons',
													iconCls : 'view-xl-icons',
													disabled : true
												}, {
													group : 'view',
													checkHandler : changeView,
													checked : true,
													text : 'Large Icons',
													iconCls : 'view-l-icons'
												}, {
													group : 'view',
													checkHandler : changeView,
													checked : false,
													text : 'Medium Icons',
													iconCls : 'view-m-icons'
												}, {
													group : 'view',
													checkHandler : changeView,
													checked : false,
													text : 'Small Icons',
													iconCls : 'view-s-icons'
												}, '-', {
													group : 'view',
													checkHandler : changeView,
													checked : false,
													text : 'List',
													iconCls : 'view-list',
													disabled : true
												}, '-', {
													group : 'view',
													checkHandler : changeView,
													checked : false,
													text : 'Details',
													iconCls : 'view-details'
												}, '-', {
													group : 'view',
													checkHandler : changeView,
													checked : false,
													text : 'Tiles',
													iconCls : 'view-tiles'
												}]
									}]
						}),
				enableDragDrop : true,
				autoExpandColumn : 'name',
				border:false,
				renderTo : 'explorerDIV'
			});
});

// Array data for the grids

