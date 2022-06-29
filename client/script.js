
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

function connect(){
    if(!socket)
        socket = io("http://192.168.86.115:80/");
    socket.on("connect", () => {

    });

    socket.on("sensor", (output) => {
        // console.log(output);
        rotation.r = parseFloat(output[1]);
        rotation.p = parseFloat(output[2]);
        rotation.y = parseFloat(output[3]);
        // r_graph.addData(0, parseInt(output[0]), rotation.r);
        // p_graph.addData(0, parseInt(output[0]), rotation.p);
        // y_graph.addData(0, parseInt(output[0]), rotation.y);
        // r_graph.render();
        // p_graph.render();
        // y_graph.render();
    })
}

function disconnect(){
    if(socket)
        socket.disconnect();
    socket = null;
}


function main(){

    canvas = document.getElementById("gl-canvas");
    resize(width,height);
    gl = glUtil.createContext(canvas);

    // r_graph = new LineGraph(document.getElementById("r-canv"), {
    //     g_width: 500,
    //     range: {
    //         data: {
    //             min: - Math.PI/2,
    //             max: Math.PI/2
    //         }
    //     },
    //     max_data: 500,
    // });
    // p_graph = new LineGraph(document.getElementById("p-canv"), {
    //     g_width: 500,
    //     range: {
    //         data: {
    //             min: - Math.PI/2,
    //             max: Math.PI/2
    //         }
    //     },
    //     max_data: 500,
    // });
    // y_graph = new LineGraph(document.getElementById("y-canv"), {
    //     g_width: 500,
    //     range: {
    //         data: {
    //             min: - Math.PI/2,
    //             max: Math.PI/2
    //         }
    //     },
    //     max_data: 500,
    // });
    // r_graph.createLine();
    // r_graph.colors[0] = colors.primary;
    // p_graph.createLine();
    // p_graph.colors[0] = colors.primary;
    // y_graph.createLine();
    // y_graph.colors[0] = colors.primary;
    // r_graph.render();
    // p_graph.render();
    // y_graph.render();

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