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
const port = 80;

const videoStream = require('raspberrypi-node-camera-web-streamer/videoStream.js');

const config = require("../config/config.json");

const SOCKET_LOCATION = config.socket_path;

const process = require("process");

const { exec, execSync } = require('child_process');

function exitHandler(options, exitCode){
    console.log(`Exiting with code "${exitCode}"`);

    server.close();
    fs.unlinkSync(SOCKET_LOCATION);

    

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




server.listen(port, () => {
    console.log("listenen");

    var running_process = null;
    var lastconn = null;

    var server = net.createServer((connection) =>{
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

    
    io.on("connection", (socket) => {
        socket.on("cmd", (cmd) => {
            cmd = `${cmd}`;
            console.log(`Sending command "${cmd}"`);
            if(lastconn !== null){
                lastconn.write(cmd);
            }
        });

        fs.readdir("./tools/", (err, files) => {
            socket.emit("proglist", files);
        });
        // socket.emit("proglist", fs);

        socket.on("make", (file) => {
            const bruh = file.split(".")[0];
            // execSync("git pull");
            // execSync(`make ${bruh}`);
            
            if(running_process !== null){
                running_process.kill();
            }

            // execSync(`sudo ./bin/${bruh}`);
            running_process = exec("git pull", (err, stdout, stderr) => {
                console.log("git pull: ");
                console.log(stdout);
                running_process = exec(`make ${bruh}`, (err, stdout, stderr) => {
                    console.log(`make ${bruh}`);
                    console.log(stdout);
                    running_process = exec(`sudo ./bin/${bruh}`, (err, stdout, stderr) => {
                        console.log(`sudo ./bin/${bruh}`);
                        console.log(stdout);
                        running_process = null;
                    });
                });
            });
        }); 
    });

    server.listen(SOCKET_LOCATION, () => {
        console.log(`Socket Server created at ${SOCKET_LOCATION}.`);
    });
});