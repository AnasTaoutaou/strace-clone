#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/reg.h>

int main(int argc, char **argv){

long orig_eax, eax;
struct user_regs_struct regs;
const char* filename = argv[1];
int status;
pid_t pid;
pid = fork();
int c = 0;
if(pid == 0){
    ptrace(PTRACE_TRACEME,0,NULL,NULL);
    printf("[*]Tracing started! \n");
    execl(filename,filename,0);
        }
else{

    wait(&status);
    while(WIFSTOPPED(status)){
        c++;
        ptrace(PTRACE_GETREGS,pid,NULL,&regs);
        if(regs.orig_eax == 4){
        printf("%lld\n",regs.orig_eax);
        printf("\n%%ecx: %x\n%%edx: %x\n%%ebx: %x\n"
        "%%eax: %x\n%%edi: %x\n%%esi: %x\n"
        "%%esp: %x \n",
        regs.ecx, regs.edx, regs.ebx,
        regs.eax, regs.edi, regs.esi,
        regs.esp);
        long d = ptrace(PTRACE_PEEKDATA,pid,regs.ecx,NULL);
        printf("/n%lx/n",d);
                          }
        ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
        wait(&status);
                     }
    }
printf("\n SYSCALLS count is: %d \n",c);
return 0;
}
