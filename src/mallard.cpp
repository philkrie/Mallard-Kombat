#include "mallard.hpp"
#include "duck.hpp"
#include "util.hpp"
#include <stdlib.h>


/* Screen resolution */
int Mallard::SCREEN_WIDTH = 640;
int Mallard::SCREEN_HEIGHT = 480;

double Mallard::xcor = SCREEN_WIDTH / 640;
double Mallard::ycor = SCREEN_HEIGHT/ 480;


Mallard::Mallard(int argc, char* argv[]) {
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
    swag = renderText("", font_name, font_color, 72, renderer);
    swagRect.x = 100 * xcor;
    swagRect.y = 100 * ycor;
    swagRect.w = 50 * xcor;
    swagRect.h = 50 * ycor;
    score = 0;
    
    std::string line;
    file.open("resources/text/hiscores.txt");
    if (file.is_open()) {
        for (int i = 0; i < 5; i++){
            getline (file,line);
            highscores[i] = std::stoi( line );
        }
    }
    file.close();
    
    // Sounds
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    quack = Mix_LoadWAV("resources/sounds/quack.wav");
    
    beaverSkin = createTexture("resources/images/beaver.bmp", renderer);
    huskySkin = createTexture("resources/images/husky.bmp", renderer);
    trojanSkin = createTexture("resources/images/trojan.bmp", renderer);
    footballSkin = createTexture("resources/images/football.bmp", renderer);

    path = "resources/images/";
    //TS stands for TitleScreens
    std::string TS[5] = {
        "title_screen",
        "title_screen_start",
        "title_screen_options",
        "title_screen_hiscores",
        "title_screen_quit",
    };
    // Creating the title screens
    
    //TSS stands for TitleScreenSurfaces
    srand(time(NULL));
    for (int i=0; i < 5; i++) {
        enemyArray[i] = NULL;
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
    
    first_stage_texture = createTexture("resources/images/field2.jpg", renderer);
    blank = createTexture("resources/images/blank.bmp", renderer);
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
    scoresVisible = false;
}

void Mallard::getBools(int x, int y){
    on_start =   (450 * xcor < x && x < 550 * xcor ) & (275 * ycor < y && y < 300 * ycor);
    on_options = (435 * xcor < x && x < 565 * xcor ) & (320 * ycor < y && y < 345 * ycor);
    on_highscores = (435 * xcor < x && x < 565 * xcor ) & (365 * ycor < y && y < 385 * ycor);
    on_quit =    (465 * xcor < x && x < 535 * xcor ) & (410 * ycor < y && y < 430 * ycor);
}

void Mallard::input(){
    
    int x, y;
    SDL_Event event;
    
    while (paused) {
        SDL_Texture *pauseTexture = renderText("Paused. Press Enter for main menu", font_name, font_color, 72, renderer);
        SDL_Rect pauseRect;
        pauseRect.x = 10;
        pauseRect.y = swagRect.y;
        pauseRect.w = 350;
        pauseRect.h = swagRect.h;
        SDL_RenderCopy(renderer, swag, NULL, &swagRect);
        SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_p) {
                    paused = false;
                    if (event.type == SDL_KEYDOWN && first_stage_visible && duck->isDead) {
                        switch(event.key.keysym.sym){
                            case SDLK_r:
                                reset();
                                break;
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    paused = false;
                    reset();
                    title_visible = true;
                    first_stage_visible = false;
                }
            }
            if (event.type == SDL_QUIT) {
                paused = false;
                exit = true;
            }

        }
    }
    
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
            if (on_highscores && title_visible) {
                std::string line;
                scoresVisible = true;
                SDL_RenderClear(renderer);
                highRect.x = 400;
                highRect.y = -170;
                highRect.w = 100;
                highRect.h = 75;
                for (int i=0; i<5; i++) {
                    highTextures[i] = renderText(std::to_string(highscores[i]), font_name, font_color, 72, renderer);
                }
                
            }
            if (on_quit && title_visible) {
                exit = true;
            }
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_RETURN && title_visible) {
                title_visible = false;
                SDL_RenderClear(renderer);
                Mix_PlayChannel(-1, quack, 0);
                first_stage_visible = true;
            }
            if (event.key.keysym.sym == SDLK_RETURN && scoresVisible) {
                title_visible = true;
                SDL_RenderClear(renderer);
                scoresVisible = false;
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
            if (duck->duckScalar.y > (SCREEN_HEIGHT - duck->duckScalar.h) * ycor) {
                duck->duckScalar.y = (SCREEN_HEIGHT - duck->duckScalar.h) * ycor;
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
        if (event.type == SDL_KEYDOWN && first_stage_visible && duck->isDead) {
            switch (event.key.keysym.sym) {
                case SDLK_r:
                    reset();
                    break;
                case SDLK_f:
                    reset();
                    first_stage_visible = false;
                    title_visible = true;
                    SDL_DestroyTexture(swag);
                    break;
            }
        }
    }
}

void Mallard::update(){
    if (first_stage_visible && !duck->isDead) {
        swagRect.x = 400 * xcor;
        swagRect.y = 25 * ycor;
        swagRect.w = 150 * xcor;
        for (int i=0; i < 5; i++) {
            if(enemyArray[i] != NULL){
                if (didCollide(duck->footballScalar, enemyArray[i]->enemyScalar)) {
                    duck->footballScalar.x = 1000 * xcor;
                    duck->footballScalar.y = 1000 * ycor;
                    // ^ need to hide the footballScalar so it doesn't
                    // mess around with where the beaver currently is
                    score += 420;
                    std::string tempscore = "Score: ";
                    tempscore += std::to_string(score);
                    swag = renderText(tempscore, font_name, font_color, 72, renderer);
                    if((rand()%100) + 1 > 50){
                        enemyArray[i]->respawn();
                        enemyArray[i]->enemyScalar.x += 50 * xcor;
                        duck->collision = true;
                    } else {
                        delete enemyArray[i];
                        enemyArray[i] = NULL;
                        continue;
                    }
                    
                }
                if (enemyArray[i]->enemyScalar.x < -75) {
                    delete enemyArray[i];
                    enemyArray[i] = NULL;
                    continue;
                }
              
                
                if ((didCollide(duck->duckScalar, enemyArray[i]->enemyScalar) && !duck->isDead) ||
                    (enemyArray[i]->hasFootball && enemyArray[i]->enemyScalar.x < 0)){
                    swagRect.x = 20 * xcor;
                    swagRect.y = 10 * ycor;
                    swagRect.w = 450 * xcor;
                    duck->footballScalar.x = 1000 * xcor;
                    duck->footballScalar.y = 1000 * ycor;
                    swag = renderText("You lost! Press r to play again", font_name, font_color, 72, renderer);
                    duck->isDead = true;
                    recordScore(score);
                    swagRect.w = 500 * xcor;
                    //exit = true;
                }
                enemyArray[i]->move();
                
            } else {
                int scaling_factor = 3 * sqrt(pow(xcor,2) + pow(ycor,2));
                spawnCount++;
                if (enemyArray[i] == NULL){
                    if (spawnCount%50 == 0){
                        int chance = rand()%100;
                        if(chance >= 0 & chance < 40){
                            enemyArray[i] = new Beaver(500 * xcor,50 * ycor * (rand()%9));
                            if(chance < 10){
                                enemyArray[i]->hasFootball = true;
                                enemyArray[i]->footballTexture = footballSkin;
                            }
                            enemyArray[i]->enemyTexture = beaverSkin;
                            enemyArray[i]->footballScalar.w = 20*1.5;                                
                            enemyArray[i]->footballScalar.h = 14*1.5;
                            enemyArray[i]->spawnPoint = 50 * (rand()%9);
                            enemyArray[i]->enemyScalar.w = 15*scaling_factor;
                            enemyArray[i]->enemyScalar.h = 15*scaling_factor;
                        }
                        else if(chance >= 40 & chance < 75){
                            enemyArray[i] = new Husky(500 * xcor, 50 * ycor * (rand()%9));
                            enemyArray[i]->enemyTexture = huskySkin;
                            enemyArray[i]->spawnPoint = 50 * (rand()%9);
                            enemyArray[i]->enemyScalar.w = 15*scaling_factor;
                            enemyArray[i]->enemyScalar.h = 15*scaling_factor;
                        }
                        else if(chance >= 75 & chance < 100){
                            enemyArray[i] = new Trojan(500 * xcor, 50 * ycor * (rand()%9));
                            enemyArray[i]->enemyTexture = trojanSkin;
                            enemyArray[i]->enemyScalar.w = 15*scaling_factor;
                            enemyArray[i]->enemyScalar.h = 30*scaling_factor;
                        }
                    }
                }
            }
            
        }
    }
}

void Mallard::render_title_screen(){
    SDL_ShowCursor(1);
    if (scoresVisible) {
        render_blank_screen();
    }
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
    else if (on_highscores) {
        if (scoresVisible) {
            render_blank_screen();
            title_visible = false;
        }else{
            SDL_RenderCopy(renderer, TST[3], NULL, NULL);
            SDL_RenderCopy(renderer, swag, NULL, &swagRect);
            SDL_RenderPresent(renderer);
        }
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

void Mallard::render_blank_screen(){
    SDL_RenderCopy(renderer, blank, NULL, NULL);
    SDL_Rect temp;
    temp.x = 50 * xcor;
    temp.y = 50 * ycor;
    temp.w = 400 * xcor;
    temp.h = 72 * ycor;
    SDL_Texture *tempswag = renderText("Here are the top five recorded scores:", font_name, font_color, 72, renderer);
    SDL_RenderCopy(renderer, tempswag, NULL, &temp);
    temp.y += 350;
    temp.w = 550 * xcor;
    tempswag = renderText("You may return back to the main menu by pressing Enter", font_name, font_color, 72, renderer);
    SDL_RenderCopy(renderer, tempswag, NULL, &temp);
    for (int i=0; i < 5; i++) {
        highRect.y+=50;
        SDL_RenderCopy(renderer, highTextures[i], NULL, &highRect);
    }
    SDL_RenderPresent(renderer);
}


void Mallard::render_first_stage(){
    SDL_ShowCursor(0);
    double duck_scaling_factor = 2 * sqrt(pow(xcor,2) + pow(ycor,2));
    double football_scaling_factor = 1 * sqrt(pow(xcor,2) + pow(ycor,2));
    //width and height get scaled by scaling_factor
    duck->duckScalar.w = 34*duck_scaling_factor;
    duck->duckScalar.h = 24*duck_scaling_factor;
    
    duck->footballScalar.w = 20*football_scaling_factor;
    duck->footballScalar.h = 14*football_scaling_factor;
    SDL_RenderCopy(renderer, first_stage_texture, NULL, NULL);
    count++;
    
    duck->renderDuck(renderer, count);
    for(int i = 0; i < 5; i++){
        if (enemyArray[i] != NULL){
            if (enemyArray[i]->hasFootball){
                enemyArray[i]->footballScalar.x = enemyArray[i]->enemyScalar.x+15;
                enemyArray[i]->footballScalar.y = enemyArray[i]->enemyScalar.y+20;
            }
            enemyArray[i]->render(renderer);   
        }
    }
    
    count = count%30;
    
    SDL_RenderCopy(renderer, swag, NULL, &swagRect);
    if (duck->isDead) {
        SDL_Texture *temp = renderText("Or press f to return to the main menu.", font_name, font_color, 72, renderer);
        SDL_Rect tempRect;
        tempRect.x = 20*xcor;
        tempRect.y = (swagRect.y + 50) * ycor;
        tempRect.w = 500;
        tempRect.h = swagRect.h;
        SDL_RenderCopy(renderer, temp, NULL, &tempRect);
    }
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
        return false;
    
    if( topA >= bottomB )
        return false;
    
    if( rightA <= leftB )
        return false;
    
    if( leftA >= rightB )
        return false;
    
    //If none of the sides from A are outside B
    return true;
}

void Mallard::reset(){
    score = 0;
    count = 0;
    duck -> isDead = false;
    for (int i = 0; i < 5; i ++){
        delete enemyArray[i];
        enemyArray[i] = NULL;
    }
    swag = renderText("", font_name, font_color, 72, renderer);
}

void Mallard::render(){
    if (title_visible) {
        render_title_screen();
    }
    if (first_stage_visible) {
        render_first_stage();
    }
}

void Mallard::recordScore(int score){
    // Check if current score belongs on the high score list
    // If so, swap with the last entry of the array
    for (int i = 0; i < 5; i++) {
        if (highscores[i] < score) {
            highscores[5-1] = score;
        }
    }
    
    //  Model the array with using a vector and sort
    std::vector<int> scores_vec (highscores, highscores+5);
    std::sort (scores_vec.begin(), scores_vec.begin()+5);
    
    // Iterate through vector and write to hiscores.txt
    file.open("resources/text/hiscores.txt");
    
    int i = 0;
    for (std::vector<int>::reverse_iterator rit = scores_vec.rbegin(); rit != scores_vec.rend(); ++rit) {
        file << *rit << std::endl;
        highscores[i] = *rit;
        i++;
    }
    file.close();
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
        if (enemyArray[i] != NULL)
            SDL_DestroyTexture( enemyArray[i]->enemyTexture);
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