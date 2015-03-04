#include "mallard.hpp"
#include "duck.hpp"

/* Screen resolution */
const int Mallard::SCREEN_WIDTH = 640;
const int Mallard::SCREEN_HEIGHT = 480;

Mallard::Mallard(int argc, char* argv[]) {
    exit = false;
    
    SDL_Init(SDL_INIT_EVERYTHING); // Initialize SDL2
    



    // Create an application window with the following settings:
    this->window = SDL_CreateWindow( "MALLARD KOMBAT",   // window title
                              SDL_WINDOWPOS_UNDEFINED,   // initial x position
                              SDL_WINDOWPOS_UNDEFINED,   // initial y position
                              SCREEN_WIDTH,              // width,  in pixels
                              SCREEN_HEIGHT,             // height, in pixels
                              SDL_WINDOW_SHOWN);         // flags - see below

    // Check that the window was successfully made
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED |
                                 SDL_RENDERER_PRESENTVSYNC );
    
    // Sounds
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    quack = Mix_LoadWAV("resources/sounds/quack.wav");
    path = "resources/images/";
    //TS stands for TitleScreens
    std::string TS[5] = {
    "title_screen",
    "title_screen_start",
    "title_screen_options",
    "title_screen_credits",
    "title_screen_quit",
    };
    // Creating the title screens
    
    //TSS stands for TitleScreenSurfaces
    
    for (int i=0; i < 5; i++) {
        std::string filepath = path + TS[i] + ".bmp";
        char *temp = (char*)filepath.c_str();
        /*
         the following arrays all hold various states of the 
         surfaces and textures rendering process
         */
        TSS[i] = SDL_LoadBMP(temp);
        CTSS[i] = SDL_ConvertSurfaceFormat(TSS[i], SDL_PIXELFORMAT_RGBA8888, 0);
        TST[i] = SDL_CreateTextureFromSurface(renderer, CTSS[i]);
        SDL_FreeSurface(CTSS[i]);
    }

    std::string DS[3] = {
    "single_duck",
    "double_duck",
    "triple_duck",
    };

    for (int i=0; i < 3; i++) {
        std::string filepath = path + DS[i] + ".bmp";
        char *temp = (char*)filepath.c_str();
        /*
         the following arrays all hold various states of the 
         surfaces and textures rendering process
         */
        DSS[i] = SDL_LoadBMP(temp);
        CDSS[i] = SDL_ConvertSurfaceFormat(DSS[i], SDL_PIXELFORMAT_RGBA8888, 0);
        DST[i] = SDL_CreateTextureFromSurface(renderer, CDSS[i]);
        SDL_FreeSurface(CDSS[i]);
    }
    
    first_stage_surface = SDL_LoadBMP("resources/images/stage.bmp");
    first_stage_surface = SDL_ConvertSurfaceFormat(first_stage_surface, SDL_PIXELFORMAT_RGBA8888, 0);
    first_stage_texture = SDL_CreateTextureFromSurface(renderer, first_stage_surface);
    
    footballSurface = SDL_LoadBMP("resources/images/football.bmp");
    footballSurface = SDL_ConvertSurfaceFormat(footballSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    footballTexture = SDL_CreateTextureFromSurface(renderer, footballSurface);
    footballVisible = false;
    
    duckScalar.x = 0;
    duckScalar.y = 350;
    jumping = false;
    yspeed = 0;
    count = 0;
   
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, TST[0], NULL, NULL); // base title screen
    SDL_RenderPresent(renderer);
    title_visible = true;
}

void Mallard::getBools(int x, int y){
    on_start = (450 < x && x < 550) && (275 < y && y < 300);
    on_options = (435 < x && x < 565) && (320 < y && y < 345);
    on_credits = (435 < x && x < 565) && (365 < y && y < 385);
    on_quit = (465 < x && x < 535) && (410 < y && y < 430);
}

void Mallard::input(){
    int x, y;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEMOTION){
            SDL_GetMouseState(&x, &y);
            getBools(x,y);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&x, &y);
            getBools(x,y);
            if (on_start && title_visible) {
                title_visible = false;
                SDL_RenderClear(renderer);
                Mix_PlayChannel(-1, quack, 0);
                first_stage_visible = true;
            }
            if (on_quit && title_visible) {
                exit = true;
            }
        }
        if (event.type == SDL_QUIT) {
            exit = true;
        }
        if (event.type == SDL_KEYDOWN && first_stage_visible) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (!jumping){
                        yspeed = 19;
                        jumping = true;
                    }
                    break;
                case SDLK_LEFT:
                    if (xspeed > -10) {
                        xspeed-=3;
                    }else{
                        xspeed = -10;
                    }
                    break;
                case SDLK_RIGHT:
                    if (xspeed < 10) {
                        xspeed+=3;
                    }else{
                        xspeed = 10;
                    }
                    break;

                case SDLK_DOWN:
                    duckScalar.y += 10;
                    break;
                case SDLK_SPACE:
                    shootFootball();
                default:
                    break;
            }
        }

    }
    if(jumping){
            if (duckScalar.y <= 350){
                duckScalar.y -= yspeed;
                yspeed--;
            }
            if (duckScalar.y > 350){
                duckScalar.y = 350;
                jumping = false;
            }

            if (duckScalar.y == 0){
                jumping = false;
            }
        }

}


void Mallard::update(){
    if (duckScalar.x > 800) {
        duckScalar.x = -200;
    }
    duckScalar.x += xspeed;
    if (duckScalar.y == 350) {
        //xspeed = xspeed * 0.9;
    }
   
}

void Mallard::render_title_screen(){
    if (on_start) {
        SDL_RenderCopy(renderer, TST[1], NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    else if (on_options) {
        SDL_RenderCopy(renderer, TST[2], NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    else if (on_credits) {
        SDL_RenderCopy(renderer, TST[3], NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    else if (on_quit) {
        SDL_RenderCopy(renderer, TST[4], NULL, NULL);
        SDL_RenderPresent(renderer);
    }else{
        SDL_RenderCopy(renderer, TST[0], NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}

void Mallard::jump(){
    int speed = 20;
    Mix_PlayChannel(-1, quack, 0);
    duckScalar.y -= speed;
    speed--;
    
    for (int i = 0; i < 20; i++){
        duckScalar.y += speed;
        speed++;
    } 
}

void Mallard::shootFootball(){
    // initial position of the football
    // near the duck's mouth
    footballScalar.x = duckScalar.x+90;
    footballScalar.y = duckScalar.y-30;
    footballScalar.w = 100;
    footballScalar.h = 100;
    footballVisible = true;
}

void Mallard::render_first_stage(){
    
    int scaling_factor = 5;
    //width and height get scaled by scaling_factor
    duckScalar.w = 34*scaling_factor;
    duckScalar.h = 24*scaling_factor;

    
    SDL_RenderCopy(renderer, first_stage_texture, NULL, NULL);

    count++;
    if (footballVisible) {
        footballScalar.x +=10;
        SDL_RenderCopy(renderer, footballTexture, NULL, &footballScalar);
        SDL_RenderCopy(renderer, DST[1], NULL, &duckScalar);
        if (footballScalar.x > 640){
            footballVisible = false;
        }
    }
    
    else if (count <= 20){
        SDL_RenderCopy(renderer, DST[2], NULL, &duckScalar);
    }
    else if (count < 40){
        SDL_RenderCopy(renderer, DST[0], NULL, &duckScalar);
    }
    if (count == 40){
        count = 0;
    }

    
    SDL_RenderPresent(renderer);
}

void Mallard::render(){
    if (title_visible) {
        render_title_screen();
    }
    if (first_stage_visible) {
        render_first_stage();
    }
}

void Mallard::execute() {
    
    while(!exit) {
        input();
        update();
        render();
        SDL_Delay(10);
    }
    std::cout << "preparing to Quit" << std::endl;
    clean_up();
    std::cout << "successfully Quit" << std::endl;
}

void Mallard::clean_up(){
    for (int i=0; i < 5; i++) {
        SDL_DestroyTexture(TST[i]);
    }
    SDL_DestroyTexture(duckTexture);
    SDL_DestroyTexture(first_stage_texture);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
