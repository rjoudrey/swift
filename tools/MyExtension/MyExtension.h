struct Var {
    const char *name;
    const char *type;
};

extern "C" {
    Var* getVars(const char *code, int *count);
}
