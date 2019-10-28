#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
#include "userprog/process.h"
#include "devices/input.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  switch(*(int*)f->esp) {
  case  SYS_HALT: {                   /* Halt the operating system. */
    shutdown_power_off();
    break;
  }
  case  SYS_EXIT: {                   /* Terminate this process. */
    struct thread* cur = thread_current();
    cur->is_done = true;
    sema_down(&cur->sema);
    thread_exit();
    break;
  }
  case  SYS_EXEC: {                   /* Start another process. */
    const char* cmd_line = (char *)((int *)f->esp + 1);
    /* TODO: cmd_line address validation */
    process_execute(cmd_line);
    break;
  }
  case  SYS_WAIT: {                   /* Wait for a child process to die. */
    tid_t tid = *(tid_t*)((char*)f->esp + 4);
	  process_wait(tid);
    break;
  }
  case  SYS_CREATE: {                 /* Create a file. */
    break;
  }
  case  SYS_REMOVE: {                 /* Delete a file. */
    break;
  }
  case  SYS_OPEN: {                  /* Open a file. */
    break;
  }
  case  SYS_FILESIZE: {               /* Obtain a file's size. */
    break;
  }
  case  SYS_READ: {                   /* Read from a file. */
    unsigned size = *(unsigned*)((char*)f->esp + 12);
    char *buffer = *(void**)((char*)f->esp + 8);
    int fd = *(int*)((char*)f->esp + 4);   
    if (fd == 0) {
      unsigned i;
      for (i = 0; i < size; ++i) {
        buffer[i] = input_getc();
      }
      f->eax = size;
    }
    else {
      f->eax = 0;
    }
    break;
  }
  case  SYS_WRITE: {                 /* Write to a file. */
    unsigned size = *(unsigned*)((char*)f->esp + 12);
    const void *buffer = *(const void**)((char*)f->esp + 8);
    int fd = *(int*)((char*)f->esp + 4);
    /* TODO: buffer address validation */
    if (fd == 1) {
      putbuf(buffer, size);
      f->eax = size;
    }
    else {
      f->eax = 0;
    }
    break;
  }
  case  SYS_SEEK: {                   /* Change position in a file. */
    break;
  }
  case  SYS_TELL: {                   /* Report current position in a file. */
    break;
  }
  case  SYS_CLOSE: {                  /* Close a file. */
    break;
  }
  }
}
