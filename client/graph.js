const colors = {
    grey1:"#b0acb0",
    grey2:"#e2dddf",
    light:"#85ebd9",
    primary:"#3d898d",
    dark:"#",
}
class LineGraph {
    constructor(element, options) {
        options = options ?? {};
        this.g_width = options.g_width ?? 300;
        this.g_height = options.g_height ?? 150;
        this.margin_l = options.margin_l ?? 30;
        this.margin_r = options.margin_r ?? 10;
        this.margin_t = options.margin_t ?? 10;
        this.margin_b = options.margin_b ?? 10;

        this.width = element.width = this.g_width + this.margin_l + this.margin_r;
        this.height = element.height = this.g_height + this.margin_t + this.margin_b;
        this.ctx = element.getContext("2d");
        this.element = element;

        this.lines = [];

        this.dataMax = 0;
        this.dataMin = 0;
        this.labelMax = 0;
        this.labelMin = 0;

        //this.drivenRange = options.driven_range ?? true;
        if (options.range) this.drivenRange = true;
        else this.drivenRange = false;
        this.range = options.range;
        if (!this.range) this.range = { data: {}, label: {} };
        if (!this.range.data) this.range.data = {};
        if (!this.range.label) this.range.label = {};

        this.max_data = options.max_data ?? 100;
        this.draw_zero_label = options.draw_zero_label ?? true;
        this.draw_zero_data = options.draw_zero_data ?? true;
        this.draw_secondary_label = options.draw_secondary_label ?? false;
        this.draw_secondary_data = options.draw_secondary_data ?? false;
        this.colors = [];

        this.visible = [];

        if(options.legend){
            this.legend = options.legend;
            this.generateLegend();
        }

    }

    generateLegend(element = this.legend.element){
        element.empty();
        // console.log(this.legend);
        this.lines.forEach((line, i) => {
            const c = $(`<span style="color: ${this.colors[i]};">⯀</span>`);
            var toggle = this.visible[i];
            const name = $(`<span>${this.legend.names[i]}</span>`);
            const ele = $(`<li/>`).append(c).append(" – ").append(name).click(() => {
                if(toggle){
                    ele.addClass("invisible");
                    toggle = false;
                }else{
                    ele.removeClass("invisible");
                    toggle = true;
                }
                this.visible[i] = toggle;
            });
            element.append(ele);
        });

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

    createLine(color) {

        const colors = [
            "#31e6c4",
            "#e68f31",
            "#e63167",
            "#744fe3",
            "#9be631",
        ];
        // const line = new Line();

        if(color == null){
            this.colors.push(colors[this.lines.push(new Line()) % 5]);
        }else {
            this.colors.push(color);
            this.lines.push(new Line());
        }
        
        this.generateLegend();
        this.visible.push(true);
    }

    createLines(n, colors){
        if(colors == null){
            colors = [
                "#31e6c4",
                "#e68f31",
                "#e63167",
                "#744fe3",
                "#9be631",
            ]
        }
        for(var i = 0; i < n; i ++){
            this.colors.push(colors[this.lines.push(new Line()) % 5]);
            this.visible.push(true);
        }

        this.generateLegend();
    }

    render(_ctx=null) {
        /**
         * @type {CanvasRenderingContext2D}
         */
        var ctx;
        if(_ctx) ctx = _ctx;
        else ctx = this.ctx;

        ctx.globalCompositeOperation = "color-dodge";
        ctx.clearRect(0, 0, this.width, this.height);
        ctx.lineWidth = 1.5;
        ctx.strokeStyle = "#f0f0f0";

        

        const stepY = 1;
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

        const _colors = this.colors;
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
        ctx.lineWidth = 1.0;
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
        ctx.strokeStyle = colors.light;
        ctx.lineWidth = 0.5;

        for (var i = startY; i <= endY; i += stepY) {
            const y = mapy(i);
            ctx.moveTo(this.margin_l, y);
            ctx.lineTo(this.g_width + this.margin_l, y);
        }
        ctx.stroke();

        // ctx.strokeStyle = "";
        // ctx.fillStytl
        ctx.fillStyle = "#174145";
        ctx.font = "bold 12px courier";
        ctx.textAlign = "right";
        ctx.textBaseline = "middle";

        for (i = startY; i <= endY; i += stepY) {
            ctx.fillText("" + i, this.margin_l - 4, mapy(i));
        }

        ctx.lineWidth = 2;
        // Draw data
        this.lines.forEach((line, i) => {
            if(!this.visible[i]) return;

            ctx.strokeStyle = _colors[i];
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
        if(isNaN(y)) return;
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




class TimeGraph {
    constructor(element, options) {
        options = options ?? {};
        this.g_width = options.g_width ?? 300;
        this.g_height = options.g_height ?? 150;
        this.margin_l = options.margin_l ?? 30;
        this.margin_r = options.margin_r ?? 10;
        this.margin_t = options.margin_t ?? 10;
        this.margin_b = options.margin_b ?? 10;

        this.width = element.width = this.g_width + this.margin_l + this.margin_r;
        this.height = element.height = this.g_height + this.margin_t + this.margin_b;
        this.ctx = element.getContext("2d");
        this.element = element;

        this.lines = [];

        this.dataMax = 0;
        this.dataMin = 0;
        this.labelMax = 0;
        this.labelMin = 0;

        //this.drivenRange = options.driven_range ?? true;
        if (options.range) this.drivenRange = true;
        else this.drivenRange = false;
        this.range = options.range;
        if (!this.range) this.range = { data: {}, label: {} };
        if (!this.range.data) this.range.data = {};
        if (!this.range.label) this.range.label = {};

        this.max_data = options.max_data ?? 100;
        this.draw_zero_label = options.draw_zero_label ?? true;
        this.draw_zero_data = options.draw_zero_data ?? true;
        this.draw_secondary_label = options.draw_secondary_label ?? false;
        this.draw_secondary_data = options.draw_secondary_data ?? false;
        this.colors = [];

        this.visible = [];

        // this.stepY = options.stepY ?? 1

        if(options.legend){
            this.legend = options.legend;
            this.generateLegend();
        }

    }

    generateLegend(element = this.legend.element){
        element.empty();
        // console.log(this.legend);
        this.lines.forEach((line, i) => {
            const c = $(`<span style="color: ${this.colors[i]};">⯀</span>`);
            var toggle = this.visible[i];
            const name = $(`<span>${this.legend.names[i]}</span>`);
            const ele = $(`<li/>`).append(c).append(" – ").append(name).click(() => {
                if(toggle){
                    ele.addClass("invisible");
                    toggle = false;
                }else{
                    ele.removeClass("invisible");
                    toggle = true;
                }

                
                this.visible[i] = toggle;
                this.calcMaxMin();
            });
            element.append(ele);
        });

    }

    calcMaxMin(){
        this.dataMax = -Infinity;
        this.dataMin = Infinity;
        this.lines.forEach((line, i) => {
            if(this.visible[i]){
                // console.log(line.maxY, line.minY);
                this.dataMax = Math.max(this.dataMax, line.maxY);
                this.dataMin = Math.min(this.dataMin, line.minY);
            }
        });
    }

    addData(line, y) {
        if(isNaN(y)) return;
        if (this.max_data == this.lines[line].points.length) {
            this.lines[line].shift();

            if(this.visible[line]){
                this.dataMax = this.lines.map((line, i) => (this.visible[i] ? line.maxY : -Infinity)).reduce((a, b) => Math.max(a, b));
                this.dataMin = this.lines.map((line, i) => (this.visible[i] ? line.minY : Infinity)).reduce((a, b) => Math.min(a, b));
            }// this.labelMax = this.lines.map((line) => line.maxX).reduce((a, b) => Math.max(a, b));
            // this.labelMin = this.lines.map((line) => line.minX).reduce((a, b) => Math.min(a, b));
        }
        this.lines[line].add(this.labelMax, y);
        if(this.visible[line]){
            this.dataMax = Math.max(this.dataMax, this.lines[line].maxY);
            this.dataMin = Math.min(this.dataMin, this.lines[line].minY);
        }
    }

    addDataList(ys){
        // console.log(ys);
        ys.forEach((y, i) => {
            this.addData(i, y);
        });
        this.next();
    }

    next(){
        this.labelMax++;
        this.labelMin=this.labelMax - this.max_data;
    }

    createLine(color) {

        const colors = [
            "#31e6c4",
            "#e68f31",
            "#e63167",
            "#e6d845",
            "#744fe3",
            "#9be631",
            "#1a7060",
            "#52701a",
            "#7a2828"
        ]
        // const line = new Line();

        if(color == null){
            this.colors.push(colors[this.lines.push(new Line()) % colors.length]);
        }else {
            this.colors.push(color);
            this.lines.push(new Line());
        }
        
        this.generateLegend();
        this.visible.push(true);
    }

    createLines(n, colors){
        if(colors == null){
            colors = [
                "#31e6c4",
                "#e68f31",
                "#e63167",
                "#e6d845",
                "#744fe3",
                "#9be631",
                "#1a7060",
                "#52701a",
                "#7a2828"
            ]
        }
        for(var i = 0; i < n; i ++){
            this.colors.push(colors[this.lines.push(new Line()) % colors.length]);
            this.visible.push(true);
        }

        this.generateLegend();
    }

    render(_ctx=null) {
        /**
         * @type {CanvasRenderingContext2D}
         */
        var ctx;
        if(_ctx) ctx = _ctx;
        else ctx = this.ctx;

        ctx.globalCompositeOperation = "color-dodge";
        ctx.clearRect(0, 0, this.width, this.height);
        ctx.lineWidth = 1.5;
        ctx.strokeStyle = "#f0f0f0";

        

        // const stepY = this.stepY;
        var stepY = 1;

        
        const diff = Math.abs(this.dataMax - this.dataMin);
        // stepY = Math.pow(10, Math.floor(Math.log10(diff)));
        if(diff < 0.01) {
            stepY = 0.001;
        }else if(diff < 0.1) {
            stepY = 0.01;
        }else if(diff < 1) {
            stepY = 0.1;
        }else if(diff < 10) {
            stepY = 1;
        }else if(diff < 100) {
            stepY = 10;
        }else if(diff < 1000) {
            stepY = 100;
        }else if(diff < 10000) {
            stepY = 1000;
        }else {
            stepY = 10000;
        }

        if(this.stepY){
            stepY = this.stepY;
        }


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

        const _colors = this.colors;
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
        ctx.lineWidth = 1.0;
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
        ctx.strokeStyle = colors.light;
        ctx.lineWidth = 0.5;

        for (var i = startY; i <= endY; i += stepY) {
            const y = mapy(i);
            ctx.moveTo(this.margin_l, y);
            ctx.lineTo(this.g_width + this.margin_l, y);
        }
        ctx.stroke();

        // ctx.strokeStyle = "";
        // ctx.fillStytl
        ctx.fillStyle = "#174145";
        ctx.font = "bold 12px courier";
        ctx.textAlign = "right";
        ctx.textBaseline = "middle";

        for (i = startY; i <= endY; i += stepY) {
            ctx.fillText("" + i, this.margin_l - 4, mapy(i));
        }

        ctx.lineWidth = 2;
        // Draw data
        this.lines.forEach((line, i) => {
            if(!this.visible[i]) return;

            ctx.strokeStyle = _colors[i];
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
