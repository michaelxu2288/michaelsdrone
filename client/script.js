/* jshint esversion:8  */
var socket;
var host = "";
var graph;


var Drone = {
    acceleration: {
        x: 0,
        y: 0,
        z: 0,
    },

    history: {
        acceleration: {
            x: [],
            y: [],
            z: [],
        },
    },
};


function setup() {
    socket = io(host);
    var i = 0;
    socket.on("sensor", (sensorsRaw) => {
        console.log(sensorsRaw);
        graph.addData(0, i++, sensorsRaw[0]);
    });

    window.onclose = () => {
        socket.disconnect();
    };
}

function main() {
    // graph_drone_accel_main();
    // graph_drone_accel_x_main();

    // var then = 0;
    // const update = (now) => {
    //     const dt = (now - then) / 1000;
    //     then = now;

    //     render_drone_accel();
    //     render_drone_accel_x();

    //     requestAnimationFrame(update);
    // };

    // requestAnimationFrame(update);

    graph = new LineGraph(document.getElementById("graph-test"), {
        g_width: 1000,
        g_height: 150,
        margin_l: 30,
        margin_r: 0,
        margin_t: 10,
        margin_b: 10,
        range: {
            data: {
                max: 100,
                min: -100
            }
        },
        max_data: 500,
    });

    graph.createLine();

    // console.log(graph.lines);
    graph.render();
    // var j = 10;
    setInterval(() => {
        // j += 0.1;
        // graph.addData(0, j, Math.random() * 81);
        graph.render();

    }, 10);
}

class LineGraph {
    constructor(element, options) {
        this.g_width = options.g_width;
        this.g_height = options.g_height;
        this.margin_l = options.margin_l;
        this.margin_r = options.margin_r;
        this.margin_t = options.margin_t;
        this.margin_b = options.margin_b;

        this.width = element.width = this.g_width + this.margin_l + this.margin_r;
        this.height = element.height = this.g_height + this.margin_t + this.margin_b;
        this.ctx = element.getContext("2d");
        this.element = element;

        this.lines = [];

        this.dataMax = 0;
        this.dataMin = 0;
        this.labelMax = 0;
        this.labelMin = 0;

        if (options.range) this.drivenRange = true;
        else this.drivenRange = false;
        this.range = options.range;
        if (!this.range) this.range = { data: {}, label: {} };
        if (!this.range.data) this.range.data = {};
        if (!this.range.label) this.range.label = {};

        this.max_data = options.max_data;
        if (!this.max_data) this.max_data = 100;
        this.draw_zero_label = options.draw_zero_label;
        this.draw_zero_label = options.draw_zero_data;
        this.draw_zero_label = options.draw_secondary_label;
        this.draw_zero_label = options.draw_secondary_data;
    }

    addData(line, x, y) {
        if (this.max_data == this.lines[line].points.length) {
            this.lines[line].shift();

            this.dataMax = this.lines.map((line) => line.maxY).reduce((a, b) => Math.max(a, b));
            this.dataMin = this.lines.map((line) => line.minY).reduce((a, b) => Math.min(a, b));
            this.labelMax = this.lines.map((line) => line.maxX).reduce((a, b) => Math.max(a, b));
            this.labelMin = this.lines.map((line) => line.minX).reduce((a, b) => Math.min(a, b));

        }
        this.lines[line].add(x, y);

        this.dataMax = Math.max(this.dataMax, this.lines[line].maxY);
        this.dataMin = Math.min(this.dataMin, this.lines[line].minY);
        this.labelMax = Math.max(this.labelMax, this.lines[line].maxX);
        this.labelMin = Math.min(this.labelMin, this.lines[line].minX);
    }

    createLine() {
        this.lines.push(new Line());
    }

    render() {
        /**
         * @type {CanvasRenderingContext2D}
         */
        const ctx = this.ctx;
        ctx.clearRect(0, 0, this.width, this.height);
        ctx.lineWidth = 1.5;
        ctx.strokeStyle = "#444";

        const stepY = 20;
        var range = {
            data: {
                max: this.dataMax + stepY / 2,
                min: this.dataMin - stepY / 2,
            },
            label: {
                max: this.labelMax,
                min: this.labelMin,
            }
        };

        const colors = [
            "#EE8434",
            "#C95D63",
            "#AE8799",
            "#717EC3",
            "#496DDB",
        ];
        if (this.range.data.max) range.data.max = this.range.data.max;
        if (this.range.data.min) range.data.min = this.range.data.min;
        if (this.range.label.max) range.label.max = this.range.label.max;
        if (this.range.label.min) range.label.min = this.range.label.min;

        // Axes
        // Label

        const x_scaling = this.g_width / (range.label.max - range.label.min);
        const mapx = (x) => {
            return (x - range.label.min) * x_scaling + this.margin_l;
        };
        const y_scaling = this.g_height / (range.data.max - range.data.min);
        const mapy = (y) => {
            return -(y - range.data.min) * y_scaling + this.margin_t + this.g_height;
        };

        const x_0 = mapx(0);
        const y_0 = mapy(0);

        ctx.beginPath();
        if (range.data.max * range.data.min <= 0) {
            ctx.moveTo(this.margin_l, y_0);
            ctx.lineTo(this.margin_l + this.g_width, y_0);
        }
        if (range.label.max * range.label.min <= 0) {
            ctx.moveTo(x_0, this.margin_t);
            ctx.lineTo(x_0, this.margin_t + this.g_height);
        }
        ctx.stroke();



        // Secondary Axis and scale
        const startY = range.data.min - range.data.min % stepY;
        const endY = range.data.max - range.data.max % stepY;

        ctx.beginPath();
        ctx.strokeStyle = "#AAA";

        for (var i = startY; i <= endY; i += stepY) {
            const y = mapy(i);
            ctx.moveTo(this.margin_l, y);
            ctx.lineTo(this.g_width + this.margin_l, y);
        }
        ctx.stroke();

        ctx.strokeStyle = "#666";
        ctx.font = "normal 16px sans-serif";
        ctx.textAlign = "right";
        ctx.textBaseline = "middle";

        for (i = startY; i <= endY; i += stepY) {
            ctx.fillText("" + i, this.margin_l - 4, mapy(i));
        }

        // Draw data
        this.lines.forEach((line, i) => {
            ctx.strokeStyle = colors[i];
            if (line.points.length > 0) {
                ctx.beginPath();
                ctx.moveTo(mapx(line.points[0].x), mapy(line.points[0].y));

                line.points.forEach((point) => {
                    // console.log(point)
                    // console
                    ctx.lineTo(mapx(point.x), mapy(point.y));
                });
                ctx.stroke();
            }
        });




    }
}


class Line {
    constructor() {
        this.maxX = 0;
        this.maxY = 0;
        this.minX = 0;
        this.minY = 0;

        this.points = [];
    }

    add(x, y) {
        this.points.push({ x: x, y: y });
        this.maxX = Math.max(x, this.maxX);
        this.minX = Math.min(x, this.minX);
        this.maxY = Math.max(y, this.maxY);
        this.minY = Math.min(y, this.minY);
    }

    shift() {
        this.points.shift();
        const x = this.points.map((point) => point.x);
        const y = this.points.map((point) => point.y);

        this.maxX = x.reduce((a, b) => Math.max(a, b));
        this.minX = x.reduce((a, b) => Math.min(a, b));
        this.maxY = y.reduce((a, b) => Math.max(a, b));
        this.minY = y.reduce((a, b) => Math.min(a, b));
    }
}

function dc() {
    if (socket) {
        socket.disconnect();
    }
}