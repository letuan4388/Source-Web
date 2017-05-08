// JavaScript Document

var strGPIO1 = "";
function GetGPIO() {
	nocache = "&nocache= " + Math.random();
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			ProcessResponse(this);
			//GetGPIO();
		}
	};
	xhttp.open("GET", strGPIO1 + nocache, true);
	xhttp.send();
	setTimeout("GetGPIO()", 1000);
	strGPIO1 = "";
}

function ProcessResponse(xml) {
	var x = xml.responseXML.getElementsByTagName("GPIO");
	document.getElementById("gpio1").innerHTML = x[0].childNodes[0].nodeValue;
}

function SetGPIO() {
	 strGPIO1 = "GPIO1";
}
