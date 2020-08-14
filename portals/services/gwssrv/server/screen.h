
#ifndef ____SCREEN_H
#define ____SCREEN_H    1


struct gws_screen_d
{

    int id;
    int used;
    int magic;
    
    unsigned long width;
    unsigned long height;
    //bpp?

    unsigned long font_size;

    unsigned long char_width; 
    unsigned long char_height;
    
    void *backbuffer;
    void *frontbuffer;

    // Belongs to this display.
    struct gws_display_d *display;
    
    struct gws_screen_d *next;

    struct gws_window_d *root;
};
struct gws_screen_d *CurrentScreen;


#endif    



