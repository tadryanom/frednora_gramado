

struct gws_keyboard_event_d
{
    int dummy;
};

struct gws_mouse_event_d
{
    int dummy;
};


struct gws_window_event_d
{
    int dummy;
};


struct gws_event_d
{

    //todo: the client
    // ...client

    //standard msg block
    int wid;    // window id.
    int msg;   // msg. (event type).
    unsigned long long1;
    unsigned long long2;
    
    // extra
    //unsigned long long3;
    //unsigned long long4;
    //unsigned long long5;
    //unsigned long long6;

    
    struct gws_keyboard_event_d  kEvent;
    struct gws_mouse_event_d     mEvent;
    struct gws_window_event_d    wEvent;
    // ...
    
    struct gws_event_d *next;
};


//#test
// Main structure for event.
struct gws_event_d gwsEvent;

//#test
struct gws_event_d *gwsCurrentEvent;


/*
 // uma janela tem o foco de entrada e o cursor esta piscando nela.
struct gws_keyboad_focus_d
{
    struct gws_cliend_d *client;
    struct gws_window_d* window;
    
    //...
};
*/





