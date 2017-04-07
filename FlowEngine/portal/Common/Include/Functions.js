var lastMouseX;
var lastMouseY;
var curPopupWindow = null;
var helpWindow = null;

function setLastMousePosition(e) {
	if (navigator.appName.indexOf("Microsoft") != -1) e = window.event;
	lastMouseX = e.screenX;
	lastMouseY = e.screenY;
}

function openClickout(url) {
		// Taken out due to inconsistent escape results between java and javascript
        //window.open('/sales/linkout.jsp?url='+escape(url), "_blank", 'width=640,height=480,dependent=no,resizable=yes,toolbar=yes,status=yes,directories=yes,menubar=yes,scrollbars=1', false);
	window.open(url, "_blank", 'width=640,height=480,dependent=no,resizable=yes,toolbar=yes,status=yes,directories=yes,menubar=yes,scrollbars=1', false);
}

function openPopup(url, name, pWidth, pHeight, features, snapToLastMousePosition) {
	closePopup();
	if (snapToLastMousePosition) {
		if (lastMouseX - pWidth < 0) {
			lastMouseX = pWidth;
		}
		if (lastMouseY + pHeight > screen.height) {
			lastMouseY -= (lastMouseY + pHeight + 50) - screen.height;
		}
                lastMouseX -= pWidth;
                lastMouseY += 10;
		features +=	"screenX=" + lastMouseX + ",left=" + lastMouseX + "screenY=" + lastMouseY + ",top=" + lastMouseY;
	}
	curPopupWindow = window.open(url, name, features, false);
}

function closePopup() {
	if (curPopupWindow != null) {
		if (!curPopupWindow.closed) {
			curPopupWindow.close();
		}
		curPopupWindow = null;
	}
}

function openLookup(baseURL,modified,searchParam) {
	if (modified == '1') baseURL = baseURL + searchParam;
	openPopup(baseURL, "lookup", 350, 300, "resizable=no,Width=400,Height=330,center=yes,status=no");
}

function pick(form,field,val) {
	eval("document."+form+"[\""+field+"\"].value=" + "\""+val+"\"");
	closePopup();
	return false;
}

function openCalendar(url) {
	openPopup(url, "Calendar", 220, 145, "width=220,height=145,dependent=yes,resizable=yes,toolbar=no,status=no,directories=no,menubar=no", true);
}

function openComboBox(url) {
	openPopup(replaceChar(url, ' ', '%'), "Select", 220, 200, "width=220,height=200,dependent=yes,resizable=yes,toolbar=no,status=no,directories=no,menubar=no,scrollbars=1", true);
}

function replaceChar(s, oldchar, newchar) {
	var retval = '';
	for (i = 0; i < s.length; i++) {
		if (s.charAt(i) == ' ') {
			retval = retval + '%';
		} else {
			retval = retval + s.charAt(i);
		}
	}	
	return retval;
}

function openHelp(url) {
	if (helpWindow != null) helpWindow.close();
	var w = window.open(url, "Help", "width=500,height=400,toolbar=no,status=no,directories=no,menubar=no,resizable=yes,screenX=" + window.screenX + ",left=" + window.screenX + "screenY=" + window.screenY + ",top=" + window.screenY, true);
    w.focus();
	helpWindow = w;
}

function doPrint() {
	alert("This dialog will be replaced by a new browser window containing a printable version" +
		  " of the current page. The user can then select print from the browsers file menu to"+
		  " print the page. Alternatively, this could be an automatically generated Acrobat file.");
}

function doExport() {
	alert("This dialog will be replaced by a File-Save As dialog. Clicking on save in this dialog will save the contents of the current page to a file (in .csv format).");
}

function listProperties(obj) {
	var names = "";
	for (var i in obj) names += i + ", ";
	alert(names);
}

function lookupPick(formName, parentIdElementName, parentEditElementName, id, display) {
	var parentIdElement = "document." + formName + "[\"" + parentIdElementName + "\"]";
	var parentEditElement = "document." + formName + "[\"" + parentEditElementName + "\"]";
	if (eval(parentIdElement + ".type") == "select-one") {
		var found = false;
		for (i = 0; i < eval(parentIdElement + ".options.length"); i++) {
			if (eval(parentIdElement + ".options[" + i + "].value") == id ) {
				eval(parentIdElement + ".selectedIndex=" + i);
				found = true;
				break;
			}
		}
		if (!found) {
			eval(parentIdElement + ".options[" + parentIdElement + ".options.length] = new Option('" + display + "','" + id + "')");
			eval(parentIdElement + ".selectedIndex=" + parentIdElement + ".options.length - 1");
		}
		eval(parentEditElement + ".value = " + "\"" + display +"\"");    
	} else {
		eval(parentIdElement + ".value = " + "\"" + id +"\"");    
		eval(parentEditElement + ".value = " + "\"" + display +"\"");    
	}
	closePopup();
	return false;
}

function insertTS(field,datetime,alias) {
	field.value = "---------------------------------\n"+ alias + " " +datetime + "\n\n\n" + field.value;
}

function setFocus() {
    var search;
    for (var frm = 0; frm < document.forms.length; frm++) {
        for (var fld = 0; fld < document.forms[frm].elements.length; fld++) {
            var elt = document.forms[frm].elements[fld];
            // skip buttons, radio, or check-boxes
            // to skip "select" types, remove from if statement
            if ( (elt.type == "text") ||
                 (elt.type == "textarea")) {
                if (elt.name == "substring") {
                   search = elt;
                }
                if (elt.name != "substring" && elt.name != "searchtype" &&  
                    elt.name.indexOf("owner") != 0) {
                    elt.focus();
                    // select text in text field or textarea
                    if (elt.type == "text") {
                        elt.select();
                    }
                    return true;
                }
            }
        }
    }

    if (search != null) {
       search.focus();
    }
    return true;
}

function setNamedFocus(element_name) {
    for (var frm = 0; frm < document.forms.length; frm++) {
        for (var fld = 0; fld < document.forms[frm].elements.length; fld++) {
            var elt = document.forms[frm].elements[fld];
            if (elt.name == element_name) {
                elt.focus();
                if (elt.type == "text") {
                    elt.select();
                }
                return true;
            }
        }
    }
    return true;
}


function formatPhone (field) {
    var ov = field.value;
    var v = "";
    var x = -1;
    var s = -1;

    // count number of digits
    var n = 0;
    for (i = 0; i < ov.length; i++) {
        var ch = ov.charAt(i);
        // skip initial chars and possiblu leading 1
        if (s < 0 && (ch > "1" || ch == "0") && ch <= "9") s = i;
        // build up formatted number
        if (s >= 0 && ch >= "0" && ch <= "9") {
            if (n == 0) v += "(";
            else if (n == 3) v += ") ";
            else if (n == 6) v += "-";
            v += ch;
            n++;
        }
        // check for extension type section
        if (ch >= "a" && ch <= "z" || ch >= "A" && ch <= "Z") {
            x = i;
            break;
        }
    }
    // add the extension
    if (x >= 0) v += " " + ov.substring(x, ov.length);

    // if we recognize the number, then format it
    if (n == 10 && v.length <= 20) field.value = v;
    return true;
}

function dc () {
    return window.confirm('Are you sure?');
}

function clearcols () {
    for (var frm = 0; frm < document.forms.length; frm++) {
        for (var fld = 0; fld < document.forms[frm].elements.length; fld++) {
            var elt = document.forms[frm].elements[fld];
            if (elt.name == "c") {
                elt.checked = false;
            }
        }
    }
}

function setcols () {
    for (var frm = 0; frm < document.forms.length; frm++) {
        for (var fld = 0; fld < document.forms[frm].elements.length; fld++) {
            var elt = document.forms[frm].elements[fld];
            if (elt.name == "c") {
                elt.checked = true;
            }
        }
    }
}

function setUsername(uname, fname, lname, suffix) {
	if (uname.value.length == 0) {
		uname.value = 
					fname.value.substring(0,1).toLowerCase() 
					+ lname.value.toLowerCase()
					+ "@"
					+ suffix.value;
	}
}
function setAlias(alias, fname, lname) {
	if (alias.value.length == 0) {
		alias.value = fname.value.substring(0,1).toLowerCase() +
					  lname.value.substring(0,4).toLowerCase();
	}
}

// POPUP WINDOW NUMBER 1 
function popWin(url) {
	closePopup();
	curPopupWindow = window.open(url,"win","toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=1,resizable=0,width=550,height=300",false);
}
function popWin2(url) {
	closePopup();
	curPopupWindow = window.open(url,"win","toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=1,resizable=0,width=720,height=500",false);
}
function printWin(url) {
   closePopup();
   curPopupWindow = window.open(url,"win","toolbar=1,location=1,directories=0,status=1,menubar=1,scrollbars=1,resizable=1,width=705,height=400",false);
}

function moveSelectElement2(sourceSelect, targetSelect, sourceLabel, targetLabel) {
	if (sourceSelect.selectedIndex > -1) {
		for (i=0; i < sourceSelect.length; ++i) {
			var selectedOption = sourceSelect.options[i];
			if (selectedOption.selected) {
				if (selectedOption.text != sourceLabel) {
					var newOption = new Option(selectedOption.text, selectedOption.value);
					if (targetSelect.options.length > 0 && targetSelect.options[0].text == targetLabel) {
						targetSelect.options[0] = newOption;
					} else {
						targetSelect.options[targetSelect.options.length] = newOption;
					}
				} else {
					sourceSelect.selectedIndex = -1;
				}
			}
		}
		for (i=sourceSelect.length-1; i > -1; i--) {
			if (sourceSelect.options[i].selected) sourceSelect.options[i] = null;
		}
		if (sourceSelect.length == 0) {
			var placeHolder = new Option(sourceLabel, sourceLabel);
			sourceSelect.options[0] = placeHolder;
		}
	}
}


