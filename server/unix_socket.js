const net = require("net");

var client = net.connect({
    path: "/run/test"
})
client.on("connect", () => {
    client.write("SUCCESS-NODE");
})
client.on("data", (data) => {
    console.log(data.toString());
});