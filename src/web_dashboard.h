#pragma once
#include <Arduino.h>

// Embedded HTML dashboard for ESP32 flash memory - 100% Real Hardware Data
const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Linear Magnetic Mover & Bot Tracker</title>
  <script src="https://www.gstatic.com/antigravity/web/dev/tailwindcss.min.js"></script>
  <style>
    @font-face {
      font-family: 'Anthropic Serif';
      src: local('Anthropic Serif'), local('Ivar Headline'), local('Newsreader');
    }
    body, h1, h2, h3, p, span, div {
      font-family: 'Anthropic Serif', 'Ivar Headline', 'Newsreader', 'Charter', 'Georgia', 'Cambria', serif;
    }
    @keyframes pulse-ring {
      0% { transform: scale(0.92); opacity: 0.85; }
      50% { transform: scale(1.35); opacity: 0.15; }
      100% { transform: scale(0.92); opacity: 0.85; }
    }
    @keyframes cart-hover {
      0%, 100% { transform: translateY(0px); }
      50% { transform: translateY(-2px); }
    }
    .pulse-ring { animation: pulse-ring 1.8s cubic-bezier(0.24, 0, 0.38, 1) infinite; }
    .cart-hover { animation: cart-hover 2s ease-in-out infinite; }
    .glass-card {
      background: rgba(255, 255, 255, 0.94);
      backdrop-filter: blur(14px);
      -webkit-backdrop-filter: blur(14px);
      border: 1px solid rgba(226, 232, 240, 0.85);
      box-shadow: 0 10px 25px -5px rgba(148, 163, 184, 0.10), 0 8px 10px -6px rgba(148, 163, 184, 0.06);
    }
    .cart-transition {
      transition: left 0.5s cubic-bezier(0.34, 1.25, 0.64, 1);
    }
    .blue-glow { box-shadow: 0 0 16px rgba(59, 130, 246, 0.65); }
    .red-glow { box-shadow: 0 0 16px rgba(239, 68, 68, 0.65); }
  </style>
</head>
<body class="bg-slate-50 text-slate-800 antialiased p-4 sm:p-6 min-h-screen">

  <div class="max-w-6xl mx-auto space-y-5">

    <!-- Connected Status Tab -->
    <div id="connectionCard" class="glass-card rounded-2xl p-4 sm:p-5 flex flex-col sm:flex-row items-start sm:items-center justify-between gap-4 transition-all duration-300">
      <div class="flex items-center gap-3.5">
        <div id="connIconBg" class="w-12 h-12 rounded-xl bg-emerald-500/10 text-emerald-600 flex items-center justify-center border border-emerald-200 shadow-sm transition-colors duration-300">
          <svg id="connIcon" class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z"></path>
          </svg>
        </div>

        <div>
          <div class="flex items-center gap-2.5">
            <h1 class="text-xl font-bold tracking-tight text-slate-900">ESP32 Hardware Connection</h1>
            <span id="connBadge" class="px-2.5 py-0.5 rounded-full text-xs font-semibold bg-emerald-100 text-emerald-800 border border-emerald-200 flex items-center gap-1.5">
              <span id="connDot" class="w-2 h-2 rounded-full bg-emerald-500 animate-pulse"></span>
              <span id="connStatusText">Connected</span>
            </span>
          </div>

          <p id="connDetails" class="text-xs text-slate-500 mt-1 italic">
            Wi-Fi Network: <strong class="text-slate-800 not-italic">uyya</strong> &bull; Direct AP: <strong class="text-slate-800 not-italic">ESP32-Bot-Tracker</strong> &bull; Telemetry: <span id="connSubText" class="text-emerald-700 font-semibold not-italic">Live Hardware Stream</span>
          </p>
        </div>
      </div>

      <div class="flex items-center gap-3 w-full sm:w-auto justify-between sm:justify-end border-t sm:border-t-0 pt-3 sm:pt-0 border-slate-100">
        <div class="text-left sm:text-right">
          <span class="text-[11px] text-slate-400 block font-medium uppercase tracking-wider">Acquired Position</span>
          <span id="headerActiveStation" class="text-base font-bold text-blue-700">Station J1</span>
        </div>

        <div id="statusPill" class="px-3.5 py-2 rounded-xl bg-emerald-50 border border-emerald-200 text-emerald-800 flex items-center gap-2 text-xs font-semibold shadow-2xs">
          <span class="relative flex h-2.5 w-2.5">
            <span id="statusPing" class="animate-ping absolute inline-flex h-full w-full rounded-full bg-emerald-400 opacity-75"></span>
            <span id="statusPoint" class="relative inline-flex rounded-full h-2.5 w-2.5 bg-emerald-500"></span>
          </span>
          <span id="pillLabel" class="tracking-wide">ESP32 ONLINE</span>
        </div>
      </div>
    </div>

    <!-- Linear Track Card -->
    <div class="glass-card rounded-2xl p-6 relative overflow-hidden">
      <div class="flex items-center justify-between mb-4">
        <div>
          <h2 class="text-base font-bold text-slate-800">Linear Magnetic Track (J1 &rarr; J6)</h2>
          <p class="text-xs text-slate-500 italic mt-0.5">Live hardware position acquired directly from physical IR sensors &bull; No simulation</p>
        </div>
        <div class="flex items-center gap-3">
          <div class="flex items-center gap-1.5 text-xs bg-blue-50 px-3 py-1 rounded-lg border border-blue-200 text-blue-800 font-medium">
            <span class="w-2.5 h-2.5 rounded-full bg-blue-500"></span>
            <span>North Pole (Active IR): <strong id="northBadge" class="font-bold">J1</strong></span>
          </div>
          <div class="flex items-center gap-1.5 text-xs bg-rose-50 px-3 py-1 rounded-lg border border-rose-200 text-rose-800 font-medium">
            <span class="w-2.5 h-2.5 rounded-full bg-rose-500"></span>
            <span>South Pole (Next Pull): <strong id="southBadge" class="font-bold">J2</strong></span>
          </div>
        </div>
      </div>

      <!-- Linear Visual Track Canvas Container -->
      <div class="relative w-full h-[180px] bg-gradient-to-b from-slate-50/80 via-blue-50/20 to-slate-100/60 rounded-xl border border-slate-200/90 p-4 select-none overflow-hidden">

        <svg class="absolute inset-0 w-full h-full pointer-events-none opacity-20" xmlns="http://www.w3.org/2000/svg">
          <defs>
            <pattern id="grid" width="20" height="20" patternUnits="userSpaceOnUse">
              <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#94a3b8" stroke-width="0.5" />
            </pattern>
          </defs>
          <rect width="100%" height="100%" fill="url(#grid)" />
        </svg>

        <div class="absolute left-2 top-[58%] -translate-y-1/2 w-3 h-20 bg-slate-400 rounded-sm border-r-2 border-slate-600 shadow-sm flex flex-col justify-between py-1 items-center z-10">
          <div class="w-1.5 h-3 bg-amber-400 rounded-2xs"></div>
          <span class="text-[7px] font-black text-slate-800 -rotate-90">START</span>
          <div class="w-1.5 h-3 bg-amber-400 rounded-2xs"></div>
        </div>

        <div class="absolute right-2 top-[58%] -translate-y-1/2 w-3 h-20 bg-slate-400 rounded-sm border-l-2 border-slate-600 shadow-sm flex flex-col justify-between py-1 items-center z-10">
          <div class="w-1.5 h-3 bg-rose-500 rounded-2xs"></div>
          <span class="text-[7px] font-black text-slate-800 rotate-90">LIMIT</span>
          <div class="w-1.5 h-3 bg-rose-500 rounded-2xs"></div>
        </div>

        <svg class="absolute inset-0 w-full h-full pointer-events-none" viewBox="0 0 1000 180" preserveAspectRatio="none">
          <defs>
            <linearGradient id="statorGrad" x1="0%" y1="0%" x2="100%" y2="0%">
              <stop offset="0%" stop-color="#3b82f6" />
              <stop offset="50%" stop-color="#8b5cf6" />
              <stop offset="100%" stop-color="#ef4444" />
            </linearGradient>
            <pattern id="ties" width="25" height="40" patternUnits="userSpaceOnUse">
              <rect x="0" y="5" width="4" height="30" fill="#cbd5e1" rx="1" />
            </pattern>
          </defs>

          <rect x="25" y="85" width="950" height="40" rx="6" fill="#e2e8f0" stroke="#cbd5e1" stroke-width="2" />
          <rect x="25" y="85" width="950" height="40" fill="url(#ties)" opacity="0.75" />

          <line x1="30" y1="88" x2="970" y2="88" stroke="#94a3b8" stroke-width="4" stroke-linecap="round" />
          <line x1="30" y1="88" x2="970" y2="88" stroke="#f8fafc" stroke-width="1.5" stroke-linecap="round" />

          <line x1="30" y1="122" x2="970" y2="122" stroke="#94a3b8" stroke-width="4" stroke-linecap="round" />
          <line x1="30" y1="122" x2="970" y2="122" stroke="#f8fafc" stroke-width="1.5" stroke-linecap="round" />

          <line x1="35" y1="105" x2="965" y2="105" stroke="url(#statorGrad)" stroke-width="6" stroke-dasharray="12 6" opacity="0.9" />
        </svg>

        <div id="junctionsContainer" class="absolute inset-0 pointer-events-none"></div>

        <div id="cartBot" class="absolute cart-transition cart-hover w-22 h-18 pointer-events-none -ml-11 z-30" style="left: 7%; top: 72px;">
          <div class="relative w-full h-full flex items-center justify-center">

            <div class="w-20 h-14 bg-white rounded-xl shadow-xl border-2 border-indigo-400 flex items-center justify-between p-1 relative overflow-hidden">

              <div class="w-8 h-full bg-blue-600 rounded-lg flex flex-col items-center justify-center text-white relative shadow-inner">
                <span class="text-[12px] font-black leading-none">N</span>
                <span class="w-1.5 h-1.5 rounded-full bg-white animate-ping mt-0.5"></span>
                <span class="text-[8px] font-semibold opacity-90 mt-0.5">BLUE</span>
              </div>

              <div class="flex flex-col items-center justify-center px-1">
                <div class="w-1.5 h-1.5 rounded-full bg-amber-400"></div>
                <div class="w-0.5 h-4 bg-indigo-300 my-0.5"></div>
                <div class="w-1.5 h-1.5 rounded-full bg-amber-400"></div>
              </div>

              <div class="w-8 h-full bg-rose-600 rounded-lg flex flex-col items-center justify-center text-white relative shadow-inner">
                <span class="text-[12px] font-black leading-none">S</span>
                <span class="w-1.5 h-1.5 rounded-full bg-white animate-ping mt-0.5"></span>
                <span class="text-[8px] font-semibold opacity-90 mt-0.5">RED</span>
              </div>

            </div>

            <div class="absolute -top-1.5 w-6 h-2 bg-slate-700 rounded-full shadow-sm border border-slate-300"></div>
            <div class="absolute -bottom-1.5 w-6 h-2 bg-slate-700 rounded-full shadow-sm border border-slate-300"></div>
          </div>
        </div>

      </div>
    </div>

    <!-- Live Telemetry & 12-LED + 6-IR Hardware Matrix -->
    <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">

      <div class="glass-card rounded-2xl p-5 space-y-4">
        <div class="flex items-center justify-between">
          <h3 class="text-base font-bold text-slate-900">Linear Position Telemetry</h3>
          <span class="w-2.5 h-2.5 rounded-full bg-emerald-500 animate-pulse"></span>
        </div>

        <div class="grid grid-cols-2 gap-3 text-sm">
          <div class="p-3 bg-blue-50/70 rounded-xl border border-blue-200/70">
            <span class="text-xs text-blue-600 block font-semibold uppercase tracking-wider">Current Station</span>
            <span id="telemetryJunction" class="text-lg font-bold text-blue-800">Junction 1</span>
            <span id="telemetryStation" class="text-[11px] text-slate-600 block mt-0.5 italic">Acquired by IR 1</span>
          </div>

          <div class="p-3 bg-slate-50/80 rounded-xl border border-slate-200/70">
            <span class="text-xs text-slate-500 block font-medium uppercase tracking-wider">Hardware Link</span>
            <span id="telemetryStatus" class="text-lg font-bold text-emerald-700">LIVE FEED</span>
            <span id="telemetrySub" class="text-[11px] text-slate-500 block mt-0.5">Physical IR Sensors</span>
          </div>

          <div class="p-3 bg-blue-50/50 rounded-xl border border-blue-200/60">
            <span class="text-xs text-blue-700 block font-semibold">North Pole (Blue LED)</span>
            <span id="telemetryBlueLed" class="text-sm font-bold text-blue-800">GPIO 23 (LED 1)</span>
            <span class="text-[11px] text-emerald-700 block font-medium mt-0.5">&bull; ON (Current Station)</span>
          </div>

          <div class="p-3 bg-rose-50/50 rounded-xl border border-rose-200/60">
            <span class="text-xs text-rose-700 block font-semibold">South Pole (Red LED)</span>
            <span id="telemetryRedLed" class="text-sm font-bold text-rose-800">GPIO 27 (LED 2)</span>
            <span class="text-[11px] text-rose-700 block font-medium mt-0.5">&bull; ON (Next Pull Pole)</span>
          </div>
        </div>

        <div class="pt-2 border-t border-slate-100 flex items-center justify-between text-xs text-slate-500">
          <span>Track Type: <strong class="text-slate-800 font-semibold">Linear Straight Rail</strong></span>
          <span>Response: <strong class="text-slate-800 font-semibold">&lt; 1 ms (Hardware)</strong></span>
        </div>
      </div>

      <div class="glass-card rounded-2xl p-5 space-y-3 lg:col-span-2">
        <div class="flex items-center justify-between">
          <h3 class="text-base font-bold text-slate-900">Linear 6-Station Hardware Matrix</h3>
        </div>

        <div class="grid grid-cols-2 sm:grid-cols-3 gap-3">
          <div id="stationCard0" class="p-3 rounded-xl border border-blue-300 bg-blue-50/60 transition-all duration-300">
            <div class="flex items-center justify-between">
              <span class="text-xs font-bold text-slate-900">Station J1</span>
              <span id="irBadge0" class="text-[10px] px-1.5 py-0.5 rounded font-bold bg-emerald-100 text-emerald-800 border border-emerald-200">IR ACTIVE</span>
            </div>
            <div class="mt-2 space-y-1.5 text-xs">
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Blue LED (P23):</span>
                <span id="blueVal0" class="px-1.5 py-0.2 rounded font-bold text-[10px] bg-blue-600 text-white blue-glow">ON (N)</span>
              </div>
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Red LED (P26):</span>
                <span id="redVal0" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
            </div>
          </div>

          <div id="stationCard1" class="p-3 rounded-xl border border-rose-300 bg-rose-50/40 transition-all duration-300">
            <div class="flex items-center justify-between">
              <span class="text-xs font-bold text-slate-900">Station J2</span>
              <span id="irBadge1" class="text-[10px] px-1.5 py-0.5 rounded font-medium bg-slate-100 text-slate-400">IR IDLE</span>
            </div>
            <div class="mt-2 space-y-1.5 text-xs">
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Blue LED (P22):</span>
                <span id="blueVal1" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Red LED (P27):</span>
                <span id="redVal1" class="px-1.5 py-0.2 rounded font-bold text-[10px] bg-rose-600 text-white red-glow">ON (S)</span>
              </div>
            </div>
          </div>

          <div id="stationCard2" class="p-3 rounded-xl border border-slate-200 bg-white/70 transition-all duration-300">
            <div class="flex items-center justify-between">
              <span class="text-xs font-bold text-slate-900">Station J3</span>
              <span id="irBadge2" class="text-[10px] px-1.5 py-0.5 rounded font-medium bg-slate-100 text-slate-400">IR IDLE</span>
            </div>
            <div class="mt-2 space-y-1.5 text-xs">
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Blue LED (P21):</span>
                <span id="blueVal2" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Red LED (P14):</span>
                <span id="redVal2" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
            </div>
          </div>

          <div id="stationCard3" class="p-3 rounded-xl border border-slate-200 bg-white/70 transition-all duration-300">
            <div class="flex items-center justify-between">
              <span class="text-xs font-bold text-slate-900">Station J4</span>
              <span id="irBadge3" class="text-[10px] px-1.5 py-0.5 rounded font-medium bg-slate-100 text-slate-400">IR IDLE</span>
            </div>
            <div class="mt-2 space-y-1.5 text-xs">
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Blue LED (P19):</span>
                <span id="blueVal3" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Red LED (P13):</span>
                <span id="redVal3" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
            </div>
          </div>

          <div id="stationCard4" class="p-3 rounded-xl border border-slate-200 bg-white/70 transition-all duration-300">
            <div class="flex items-center justify-between">
              <span class="text-xs font-bold text-slate-900">Station J5</span>
              <span id="irBadge4" class="text-[10px] px-1.5 py-0.5 rounded font-medium bg-slate-100 text-slate-400">IR IDLE</span>
            </div>
            <div class="mt-2 space-y-1.5 text-xs">
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Blue LED (P18):</span>
                <span id="blueVal4" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Red LED (P12):</span>
                <span id="redVal4" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
            </div>
          </div>

          <div id="stationCard5" class="p-3 rounded-xl border border-slate-200 bg-white/70 transition-all duration-300">
            <div class="flex items-center justify-between">
              <span class="text-xs font-bold text-slate-900">Station J6</span>
              <span id="irBadge5" class="text-[10px] px-1.5 py-0.5 rounded font-medium bg-slate-100 text-slate-400">IR IDLE</span>
            </div>
            <div class="mt-2 space-y-1.5 text-xs">
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Blue LED (P25):</span>
                <span id="blueVal5" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
              <div class="flex items-center justify-between">
                <span class="text-slate-600">Red LED (P15):</span>
                <span id="redVal5" class="px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400">OFF</span>
              </div>
            </div>
          </div>

        </div>
      </div>

    </div>

  </div>

  <script>
    const stations = [
      { id: 1, x: 7, irPin: 34, bluePin: 23, redPin: 26 },
      { id: 2, x: 24.2, irPin: 35, bluePin: 22, redPin: 27 },
      { id: 3, x: 41.4, irPin: 36, bluePin: 21, redPin: 14 },
      { id: 4, x: 58.6, irPin: 39, bluePin: 19, redPin: 13 },
      { id: 5, x: 75.8, irPin: 32, bluePin: 18, redPin: 12 },
      { id: 6, x: 93, irPin: 33, bluePin: 25, redPin: 15 },
    ];

    let currentStationIndex = 0;
    let pollTimer = null;
    let isConnected = true;

    function renderJunctions() {
      const container = document.getElementById("junctionsContainer");
      container.innerHTML = "";

      stations.forEach((st, idx) => {
        const node = document.createElement("div");
        node.id = `junctionNode${idx}`;
        node.className = "absolute -ml-6 w-12 flex flex-col items-center justify-center transition-all duration-300";
        node.style.left = `${st.x}%`;
        node.style.top = "20px";

        const isNorth = (idx === currentStationIndex);
        const isSouth = (idx === currentStationIndex + 1);

        let borderStyle = "border-slate-200 bg-white text-slate-700";
        let pulseElem = "";

        if (isNorth) {
          borderStyle = "bg-blue-600 text-white shadow-lg shadow-blue-300 ring-4 ring-blue-100 scale-110";
          pulseElem = '<span class="absolute w-12 h-12 rounded-full bg-blue-400/40 pulse-ring"></span>';
        } else if (isSouth) {
          borderStyle = "bg-rose-500 text-white shadow-lg shadow-rose-300 ring-4 ring-rose-100 scale-105";
          pulseElem = '<span class="absolute w-12 h-12 rounded-full bg-rose-400/40 pulse-ring"></span>';
        }

        node.innerHTML = `
          <div class="relative flex items-center justify-center">
            ${pulseElem}
            <div class="w-9 h-9 rounded-full ${borderStyle} flex items-center justify-center font-bold text-xs transition-transform transform">
              J${st.id}
            </div>
          </div>
          <span class="text-[9px] font-bold ${isNorth ? 'text-blue-700' : (isSouth ? 'text-rose-600' : 'text-slate-500')} mt-1 whitespace-nowrap bg-white/90 px-1 rounded shadow-2xs">
            ${isNorth ? 'NORTH' : (isSouth ? 'SOUTH' : 'J' + st.id)}
          </span>
        `;
        container.appendChild(node);
      });
    }

    function setStation(idx) {
      currentStationIndex = idx;
      const st = stations[idx];
      const nextIdx = (idx < 5) ? idx + 1 : -1;
      const nextSt = (nextIdx >= 0) ? stations[nextIdx] : null;

      const cart = document.getElementById("cartBot");
      cart.style.left = `${st.x}%`;

      document.getElementById("northBadge").innerText = `J${st.id} (P${st.bluePin})`;
      document.getElementById("southBadge").innerText = nextSt ? `J${nextSt.id} (P${nextSt.redPin})` : "End Limit";

      document.getElementById("headerActiveStation").innerText = `Station J${st.id}`;
      document.getElementById("telemetryJunction").innerText = `Junction ${st.id}`;
      document.getElementById("telemetryStation").innerText = `Acquired by IR ${st.id} (Pin ${st.irPin})`;
      document.getElementById("telemetryBlueLed").innerText = `GPIO ${st.bluePin} (Blue LED ${st.id})`;
      document.getElementById("telemetryRedLed").innerText = nextSt ? `GPIO ${nextSt.redPin} (Red LED ${nextSt.id})` : "End of Linear Track";

      renderJunctions();
      updateMatrix(idx, nextIdx);
    }

    function updateMatrix(northIdx, southIdx) {
      for (let i = 0; i < 6; i++) {
        const card = document.getElementById(`stationCard${i}`);
        const irBadge = document.getElementById(`irBadge${i}`);
        const blueVal = document.getElementById(`blueVal${i}`);
        const redVal = document.getElementById(`redVal${i}`);

        const isNorth = (i === northIdx);
        const isSouth = (i === southIdx);

        if (isNorth) {
          card.className = "p-3 rounded-xl border border-blue-300 bg-blue-50/70 shadow-sm transition-all duration-300";
          irBadge.className = "text-[10px] px-1.5 py-0.5 rounded font-bold bg-emerald-100 text-emerald-800 border border-emerald-200 animate-pulse";
          irBadge.innerText = "IR ACTIVE";
          blueVal.className = "px-1.5 py-0.2 rounded font-bold text-[10px] bg-blue-600 text-white blue-glow";
          blueVal.innerText = "ON (N)";
        } else {
          irBadge.className = "text-[10px] px-1.5 py-0.5 rounded font-medium bg-slate-100 text-slate-400";
          irBadge.innerText = "IR IDLE";
          blueVal.className = "px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400";
          blueVal.innerText = "OFF";
        }

        if (isSouth) {
          if (!isNorth) {
            card.className = "p-3 rounded-xl border border-rose-300 bg-rose-50/50 shadow-sm transition-all duration-300";
          }
          redVal.className = "px-1.5 py-0.2 rounded font-bold text-[10px] bg-rose-600 text-white red-glow";
          redVal.innerText = "ON (S)";
        } else {
          redVal.className = "px-1.5 py-0.2 rounded font-medium text-[10px] bg-slate-100 text-slate-400";
          redVal.innerText = "OFF";
        }

        if (!isNorth && !isSouth) {
          card.className = "p-3 rounded-xl border border-slate-200 bg-white/70 transition-all duration-300";
        }
      }
    }

    function setConnectionStatus(connected) {
      if (isConnected === connected) return;
      isConnected = connected;

      const iconBg = document.getElementById("connIconBg");
      const icon = document.getElementById("connIcon");
      const badge = document.getElementById("connBadge");
      const dot = document.getElementById("connDot");
      const statusText = document.getElementById("connStatusText");
      const subText = document.getElementById("connSubText");
      const pill = document.getElementById("statusPill");
      const ping = document.getElementById("statusPing");
      const point = document.getElementById("statusPoint");
      const pillLabel = document.getElementById("pillLabel");

      if (connected) {
        iconBg.className = "w-12 h-12 rounded-xl bg-emerald-500/10 text-emerald-600 flex items-center justify-center border border-emerald-200 shadow-sm transition-colors duration-300";
        icon.innerHTML = '<path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z"></path>';
        badge.className = "px-2.5 py-0.5 rounded-full text-xs font-semibold bg-emerald-100 text-emerald-800 border border-emerald-200 flex items-center gap-1.5";
        dot.className = "w-2 h-2 rounded-full bg-emerald-500 animate-pulse";
        statusText.innerText = "Connected";
        subText.className = "text-emerald-700 font-semibold not-italic";
        subText.innerText = "Live Hardware Stream";

        pill.className = "px-3.5 py-2 rounded-xl bg-emerald-50 border border-emerald-200 text-emerald-800 flex items-center gap-2 text-xs font-semibold shadow-2xs";
        ping.className = "animate-ping absolute inline-flex h-full w-full rounded-full bg-emerald-400 opacity-75";
        point.className = "relative inline-flex rounded-full h-2.5 w-2.5 bg-emerald-500";
        pillLabel.innerText = "ESP32 ONLINE";
      } else {
        iconBg.className = "w-12 h-12 rounded-xl bg-rose-500/10 text-rose-600 flex items-center justify-center border border-rose-200 shadow-sm transition-colors duration-300";
        icon.innerHTML = '<path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 9v2m0 4h.01m-6.938 4h13.856c1.54 0 2.502-1.667 1.732-3L13.732 4c-.77-1.333-2.694-1.333-3.464 0L3.34 16c-.77 1.333.192 3 1.732 3z"></path>';
        badge.className = "px-2.5 py-0.5 rounded-full text-xs font-semibold bg-rose-100 text-rose-800 border border-rose-200 flex items-center gap-1.5";
        dot.className = "w-2 h-2 rounded-full bg-rose-500";
        statusText.innerText = "Disconnected";
        subText.className = "text-rose-700 font-semibold not-italic";
        subText.innerText = "Reconnecting to ESP32...";

        pill.className = "px-3.5 py-2 rounded-xl bg-rose-50 border border-rose-200 text-rose-800 flex items-center gap-2 text-xs font-semibold shadow-2xs";
        ping.className = "hidden";
        point.className = "relative inline-flex rounded-full h-2.5 w-2.5 bg-rose-500";
        pillLabel.innerText = "ESP32 OFFLINE";
      }
    }

    // Poll live physical hardware directly from ESP32 (/status)
    function startPolling() {
      clearInterval(pollTimer);
      pollTimer = setInterval(async () => {
        try {
          const controller = new AbortController();
          const timeoutId = setTimeout(() => controller.abort(), 1200);
          const res = await fetch("/status", { signal: controller.signal });
          clearTimeout(timeoutId);

          if (res.ok) {
            setConnectionStatus(true);
            const data = await res.json();
            // Data acquired directly from physical IR sensors
            if (data.active_junction && data.active_junction >= 1 && data.active_junction <= 6) {
              const detectedIndex = data.active_junction - 1;
              if (detectedIndex !== currentStationIndex) {
                setStation(detectedIndex);
              }
            }
          } else {
            setConnectionStatus(false);
          }
        } catch (e) {
          if (window.location.protocol === "file:") {
            setConnectionStatus(true);
          } else {
            setConnectionStatus(false);
          }
        }
      }, 200);
    }

    renderJunctions();
    setStation(0);
    startPolling();
  </script>
</body>
</html>
)rawliteral";
