//
//  main.cpp
//  USDWalk
//
//  Created by Simo Virokannas on 8/17/18.
//  Copyright © 2018 IHIHI. All rights reserved.
//

#include <pxr/usd/sdf/layer.h>
#include <pxr/usd/usd/stage.h>
#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <pxr/usdImaging/usdImagingGL/gl.h>
#include <math.h>

#define DEGRAD 0.017453292519943

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        printf("USAGE: USDWalk <file.usd>\n");
        exit(1);
    }
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    int w = 640;
    int h = 360;
    SDL_Window *window = SDL_CreateWindow("USD Walk",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          w, h,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
                                          );
    
    pxr::UsdStageRefPtr stage = pxr::UsdStage::Open(argv[1]);
    pxr::SdfLayerRefPtr layer = pxr::SdfLayer::Find(argv[1]);
    
    bool quit = false;
    SDL_GLContext gl_c = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_c);
    pxr::UsdImagingGL *imager = new pxr::UsdImagingGL();
    
    float t = 0.0f;
    
    // eye position
    float x=0.0f,y=150.0f,z=0.0f;
    // eye rotation :rolleyes:
    float r=0.0f;
    
    // rotation / advance speed values
    float rsp=0.0f;
    float asp=0.0f;
    
    // TODO: implement a proper controller scheme
    bool k_u = false, k_d = false, k_l = false, k_r = false;
    
    while(!quit) {
        SDL_GL_MakeCurrent(window, gl_c);
        
        // set a whole bunch of opengl flags old style
        glClearColor(0,0,0.2,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(true);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        // yes these are deprecated but the OpenGL reference manual from 15 years ago has them
        gluPerspective(45.0,16.0/9.0,1.0,10000.0);
        gluLookAt(x, y, z, x - cos(DEGRAD*r), y, z - sin(DEGRAD*r), 0.0, 1.0, 0.0);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // z-up to y-up
        glRotatef(-90.0,1.0,0.0,0.0);
        
        // draw here
        glPushMatrix();
        imager->Render(stage->GetPseudoRoot(),pxr::UsdImagingGLEngine::RenderParams());
        glPopMatrix();
        
        // display
        glFlush();
        SDL_GL_SwapWindow(window);
        
        // scientific method of inducing a framerate
        SDL_Delay(16);
        
        SDL_Event event;
        
        while( SDL_PollEvent(&event) ) {
            if(event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
                bool state = (event.type == SDL_KEYDOWN);
                if (event.key.keysym.sym == SDLK_LEFT) {
                    k_l=state;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    k_r=state;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    k_u=state;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    k_d=state;
                }
            }
        }
        
        // rotate
        if (k_l) {
            rsp -= 0.3f;
        }
        if (k_r) {
            rsp += 0.3f;
        }
        if(rsp > 10.0f) rsp = 10.0f;
        if(rsp < -10.0f) rsp = -10.0f;
        r += rsp;
        rsp = rsp * 0.9f;
        
        // walk
        if (k_u) {
            asp += 0.3f;
        }
        if (k_d) {
            asp -= 0.3f;
        }
        if (asp > 10.0f) asp = 10.0f;
        if (asp < -10.0f) asp = -10.0f;
        asp = asp * 0.9f;
        if (fabs(asp)>0.001f) {
            x -= cos(r * DEGRAD) * asp;
            z -= sin(r * DEGRAD) * asp;
        }
        
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
