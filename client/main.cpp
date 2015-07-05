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
const int SCREEN_HEIGHT = 480;

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

//The window renderer
SDL_Renderer* gRenderer = NULL;

int screen_counter = 0;
int numdisplays = 0;

int sizeX = 0;
int sizeY = 0;

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
                                    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			if(is_fullscreen)
				SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);


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
				glrenderer::renderGL();
			}
		    //

			//Create renderer for window
			/*gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			*/

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
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


int draw_frame(){
	glrenderer::renderGL();
	//Update screen
    SDL_GL_SwapWindow( gWindow );
	return 0;
}

SDL_Event event;

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init(false) )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			draw_frame();
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

		switch (e.type)
        {
			PrintEvent(&e);
			case SDL_WINDOWEVENT:
				printf("Window event \n");
				if( !glrenderer::initGL(sizeX, sizeY) )
		        {
					printf( "Unable to initialize OpenGL!\n" );
		        }
				draw_frame();
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
                    close();
                    is_fullscreen = !is_fullscreen;
                    init(is_fullscreen);
					draw_frame();
                }
                if(e.key.keysym.sym == SDLK_d)
                {
                    close();
                    printf("Display counter %d, numdisplays %d \n", screen_counter, numdisplays);
                    screen_counter++;
                    if (screen_counter >= numdisplays)
                        screen_counter = 0;
                    init(is_fullscreen);
					draw_frame();
                }
                break;
        }	
				}


			}
		}
	}






	//Free resources and close SDL
	close();

	return 0;
}




void PrintEvent(const SDL_Event * event)
{
    if (event->type == SDL_WINDOWEVENT) {
        switch (event->window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            SDL_Log("Window %d shown", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            SDL_Log("Window %d hidden", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_Log("Window %d exposed", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MOVED:
            SDL_Log("Window %d moved to %d,%d",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            SDL_Log("Window %d resized to %dx%d",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            SDL_Log("Window %d minimized", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            SDL_Log("Window %d maximized", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_RESTORED:
            SDL_Log("Window %d restored", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_ENTER:
            SDL_Log("Mouse entered window %d",
                    event->window.windowID);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            SDL_Log("Mouse left window %d", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            SDL_Log("Window %d gained keyboard focus",
                    event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            SDL_Log("Window %d lost keyboard focus",
                    event->window.windowID);
            break;
        case SDL_WINDOWEVENT_CLOSE:
            SDL_Log("Window %d closed", event->window.windowID);
            break;
        default:
            SDL_Log("Window %d got unknown event %d",
                    event->window.windowID, event->window.event);
            break;
        }
    }
}
