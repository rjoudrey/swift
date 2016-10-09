#include "../MyExtension/MyExtension.h"
#include <iostream>

using namespace std;
int main() {
    int numVars;
    Var *var = getVars("var a: Int", &numVars);
    cout << "name: " << var->name << ", type: " << var->type << endl;
    return 0;
}
