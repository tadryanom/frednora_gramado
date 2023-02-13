
// comp.h

#ifndef __COMP_H
#define __COMP_H    1

extern int __compose_lock;

void set_refresh_pointer_status(int value);

long comp_get_mouse_x_position(void);
long comp_get_mouse_y_position(void);
void compose(void);
void comp_set_mouse_position(long x, long y);
void comp_initialize_mouse(void);
void mouse_at(void);

#endif    


