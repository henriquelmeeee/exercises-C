#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("henriquedev");
MODULE_DESCRIPTION("Hook de syscall");
MODULE_VERSION("0.01");

unsigned long **sys_call_table;
unsigned long *original_address;

unsigned long** find_sys_call() {
   unsigned long offset = PAGE_OFFSET;
   unsigned long **sct;
   while(offset < ULLONG_MAX) {// para evitar overflow
     sct = (unsigned long **) offset;
     if(sct[__NR_close] == (unsigned long *) sys_close) return sct;
     // sct[__NR_close] nos dá outro ponteiro (porque sct em si é um ponteiro duplo)
     // então, baseado nisso, nós vemos se esse ponteiro é igual ao ponteiro de sys_close
     offset += sizeof(void *);
   }
   return NULL;
}

asmlinkage long new_function() {
  printk(KERN_INFO "Syscall hookada com sucesso");
  return (*original_address)();
}

void init_module(void) {
  write_cr0(read_cr0() & (~0x10000));
  sys_call_table = find_sys_call();
  unsigned long *addr_sys_get_ppid =  (*(sys_call_table[__NR_getppid]));
  original_address = *(addr_sys_get_ppid);
  sys_call_table[__NR_getppid] = (unsigned long *) new_function;
  write_cr0(read_cr0() | 0x10000);
}

void __exit exit_module(void) {
}

module_init(init_module);
module_exit(exit_module)
