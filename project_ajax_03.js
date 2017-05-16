// JavaScript Document

var strGPIO1 = "";
var strGPIO2 = "";

function GetGPIO() {
	nocache = "&nocache=" + Math.random();
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState == 4 && this.status == 200) {
			Process(this);
		}
	};
	xhttp.open("GET", strGPIO1 + strGPIO2 + nocache, true);
	xhttp.send();
	strGPIO1 = "";
	strGPIO2 = "";
}

function Process(xml) {
	var x = xml.responseXML.getElementsByTagName("GPIO");
	document.getElementById("gpio1").innerHTML = x[0].childNodes[0].nodeValue;
	document.getElementById("gpio2").innerHTML = x[1].childNodes[0].nodeValue;
	document.getElementById("gpio3").innerHTML = x[2].childNodes[0].nodeValue;
}

function SetGPIO(value) {
	if (value == 1) {
		strGPIO1 = "GPIO1";
	}
	if (value == 2) {
		strGPIO2 = "GPIO2";
	}
}

function myFunction() {
	setInterval("GetGPIO()", 500);
}