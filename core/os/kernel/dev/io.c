
// io.c

#include <kernel.h> 


int ioInit(void)
{
    int Status = 0;

    debug_print ("ioInit: [TODO]\n");

    // ...

    return (int) Status;
}


//==========================================
// This is called by ioctl() in ring3.

// OK Isso eh um wrapper.
// Chamaremos tty_ioctl() ou outros ...  
// ...

// See:
// http://man7.org/linux/man-pages/man2/ioctl.2.html
// https://en.wikipedia.org/wiki/Ioctl

// The ioctl() system call manipulates the 
// underlying device parameters of special files.
// In particular, many operating characteristics of
// character special files (e.g., terminals) may be controlled with
// ioctl() requests.  The argument fd must be an open file descriptor.

// return:
// EBADF  fd is not a valid file descriptor.
// EFAULT argp references an inaccessible memory area.
// EINVAL request or argp is not valid.
// ENOTTY fd is not associated with a character special device.
// ENOTTY 
// The specified request does not apply to the kind of object
// that the file descriptor fd references
// See:
// https://man7.org/linux/man-pages/man2/ioctl.2.html

    // #bugbug
    // arg is the address for the arguments.
    // We are using argument not as an address sometimes.
    // it depends on the request number.

// Called by sys_ioctl() in sys.c
// But this routine can be called by the routines inside the kernel.

// pega o arquivo.
// checa o tipo de objeto.
// Isso deve ser usado principalmente com dispositivos 
// de caracteres como o terminal.
// #todo
// check file structure validation.
// The TIOCSTI (terminal I/O control, 
// simulate terminal input) ioctl 
// function can push a character into a device stream
// ENOTTY -  "Not a typewriter"
// #todo
// Now we can use a swit to call different
// functions, as tty_ioctl etc.

int 
io_ioctl ( 
    int fd, 
    unsigned long request, 
    unsigned long arg )
{
    file *f;
    int ObjectType = -1;

    debug_print ("io_ioctl: [TODO]\n");
    //printf ("io_ioctl: [TODO]\n");

    if ( fd < 0 || fd >= OPEN_MAX ){
        return (int) (-EBADF);
    }

// Get file pointer.

    f = (file *) get_file_from_fd(fd);
    if ( (void *) f == NULL ){
        debug_print("io_ioctl: [FAIL] f\n");
        return -1;
    }
    if (f->used != TRUE){
        return -1;
    }
    if ( f->magic != 1234 ){
        return -1;
    }

// Object types.
// #todo:
// What type of file we will support here?

    ObjectType = (int) f->____object;

    if (ObjectType < 0){
        debug_print("io_ioctl: ObjectType\n");
        return -1;
    }

    switch (ObjectType){

    // Pode isso ??
    // Normal file ???
    // See: lib/kstdio.c
    case ObjectTypeFile:
        debug_print ("io_ioctl: ObjectTypeFile [TEST]\n");
        return (int) regularfile_ioctl ( 
                         (int) fd, 
                         (unsigned long) request, 
                         (unsigned long) arg );
        break;

    // tty object
    // See: drivers/tty/tty.c
    case ObjectTypeTTY:
    //case ObjectTypeTerminal: 
        debug_print ("io_ioctl: ObjectTypeTTY\n"); 
        return (int) tty_ioctl ( 
                         (int) fd, 
                         (unsigned long) request, 
                         (unsigned long) arg );
        break;

    // socket object
    // see: net/socket.c ?
    case ObjectTypeSocket:
        debug_print ("io_ioctl: ObjectTypeSocket\n");
        return (int) socket_ioctl ( 
                         (int) fd, 
                         (unsigned long) request, 
                         (unsigned long) arg );
        break;

    // Console object
    // See: user/console.c
    case ObjectTypeVirtualConsole: 
        debug_print ("io_ioctl: ObjectTypeVirtualConsole\n");
        return (int) console_ioctl ( 
                         (int) fd, 
                         (unsigned long) request, 
                         (unsigned long) arg );
        break; 

    // keyboard
    // mouse
    // serial
    // disk
    // ...

    default:
        debug_print ("io_ioctl: [FAIL] default object\n");
        return -1;  //ENOTTY maybe
        break;
    };

    //fail
    debug_print ("io_ioctl: Fail\n");
    return -1;
}







