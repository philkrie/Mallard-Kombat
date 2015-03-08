#include "mallard.hpp"
#include "duck.hpp"
#include "util.hpp"
#include <fstream>
#include <stdlib.h>

/* Screen resolution */
int Mallard::SCREEN_WIDTH = 640;
int Mallard::SCREEN_HEIGHT = 480;
int Mallard::xcor = SCREEN_WIDTH / 640;
int Mallard::ycor = SCREEN_HEIGHT/ 480;


Mallard::Mallard(int argc, char* argv[]) {
    std::ofstream myfile;
    myfile.open("resources/text/hiscores.txt", std::ios::app);
    myfile << "holy balls\n";
    myfile.close();
    
    exit = false;
    
    SDL_Init(SDL_INIT_EVERYTHING); // Initialize SDL2
    
    
    
    // Create an application window with the following settings:
    this->window = SDL_CreateWindow( "MALLARD KOMBAT",         // window title
                                    SDL_WINDOWPOS_UNDEFINED,   // initial x position
                                    SDL_WINDOWPOS_UNDEFINED,   // initial y position
                                    SCREEN_WIDTH,              // width,  in pixels
                                    SCREEN_HEIGHT,             // height, in pixels
                                    SDL_WINDOW_SHOWN);         // flags - see below
    
    // Check that the window was successfully made
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC );
    
    duck = new Duck();
    
    TTF_Init();
    font_color = {0, 0, 0, 0};
    font_name = "resources/fonts/comic_sans.ttf";
    
    swag = renderText("SWAG", font_name, font_color, 72, renderer);
    swagRect.x = 100 * xcor;
    swagRect.y = 100 * ycor;
    swagRect.w = 50 * xcor;
    swagRect.h = 50 * ycor;
    score = 0;
    
    
    
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
    srand(time(NULL));
    for (int i=0; i < 5; i++) {
        beaverArray[i] = NULL;
        std::string filepath = path + TS[i] + ".bmp";
        char *temp = (char*)filepath.c_str();
        TST[i] = createTexture(temp, renderer);
    }

    
    std::string DS[4] = {
        "single_duck",
        "double_duck",
        "triple_duck",
        "quadruple_duck",
    };
    
    for (int i=0; i < 4; i++) {
        std::string filepath = path + DS[i] + ".bmp";
        char *temp = (char*)filepath.c_str();
        duck->DST[i] = createTexture(temp, renderer);
    }
    //Loads individual image as texture
    SDL_Texture* loadTexture( std::string path );
    
    
    first_stage_texture = createTexture("resources/images/field3.jpg", renderer);
    
    duck->footballTexture = createTexture("resources/images/football.bmp", renderer);
    duck->footballVisible = false;
    
    duck->isDead = false;
    count = 0;
    spawnCount = 0;
    
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, TST[0], NULL, NULL); // base title screen
    SDL_RenderPresent(renderer);
    title_visible = true;
    first_stage_visible = false;
    paused = false;
    gameBreaker = false;
}

void Mallard::getBools(int x, int y){
    on_start =   (450 * xcor < x && x < 550 * xcor ) & (275 * ycor < y && y < 300 * ycor);
    on_options = (435 * xcor < x && x < 565 * xcor ) & (320 * ycor < y && y < 345 * ycor);
    on_credits = (435 * xcor < x && x < 565 * xcor ) & (365 * ycor < y && y < 385 * ycor);
    on_quit =    (465 * xcor < x && x < 535 * xcor ) & (410 * ycor < y && y < 430 * ycor);
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
        if (event.type == SDL_MOUSEMOTION && !duck->isDead){
            SDL_GetMouseState(NULL, &duck->duckScalar.y);
            if (duck->duckScalar.y < 0) {
                duck->duckScalar.y = 0;
            }
            if (duck->duckScalar.y > 400 * ycor) {
                duck->duckScalar.y = 400 * ycor;
            }
        }
        
        if (event.type == SDL_KEYDOWN && first_stage_visible && !duck->isDead) {
            
            switch (event.key.keysym.sym) {
                case SDLK_p:
                    paused = true;
                    break;
                case SDLK_SPACE:
                    if (!duck->footballVisible) {
                        Mix_PlayChannel(-1, quack, 0);
                        duck->shootFootball();
                    }
                    break;
                default:
                    break;
            }
        }
    }
}



void Mallard::update(){
    while (paused) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_p) {
                    paused = false;
                }
            }
        }
    }

        if (first_stage_visible && !duck->isDead) {
            swagRect.x = 400 * xcor;
            swagRect.y = 25 * ycor;
            swagRect.w = 150 * xcor;
            for (int i=0; i < 5; i++) {
                if(beaverArray[i] != NULL){
                    if (didCollide(duck->footballScalar, beaverArray[i]->beaverScalar)) {
                        duck->footballScalar.x = 1000 * xcor;
                        duck->footballScalar.y = 1000 * ycor;
                        // ^ need to hide the footballScalar so it doesn't
                        // mess around with where the beaver currently is
                        score += 420;
                        std::string tempscore = "Score: ";
                        tempscore += std::to_string(score);
                        swag = renderText(tempscore, font_name, font_color, 72, renderer);
                        if((rand()%100) + 1 > 50){
                            beaverArray[i]->respawn();
                            beaverArray[i]->beaverScalar.x += 50 * xcor;
                            duck->collision = true;
                        } else {
                            beaverArray[i] = NULL;
                            continue;
                        }
                        
                    }
                    beaverArray[i]->beaverScalar.x -= 1;
                    
                    if (didCollide(duck->duckScalar, beaverArray[i]->beaverScalar)) {
                        gameBreaker++;
                    }
                
                    if (didCollide(duck->duckScalar, beaverArray[i]->beaverScalar) && gameBreaker > 1 && !duck->isDead) {
                        swagRect.x = 100 * xcor;
                        duck->footballScalar.x = 1000 * xcor;
                        duck->footballScalar.y = 1000 * ycor;
                        swag = renderText("YOU FUCKING LOSER, YOU LOST", font_name, font_color, 72, renderer);
                        duck->isDead = true;
                        swagRect.w = 500 * xcor;
                        //exit = true;
                    }
                    beaverArray[i]->beaverScalar.y = beaverArray[i]->spawnPoint + 50 * sin(beaverArray[i]->beaverScalar.x * PI/30);
                
                } else {
                    spawnCount++;
                    if (beaverArray[i] == NULL){
                        if (spawnCount%50 == 0){
                            if((rand()%100) + 1 > 50){
                                int beaver_scaling_factor = 5;
                                beaverArray[i] = new Beaver(500 * xcor,200 * ycor);
                                beaverArray[i]->beaverTexture = createTexture("resources/images/beaver.bmp", renderer);
                                beaverArray[i]->spawnPoint = 50 * (rand()%9);
                                beaverArray[i]->beaverScalar.w = 15*beaver_scaling_factor;
                                beaverArray[i]->beaverScalar.h = 15*beaver_scaling_factor;
                            }
                    }
                }
            }

        }
    }
}

void Mallard::render_title_screen(){
    
    if (on_start) {
        SDL_RenderCopy(renderer, TST[1], NULL, NULL);
        SDL_RenderCopy(renderer, swag, NULL, &swagRect);
        SDL_RenderPresent(renderer);
    }
    else if (on_options) {
        SDL_RenderCopy(renderer, TST[2], NULL, NULL);
        SDL_RenderCopy(renderer, swag, NULL, &swagRect);
        SDL_RenderPresent(renderer);
    }
    else if (on_credits) {
        SDL_RenderCopy(renderer, TST[3], NULL, NULL);
        SDL_RenderCopy(renderer, swag, NULL, &swagRect);
        SDL_RenderPresent(renderer);
    }
    else if (on_quit) {
        SDL_RenderCopy(renderer, TST[4], NULL, NULL);
        SDL_RenderCopy(renderer, swag, NULL, &swagRect);
        SDL_RenderPresent(renderer);
    }else{
        SDL_RenderCopy(renderer, TST[0], NULL, NULL);
        SDL_RenderCopy(renderer, swag, NULL, &swagRect);
        SDL_RenderPresent(renderer);
    }
}



void Mallard::render_first_stage(){
    SDL_ShowCursor(0);
    int duck_scaling_factor = 3;
    //width and height get scaled by scaling_factor
    duck->duckScalar.w = 34*duck_scaling_factor;
    duck->duckScalar.h = 24*duck_scaling_factor;
    
    for (int i=0; i < 5; i++) {
        //beaverArray[i]->renderBeaver(renderer);
    }
    SDL_RenderCopy(renderer, first_stage_texture, NULL, NULL);
    count++;
  
    duck->renderDuck(renderer, count);
    for(int i = 0; i < 5; i++){
        if (beaverArray[i] != NULL){
            beaverArray[i]->renderBeaver(renderer);
        }
    }
    
    
    
    count = count%30;
    
    SDL_RenderCopy(renderer, swag, NULL, &swagRect);
    
    SDL_RenderPresent(renderer);
}

bool Mallard::didCollide( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    
    
    
    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;
    
    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    //If any of the sides from A are outside of B
    
    if( bottomA <= topB )
    {
        return false;
    }
    
    if( topA >= bottomB )
    {
        return false;
    }
    
    if( rightA <= leftB )
    {
        return false;
    }
    
    if( leftA >= rightB )
    {
        return false;
    }
    
    //If none of the sides from A are outside B
    return true;
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
        if (beaverArray[i] != NULL)
            SDL_DestroyTexture( beaverArray[i]->beaverTexture);
    }
    for (int i=0; i < 4; i++) {
        SDL_DestroyTexture(duck->DST[i]);
    }
    SDL_DestroyTexture(first_stage_texture);
    SDL_DestroyTexture(duck->footballTexture);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}