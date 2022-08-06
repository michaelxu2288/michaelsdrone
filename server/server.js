/* jshint esversion: 8 */
const PATH = "/home/pi/drone";
const port = 8080;


function log(str) {
    console.log(`[SERVER] ${str}`);
}


const path = require("path");
var config = require(path.join(PATH, "config/config.json"));

const SOCKET_LOCATION = config.message.socket_path;


const http = require("http");
const express = require("express");
const fs = require("fs");
const net = require("net");

const app = express();
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server, {
    cors: {
        origin: "*",
        methods: ["GET", "POST"]
    }
});

// const videoStream = require('raspberrypi-node-camera-web-streamer/videoStream.js');
// videoStream.acceptConnections(app, {
//     width: 2560,
//     height: 1440,
//     fps: 15,
//     encoding: 'JPEG',
//     quality: 12 //lower is faster
// }, '/stream.mjpg', true);

const process = require("process");

const { exec } = require('child_process');

var socketServer;

{
    function exitHandler(options, exitCode){
        log(`Exiting with code "${exitCode}"`);
        socketServer.close();
        if (options.cleanup) log('clean');
        if (exitCode || exitCode === 0) log(exitCode);
        if (options.exit) process.exit();
    }

    //do something when app is closing
    process.on('exit', exitHandler.bind(null,{cleanup:true}));
    //catches ctrl+c event
    process.on('SIGINT', exitHandler.bind(null, {exit:true}));
    // catches "kill pid" (for example: nodemon restart)
    process.on('SIGUSR1', exitHandler.bind(null, {exit:true}));
    process.on('SIGUSR2', exitHandler.bind(null, {exit:true}));
    //catches uncaught exceptions
    process.on('uncaughtException', exitHandler.bind(null, {exit:true}));
}

app.use("/", express.static(path.join(PATH, "client")));
server.listen(port, () => {
    log("listenen");
    function createUnixSocketServer(){
        if(fs.existsSync(SOCKET_LOCATION)){
            log(`Socket already exists. Unlinking ${SOCKET_LOCATION}.`);
            fs.unlinkSync(SOCKET_LOCATION);
        }
        socketServer = net.createServer((connection) =>{
            log("Recieved connection");
            lastconn = connection;
            connection.on("data", (data) => {
                message = data.toString();
                io.emit("sensor", message);       
            });
            connection.on("end", () => {
                log("Connection lost");
                lastconn = null;
                bindings = null;
            })

            
        });

        
        socketServer.listen(SOCKET_LOCATION, () => {
            log(`Socket Server created at ${SOCKET_LOCATION}.`);
        });

        return socketServer;
    }


    // var running_process = null;
    var lastconn = null;
    var bindings = null;
    var connecting_bluetooth = false;

    const sockets = new Set();

    var socketServer = createUnixSocketServer();

    io.on("connection", (socket) => {
        socket.emit("bindings", bindings)
        sockets.add(socket);
        socket.on("cmd", (cmd) => {
            cmd = `${cmd}\0`;
            log(`Sending command "${cmd}"`);
            if(lastconn !== null){
                lastconn.write(cmd);
            }
        });

        socket.on("req-json", () => {
            socket.emit("json" , config);
        });

        socket.on("update-json", (newjson) => {
            config = newjson;
            fs.writeFileSync(path.join(PATH, "config/config.json"), JSON.stringify(newjson));
        });

        socket.on("disconnect", ()=>{
            sockets.delete(socket);
        });

        socket.on("bluetooth", () => {
            if(!connecting_bluetooth){
                connecting_bluetooth = true;
                exec(`echo -e "connect F4:93:9F:3D:BC:77 \nquit" | bluetoothctl `, (error, stdout, stderr) => {
                    connecting_bluetooth = false;
                });
            }
        });

        socket.on("chg-pid", (type, val) => {
            const parameter = `3 ${type} ${val}\0`;
            log(`Changing parameter \"${parameter}\"`);
            lastconn.write(parameter);
        });

        socket.on("chg-value", (name, value) => {
            const cmd = `4 ${name} ${value}\0`;
            log(`Changing value \"${cmd}\"`);
            lastconn.write(cmd);

        });
    });
});