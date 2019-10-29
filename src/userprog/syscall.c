#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
#include "userprog/process.h"
#include "devices/input.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}
  
/* Reads a byte at user virtual address UADDR.
   UADDR must be below PHYS_BASE.
   Returns the byte value if successful, -1 if a segfault
   occurred. */
static int
get_user (const uint8_t *uaddr)
{
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
       : "=&a" (result) : "m" (*uaddr));
  return result;
}
 
/* Writes BYTE to user address UDST.
   UDST must be below PHYS_BASE.
   Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
       : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}

static bool check_range(const uint8_t* begin, const uint8_t* end) {
  if (is_user_vaddr(begin) && is_user_vaddr(end - 1)) {
    if (get_user(begin) != -1 && get_user(end - 1) != -1) {
      return true;
    }
  }
  return false;
}

static void
syscall_handler (struct intr_frame *f) 
{
  switch (*(int*)f->esp) {
  case  SYS_HALT: {                   /* Halt the operating system. */
    shutdown_power_off();
    break;
  }

  case  SYS_EXIT: {                   /* Terminate this process. */
    struct thread* cur = thread_current();
    cur->is_done = true;
    cur->ret_stat = *((int*)f->esp + 1);
    printf ("%s: exit(%d)\n", cur->name, cur->ret_stat); 
    sema_down(&cur->sema);
    thread_exit();
    break;
  }

  case  SYS_EXEC: {                   /* Start another process. */
    const char* cmd_line = *(char **)((int *)f->esp + 1);
    const char* p = cmd_line;
    bool memory_error = false;
    while (is_user_vaddr(p)) {
      int byte = get_user((uint8_t*)p);
      if (byte == -1) {
        memory_error = true;
        break;
      }
      if (byte == 0) {
        break;
      }
      ++p;
    }
    if (!memory_error) {
      f->eax = process_execute(cmd_line);
    }
    else {
      f->eax = -1;
    }
    break;
  }

  case  SYS_WAIT: {                   /* Wait for a child process to die. */
    tid_t tid = *(tid_t*)((char*)f->esp + 4);
	  f->eax = process_wait(tid);
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
    if (!check_range((uint8_t*)buffer, (uint8_t*)buffer + size)) {
      f->eax = -1;
      break;
    }
    if (fd == 0) {
      unsigned i; 
      for (i = 0; i < size; ++i) {
        if (!put_user((uint8_t*)(buffer + i), input_getc())) {
          break;
        }
      }
      f->eax = i;
    }
    else {
      f->eax = -1;
    }
    break;
  }

  case  SYS_WRITE: {                 /* Write to a file. */
    unsigned size = *(unsigned*)((char*)f->esp + 12);
    const void *buffer = *(const void**)((char*)f->esp + 8);
    int fd = *(int*)((char*)f->esp + 4);
    if (!check_range(buffer, buffer + size)) {
      f->eax = -1;
      break;
    }
    if (fd == 1) {
      putbuf(buffer, size);
      f->eax = size;
    }
    else {
      f->eax = -1;
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
