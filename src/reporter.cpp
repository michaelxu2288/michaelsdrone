#include <reporter.h>
#include <vector>

static vector<const char *> names;
static vector<void *> bindings;

// 0 - bool
// 1 - int
// 2 - dbl
// 3 - str
static vector<uint8_t> types;




void reporter::bind_bool(const char * name, bool * value){
    names.push_back(name);
    bindings.push_back((void*) value);
    types.push_back(0);
}
