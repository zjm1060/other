/**
 * $Id: DiagramStore.js,v 1.5 2010/09/06 Modified by Wuzhiming.Add a storage.
 * Class: DiagramStore
 * 
 * A class for storing diagrams. This implementation uses Google Gears, HTML 5
 * (disabled) or a local variable.
 */
var DiagramStore = {
	/**
	 * Variable: useLocalStorage
	 * 
	 * Uses localStorage object in HTML 5. The support in browsers for this
	 * feature is still shaky so it's disabled.
	 */
	useLocalStorage : false,

	/**
	 * Variable: userRemoteStorage
	 * 
	 * Uses Ajax to store the diagram to the remote server. Added by WuZhiming
	 * 2010-09-06
	 */
	useRemoteStorage : false,

	remoteServiceUrl : basePath + 'StoreDiagram',

	/**
	 * Variable: diagrams
	 * 
	 * Array for in-memory storage of the diagrams. This is not persistent
	 * across multiplace invocations and is only used as a fallback if no
	 * client- or server-side storage is available.
	 */
	diagrams : new Object(),

	/**
	 * Variable: diagrams
	 * 
	 * Array for in-memory storage of the diagrams. This is not persistent
	 * across multiplace invocations and is only used as a fallback if no
	 * client- or server-side storage is available.
	 */
	eventSource : new mxEventSource(this),

	/**
	 * Function: isAvailable
	 * 
	 * Returns true if any of the storage mechanisms for driving the diagram
	 * store is available. Currently supported mechanisms are Google Gears and
	 * HTML 5 local storage.
	 */
	isAvailable : function() {
		return DiagramStore.useRemoteStorage;
	},

	/**
	 * Function: init
	 * 
	 * Initializes the diagram store. This is invoked at class creation time and
	 * returns the db instance to operate on.
	 */
	db : function() {
		var db = null;

		try {
			db = google.gears.factory.create('beta.database', '1.0');
			db.open('mxGraphEditor');
			db.execute('CREATE TABLE IF NOT EXISTS Diagrams ('
					+ 'NAME PRIMARY KEY,' + 'XML TEXT' + ');');

			return db;
		} catch (e) {
			// ignore
		}

		return db;
	}(),

	/**
	 * Function: addListener
	 */
	addListener : function(name, funct) {
		DiagramStore.eventSource.addListener(name, funct);
	},

	/**
	 * Function: removeListener
	 */
	removeListener : function(funct) {
		DiagramStore.eventSource.removeListener(funct);
	},

	/**
	 * Function: put
	 * 
	 * Puts the given diagram into the store, replacing any existing diagram for
	 * the given name.
	 */
	put : function(name, xml) {
		if (DiagramStore.useLocalStorage && typeof(localStorage) != 'undefined') {
			return localStorage.setItem(name, xml);
		} else if (DiagramStore.db != null) {
			DiagramStore.db.execute('DELETE FROM Diagrams WHERE name = ?;',
					[name]);
			DiagramStore.db.execute(
					'INSERT INTO Diagrams (NAME, XML) VALUES (?, ?);', [name,
							xml]);
		} else if (DiagramStore.useRemoteStorage) {
			Ext.Ajax.request({
						url : DiagramStore.remoteServiceUrl + '?method=put',
						method : 'POST',
						success : function(response, opts) {
							mxUtils.alert(response.responseText);
						},
						failure : function(response, opts) {
							mxUtils
									.alert('server-side failure with status code '
											+ response.status);
						},
						params : {
							name : name,
							xml : xml
						}
					});
		} else {
			DiagramStore.diagrams[name] = xml;
		}

		DiagramStore.eventSource.fireEvent(new mxEventObject('put'));
	},

	/**
	 * Function: remove
	 * 
	 * Removes the given diagram from the store and returns
	 */
	remove : function(name) {
		if (DiagramStore.useLocalStorage && typeof(localStorage) != 'undefined') {
			localStorage.removeItem(name);
		} else if (DiagramStore.db != null) {
			DiagramStore.db.execute('DELETE FROM Diagrams WHERE name = ?;',
					[name]);
		} else if (DiagramStore.useRemoteStorage) {
			Ext.Ajax.request({
						url : DiagramStore.remoteServiceUrl + '?method=remove',
						method : 'POST',
						success : function(response, opts) {
							mxUtils.alert(response.responseText);
						},
						failure : function(response, opts) {
							mxUtils
									.alert('server-side failure with status code '
											+ response.status);
						},
						params : {
							name : name
						}
					});
		} else {
			delete DiagramStore.diagrams[name];
		}

		DiagramStore.eventSource.fireEvent(new mxEventObject('remove'));
	},

	/**
	 * Function: get
	 * 
	 * Returns the given diagram from the store or null of no such diagram can
	 * be found.
	 */
	get : function(name, callback) {
		var xml = null;

		var handler = function(response, opts) {
			xmlstr = response.responseText;
			callback(xmlstr);
		};

		if (DiagramStore.useLocalStorage && typeof(localStorage) != 'undefined') {
			xml = localStorage.getItem(name);
		} else if (DiagramStore.db != null) {
			var rs = DiagramStore.db.execute(
					'SELECT xml FROM Diagrams WHERE NAME = ?;', [name]);

			if (rs.isValidRow()) {
				xml = rs.field(0);
			}

			rs.close();
		} else if (DiagramStore.useRemoteStorage) {
			Ext.Ajax.request({
						url : DiagramStore.remoteServiceUrl + '?method=get',
						method : 'POST',
						success : handler,
						failure : function(response, opts) {
							mxUtils
									.alert('server-side failure with status code '
											+ response.status);
						},
						params : {
							name : name
						}
					});
		} else {
			xml = DiagramStore.diagrams[name];
		}

		return xml;
	},

	/**
	 * Function: getNames
	 * 
	 * Update the store
	 */
	getNames : function(store) {
		var names = [];

		if (DiagramStore.useLocalStorage && typeof(localStorage) != 'undefined') {
			for (var i = 0; i < localStorage.length; i++) {
				names.push(localStorage.key(i));
			}
		} else if (DiagramStore.db != null) {
			var rs = DiagramStore.db.execute('SELECT name FROM Diagrams;');

			while (rs.isValidRow()) {
				names.push(rs.field(0));
				rs.next();
			}

			rs.close();
		} else if (DiagramStore.useRemoteStorage) {
			Ext.Ajax.request({
						url : DiagramStore.remoteServiceUrl
								+ '?method=getNames',
						method : 'POST',
						success : function(response, opts) {
							names = eval(response.responseText);
							var data = [];
							for (var i = 0; i < names.length; i++) {
								data.push([names[i]]);
							}
							store.loadData(data);
						},
						failure : function(response, opts) {
							mxUtils
									.alert('server-side failure with status code '
											+ response.status);
						}
					});
		} else {
			for (var name in DiagramStore.diagrams) {
				names.push(name);
			}
		}
		var data = [];
		for (var i = 0; i < names.length; i++) {
			data.push([names[i]]);
		}
		store.loadData(data);
	},
	deploy : function(name) {
		Ext.Ajax.request({
					url : DiagramStore.remoteServiceUrl + '?method=deploy',
					method : 'POST',
					params : {
						name : name
					},
					success : function(response, opts) {
						mxUtils.alert(response.responseText);
					},
					failure : function(response, opts) {
						mxUtils.alert('server-side failure with status code '
								+ response.status);
					}
				});
	}

};
