#include <WiFi.h>
#include <ArduinoOTA.h>

const char htmlPage[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Rover Control</title>
<style>
  * { margin: 0; padding: 0; box-sizing: border-box; -webkit-tap-highlight-color: transparent; }

  body {
    background: #0f1117;
    color: #e0e6ed;
    font-family: 'Segoe UI', system-ui, sans-serif;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
  }

  /* HEADER */
  header {
    padding: 1rem 1.5rem;
    border-bottom: 1px solid #1e2530;
    display: flex;
    align-items: center;
    justify-content: space-between;
  }

  .title { font-size: 1.1rem; font-weight: 600; color: #fff; letter-spacing: 0.03em; }
  .title span { font-size: 0.7rem; color: #556070; font-weight: 400; margin-left: 8px; }

  .status {
    display: flex;
    align-items: center;
    gap: 6px;
    font-size: 0.72rem;
    color: #556070;
  }

  .dot {
    width: 7px; height: 7px;
    border-radius: 50%;
    background: #22c55e;
    box-shadow: 0 0 6px #22c55e;
    animation: blink 2s infinite;
  }

  @keyframes blink { 0%,100%{opacity:1} 50%{opacity:0.3} }

  /* PAGE */
  .page {
    flex: 1;
    padding: 1.2rem;
    display: flex;
    flex-direction: column;
    gap: 1rem;
    max-width: 500px;
    margin: 0 auto;
    width: 100%;
  }

  /* SENSORS */
  .sensor-row { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 8px; }

  .sensor {
    background: #161c24;
    border: 1px solid #1e2530;
    border-radius: 8px;
    padding: 0.8rem 0.6rem;
    text-align: center;
  }

  .sensor-label { font-size: 0.6rem; color: #556070; text-transform: uppercase; letter-spacing: 0.1em; margin-bottom: 4px; }
  .sensor-val { font-size: 1.3rem; font-weight: 700; color: #38bdf8; }
  .sensor-val.warn { color: #f87171; }
  .sensor-unit { font-size: 0.55rem; color: #556070; margin-top: 1px; }

  /* MODE */
  .mode-row { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; }

  .mode-btn {
    padding: 0.65rem 1rem;
    border-radius: 8px;
    border: 1px solid #1e2530;
    background: #161c24;
    color: #556070;
    font-size: 0.75rem;
    font-weight: 500;
    cursor: pointer;
    text-decoration: none;
    text-align: center;
    transition: all 0.15s;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 6px;
  }

  .mode-btn:hover { border-color: #2e3a4a; color: #94a3b8; }
  .mode-btn.active-auto   { border-color: #22c55e; color: #22c55e; background: #0f2018; }
  .mode-btn.active-manual { border-color: #f59e0b; color: #f59e0b; background: #1c1608; }

  /* D-PAD */
  .dpad-section {
    background: #161c24;
    border: 1px solid #1e2530;
    border-radius: 10px;
    padding: 1.2rem 1rem;
  }

  .section-title {
    font-size: 0.6rem;
    text-transform: uppercase;
    letter-spacing: 0.12em;
    color: #556070;
    margin-bottom: 1rem;
  }

  .dpad {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    gap: 8px;
    max-width: 210px;
    margin: 0 auto;
  }

  .btn {
    aspect-ratio: 1;
    background: #0f1117;
    border: 1px solid #1e2530;
    border-radius: 8px;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    text-decoration: none;
    color: #94a3b8;
    transition: all 0.1s;
    user-select: none;
    -webkit-user-select: none;
    font-size: 1.1rem;
  }

  .btn:hover { border-color: #38bdf8; color: #38bdf8; background: #0c1c26; }
  .btn:active, .btn.pressed { background: #0c1c26; border-color: #38bdf8; transform: scale(0.92); color: #38bdf8; }

  .btn.stop {
    background: #1a0d10;
    border-color: #3a1520;
    color: #f87171;
    font-size: 0.65rem;
    font-weight: 600;
    letter-spacing: 0.05em;
  }

  .btn.stop:hover { border-color: #f87171; }
  .btn.stop:active, .btn.stop.pressed { border-color: #f87171; transform: scale(0.92); }
  .btn.empty { background: transparent; border-color: transparent; pointer-events: none; }

  /* BOTTOM ROW */
  .bottom-row {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 8px;
  }

  .info-box {
    background: #161c24;
    border: 1px solid #1e2530;
    border-radius: 8px;
    padding: 0.8rem 1rem;
  }

  .info-label { font-size: 0.6rem; color: #556070; text-transform: uppercase; letter-spacing: 0.1em; margin-bottom: 5px; }
  .info-val { font-size: 0.78rem; color: #e0e6ed; font-weight: 500; }
  .info-val.green { color: #22c55e; }
  .info-val.red { color: #f87171; }
  .info-val.blue { color: #38bdf8; }

  /* LOG */
  .log-box {
    background: #161c24;
    border: 1px solid #1e2530;
    border-radius: 8px;
    padding: 0.8rem 1rem;
  }

  .log-inner {
    font-size: 0.65rem;
    color: #556070;
    line-height: 1.7;
    max-height: 80px;
    overflow-y: auto;
  }

  .log-inner .c { color: #38bdf8; }
  .log-inner .g { color: #22c55e; }
  .log-inner .r { color: #f87171; }

  /* TELEM */
  .telem {
    display: flex;
    justify-content: space-between;
    font-size: 0.62rem;
    color: #556070;
    padding: 0 2px 0.5rem;
  }

  .telem b { color: #94a3b8; font-weight: 500; }

  /* KEYBOARD HINT */
  .kbd-hint {
    display: none;
    font-size: 0.62rem;
    color: #556070;
    background: #161c24;
    border: 1px solid #1e2530;
    border-radius: 6px;
    padding: 0.5rem 0.8rem;
    text-align: center;
  }

  kbd {
    background: #1e2530;
    border-radius: 3px;
    padding: 1px 5px;
    font-size: 0.6rem;
    color: #94a3b8;
    font-family: inherit;
  }

  /* DESKTOP */
  @media (min-width: 860px) {
    .kbd-hint { display: block; }

    .page {
      max-width: 1000px;
      display: grid;
      grid-template-columns: 360px 1fr;
      grid-template-rows: auto auto auto 1fr auto;
      grid-template-areas:
        "sensors  sensors"
        "modes    modes"
        "dpad     right"
        "dpad     right"
        "telem    telem";
      gap: 1rem;
      align-items: start;
    }

    .sensor-row  { grid-area: sensors; }
    .mode-row    { grid-area: modes; }
    .dpad-section{ grid-area: dpad; }
    .right-col   { grid-area: right; display: flex; flex-direction: column; gap: 1rem; }
    .telem       { grid-area: telem; padding-bottom: 0; }
    .kbd-hint    { grid-column: 1 / -1; }

    .sensor-val { font-size: 1.6rem; }
    .dpad { max-width: 240px; gap: 10px; }
    .log-inner { max-height: 130px; }
  }
</style>
</head>
<body>

<header>
  <div class="title">Rover Control <span>ESP32</span></div>
  <div class="status"><div class="dot"></div> Connected</div>
</header>

<div class="page">

  <!-- Sensors -->
  <div class="sensor-row">
    <div class="sensor">
      <div class="sensor-label">Left</div>
      <div class="sensor-val" id="distL">—</div>
      <div class="sensor-unit">cm</div>
    </div>
    <div class="sensor">
      <div class="sensor-label">Front</div>
      <div class="sensor-val" id="distF">—</div>
      <div class="sensor-unit">cm</div>
    </div>
    <div class="sensor">
      <div class="sensor-label">Right</div>
      <div class="sensor-val" id="distR">—</div>
      <div class="sensor-unit">cm</div>
    </div>
  </div>

  <!-- Mode -->
  <div class="mode-row">
    <a href="/auto" class="mode-btn" id="btn-auto" onclick="setMode('auto');return false;">
      <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><path d="M12 1v4M12 19v4M4.22 4.22l2.83 2.83M16.95 16.95l2.83 2.83M1 12h4M19 12h4M4.22 19.78l2.83-2.83M16.95 7.05l2.83-2.83"/></svg>
      Auto Mode
    </a>
    <a href="/manual" class="mode-btn" id="btn-manual" onclick="setMode('manual');return false;">
      <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="2" y="7" width="20" height="15" rx="2"/><path d="M16 7V5a2 2 0 0 0-4 0v2M12 12v5M9.5 14.5l2.5-2.5 2.5 2.5"/></svg>
      Manual Mode
    </a>
  </div>

  <!-- D-pad -->
  <div class="dpad-section">
    <div class="section-title">Directional Control</div>
    <div class="dpad">
      <div class="btn empty"></div>
      <a href="/forward" class="btn" id="btn-fwd">▲</a>
      <div class="btn empty"></div>

      <a href="/left" class="btn" id="btn-left">◀</a>
      <a href="/stop" class="btn stop" id="btn-stop">STOP</a>
      <a href="/right" class="btn" id="btn-right">▶</a>

      <div class="btn empty"></div>
      <a href="/back" class="btn" id="btn-back">▼</a>
      <div class="btn empty"></div>
    </div>
  </div>

  <!-- Right column -->
  <div class="right-col">

    <div class="bottom-row">
      <div class="info-box">
        <div class="info-label">Status</div>
        <div class="info-val green" id="statusText">Idle</div>
      </div>
      <div class="info-box">
        <div class="info-label">Mode</div>
        <div class="info-val blue" id="modeDisp">Auto</div>
      </div>
      <div class="info-box">
        <div class="info-label">Packets</div>
        <div class="info-val" id="pktDisp">0</div>
      </div>
      <div class="info-box">
        <div class="info-label">Uptime</div>
        <div class="info-val" id="uptimeDisp">00:00</div>
      </div>
    </div>

    <div class="log-box">
      <div class="info-label" style="margin-bottom:6px;">Log</div>
      <div class="log-inner" id="logBox">
        <span class="g">System ready</span><br>
        <span class="c">Awaiting command...</span>
      </div>
    </div>

  </div>

  <!-- Keyboard hint (desktop) -->
  <div class="kbd-hint">
    <kbd>W</kbd> / <kbd>↑</kbd> Forward &nbsp;·&nbsp;
    <kbd>S</kbd> / <kbd>↓</kbd> Reverse &nbsp;·&nbsp;
    <kbd>A</kbd> / <kbd>←</kbd> Left &nbsp;·&nbsp;
    <kbd>D</kbd> / <kbd>→</kbd> Right &nbsp;·&nbsp;
    <kbd>Space</kbd> Stop
  </div>

  <!-- Telemetry -->
  <div class="telem">
    <span>Signal <b id="sigDisp">98%</b></span>
    <span>IP <b>192.168.x.x</b></span>
    <span>Uptime <b id="uptimeTelem">00:00</b></span>
    <span>Pkts <b id="pktTelem">0</b></span>
  </div>

</div>

<script>
  let pkt = 0, pathLog = '';
  const start = Date.now();

  const actionMap = {
    'btn-fwd':   { url:'forward', label:'Forward',    cls:'blue' },
    'btn-back':  { url:'back',    label:'Reverse',    cls:'blue' },
    'btn-left':  { url:'left',    label:'Turn Left',  cls:'blue' },
    'btn-right': { url:'right',   label:'Turn Right', cls:'blue' },
    'btn-stop':  { url:'stop',    label:'Stopped',    cls:'red'  }
  };

  function doAction(id) {
    const a = actionMap[id];
    if (!a) return;
    const st = document.getElementById('statusText');
    st.textContent = a.label;
    st.className = 'info-val ' + a.cls;
    addLog(a.label);
    pkt++;
    document.getElementById('pktDisp').textContent  = pkt;
    document.getElementById('pktTelem').textContent = pkt;
    fetch('/' + a.url).catch(() => {});
  }

  function press(id) {
    document.getElementById(id)?.classList.add('pressed');
    doAction(id);
  }

  function release(id) {
    setTimeout(() => document.getElementById(id)?.classList.remove('pressed'), 140);
  }

  function setMode(m) {
    document.getElementById('btn-auto').className   = 'mode-btn' + (m === 'auto'   ? ' active-auto'   : '');
    document.getElementById('btn-manual').className = 'mode-btn' + (m === 'manual' ? ' active-manual' : '');
    document.getElementById('modeDisp').textContent = m === 'auto' ? 'Auto' : 'Manual';
    document.getElementById('modeDisp').className   = 'info-val ' + (m === 'auto' ? 'green' : 'info-val');
    addLog(m === 'auto' ? 'Auto mode on' : 'Manual mode on', m === 'auto' ? 'g' : 'r');
    fetch('/' + m).catch(() => {});
  }

  function addLog(msg, cls) {
    const box = document.getElementById('logBox');
    const br = document.createElement('br');
    const sp = document.createElement('span');
    sp.className = cls || 'c';
    sp.textContent = msg;
    box.appendChild(br);
    box.appendChild(sp);
    while (box.children.length > 30) box.removeChild(box.firstChild);
    box.scrollTop = box.scrollHeight;
  }

  // Keyboard
  const keyMap = { 'arrowup':'btn-fwd','w':'btn-fwd','arrowdown':'btn-back','s':'btn-back','arrowleft':'btn-left','a':'btn-left','arrowright':'btn-right','d':'btn-right',' ':'btn-stop' };
  const held = new Set();

  document.addEventListener('keydown', e => {
    const id = keyMap[e.key.toLowerCase()];
    if (!id || held.has(id)) return;
    e.preventDefault(); held.add(id); press(id);
  });

  document.addEventListener('keyup', e => {
    const id = keyMap[e.key.toLowerCase()];
    if (!id) return; held.delete(id); release(id);
  });

  // Touch + click
  document.querySelectorAll('.btn:not(.empty)').forEach(b => {
    b.addEventListener('touchstart', e => { e.preventDefault(); press(b.id); }, { passive: false });
    b.addEventListener('touchend',   e => { e.preventDefault(); release(b.id); }, { passive: false });
    b.addEventListener('click',      e => { e.preventDefault(); doAction(b.id); });
  });

  // Sensors
  function fakeSensors() {
    ['F','L','R'].forEach(d => {
      const v = (Math.random() * 200 + 5).toFixed(1);
      const el = document.getElementById('dist' + d);
      el.textContent = v;
      el.className = 'sensor-val' + (parseFloat(v) < 25 ? ' warn' : '');
    });
  }

  function tick() {
    const e = Math.floor((Date.now() - start) / 1000);
    const t = String(Math.floor(e/60)).padStart(2,'0') + ':' + String(e%60).padStart(2,'0');
    document.getElementById('uptimeDisp').textContent  = t;
    document.getElementById('uptimeTelem').textContent = t;
    document.getElementById('sigDisp').textContent = (94 + Math.floor(Math.random()*6)) + '%';
  }

  setMode('auto');
  setInterval(fakeSensors, 2000);
  setInterval(tick, 1000);
  fakeSensors();
</script>
</body>
</html>
)rawhtml";

// -------- WIFI --------
const char* ssid = "Sam33";
const char* password = "123456789";

// -------- MOTOR PINS --------
#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25

// -------- SENSOR PINS --------
#define TRIG_F 5
#define ECHO_F 18
#define TRIG_L 19
#define ECHO_L 21
#define TRIG_R 22
#define ECHO_R 23

// -------- TUNING --------
const float THRESH_FWD  = 25.0;
const float THRESH_SIDE = 25.0;

float distF = 0, distL = 0, distR = 0;
String pathLog = "";

// -------- MODE --------
bool manualMode = false;

// -------- WEB SERVER --------
WiFiServer server(80);

// -------- MOTOR CONTROL --------
void setLeftMotor(int dir) {
  if (dir == 1) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  } else if (dir == -1) {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  }
}

void setRightMotor(int dir) {
  if (dir == 1) {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  } else if (dir == -1) {
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  }
}

void forward() {
  setLeftMotor(1);
  setRightMotor(1);
}

void left() {
  setLeftMotor(-1);
  setRightMotor(1);
}

void right() {
  setLeftMotor(1);
  setRightMotor(-1);
}

void backward() {
  setLeftMotor(-1);
  setRightMotor(-1);
}

void stopBot() {
  setLeftMotor(0);
  setRightMotor(0);
}

// -------- SENSOR --------
float readDistance(int trig, int echo) {
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000);
  if (duration == 0) return 300;

  return duration * 0.034 / 2;
}

// -------- MOBILE CONTROL --------
void handleWebControl() {
  WiFiClient client = server.available();

  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/forward") != -1) forward();
    else if (request.indexOf("/back") != -1) backward();
    else if (request.indexOf("/left") != -1) left();
    else if (request.indexOf("/right") != -1) right();
    else if (request.indexOf("/stop") != -1) stopBot();
    else if (request.indexOf("/manual") != -1) manualMode = true;
    else if (request.indexOf("/auto") != -1) manualMode = false;

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println(htmlPage);
    client.stop();
  }
}

// -------- SETUP --------
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  pinMode(TRIG_F, OUTPUT); pinMode(ECHO_F, INPUT);
  pinMode(TRIG_L, OUTPUT); pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, OUTPUT); pinMode(ECHO_R, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  server.begin();
  ArduinoOTA.begin();

  delay(1500);
}

// -------- LOOP --------
void loop() {

  ArduinoOTA.handle();
  handleWebControl();

  // MANUAL MODE → skip auto
  if (manualMode) return;

  // -------- AUTO MODE --------
  distF = readDistance(TRIG_F, ECHO_F);
  delay(20);
  distL = readDistance(TRIG_L, ECHO_L);
  delay(20);
  distR = readDistance(TRIG_R, ECHO_R);
  delay(20);

  Serial.print("F: "); Serial.print(distF);
  Serial.print(" L: "); Serial.print(distL);
  Serial.print(" R: "); Serial.println(distR);

  if (distF > THRESH_FWD) {
    forward();
    pathLog += 'F';
    delay(200);
  }
  else if (distL > THRESH_SIDE) {
    left();
    pathLog += 'L';
    delay(300);
  }
  else if (distR > THRESH_SIDE) {
    right();
    pathLog += 'R';
    delay(300);
  }
  else {
    right(); delay(300);
    right(); delay(300);
  }

  stopBot();
  delay(100);
}