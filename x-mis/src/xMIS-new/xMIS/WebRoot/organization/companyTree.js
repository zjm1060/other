function buildPanel(compID) {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	Ext.form.Field.prototype.blankText = '该项为必输项';

	var selected;

	var newDept = ""; // 新部门 (id:text,...)
	var newEmpl = ""; // 新员工 (id:text,...)
	var newMapping = ""; // 新部门新员工映射 (cid:pid,...)

	var dragRelation = ""; // 拖拽关系 (cid:nid,...)
	var changeEmplName = ""; // 改名(id:修改后名字,...)

	var changeDeptName = "";
	var frontParentIds = "";

	var currentNodePath = "";

	var addressIDParam = '';
	var cityIDParam = '';
	var zipCodeAddressParam = '';
	var politicalStatusParam = '';
	var refid_empID = '';

	var positions = Ext.data.Record.create([{
				name : "ID",
				type : "int",
				mapping : "id"
			}, {
				name : "description",
				type : "string",
				mapping : "description"
			}]);

	var positionsStore = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/orgMaint.do?method=getPositionTotalSummary',
							method : "POST"
						}),
				autoLoad : true,
				reader : new Ext.data.JsonReader({}, positions)
			});

	function ergodic(currentNode) {// 遍历树
		var childNodes = currentNode.childNodes;// 获取根节点的子节点
		for (var i = 0; i < childNodes.length; i++) {
			var childNode = childNodes[i];
			if ("new" == childNode.id.substr(5, 3)) {
				if ("dept" == childNode.id.substr(0, 4)) {
					if ("" == newDept) {
						newDept = newDept + childNode.id + ":" + childNode.text;
					} else {
						newDept = newDept + "," + childNode.id + ":"
								+ childNode.text;
					}
				} // end if dept
				if ("empl" == childNode.id.substr(0, 4)) {
					if ("" == newEmpl) {
						newEmpl = newEmpl + childNode.id + ":" + childNode.text;
					} else {
						newEmpl = newEmpl + "," + childNode.id + ":"
								+ childNode.text;
					}
				} // end if empl

				if ("" == newMapping) {
					newMapping = newMapping + childNode.id + ":"
							+ childNode.parentNode.id;
				} else {
					newMapping = newMapping + "," + childNode.id + ":"
							+ childNode.parentNode.id;
				}

			} // end if new

			if (childNode.hasChildNodes()) {
				ergodic(childNode);// 递归调用
			}
		}

	}

	function ajaxSyncCall(urlStr, paramsStr) {

		var XMLHttpReq;
		var value;
		if (window.ActiveXObject) {
			try {
				XMLHttpReq = new ActiveXObject("Msxml2.XMLHTTP");
			} catch (e) {
				try {
					XMLHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
				} catch (e) {
				}
			}
		} else if (window.XMLHttpRequest) {
			XMLHttpReq = new XMLHttpRequest();
		}
		XMLHttpReq.open('POST', urlStr, false);
		XMLHttpReq.setRequestHeader('Content-Type',
				'application/x-www-form-urlencoded');
		XMLHttpReq.send(paramsStr);
		var result = Ext.util.JSON.decode(XMLHttpReq.responseText);
		return result;
	}

	var politicsStatus = Ext.data.Record.create([{
				name : "ID",
				type : "int",
				mapping : "id"
			}, {
				name : "Name",
				type : "string",
				mapping : "name"
			}]);

	var politicalStatusStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/orgMaint.do?method=getPoliticalStatusTotalSummary',
					method : "POST"
				}),
		autoLoad : true,
		reader : new Ext.data.JsonReader({}, politicsStatus)
	});

	var Human = Ext.data.Record.create([{
				name : "userid",
				type : "int",
				mapping : "userID"
			}, {
				name : "lastName",
				type : "string",
				convert : function(v, record) {
					return record.lastName.trim();
				}
			}, {
				name : "firstName",
				type : "string",
				convert : function(v, record) {
					return record.firstName.trim();
				}
			}, {
				name : "imgName",
				type : "string",
				mapping : "imgName"
			}, {
				name : "postCardName",
				type : "string",
				mapping : "postCardName"
			}, {
				name : "postCardBackName",
				type : "string",
				mapping : "postCardBackName"
			}, {
				name : "sex",
				type : "string",
				mapping : "sex.id"
			}, {
				name : "loginName",
				type : "string",
				mapping : "loginName"
			}, {
				name : 'birth',
				type : 'string',
				mapping : 'birthday'
			}, {
				name : 'password',
				type : 'string',
				mapping : 'password'
			}, {
				name : 'pathId',
				type : 'string',
				convert : function(v, record) {
					// 事业部ID
					if (record.pathId != null) {
						var a = record.pathId.substring(0, record.pathId
										.lastIndexOf(','));
						var array = a.split(',');
						var divisionID = array[array.length - 1];
						return divisionID;
					} else {
						return '';
					}
				}
			}, {
				name : 'IDCard',
				type : 'string',
				mapping : 'IDCard'
			}, {
				name : 'join',
				type : 'string',
				mapping : 'joinDate'
			}, {
				name : 'mobileNo',
				type : 'string',
				convert : function(v, record) {
					return record.mobileNo.trim();
				}
			}, {
				name : 'extNo',
				type : 'string',
				convert : function(v, record) {
					return record.extNo.trim();
				}
			}, {
				name : 'faxNo',
				type : 'int',
				convert : function(v, record) {
					return record.faxNo.trim();
				}
			}, {
				name : 'email',
				type : 'string',
				convert : function(v, record) {
					return record.emailAddress.trim();
				}
			}, {
				name : 'nativePlace',
				type : 'text',
				mapping : 'nativePlace'
			}, {
				name : 'zipCode',
				type : 'string',
				mapping : 'zipCode',
				convert : function(v, record) {
					return record.zipCode == 0 ? '' : record.zipCode;
				}
			}, {
				name : 'homeTelNo',
				type : 'string',
				convert : function(v, record) {
					return record.homeTelNo.trim();
				}
			}, {
				name : 'spouseName',
				type : 'string',
				mapping : 'spouseName'
			}, {
				name : 'issueName',
				type : 'string',
				mapping : 'issueName'
			}, {
				name : 'homeAddress',
				type : 'type',
				mapping : 'homeAddress'
			}, {
				name : 'webPage',
				type : 'type',
				convert : function(v, record) {
					return record.webPage.trim();
				}
			}, {
				name : 'hobby',
				type : 'string',
				mapping : 'hobby'
			}, {
				name : 'specialty',
				type : 'type',
				mapping : 'specialty'
			}, {
				name : 'urgentinfo',
				type : 'type',
				mapping : 'urgentinfo'
			}, {
				name : 'remark',
				type : 'type',
				mapping : 'remark'
			}, {
				name : 'maritalStatus',
				type : 'string',
				convert : function(v, record) {
					return record.marStat == null ? null : record.marStat.id;
				}
			}, {
				name : 'issueSex',
				type : 'string',
				convert : function(v, record) {
					return record.issSex == null ? null : record.issSex.id;
				}
			}, {
				name : 'disabledFlag',
				type : 'string',
				mapping : 'flag'
			}, {
				name : 'disabledReason',
				type : 'string',
				mapping : 'disabledReason'
			}, {
				name : 'disabledReasonID',
				type : 'string',
				mapping : 'disabledReasonID'
			}, {
				name : 'disabledDate',
				type : 'string',
				mapping : 'disabledDate',
				convert : function(val) {
					return val != null ? val.substring(0, 10) : '';
				}
			}, {
				name : 'educationHighestLevel',
				type : 'string',
				convert : function(v, record) {
					return record.edu == null ? "" : record.edu.name.trim();
				}
			}, {
				name : 'eduID',
				type : 'string',
				convert : function(v, record) {
					return record.edu == null ? null : record.edu.id;
				}
			}, {
				name : 'oldEduLevel',
				type : 'string',
				convert : function(v, record) {
					return record.edu == null ? "" : record.edu.name.trim();
				}
			}, {
				name : 'oldSchoolName',
				type : 'string',
				convert : function(v, record) {
					return record.edu == null ? "" : record.edu.schoolName
							.trim();
				}
			}, {
				name : 'oldGraduationDate',
				type : 'date',
				convert : function(v, record) {
					return record.edu == null ? "" : record.edu.graduationDate;
				}
			}, {
				name : 'oldMajor',
				type : 'string',
				convert : function(v, record) {
					return record.edu == null ? "" : record.edu.major.trim();
				}
			}, {
				name : 'oldEduRemark',
				type : 'string',
				convert : function(v, record) {
					return record.edu == null ? "" : record.edu.eduRemark
							.trim();
				}
			}, {
				name : 'polStat',
				type : 'string',
				convert : function(v, record) {
					return record.polStat == null ? null : record.polStat.id;
				}
			}, {
				name : 'disabledFlag',
				type : 'string',
				convert : function(v, record) {
					return record.flag == true ? 1 : 0;
				}
			}, {
				name : 'positionEdit',
				type : 'string'
			}, {
				name : 'position',
				type : 'string',
				convert : function(v, record) {
					return record.postList[0].post != null
							? record.postList[0].post.name
							: '';
				}
			}, {
				name : 'positionID',
				type : 'string',
				convert : function(v, record) {
					return record.postList[0].post != null
							? record.postList[0].post.id
							: 0;
				}
			}, {
				name : "switchboardNo",
				type : "string",
				convert : function(v, record) {
					return record.switchboardNo.trim();
				}
			}, {
				name : "ExtNo",
				type : "string",
				mapping : "extNo"
			}, {
				name : "officeAddress",
				type : "string",
				mapping : "officeAddress"
			}, {
				name : 'chooseAddress',
				mapping : 'address',
				convert : function(v) {
					if (v != null) {
						var isTrue = (v.city.province.provinceName.trim()) == (v.city.cityName
								.trim());
						var showname = '';
						if (isTrue) {
							showname = v.city.province.area.country.internation.internationName
									.trim()
									+ '·'
									+ v.city.province.area.country.countryName
											.trim()
									+ '·'
									+ v.city.province.area.areaName.trim()
									+ '·' + v.city.province.provinceName.trim()
						} else {
							showname = v.city.province.area.country.internation.internationName
									.trim()
									+ '·'
									+ v.city.province.area.country.countryName
											.trim()
									+ '·'
									+ v.city.province.area.areaName.trim()
									+ '·'
									+ v.city.province.provinceName.trim()
									+ '·' + v.city.cityName
						}
						return showname;
					} else {
						return ''
					}
				}
			}, {
				name : 'district',
				mapping : 'address',
				convert : function(v) {
					return v != null ? (v.district) : '';
				}
			}, {
				name : 'addressID',
				mapping : 'address',
				convert : function(v) {
					return v != null ? (v.addressID) : '';
				}
			}, {
				name : 'zipCodeAddress',
				mapping : 'address',
				convert : function(v) {
					return v != null ? (v.zipCode != 0 ? v.zipCode : '') : '';
				}
			}, {
				name : 'addressInfo',
				mapping : 'address',
				convert : function(v) {
					return v != null ? (v.addressInfo) : '';
				}
			}, {
				name : 'internationID',
				mapping : 'address',
				convert : function(v) {
					return v != null
							? (v.city != null
									? v.city.province.area.country.internation.internationID
									: '')
							: '';
				}
			}, {
				name : 'countryID',
				mapping : 'address',
				convert : function(v) {
					return v != null ? (v.city != null
							? v.city.province.area.country.countryID
							: '') : '';
				}
			}, {
				name : 'areaID',
				mapping : 'address',
				convert : function(v) {
					return v != null ? (v.city != null
							? v.city.province.area.areaID
							: '') : '';
				}
			}, {
				name : 'provinceID',
				mapping : 'address',
				convert : function(v) {
					return v != null ? (v.city != null
							? v.city.province.provinceID
							: '') : '';
				}
			}, {
				name : 'cityID',
				mapping : 'address',
				convert : function(v) {
					return v != null
							? (v.city != null ? v.city.cityID : '')
							: '';
				}
			}, {
				name : "updateDate",
				type : "string",
				mapping : "updateDate"
			}, {
				name : 'updatePerson',
				mapping : 'updatePerson',
				convert : function(v) {
					return v != null ? (v.userName) : '';
				}
			}, {
				name : "createDate",
				type : "string",
				mapping : "createDate"
			}, {
				name : 'createPerson',
				mapping : 'createPerson',
				convert : function(v) {
					return v != null ? (v.userName) : '';
				}
			}, {
				name : 'countryAreaCode',
				mapping : 'address',
				convert : function(v) {
					return v != null ? (v.city != null
							? v.city.province.area.country.areaCode
							: '') : '';
				}
			}, {
				name : 'cityAreaCode',
				mapping : 'address',
				convert : function(v) {
					return v != null
							? (v.city != null ? v.city.areaCode : '')
							: '';
				}
			}, {
				name : 'countryAreaCode2',
				mapping : 'address',
				convert : function(v) {
					return v != null ? (v.city != null
							? v.city.province.area.country.areaCode
							: '') : '';
				}
			}, {
				name : 'cityAreaCode2',
				mapping : 'address',
				convert : function(v) {
					return v != null
							? (v.city != null ? v.city.areaCode : '')
							: '';
				}
			}]);

	var rowHome = {
		xtype : 'panel',
		width : 510,
		border : false,
		items : [{
					layout : 'form',
					border : false,
					items : [{
								xtype : 'textfield',
								fieldLabel : '籍贯',
								name : 'nativePlace',
								width : 180
							}]
				}, {
					layout : 'form',
					border : false,
					items : [{
								layout : 'form',
								border : false,
								items : [{
											xtype : 'textfield',
											fieldLabel : '邮政编码',
											regex : /^[0-9]{6}$/,
											invalidText : "邮政编码格式有误，请检查！",
											name : 'zipCode',
											id : 'zipCode',
											width : 180
										}]
							}, {
								layout : 'form',
								border : false,
								items : [{
									xtype : 'textfield',
									fieldLabel : '住宅电话',
									maskRe : new RegExp('['
											+ Ext.escapeRe('0123456789+-_ ()')
											+ ']'),
									// regex : /^\d{7,8}$|^\d{3,4}-\d{7,8}$/,
									invalidText : "住宅电话号码格式有误，请检查！",
									name : 'homeTelNo',
									id : 'homeTelNo',
									width : 180
								}]
							}, {
								layout : 'form',
								width : 200,
								border : false,
								items : [{
											fieldLabel : '婚姻状况',
											xtype : 'radiogroup',
											name : "maritalStatus",
											items : [{
												name : "maritalStatus",
												xtype : 'radio',
												inputValue : 0,// 实际值
												boxLabel : "未婚"// ,//显示值
													// 默认为男
												}, {
												name : "maritalStatus",
												xtype : 'radio',
												inputValue : 1,// 实际值
												boxLabel : "已婚"// 显示值
											}]
										}]
							}, {
								layout : 'form',
								border : false,
								items : [{
											fieldLabel : '配偶姓名',
											xtype : 'textfield',
											name : 'spouseName',
											width : 180
										}]
							}, {
								layout : 'form',
								border : false,
								width : 160,
								items : [{
											xtype : 'radiogroup',
											fieldLabel : '子女性别',
											name : "issueSex",
											items : [{
												name : "issueSex",
												xtype : 'radio',
												inputValue : "1",// 实际值
												boxLabel : "男"// ,//显示值
													// 默认为男
												}, {
												name : "issueSex",
												xtype : 'radio',
												inputValue : "0",// 实际值
												boxLabel : "女"// 显示值
											}]
										}]
							}, {
								layout : 'form',
								border : false,
								items : [{
											xtype : 'textfield',
											fieldLabel : '子女姓名',
											name : 'issueName',
											width : 180
										}]
							}, {
								layout : 'form',
								border : false,
								items : [{
											xtype : 'textarea',
											fieldLabel : '住宅地址',
											name : 'homeAddress',
											width : 328
										}]
							}, {
								layout : 'form',
								border : false,
								items : [{
											xtype : 'textfield',
											fieldLabel : '个人主页',
											name : 'webPage',
											id : 'webPage',
											vtype : "url",
											width : 328
										}]
							}, {
								layout : 'form',
								border : false,
								items : [{
											xtype : 'textfield',
											fieldLabel : '兴趣爱好',
											name : 'hobby',
											width : 328
										}]
							}, {
								layout : 'form',
								border : false,
								items : [{
											xtype : 'textfield',
											fieldLabel : '特长',
											name : 'specialty',
											width : 328
										}]
							}, {
								layout : 'form',
								border : false,
								items : [{
											xtype : 'textarea',
											fieldLabel : '紧急联系人及电话',
											name : 'urgentinfo',
											width : 328
										}]
							}]
				}]
	};
	var imgs = new Ext.ux.ImgsBoxComponent({
		width : 115, // 图片宽度
		style : 'padding :0 0 0 0; margin: 0px 0 0px 8px ;!important',
		height : 135
			// 图片高度
		});
	var imgsCard = new Ext.ux.ImgsBoxComponent({
		width : 275, // 图片宽度
		style : 'margin:10px 10px 0 10px;!important',
		picName : 'postCard.png',
		height : 175
			// 图片高度
		});
	var imgsCard2 = new Ext.ux.ImgsBoxComponent({
		width : 275, // 图片宽度
		style : 'margin:10px 10px 0 10px;!important',
		picName : 'postCard.png',
		height : 175
			// 图片高度
		});

	var allAdressStoreRecord = Ext.data.Record.create([{
				"mapping" : "addressID",
				"name" : "addressID",
				"type" : "string"
			}, {
				"mapping" : "zipCode",
				"name" : "zipCode",
				"type" : "int"
			}, {
				"mapping" : "addressInfo",
				"name" : "addressInfo",
				"type" : "string"
			}, {
				"mapping" : "district",
				"name" : "district",
				"type" : "string"
			}, {
				"mapping" : "city",
				"name" : "city",
				"type" : "auto"
			}]);

	var allAdressStore = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/crmMaint.do?method=getAllAddressList',
							method : "POST"
						}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, allAdressStoreRecord),
				remoteSort : true
			});

	var allAdressCm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
				header : 'addressID',
				hidden : true,
				dataIndex : 'addressID'
			}, {
				header : '<font color="#003366"><b>地址</b></font>',
				width : 382 + 40,
				dataIndex : 'city',
				renderer : function(city, cellmeta, record) {
					if (city != null) {
						var isTrue = (city.province.provinceName.trim()) == (city.cityName
								.trim());
						var showname = '';
						if (isTrue) {
							showname = city.province.provinceName
									+ record.data.district
									+ record.data.addressInfo
									+ (record.data.zipCode == 0 ? '' : " 邮编："
											+ record.data.zipCode);
						} else {
							showname = city.province.provinceName
									+ city.cityName
									+ record.data.district
									+ record.data.addressInfo
									+ (record.data.zipCode == 0 ? '' : " 邮编："
											+ record.data.zipCode);
						}
						return showname;
					} else {
						return ''
					}
				}
			}]);

	var allAdressGrid = new Ext.grid.GridPanel({
				region : 'north',
				forceFit : true,
				width : 447,
				height : 200,
				border : false,
				stripeRows : true,
				cm : allAdressCm,
				// collapsed : true,
				store : allAdressStore
			});
	// 相关地址的grid添加rowclick事件
	allAdressGrid.addListener('rowclick', function() {
		var rec = allAdressGrid.getSelectionModel().getSelected();

		Ext.getCmp('district').setValue(rec.get('district'));
		Ext.getCmp('addressInfo').setValue(rec.get('addressInfo'));
		Ext.getCmp('zipCodeAddress').setValue(rec.get('zipCode') == 0
				? ''
				: rec.get('zipCode'));
		var isTrue = (rec.get('city.province.provinceName')) == (rec
				.get('city.cityName'));
		var showname = '';
		if (isTrue) {
			showname = rec
					.get('city.province.area.country.internation.internationName')
					+ '·'
					+ rec.get('city.province.area.country.countryName')
					+ '·'
					+ rec.get('city.province.area.areaName')
					+ '·'
					+ rec.get('city.province.provinceName');
		} else {
			showname = rec
					.get('city.province.area.country.internation.internationName')
					+ '·'
					+ rec.get('city.province.area.country.countryName')
					+ '·'
					+ rec.get('city.province.area.areaName')
					+ '·'
					+ rec.get('city.province.provinceName')
					+ '·'
					+ rec.get('city.cityName');
		}
		Ext.getCmp('chooseAddress').setValue(showname);

		Ext.getCmp('internationID').setValue(rec
				.get('city.province.area.country.internation.internationID'));
		Ext.getCmp('countryID').setValue(rec
				.get('city.province.area.country.countryID'));
		Ext.getCmp('areaID').setValue(rec.get('city.province.area.areaID'));
		Ext.getCmp('provinceID').setValue(rec.get('city.province.provinceID'));
		Ext.getCmp('cityID').setValue(rec.get('city.cityID'));

		// 單位電話 的國際區號 地區區號 传真的国际区号、城市区号
		var countryCode = rec.get('city.province.area.country.areaCode');
		var cityCode = rec.get('city.areaCode');
		Ext.getCmp('countryAreaCode').setValue(countryCode);
		Ext.getCmp('cityAreaCode').setValue(cityCode);
		Ext.getCmp('countryAreaCode2').setValue(countryCode);
		Ext.getCmp('cityAreaCode2').setValue(cityCode);

		addressIDParam = Ext.getCmp('addressID').getValue();
		cityIDParam = Ext.getCmp('cityID').getValue();
		zipCodeAddressParam = Ext.getCmp('zipCodeAddress').getValue();
		politicalStatusParam = Ext.getCmp('politicalStatus').getValue();

		allAdressWin.hide();

		Ext.getCmp("btn_selectAddress2").setVisible(true);

	});

	var allAdressWin = new Ext.Window({
				title : '相关地址信息',
				closable : true,
				closeAction : 'hide',
				width : 464,
				height : 260,
				plain : true,
				modal : true,
				layout : 'fit',
				bbar : ['<font color="#003366">*单击地址名称，选用此地址为当前办公地址。</font>'],
				// x:660,
				// y:200,
				items : [allAdressGrid]
			})

	// 办公地址的form
	// new Ext.form.FormPanel(
	var addressPanel = {
		xtype : 'panel',
		layout : 'form',
		border : false,
		width : '420',
		items : [{
			xtype : 'fieldset',
			title : '办公地址',
			layout : 'form',
			defaultType : 'textfield',
			items : [{
				xtype : 'panel',
				width : 180,
				layout : 'column',
				style : 'padding-left:5px;padding-bottom:5px;',
				border : false,
				items : [{
					layout : 'form',
					xtype : 'button',
					text : '<font color="#003366"><b>相关地址列表</b></font>',
					style : 'padding-bottom:5px;',
					ref : '../../btn_selectAddress',
					id : 'btn_selectAddress',
					iconCls : 'application_put',
					columnWidth : .6,
					handler : function() {
						allAdressStore.load({
									params : {
										companyId : compID,
										start : 0,
										limit : 20
									}
								});
						// allAdressGrid.store.reload();// 保证数据是最新数据
						allAdressWin.show();

						if (!Ext.getCmp("btn_selectAddress2").isVisible()) {
							// 将之前的内容放入临时字段中
							Ext.getCmp('district2').setValue(Ext
									.getCmp('district').getValue());
							Ext.getCmp('addressInfo2').setValue(Ext
									.getCmp('addressInfo').getValue());
							Ext.getCmp('zipCodeAddress2').setValue(Ext
									.getCmp('zipCodeAddress').getValue());
							Ext.getCmp('chooseAddress2').setValue(Ext
									.getCmp('chooseAddress').getValue())

							Ext.getCmp('internationID2').setValue(Ext
									.getCmp('internationID').getValue())
							Ext.getCmp('countryID2').setValue(Ext
									.getCmp('countryID').getValue())
							Ext.getCmp('areaID2').setValue(Ext.getCmp('areaID')
									.getValue())
							Ext.getCmp('provinceID2').setValue(Ext
									.getCmp('provinceID').getValue())
							Ext.getCmp('cityID2').setValue(Ext.getCmp('cityID')
									.getValue())

						};
					}
				}, {
					layout : 'form',
					xtype : 'button',
					text : '<font color="#003366"><b>取消</b></font>',
					style : 'width:52px;padding-left:10px;',
					ref : '../../btn_selectAddress2',
					id : 'btn_selectAddress2',
					iconCls : 'remove',
					hidden : true,
					columnWidth : .4,
					handler : function() {
						Ext.getCmp('district').setValue(Ext.getCmp('district2')
								.getValue());
						Ext.getCmp('addressInfo').setValue(Ext
								.getCmp('addressInfo2').getValue());
						Ext.getCmp('zipCodeAddress').setValue(Ext
								.getCmp('zipCodeAddress2').getValue());
						Ext.getCmp('chooseAddress').setValue(Ext
								.getCmp('chooseAddress2').getValue())

						Ext.getCmp('internationID').setValue(Ext
								.getCmp('internationID2').getValue())
						Ext.getCmp('countryID').setValue(Ext
								.getCmp('countryID2').getValue())
						Ext.getCmp('areaID').setValue(Ext.getCmp('areaID2')
								.getValue())
						Ext.getCmp('provinceID').setValue(Ext
								.getCmp('provinceID2').getValue())
						Ext.getCmp('cityID').setValue(Ext.getCmp('cityID2')
								.getValue())

						Ext.getCmp("btn_selectAddress2").setVisible(false);

					}
				}]
			}, new Ext.ux.form.AddressChooseField({
						fieldLabel : '<font color="#CC0000">*</font>城市选择',
						id : 'chooseAddress',
						name : 'chooseAddress',
						anchor : '96%',
						editable : false,
						blankText : '该输入项为必输项',
						msgTarget : 'under',
						allowBlank : false,
						setValueFun : function(name, ids, areaCodes) {
							Ext.getCmp('chooseAddress').setValue(name);
							Ext.getCmp('internationID')
									.setValue(ids.internationID);
							Ext.getCmp('countryID').setValue(ids.countryID);
							Ext.getCmp('areaID').setValue(ids.areaID);
							Ext.getCmp('provinceID').setValue(ids.provinceID);
							Ext.getCmp('cityID').setValue(ids.cityID);

							addressIDParam = Ext.getCmp('addressID').getValue();
							cityIDParam = Ext.getCmp('cityID').getValue();

							// 單位電話 的國際區號 地區區號 传真的国际区号、城市区号
							var place = areaCodes.indexOf(',');
							var countryCode = areaCodes.substring(0, place);
							var cityCode = areaCodes.substring(place + 1,
									areaCodes.length);
							Ext.getCmp('countryAreaCode').setValue(countryCode);
							Ext.getCmp('cityAreaCode').setValue(cityCode);
							Ext.getCmp('countryAreaCode2')
									.setValue(countryCode);
							Ext.getCmp('cityAreaCode2').setValue(cityCode);

							zipCodeAddressParam = Ext.getCmp('zipCodeAddress')
									.getValue();
							politicalStatusParam = Ext
									.getCmp('politicalStatus').getValue();
						},
						getValueFun : function() {
							var a, b, c, d, e;

							a = Ext.getCmp('internationID').getValue();

							b = Ext.getCmp('countryID').getValue();

							c = Ext.getCmp('areaID').getValue();

							d = Ext.getCmp('provinceID').getValue();

							e = Ext.getCmp('cityID').getValue();

							return [a, b, c, d, e];
						},
						clearValueFun : function() {
							Ext.getCmp('chooseAddress').setValue('');

							Ext.getCmp('internationID').setValue('');

							Ext.getCmp('countryID').setValue('');

							Ext.getCmp('areaID').setValue('');

							Ext.getCmp('provinceID').setValue('');

							Ext.getCmp('cityID').setValue('');
						}
					}), {
				xtype : 'textfield',
				fieldLabel : '区县',
				name : 'district',
				id : 'district',
				allowBlank : true,
				anchor : '96%'
			}, {
				xtype : 'textfield',
				fieldLabel : '邮编',
				maskRe : new RegExp('[' + Ext.escapeRe('0123456789') + ']'),
				regex : /^[1-9]\d{5}$/,
				name : 'zipCodeAddress',
				id : 'zipCodeAddress',
				allowBlank : true,
				anchor : '96%'
			}, {
				xtype : 'textarea',
				fieldLabel : '详细地址',
				name : 'addressInfo',
				id : 'addressInfo',
				allowBlank : true,
				anchor : '96%'
			}, {
				xtype : 'hidden',
				fieldLabel : '地址ID',
				name : 'addressID',
				id : 'addressID',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '洲际ID',
				name : 'internationID',
				id : 'internationID',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '国家ID',
				name : 'countryID',
				id : 'countryID',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '地区ID',
				name : 'areaID',
				id : 'areaID',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '省ID',
				name : 'provinceID',
				id : 'provinceID',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '市ID',
				name : 'cityID',
				id : 'cityID',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '洲际ID2',
				name : 'internationID2',
				id : 'internationID2',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '国家ID2',
				name : 'countryID2',
				id : 'countryID2',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '地区ID2',
				name : 'areaID2',
				id : 'areaID2',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '省ID2',
				name : 'provinceID2',
				id : 'provinceID2',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '市ID2',
				name : 'cityID2',
				id : 'cityID2',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : 'chooseAddress2',
				name : 'chooseAddress2',
				id : 'chooseAddress2',
				allowBlank : true
			}, {
				xtype : 'hidden',
				fieldLabel : '区县2',
				name : 'district2',
				id : 'district2',
				allowBlank : true,
				anchor : '96%'
			}, {
				xtype : 'hidden',
				fieldLabel : '邮编2',
				name : 'zipCodeAddress2',
				id : 'zipCodeAddress2',
				allowBlank : true,
				anchor : '96%'
			}, {
				xtype : 'hidden',
				fieldLabel : '详细地址2',
				name : 'addressInfo2',
				id : 'addressInfo2',
				allowBlank : true,
				anchor : '96%'
			}

			]

		}]
	};

	/**
	 * 附件列表
	 */
	var attFileGrid = new com.bitc.xmis.AttachmentList({
				title : '其他相关文件',
				stripeRows : true,
				REFID : refid_empID,
				attachmentTypeID : com.bitc.xmis.Attachment.staffInfo,
				height : 100
			});
	var rowWork = {
		xtype : 'panel',
		height : (isAdmin || isDirectDeptAdmin) ? 950 : 930,
		border : false,
		items : [{
			layout : 'column', // 从左往右布局
			xtype : 'panel',
			border : false,
			width : 1000,
			items : [{
						xtype : 'hidden',
						name : 'userid',
						id : 'userid'
					}, {
						xtype : 'hidden',
						name : 'disabledReasonID',
						id : 'disabledReasonID'
					}, {
						xtype : 'hidden',
						name : 'imgName',
						id : 'image'
					}, {
						xtype : 'hidden',
						name : 'postCardName',
						id : 'postCard'
					}, {
						xtype : 'hidden',
						name : 'postCardBackName',
						id : 'postCardBack'
					}, {
						xtype : 'hidden',
						name : 'password',
						id : 'password'
					}, {
						xtype : 'hidden',
						name : 'pathId',
						id : 'pathId'
					}, {
						xtype : 'hidden',
						name : 'eduID',
						id : 'eduID'
					}, {
						xtype : 'hidden',
						name : 'oldEduLevel',
						id : 'oldEduLevel'
					}, {
						xtype : 'hidden',
						name : 'oldSchoolName',
						id : 'oldSchoolName'
					}, {
						xtype : 'hidden',
						name : 'oldGraduationDate',
						id : 'oldGraduationDate'
					}, {
						xtype : 'hidden',
						name : 'oldMajor',
						id : 'oldMajor'
					}, {
						xtype : 'hidden',
						name : 'oldEduRemark',
						id : 'oldEduRemark'
					}, {
						xtype : 'hidden',
						name : 'polStat',
						id : 'polStat'
					}, {
						xtype : 'hidden',
						name : 'birth',
						id : 'birth'
					}, {
						xtype : 'hidden',
						name : 'join',
						id : 'join'
					}, {
						xtype : 'hidden',
						name : 'positionID',
						id : 'positionID'
					}, {
						xtype : 'hidden',
						name : 'postName',
						id : 'postName'
					}, {// 第一列
						columnWidth : .44,
						layout : 'form',
						xtype : 'panel',
						border : false,
						items : [{
							layout : 'form',
							border : false,
							items : [{
								xtype : 'textfield',
								fieldLabel : '<font color ="#CC0000">◎</font>姓',
								name : 'lastName',
								id : 'lastName',
								readOnly : (isAdmin || isDirectDeptAdmin)
										? false
										: (compID == 1 ? true : false),
								allowBlank : false,
								width : 180
							}]
						}, {
							layout : 'form',
							border : false,
							items : [{
								xtype : 'textfield',
								fieldLabel : '<font color ="#CC0000">◎</font>名',
								name : 'firstName',
								id : 'firstName',
								readOnly : (isAdmin || isDirectDeptAdmin)
										? false
										: (compID == 1 ? true : false),
								allowBlank : false,
								width : 180
							}]
						}, {
							layout : 'form',
							border : false,
							items : [{
								xtype : 'textfield',
								fieldLabel : '登录名',
								name : 'loginName',
								id : 'loginName',
								hidden : (compID == 1 ? false : true),
								listeners : {
									'blur' : function(t) {
										var newValue = t.getValue();
										if (compID != 1) {
											Ext.getCmp("loginName")
													.setValue(new Date()
															.getTime());
										}
										if (newValue.trim() == "") {
											t.markInvalid('该输入项为必输项');
										} else {
											Ext.Ajax.request({
												url : __cxtPath
														+ '/orgMaint.do',
												success : function(response,
														config) {
													var action = Ext
															.decode(response.responseText);
													if (!action.success) {
														t
																.markInvalid(action.message);
													}

												},
												failure : function() {
												},
												method : "post",
												params : {
													method : 'checkLoginNameExist',
													loginName : newValue,
													userid : Ext.get("userid")
															.getValue()
												}
											});
										}
									}
								},
								width : 180
							}]

						}, {
							xtype : 'radiogroup',
							fieldLabel : '<font color ="#CC0000">◎</font>性别',
							name : 'sex',
							id : 'sex',
							width : 100,
							disabled : (isAdmin || isDirectDeptAdmin)
									? false
									: (compID == 1 ? true : false),
							items : [{
								name : "sex",
								xtype : 'radio',
								border : false,
								inputValue : 1,// 实际值
								boxLabel : "男",// 显示值
								checked : true
									// 默认为男
								}, {
								name : "sex",
								xtype : 'radio',
								border : false,
								inputValue : 0,// 实际值
								boxLabel : "女"// 显示值
							}]

						}, {
							xtype : 'datefield',
							format : 'Y-m-d',
							name : 'birthday',
							id : 'birthday',
							maxValue : new Date(),
							minValue : '1900-01-01',
							fieldLabel : '<font color ="#CC0000">◎</font>出生日期',
							editable : false,
							readOnly : (isAdmin || isDirectDeptAdmin)
									? false
									: (compID == 1 ? true : false),
							width : 180,
							listeners : {
								change : function(birth, newValue, oldValue) {
									var year = new Date().getYear()
											- newValue.format('Y') >= 0
											? new Date().getYear()
													- newValue.format('Y')
											: 1900 - newValue.format('Y')
													+ new Date().getYear();
									Ext.getCmp("mainForm").getForm()
											.findField("age").setValue(year
													+ "周岁");
								}
							}
						}, {
							xtype : 'displayfield',
							name : 'age',
							id : 'age',
							fieldLabel : '年龄',
							width : 180
						}, {
							xtype : 'combo',
							fieldLabel : '<font color ="#CC0000">◎</font>政治面貌',
							emptyText : '请选择政治面貌',
							editable : false,
							name : 'politicalStatus',
							id : 'politicalStatus',
							triggerAction : "all",
							store : politicalStatusStore,
							displayField : "Name",
							valueField : "ID",
							readOnly : (isAdmin || isDirectDeptAdmin)
									? false
									: (compID == 1 ? true : false),
							mode : "remote",
							width : 180
						}, {
							xtype : 'displayfield',
							fieldLabel : '最高学历',
							name : 'educationHighestLevel',
							id : 'educationHighestLevel',
							width : 180
						}, {
							xtype : 'textfield',
							fieldLabel : '<font color ="#CC0000">◎</font>身份证',
							name : 'IDCard',
							id : 'IDCard',
							regex : /(^\d{15}$)|(^\d{17}([0-9]|X)$)/,
							invalidText : "身份证格式有误，请检查！",
							readOnly : (isAdmin || isDirectDeptAdmin)
									? false
									: (compID == 1 ? true : false),
							width : 180
						}, {
							xtype : 'displayfield',
							fieldLabel : '所属单位',
							name : 'companyName',
							id : 'companyName',
							width : 328
						}, {
							xtype : 'displayfield',
							fieldLabel : '部门',
							name : 'deptName',
							id : 'deptName',
							width : 180
						}, addressPanel, {
							xtype : 'displayfield',
							fieldLabel : '职务',// (中水)
							hidden : (compID == 1 ? false : true),
							name : 'position',
							id : 'position',
							width : 180
						}, {
							xtype : 'textfield',
							fieldLabel : '职务',// (非中水)
							name : 'positionEdit',
							hidden : (compID == 1 ? true : false),
							id : 'positionEdit',
							width : 180
						}, {
							xtype : 'datefield',
							format : 'Y-m-d',
							name : 'joinDate',
							id : 'joinDate',
							fieldLabel : '<font color ="#CC0000">◎</font>入职时间',
							editable : false,
							readOnly : (isAdmin || isDirectDeptAdmin)
									? false
									: (compID == 1 ? true : false),
							width : 180
						}, {
							xtype : 'textfield',
							fieldLabel : '手机号码',
							name : 'mobileNo',
							id : 'mobileNo',
							regex : /^(1[3458]{1}[0-9]{9})$/,
							invalidText : "手机号码格式有误，请检查！",
							width : 180

						}, {
							// xtype : 'textfield',
							// fieldLabel : '单位电话',
							// name : 'switchboardNo',
							// id : 'switchboardNo',
							// maskRe : new RegExp('[' +
							// Ext.escapeRe('0123456789+-_ ()')
							// + ']'),
							// // regex : /^\d{7,8}$|^\d{3,4}-\d{7,8}$/,
							// invalidText : "单位电话号码格式有误，请检查！",
							// width : 180

							// } , {

							xtype : 'panel',
							border : false,
							layout : 'hbox',
							fieldLabel : '单位电话',
							items : [{
										xtype : 'displayfield',
										name : 'countryAreaCode',
										id : 'countryAreaCode',
										ref : 'countryAreaCode',
										emptyText : '国际区号',
										readOnly : true,
										width : 32,
										height : 22,
										allowBlank : true
									}, {
										xtype : 'displayfield',
										value : '-'
									}, {
										xtype : 'displayfield',
										name : 'cityAreaCode',
										ref : 'cityAreaCode',
										id : 'cityAreaCode',
										margins : '0 0 0 5',
										readOnly : true,
										emptyText : '地区号码',
										width : 32,
										height : 22,
										allowBlank : true
									}, {
										xtype : 'displayfield',
										value : '-'
										// }, {
									// xtype : 'numberfield',
									// name : 'switchboardNo',
									// emptyText : '请输入电话号码',
									// invalidText : "单位电话号码格式有误，请检查！",
									// id : 'switchboardNo',
									// margins : '0 0 0 5',
									// width : 98,
									// height : 22,
									// allowBlank : true
								}	, {
										xtype : 'textfield',
										name : 'switchboardNo',
										emptyText : '请输入电话号码',
										invalidText : "单位电话号码格式有误，请检查！",
										id : 'switchboardNo',
										margins : '0 0 0 5',
										maskRe : new RegExp('['
												+ Ext
														.escapeRe('0123456789+-_ ()')
												+ ']'),
										width : 98,
										height : 22,
										allowBlank : true
									}]
						}, {
							xtype : 'textfield',
							fieldLabel : '分机号码',
							name : 'extNo',
							id : 'extNo',
							width : 180
								// }, {
								// xtype : 'textfield',
								// fieldLabel : '传真号码',
								// name : 'faxNo',
								// id : 'faxNo',
								// maskRe : new RegExp('[' +
								// Ext.escapeRe('0123456789+-_ ()')
								// + ']'),
								// //regex : /^[+]{0,1}(\d){1,3}[ ]?([-]?((\d)|[
								// ]){1,12})+$/,
								// width : 180
							}, {
							xtype : 'panel',
							border : false,
							layout : 'hbox',
							fieldLabel : '传真号码',
							items : [{
										xtype : 'displayfield',
										name : 'countryAreaCode2',
										id : 'countryAreaCode2',
										emptyText : '国际区号',
										readOnly : true,
										width : 32,
										height : 22,
										allowBlank : true
									}, {
										xtype : 'displayfield',
										value : '-'
									}, {
										xtype : 'displayfield',
										name : 'cityAreaCode2',
										id : 'cityAreaCode2',
										emptyText : '地区号码',
										readOnly : true,
										width : 32,
										margins : '0 0 0 5',
										height : 22,
										allowBlank : true
									}, {
										xtype : 'displayfield',
										value : '-'
									}, {
										xtype : 'textfield',
										id : 'faxNo',
										name : 'faxNo',
										emptyText : '请输入传真号码',
										width : 98,
										margins : '0 0 0 5',
										maskRe : new RegExp('['
												+ Ext
														.escapeRe('0123456789+-_ ()')
												+ ']'),
										height : 22,
										allowBlank : true
									}]
						}, {
							layout : 'column',
							xtype : 'panel',
							border : false,
							width : 360,
							items : [{
										columnWidth : .7,
										layout : 'form',
										border : false,
										items : [{
													xtype : 'textfield',
													fieldLabel : 'Email',
													name : 'email',
													id : 'email',
													vtype : "email",
													width : 180
												}]
									}, {
										columnWidth : .3,
										layout : 'form',
										border : false,
										items : [{
											layout : 'form',
											xtype : 'button',
											text : '<font color="#003366"><b>发送email</b></font>',
											ref : '../../btn_email',
											id : 'btn_email',
											iconCls : 'email',
											handler : function() {
												if ("" != Ext.getCmp("email")
														.getValue()
														&& Ext.getCmp("email")
																.isValid()) {
													document.location = "mailto:"
															+ Ext
																	.getCmp("email")
																	.getValue();
												} else {
													Ext.Msg.alert("提示",
															"请先输入邮箱地址！");
												}
											}
										}]
									}]
						}, {
							xtype : 'displayfield',
							hidden : true,
							fieldLabel : '控制文件',
							width : 180

								// 控制文件 和禁用
							}, {
							xtype : 'fieldset',
							title : '禁用管理',
							hidden : (isAdmin || isDirectDeptAdmin || compID != 1)
									? false
									: true,
							items : [{
								xtype : 'radiogroup',
								fieldLabel : ((isAdmin || isDirectDeptAdmin) || compID != 1)
										? '禁用'
										: '',
								width : 100,
								name : "disabledFlag",
								id : "disabledFlag",
								items : [{
									name : "disabledFlag",
									xtype : 'radio',
									inputValue : 0,// 实际值
									boxLabel : "否",// 显示值默认为否
									listeners : {
										focus : function(ra) {
											var mydivisionID = Ext
													.get('pathId').getValue();// 查看人的事业部ID

											var win = new Ext.Window({
												layout : 'form',
												modal : true,
												title : "解除禁用",
												resizable : false,
												buttonAlign : "center",
												closable : true,
												bodyStyle : "padding:10px",
												width : 360,
												height : 180,
												minWidth : 360,
												minHeight : 180,
												items : [{
													xtype : 'form',
													frame : true,
													ref : 'form',
													baseCls : 'x-plain',
													items : [{
														xtype : 'textarea',
														fieldLabel : '备注',
														emptyText : '输入解除此人禁用的原因',
														width : 200,
														name : 'disableReason'
													}]
												}],
												buttons : [{
													text : "确认",
													disabled : (isAdmin || (isDirectDeptAdmin && mydivisionID == __emp.divisionID))
															? false
															: true,
													handler : function() {
														win.form.getForm()
																.submit({
																	url : __cxtPath
																			+ '/orgMaint.do?method=updateDisableReasonAndDate',
																	method : 'post',
																	params : {
																		empId : Ext
																				.get("userid")
																				.getValue(),
																		disable : '0',
																		reason : win.form
																				.getForm()
																				.findField('disableReason')
																				.getValue()
																	},
																	waitMsg : '正在保存，请稍候……',
																	waitTitle : '正在处理',
																	success : function(
																			response,
																			action) {
																		Ext
																				.getCmp('disabledFlag')
																				.setValue('0');
																		Ext
																				.getCmp('disabledDate')
																				.setValue(action.result.attributes.reasonDate);
																		Ext
																				.getCmp('disabledReason')
																				.setValue(win.form
																						.getForm()
																						.findField('disableReason')
																						.getValue());
																		Ext.Msg
																				.alert(
																						action.result.messageTitle,
																						action.result.message);
																		treeloader
																				.load(
																						west
																								.getRootNode(),
																						function(
																								n) {
																							west
																									.expandPath(currentNodePath);
																						});
																		win
																				.close();
																	},
																	failure : function(
																			response,
																			action) {
																		Ext.MessageBox
																				.hide();
																		Ext.MessageBox
																				.alert(
																						"提示",
																						"操作失败！");
																	}
																});

													}
												}, {
													text : "取消",
													handler : function() {
														win.close();
													}
												}]
											}).show();

											if (Ext.getCmp('disabledFlag')
													.getValue() == 0) {
												win.form
														.getForm()
														.findField('disableReason')
														.setValue(Ext
																.getCmp('disabledReason')
																.getValue())
											}
										}
									}
								}, {
									name : "disabledFlag",
									xtype : 'radio',
									inputValue : 1,// 实际值
									boxLabel : "是",// 显示值
									listeners : {
										focus : function(ra) {
											var mydivisionID = Ext
													.get('pathId').getValue();// 查看人的事业部ID
											var win = new Ext.Window({
												layout : 'form',
												modal : true,
												title : "禁用原因",
												resizable : false,
												buttonAlign : "center",
												closable : true,
												bodyStyle : "padding:10px",
												frame : true,
												width : 360,
												height : 180,
												minWidth : 360,
												minHeight : 180,
												border : false,
												items : [{
													xtype : 'form',
													frame : true,
													ref : 'form',
													baseCls : 'x-plain',
													items : [new com.bitc.xmis.Combos.DisabledReasonCombo(
															{
																fieldLabel : '选择禁用原因',
																name : 'disableReason'
															})]
												}],
												// items : [new
												// Ext.FormPanel({
												// ref : 'form',
												// region : 'center',
												// frame : true,
												// labelAlign : 'right',
												// bodyStyle :
												// 'padding:10px;',
												// border : false,
												// items : [new
												// com.bitc.xmis.Combos.CreditLevelCombo(
												// {
												// fieldLabel : '选择禁用原因',
												// name : 'disableReason'
												// })]
												// })],
												buttons : [{
													text : "确认",
													disabled : (isAdmin ||compID!=1|| (isDirectDeptAdmin && mydivisionID == __emp.divisionID))
															? false
															: true,
													handler : function() {
														if (win.form
																.getForm()
																.findField('disableReason')
																.getValue() == null
																|| win.form
																		.getForm()
																		.findField('disableReason')
																		.getValue() == '') {
															Ext.MessageBox
																	.alert(
																			"提示",
																			"请选择禁用原因!");
															return;
														}
														win.form.getForm()
																.submit({
																	url : __cxtPath
																			+ '/orgMaint.do?method=updateDisableReasonAndDate',
																	method : 'post',
																	params : {
																		empId : Ext
																				.get("userid")
																				.getValue(),
																		disable : '1',
																		reason : win.form
																				.getForm()
																				.findField('disableReason')
																				.getValue()
																	},
																	waitMsg : '正在保存，请稍候……',
																	waitTitle : '正在处理',
																	success : function(
																			form,
																			action) {
																		Ext
																				.getCmp('disabledReason')
																				.setValue(action.result.attributes.reasonName);
																		Ext
																				.getCmp('disabledDate')
																				.setValue(action.result.attributes.reasonDate);
																		Ext
																				.getCmp('disabledFlag')
																				.setValue('1');
																		Ext
																				.getCmp('disabledReasonID')
																				.setValue(action.result.attributes.reasonID);
																		Ext.Msg
																				.alert(
																						action.result.messageTitle,
																						action.result.message);
																		treeloader
																				.load(
																						west
																								.getRootNode(),
																						function(
																								n) {
																							west
																									.expandPath(currentNodePath);
																						});

																		win
																				.close();

																	},
																	failure : function(
																			form,
																			action) {
																		Ext.Msg
																				.alert(
																						action.result.messageTitle,
																						action.result.message);

																	}

																});
													}
												}, {
													text : "取消",
													handler : function() {
														win.close();
													}
												}]
											}).show();

											if (Ext.getCmp('disabledFlag')
													.getValue() == 1) {
												win.form
														.getForm()
														.findField('disableReason')
														.setValue(Ext
																.getCmp('disabledReasonID')
																.getValue())
											}

										}
									}
								}]
							}, {
								xtype : 'displayfield',
								fieldLabel : '变更原因',
								emptyText : '',
								width : 200,
								id : 'disabledReason',
								name : 'disabledReason'
							}, {
								xtype : 'displayfield',
								fieldLabel : '变更日期',
								emptyText : '',
								format : 'Y-m-d',
								width : 200,
								id : 'disabledDate',
								name : 'disabledDate'
							}]
						}, {
							xtype : 'textarea',
							fieldLabel : '附注',
							name : 'remark',
							width : 328
						}, {
							xtype : 'panel',
							layout : 'column',
							width : 450,
							border : false,
							items : [{
										xtype : 'panel',
										layout : 'form',
										columnWidth : .5,
										labelAlign : 'right',
										border : false,
										labelWidth : 90,
										items : [{
													xtype : 'hidden',
													name : 'updatePersonId'
												}, {
													xtype : 'displayfield',
													fieldLabel : '最后修改人',
													name : 'updatePerson',
													value : '',
													anchor : '100%'
												}]
									}, {
										xtype : 'panel',
										layout : 'form',
										border : false,
										columnWidth : .5,
										labelAlign : 'right',
										labelWidth : 90,
										items : [{
													xtype : 'hidden',
													name : 'updateDateId'
												}, {
													xtype : 'displayfield',
													fieldLabel : '最后修改日期',
													name : 'updateDate',
													id : 'updateDate',
													value : '',
													anchor : '100%'
												}]
									}]
						}]
					}, {
						// 第二列
						columnWidth : .44,
						style : 'padding-left:100px;',
						border : false,
						items : [{
							// xtype : 'panel',
							// layout : 'form',
							// border : false,
							// style : 'padding-left:90px;',
							// items : [{}] 头像右对齐时候会用
							// }, {
							xtype : 'fieldset',
							title : '照片预览',
							name : 'nickName',
							id : 'nickName',
							style : 'padding :5px 3px 5px 3px; margin:0 0 0 0px ;!important',
							height : 167,
							width : 140,
							items : [imgs]
						}, {
							xtype : 'uploadImgButton',
							EMPIDFun : function() {
								return Ext.get('userid').getValue();
							},
							IMGName : imgs,
							ISPOSTCARD : '',
							reuses : false,
							text : '上传头像',
							id : 'imgs_btn',
							style : 'padding:10px 5px 0px 32px;',
							iconCls : 'folder_image'

							}, {
							xtype : 'panel',
							style : 'margin-top:50px;',
							layout : 'auto',
							width : 300,
							items : new Ext.TabPanel({
								frame : false,
								id : 'postCardTab',
								border : false,
								activeTab : 0,
								items : [{
									title : "名片正面",
									xtype : 'panel',
									height : 230,
									items : [imgsCard, {
										xtype : 'panel',
										layout : 'column',
										border : false,
										items : [{
											xtype : 'uploadImgButton',
											style : 'margin : 10px 0px 0px 70px',
											EMPIDFun : function() {
												return Ext.get('userid')
														.getValue();
											},
											IMGName : imgsCard,
											ISPOSTCARD : 'front',
											reuses : false,
											id : 'postCard_btn',
											text : '上传',
											iconCls : 'save'
										}, {
											xtype : 'button',
											id : 'postCardDel_btn',
											style : 'margin : 10px 0px 0px 60px',
											text : '删除',
											handler : function() {
												Ext.MessageBox.confirm("提示",
														"是否删除?", function(btn) {
															if (btn == 'yes') {
																Ext.Ajax
																		.request(
																				{
																					url : __cxtPath
																							+ '/orgMaint.do?method=updatePostCard',
																					success : function(
																							response,
																							action) {
																						Ext.MessageBox
																								.alert(
																										"提示",
																										"操作成功！");
																						imgsCard
																								.setSrc(__cxtPath
																										+ '/resources/images/020xx.png');
																					},
																					failure : function(
																							response,
																							action) {
																						Ext.MessageBox
																								.alert(
																										"提示",
																										"操作失败！");
																					},
																					method : "post",
																					params : {
																						empId : Ext
																								.get("userid")
																								.getValue(),
																						isFront : 'true'
																					}
																				});

															}

														});
											}
										}]
									}]
								}, {
									title : "名片背面",
									height : 230,
									xtype : 'panel',
									items : [imgsCard2, {
										xtype : 'panel',
										layout : 'column',
										border : false,
										items : [{
											xtype : 'uploadImgButton',
											EMPIDFun : function() {
												return Ext.get('userid')
														.getValue();
											},
											IMGName : imgsCard2,
											id : 'postCardBack_btn',
											ISPOSTCARD : 'back',
											reuses : false,
											text : '上传',
											style : 'margin : 10px 0px 0px 70px',
											iconCls : 'save'
										}, {
											xtype : 'button',
											text : '删除',
											id : 'postCardBackDel_btn',
											style : 'margin : 10px 0px 0px 60px',
											handler : function() {
												Ext.MessageBox.confirm("提示",
														"是否删除?", function(btn) {
															if (btn == 'yes') {
																Ext.Ajax
																		.request(
																				{
																					url : __cxtPath
																							+ '/orgMaint.do?method=updatePostCard',
																					success : function(
																							response,
																							action) {
																						Ext.MessageBox
																								.alert(
																										"提示",
																										"操作成功！");
																						imgsCard2
																								.setSrc(__cxtPath
																										+ '/resources/images/020xx.png');
																					},
																					failure : function(
																							response,
																							action) {
																						Ext.MessageBox
																								.alert(
																										"提示",
																										"操作成功！");
																					},
																					method : "post",
																					params : {
																						empId : Ext
																								.get("userid")
																								.getValue(),
																						isFront : 'false'
																					}
																				});

															}
														});
											}
										}]

									}]
								}]
							})
						}, {
							xtype : 'fieldset',
							title : '相关文件列表',
							style : 'margin-top:50px;',
							layout : 'auto',
							width : 300,
							items : [attFileGrid]

						}]
					}]
		}]

	};
	// tabPanel
	var center = new Ext.TabPanel({
		hidden : true,
		deferredRender : false,
		border : false,
		activeTab : 0,
		width : wd - 208,
		height : (ht - 36),
		listeners : {
			tabchange : function(tab, p) {
				p.doLayout();
			}
		},
		items : [{
			title : "工作信息",
			// layout:'auto',
			xtype : 'panel',
			layout : 'border',
			tbar : {
				xtype : 'toolbar',
				id : 'tbar1',
				items : [
						{
							xtype : 'button',
							text : '保存',
							id : 'saveUserInfo',
							iconCls : 'save',
							handler : function() {
								Ext.getCmp("btn_selectAddress2")
										.setVisible(false);

								// 验证必填项
								var checkLastName = Ext.get("lastName")
										.getValue().trim();
								if ("" == checkLastName) {
									Ext.Msg.alert("错误", "姓为必输项，不能为空！");
									return false;
								}
								var checkFirstName = Ext.get("firstName")
										.getValue().trim();
								if ("" == checkFirstName) {
									Ext.Msg.alert("错误", "名为必输项，不能为空！");
									return false;
								}
								var checkLoginName = Ext.get("loginName")
										.getValue().trim();
								if ("" == checkLoginName) {
									Ext.Msg.alert("错误", "登录名为必输项，不能为空！");
									Ext.getCmp("loginName")
											.markInvalid('该输入项为必输项');
									return false;
								}
								if (!Ext.getCmp("IDCard").isValid()) {
									Ext.Msg.alert("错误", "身份证格式有误，请检查！");
									Ext.getCmp("IDCard")
											.markInvalid('身份证格式有误，请检查！');
									return false;
								}
								if (!Ext.getCmp("email").isValid()) {
									Ext.Msg.alert("错误", "email格式有误，请检查！");
									Ext.getCmp("email")
											.markInvalid('email格式有误，请检查！');
									return false;
								}
								if (!Ext.getCmp("homeTelNo").isValid()) {
									Ext.Msg.alert("错误", "住宅电话格式有误，请检查！");
									Ext.getCmp("homeTelNo")
											.markInvalid('住宅电话有误，请检查！');
									return false;
								}
								if (!Ext.getCmp("zipCode").isValid()) {
									Ext.Msg.alert("错误", "家庭信息的邮政编码格式有误，请检查！");
									Ext.getCmp("zipCode")
											.markInvalid('家庭信息的邮政编码格式有误，请检查！');
									return false;
								}
								if (!Ext.getCmp("webPage").isValid()) {
									Ext.Msg.alert("错误", "请输入正确的个人主页！");
									Ext.getCmp("webPage")
											.markInvalid('请输入正确的个人主页！');
									return false;
								}
								if (!Ext.getCmp("mobileNo").isValid()) {
									Ext.Msg.alert("错误", "手机号码格式有误，请检查！");
									Ext.getCmp("mobileNo")
											.markInvalid('手机号码格式有误，请检查！');
									return false;
								}
								if (!Ext.getCmp("switchboardNo").isValid()) {
									Ext.Msg.alert("错误", "单位电话号码格式有误，请检查！");
									Ext.getCmp("switchboardNo")
											.markInvalid('单位电话号码格式有误，请检查！');
									return false;
								}
								if (!Ext.getCmp("extNo").isValid()) {
									Ext.Msg.alert("错误", "分机号格式有误，请输入3位分机号！");
									Ext.getCmp("extNo")
											.markInvalid('分机号格式有误，请输入3位分机号！');
									return false;
								}
								if (!Ext.getCmp("faxNo").isValid()) {
									Ext.Msg.alert("错误", "传真号格式错误，请修改！");
									Ext.getCmp("faxNo").markInvalid('传真号格式错误！');
									return false;
								}

								if (!Ext.getCmp('chooseAddress').isValid()) {
									Ext.Msg.alert("错误", "请进行城市选择！");
									Ext.getCmp('chooseAddress')
											.markInvalid('请进行城市选择');
									return false;
								}
								if (!Ext.getCmp('zipCodeAddress').isValid()) {
									Ext.Msg.alert("错误", "单位邮编格式有误，请检查！！");
									Ext.getCmp('zipCodeAddress')
											.markInvalid('单位邮编格式有误，请检查！');
									return false;
								}
								// 非中水科技的人员登录名 默认产生一个数据
								if (compID != 1) {
									Ext.getCmp("loginName").setValue(new Date()
											.getTime());
								}
								var result = ajaxSyncCall(__cxtPath
												+ '/orgMaint.do',
										'method=checkLoginNameExist&userid='
												+ Ext.get("userid").getValue()
												+ '&loginName='
												+ Ext.get("loginName")
														.getValue());
								if (result.success) {
									Ext.getCmp("mainForm").getForm().submit({
										url : __cxtPath + '/orgMaint.do',
										method : 'post',
										params : {
											method : 'saveEmpInfo',
											addressID : Ext.getCmp("addressID")
													.getValue(),
											cityID : Ext.getCmp("cityID")
													.getValue(),
											zipCodeAddress : Ext
													.getCmp("zipCodeAddress")
													.getValue(),
											sex : Ext.getCmp("sex").getValue(),
											IDCard : Ext.getCmp("IDCard")
													.getValue(),
											positionEdit : Ext
													.getCmp("positionEdit")
													.getValue(),
											politicalStatus : Ext
													.getCmp("politicalStatus")
													.getValue(),
											switchboardNo : Ext
													.getCmp("switchboardNo")
													.getValue(),
											faxNo : Ext.getCmp("faxNo")
													.getValue()
										},
										waitMsg : '正在保存，请稍候……',
										waitTitle : '正在处理',
										success : function(form, action) {
											Ext.Msg.alert(
													action.result.messageTitle,
													action.result.message,
													function() {
														// 点击保存按钮后，左侧树刷新，并指定到人员的节点
														var empNodeID = currentNodePath
																.substring(currentNodePath
																		.lastIndexOf("/")
																		+ 1);
														treeloader
																.load(
																		west
																				.getRootNode(),
																		function(
																				n) {
																			west
																					.expandPath(
																							currentNodePath,
																							"",
																							function() {
																								west
																										.getNodeById(empNodeID)
																										.select();
																								showMethod(west
																										.getNodeById(empNodeID));
																							});
																		});
													});
										},
										failure : function(form, action) {
											Ext.Msg.alert(
													action.result.messageTitle,
													action.result.message);
										}
									});
								} else {
									Ext.Msg.alert(result.messageTitle,
											result.message);
								}
							}
						},
						'-',
						{
							xtype : 'uploadButton',
							text : '附件上传',
							reuses : false,
							REFIDFun : function() {
								return Ext.get('userid').getValue();
							},
							attachmentTypeID : com.bitc.xmis.Attachment.staffInfo,
							uploadPanelConfig : {
								listeners : {
									'uploadcomplete' : function() {
										attFileGrid.reload();
									}
								}
							}
						},
						'-',
						{
							xtype : 'button',
							text : '学历',
							id : 'saveEduInfo',
							iconCls : 'menu-contract-edit',
							handler : function() {
								var eduD = new Ext.form.DateField({
											name : 'graduationDate',
											id : 'graduationDate',
											format : 'Y-m-d',
											maxValue : new Date(),
											minValue : '1900-01-01',
											fieldLabel : '毕业日期',
											showToday : true,
											editable : false,
											value : Ext
													.get("oldGraduationDate")
													.getValue()
										});
								var form = new Ext.form.FormPanel({
									baseCls : 'x-plain',
									anchor : '100%',
									lableWidth : 10,
									defaultType : 'textfield',
									defaults : {
										xtype : "textfield",
										width : 200
									},
									items : [{
												inputType : 'hidden',
												name : 'eduID',
												value : document.all.eduID.value
											}, {
												inputType : 'hidden',
												name : 'oldEduName',
												id : 'oldEduName'
											}, {
												fieldLabel : "学&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;历",
												name : 'eduLevel',
												id : 'eduLevel',
												value : Ext.get("oldEduLevel")
														.getValue()
											}, {
												fieldLabel : "毕业院校",
												inputType : 'text',
												name : 'schoolName',
												id : 'schoolName',
												value : Ext
														.get("oldSchoolName")
														.getValue()
											}, eduD, {
												fieldLabel : "学习专业",
												inputType : 'text',
												name : 'major',
												id : 'major',
												value : Ext.get("oldMajor")
														.getValue()
											}, {
												fieldLabel : "附&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;注",
												inputType : 'text',
												name : 'eduRemark',
												id : 'eduRemark',
												value : Ext.get("oldEduRemark")
														.getValue()
											}]
								});

								var winEduEdit = new Ext.Window({
									layout : 'fit',
									modal : true,
									title : "学历记录编辑",
									frame : true,
									plain : true,
									resizable : false,
									buttonAlign : "center",
									closable : true,
									bodyStyle : "padding:10px",
									width : 360,
									height : 220,
									layout : "form",
									defaults : {
										xtype : "textfield",
										width : 200
									},
									items : form,
									buttons : [{
										text : "确认",
										handler : function() {
											if (document.all.eduLevel.value == "") {
												alert("请输入学历名称！");
												document.all.eduLevel.focus();
												return false;
											}
											// else if
											// (document.all.schoolName.value ==
											// "") {
											// alert("请输入学校名称！");
											// document.all.schoolName.focus();
											// return false;
											// }
											else if (document.all.graduationDate.value == "") {
												alert("请输入毕业日期！");
												document.all.graduationDate
														.focus();
												return false;
											}
											// else if (document.all.major.value
											// == "")
											// {
											// alert("请输入所学专业！");
											// document.all.major.focus();
											// return false;
											// }

											form.getForm().submit({
												url : __cxtPath
														+ '/orgMaint.do?method=saveEducation',
												method : 'post',
												params : {
													userid : Ext
															.getCmp("mainForm")
															.getForm()
															.findField('userid')
															.getValue(),
													oldEdu : '<%=emp.getPoliticalStatus() %>'
												},
												success : function(form, action) {
													Ext.Msg
															.alert(
																	action.result.messageTitle,
																	action.result.message);
													Ext
															.getCmp("eduID")
															.setValue(action.result.eduID);
													Ext
															.getCmp("oldEduLevel")
															.setValue(action.result.eduName);
													Ext
															.getCmp("oldSchoolName")
															.setValue(action.result.eduSchool);
													Ext
															.getCmp("oldGraduationDate")
															.setValue(action.result.eduDate);
													Ext
															.getCmp("oldMajor")
															.setValue(action.result.eduMajor);
													Ext
															.getCmp("oldEduRemark")
															.setValue(action.result.eduRemark);
													Ext
															.getCmp('educationHighestLevel')
															.setValue(action.result.eduName);
													winEduEdit.close();
												},
												failure : function(form, action) {
													Ext.Msg
															.alert(
																	action.result.messageTitle,
																	action.result.message);
													winEduEdit.close();
												}
											});
										}
									}, {
										text : "取消",
										handler : function() {
											winEduEdit.close();
										}
									}]
								})
								winEduEdit.setPosition(240, 100);
								winEduEdit.show();
							}
						},
						'-',
						{
							xtype : 'button',
							text : '设置密码',
							id : 'opPassword',
							iconCls : 'menu-password-edit',
							hidden : (compID == 1 ? false : true),
							handler : function() {
								var win = new Ext.Window({
									layout : 'fit',
									modal : true,
									title : "设置登录密码",
									frame : true,
									plain : true,
									resizable : false,
									buttonAlign : "center",
									closable : true,
									bodyStyle : "padding:10px",
									width : 300,
									height : 180,
									layout : "form",
									lableWidth : 30,
									defaults : {
										xtype : "textfield",
										width : 120
									},
									monitorValid : true,
									items : [{
												fieldLabel : "旧的密码",
												inputType : 'password',
												name : 'oldPwd',
												id : 'oldPwd',
												allowBlank : false,
												blankText : "必须输入旧密码"
											}, {
												fieldLabel : "新的密码",
												inputType : 'password',
												name : 'newPwd',
												id : 'newPwd',
												allowBlank : false,
												blankText : "必须输入新密码"
											}, {
												fieldLabel : "重新输入",
												inputType : 'password',
												name : 'renewPwd',
												id : 'renewPwd',
												allowBlank : false,
												blankText : "必须再输入新密码"
											}],
									buttons : [{
										text : "确认",
										handler : function() {
											if (Ext.getCmp("password")
													.getValue().trim() != Ext
													.getCmp("oldPwd")
													.getValue().trim()) {
												Ext.Msg.alert("错误",
														"旧密码有误，请重新输入！");
												oldPwd.focus();
												return false;
											}

											if (newPwd.value == "") {
												Ext.Msg.alert("错误", "请输入新密码！");
												newPwd.focus();
												return false;
											} else if (renewPwd.value == "") {
												Ext.Msg
														.alert("错误",
																"请再次输入新密码！");
												renewPwd.focus();
												return false;
											} else if (newPwd.value != renewPwd.value) {
												Ext.Msg.alert("错误",
														"新密码2次输入不同，请重新输！");
												newPwd.focus();
												return false;
											} else {
												Ext.getCmp("password")
														.setValue(newPwd.value);
												Ext.Ajax.request({
													url : __cxtPath
															+ '/orgMaint.do',
													success : function(
															response, config) {
														Ext.MessageBox
																.alert(
																		"result",
																		response.responseText);
													},
													failure : function() {
														Ext.MessageBox
																.alert(
																		"result",
																		response.responseText);
													},
													method : "post",
													params : {
														method : 'updatePwd',
														userid : Ext
																.get("userid")
																.getValue(),
														pwd : newPwd.value
													}
												});
												win.close();
											}
										}
									}, {
										text : "取消",
										handler : function() {
											win.close();
										}
									}, {
										text : "初始化",
										hidden : ((isAdmin || isDirectDeptAdmin))
												? false
												: true,
										handler : function() {
											Ext.MessageBox
													.confirm(
															"",
															"是否将<b>"
																	+ Ext
																			.get("lastName")
																			.getValue()
																			.trim()
																	+ Ext
																			.get("firstName")
																			.getValue()
																			.trim()
																	+ "</b>的密码初始化为<b>000000</b>？",
															function(btn) {
																if (btn == 'yes') {
																	Ext.Ajax
																			.request(
																					{
																						url : __cxtPath
																								+ '/orgMaint.do',
																						success : function(
																								response,
																								config) {
																							Ext
																									.getCmp("password")
																									.setValue('000000');
																							Ext.MessageBox
																									.alert(
																											"提示",
																											response.responseText);
																						},
																						failure : function(
																								response,
																								config) {
																							Ext.MessageBox
																									.alert(
																											"提示",
																											response.responseText);
																						},
																						method : "post",
																						params : {
																							method : 'updatePwd',
																							userid : Ext
																									.get("userid")
																									.getValue(),
																							pwd : '000000'
																						}
																					});

																}
															})
											win.close();
										}
									}]
								});
								win.setPosition(300, 100);
								win.show();
							}
						},
						// '-',
						{
							xtype : 'button',
							text : '设置午餐地点',
							hidden : true,
							id : 'opLunchAddress',
							handler : function() {
								var combo = new Ext.form.ComboBox({
											id : 'lunchAddID',
											// blankText: '请选择午餐地点.',
											// allowBlank: false,
											// store :
											// '<%=emp.getLunchAddressData()
											// %>',
											displayField : 'state',
											typeAhead : true,
											mode : 'local',
											forceSelection : true,
											triggerAction : 'all',
											emptyText : '请选择...',
											editable : false, // false则不可编辑，默认为true
											selectOnFocus : true
										});

								var but = new Ext.Button({
									id : 'a',
									text : '确定',
									display : false,
									handler : function() {
										// alert(Ext.getCmp("lunchAddID").getValue());
										alert("本功能每月1日至<%=String.valueOf(WebKeys.LunchUpdatedTerm) %>日开放！");
										return false;

										comboForm.getForm().submit({
											url : 'orgMaint.do?method=changeLunchAddress',
											method : 'post',
											params : {
												userid : '<%=emp.getUserID() %>',
												newAddress : Ext
														.getCmp("lunchAddID")
														.getValue()
											},
											success : function(form, action) {
												// alert(action.result.message);
												Ext.Msg
														.alert(
																action.result.messageTitle,
																action.result.message);

												document.all.lunchAddress.value = Ext
														.get("lunchAddID").dom.value;
												document.all.lunchAddressID.value = Ext
														.getCmp("lunchAddID")
														.getValue();
												store.load();
											},
											failure : function(form, action) {
												Ext.Msg
														.alert(
																action.result.messageTitle,
																action.result.message);
											}
										});
									}
								});

								var jBut = new Ext.Button({
									id : 'jb',
									text : 'Journal',
									display : false,
									handler : function() {

										// alert(Ext.getCmp("lunchAddID").getValue());

										alert("本功能每月<%=String.valueOf(WebKeys.LunchUpdatedTerm) %>日以后开放！");
										// return false;

										comboForm.getForm().submit({
											url : 'orgMaint.do?method=lunchAddressJournal',
											method : 'post',
											success : function(form, action) {
												// alert(action.result.message);
												Ext.Msg
														.alert(
																action.result.messageTitle,
																action.result.message);

												document.all.lunchAddress.value = Ext
														.get("lunchAddID").dom.value;
												document.all.lunchAddressID.value = Ext
														.getCmp("lunchAddID")
														.getValue();
												store.load();
											},
											failure : function(form, action) {
												Ext.Msg
														.alert(
																action.result.messageTitle,
																action.result.message);
											}
										});

									}
								});

								var comboForm = new Ext.form.FormPanel({

									baseCls : 'x-plain',
									anchor : '80%',
									width : 600,

									items : [{
												baseCls : 'x-plain',
												xtype : "panel",
												layout : "column",
												fieldLabel : "选择午餐地点",
												isFormField : true,
												items : [combo, but]
											}]
										// buttons:[{text:"确定"},{text:"取消"}]
									});

								// var store = new Ext.data.GroupingStore(
								// {
								// url :
								// 'orgMaint.do?method=getLunchAddressList',
								// reader : new Ext.data.JsonReader(
								{
									// root : 'rows',
									// totalProperty : 'totalCount',
									// remoteSort : true,
									// fields : [
									// {
									// name : 'dept_name'
									// },
									// {
									// name : 'user_name'
									// },
									// {
									// name : 'old_address_name'
									// },
									// {
									// name : 'lunchAddress_name'
									// },
									// {
									// name : 'changed_flag'
									// } ]
									// }),
									// sortInfo : {
									// field : 'dept_name',
									// direction : 'ASC'
									// },
									// groupField : 'changed_flag'

									// });

									var colM = new Ext.grid.ColumnModel([
											new Ext.grid.RowNumberer(), {
												header : "部门名称",
												dataIndex : "dept_name",
												sortable : true
											}, {
												header : "员工姓名",
												dataIndex : "user_name",
												sortable : true
											}, {
												header : "原午餐地点",
												dataIndex : "old_address_name",
												sortable : true
											}, {
												header : "新午餐地点",
												dataIndex : "lunchAddress_name",
												sortable : true
											}, {
												header : "状态",
												dataIndex : "changed_flag",
												sortable : true
											}]);

									// var gridM = new Ext.grid.GridPanel(
									// {
									// cm : colM,
									// //store : store,
									// view : new Ext.grid.GroupingView(
									// {
									// forceFit : true,
									// hideGroupedColumn : true,
									// //用来分组的数据点这一列是否要显示
									//
									// showGroupName : true,
									// //用来分组的数据点这一列的header是否要随group name一起显示
									//
									// startCollapsed : false,
									// //一开始进到grid这页，它的group是合起还是展开
									//
									// groupTextTpl : '{text}
									// ({[values.rs.length]} {[values.rs.length
									// > 1 ? "人" : "人"]})'
									// }),

									// collapsible : true,
									// animCollapse : false,
									// iconCls : 'icon-grid',
									// frame : true,

									// bbar : new Ext.PagingToolbar(
									// {
									// pageSize : 0,
									// //store : store,
									// displayInfo : true,
									// displayMsg : '当前显示 {0} - {1} 条记录 /共 {2}
									// 条记录',
									// emptyMsg : "无显示数据"
									// }),

									// height : 320,
									// autoHeight : false,
									// title : '午餐地点统计'
									// });

									var winLunch = new Ext.Window({
												modal : true,
												title : "午餐地点设置",
												frame : true,
												plain : true,
												resizable : false,
												closeAction : "hide",
												// maximizable:true,
												closable : true,
												bodyStyle : "padding:10px",
												width : 600,
												height : 400,
												items : [comboForm // ,
												// gridM
												]
											});
									winLunch.setPosition(120, 80);
									winLunch.show();
									// store
									// .removeAll();
									// store
									// .load();

									function biuldLinker(val) {
										if (val.indexOf("^") > 0) {
											var text = val.substring(0, val
															.indexOf("^"));
											var url = val.substring(val
															.indexOf("^")
															+ 1, val.length);
											if (text != "0") {
												return "<a href=\"" + url
														+ "\">" + text + "</a>";
											} else {
												return text;
											}
										} else {
											return val;
										}
									}
									// Ext
									// .getCmp(
									// "lunchAddID")
									// .setValue(
									// document.all.lunchAddressID.value);
								}
							}
						},
						'-',
						{
							xtype : 'button',
							text : '设置职务',
							hidden : (compID == 1 ? false : true),
							iconCls : 'cogAdd_image',
							id : 'opPosition',
							handler : function() {
								var winPostEdit = new Ext.Window({
									layout : 'form',
									modal : true,
									title : "设置职务",
									frame : true,
									plain : true,
									resizable : false,
									buttonAlign : "center",
									closable : true,
									bodyStyle : "padding:10px",
									width : 360,
									height : 180,
									minWidth : 360,
									minHeight : 180,
									items : {
										xtype : 'form',
										frame : true,
										ref : 'postForm',
										baseCls : 'x-plain',
										id : 'postForm',
										items : {
											xtype : 'combo',
											fieldLabel : '选择职务',
											name : 'positions',
											id : 'positions',
											editable : false,
											triggerAction : "all",
											store : positionsStore,
											displayField : "description",
											emptyText : '请选择职务',
											valueField : "ID",
											mode : "remote",
											width : 180
										}

									},
									buttons : [{
										text : "确认",
										handler : function() {
											var postID = Ext
													.getCmp("positions")
													.getValue();
											if ("" == postID) {
												Ext.Msg.alert("提示", "请选择职务！");
											} else {
												Ext.getCmp("postForm")
														.getForm().submit({
															url : __cxtPath
																	+ '/orgMaint.do?method=updatePost',
															method : 'post',
															params : {
																id : Ext
																		.getCmp("userid")
																		.getValue(),
																postID : postID
															},
															waitMsg : '正在保存，请稍候……',
															waitTitle : '正在处理',
															success : function(
																	form,
																	action) {
																Ext
																		.getCmp("position")
																		.setValue(action.result.attributes.postName);
																Ext
																		.getCmp("positionID")
																		.setValue(postID);
																Ext.Msg
																		.alert(
																				action.result.messageTitle,
																				action.result.message);
																treeloader
																		.load(
																				west
																						.getRootNode(),
																				function(
																						n) {
																					west
																							.expandPath(currentNodePath);
																				});
																winPostEdit
																		.close();
															},
															failure : function(
																	form,
																	action) {
																Ext.Msg
																		.alert(
																				action.result.messageTitle,
																				action.result.message);

															}
														});
											}
										}
									}, {
										text : "取消",
										handler : function() {
											winPostEdit.close();
										}
									}]
								});

								winPostEdit.setPosition(240, 100);
								winPostEdit.show();
								if (Ext.getCmp("positionID").getValue().trim() != 0) {
									Ext.getCmp("positions").setValue(Ext
											.getCmp("positionID").getValue()
											.trim());
								}
							}
						}, '-',
						'<font color ="blue">注：<font color ="#CC0000">◎</font> 标记为只允许管理员修改</font>']
			},
			items : [{
						xtype : 'panel',
						region : 'center',
						layout : 'auto',
						bodyStyle : "padding: 10px",
						labelWidth : 65,
						autoScroll : true,
						labelAlign : 'right',
						border : false,
						items : [rowWork]
					}]
		}, {
			title : "家庭信息",
			xtype : 'panel',
			id : 'homeMsg',
			bodyStyle : "padding: 10px",
			items : [{
						xtype : 'panel',
						border : false,
						layout : 'form',
						width : 800,
						items : [rowHome]
					}],
			layout : 'auto',
			labelWidth : 65,
			labelAlign : 'right',
			defaults : {
				border : false,
				defaults : {
					border : false
				}
			},
			tbar : {
				xtype : 'toolbar',
				id : 'tbar2',
				items : [{
					xtype : 'button',
					text : '保存',
					id : 'saveUserInfo2',
					iconCls : 'save',
					handler : function() {
						// 验证必填项
						var checkLastName = Ext.get("lastName").getValue()
								.trim();
						if ("" == checkLastName) {
							Ext.Msg.alert("错误", "姓为必输项，不能为空！");
							return false;
						}
						var checkFirstName = Ext.get("firstName").getValue()
								.trim();
						if ("" == checkFirstName) {
							Ext.Msg.alert("错误", "名为必输项，不能为空！");
							return false;
						}
						var checkLoginName = Ext.get("loginName").getValue()
								.trim();
						if ("" == checkLoginName) {
							Ext.Msg.alert("错误", "登录名为必输项，不能为空！");
							Ext.getCmp("loginName").markInvalid('该输入项为必输项');
							return false;
						}
						if (!Ext.getCmp("IDCard").isValid()) {
							Ext.Msg.alert("错误", "身份证格式有误，请检查！");
							Ext.getCmp("IDCard").markInvalid('身份证格式有误，请检查！');
							return false;
						}
						if (!Ext.getCmp("email").isValid()) {
							Ext.Msg.alert("错误", "email格式有误，请检查！");
							Ext.getCmp("email").markInvalid('email格式有误，请检查！');
							return false;
						}
						if (!Ext.getCmp("homeTelNo").isValid()) {
							Ext.Msg.alert("错误", "住宅电话格式有误，请检查！");
							Ext.getCmp("homeTelNo")
									.markInvalid('住宅电话格式有误，请检查！');
							return false;
						}
						if (!Ext.getCmp("zipCode").isValid()) {
							Ext.Msg.alert("错误", "家庭信息的邮政编码格式有误，请检查！");
							Ext.getCmp("zipCode")
									.markInvalid('家庭信息的邮政编码格式有误，请检查！');
							return false;
						}
						if (!Ext.getCmp("webPage").isValid()) {
							Ext.Msg.alert("错误", "请输入正确的个人主页！");
							Ext.getCmp("webPage").markInvalid('请输入正确的个人主页！');
							return false;
						}
						if (!Ext.getCmp("mobileNo").isValid()) {
							Ext.Msg.alert("错误", "手机号码格式有误，请检查！");
							Ext.getCmp("mobileNo").markInvalid('手机号码格式有误，请检查！');
							return false;
						}
						if (!Ext.getCmp("switchboardNo").isValid()) {
							Ext.Msg.alert("错误", "单位电话号码格式有误，请检查！");
							Ext.getCmp("switchboardNo")
									.markInvalid('单位电话号码格式有误，请检查！');
							return false;
						}
						if (!Ext.getCmp("extNo").isValid()) {
							Ext.Msg.alert("错误", "分机号格式有误，请输入3位分机号！");
							Ext.getCmp("extNo")
									.markInvalid('分机号格式有误，请输入3位分机号！');
							return false;
						}
						if (!Ext.getCmp("faxNo").isValid()) {
							Ext.Msg.alert("错误", "传真号格式错误，请修改！");
							Ext.getCmp("faxNo").markInvalid('传真号格式错误！');
							return false;
						}

						if (!Ext.getCmp('chooseAddress').isValid()) {
							Ext.Msg.alert("错误", "请进行城市选择！");
							Ext.getCmp('chooseAddress').markInvalid('请进行城市选择');
							return false;
						}
						if (!Ext.getCmp('zipCodeAddress').isValid()) {
							Ext.Msg.alert("错误", "单位邮编格式有误，请检查！");
							Ext.getCmp('zipCodeAddress')
									.markInvalid('单位邮编格式有误，请检查！');
							return false;
						}

						// 非中水科技的人员登录名 默认产生一个数据
						if (compID != 1) {
							Ext.getCmp("loginName").setValue(new Date()
									.getTime());
						}
						var result = ajaxSyncCall(__cxtPath + '/orgMaint.do',
								'method=checkLoginNameExist&userid='
										+ Ext.get("userid").getValue()
										+ '&loginName='
										+ Ext.get("loginName").getValue());
						if (result.success) {
							Ext.getCmp("mainForm").getForm().submit({
								url : __cxtPath + '/orgMaint.do',
								method : 'post',
								params : {
									method : 'saveEmpInfo',
									addressID : Ext.getCmp("addressID")
											.getValue(),
									cityID : Ext.getCmp("cityID").getValue(),
									zipCodeAddress : Ext
											.getCmp("zipCodeAddress")
											.getValue(),
									sex : Ext.getCmp("sex").getValue(),
									IDCard : Ext.getCmp("IDCard").getValue(),
									positionEdit : Ext.getCmp("positionEdit")
											.getValue(),
									politicalStatus : Ext
											.getCmp("politicalStatus")
											.getValue(),
									switchboardNo : Ext.getCmp("switchboardNo"),
									faxNo : Ext.getCmp("faxNo").getValue()
								},
								waitMsg : '正在保存，请稍候……',
								waitTitle : '正在处理',
								success : function(form, action) {
									Ext.Msg.alert(action.result.messageTitle,
											action.result.message);
									// 点击保存按钮后，左侧树刷新，并指定到人员的节点
									var empNodeID = currentNodePath
											.substring(currentNodePath
													.lastIndexOf("/")
													+ 1);
									treeloader.load(west.getRootNode(),
											function(n) {
												west.expandPath(
														currentNodePath, "",
														function() {
															west
																	.getNodeById(empNodeID)
																	.select();
														});
											});
								},
								failure : function(form, action) {
									Ext.Msg.alert(result.messageTitle,
											result.message);
								}
							});
						} else {
							Ext.Msg.alert(result.messageTitle, result.message);
						}

					}
				}]
			}
		}]
	});
	var treeloader = new Ext.tree.TreeLoader({
				dataUrl : __cxtPath + '/orgMaint.do?method=getOrgTree&compID='
						+ compID
			});
			
//2013-12添加人员按钮		
	function addPersonBtn(selected) {
		if (selected.isLeaf()) {
			selected.leaf = false;
		}
		node1 = new Ext.tree.TreeNode({
					text : '新员工',
					icon : __cxtPath + '/resources/images/0201.png',
					// iconCls : 'personPic',
					id : 'empl-new-' + new Date().format("YmdHis")
							+ parseInt(Math.random() * 1000000)
				});
		selected.expand(1, '', function() {
					selected.appendChild(node1); // 增加子节点
					node1.select();

				}); // 展开节点

		var d = __emp.divisionID;// 管理员所在的事业部ID
		if ((isAdmin || compID != 1) || (isDirectDeptAdmin)) {
			tbar_new.addPersonBtn.enable();
			tbar_new.addDeptBtn.enable();
			tbar_new.removeBtn.enable();
			tbar_new.saveBtn.enable();
			if ("empl-" == node1.id.substr(0, 5)) {
				tbar_new.addPersonBtn.disable();
				tbar_new.addDeptBtn.disable();
			tbar_new.removeBtn.disable();//不让禁用
				if ("empl-old" == node1.id.substr(0, 8)) {
					tbar_new.removeBtn.disable();
				}

			}
		} else {
			tbar_new.setDisabled(true);
		}
	}

	//2013-12添加部门按钮	 
	function addDeptBtn(selected){
			if (selected.isLeaf()) {
				selected.leaf = false;
			}
			node1 = new Ext.tree.TreeNode({
						text : '新部门',
						iconCls : 'deptUnsaved',
						id : 'dept-new-' + new Date().format("YmdHis")
								+ parseInt(Math.random() * 1000000)
					});
			selected.expand(1, '', function() {
						selected.appendChild(node1); // 增加子节点
						node1.select();

					});

			var d = __emp.divisionID;// 管理员所在的事业部ID
			if ((isAdmin || compID != 1) || (isDirectDeptAdmin)) {
				tbar_new.addPersonBtn.enable();
				tbar_new.addDeptBtn.enable();
				tbar_new.removeBtn.enable();
				tbar_new.saveBtn.enable();
				if ("dept-" == selected.id.substr(0, 5)) {
					if (compID == 1) {
						tbar_new.addDeptBtn.disable();
					}
				}
			} else {
				tbar_new.setDisabled(true);
			}
	}		
	
	//2013-12禁用按钮	
	function removeInfoByBtn(selected) {
		if ("new" == selected.id.substr(5, 3)) {
			Ext.MessageBox.confirm('信息提示', '请确认禁用' + selected.text,
					function showResult(btn) {
						if (btn == 'yes') {
							selected.remove();
						}
					});
		} else {
			if ("empl-" == selected.id.substr(0, 5)) {
				Ext.MessageBox.alert("提示", "请在右侧面板的“禁用管理”中进行禁用操作!");
			} else {
				selected.expand(false, null, function() {
					var childNum = selected.childNodes;
					if (childNum != 0) {
						Ext.Msg
								.alert("提示信息", selected.text
												+ "还有员工或下属部门，不能禁用!");
					} else {
						var msg = selected.text;
						if (msg.indexOf("(") > 0) {
							msg = msg.substr(0, msg.indexOf("("));
						}
						Ext.MessageBox.confirm('信息提示', '请确认是否禁用' + msg,
								function showResult(btn) {
									if (btn == 'yes') {
										Ext.Ajax.request({
											url : __cxtPath
													+ '/orgMaint.do?method=disableTreeNode',
											success : function(response, config) {
												Ext.MessageBox.alert("result",
														response.responseText);
												treeloader.load(west
																.getRootNode(),
														function(n) {
															west.getRootNode()
																	.expand();
														});
											},
											failure : function(response, config) {
												Ext.MessageBox.alert("result",
														response.responseText);
											},
											method : "post",
											params : {
												item : selected.id
											}
										});
									}
								});
					}
				});

			}
		}

	}
	
	var tbar_new = new Ext.Toolbar([{
		ref : 'addPersonBtn',
		iconCls : 'add',
		align : 'right',
		text : '员工',
		handler : function() {
			var selected = west.getSelectionModel().getSelectedNode();
			if (selected == null) {
				Ext.Msg.alert('提示', '请先选中一个部门');
				return;
			}
			addPersonBtn(selected);

		}
	}, {
		ref : 'addDeptBtn',
		iconCls : 'add',
		align : 'right',
		text : '部门',
		handler : function() {
			var selected = west.getSelectionModel().getSelectedNode();
			if (selected == null) {
				Ext.Msg.alert('提示', '请先选中一个公司或部门');
				return;
			}
			addDeptBtn(selected);
		}
	}, {
		ref : 'removeBtn',
		iconCls : 'remove',
		align : 'right',
		text : '禁用',
		handler : function() {
			var selected = west.getSelectionModel().getSelectedNode();
			if (selected == null) {
				Ext.Msg.alert('提示', '请先选中要禁用的部门或人员');
				return;
			}
			removeInfoByBtn(selected);
		}
	}, {
		ref : 'saveBtn',
		iconCls : 'ok',
		align : 'right',
		text : '保存',
		handler : function() {
			saveAllByBtn();
		}
	}]);
	// treePanel
	var west = new Ext.tree.TreePanel({
		region : "west",
		title : '组织机构树',
		ref : 'orgtree',
		width : 205,
		maxWidth : 250,
		minWidth : 200,
		autoScroll : true, // 滚动条
		animate : true, // 展开,收缩动画
		enableDD : true, // 节点是否可拖动
		border : false,
		loader : treeloader,
		lines : false,
		split : true,
		collapsible : true, // 右上方展开收缩条
		collapseMode : 'mini', // 将滚动收缩条模式‘迷你’
		rootVisible : false, // 是否显示根节点
		singleExpand : true, // 只有一个节点展开
		iconCls : 'tree',
		containerScroll : true,
		root : {
			nodeType : 'async',
			draggable : false, // 可拖动的
			id : 'root'
		},
		bbar : {
			xtype : 'toolbar',
			name : 'bbar_text',
			hidden : (compID == 1 ? true : false),
			items : ['<font color ="#003366">*选中节点后单击进行“修改”操作 *</font>']
		},
		tbar : [new Ext.ux.form.SearchField({
			width : 140,
			emptyText : '请输入员工姓名',
			onRender : function(ct, position) {
				Ext.ux.InklingSearchField.superclass.onRender.call(this, ct,
						position);
				if (this.tooltip && this.tooltip.text)
					new Ext.ToolTip({
								target : this.id,
								trackMouse : false,
								draggable : false,
								maxWidth : 200,
								minWidth : 100,
								title : this.tooltip.title,
								html : this.tooltip.text
							});
			},
			tooltip : {
				title : '姓名查询',
				text : '输入查询的员工的姓名'
			},
			onTrigger1Click : function() {
				this.el.dom.value = '';
				this.triggers[0].hide();
				treeloader.load(west.getRootNode(), function(n) {
							west.getRootNode().expand();
						});
			},
			onTrigger2Click : function() {
				var inputName = this.el.dom.value;
				if (inputName != null && '' != inputName.trim()) {
					Ext.Ajax.request({
						url : __cxtPath
								+ '/orgMaint.do?method=getEmpListByName',
						method : 'post',
						params : {
							name : inputName,
							isAdmin : (isAdmin || isDirectDeptAdmin),
							compID : compID
						},
						success : function(response, action) {
							var result = Ext.decode(response.responseText);
							var nodePath = result.attributes.nodepath;
							if ("" != nodePath && nodePath.indexOf("/") > -1) {
								var userID = nodePath.substring(nodePath
										.lastIndexOf("/")
										+ 1);
								treeloader.load(west.getRootNode(), function() {
											west.expandPath(nodePath, "",
													function() {
														west
																.getNodeById(userID)
																.select();

														currentNodePath = nodePath;// 当前节点值改变
														// 为点击保存按钮所用
														var node = west
																.getNodeById(userID);
														showMethod(node);
													})
										});
							} else {
								Ext.MessageBox.alert("提示", "此公司无此人！");
								treeloader.load(west.getRootNode(),
										function(n) {
											west.getRootNode().expand();
										});
							}
						},
						failure : function(response, action) {
							Ext.MessageBox.hide();
							Ext.MessageBox.alert("提示", "操作失败！");
						}
					});
					this.triggers[0].show();
				} else {
					Ext.MessageBox.alert("提示", "请输入要查询员工的姓名后进行查询");
				}
			}
		})],
		listeners : {
			append : function(tree, n, node, index) {
				var forbidden = node.attributes;
				if (forbidden && forbidden.attributes
						&& forbidden.attributes.forbidden == '1'
						&& !(isAdmin || isDirectDeptAdmin)) {
					node.remove();
				} else {
					if (!node.isLeaf() && node.getDepth() < 2) {
						node.expand();
					}
				}
			},
			dblclick : function(node, e) {
				console.info(node.getPath());

			},
			click : function(node) {
				Ext.getCmp("btn_selectAddress2").setVisible(false);
				currentNodePath = node.getPath();// 当前节点值改变 为点击保存按钮所用
				showMethod(node);

				// -start
				selected = node;
				var d = __emp.divisionID;// 管理员所在的事业部ID
				if ((isAdmin || compID != 1)
						|| (isDirectDeptAdmin && (selected.getPath()
								.indexOf('/dept-old-' + d + '/') > -1 || selected
								.getPath() == '/root/comp-old-1/dept-old-' + d))) {
					tbar_new.addPersonBtn.enable();
					tbar_new.addDeptBtn.enable();
					tbar_new.removeBtn.enable();
					tbar_new.saveBtn.enable();
					if ("comp-" == selected.id.substr(0, 5)) {
						tbar_new.addPersonBtn.disable();
						tbar_new.removeBtn.disable();
					}
					if ("empl-" == selected.id.substr(0, 5)) {
						tbar_new.addPersonBtn.disable();
						tbar_new.addDeptBtn.disable();
						if ("empl-old" == selected.id.substr(0, 8)) {
							tbar_new.removeBtn.disable();
						}
					}
					if ("dept-" == selected.id.substr(0, 5)) {
						if ("dept-" == selected.parentNode.id.substr(0, 5)) {
							if (compID == 1) {
								tbar_new.addDeptBtn.disable();
							}
						}
					}
				} else {
					tbar_new.setDisabled(true);
				}
				// -stop
			},
			render : function() {
				tbar_new.render(west.tbar);

				if (!(compID != 1 || isDirectDeptAdmin || isAdmin)) {
					tbar_new.setDisabled(true);
				}
			}
		}
	});

	var contextmenu = new Ext.menu.Menu({
				id : 'theContextMenu',
				items : [{
					text : "新建员工",
					iconCls : "add",
					id : 'addemp',
					pressed : true,
					handler : function() {
						addPersonBtn(selected) ;
						
//						if (selected.isLeaf()) {
//							selected.leaf = false;
//						}
//						node1 = new Ext.tree.TreeNode({
//									text : '新员工',
//									icon : __cxtPath
//											+ '/resources/images/0201.png',
//									// iconCls : 'personPic',
//									id : 'empl-new-'
//											+ new Date().format("YmdHis")
//											+ parseInt(Math.random() * 1000000)
//								});
//						selected.expand(); // 展开节点
//						selected.appendChild(node1); // 增加子节点
//						node1.select();
//						
					}
				}, {
					text : "新建部门",
					iconCls : "add",
					id : 'adddept',
					pressed : true,
					handler : function() {
						 addDeptBtn(selected);
//						if (selected.isLeaf()) {
//							selected.leaf = false;
//						}
//						node1 = new Ext.tree.TreeNode({
//									text : '新部门',
//									iconCls : 'deptUnsaved',
//									id : 'dept-new-'
//											+ new Date().format("YmdHis")
//											+ parseInt(Math.random() * 1000000)
//								});
//						selected.expand(); // 展开节点
//						selected.appendChild(node1); // 增加子节点
//						node1.select();
//						
					}
				}, {
					text : "禁用",
					iconCls : "remove",
					id : "delete",
					pressed : true,
					handler : function() {
						removeInfoByBtn(selected);

					}
				}, {
					text : "保存全部",
					iconCls : "ok",
					id : "saveAll",
					pressed : true,
					handler : function() {
						saveAllByBtn();
					}
				}]
			});

	west.on("contextmenu", function(node, e) {
				var model = west.getSelectionModel();// 获取选择模型
				e.preventDefault();// 关闭默认的菜单，以避免弹出两个菜单
				node.select();
				selected = model.select(node);

				var d = __emp.divisionID;// 管理员所在的事业部ID
				if ((isAdmin || compID != 1)
						|| (isDirectDeptAdmin && (selected.getPath()
								.indexOf('/dept-old-' + d + '/') > -1 || selected
								.getPath() == '/root/comp-old-1/dept-old-' + d))) {

					contextmenu.findById('addemp').enable();
					contextmenu.findById('adddept').enable();
					contextmenu.findById('delete').enable();
					contextmenu.findById('saveAll').enable();
					if ("comp-" == selected.id.substr(0, 5)) {
						contextmenu.findById('addemp').disable();
						contextmenu.findById('delete').disable();
					}
					if ("empl-" == selected.id.substr(0, 5)) {
						contextmenu.findById('addemp').disable();
						contextmenu.findById('adddept').disable();
						contextmenu.findById('delete').disable();
					}
					if ("dept-" == selected.id.substr(0, 5)) {
						if ("dept-" == selected.parentNode.id.substr(0, 5)) {
							if (compID == 1) {
								contextmenu.findById('adddept').disable();
							}
						}
						selected.expand(); // 展开节点
					}
					contextmenu.showAt(e.getXY());
				}
				
				// -start
				selected = node;
				var d = __emp.divisionID;// 管理员所在的事业部ID
				if ((isAdmin || compID != 1)
						|| (isDirectDeptAdmin && (selected.getPath()
								.indexOf('/dept-old-' + d + '/') > -1 || selected
								.getPath() == '/root/comp-old-1/dept-old-' + d))) {
					tbar_new.addPersonBtn.enable();
					tbar_new.addDeptBtn.enable();
					tbar_new.removeBtn.enable();
					tbar_new.saveBtn.enable();
					if ("comp-" == selected.id.substr(0, 5)) {
						tbar_new.addPersonBtn.disable();
						tbar_new.removeBtn.disable();
					}
					if ("empl-" == selected.id.substr(0, 5)) {
						tbar_new.addPersonBtn.disable();
						tbar_new.addDeptBtn.disable();
						if ("empl-old" == selected.id.substr(0, 8)) {
							tbar_new.removeBtn.disable();
						}
					}
					if ("dept-" == selected.id.substr(0, 5)) {
						if ("dept-" == selected.parentNode.id.substr(0, 5)) {
							if (compID == 1) {
								tbar_new.addDeptBtn.disable();
							}
						}
					}
				} else {
					tbar_new.setDisabled(true);
				}
				// -stop
			}, this);

	var treeEditor = new Ext.tree.TreeEditor(west, {
				id : 'treeEdit',
				allowBlank : false
			});

	// 设置根节点不可编辑
	treeEditor.on("beforestartedit", function(treeEditor) {
				var tempNode = treeEditor.editNode;// 将要编辑的节点
				var d = __emp.divisionID;// 管理员所在的事业部ID

				if (compID != 1
						|| isAdmin
						|| (isDirectDeptAdmin && (tempNode.getPath()
								.indexOf('/dept-old-' + d + '/') > -1 || tempNode
								.getPath() == '/root/comp-old-1/dept-old-' + d))) {
					return true;
				} else {
					return false;
				}
				if (tempNode == west.getRootNode()
						|| "comp-" == treeEditor.editNode.id.substr(0, 5)) {// 这里设置根节点不可编辑
					return false;
				} else {
					return tempNode;
				}
			});

	// 此处将编辑的值传给 changeName
	treeEditor.on("complete", function(treeEditor, newValue) {
				var n = newValue.lastIndexOf("(");
				if (n > 0) {
					newValue = newValue.substr(0, n);
				}
				if ("old" == treeEditor.editNode.id.substr(5, 3)) {
					if ("empl-old-" == treeEditor.editNode.id.substr(0, 9)) {
						var index = changeEmplName
								.indexOf(treeEditor.editNode.id + ":");
						if (index >= 0) {
							var front = changeEmplName
									.substr(
											0,
											index
													+ treeEditor.editNode.id.length
													+ 1);
							var latter = changeEmplName.substr(index
									+ treeEditor.editNode.id.length + 1);
							var latterIndex = latter.indexOf(",");
							if (latterIndex != -1) {
								latter = latter.substr(latterIndex);
								changeEmplName = front + newValue + latter;
							} else {
								changeEmplName = front + newValue;
							}
						} else {
							if ("" == changeEmplName.trim()) {
								changeEmplName = changeEmplName
										+ treeEditor.editNode.id + ":"
										+ newValue;
							} else {
								changeEmplName = changeEmplName + ","
										+ treeEditor.editNode.id + ":"
										+ newValue;
							}
						}
					} else {
						var index = changeDeptName
								.indexOf(treeEditor.editNode.id + ":");
						if (index >= 0) {
							var front = changeDeptName
									.substr(
											0,
											index
													+ treeEditor.editNode.id.length
													+ 1);
							var latter = changeDeptName.substr(index
									+ treeEditor.editNode.id.length + 1);
							var latterIndex = latter.indexOf(",");
							if (latterIndex != -1) {
								latter = latter.substr(latterIndex);
								changeDeptName = front + newValue + latter;
							} else {
								changeDeptName = front + newValue;
							}
						} else {
							if ("" == changeDeptName.trim()) {
								changeDeptName = changeDeptName
										+ treeEditor.editNode.id + ":"
										+ newValue;
							} else {
								changeDeptName = changeDeptName + ","
										+ treeEditor.editNode.id + ":"
										+ newValue;
							}
						}
					}

				}
			});

	west.on('nodedragover', function(e) {
				var selectedNode = e.dropNode;
				if ("" == frontParentIds) {
					frontParentIds = frontParentIds + selectedNode.id.trim()
							+ ":" + selectedNode.parentNode.id.trim();
				} else {
					var index = frontParentIds.indexOf(selectedNode.id.trim()
							+ ":");
					if (index == -1) {
						frontParentIds = frontParentIds + ","
								+ selectedNode.id.trim() + ":"
								+ selectedNode.parentNode.id.trim();
					}
				}

			});

	west.on('beforemovenode',
			function(tree, node, oldParent, newParent, index) {
				// 是监控事业部管理员 拖拽之前的路径包含 ‘监控事业部’
				// 、拖拽之后的路径包含‘监控事业部’ 的情况可以拖拽，否则不可以拖拽
				var d = __emp.divisionID;// 管理员所在的事业部ID
				if (isDirectDeptAdmin && compID==1) {
					if ((oldParent.getPath().indexOf('/dept-old-' + d + '/') > -1 || oldParent
							.getPath() == '/root/comp-old-1/dept-old-' + d)
							&& (newParent.getPath().indexOf('/dept-old-' + d
									+ '/') > -1 || newParent.getPath() == '/root/comp-old-1/dept-old-'
									+ d)) {
						return true;
					} else {
						Ext.Msg.alert('提示', '您只有维护监控事业部的权限，有疑问请联系管理员~');
						return false;
					}
				} else if ("empl-" == node.id.substr(0, 5)
						&& ("root" == newParent.id || "root" == newParent.parentNode.id)) {
					Ext.Msg.alert('提示', '请重新操作,将人员移动到部门下进行保存！');
					return false;
				} else {
					return true;
				}
			});

	west.on('nodedrop', function(e) {
		var selectedNode = e.dropNode;
		if ("old" == selectedNode.id.substr(5, 3)) {
			// 非将人员移动到公司的情况：
			if (!("empl-" == selectedNode.id.trim().substr(0, 5) && "comp-old-" == selectedNode.parentNode.id
					.trim().substr(0, 9))) {
				var targetNodeId = selectedNode.parentNode.id;
				var temp = frontParentIds.substr(frontParentIds
						.indexOf(selectedNode.id.trim() + ":")
						+ (selectedNode.id.trim() + ":").length);
				var frontParentId = "";
				if (temp.indexOf(",") == -1) {
					frontParentId = temp;
				} else {
					frontParentId = temp.substr(0, temp.indexOf(","));
				}
				if ("" == dragRelation.trim()) {
					dragRelation = dragRelation + selectedNode.id + ":"
							+ frontParentId + ":" + targetNodeId;
				} else {
					var index = dragRelation.indexOf(selectedNode.id + ":");
					if (index >= 0) {
						// 已存在
						var front = dragRelation.substr(0, index
										+ selectedNode.id.length + 1);
						var latter = dragRelation.substr(index
								+ selectedNode.id.length + 1);
						var latterIndex = latter.indexOf(",");
						if (latterIndex != -1) {
							latter = latter.substr(latterIndex);
							dragRelation = front + frontParentId + targetNodeId
									+ latter;
						} else {
							dragRelation = front + frontParentId + ":"
									+ targetNodeId;
						}
					} else {
						dragRelation = dragRelation + "," + selectedNode.id
								+ ":" + frontParentId + ":" + targetNodeId;
					}
				}
			}
		}
			//
	});
	west.getRootNode().expand();

	// 显示指定人员信息
	function showMethod(node) {

		if ("empl-old-" == node.id.substr(0, 9)) {
			refid_empID = node.id.substr(9, node.id.length);
			attFileGrid.reload({
						params : {
							REFID : refid_empID
						}
					});
			// 加载成功后 tabpanel显示
			center.show();
			// start
			// 点击人员节点，加载先关地址的列表
			Ext.getCmp("mainForm").getForm().load({
				url : __cxtPath
						+ '/orgMaint.do?method=getOneEmployee&employeeId='
						+ node.id,
				waitMsg : '正在载入数据...',
				waitTitle : '请等待...',
				success : function(form, action) {
					if ("" != Ext.getCmp("mainForm").getForm()
							.findField('image').getValue().trim()) {
						imgs.setSrc(__cxtPath
								+ "/fileUpload.do?type=7&userPic=/"
								+ Ext.getCmp("mainForm").getForm()
										.findField('image').getValue())
								+ Math.random();
					} else {
						imgs.setSrc(__cxtPath + '/resources/images/020.png');
					}
					if ("" != Ext.getCmp("mainForm").getForm()
							.findField('postCard').getValue().trim()) {
						imgsCard
								.setSrc(__cxtPath
										+ "/fileUpload.do?type=7&isPostCard=front&userPic=/"
										+ Ext.getCmp("mainForm").getForm()
												.findField('postCard')
												.getValue())
								+ (Math.random());
					} else {
						imgsCard.setSrc(__cxtPath
								+ '/resources/images/postCard.png');
					}
					if ("" != Ext.getCmp("mainForm").getForm()
							.findField('postCardBack').getValue().trim()) {
						imgsCard2
								.setSrc(__cxtPath
										+ "/fileUpload.do?type=7&isPostCard=back&userPic=/"
										+ Ext.getCmp("mainForm").getForm()
												.findField('postCardBack')
												.getValue())
								+ (Math.random());
					} else {
						imgsCard2.setSrc(__cxtPath
								+ '/resources/images/postCard.png');
					}

					Ext.getCmp("politicalStatus").setValue(Ext
							.getCmp("polStat").getValue());
					dept = node.parentNode.text;
					if (dept.indexOf("(") > 0) {
						dept = dept.substr(0, dept.indexOf("("));
					}
					Ext.getCmp("deptName").setValue(dept);

					Ext.getCmp("nickName").setTitle(Ext.getCmp("lastName")
							.getValue().trim()
							+ Ext.getCmp("firstName").getValue().trim());

					var comp = "";
					var tagNode = node;
					while ("comp-old-" != tagNode.parentNode.id.substr(0, 9)) {
						tagNode = tagNode.parentNode;
					}
					comp = tagNode.parentNode.text;
					if (comp.indexOf("(") > 0) {
						comp = comp.substr(0, comp.indexOf("("));
					}
					Ext.getCmp("companyName").setValue(comp);
					Ext.getCmp("birthday").setValue(Ext.getCmp("birth")
							.getValue());
					Ext.getCmp("joinDate").setValue(Ext.getCmp("join")
							.getValue());
					var t = parseInt(Ext.getCmp("birth").getValue().trim()
							.substr(0, 4));
					if (!(Ext.getCmp("birth").getValue() == null || Ext
							.getCmp("birth").getValue().trim() == "")) {
						var year = new Date().getYear() - t >= 0 ? new Date()
								.getYear()
								- t : 1900 - t + new Date().getYear();
						Ext.getCmp("age").setValue(year + "周岁");
					} else {
						Ext.getCmp("age").setValue("");
					}
					// 是中水科技的条件下 判断
					var mydivisionID = Ext.get('pathId').getValue();// 查看人的事业部ID
					if (compID == 1) {
						if (isAdmin) {
							Ext.getCmp("tbar1").show();
							Ext.getCmp("tbar2").show();
							Ext.getCmp('homeMsg').enable();

							Ext.getCmp('imgs_btn').show();

							Ext.getCmp('postCard_btn').enable();
							Ext.getCmp('postCardBack_btn').enable();
							Ext.getCmp('postCardDel_btn').enable();
							Ext.getCmp('postCardBackDel_btn').enable();

							Ext.getCmp('IDCard').show();// 身份证
							Ext.getCmp("opPosition").show();// 职务
							Ext.getCmp('saveEduInfo').show();// 学历

							tbar_new.setDisabled(false);
						} else if (isDirectDeptAdmin
								&& mydivisionID == __emp.divisionID) {
							// 是事业部级管理员 ：管理员所在事业部ID==被查看人的事业部ID
							Ext.getCmp("tbar1").show();
							Ext.getCmp("tbar2").show();
							Ext.getCmp('homeMsg').enable();

							Ext.getCmp('imgs_btn').show();

							Ext.getCmp('postCard_btn').enable();
							Ext.getCmp('postCardBack_btn').enable();
							Ext.getCmp('postCardDel_btn').enable();
							Ext.getCmp('postCardBackDel_btn').enable();

							Ext.getCmp('IDCard').show();// 身份证
							Ext.getCmp("opPosition").show();// 职务
							Ext.getCmp('saveEduInfo').show();// 学历

							tbar_new.setDisabled(false);
						} else if (currentUserID == Ext.getCmp("userid")
								.getValue()) {
							// 不是管理员 是本人
							Ext.getCmp("tbar1").show();
							Ext.getCmp("tbar2").show();
							Ext.getCmp('homeMsg').enable();

							Ext.getCmp('imgs_btn').show();

							Ext.getCmp('postCard_btn').enable();
							Ext.getCmp('postCardBack_btn').enable();
							Ext.getCmp('postCardDel_btn').enable();
							Ext.getCmp('postCardBackDel_btn').enable();

							Ext.getCmp('IDCard').show();// 身份证
							Ext.getCmp("opPosition").hide();// 职务
							Ext.getCmp('saveEduInfo').hide();// 学历

							tbar_new.setDisabled(true);
						} else {
							// 不是管理员 也不是本人
							Ext.getCmp("tbar1").hide();// hide 高度受影响
							Ext.getCmp("tbar2").hide();
							Ext.getCmp('homeMsg').disable();

							Ext.getCmp('IDCard').hide();// 身份证
							Ext.getCmp("opPosition").hide();// 职务
							Ext.getCmp('saveEduInfo').hide();// 学历

							Ext.getCmp('imgs_btn').hide();

							Ext.getCmp('postCard_btn').disable();
							Ext.getCmp('postCardBack_btn').disable();
							Ext.getCmp('postCardDel_btn').disable();
							Ext.getCmp('postCardBackDel_btn').disable();

							tbar_new.setDisabled(true);
						}

						center.setActiveTab(0);
						Ext.getCmp('postCardTab').setActiveTab(0);

						// -start
						selected = node;
						var d = __emp.divisionID;// 管理员所在的事业部ID
						if ((isAdmin || compID != 1)
								|| (isDirectDeptAdmin && (selected.getPath()
										.indexOf('/dept-old-' + d + '/') > -1 || selected
										.getPath() == '/root/comp-old-1/dept-old-'
										+ d))) {
							tbar_new.addPersonBtn.enable();
							tbar_new.addDeptBtn.enable();
							tbar_new.removeBtn.enable();
							tbar_new.saveBtn.enable();
							if ("comp-" == selected.id.substr(0, 5)) {
								tbar_new.addPersonBtn.disable();
								tbar_new.removeBtn.disable();
							}
							if ("empl-" == selected.id.substr(0, 5)) {
								tbar_new.addPersonBtn.disable();
								tbar_new.addDeptBtn.disable();
								if ("empl-old" == selected.id.substr(0, 8)) {
									tbar_new.removeBtn.disable();
								}
							}
							if ("dept-" == selected.id.substr(0, 5)) {
								if ("dept-" == selected.parentNode.id.substr(0,
										5)) {
									if (compID == 1) {
										tbar_new.addDeptBtn.disable();
									}
								}
							}
						} else {
							tbar_new.setDisabled(true);
						}
						// -stop
					}

					// 点击人员节点，加载先关地址的列表
					attFileGrid
							.setActionColumnHidden((compID == 1
									? ((isAdmin || (isDirectDeptAdmin && mydivisionID == __emp.divisionID))
											? false
											: (currentUserID == Ext
													.get('userid').getValue())
													? false
													: true)
									: false));
				},
				failure : function(form, action) {
					Ext.MessageBox.alert('提示', '载入失败');
				}
			});
		}
	}



	// 2013-12 右键保存全部提取出来
	function saveAllByBtn() {
		ergodic(west.getRootNode());
		if ("" != newDept || "" != newEmpl || "" != dragRelation
				|| "" != changeEmplName || "" != changeDeptName) {
			// 编辑人的节点ID，及所在路径
			var empID = "";
			var empIDPath = "";
			if ("" != changeEmplName) {
				empID = changeEmplName.split(":")[0];// 如果要指定修改的第一条
				var empIDNum = changeEmplName.split(",").length;
				if (empIDNum > 0) {
					empID = changeEmplName.split(",")[empIDNum - 1].split(":")[0];
				}
				empIDPath = west.getNodeById(empID).getPath();
			}
			// 编辑人的节点ID，及所在路径
			var dragempID = "";
			var dragempIDPath = "";
			if ("" != dragRelation) {
				dragempID = dragRelation.split(":")[0];// 如果要指定修改的第一条
				var empIDNum = dragRelation.split(",").length;
				if (empIDNum > 0) {
					dragempID = dragRelation.split(",")[empIDNum - 1]
							.split(":")[0];
				}
				dragempIDPath = west.getNodeById(dragempID).getPath();
			}
			// 编辑的部门的节点ID，及其路径
			var deptID = "";
			var deptIDPath = "";
			if ("" != changeDeptName && "" == changeEmplName) {
				deptID = changeDeptName.split(":")[0];
				var deptIDNum = changeDeptName.split(",").length;
				if (deptIDNum > 0) {
					deptID = changeDeptName.split(",")[deptIDNum - 1]
							.split(":")[0];
				}
				deptIDPath = west.getNodeById(deptID).getPath();
				var pathNum = deptIDPath.lastIndexOf("/"); // 指定编辑的部门的节点路径，编辑的部门节点被选中，不展开
				deptIDPath = deptIDPath.substr(0, pathNum);
			}

			Ext.Ajax.request({
				url : __cxtPath + '/orgMaint.do?method=saveAllOperation',
				success : function(response, config) {
					var result = Ext.decode(response.responseText);
					var nodePath = result.attributes.nodepath;

					newDept = ""; // 新部门 (id:text,...)
					newEmpl = ""; // 新员工 (id:text,...)
					newMapping = ""; // 新部门新员工映射
					// (cid:pid,...)

					dragRelation = ""; // 拖拽关系 (cid:nid,...)
					changeEmplName = ""; // 改名(id:修改后名字,...)
					changeDeptName = "";
					frontParentIds = "";
					// 新建人员或新建部门 被修改节点的ID，及所在路径
					if ("" != nodePath) {
						var newID = nodePath.substring(nodePath
								.lastIndexOf("/")
								+ 1);
						treeloader.load(west.getRootNode(), function() {
									west.expandPath(nodePath, "", function() {
												west.getNodeById(newID)
														.select();

												currentNodePath = nodePath;// 当前节点值改变
												// 为点击保存按钮所用
												var node = west
														.getNodeById(newID);
												showMethod(node);
											})
								})
					} else if ("" != empID) {
						// 编辑的人员节点
						treeloader.load(west.getRootNode(), function(n) {
									west.expandPath(empIDPath, "", function() {
												west.getNodeById(empID)
														.select();

												currentNodePath = empIDPath;// 当前节点值改变
												// 为点击保存按钮所用
												var node = west
														.getNodeById(empID);
												showMethod(node);
											});
								});
					} else if ("" != dragempID) {
						// 拖拽的人员节点
						treeloader.load(west.getRootNode(), function(n) {
									west.expandPath(dragempIDPath, "",
											function() {
												west.getNodeById(dragempID)
														.select();

												currentNodePath = dragempIDPath;// 当前节点值改变
												// 为点击保存按钮所用
												var node = west
														.getNodeById(dragempID);
												showMethod(node);
											});
								});
					} else if ("" != deptID) {
						// 编辑的部门节点
						treeloader.load(west.getRootNode(), function(n) {
									west.expandPath(deptIDPath, "", function() {
												west.getNodeById(deptID)
														.select();
											});
								});
					} else {
						treeloader.load(west.getRootNode(), function(n) {
									west.getRootNode().expand();
								});
					}

					Ext.MessageBox.alert("提示", "操作成功！");

					addressIDParam = '';
					cityIDParam = '';
					zipCodeAddressParam = '';
					politicalStatusParam = '';
				},
				failure : function() {
					Ext.MessageBox.alert("提示", "保存失败！");
				},
				method : "post",
				params : {
					newDept : newDept,
					newEmpl : newEmpl,
					newMapping : newMapping,
					dragRelation : dragRelation,
					changeEmplName : changeEmplName,
					changeDeptName : changeDeptName,
					addressID : addressIDParam,
					cityID : cityIDParam,
					zipCodeAddress : zipCodeAddressParam,
					politicalStatus : politicalStatusParam
				}
			});
		} else {
			Ext.MessageBox.alert("提示", "您没有做任何操作，无需保存！");
		}
	}

	var mainForm = new Ext.form.FormPanel({
				id : 'mainForm',
				name : 'mainForm',
				reader : new Ext.data.JsonReader({}, Human),
				region : "center",
				layout : 'auto',
				border : false,
				items : center
			});

	return [west, mainForm];

}