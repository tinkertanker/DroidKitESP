#ifndef Html_h
#define Html_h
#include <pgmspace.h>

extern const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no' />
        <meta charset="utf-8" />
        <title>ESP32-Droid Controller</title>
        <style>
            #container {
                width: 100%;
                height: 49vh;
                background-color: #333;
                display: flex;
                align-items: center;
                justify-content: center;
                overflow: hidden;
                border-radius: 7px;
                touch-action: none;
            }
            #item {
                width: 100px;
                height: 100px;
                background-color: rgb(245, 230, 99);
                border: 10px solid rgba(136, 136, 136, .5);
                border-radius: 50%;
                touch-action: none;
                user-select: none;
            }
            #item:active {
                background-color: rgba(168, 218, 220, 1.00);
            }
            #item:hover {
                cursor: pointer;
                border-width: 20px;
            }
            #area {
                position: fixed;
                right: 0;
                top: 0;
            }
            p {
                font-family: Arial, Helvetica, sans-serif;
                font-size: 32px;
            }
        </style>
        <script type="text/javascript">
            var JoyStick = function (t, e) { var i = void 0 === (e = e || {}).title ? "joystick" : e.title, n = void 0 === e.width ? 0 : e.width, o = void 0 === e.height ? 0 : e.height, h = void 0 === e.internalFillColor ? "#00AA00" : e.internalFillColor, r = void 0 === e.internalLineWidth ? 2 : e.internalLineWidth, d = void 0 === e.internalStrokeColor ? "#003300" : e.internalStrokeColor, a = void 0 === e.externalLineWidth ? 2 : e.externalLineWidth, l = void 0 === e.externalStrokeColor ? "#008000" : e.externalStrokeColor, c = document.getElementById(t), u = document.createElement("canvas"); u.id = i, 0 == n && (n = c.clientWidth), 0 == o && (o = c.clientHeight), u.width = n, u.height = o, c.appendChild(u); var s = u.getContext("2d"), f = 0, v = 2 * Math.PI, g = (u.width - 110) / 2, w = g + 5, C = g + 30, m = u.width / 2, p = u.height / 2, L = u.width / 10, E = -1 * L, S = u.height / 10, k = -1 * S, W = m, G = p; function x() { s.beginPath(), s.arc(m, p, C, 0, v, !1), s.lineWidth = a, s.strokeStyle = l, s.stroke() } function y() { s.beginPath(), W < g && (W = w), W + g > u.width && (W = u.width - w), G < g && (G = w), G + g > u.height && (G = u.height - w), s.arc(W, G, g, 0, v, !1); var t = s.createRadialGradient(m, p, 5, m, p, 200); t.addColorStop(0, h), t.addColorStop(1, d), s.fillStyle = t, s.fill(), s.lineWidth = r, s.strokeStyle = d, s.stroke() } "ontouchstart" in document.documentElement ? (u.addEventListener("touchstart", function (t) { f = 1 }, !1), u.addEventListener("touchmove", function (t) { t.preventDefault(), 1 == f && (W = t.touches[0].pageX, G = t.touches[0].pageY, W -= u.offsetLeft, G -= u.offsetTop, s.clearRect(0, 0, u.width, u.height), x(), y()) }, !1), u.addEventListener("touchend", function (t) { f = 0, W = m, G = p, s.clearRect(0, 0, u.width, u.height), x(), y() }, !1)) : (u.addEventListener("mousedown", function (t) { f = 1 }, !1), u.addEventListener("mousemove", function (t) { 1 == f && (W = t.pageX, G = t.pageY, W -= u.offsetLeft, G -= u.offsetTop, s.clearRect(0, 0, u.width, u.height), x(), y()) }, !1), u.addEventListener("mouseup", function (t) { f = 0, W = m, G = p, s.clearRect(0, 0, u.width, u.height), x(), y() }, !1)), x(), y(), this.GetWidth = function () { return u.width }, this.GetHeight = function () { return u.height }, this.GetPosX = function () { return W }, this.GetPosY = function () { return G }, this.GetX = function () { return ((W - m) / w * 100).toFixed() }, this.GetY = function () { return ((G - p) / w * 100 * -1).toFixed() }, this.GetDir = function () { var t = "", e = W - m, i = G - p; return i >= k && i <= S && (t = "C"), i < k && (t = "N"), i > S && (t = "S"), e < E && ("C" == t ? t = "W" : t += "W"), e > L && ("C" == t ? t = "E" : t += "E"), t } };
        </script>
    </head>
    <body>
        <p>Speed: <span id="speed">0</span> (-255 to 255)</p>
        <p>Angle: <span id="angle">0</span> (-60 to 60)</p>
        <div id='outerContainer'>
            <div id="joyDiv" style="width:200px;height:200px;margin:auto;"></div>
        </div>
        <script>
            window.addEventListener('load', function() {
                const view = document.getElementById('stream');
                var ws = new WebSocket(`ws://${window.location.hostname}/ws`);
                ws.onopen = function(event) {
                    console.log('Connection established');
                }
                ws.onclose = function(event) {
                    console.log('Connection died');
                }
                ws.onerror = function(error) {
                    console.log('error');
                };
                ws.onmessage = function(event) {
                    console.log(event.data);
                    var myObj = JSON.parse(event.data);
                    var keys = Object.keys(myObj);

                    for (var i = 0; i < keys.length; i++){
                        var key = keys[i];
                        document.getElementById(key).innerHTML = myObj[key];
                    }
                };
                
                var lastText, lastSend, sendTimeout;
                // limit sending to one message every 30 ms
                // https://github.com/neonious/lowjs_esp32_examples/blob/master/neonious_one/cellphone_controlled_rc_car/www/index.html
                function send(txt) {
                    var now = new Date().getTime();
                    if (lastSend === undefined || now - lastSend >= 30) {
                        try {
                            ws.send(txt);
                            lastSend = new Date().getTime();
                            return;
                        } catch (e) {
                            console.log(e);
                        }
                    }
                    lastText = txt;
                    if (!sendTimeout) {
                        var ms = lastSend !== undefined ? 30 - (now - lastSend) : 30;
                        if (ms < 0)
                            ms = 0;
                        sendTimeout = setTimeout(() => {
                            sendTimeout = null;
                            send(lastText);
                        }, ms);
                    }
                }
                // Create JoyStick object into the DIV 'joyDiv'
                var joy = new JoyStick('joyDiv');
                var inputPosX = document.getElementById("posizioneX");
                var inputPosY = document.getElementById("posizioneY");
                var direzione = document.getElementById("direzione");
                var fuerzaI = document.getElementById("fuerzaI");
                var fuerzaD = document.getElementById("fuerzaD");
                var x = document.getElementById("X");
                var y = document.getElementById("Y");
                setInterval(function () { send(joy.GetX()+'&'+joy.GetY()); }, 100);
            });
        </script>
    </body>
</html>
)rawliteral";

#endif