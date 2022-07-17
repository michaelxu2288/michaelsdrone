

// const elements = {};
// const curr = {};

function getType(obj){
    if(obj === null) {
        return "null";
    } 
    const type = typeof(obj);
    
    if(type === "object"){
        if(Array.isArray(obj)){
            return "array";
        }
    }
    return type;
}


var colorIndex = 0;
const colorClasses = [
    "col-1", "col-2", "col-3"
];

const arrowDownSrc = `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 448 512" fill="#FFFFFF"><!--! Font Awesome Pro 6.1.1 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license (Commercial License) Copyright 2022 Fonticons, Inc. --><path d="M224 416c-8.188 0-16.38-3.125-22.62-9.375l-192-192c-12.5-12.5-12.5-32.75 0-45.25s32.75-12.5 45.25 0L224 338.8l169.4-169.4c12.5-12.5 32.75-12.5 45.25 0s12.5 32.75 0 45.25l-192 192C240.4 412.9 232.2 416 224 416z"/></svg>`;
const options = [{name: "string", val: 0}, {name: "number", val: 1}, {name: "object", val: 2}];
function makeTypeSelect(){
    return $(`<select>${options.reduce((prev, curr) => prev+`<option value='${curr.val}'>${curr.name}</option>`,"")}</select>`);
}
var id = 0;
function makeId(){
    return id++;
}

function getDefaultVal(t){
    if(t == 0){
        return "-";
    }else if(t == 1) {
        return 0;
    }else if(t == 2){
        return {};
    }
}

function makeObjInput(initval, name, parent, curr, color, changeType = () => {}, remove = ()=>{}){
    // const id = `drop-${makeId()}`;
    const bruh =$(`<div/>`);
    const nextColor = (color+1) % colorClasses.length
    console.log(initval);

    function keyInput(key) {
        const dude = $("<div/>");

        const c = () => {
            return buildConfigurer(initval[key], initval, key, curr, nextColor,(type) => {
                initval[key] = getDefaultVal(type);
                // console.log(initval, parent);
                dude.empty();
                dude.append(c());
            }, () => {
                dude.remove();
                delete initval[key];
                // console.log(parent);
            })
        }

        dude.append(c());
        bruh.append(dude);
    }

    Object.keys(initval).forEach((key) => {
        keyInput(key);
    });
    const arr = $(arrowDownSrc).addClass("rotated");
    var toggle = true;
    const dropdown = $("<div class='reduce closed'/>")
                        .append($(`<div class="color ${colorClasses[color]}"/>`))
                        .append($("<div/>")
                        .append(bruh)
                        .append($("<div class='cell center add'>+</div>")
                            .click(() => {
                                // addVal();
                                var key = "_"
                                while(initval.hasOwnProperty(key)){
                                    key += "_";
                                }
                                initval[key] = 0;
                                
                                keyInput(key);
                            })
                        ));

    const title = $(`<div class="title cell"></div>`)
        .append($("<div/>")
            .append(arr)
            .append($(`<input ${(parent == null ? "disabled" : "")}/>`)
                .val(name)
                // .attr("disabled","disabled")
                .on("change" , (e) => {
                    parent[e.target.value] = parent[name];
                    delete parent[name];
                    name = e.target.value;
                    console.log(parent);
                }))
            .click(() => {
                if(toggle){
                    arr.removeClass("rotated");
                    dropdown.removeClass("closed");
                }else{
                    arr.addClass("rotated");
                    dropdown.addClass("closed");
                }
                toggle = !toggle;
            }));
        
        if(parent !== null) {
            title.append(makeTypeSelect().val(2).on("change", (e) => {
                changeType(parseInt(e.target.value));
            }));
            title.append(
            $(`<div class='center hoverable'>remove</div>`).click(() => {
                // delete parent[name];
                remove();
            }));
        }


    return $(`<div/>`).append(title).append(dropdown);
}

var curr = null;
function buildConfigurer(obj,parent=null, name="", curr, color=0,changeType = () => {}, remove=()=>{}) {
    const typeobj = getType(obj);

    if(typeobj === "object"){
        return makeObjInput(obj, name, parent, curr, color, changeType, remove);
    }else if(typeobj === "number") {
        const keyNameInput = $(`<input class="key"/>`).val(name).on("change" , (e) => {
            parent[e.target.value] = parent[name];
            delete parent[name];
            name = e.target.value;
        });
        return $(`<div class=\"key-val\"/>`)
            .append(keyNameInput)
            .append($("<div/>")
                .append(makeTypeSelect()
                    .val(1)
                    .on("change", (e) => {changeType(parseInt(e.target.value));})
                ).append(
                    $("<input class='value'>")
                        .val(obj)
                        .on("change", (e) => {
                            // parentobj[key] = parseFloat(e.target.value);
                            parent[name] = parseFloat(e.target.value);
                    })
            )).append(
                $("<div class='center hoverable'>remove</div>").click(() => {
                    remove(name);
                })
            );
    }else if (typeobj === "string") {
        const keyNameInput = $(`<input class="key"/>`).val(name).on("change" , (e) => {
            parent[e.target.value] = parent[name];
            delete parent[name];
            name = e.target.value;
        });
        return $(`<div class=\"key-val\"/>`)
            .append(keyNameInput)
            .append($("<div/>")
                .append(makeTypeSelect()
                    .val(0)
                    .on("change", (e) => {changeType(parseInt(e.target.value));})
                ).append(
                    $("<input class='value'>")
                        .val(obj)
                        .on("change", (e) => {
                            parent[name] = e.target.value;
                    })
            )).append(
                $("<div class='center hoverable'>remove</div>").click(() => {
                    remove(name);
                })
            );
    }
}

function main() {
}