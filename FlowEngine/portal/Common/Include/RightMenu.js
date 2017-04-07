<!--
var menuskin = "skin";
var display_url = 0; // Show URLs in status bar?

function showmenuie5() {
	var rightedge = document.body.clientWidth-event.clientX;
	var bottomedge = document.body.clientHeight-event.clientY;
	if (rightedge < ie5menu.offsetWidth)
		ie5menu.style.left = document.body.scrollLeft + event.clientX - ie5menu.offsetWidth;
	else
		ie5menu.style.left = document.body.scrollLeft + event.clientX;
	if (bottomedge < ie5menu.offsetHeight)
		ie5menu.style.top = document.body.scrollTop + event.clientY - ie5menu.offsetHeight;
	else
		ie5menu.style.top = document.body.scrollTop + event.clientY;
	showHideLayers("document.layers['ie5menu']","document.all['ie5menu']","show");
	nereidFade(document.all["ie5menu"],100,50,20);
	return false;
}

function hidemenuie5() {
	nereidFade(document.all["ie5menu"],0,50,10);
	showHideLayers("document.layers['ie5menu']","document.all['ie5menu']","hide");
}

function highlightie5() {
	if (event.srcElement.className == "menuitems") {
		event.srcElement.style.backgroundColor = "highlight";
		event.srcElement.style.color = "white";
		if (display_url)
			window.status = event.srcElement.url;
	}
}

function lowlightie5() {
	if (event.srcElement.className == "menuitems") {
		event.srcElement.style.backgroundColor = "menu";
		event.srcElement.style.color = "black";
		window.status = "";
	}
}

function jumptoie5() {
	if (event.srcElement.className == "menuitems") {
		if (event.srcElement.getAttribute("target") != null)
			window.open(event.srcElement.url, event.srcElement.getAttribute("target"));
		else
			window.location = event.srcElement.url;
	}
}
//-->