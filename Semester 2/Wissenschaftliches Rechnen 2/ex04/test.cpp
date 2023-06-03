#include <iostream>
#include <bsp/bsp.h>

int main(int argc, char** argv) {
    bsp_begin(4);

    int remote_int;
    bsp_push_reg(&remote_int, sizeof(int));
    bsp_sync();

    int src = bsp_pid();
    bsp_put((bsp_pid()+1) % bsp_nprocs(), &src, &remote_int, 0, sizeof(int));
    bsp_sync();

    bsp_pop_reg(&remote_int);
    std::cout << bsp_pid() << ": " << remote_int << std::endl;
    bsp_end();
}