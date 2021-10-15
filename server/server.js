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

// const index = fs.readFileSync("./client/index.html");
// const style = fs.readFileSync("./client/style.css");
// const script = fs.readFileSync("./client/script.js");


// app.get("/", (req, res) => {
//     res.setHeader('content-type', 'text/html');
//     res.send(index);
// });
// app.get("/style.css", (req, res) => {
//     res.setHeader('content-type', 'text/css');
//     res.send(style);
// });
// app.get("/script.js", (req, res) => {
//     res.setHeader('content-type', 'text/javascript');
//     res.send(script);
// });


server.listen(port, () => {
    console.log("listenen");

    var lastSensorOutput = [];

    var server = net.createServer((connection) =>{
        connection.on("data", (data) => {
            lastSensorOutput = data.toString().split(" ");
            console.log(lastSensorOutput);
            io.emit("sensor", lastSensorOutput);
        });
    });

    server.listen("/run/test", () => {
        console.log("Socket Server created at /run/test!");
    });
    // var client = net.connect({
    //     path: "/run/test"
    // })
    // client.on("connect", () => {
    //     client.write("SUCCESS-NODE");
    // })
    // client.on("data", (data) => {
    //     lastSensorOutput = data.toString().split(" ");
    //     console.log(lastSensorOutput);
    //     io.emit("sensor", lastSensorOutput);
    // });
    // process.stdin.resume();
    // process.stdin.setEncoding('utf8');
    // process.stdin.on("data", function(chunk) {
    //     const lines = chunk.toString("utf8").split("\n");
    //     // console.log(chunk);
    //     // console.log(lines);
    //     lastSensorOutput = lines[lines.length - 2].split(" ");
    //     //console.log(lastSensorOutput);
    //     io.emit("sensor", lastSensorOutput);
    // });

    // io.on("connection", (socket) => {
    //     console.log('socket conected');
    // });
});