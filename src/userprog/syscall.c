#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}
/*
    SYS_HALT,                   //Halt the operating system
    SYS_EXIT,                   // Terminate this process. 
    SYS_EXEC,                   // Start another process. 
    SYS_WAIT,                   // Wait for a child process to die. 
    SYS_CREATE,                 // Create a file. 
    SYS_REMOVE,                 // Delete a file. 
    SYS_OPEN,                   // Open a file. 
    SYS_FILESIZE,               // Obtain a file's size. 
    SYS_READ,                   // Read from a file. 
    SYS_WRITE,                  // Write to a file. 
    SYS_SEEK,                   // Change position in a file. 
    SYS_TELL,                   // Report current position in a file. 
    SYS_CLOSE,                  // Close a file. 
	*/
static void
syscall_handler (struct intr_frame *f) 
{
  switch(*(int*)f->esp) {
  case  SYS_HALT:                   /* Halt the operating system. */
    shutdown_power_off();
    break;
  case  SYS_EXIT:                   /* Terminate this process. */
    break;
  case  SYS_EXEC:                   /* Start another process. */
    break;
  case  SYS_WAIT:                   /* Wait for a child process to die. */
    break;
  case  SYS_CREATE:                 /* Create a file. */
    break;
  case  SYS_REMOVE:                 /* Delete a file. */
    break;
  case  SYS_OPEN:                   /* Open a file. */
    break;
  case  SYS_FILESIZE:               /* Obtain a file's size. */
    break;
  case  SYS_READ:                   /* Read from a file. */
    break;
  case  SYS_WRITE:                  /* Write to a file. */
    break;
  case  SYS_SEEK:                   /* Change position in a file. */
    break;
  case  SYS_TELL:                   /* Report current position in a file. */
    break;
  case  SYS_CLOSE:                  /* Close a file. */
    break;
  }
  thread_exit ();
}
