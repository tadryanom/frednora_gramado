
// demo4.c
// Using fpu and eng4.
// Credits:
// OneLoneCoder.com - 3D Graphics Part #1 - Triangles & Projections
// GNU GPLv3

#include "../../eng4/gws.h"

char *args[] = { "nothing", NULL };


char obj_buffer[1024];


//struct mesh meshCube;
struct gr_mat4x4_d matProj;
float fTheta;
float model_distance;

/*
struct vec3d
{
    float x, y, z;
};
*/

/*
struct triangle
{
    struct gr_vecF3D_d p[3];
};
*/

/*
struct mesh
{
    //int dummy;
    //vector<triangle> tris;

    struct gr_triangleF3D_d tris[12];
};
*/


/*
void 
MultiplyMatrixVector(
    struct gr_vecF3D_d *i, 
    struct gr_vecF3D_d *o, 
    struct gr_mat4x4_d *m )
{
    o->x = 
        i->x * m->m[0][0] + 
        i->y * m->m[1][0] + 
        i->z * m->m[2][0] + 
        m->m[3][0];

    o->y = 
        i->x * m->m[0][1] + 
        i->y * m->m[1][1] + 
        i->z * m->m[2][1] + 
        m->m[3][1];
    
    o->z = 
        i->x * m->m[0][2] + 
        i->y * m->m[1][2] + 
        i->z * m->m[2][2] + 
        m->m[3][2];

    float w = 
        i->x * m->m[0][3] + 
        i->y * m->m[1][3] + 
        i->z * m->m[2][3] + 
        m->m[3][3];

    if (w != 0.0f)
    {
        o->x /= w; 
        o->y /= w; 
        o->z /= w;
    }
}
*/

int OnUserCreate(void)
{

/*
    //meshCube.tris = {

		// SOUTH
		meshCube.tris[0].p[0].x = 0.0f;
		meshCube.tris[0].p[0].y = 0.0f;
		meshCube.tris[0].p[0].z = 0.0f,    
		meshCube.tris[0].p[1].x = 0.0f; 
		meshCube.tris[0].p[1].y = 1.0f; 
		meshCube.tris[0].p[1].z = 0.0f;    
		meshCube.tris[0].p[2].x = 1.0f, 
		meshCube.tris[0].p[2].x = 1.0f, 
		meshCube.tris[0].p[2].x = 0.0f },

		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};
*/

// Projection Matrix

    float fNear = 0.1f;
    float fFar  = 1000.0f;

    float fFov = 90.0f;

    float fAspectRatio = (float) 800 / (float) 600;
    //float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
 
    float fFovRad = 
        1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
    //float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

    return 0;
}


// fake Wavefront File Format (.obj)
int DrawModel1(float fElapsedTime)
{
    //demoClearSurface(NULL,COLOR_BLACK);

    struct gr_mat4x4_d matRotZ; 
    struct gr_mat4x4_d matRotX;
    fTheta += 1.0f * fElapsedTime;
// Rotation Z
	matRotZ.m[0][0] =  cosf(fTheta);
	matRotZ.m[0][1] =  sinf(fTheta);
	matRotZ.m[1][0] = -sinf(fTheta);
	matRotZ.m[1][1] =  cosf(fTheta);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;
// Rotation X
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] =  cosf(fTheta * 0.5f);
	matRotX.m[1][2] =  sinf(fTheta * 0.5f);
	matRotX.m[2][1] = -sinf(fTheta * 0.5f);
	matRotX.m[2][2] =  cosf(fTheta * 0.5f);
	matRotX.m[3][3] = 1;
//------------------------------------------------

// triangles
    struct gr_triangleF3D_d tri;            // triângulo original.
    struct gr_triangleF3D_d triRotatedZ; 
    struct gr_triangleF3D_d triRotatedZX;
    struct gr_triangleF3D_d triProjected;   // triangulo final, mostrado na tela.
// vectors
    struct gr_vecF3D_d vecs[32];


// #bugbug
// Isso foi projetado para estar dentro 
// de um model space. Se usarmos num view space
// que é o que a camera vê, então teríamos valores negativos,
// que não deviram aparecer na imagem final.

    // v -0.5  -0.5  0.5
    vecs[1].x = -0.2;  vecs[1].y = -0.2;  vecs[1].z =  0.2;
    // v 0.5  -0.5  0.5
    vecs[2].x =  0.2;  vecs[2].y = -0.2;  vecs[2].z =  0.2;
    //v -0.5  0.5  0.5
    vecs[3].x = -0.2;  vecs[3].y =  0.2;  vecs[3].z =  0.2;
    // v 0.5  0.5  0.5
    vecs[4].x =  0.2;  vecs[4].y =  0.2;  vecs[4].z =  0.2;
    //v -0.5  0.5  -0.5
    vecs[5].x = -0.2;  vecs[5].y =  0.2;  vecs[5].z = -0.2;
    // v 0.5  0.5  -0.5
    vecs[6].x =  0.2;  vecs[6].y =  0.2;  vecs[6].z = -0.2;
    // v -0.5  -0.5  -0.5
    vecs[7].x = -0.2;  vecs[7].y = -0.2;  vecs[7].z = -0.2;
    // v 0.5  -0.5  -0.5
    vecs[8].x =  0.2;  vecs[8].y = -0.2;  vecs[8].z = -0.2;


    int sequence[3*16];
    
    sequence[0]  = 1; sequence[1]  = 2;  sequence[2] = 3; //f 1 2 4
    sequence[3]  = 1; sequence[4]  = 4;  sequence[5] = 3; //f 1 4 3
    sequence[6]  = 3; sequence[7]  = 4;  sequence[8] = 6; //f 3 4 6
    sequence[9]  = 3; sequence[10] = 6; sequence[11] = 5; //f 3 6 5
    sequence[12] = 5; sequence[13] = 6; sequence[14] = 8; //f 5 6 8
    sequence[15] = 5; sequence[16] = 8; sequence[17] = 7; //f 5 8 7
    sequence[18] = 7; sequence[19] = 8; sequence[20] = 2; //f 7 8 2
    sequence[21] = 7; sequence[22] = 2; sequence[23] = 1; //f 7 2 1
    sequence[24] = 2; sequence[25] = 8; sequence[26] = 6; //f 2 8 6
    sequence[27] = 2; sequence[28] = 6; sequence[29] = 4; //f 2 6 4
    sequence[30] = 7; sequence[31] = 1; sequence[32] = 3; //f 7 1 3
    sequence[33] = 7; sequence[34] = 3; sequence[35] = 5; //f 7 3 5
    
    int i=0;
    int j=0;
    int off=0;
    int v=0;
    //for(i=1; i<=8; i++)
    for(i=1; i<=12; i++)
    {
        off = (i-1)*3;
        
        v = sequence[off+0];
        tri.p[0].x = vecs[v].x;
        tri.p[0].y = vecs[v].y;
        tri.p[0].z = vecs[v].z;
        tri.p[0].color = COLOR_WHITE;  //COLOR_RED; 
        
        v = sequence[off+1];
        tri.p[1].x = vecs[v].x;
        tri.p[1].y = vecs[v].y;
        tri.p[1].z = vecs[v].z;
        tri.p[1].color = COLOR_WHITE;  // COLOR_GREEN;

        v = sequence[off+2];
        tri.p[2].x = vecs[v].x;
        tri.p[2].y = vecs[v].y;
        tri.p[2].z = vecs[v].z;
        tri.p[2].color = COLOR_WHITE;  // COLOR_BLUE;
        
        //-----------------------------    
        // Rotate in Z-Axis
        gr_MultiplyMatrixVector(&tri.p[0], &triRotatedZ.p[0], &matRotZ);
        gr_MultiplyMatrixVector(&tri.p[1], &triRotatedZ.p[1], &matRotZ);
        gr_MultiplyMatrixVector(&tri.p[2], &triRotatedZ.p[2], &matRotZ);
        //-----------------------------    
        // Rotate in X-Axis
        gr_MultiplyMatrixVector(&triRotatedZ.p[0], &triRotatedZX.p[0], &matRotX);
        gr_MultiplyMatrixVector(&triRotatedZ.p[1], &triRotatedZX.p[1], &matRotX);
        gr_MultiplyMatrixVector(&triRotatedZ.p[2], &triRotatedZX.p[2], &matRotX);

        triRotatedZX.p[0].color = tri.p[0].color;
        triRotatedZX.p[1].color = tri.p[1].color;
        triRotatedZX.p[2].color = tri.p[2].color;


        // Translate in z
        model_distance = (float) (model_distance + 0.002f);
        if(model_distance>10.0f)
            model_distance = 0.8f;
        triRotatedZX.p[0].z = triRotatedZX.p[0].z + model_distance; 
        triRotatedZX.p[1].z = triRotatedZX.p[1].z + model_distance; 
        triRotatedZX.p[2].z = triRotatedZX.p[2].z + model_distance; 

        //We need a valid window, to use the rasterization features.
        if( (void*) __root_window != NULL ){
            plotTriangleF(__root_window, &triRotatedZX,FALSE);
        }
    };

    //demoFlushSurface(NULL);
    //rtl_yield();
    
    return 0;
}

// fake Wavefront File Format (.obj)
int DrawModel2(float fElapsedTime)
{
    //demoClearSurface(NULL,COLOR_BLACK);

    struct gr_mat4x4_d matRotZ; 
    struct gr_mat4x4_d matRotX;
    fTheta += 1.0f * fElapsedTime;
// Rotation Z
	matRotZ.m[0][0] =  cosf(fTheta);
	matRotZ.m[0][1] =  sinf(fTheta);
	matRotZ.m[1][0] = -sinf(fTheta);
	matRotZ.m[1][1] =  cosf(fTheta);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;
// Rotation X
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] =  cosf(fTheta * 0.5f);
	matRotX.m[1][2] =  sinf(fTheta * 0.5f);
	matRotX.m[2][1] = -sinf(fTheta * 0.5f);
	matRotX.m[2][2] =  cosf(fTheta * 0.5f);
	matRotX.m[3][3] = 1;
//------------------------------------------------

// triangles
    struct gr_triangleF3D_d tri;            // triângulo original.
    struct gr_triangleF3D_d triRotatedZ; 
    struct gr_triangleF3D_d triRotatedZX;
    struct gr_triangleF3D_d triProjected;   // triangulo final, mostrado na tela.
// vectors
    struct gr_vecF3D_d vecs[32];


// #bugbug
// Isso foi projetado para estar dentro 
// de um model space. Se usarmos num view space
// que é o que a camera vê, então teríamos valores negativos,
// que não deviram aparecer na imagem final.

    // v -0.5  -0.5  0.5
    vecs[1].x = -0.2;  vecs[1].y = -0.2;  vecs[1].z =  0.2;
    // v 0.5  -0.5  0.5
    vecs[2].x =  0.2;  vecs[2].y = -0.2;  vecs[2].z =  0.2;
    //v -0.5  0.5  0.5
    vecs[3].x = -0.2;  vecs[3].y =  0.2;  vecs[3].z =  0.2;
    // v 0.5  0.5  0.5
    vecs[4].x =  0.2;  vecs[4].y =  0.2;  vecs[4].z =  0.2;
    //v -0.5  0.5  -0.5
    vecs[5].x = -0.2;  vecs[5].y =  0.2;  vecs[5].z = -0.2;
    // v 0.5  0.5  -0.5
    vecs[6].x =  0.2;  vecs[6].y =  0.2;  vecs[6].z = -0.2;
    // v -0.5  -0.5  -0.5
    vecs[7].x = -0.2;  vecs[7].y = -0.2;  vecs[7].z = -0.2;
    // v 0.5  -0.5  -0.5
    vecs[8].x =  0.2;  vecs[8].y = -0.2;  vecs[8].z = -0.2;


    int sequence[3*16];
    
    sequence[0]  = 1; sequence[1]  = 2;  sequence[2] = 3; //f 1 2 4
    sequence[3]  = 1; sequence[4]  = 4;  sequence[5] = 3; //f 1 4 3
    sequence[6]  = 3; sequence[7]  = 4;  sequence[8] = 6; //f 3 4 6
    sequence[9]  = 3; sequence[10] = 6; sequence[11] = 5; //f 3 6 5
    sequence[12] = 5; sequence[13] = 6; sequence[14] = 8; //f 5 6 8
    sequence[15] = 5; sequence[16] = 8; sequence[17] = 7; //f 5 8 7
    sequence[18] = 7; sequence[19] = 8; sequence[20] = 2; //f 7 8 2
    sequence[21] = 7; sequence[22] = 2; sequence[23] = 1; //f 7 2 1
    sequence[24] = 2; sequence[25] = 8; sequence[26] = 6; //f 2 8 6
    sequence[27] = 2; sequence[28] = 6; sequence[29] = 4; //f 2 6 4
    sequence[30] = 7; sequence[31] = 1; sequence[32] = 3; //f 7 1 3
    sequence[33] = 7; sequence[34] = 3; sequence[35] = 5; //f 7 3 5
    
    int i=0;
    int j=0;
    int off=0;
    int v=0;
    //for(i=1; i<=8; i++)
    for(i=1; i<=12; i++)
    {
        off = (i-1)*3;
        
        v = sequence[off+0];
        tri.p[0].x = vecs[v].x;
        tri.p[0].y = vecs[v].y;
        tri.p[0].z = vecs[v].z;
        tri.p[0].color = COLOR_WHITE;  //COLOR_RED; 
        
        v = sequence[off+1];
        tri.p[1].x = vecs[v].x;
        tri.p[1].y = vecs[v].y;
        tri.p[1].z = vecs[v].z;
        tri.p[1].color = COLOR_WHITE;  // COLOR_GREEN;

        v = sequence[off+2];
        tri.p[2].x = vecs[v].x;
        tri.p[2].y = vecs[v].y;
        tri.p[2].z = vecs[v].z;
        tri.p[2].color = COLOR_WHITE;  // COLOR_BLUE;
        
        //-----------------------------    
        // Rotate in Z-Axis
        gr_MultiplyMatrixVector(&tri.p[0], &triRotatedZ.p[0], &matRotZ);
        gr_MultiplyMatrixVector(&tri.p[1], &triRotatedZ.p[1], &matRotZ);
        gr_MultiplyMatrixVector(&tri.p[2], &triRotatedZ.p[2], &matRotZ);
        //-----------------------------    
        // Rotate in X-Axis
        gr_MultiplyMatrixVector(&triRotatedZ.p[0], &triRotatedZX.p[0], &matRotX);
        gr_MultiplyMatrixVector(&triRotatedZ.p[1], &triRotatedZX.p[1], &matRotX);
        gr_MultiplyMatrixVector(&triRotatedZ.p[2], &triRotatedZX.p[2], &matRotX);

        triRotatedZX.p[0].color = tri.p[0].color;
        triRotatedZX.p[1].color = tri.p[1].color;
        triRotatedZX.p[2].color = tri.p[2].color;


        // Translate in z
        model_distance = (float) (model_distance + 0.002f);
        if(model_distance>10.0f)
            model_distance = 0.8f;
        triRotatedZX.p[0].z = triRotatedZX.p[0].z + model_distance; 
        triRotatedZX.p[1].z = triRotatedZX.p[1].z + model_distance; 
        triRotatedZX.p[2].z = triRotatedZX.p[2].z + model_distance; 

        //translate in x
        triRotatedZX.p[0].x = triRotatedZX.p[0].x + 0.8f; 
        triRotatedZX.p[1].x = triRotatedZX.p[1].x + 0.8f; 
        triRotatedZX.p[2].x = triRotatedZX.p[2].x + 0.8f; 


        //We need a valid window, to use the rasterization features.
        if( (void*) __root_window != NULL ){
            plotTriangleF(__root_window, &triRotatedZX,FALSE);
        }
    };

    //demoFlushSurface(NULL);
    //rtl_yield();
    
    return 0;
}




// include inside the engine.
void gr_setup(void)
{
    // #test
    //printf("gr_setup:\n");
    //__setupCatModel(TRUE,TRUE,TRUE);
    
    OnUserCreate();
}


// include inside the engine.
void gr_draw(void)
{
    demoClearSurface(NULL,COLOR_BLACK);
    
    //#tests
    //printf("gr_draw:\n");
    //demoCat();
    //OnUserUpdate(1);
    //OnUserUpdate2(1);

//#draw model 1
    DrawModel1(0.02f);
//#draw model 2
    DrawModel2(0.02f);

    demoFlushSurface(NULL);
}


int main(int argc, char **argv)
{
    // chama o main() da engine
    
    printf("DEMO4.BIN: Initializing ...\n");

/*
    FILE *fp;
    fp = (FILE *) fopen("test1.obj", "a+");
    int nreads=-1;
    nreads = read( fp->_file, obj_buffer, 511 );
    printf("%s\n",obj_buffer);
    while(1){}
*/

    eng4_main(1,args);
    
    return 0;
}


