
// devmgr.h

#ifndef ____DEVMGR_H
#define ____DEVMGR_H    1

extern unsigned long nicList[8]; 

// unix-like
#define DEVICE_CLASS_CHAR     1
#define DEVICE_CLASS_BLOCK    2
#define DEVICE_CLASS_NETWORK  3

//#test
#define DEVICE_TYPE_PCI  1
#define DEVICE_TYPE_LEGACY  2  //ISA?

// struct more complete, with a lot of information.
struct device_class_d 
{
    //object_type_t  objectType;
    //object_class_t objectClass;
    int device_class;
    int device_subclass;    
};

// Device structure.
struct device_d 
{
    object_type_t  objectType;
    object_class_t objectClass;
    int index;
    int used;
    int magic;

    //name for pci devices: "/DEV_8086_8086"  
    char name[64];
    size_t Name_len;    
    // #todo: merge.

// #todo: good for pci devices?
// struct more complete, with a lot of information.
    struct device_class_d *_class; 
    
//
// Device class
//

// class: char, block, network
    unsigned char __class;
// pci, legacy ...
    unsigned char __type;

//
// == (1) storage ========
//

// object? or buffer?
    file *_fp;
// #importante
// Isso deve ser um pathname 
// do mesmo tipo usado no sistema de arquivos.
// /dev/tty0
    char *mount_point;
// Se o tipo for pci.
    struct pci_device_d *pci_device;

// se o dispositivo for do tipo legado,
// como PIC, PIT, ps2, etc ...
// Qualquer coisa que não esteja na interface pci.
    //struct legacy_device_d *legacy_device;

//#todo: 
//estruturas para outros grupos de dispositivos.

//?? why light - suspenso.
//Se o dispositivo petence ao grupo dos prioritários.
    //int Light;

//Fila de dispositivos que está esperando
//por esse dispositivo.
//Na verdade podemos usar uma lista linkada ou outro recurso.
//de gerenciamento.
    //int pid;
    //unsigned long queue[8];
    //struct process_d *list;

// Driver.
// ?? Talvez pudesse ser 'device driver' e não 'tty driver'
// mas está bom assim.
    struct ttydrv_d *ttydrv;
    
//
// == (1) storage ========
//
    struct tty_d *tty;

//
// == (2) synchronization ========
//
    //int stopped;

//
// == (3) transmition ========
//

// Continua ...

    // maybe not.
    //struct device_d *next;
};

struct device_d  *devices;  //?? What
// struct device_d *CurrentDevice;
//...

// List of legacy devices.
// 'file->device'
struct device_d  *PS2KeyboardDevice;
struct device_d  *PS2MouseDevice;
// ...

// #todo: 
// Parece uma lista muito grande para o número de dispositivos.
// mas se estamos falando de dispositivos PCI a lista é grande mesmo.
// #importante
// O número de dispositivos será o mesmo número de arquivos
// na lista Streams.
// Se o arquivo for um dispositivo então teremos
// um ponteiro na lista deviceList.

//
// The list
//

// NUMBER_OF_FILES
#define DEVICE_LIST_MAX    512

// see: devmgr.c
extern unsigned long deviceList[DEVICE_LIST_MAX];

//
// == Objects ======================
//

// #bugbug
// Temos um header no lugar certo para isso?

file *PS2KeyboardDeviceObject;
file *PS2MouseDeviceObject;
// ...

//
// == ttys ======================
//

// #bugbug
// Temos um header no lugar certo para isso?

struct tty_d *PS2KeyboardDeviceTTY;
struct tty_d *PS2MouseDeviceTTY;
// ...

//
// == tty drivers ======================
//

// #bugbug
// Temos um header no lugar certo para isso?

struct ttydrv_d *PS2KeyboardDeviceTTYDriver;
struct ttydrv_d *PS2MouseDeviceTTYDriver;
// ...

//
// == prototypes ========================================
//

file *devmgr_search_in_dev_list( char *path );

int devmgr_init_device_list(void);
void devmgr_show_device_list(int object_type);
void init_device_manager (void);
struct device_d *devmgr_device_object (void);

int 
devmgr_register_device ( 
    file *f, 
    char *name,
    unsigned char dev_class, 
    unsigned char dev_type,
    struct pci_device_d *pci_device,
    struct tty_d *tty_device );

#endif    



