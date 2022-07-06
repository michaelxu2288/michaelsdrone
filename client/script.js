
const colors = {
    grey1:"#b0acb0",
    grey2:"#e2dddf",
    light:"#85ebd9",
    primary:"#3d898d",
    dak:".0",
}

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





function processSmallNumbers(n){

    if(n < .01 && n > -.01){
        return Math.round(n * 10000) / 10000;
    }else if(n < .1 && n > -.1){
        return Math.round(n * 1000) / 1000;
    
    }else if(n < 1 && n > -1){
        return Math.round(n * 100) / 100;
    }

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
        container: "#x-gauges",
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
        container: "#y-gauges",
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
        container: "#z-gauges",
    },
    vx: {
        value: 0,
        title: "Vx",
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
        container: "#x-gauges",
    },
    vy: {
        value: 0,
        title: "Vy",
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
        container: "#y-gauges",
    },
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
        container: "#z-gauges",
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
    a: {
        value: 0,
        title: "Acceleration",
        width: 360,
        height: 80,
        valueToPercent: (val) => {
            return val / 20; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)} m/s²`;
        },
        type: 0,
        gaugeSize: 340,
        container: ".bruh",
    },
    v: {
        value: 0,
        title: "Velocity",
        width: 360,
        height: 80,
        valueToPercent: (val) => {
            return val / 20; 
        },
        textFromValue: (angle) => {
            return `${processSmallNumbers(angle)} m/s`;
        },
        type: 0,
        gaugeSize: 340,
        container: ".bruh",
    },
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
            return `${processSmallNumbers(temp)} m`;
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
            return `${processSmallNumbers(angle)}%`;
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
            return `${processSmallNumbers(angle)}%`;
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
            return `${processSmallNumbers(angle)}%`;
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
            return `${processSmallNumbers(angle)}%`;
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
    }
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


function connect(){

    
    if(!socket){
        $("#con-btn").text("Disconnect")
        socket = io("http://pi@drone/");

        socket.on("connect", () => {
            $("#zero-btn").click(() => {socket.emit("cmd", 0)});
            $("#calib-btn").click(() => {socket.emit("cmd", 1)});
        });
        const DEG_TO_RAD = Math.PI / 180;
        var k = 0;
        socket.on("sensor", (output) => {
            output = output.map((a) => parseFloat(a));
    
            rotation.r = output[12];
            rotation.p = output[13];
            rotation.y = output[14];
            
            const a = Math.sqrt(output[0]*output[0] + output[1]*output[1] + output[2]*output[2]);
            Gauges.a.gauge.changeValue(a);
            Gauges.v.gauge.changeValue(Math.sqrt(output[6]*output[6] + output[7]*output[7] + output[8]*output[8]));
    
            Gauges.ax.gauge.changeValue(output[0]);
            Gauges.ay.gauge.changeValue(output[1]);
            Gauges.az.gauge.changeValue(output[2]);
    
            Gauges.vx.gauge.changeValue(output[6]);
            Gauges.vy.gauge.changeValue(output[7]);
            Gauges.vz.gauge.changeValue(output[8]);
    
            Gauges.roll.gauge.changeValue(rotation.r / DEG_TO_RAD);
            Gauges.pitch.gauge.changeValue(rotation.p / DEG_TO_RAD);
            Gauges.yaw.gauge.changeValue(rotation.y / DEG_TO_RAD);
    
            Gauges.vroll.gauge.changeValue(output[3]);
            Gauges.vpitch.gauge.changeValue(output[4]);
            Gauges.vyaw.gauge.changeValue(output[5]);

            Gauges.temp.gauge.changeValue(output[15]);
            Gauges.press.gauge.changeValue(output[16]);
            Gauges.alt.gauge.changeValue(output[17]);

            Gauges.mfl.gauge.changeValue(output[36]);
            Gauges.mfr.gauge.changeValue(output[37]);
            Gauges.mbl.gauge.changeValue(output[38]);
            Gauges.mbr.gauge.changeValue(output[39]);

            
            // double x_Buff = float(x);
            // double y_Buff = float(y);
            // double z_Buff = float(z);
            // roll = atan2(y_Buff , -z_Buff) * 57.3;
            // pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
            if(Math.abs(a - 9.8) < 0.5){
                const mu = 0.001;
                // Roll  = atan2( Y,   sign* sqrt(Z*Z+ miu*X*X));
                // sign  = 1 if accZ>0, -1 otherwise 
                // miu = 0.001
                console.log(output[1], output[2])
                // Gauges.rollA.gauge.changeValue(Math.atan2(output[1], (output[2] > 0 ? -1 : 1) * Math.sqrt(output[2] * output[2] + mu * output[0] * output[0])) * 57.3);
                Gauges.rollA.gauge.changeValue(Math.atan2(output[1], output[2]) * 57.3);
                Gauges.pitchA.gauge.changeValue(Math.atan2(output[0], Math.sqrt(output[1] * output[1] + output[2] * output[2])) * 57.3);
            }else{
                Gauges.rollA.gauge.changeValue(NaN);
                Gauges.pitchA.gauge.changeValue(NaN);
            }
            chart.addData(0, k, output[0])
            chart.render();
    
            k++;
        });
    }else{
        $("#con-btn").text("Connect")
        socket.disconnect();
        socket = null;
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
        this.settings = settings;
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
        this.drawCanvas();
    } 
}

var chart;

function main(){

    const vals = [];
    for(var i = 0; i < 100 ; i++){
        vals.push(i);
    }

    chart = new LineGraph(document.getElementById("vroll-graph"),{
        range: {
            data: {
                max: 10,
                min: -10,
            }
        }
    });
    chart.createLine();

    Object.keys(Gauges).forEach((key) => {
        if(Gauges[key] !== null){
            // Plotly.newPlot(key, [Gauges[key]]);
            Gauges[key].gauge = new Gauge(Gauges[key]);
        }
    })


    canvas = document.getElementById("gl-canvas");
    resize(width,height);
    gl = glUtil.createContext(canvas);





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
        gl.clearColor(0.0, 0.0, 0.0, 1.0);
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