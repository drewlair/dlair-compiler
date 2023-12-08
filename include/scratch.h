
enum {
    RBX,
    R12,
    R13,
    R14,
    R15,
};

enum {
    RDI,
    RSI,
    RDX,
    RCX,
};


int scratch_alloc();
void scratch_free( int reg );
const char *scratch_name( int reg );
int param_allocate();
void param_free();
const char *param_name( int reg );
int float_allocate();
void float_free( int reg );
const char *float_name( int reg );
const char *float_label();
