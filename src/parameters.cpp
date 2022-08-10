#include <parameters.h>
#include <vector>
#include <logger.h>

static std::vector<const char *> readonly_names;
static std::vector<void *> readonly_bindings;

static std::vector<const char *> writable_names;
static std::vector<void *> writable_bindings;

// 0 - bool
// 1 - int
// 2 - dbl
// 3 - str
static std::vector<char> readonly_types;
static std::vector<char> writable_types;

void parameters::bind_bool(const char * name, bool * value, bool readonly){
    if(readonly) {
        readonly_names.push_back(name);
        readonly_bindings.push_back((void*) value);
        readonly_types.push_back(0);
    }else {
        writable_names.push_back(name);
        writable_bindings.push_back((void*) value);
        writable_types.push_back(0);
    }
}


void parameters::bind_int(const char * name, int * value, bool readonly){
    if(readonly) {
        readonly_names.push_back(name);
        readonly_bindings.push_back((void*) value);
        readonly_types.push_back(1);
    }else {
        writable_names.push_back(name);
        writable_bindings.push_back((void*) value);
        writable_types.push_back(1);
    }
}


void parameters::bind_dbl(const char * name, double * value, bool readonly){
    if(readonly) {
        readonly_names.push_back(name);
        readonly_bindings.push_back((void*) value);
        readonly_types.push_back(2);
    }else {
        writable_names.push_back(name);
        writable_bindings.push_back((void*) value);
        writable_types.push_back(2);
    }
}


void parameters::bind_str(const char * name, std::string * value, bool readonly){
    if(readonly) {
        readonly_names.push_back(name);
        readonly_bindings.push_back((void*) value);
        readonly_types.push_back(3);
    }else {
        writable_names.push_back(name);
        writable_bindings.push_back((void*) value);
        writable_types.push_back(3);
    }
}

// std::string parameters::get_report(){
//     std::string out = "";
//     for(int i = 0; i < readonly_names.size(); i ++){
//         switch(readonly_types[i]){
//             case 0:
//                 out += std::to_string(*((bool *) (readonly_bindings[i])));
//                 break;
//             case 1:
//                 out += std::to_string(*((int *) (readonly_bindings[i])));
//                 break;
//             case 2:
//                 out += std::to_string(*((double *) (readonly_bindings[i])));
//                 break;
//             case 3:
//                 out +=*((std::string *) readonly_bindings[i]);
//                 break;
//             default:
//                 out += "null";
//                 break;
//         }
//         out += " ";
//     }
//     return out;
// }


static std::string json_helper(std::vector<const char *> & names, std::vector<void *> & bindings, std::vector<char> & types){
    std::string out = "{";
    int k = names.size();
    for(int i = 0; i < k; i ++){
        std::string name(names[i]);
        out+="\""+name+"\":";
        switch(types[i]){
            case 0:
                out += std::to_string(*((bool *) (bindings[i])));
                break;
            case 1:
                out += std::to_string(*((int *) (bindings[i])));
                break;
            case 2:
                out += std::to_string(*((double *) (bindings[i])));
                break;
            case 3:
                out += '"'+*((std::string *) bindings[i])+'"';
                break;
            default:
                out += "null";
                break;
        }
        if(i != k-1) {
            out += ",";
        }
    }
    return out+"}";
}

std::string parameters::get_json_report(){
    return "{writable:"+json_helper(writable_names, writable_bindings, writable_types)+",readable:{"+json_helper(readonly_names, readonly_bindings, readonly_types)+"}";
}

// std::string parameters::get_keys() {
//     std::string out = "";
//     for(int i = 0; i < readonly_names.size(); i ++){
//         out += readonly_names[i];
//         out += " ";
//     }
//     return out;
// }


char buf[20];
void parameters::chg(const char * str) {
    int i;
    for(i = 0; str[i] != ' '; i++) {
        if(str[i] == '\0' || i >= 20){
            logger::crit("\"{}\" is not a valid input.", str);
            return;
        }
        buf[i] = str[i];
    }
    buf[i] = '\0';
    i++;
    int j = std::atoi(buf);
    switch(readonly_types[j]){
    case 0:
        (*((bool *) readonly_bindings[j])) = str[i] == '1';
        break;
    case 1:
        (*((int *) readonly_bindings[j]))  = std::atoi(str + i);
        break;
    case 2:
        (*((double *) readonly_bindings[j]))  = std::atof(str+i);
        break;
    case 3:
        break;
    default:
        break;
    }

}