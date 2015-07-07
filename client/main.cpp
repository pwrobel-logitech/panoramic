#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
//#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include "glrenderer.h"

//Window initial dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

bool is_fullscreen = false;
//Starts up SDL and creates window
bool init(bool is_fullscreen);

void PrintEvent(const SDL_Event * event);

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;


int screen_counter = 0;
int numdisplays = 0;

int sizeX = 0;
int sizeY = 0;

bool is_spherical_grid = false;

void create_thread();
static int MyThread(void *ptr);
bool is_thread_running = true;
SDL_Thread *thread;
SDL_mutex *mutex;


bool lock_mutex(){
	if (SDL_LockMutex(mutex) == 0) {
		return true;
	} else {
		fprintf(stderr, "Couldn't lock mutex\n");
		return false;
	}
};
bool unlock_mutex(){
	SDL_UnlockMutex(mutex);
	return true;///fix this latex to check if it succeeds
};


bool is_requested_reread_winsize = false;
bool is_fullscreenchange_requested = false;
bool is_displaychange_requested = false;
void request_reread_win_size();
void request_fulscreen_change();
void request_display_change();


//invalidate logics - invalidate = request for a redraw in a separate thread
bool is_screen_valid = true;
void invalidate_screen(){
	if(is_screen_valid){
		lock_mutex();
		is_screen_valid = false;
		unlock_mutex();
	}
}

bool init(bool is_fullscreen)
{
	//Initialization flag
	bool success = true;


	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
        gWindow = SDL_CreateWindow( "GL_RENDERER", SDL_WINDOWPOS_CENTERED_DISPLAY(screen_counter),
                                    SDL_WINDOWPOS_CENTERED_DISPLAY(screen_counter),
                                    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE );



		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//if(is_fullscreen)
				//SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);


			//gl sdl stuff
			printf("started to initialize opengl context \n");
			glrenderer::gContext = SDL_GL_CreateContext( gWindow );
			if( glrenderer::gContext == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{

				SDL_GetWindowSize(gWindow, &sizeX, &sizeY);
				printf("Got window size x: %d, y: %d \n", sizeX, sizeY);
				//Use Vsync
				//if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				//{
				//	printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				//}
				//Initialize OpenGL
		        if( !glrenderer::initGL(sizeX, sizeY) )
		        {
					printf( "Unable to initialize OpenGL!\n" );
		        }

				//render gl
				glrenderer::renderGL(is_spherical_grid, sizeX, sizeY);
			}
		    //

		}
	}



    numdisplays = SDL_GetNumVideoDisplays();
    printf("Numdisplays %d \n", numdisplays);
    if(numdisplays <= 0)
    {
        printf("Error in the number of the displays \n");
        exit(0);
    }


	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}


int draw_frame(){
	lock_mutex();
	SDL_GetWindowSize(gWindow, &sizeX, &sizeY);
	glrenderer::set_viewport_size(sizeX, sizeY);
	glrenderer::renderGL(is_spherical_grid, sizeX, sizeY);
  SDL_GL_SwapWindow( gWindow );
	unlock_mutex();
	return 0;
}

SDL_Event event;

int main( int argc, char* args[] )
{

	create_thread();


	lock_mutex();
unlock_mutex();



	//Start up SDL and create window
	
	//else
	{
		//Load media
		//if( !loadMedia() )
		//{
		//	printf( "Failed to load media!\n" );
		//}
		//else
		{	
			
			//draw_frame();
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					PrintEvent(&e);
		switch (e.type)
        {
			case SDL_WINDOWEVENT:
				printf("Window event \n");
				invalidate_screen();
				//if( !glrenderer::initGL(sizeX, sizeY) )
		       // {
					//printf( "Unable to initialize OpenGL!\n" );
		       // }
				//draw_frame();
				if(e.window.event == SDL_WINDOWEVENT_EXPOSED){
					if(is_fullscreen){

					}
				}
				break;
            case SDL_MOUSEMOTION:
            break;

            case SDL_QUIT:
				quit = true;
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    quit = true; //quit
                if(e.key.keysym.sym == SDLK_f)
                {
                    //close();
                    //is_fullscreen = !is_fullscreen;
                    //init(is_fullscreen);
										//draw_frame();
										request_fulscreen_change();
                }
                if(e.key.keysym.sym == SDLK_d)
                {
										request_display_change();
                    //close();
                    //printf("Display counter %d, numdisplays %d \n", screen_counter, numdisplays);
                    //screen_counter++;
                    //if (screen_counter >= numdisplays)
                        //screen_counter = 0;
                    //init(is_fullscreen);
					//draw_frame();
                }
				if(e.key.keysym.sym == SDLK_s)
                {
                    printf("Grid toggled \n");
					is_spherical_grid = !is_spherical_grid;
					invalidate_screen();
                }
                break;
        }	
				}


			}
		}
	}





	is_thread_running = false;
	int threadReturnValue;
	SDL_WaitThread(thread, &threadReturnValue);
	printf("\nThread returned value: %d", threadReturnValue);
	SDL_DestroyMutex(mutex);
	//Free resources and close SDL
	close();

	return 0;
}




void create_thread(){

	mutex = SDL_CreateMutex();
	if (!mutex) {
	  printf( "Couldn't create mutex\n");
	  return;
	}
	int threadReturnValue;


	

	printf("\nSimple SDL_CreateThread test:");

	// Simply create a thread
	thread = SDL_CreateThread(MyThread, "MyThread", (void *)NULL);
	if (NULL == thread) {
		printf("\nSDL_CreateThread failed: %s\n", SDL_GetError());
	}
}


int MyThread(void *ptr)
{
    int cnt = 0;

	lock_mutex();
		if( !init(false) )
		{
			printf( "Failed to initialize!\n" );
		}
		unlock_mutex();

	

draw_frame();

    while(is_thread_running) {

lock_mutex();


if(is_requested_reread_winsize){
	SDL_GetWindowSize(gWindow, &sizeX, &sizeY);
	if( !glrenderer::initGL(sizeX, sizeY) )
    {
		printf( "Unable to initialize OpenGL!\n" );
    }
						draw_frame();
}


if(is_fullscreenchange_requested){

                    is_fullscreen = !is_fullscreen;

					if(is_fullscreen){
						SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);}
					else{SDL_SetWindowFullscreen(gWindow, 0);}
					SDL_GetWindowSize(gWindow, &sizeX, &sizeY);
					printf("Got window size x: %d, y: %d \n", sizeX, sizeY);

										draw_frame();
	is_fullscreenchange_requested = false;
}
unlock_mutex();

lock_mutex();
if(is_displaychange_requested){
 numdisplays = SDL_GetNumVideoDisplays();
	screen_counter++;
    if (screen_counter >= numdisplays)
        screen_counter = 0;



 //close();
                    //printf("Display counter %d, numdisplays %d \n", screen_counter, numdisplays);
                    //screen_counter++;
                    //if (screen_counter >= numdisplays)
                        //screen_counter = 0;
                    //init(is_fullscreen);
					//draw_frame();
	is_displaychange_requested = false;
}
unlock_mutex();

        printf("\nThread counter: %d", cnt);
        SDL_Delay(50);
		if (!is_screen_valid){
			draw_frame();
			is_screen_valid = true;
		}
		cnt++;
    }

    return cnt;
}


void request_reread_win_size(){
	lock_mutex();
	is_requested_reread_winsize = true;
	unlock_mutex();
}

void request_fulscreen_change(){
	lock_mutex();
	is_fullscreenchange_requested = true;
	unlock_mutex();
}


void request_display_change(){
	lock_mutex();
	is_displaychange_requested = true;
	unlock_mutex();
}

void PrintEvent(const SDL_Event * event)
{
    if (event->type == SDL_WINDOWEVENT) {
        switch (event->window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            printf("Window %d shown \n", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            printf("Window %d hidden \n", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            printf("Window %d exposed \n", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MOVED:
            printf("Window %d moved to %d,%d \n",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            printf("Window %d resized to %dx%d \n",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            printf("Window %d minimized \n", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            printf("Window %d maximized \n", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_RESTORED:
            printf("Window %d restored \n", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_ENTER:
            printf("Mouse entered window %d \n",
                    event->window.windowID);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            printf("Mouse left window %d \n", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            printf("Window %d gained keyboard focus \n",
                    event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            printf("Window %d lost keyboard focus \n",
                    event->window.windowID);
            break;
        case SDL_WINDOWEVENT_CLOSE:
            printf("Window %d closed \n", event->window.windowID);
            break;
        default:
            printf("Window %d got unknown event %d \n",
                    event->window.windowID, event->window.event);
            break;
        }
    }
}
