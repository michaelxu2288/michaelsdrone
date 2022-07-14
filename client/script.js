

const deg_t_rad = 0.017453292519943;

var width=700, height=700;
var canvas;
var gl;

function resize(w,h){
    canvas.width = width = w;
    canvas.height = height = h;
}

var rotation = {r:0, p:0, y:0};


var buffers = [];

function initBuffers(){
    buffers.push(Mesh.cube(gl, 1));
}


var socket;

var r_graph, p_graph, y_graph;

var terminal;

//configuring = 0, ready = 1, calibrating = 2, idle = 3, init = 4, settling = 5, destroying = 6
const states = ["configuring", "ready", "calibrating", "idle", "init", "settling", "destroying"];


function changeTabs(id){
    $(".tabcontent").removeClass("shown");
    $(id).addClass("shown");
}


function processSmallNumbers(n){

    // if(n < .01 && n > -.01){
    //     return Math.round(n * 10000) / 10000;
    // }else if(n < .1 && n > -.1){
    //     return Math.round(n * 1000) / 1000;
    
    // }else if(n < 1 && n > -1){
    //     return Math.round(n * 100) / 100;
    // }

    return Math.round(n * 10) / 10;

}


/**
 * @type {Map<string, {value:number}}}
 */
const Gauges = {
    ax: {
        value: 0,
        title: "Ax",
        width: 180,
        height: 80,
        valueToPercent: (val) => {
            return val / 20 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)} m/s²`;
        },
        type: 0,
        gaugeSize: 160,
        container: "#accel-gauges",
    },
    ay: {
        value: 0,
        title: "Ay",
        width: 180,
        height: 80,
        valueToPercent: (val) => {
            return val / 20 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)} m/s²`;
        },
        type: 0,
        gaugeSize: 160,
        container: "#accel-gauges",
    },
    az: {
        value: 0,
        title: "Az",
        width: 180,
        height: 80,
        valueToPercent: (val) => {
            return val / 20 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)} m/s²`;
        },
        type: 0,
        gaugeSize: 160,
        container: "#accel-gauges",
    },
    // vx: {
    //     value: 0,
    //     title: "Vx",
    //     width: 180,
    //     height: 80,
    //     valueToPercent: (val) => {
    //         return val / 20 + .5; 
    //     },
    //     textFromValue: (angle) => {
    //         return `${processSmallNumbers(angle)} m/s`;
    //     },
    //     type: 0,
    //     gaugeSize: 160,
    //     container: "#x-gauges",
    // },
    // vy: {
    //     value: 0,
    //     title: "Vy",
    //     width: 180,
    //     height: 80,
    //     valueToPercent: (val) => {
    //         return val / 20 + .5; 
    //     },
    //     textFromValue: (angle) => {
    //         return `${processSmallNumbers(angle)} m/s`;
    //     },
    //     type: 0,
    //     gaugeSize: 160,
    //     container: "#y-gauges",
    // },
    vz: {
        value: 0,
        title: "Vz",
        width: 180,
        height: 80,
        valueToPercent: (val) => {
            return val / 20 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)} m/s`;
        },
        type: 0,
        gaugeSize: 160,
        container: "#accel-gauges",
    },
    vroll: {
        value: 0,
        title: "Vroll",
        width: 180,
        height: 180,
        valueToPercent: (val) => {
            return val / 180 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)}°`;
        },
        type: 2,
        gaugeSize: 45,
        container: "#roll-gauges",
    },
    vpitch: {
        value: 0,
        title: "Vpitch",
        width: 180,
        height: 180,
        valueToPercent: (val) => {
            return val / 180 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)}°`;
        },
        type: 2,
        gaugeSize: 45,
        container: "#pitch-gauges",
    },
    vyaw: {
        value: 0,
        title: "Vyaw",
        width: 180,
        height: 180,
        valueToPercent: (val) => {
            return val / 180 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)}°`;
        },
        type: 2,
        gaugeSize: 45,
        container: "#yaw-gauges",
    },
    roll: {
        value: 0,
        title: "Roll",
        width: 180,
        height: 180,
        setpoint: 0,
        valueToPercent: (val) => {
            return val / 180 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)}°`;
        },
        type: 2,
        gaugeSize: 45,
        container: "#roll-gauges",
    },
    pitch: {
        value: 0,
        title: "Pitch",
        width: 180,
        height: 180,
        valueToPercent: (val) => {
            return val / 180 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)}°`;
        },
        type: 2,
        gaugeSize: 45,
        container: "#pitch-gauges",
    },
    yaw: {
        value: 0,
        title: "Yaw",
        width: 180,
        height: 180,
        valueToPercent: (val) => {
            return val / 180 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)}°`;
        },
        type: 2,
        gaugeSize: 45,
        container: "#yaw-gauges",
    },
    // a: {
    //     value: 0,
    //     title: "Acceleration",
    //     width: 360,
    //     height: 80,
    //     valueToPercent: (val) => {
    //         return val / 20; 
    //     },
    //     textFromValue: (angle) => {
    //         return `${processSmallNumbers(angle)} m/s²`;
    //     },
    //     type: 0,
    //     gaugeSize: 340,
    //     container: ".bruh",
    // },
    // v: {
    //     value: 0,
    //     title: "Velocity",
    //     width: 360,
    //     height: 80,
    //     valueToPercent: (val) => {
    //         return val / 20; 
    //     },
    //     textFromValue: (angle) => {
    //         return `${processSmallNumbers(angle)} m/s`;
    //     },
    //     type: 0,
    //     gaugeSize: 340,
    //     container: ".bruh",
    // },
    temp: {
        value: 0,
        title: "Temp",
        width: 180,
        height: 80,
        textFromValue: (temp) => {
            return `${processSmallNumbers(temp)}°C`;
        },
        type: 3,
        container: "#bmp-gauges",
    },
    press: {
        value: 0,
        title: "Pressure",
        width: 180,
        height: 80,
        textFromValue: (temp) => {
            return `${processSmallNumbers(temp)} P`;
        },
        type: 3,
        container: "#bmp-gauges",
    },
    alt: {
        value: 0,
        title: "Altitude",
        width: 180,
        height: 90,
        textFromValue: (temp) => {
            return `${processAltitude(temp)} ft`;
        },
        type: 3,
        container: "#bmp-gauges",
    },
    mfl: {
        value: 0,
        title: "Motor FL",
        width: 180,
        height: 90,
        valueToPercent: (val) => {
            return val; 
        },
        textFromValue: (angle) => {
            return `${Math.round(angle*100)}%`;
        },
        type: 1,
        gaugeSize: 160,
        container: "#motor-gauges",
    },
    mfr: {
        value: 0,
        title: "Motor FR",
        width: 180,
        height: 90,
        valueToPercent: (val) => {
            return val; 
        },
        textFromValue: (angle) => {
            return `${Math.round(angle*100)}%`;
        },
        type: 1,
        gaugeSize: 160,
        container: "#motor-gauges",
    },
    mbl: {
        value: 0,
        title: "Motor BL",
        width: 180,
        height: 90,
        valueToPercent: (val) => {
            return val; 
        },
        textFromValue: (angle) => {
            return `${Math.round(angle*100)}%`;
        },
        type: 1,
        gaugeSize: 160,
        container: "#motor-gauges",
    },
    mbr: {
        value: 0,
        title: "Motor BR",
        width: 180,
        height: 90,
        valueToPercent: (val) => {
            return val; 
        },
        textFromValue: (angle) => {
            return `${Math.round(angle*100)}%`;
        },
        type: 1,
        gaugeSize: 160,
        container: "#motor-gauges",
    },
    rollA: {
        value: 0,
        title: "Roll (Accel)",
        width: 180,
        height: 180,
        valueToPercent: (val) => {
            return val / 180 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)}°`;
        },
        type: 2,
        gaugeSize: 45,
        container: "#roll-gauges",
    },
    pitchA: {
        value: 0,
        title: "Pitch (Accel)",
        width: 180,
        height: 180,
        valueToPercent: (val) => {
            return val / 180 + .5; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)}°`;
        },
        type: 2,
        gaugeSize: 45,
        container: "#pitch-gauges",
    },
    z: {
        value: 0,
        title: "z",
        width: 160,
        height: 80,
        textFromValue: (temp) => {
            return `${processAltitude(temp)} ft`;
        },
        type: 3,
        container: "#z-gauges",
    },
    valt: {
        value: 0,
        title: "Valt",
        width: 180,
        height: 90,
        textFromValue: (temp) => {
            return `${processAltitude(temp)} ft/s`;
        },
        type: 3,
        container: "#bmp-gauges",
    },
    initalt: {
        value: 0,
        title: "Initial Alt",
        width: 180,
        height: 90,
        textFromValue: (temp) => {
            return `${processAltitude(temp)} ft`;
        },
        type: 3,
        container: "#bmp-gauges",
    },

}

function processAltitude(alt){
    return Math.round(alt * 3.281 * 100) / 100;
}

const history = {
    ax: [],
    ay: [],
    az: [],
    vx: [],
    vy: [],
    vz: [],
    x: [],
    y: [],
    z: [],
    vroll: [],
    vpitch: [],
    vyaw: [],
    roll: [],
    pitch: [],
    yaw: [],
    // time: [],
}



var z_pid_graph;
var r_pid_graph;
var p_pid_graph;
var vy_pid_graph;
var alt_graph;
var xyz_graph;
var rpy_graph;
var tpa_graph;

var graph1;
var graph2;

const DEG_TO_RAD = Math.PI / 180;
var k = 0;
var currState = -1;
var droneStatusDot;
var controllerStatusDot;

var lastUpdate = new Date();







const changeTunerVals = [];


function connect(){

    
    if(!socket){
        $("#con-btn").text("Connecting ...")
        socket = io("http://pi@drone/");
        droneStatusDot.connecting();

        socket.on("connect", () => {
            $("#zero-btn").click(() => {socket.emit("cmd", 0)});
            $("#calib-btn").click(() => {socket.emit("cmd", 1)});
            $("#rld-config-btn").click(() => {socket.emit("cmd", 2)});
            $("#con-ctrl-btn").click(() => {socket.emit("bluetooth")})

            
            $("#con-btn").text("Disconnect");
            droneStatusDot.connected();
            $("#kill").click(() => {socket.emit("kill");});

        });

        socket.on("disconnect", () => {
            droneStatusDot.disconnected();
            $("#zero-btn").unbind("click");
            $("#calib-btn").unbind("click");
            $("#rld-config-btn").unbind("click");
            $("#con-ctrl-btn").unbind("click");
        });

        

        const bruheles = [];
        const pidTunerEle = $("#pid-tuner");
        [
            "Z P", "Z I", "Z D",
            "VYaw P", "VYaw I", "VYaw D",
            "Roll P", "Roll I", "Roll D",
            "Pitch P", "Pitch I", "Pitch D",
        ].forEach((name, i) => {
            const ihatethis = $(`<input id="tuner-${i}"/>`);
            ihatethis.on("change", (e) => {
                console.log("FUICK THIS SHIT", e);
                changeTunerVals[i](parseFloat(e.target.value));
            })
            bruheles.push(ihatethis);
            const ele = $(`<div class="key-val"/>`).append($(`<span class="center">${name}</span>`)).append(ihatethis)
            pidTunerEle.append(ele);
        }); 
        // console.log(bruheles);
        socket.on("json", (config) => {
            const pid = config.pid;
            // console.log(pid);
            ["z", "vyaw", "r", "p"].forEach((name, i) => {
                // console.log(name);
                ["kP", "kI", "kD"].forEach((wtf, j) => {
                    // console.log(wtf);
                    // console.log(pid[name][wtf])
                    bruheles[i*3+j].val(pid[name][wtf]);
                    changeTunerVals.push((val) => {
                        pid[name][wtf] = val;
                        socket.emit("chg-pid", i*4+j, val);
                        socket.emit("update-json", config);
                    });
                })
            })
        })

        socket.emit("req-json");

        var k = 0;
        var currState = -1;
        socket.on("sensor", (output) => {
            parseOutput(output);
        });
    }else{
        $("#con-btn").text("Connect");
        droneStatusDot.disconnected();
        socket.disconnect();
        $("#zero-btn").unbind("click");
        $("#calib-btn").unbind("click");
        $("#rld-config-btn").unbind("click");
        $("#con-ctrl-btn").unbind("click");
        socket = null;
    }
    
}

class StatusDot {
    constructor(element){
        this.ele = element;
        this.state = 0;
    }
    disconnected(){
        if(this.state == 0) return;
        this.state = 0;
        this.ele.removeClass("connected");
        this.ele.removeClass("connecting");
    }
    connecting(){
        if(this.state == 1) return;
        this.state = 1;
        this.ele.removeClass("connected");
        this.ele.addClass("connecting");
    }
    connected(){
        if(this.state == 2) return;
        this.state = 2;
        this.ele.addClass("connected");
        this.ele.removeClass("connecting");
    }
}
var graph1ctx, graph2ctx;
function parseOutput(output) {
    output = output.map((a) => parseFloat(a));
    lastUpdate = new Date();
    
    rotation.r = output[12];
    rotation.p = output[13];
    rotation.y = output[14];
    
    const a = Math.sqrt(output[0]*output[0] + output[1]*output[1] + output[2]*output[2]);

    Gauges.ax.gauge.changeValue(output[0]);
    Gauges.ay.gauge.changeValue(output[1]);
    Gauges.az.gauge.changeValue(output[2]);

    Gauges.vz.gauge.changeValue(output[8]);

    Gauges.roll.gauge.changeValue(rotation.r / DEG_TO_RAD);
    Gauges.pitch.gauge.changeValue(rotation.p / DEG_TO_RAD);
    Gauges.yaw.gauge.changeValue(rotation.y / DEG_TO_RAD);

    Gauges.roll.gauge.changeSetpoint(output[22] / DEG_TO_RAD);
    Gauges.pitch.gauge.changeSetpoint(output[23] / DEG_TO_RAD);
    Gauges.vyaw.gauge.changeSetpoint(output[21] / DEG_TO_RAD);

    Gauges.vroll.gauge.changeValue(output[3] / DEG_TO_RAD);
    Gauges.vpitch.gauge.changeValue(output[4] / DEG_TO_RAD);
    Gauges.vyaw.gauge.changeValue(output[5] / DEG_TO_RAD);

    Gauges.temp.gauge.changeValue(output[15]);
    Gauges.press.gauge.changeValue(output[16]);
    Gauges.alt.gauge.changeValue(output[17]);

    Gauges.mfl.gauge.changeValue(output[28]);
    Gauges.mfr.gauge.changeValue(output[29]);
    Gauges.mbl.gauge.changeValue(output[30]);
    Gauges.mbr.gauge.changeValue(output[31]);

    Gauges.z.gauge.changeValue(output[11]);
    Gauges.valt.gauge.changeValue(output[19]);
    Gauges.initalt.gauge.changeValue(output[18]);
    
    var r_a = Math.atan2(output[1], output[2]) * 57.3
    var p_a = Math.atan2(output[0], Math.sqrt(output[1] * output[1] + output[2] * output[2])) * 57.3;
    if(Math.abs(a - 9.8) < 0.5){
        const mu = 0.001;
        Gauges.rollA.gauge.changeValue(r_a);
        Gauges.pitchA.gauge.changeValue(p_a);
    }else{
        r_a = p_a = NaN;
        Gauges.rollA.gauge.changeValue(NaN);
        Gauges.pitchA.gauge.changeValue(NaN);
    }
    
    const M_TO_FT = 3.281;


    alt_graph.addDataList([output[8] * M_TO_FT, (output[2] - 9.81) * M_TO_FT, (output[17] - output[18]) * M_TO_FT, (output[11]) * M_TO_FT, output[19] * M_TO_FT])
    xyz_graph.addDataList([output[0] * M_TO_FT, output[1] * M_TO_FT, output[2] * M_TO_FT, output[8] * M_TO_FT, output[11] * M_TO_FT])
    rpy_graph.addDataList([
        output[3] / DEG_TO_RAD, output[4] / DEG_TO_RAD, output[5] / DEG_TO_RAD,
        r_a, p_a,
        rotation.r / DEG_TO_RAD, rotation.p / DEG_TO_RAD, rotation.y / DEG_TO_RAD
    ])
    // alt_graph.render();

    function b(k, i){
        k.addDataList([
            output[20 + i],
            output[24 + i],
            output[57 + i],
            output[45 + i],
            output[49 + i],
            output[53 + i],
            output[37 + i],
            output[41 + i],
        ])
    }

    b(z_pid_graph, 0);
    b(vy_pid_graph, 1);
    b(r_pid_graph, 2);
    b(p_pid_graph, 3);

    graph1.render(graph1ctx);
    graph2.render(graph2ctx);
    
    Object.keys(Gauges).forEach((key) => {if(Gauges[key].gauge.updated) {Gauges[key].gauge.drawCanvas();}})
    // console.log(output);
    if(output[32] !== currState){
        // console.log(output[32]);
        $("#status").text(states[output[32]]);
        currState = output[32];
    }
    if(output[36] === 1) {
        controllerStatusDot.connected();
    }else{
        controllerStatusDot.disconnected();
    }

    k++;
}

var connectedStream = false;
function toggleStream(){
    if(connectedStream){
        connectedStream = false;
        $("#con-strm").text("Connect Stream");
        $("#stream").attr("src", "");
    }else{
        $("#con-strm").text("Disconnect Stream");
        connectedStream = true;
        $("#stream").attr("src", "http://drone:80/stream.mjpg");
    }
}

function disconnect(){
    if(socket)
        socket.disconnect();
    
    socket = null;
}


class Gauge {
    constructor(settings){
        settings.title = settings.title ?? "Title";
        settings.width = settings.width ?? 300;
        settings.height = settings.height ?? 150;
        settings.valueToPercent = settings.valueToPercent ?? ((value) => {return (value - this.settings.min) / (this.settings.max - this.settings.min)});
        settings.max = settings.max ?? 0;
        settings.min = settings.min ?? 100;
        settings.startFrom = settings.startFrom ?? 0;
        settings.textFromValue = settings.textFromValue ?? ((value) => Math.round(value*100) + "%")
        settings.type = settings.type ?? 0;
        settings.gaugeSize = settings.gaugeSize ?? settings.height * .3;
        settings.container = settings.container ?? ".bruh";
        settings.setpoint = settings.setpoint ?? NaN;

        this.settings = settings;
        this.updated = true;
        this.createElements()
    }

    createElements(){
        this.container = $("<div class='gauge-container'></div>").width(`${this.settings.width}px`).height(`${this.settings.height}px`)

        this.title = $("<div class='gauge-title'/>");
        this.title.text(this.settings.title).width(`${this.settings.width}px`).height(`${this.settings.height}px`);

        this.val_disp = $("<span class='gauge-val-disp'></span>");
        if(this.settings.type == 2){
            this.val_disp.addClass("center");
        }
        this.val_disp.text(this.settings.textFromValue(this.settings.value));
        
        this.canvas = $("<canvas class='gauge-canvas'/>");

        this.canvas[0].width = this.settings.width;
        this.canvas[0].height = this.settings.height;
        this.ctx = this.canvas[0].getContext("2d");

        this.container.append(this.title).append(this.canvas).append(this.val_disp);

        // $(".bruh").append(this.container);
        $(this.settings.container).append(this.container);
        this.drawCanvas();
    }

    drawCanvas(){
        this.updated = false;
        if(this.settings.type == 3){ // Text only

        }if(this.settings.type == 2) { // Full angle Gauge
            this.ctx.clearRect(0,0,this.settings.width, this.settings.height);
            this.ctx.beginPath();
            this.ctx.strokeStyle = "#0F0";
            this.ctx.lineWidth = 8;
            const beg = this.settings.valueToPercent(this.settings.startFrom) * Math.PI - Math.PI;
            const end = this.settings.valueToPercent(this.settings.value) * Math.PI - Math.PI;
            const a_start = Math.min(beg, end);
            const a_end = Math.max(beg, end);
            this.ctx.arc(this.settings.width/2, this.settings.height/2, this.settings.gaugeSize, a_start, a_end);
            this.ctx.stroke();

            if(!isNaN(this.settings.setpoint)){
                this.ctx.fillStyle = "#ff992b";
                const tri_size = 5;
                const offx = this.settings.gaugeSize - tri_size * 3.1, offy = 0;
                const tri = [[-tri_size-1 - offx, offy], [+tri_size/2-offx, -tri_size * Math.sqrt(3) / 2+offy], [tri_size/2 - offx, tri_size * Math.sqrt(3) / 2 + offy]];
                const angle = this.settings.valueToPercent(this.settings.setpoint) * Math.PI - Math.PI;
                // const angle = 0;
                this.ctx.beginPath();
                const cos = Math.cos(-angle);
                const sin = Math.sin(-angle);
                for(var i = 0; i < 3; i ++){
                    const x = tri[i][0] * cos - tri[i][1] * sin + this.settings.width/2, y = tri[i][0] * sin + tri[i][1] * cos + this.settings.height/2;
                    
                    if(i == 0){
                        this.ctx.moveTo(x,y);
                    }else{
                        this.ctx.lineTo(x,y);
                    }
                }
                this.ctx.fill();
            }

        }else if(this.settings.type == 1) { // Half angle Gauge
            this.ctx.clearRect(0,0,this.settings.width, this.settings.height);
            this.ctx.beginPath();
            this.ctx.strokeStyle = "#0F0";
            this.ctx.lineWidth = 8;
            const beg = this.settings.valueToPercent(this.settings.startFrom) * Math.PI - Math.PI;
            const end = this.settings.valueToPercent(this.settings.value) * Math.PI - Math.PI;
            const a_start = Math.min(beg, end);
            const a_end = Math.max(beg, end);
            this.ctx.arc(this.settings.width/2, this.settings.height, this.settings.gaugeSize, a_start, a_end);
            this.ctx.stroke();
        }else if(this.settings.type == 0) { // Horizontal Bar Gauge
            this.ctx.clearRect(0,0,this.settings.width, this.settings.height);
            this.ctx.beginPath();
            this.ctx.strokeStyle = "#0F0";
            this.ctx.lineWidth = 8;
            const off  = (this.settings.width - this.settings.gaugeSize) / 2;
            const beg = this.settings.valueToPercent(this.settings.startFrom) * this.settings.gaugeSize + off;
            const end = this.settings.valueToPercent(this.settings.value) * this.settings.gaugeSize + off;
            const a_start = Math.min(beg, end);
            const a_end = Math.max(beg, end);
            // this.ctx.arc(this.settings.width/2, this.settings.height/2, Math.min(this.settings.width/2, this.settings.height/2) * .6, a_start, a_end);
            this.ctx.moveTo(a_start, this.settings.height / 2);
            this.ctx.lineTo(a_end, this.settings.height / 2);
            this.ctx.stroke();
        }

    }

    changeValue(val){
        this.settings.value = val;
        this.val_disp.text(this.settings.textFromValue(this.settings.value));
        this.updated = true;
        // this.drawCanvas();
    }

    changeSetpoint(setpoint){
        this.settings.setpoint = setpoint;
        this.updated = true;
        // this.drawCanvas();
    }
}

var chart;

function main(){
    droneStatusDot = new StatusDot($("#conn-dot"));
    controllerStatusDot = new StatusDot($("#ctrller-dot"));

    droneStatusDot.disconnected();
    controllerStatusDot.disconnected();

    const timeSinceEle = $("#last-update");
    setInterval(() => {
        const now = new Date();
        const s = Math.round((now - lastUpdate) / 1000);
        if(s < 1){
            timeSinceEle.text(`just now`);
        }else{
            timeSinceEle.text(`${s} s`);
        }
    }, 1000);
    {
        alt_graph = new TimeGraph(document.getElementById("hidden-canvas"),{
            range: {
                data: {
                    max: 3,
                    min: -3,
                }
            },
            g_width: 500,
            g_height: 400,
            legend: {
                element: $("#hidden-div"),
                names: ["vz (ft)", "az (ft)", "altitude (ft)", "z (ft)", "valt (ft)"]
            }
        });
        alt_graph.createLines(5);

        
        xyz_graph = new TimeGraph(document.getElementById("hidden-canvas"),{
            range: {
                data: {
                    // max: 3,
                    // min: -3,
                }
            },
            // stepY: 5,
            g_width: 500,
            g_height: 400,
            legend: {
                element: $("#hidden-div"),
                names: ["ax (ft/s/s)", "ay (ft/s/s)", "az (ft/s/s)", "vz (ft/s)", "z (ft)"]
            }
        });
        xyz_graph.createLines(5);
        
        rpy_graph = new TimeGraph(document.getElementById("hidden-canvas"),{
            range: {
                data: {
                    // max: 3,
                    // min: -3,
                }

            },
            // stepY: 5,
            g_width: 500,
            g_height: 400,
            legend: {
                element: $("#hidden-div"),
                names: [
                    "Vr (gyro) (°/s)", "Vp (gyro) (°/s)", "Vy (gyro) (°/s)",
                    "r (accel) (°/s)", "p (accel) (°/s)", 
                    "r (°)", "p (°)", "y (°)"]
            }
        });
        rpy_graph.createLines(9);

        function a(){
            const b = new TimeGraph(document.getElementById("hidden-canvas"),{
                range: {
                    data: {
                        // max: 3,
                        // min: -3,
                    }
                },
                g_width: 500,
                g_height: 400,
                legend: {
                    element: $("#hidden-div"),
                    names: ["setpoint", "error", "output", "p", "i", "d", "curr i", "derr"]
                }
            });
            b.createLines(8);
            return b;
        }

       z_pid_graph = a();
       r_pid_graph = a();
       p_pid_graph = a();
       vy_pid_graph = a();
    }

    const graphs = [alt_graph, xyz_graph, rpy_graph, z_pid_graph, r_pid_graph, p_pid_graph, vy_pid_graph];

    const sels = $(".graph-select");

    ["altitude", "xyz", "rpy", "z pid", "roll pid", "pitch pid", "vyaw pid"].forEach(
        (name, i) => {
            sels.append($(`<option value=${i}>${name}</option>`));
        });


    sels.on("change", (e) => {
        if(e.target.id == "graph-1-select"){
            graph1 = graphs[parseInt(e.target.value)];
            graph1.generateLegend($("#graph-1-legend"));
        }else if(e.target.id == "graph-2-select"){
            graph2 = graphs[parseInt(e.target.value)];
            graph2.generateLegend($("#graph-2-legend"));
        }
    })

    $("#graph-1-select").val(0);
    $("#graph-2-select").val(1);
    graph1 = graphs[0];
    graph2 = graphs[1];
    graph1.generateLegend($("#graph-1-legend"));
    graph2.generateLegend($("#graph-2-legend"));

    graph1ctx = document.getElementById("graph-1").getContext("2d");
    graph2ctx = document.getElementById("graph-2").getContext("2d");

    // $(".graph").width(500).height(400);
    const eles = document.getElementsByClassName("graph");

    for(var i = 0; i < eles.length; i ++){
        eles[i].width = 500;
        eles[i].height = 400;
    }






    Object.keys(Gauges).forEach((key) => {
        if(Gauges[key] !== null){
            // Plotly.newPlot(key, [Gauges[key]]);
            Gauges[key].gauge = new Gauge(Gauges[key]);
        }
    })


    canvas = document.getElementById("gl-canvas");
    resize(width,height);
    gl = glUtil.createContext(canvas);



{

    initBuffers();

    var prog = glUtil.createProgramInfo(gl, vsSource, fsSource, {
        attribute:[
            "pos",
            "norm",
        ],
        uniform: [
            "NormMat",
            "ModelViewMat",
            "ProjMat"
        ]
    });
    console.log(prog)

    const render = () => {
        gl.clearColor(0.0, 0.0, 0.0, 0.0);
        gl.clearDepth(1.0);
        gl.enable(gl.DEPTH_TEST);
        gl.depthFunc(gl.LEQUAL);
    
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        gl.enable(gl.CULL_FACE);
        gl.cullFace(gl.BACK);

        const fieldOfView = 45 * Math.PI / 180;   // in radians
        const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
        const zNear = 0.1;
        const zFar = 100.0;
        const projectionMatrix = glMatrix.mat4.create();

        // note: glmatrix.js always has the first argument
        // as the destination to receive the result.
        glMatrix.mat4.perspective(projectionMatrix,
                        fieldOfView,
                        aspect,
                        zNear,
                        zFar);


        

        // Set the drawing position to the "identity" point, which is
        // the center of the scene.
        const model = glMatrix.mat4.create();
        const view = glMatrix.mat4.create();
        const modelViewMatrix = glMatrix.mat4.create();

        
        glMatrix.mat4.rotateY(model, model, rotation.y);
        glMatrix.mat4.rotateZ(model, model, rotation.r);
        glMatrix.mat4.rotateX(model, model, rotation.p);

        glMatrix.mat4.translate(view,     // destination matrix
            view,     // matrix to translate
            [-0.0, 0.0, -6.0]);
        //glMatrix.mat4.rotateX(view, view, Math.PI/4);


        //glMatrix.mat4.rotateX(model,model,theta/3);
            
        //glMatrix.mat4.mul(modelViewMatrix, view, model);
        //glMatrix.mat4.rotateY(view, view, theta/4);




        //glMatrix.mat4.mul(modelViewMatrix, model, view);
        glMatrix.mat4.mul(modelViewMatrix, view, model);

        const normalMatrix = glMatrix.mat4.create();
        glMatrix.mat4.invert(normalMatrix, modelViewMatrix);
        glMatrix.mat4.transpose(normalMatrix, normalMatrix);


        {
            const numComponents = 3;  // pull out 2 values per iteration
            const type = gl.FLOAT;    // the data in the buffer is 32bit floats
            const normalize = false;  // don't normalize
            const stride = 0;         // how many bytes to get from one set of values to the next
                                      // 0 = use type and numComponents above
            const offset = 0;   
            gl.bindBuffer(gl.ARRAY_BUFFER, buffers[0].vertices);
            gl.vertexAttribPointer(
                prog.attribute.pos,
                numComponents,
                type,
                normalize,
                stride,
                offset);
            gl.enableVertexAttribArray(
                prog.attribute.pos);
          }

           // Tell WebGL how to pull out the normals from
        // the normal buffer into the vertexNormal attribute.
        {
            const numComponents = 3;
            const type = gl.FLOAT;
            const normalize = false;
            const stride = 0;
            const offset = 0;
            gl.bindBuffer(gl.ARRAY_BUFFER, buffers[0].normals);
            gl.vertexAttribPointer(
                prog.attribute.norm,
                numComponents,
                type,
                normalize,
                stride,
                offset);
            gl.enableVertexAttribArray(
                prog.attribute.norm);
        }


        // Tell WebGL to use our program when drawing

        gl.useProgram(prog.program);

        // Set the shader uniforms

        gl.uniformMatrix4fv(
            prog.uniform.ProjMat,
            false,
            projectionMatrix);
        gl.uniformMatrix4fv(
            prog.uniform.ModelViewMat,
            false,
            modelViewMatrix);

        gl.uniformMatrix4fv(
            prog.uniform.NormMat,
            false,
            normalMatrix);
        
        {
            const vertexCount = buffers[0].nvert;
            const type = gl.UNSIGNED_SHORT;
            const offset = 0;
            gl.drawElements(gl.TRIANGLES, vertexCount, type, offset);
        }
        
    }

    connect();

    var theta = 0;
    var then = 0;
    const update = (now) => {
        const dt = (now-then) / 1000;
        then = now;
        theta+=dt;
        render();

        requestAnimationFrame(update);
    }


    requestAnimationFrame(update);
}
}

const vsSource = `
    attribute vec4 pos;
    attribute vec3 norm;

    uniform mat4 NormMat;
    uniform mat4 ModelViewMat;
    uniform mat4 ProjMat;

    varying highp vec3 vLight;

    void main(void) {
    gl_Position = ProjMat * ModelViewMat * pos;

    // Apply lighting effect

    highp vec3 ambientLight = vec3(0.01, 0.09, 0.07);
    highp vec3 directionalLightColor = vec3(0.19, 0.9, 0.77);
    highp vec3 directionalVector = normalize(vec3(0.85, 0.8, 0.75));

    highp vec4 transformedNormal = NormMat * vec4(norm, 1.0);

    highp float directional = max(dot(transformedNormal.xyz, directionalVector), 0.0);
    vLight = ambientLight + (directionalLightColor * directional);
    }
`;
const fsSource = `
varying highp vec2 vTex;
varying highp vec3 vLight;

void main(void) {

    gl_FragColor = vec4(vec3(1.0,1.0,1.0) * vLight, 1.0); //vec4(1.0,1.0,1.0,1.0);//vec4(vec3(1.0,1.0,1.0) * vLight, 1.0);
}
`;