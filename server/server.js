/* jshint esversion: 8 */
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

const { parseArgsStringToArgv } = require('string-argv');

const port = 80;

const videoStream = require('raspberrypi-node-camera-web-streamer/videoStream.js');

var config = require("../config/config.json");

const SOCKET_LOCATION = config.socket_path;

const process = require("process");

const { exec, execSync, spawn } = require('child_process');

var socket_server;

function exitHandler(options, exitCode){
    console.log(`Exiting with code "${exitCode}"`);

    socket_server.close();
    // fs.unlinkSync(SOCKET_LOCATION);

    

    if (options.cleanup) console.log('clean');
    if (exitCode || exitCode === 0) console.log(exitCode);
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






videoStream.acceptConnections(app, {
    width: 2560,
    height: 1440,
    fps: 15,
    encoding: 'JPEG',
    quality: 12 //lower is faster
}, '/stream.mjpg', true);


// app.get("/", (req, res) => {

// });



server.listen(port, () => {
    console.log("listenen");

    // var running_process = null;
    var lastconn = null;

    socket_server = net.createServer((connection) =>{
        lastconn = connection;
        connection.on("data", (data) => {
            message = data.toString().split(" ");
            io.emit("sensor", message);
            // console.log(message);
            
        });
        connection.on("end", () => {
            console.log("Connection lost");
            lastconn = null;
        })
    });

    const sockets = new Set();

    io.on("connection", (socket) => {
        sockets.add(socket);
        socket.on("cmd", (cmd) => {
            cmd = `${cmd}`;
            console.log(`Sending command "${cmd}"`);
            if(lastconn !== null){
                lastconn.write(cmd);
            }
        });

        socket.on("req-json", () => {
            socket.emit("json" , config);
        });

        socket.on("update-json", (newjson) => {
            config = newjson;
            fs.writeFileSync("./config/config.json", newjson);
        });

        socket.on("disconnect", ()=>{
            sockets.delete(socket);
        });
    });

    server.listen(SOCKET_LOCATION, () => {
        console.log(`Socket Server created at ${SOCKET_LOCATION}.`);
    });
});