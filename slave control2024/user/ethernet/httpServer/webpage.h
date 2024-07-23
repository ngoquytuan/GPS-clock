//last edit by Tuannq 07July2022
#ifndef _WEBPAGE_H_
#define _WEBPAGE_H_

/*************************************************************************************
 * JavaScript Functions
 *************************************************************************************/

#define wiz550web_ajax_js 	"function AJAX(a,e){var c=d();c.onreadystatechange=b;function d(){if(window.XMLHttpRequest){return new XMLHttpRequest()}else{if(window.ActiveXObject){return new ActiveXObject(\"Microsoft.XMLHTTP\")}}}function b(){if(c.readyState==4){if(c.status==200){if(e){e(c.responseText)}}}}this.doGet=function(){c.open(\"GET\",a,true);c.send(null)};this.doPost=function(f){c.open(\"POST\",a,true);c.setRequestHeader(\"Content-Type\",\"application/x-www-form-urlencoded\");c.setRequestHeader(\"ISAJAX\",\"yes\");c.send(f)}}function $(a){return document.getElementById(a)}function $$(a){return document.getElementsByName(a)}function $$_ie(a,c){if(!a){a=\"*\"}var b=document.getElementsByTagName(a);var e=[];for(var d=0;d<b.length;d++){att=b[d].getAttribute(\"name\");if(att==c){e.push(b[d])}}return e}"

/* Get: Network Information: function NetinfoCallback(o), getNetinfo() */
#define wiz550web_netinfo_js	"function NetinfoCallback(o){"\
									 "$('txtmac').value=o.mac;"\
									 "$('txtip').value=o.ip;"\
									 "$('txtgw').value=o.gw;"\
									 "$('txtsn').value=o.sn;"\
									 "$('txtdns').value=o.dns;"\
									 "$('txtntpip').value=o.ntp;"\
									 "if(typeof(window.external)!='undefined'){"\
										"obj=$$_ie('input','dhcp');"\
									 "}else{"\
										"obj=$$('dhcp');"\
									 "}"\
								"}"\
								" "\
								"function getNetinfo(){"\
									 "var oUpdate;"\
									 "setTimeout(function(){"\
									 	"oUpdate=new AJAX('get_netinfo.cgi',function(t){"\
									 		"try{eval(t);}catch(e){alert(e);}"\
									 "});"\
									 "oUpdate.doGet();},300);"\
								 "}"




/*************************************************************************************
 * HTML Pages (web pages)
 *************************************************************************************/
#define index_page "<!DOCTYPE html>"\
"<html lang=\"en\">"\
"<head>"\
"    <meta charset=\"UTF-8\">"\
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"\
"    <title>Clock Configuration</title>"\
"    <style>"\
"        body {"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"            height: 100vh;"\
"            margin: 0;"\
"            font-family: Arial, Tahoma;"\
"            background-color: #f4f4f9;"\
"        }"\
"        #main {"\
"            width: 520px;"\
"            border: 1px solid #6495ED;"\
"            background-color: #fff;"\
"            padding: 20px;"\
"            border-radius: 8px;"\
"            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);"\
"            text-align: center;"\
"        }"\
"        #head {"\
"            border: 1px solid #CDCDCD;"\
"            height: 70px;"\
"            background-color: #008800;"\
"            color: white;"\
"            font-weight: bold;"\
"            font-size: 30px;"\
"            margin-bottom: 20px;"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"        }"\
"        #foot {"\
"            background-color: #0099FF;"\
"            border: 1px solid #ccc;"\
"            color: black;"\
"            font-size: 15px;"\
"            height: 30px;"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"            margin-top: 20px;"\
"        }"\
"        h1 {"\
"            color: black;"\
"            font-size: 18px;"\
"            margin: 20px 0;"\
"        }"\
"        #net_info_general,"\
"        #clock_info,"\
"        #info {"\
"            min-height: 150px;"\
"            border: 1px solid #CDCDCD;"\
"            background-color: #F5F5DC;"\
"            padding: 10px;"\
"            border-radius: 8px;"\
"            margin-bottom: 20px;"\
"            text-align: left;"\
"        }"\
"        #list_head {"\
"            background-color: #FFCC99;"\
"            height: 40px;"\
"            width: 100%;"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"            font-size: 18px;"\
"            border-radius: 8px;"\
"            margin-bottom: 10px;"\
"        }"\
"        .tab {"\
"            width: 100%;"\
"            border: none;"\
"            border-spacing: 5px;"\
"        }"\
"        .tab td {"\
"            padding: 5px;"\
"        }"\
"        .btn {"\
"            display: inline-block;"\
"            text-align: center;"\
"            cursor: pointer;"\
"            color: #fff;"\
"            background-color: #D2691E;"\
"            height: 30px;"\
"            width: 80px;"\
"            border-radius: 10pt;"\
"            border: none;"\
"            margin-top: 10px;"\
"        }"\
"        .btn:hover {"\
"            background-color: #E9967A;"\
"        }"\
"        .sts_label {"\
"            border: none;"\
"            color: black;"\
"            font-size: 15px;"\
"            height: 30px;"\
"            width: 60px;"\
"            background: #F5F5DC;"\
"        }"\
"        #clockdiv {"\
"            color: #fff;"\
"            display: flex;"\
"            justify-content: space-around;"\
"            font-size: 30px;"\
"        }"\
"        #clockdiv > div {"\
"            padding: 10px;"\
"            border-radius: 3px;"\
"            background: #00BF96;"\
"            display: flex;"\
"            flex-direction: column;"\
"            align-items: center;"\
"            height: 70px;"\
"            width: 50px;"\
"        }"\
"        .clock_label {"\
"            padding: 5px;"\
"            border-radius: 3px;"\
"            background: #00816A;"\
"            display: inline-block;"\
"            height: 20px;"\
"            width: 40px;"\
"            color: white;"\
"            font-weight: bold;"\
"            font-size: 16px;"\
"            text-align: center;"\
"            margin-bottom: 5px;"\
"        }"\
"        .smalltext {"\
"            font-size: 16px;"\
"        }"\
"    </style>"\
"</head>"\
"<body>"\
"    <div id=\"main\">"\
"        <div id=\"head\">NTP CLOCK SLAVE</div>"\
"        <div id=\"net_info_general\">"\
"            <div id=\"list_head\"><h1>Network Information</h1></div>"\
"            <table class=\"tab\">"\
"                <tbody>"\
"                    <tr>"\
"                        <td>IP Address:&nbsp</td>"\
"                        <td colspan=\"2\"><input id=\"txtip\" name=\"ip\" type=\"text\" disabled /></td>"\
"                    </tr>"\
"                    <tr>"\
"                        <td>NTP Server:&nbsp</td>"\
"                        <td colspan=\"2\"><input id=\"txtsn\" name=\"sn\" type=\"text\" disabled /></td>"\
"                    </tr>"\
"                </tbody>"\
"            </table>"\
"            <button type=\"button\" class=\"btn\" onclick=\"location.href='config.html'\">Setup</button>"\
"        </div>"\
"        <div id=\"clock_info\">"\
"            <div id=\"list_head\"><h1>Time UTC</h1></div>"\
"            <div id=\"clockdiv\">"\
"                <div>"\
"                    <input type=\"text\" class=\"clock_label\" id=\"txtdays\" name=\"days\" disabled value=\" \" />"\
"                    <div class=\"smalltext\">Day</div>"\
"                </div>"\
"                <div>"\
"                    <input type=\"text\" class=\"clock_label\" id=\"txtmonths\" name=\"months\" disabled value=\" \" />"\
"                    <div class=\"smalltext\">Month</div>"\
"                </div>"\
"                <div>"\
"                    <input type=\"text\" class=\"clock_label\" id=\"txtyears\" name=\"years\" disabled value=\" \" />"\
"                    <div class=\"smalltext\">Year</div>"\
"                </div>"\
"                <div>"\
"                    <input type=\"text\" class=\"clock_label\" id=\"txthours\" name=\"hours\" disabled value=\" \" />"\
"                    <div class=\"smalltext\">Hour</div>"\
"                </div>"\
"                <div>"\
"                    <input type=\"text\" class=\"clock_label\" id=\"txtminutes\" name=\"minutes\" disabled value=\" \" />"\
"                    <div class=\"smalltext\">Minute</div>"\
"                </div>"\
"                <div>"\
"                    <input type=\"text\" class=\"clock_label\" id=\"txtseconds\" name=\"seconds\" disabled value=\" \" />"\
"                    <div class=\"smalltext\">Second</div>"\
"                </div>"\
"            </div>"\
"        </div>"\
"        <div id=\"info\">"\
"            <div id=\"list_head\"><h1>Device Version</h1></div>"\
"            <table class=\"tab\">"\
"                <tbody>"\
"                    <tr>"\
"                        <td style=\"color: #000000; width:30%; text-align:center\">Hardware Version:&nbsp</td>"\
"                        <td style=\"color: #000000; width:10%; text-align:left\"><input type=\"text\" class=\"sts_label\" value=\"Ver 2.0\" disabled /></td>"\
"                        <td style=\"color: #000000; width:30%; text-align:center\">Software Version:&nbsp</td>"\
"                        <td style=\"color: #000000; width:10%; text-align:left\"><input type=\"text\" class=\"sts_label\" value=\"Ver 2.0\" disabled /></td>"\
"                    </tr>"\
"                </tbody>"\
"            </table>"\
"        </div>"\
"        <div id=\"foot\">Powered by <a style=\"color: #000000; font-weight: bold; text-decoration: none\">Air Traffic Technical Company - <span style=\"color:red\">A</span><span style=\"color: blue;\">TTECH</span></a></div>"\
"    </div>"\
"    <script>"\
"        setInterval(calldata, 1000);"\
"        function calldata() {"\
"            var xhttp = new XMLHttpRequest();"\
"            xhttp.onreadystatechange = function () {"\
"                if (this.readyState == 4 && this.status == 200) {"\
"                    var obj = JSON.parse(this.responseText);"\
"                    document.getElementById('txtip').value = obj.txtip;"\
"                    document.getElementById('txtsn').value = obj.txtsn;"\
"                    document.getElementById('txtdays').value = obj.txtdays;"\
"                    document.getElementById('txtmonths').value = obj.txtmonths;"\
"                    document.getElementById('txtyears').value = obj.txtyears;"\
"                    document.getElementById('txthours').value = obj.txthours;"\
"                    document.getElementById('txtminutes').value = obj.txtminutes;"\
"                    document.getElementById('txtseconds').value = obj.txtseconds;"\
"                }"\
"            };"\
"            xhttp.open('GET', 'alldata.json', true);"\
"            xhttp.send();"\
"        }"\
"    </script>"\
"</body>"\
"</html>"


#define netinfo_page_slave 		"<!DOCTYPE html>"\
"<html lang=\"en\">"\
"<head>"\
"    <meta charset=\"UTF-8\">"\
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"\
"    <title>Slave Clock Web Server Network Info</title>"\
"    <style>"\
"        body {"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"            height: 100vh;"\
"            margin: 0;"\
"            font-family: Arial, sans-serif;"\
"            background-color: #e0f7fa; /* Light blue background */"\
"        }"\
"        .container {"\
"            background-color: #fff;"\
"            border: 1px solid #0288d1; /* Blue border */"\
"            padding: 20px;"\
"            border-radius: 8px;"\
"            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);"\
"            width: 400px;"\
"        }"\
"        .container div {"\
"            font-size: 18px;"\
"            font-weight: bold;"\
"            margin-bottom: 20px;"\
"            text-align: center;"\
"            color: #fff;"\
"            background-color: #0288d1; /* Blue background */"\
"            padding: 10px;"\
"            border-radius: 5px;"\
"        }"\
"        ul {"\
"            padding: 0;"\
"            list-style: none;"\
"        }"\
"        li {"\
"            margin-bottom: 15px;"\
"            display: flex;"\
"            align-items: center;"\
"        }"\
"        label {"\
"            width: 150px;"\
"            text-align: right;"\
"            margin-right: 10px;"\
"            color: #0288d1; /* Blue text color */"\
"        }"\
"        input {"\
"            flex: 1;"\
"            padding: 5px;"\
"            border: 1px solid #0288d1; /* Blue border */"\
"            border-radius: 4px;"\
"            background-color: #f1f8e9; /* Light green background for contrast */"\
"        }"\
"        .btn {"\
"            display: inline-block;"\
"            text-align: center;"\
"            cursor: pointer;"\
"            color: #fff;"\
"            background-color: #0288d1; /* Blue button */"\
"            height: 30px;"\
"            width: 100px;"\
"            margin: auto;"\
"            border-radius: 10pt;"\
"            border: none;"\
"            margin-top: 20px;"\
"        }"\
"        .btn:hover {"\
"            background-color: #0277bd; /* Darker blue on hover */"\
"        }"\
"    </style>"\
"    <script type=\"text/javascript\" src=\"ajax.js\"></script>"\
"    <script type=\"text/javascript\" src=\"netinfo.js\"></script>"\
"</head>"\
"<body onload=\"getNetinfo();\">"\
"    <div class=\"container\">"\
"        <div>Slave Clock Web Server Network Info</div>"\
"        <ul>"\
"            <li>"\
"                <label for=\"txtmac\">MAC:</label>"\
"                <input id=\"txtmac\" name=\"mac\" type=\"text\" size=\"20\" disabled>"\
"            </li>"\
"            <li>"\
"                <label for=\"txtip\">IP:</label>"\
"                <input id=\"txtip\" name=\"ip\" type=\"text\" size=\"20\" disabled>"\
"            </li>"\
"            <li>"\
"                <label for=\"txtgw\">GW:</label>"\
"                <input id=\"txtgw\" name=\"gw\" type=\"text\" size=\"20\" disabled>"\
"            </li>"\
"            <li>"\
"                <label for=\"txtsn\">SN:</label>"\
"                <input id=\"txtsn\" name=\"sn\" type=\"text\" size=\"20\" disabled>"\
"            </li>"\
"            <li>"\
"                <label for=\"txtdns\">DNS:</label>"\
"                <input id=\"txtdns\" name=\"dns\" type=\"text\" size=\"20\" disabled>"\
"            </li>"\
"            <li>"\
"                <label for=\"txtntpip\">NTP Time Server:</label>"\
"                <input id=\"txtntpip\" name=\"ntp\" type=\"text\" size=\"20\" disabled>"\
"            </li>"\
"        </ul>"\
"        <button type=\"button\" class=\"btn\" onclick=\"location.href='config.html'\">Setup</button>"\
"    </div>"\
"</body>"\
"</html>"



#define confightml "<!DOCTYPE html>"\
"<html lang=\"en\">"\
"<head>"\
"    <meta charset=\"UTF-8\">"\
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"\
"    <title>Clock Configuration</title>"\
"    <style>"\
"        body {"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"            height: 100vh;"\
"            margin: 0;"\
"            font-family: Arial, Tahoma;"\
"            background-color: #f4f4f9;"\
"        }"\
"        #main {"\
"            width: 500px;"\
"            border: 1px solid #6495ED;"\
"            background-color: #fff;"\
"            padding: 20px;"\
"            border-radius: 8px;"\
"            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);"\
"            text-align: center;"\
"        }"\
"        #head {"\
"            border: 1px solid #CDCDCD;"\
"            height: 70px;"\
"            background-color: #008800;"\
"            color: white;"\
"            font-weight: bold;"\
"            font-size: 30px;"\
"            margin-bottom: 20px;"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"        }"\
"        #foot {"\
"            background-color: #0099FF;"\
"            border: 1px solid #ccc;"\
"            color: black;"\
"            font-size: 15px;"\
"            height: 30px;"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"            margin-top: 20px;"\
"        }"\
"        h1 {"\
"            color: black;"\
"            font-size: 18px;"\
"            margin: 20px 0;"\
"        }"\
"        #net_info_general {"\
"            min-height: 150px;"\
"            border: 1px solid #CDCDCD;"\
"            background-color: #F5F5DC;"\
"            padding: 10px;"\
"            border-radius: 8px;"\
"            margin-bottom: 20px;"\
"        }"\
"        #list_head {"\
"            background-color: #FFCC99;"\
"            height: 40px;"\
"            width: 100%;"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"            font-size: 18px;"\
"            border-radius: 8px;"\
"            margin-bottom: 10px;"\
"        }"\
"        .tab {"\
"            width: 100%;"\
"            border: none;"\
"            border-spacing: 5px;"\
"        }"\
"        .tab td {"\
"            padding: 5px;"\
"        }"\
"        .btn {"\
"            display: inline-block;"\
"            text-align: center;"\
"            cursor: pointer;"\
"            color: #fff;"\
"            background-color: #D2691E;"\
"            height: 30px;"\
"            width: 80px;"\
"            border-radius: 10pt;"\
"            border: none;"\
"            margin-top: 10px;"\
"        }"\
"        .btn:hover {"\
"            background-color: #E9967A;"\
"        }"\
"    </style>"\
"</head>"\
"<body>"\
"    <div id=\"main\">"\
"        <div id=\"head\">NTP CLOCK SLAVE</div>"\
"        <div id=\"net_info_general\">"\
"            <div id=\"list_head\"><h1>Network Information</h1></div>"\
"            <form method=\"POST\" action=\"config.cgi\">"\
"                <table class=\"tab\">"\
"                    <tbody>"\
"                        <tr>"\
"                            <td>IP Address:</td>"\
"                            <td colspan=\"2\"><input id=\"txtip\" name=\"ip\" type=\"text\" value=\"192.168.1.39\"></td>"\
"                        </tr>"\
"                        <tr>"\
"                            <td>NTP Time Server:</td>"\
"                            <td colspan=\"2\"><input id=\"txtsn\" name=\"ntpip\" type=\"text\" value=\"192.168.1.236\"></td>"\
"                        </tr>"\
"                        <tr>"\
"                            <td></td>"\
"                            <td colspan=\"2\"><input type=\"submit\" class=\"btn\" name=\"bt_setup\" value=\"Save\" id=\"bt_setup\"></td>"\
"                        </tr>"\
"                    </tbody>"\
"                </table>"\
"            </form>"\
"        </div>"\
"        <div id=\"foot\">Powered by <a style=\"color: #000000; font-weight: bold; text-decoration: none;\">Air Traffic Technical Company - <span style=\"color:red\">A</span><span style=\"color: blue;\">TTECH</span></a></div>"\
"    </div>"\
"</body>"\
"</html>"


#define fullconfightml "<!DOCTYPE html>"\
"<html lang=\"en\">"\
"<head>"\
"    <meta charset=\"UTF-8\">"\
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"\
"    <title>Clock Web Server Network Config</title>"\
"    <style>"\
"        body {"\
"            display: flex;"\
"            justify-content: center;"\
"            align-items: center;"\
"            height: 100vh;"\
"            margin: 0;"\
"            font-family: Arial, sans-serif;"\
"            background-color: #e0f7fa;"\
"        }"\
"        .container {"\
"            background-color: #fff;"\
"            border: 1px solid #0288d1;"\
"            padding: 20px;"\
"            border-radius: 8px;"\
"            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);"\
"            width: 400px;"\
"        }"\
"        .container div {"\
"            font-size: 18px;"\
"            font-weight: bold;"\
"            margin-bottom: 20px;"\
"            text-align: center;"\
"            color: #fff;"\
"            background-color: #0288d1;"\
"            padding: 10px;"\
"            border-radius: 5px;"\
"        }"\
"        ul {"\
"            padding: 0;"\
"            list-style: none;"\
"        }"\
"        li {"\
"            margin-bottom: 15px;"\
"            display: flex;"\
"            align-items: center;"\
"        }"\
"        label {"\
"            width: 80px;"\
"            text-align: right;"\
"            margin-right: 10px;"\
"            color: #0288d1;"\
"        }"\
"        input[type=\"text\"] {"\
"            flex: 1;"\
"            padding: 5px;"\
"            border: 1px solid #0288d1;"\
"            border-radius: 4px;"\
"            background-color: #f1f8e9;"\
"        }"\
"        input[type=\"radio\"] {"\
"            margin-left: 90px;"\
"            margin-top: 10px;"\
"        }"\
"        .btn {"\
"            display: inline-block;"\
"            text-align: center;"\
"            cursor: pointer;"\
"            color: #fff;"\
"            background-color: #0288d1;"\
"            height: 30px;"\
"            width: 100px;"\
"            margin: auto;"\
"            border-radius: 10pt;"\
"            border: none;"\
"            margin-top: 20px;"\
"        }"\
"        .btn:hover {"\
"            background-color: #0277bd;"\
"        }"\
"    </style>"\
"</head>"\
"<body>"\
"    <form method=\"POST\" action=\"config.cgi\">"\
"        <div class=\"container\">"\
"            <div>Web Server Network Config</div>"\
"            <ul>"\
"                <li>"\
"                    <label for=\"ip\">IP:</label>"\
"                    <input id=\"txtip\" name=\"ip\" type=\"text\" size=\"20\" value=\"192.168.22.212\">"\
"                </li>"\
"                <li>"\
"                    <label for=\"gw\">GW:</label>"\
"                    <input id=\"txtgw\" name=\"gw\" type=\"text\" size=\"20\" value=\"192.168.22.252\">"\
"                </li>"\
"                <li>"\
"                    <label for=\"sn\">SN:</label>"\
"                    <input id=\"txtsn\" name=\"sn\" type=\"text\" size=\"20\" value=\"255.255.255.0\">"\
"                </li>"\
"                <li>"\
"                    <label for=\"txtdns\">DNS:</label>"\
"                    <input id=\"txtdns\" name=\"dns\" type=\"text\" size=\"20\" value=\"8.8.4.4\">"\
"                </li>"\
"                <li>"\
"                    <label for=\"txtntp\">NTP Server:</label>"\
"                    <input id=\"txtntp\" name=\"ntpip\" type=\"text\" size=\"20\" value=\"139.199.215.251\">"\
"                </li>"\
"                <li>"\
"                    <label for=\"txtlight\">LED Light:</label>"\
"                    <input id=\"txtlight\" name=\"light\" type=\"text\" size=\"20\" value=\"15\">"\
"                </li>"\
"                <li>"\
"                    <label for=\"txtTZ\">Time Zone:</label>"\
"                    <input id=\"txtTZ\" name=\"tz\" type=\"text\" size=\"20\" value=\"0\">"\
"                </li>"\
"                <li>"\
"                    <label for=\"txtntpserver\">NTP server:</label>"\
"                    <input id=\"txtntpserver\" name=\"ntpserver\" type=\"text\" size=\"20\" value=\"0\">"\
"                </li>"\
"            </ul>"\
"            <input type=\"submit\" value=\"SAVE\" class=\"btn\">"\
"            <button type=\"button\" id=\"loadDefaultBtn\" class=\"btn\">LOAD</button>"\
"        </div>"\
"    </form>"\
"    <script>"\
"        document.getElementById('loadDefaultBtn').addEventListener('click', function() {"\
"            document.getElementById('txtip').value = '192.168.1.98';"\
"            document.getElementById('txtgw').value = '192.168.1.1';"\
"            document.getElementById('txtsn').value = '255.255.255.0';"\
"            document.getElementById('txtdns').value = '8.8.4.4';"\
"            document.getElementById('txtntp').value = '192.168.1.39';"\
"            document.getElementById('txtlight').value = '15';"\
"            document.getElementById('txtTZ').value = '0';"\
"            document.getElementById('txtntpserver').value = '0';"\
"        });"\
"    </script>"\
"</body>"\
"</html>"


#endif
