// JavaScript Documentstr

GPIO1 = '';
function GetGPIO() {
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState == 4 && this.status == 200) {			
			Process(this);
		}
	}
	xhttp.open('GET', 'response.xml', true);
	xhttp.send();
	setTimeout('GetGPIO()', 1000);
	strGPIO1 = '';
}

function Process(xml) {
	var x = xml.getElementsByTagName('LED');
	if (x[0].childNodes[0].nodeValue == 'ON')
		document.getElementById('gpio1').innerHTML = 'DEVICE IS ON';
	else
		document.getElementById('gpio1').innerHTML = 'DEVICE IS OFF';
}
function SetGPIO() {
	strGPIO1 = "GPIO1";
	alert("GPIO1");
}
