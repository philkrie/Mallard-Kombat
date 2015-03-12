#include "mallard.hpp"
#include "duck.hpp"
#include "util.hpp"
#include <stdlib.h>

/* Screen resolution */
int Mallard::SCREEN_WIDTH = 640;
int Mallard::SCREEN_HEIGHT = 480;

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
    
    //Create a new Duck object: this is the main character
    duck = new Duck();
    
    //Initialize fonts, and set color to black, and comic sans as the font
    TTF_Init();
    font_color = {0, 0, 0, 0};
    font_name = "resources/fonts/comic_sans.ttf";
    text = renderText("", font_name, font_color, 72, renderer);
    textRect.x = 100;
    textRect.y = 100;
    textRect.w = 50;
    textRect.h = 50;
    pauseTexture = renderText("Paused. Press Enter for main menu", font_name, font_color, 72, renderer);
    losingText = renderText("Or press f to return to the main menu.", font_name, font_color, 72, renderer);
    pauseRect.x = 10;
    pauseRect.y = textRect.y - 50;
    pauseRect.w = 350;
    pauseRect.h = textRect.h;
    
    //Read in highscores stored in hiscores.txt
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
    
    //Importing and creating textures for game objects
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

    for (int i=0; i < 5; i++) {
        enemyArray[i] = NULL;
        std::string filepath = path + TS[i] + ".bmp";
        char *temp = (char*)filepath.c_str();
        TST[i] = createTexture(temp, renderer);
    }
    controls = createTexture("resources/images/controls.bmp", renderer);
    
    //Create duck textures
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
    
    //create some more textures
    first_stage_texture = createTexture("resources/images/field2.jpg", renderer);
    blank = createTexture("resources/images/blank.bmp", renderer);
    duck->footballTexture = createTexture("resources/images/football.bmp", renderer);
    duck->footballVisible = false;
    
    //Initialize some variables, including seeding the random number generator using current computer time
    srand(time(NULL));
    duck->isDead = false;
    count = 0;
    spawnCount = 0;
    score = 0;
    
    //Clear renderer, place the opening screen, and get ready to play!
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, TST[0], NULL, NULL); // base title screen
    SDL_RenderPresent(renderer);
    title_visible = true;
    first_stage_visible = false;
    paused = false;
    scoresVisible = false;
}

//Determines where mouse pointer is on the title screen
void Mallard::getBools(int x, int y){
    on_start =   (450 < x && x < 550 ) & (275 < y && y < 300);
    on_controls = (435 < x && x < 565 ) & (320 < y && y < 345);
    on_highscores = (435 < x && x < 565 ) & (365 < y && y < 385);
    on_quit =    (465 < x && x < 535 ) & (410 < y && y < 430);
}

//Controls the input of the user
void Mallard::input(){
    
    int x, y;
    SDL_Event event;
    
    //if game is paused
    while (paused) {

        SDL_RenderCopy(renderer, text, NULL, &textRect);
        SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_p) {
                    paused = false;
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
    
    //During gameplay and title screen
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
            if (duck->duckScalar.y > (SCREEN_HEIGHT - duck->duckScalar.h)) {
                duck->duckScalar.y = (SCREEN_HEIGHT - duck->duckScalar.h);
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
                    SDL_DestroyTexture(text);
                    break;
            }
        }
    }
}

//Updates positions of every single object on the screen, every 10ms.
void Mallard::update(){
    if (first_stage_visible && !duck->isDead) {
        textRect.x = 400;
        textRect.y = 25;
        textRect.w = 150;
        for (int i=0; i < 5; i++) {
            if(enemyArray[i] != NULL){
                //Updates score and kills enemy if ball hits it
                if (didCollide(duck->footballScalar, enemyArray[i]->enemyScalar)) {
                    duck->footballScalar.x = 1000;
                    duck->footballScalar.y = 1000;
                    score += 420;
                    std::string tempscore = "Score: ";
                    tempscore += std::to_string(score);
                    text = renderText(tempscore, font_name, font_color, 72, renderer);
                    //Sometimes enemy dies, sometimes it doesn't and is just hit back
                    if((rand()%100) + 1 > 50){
                        enemyArray[i]->respawn();
                        enemyArray[i]->enemyScalar.x += 50;
                        duck->collision = true;
                    } else {
                        delete enemyArray[i];
                        enemyArray[i] = NULL;
                        continue;
                    }   
                }

                //If enemy without ball leaves screen, it gets reset
                if (enemyArray[i]->enemyScalar.x < -75) {
                    delete enemyArray[i];
                    enemyArray[i] = NULL;
                    continue;
                }
              
                //Check if enemy collides with duck OR football carrying beaver reaches the endzone
                if ((didCollide(duck->duckScalar, enemyArray[i]->enemyScalar) && !duck->isDead) ||
                    (enemyArray[i]->hasFootball && enemyArray[i]->enemyScalar.x < 0)){
                    textRect.x = 20;
                    textRect.y = 10;
                    textRect.w = 450;
                    duck->footballScalar.x = 1000;
                    duck->footballScalar.y = 1000;
                    text = renderText("You lost! Press r to play again", font_name, font_color, 72, renderer);
                    duck->isDead = true;
                    recordScore(score);
                    textRect.w = 500;
                    //exit = true;
                }
                enemyArray[i]->move();
               
            //Logic for spawning enemies: there is a spawn counter, every time it reaches 50, chance for enemy to spawn. Each type has a certain chance to spawn as well. 
            } else {
                int scaling_factor = 4;
                spawnCount++;
                if (enemyArray[i] == NULL){
                    if (spawnCount%50 == 0){
                        int chance = rand()%100;
                        if(chance >= 0 & chance < 40){
                            enemyArray[i] = new Beaver(500,50 * (rand()%9));
                            if(chance < 10){
                                enemyArray[i]->hasFootball = true;
                                enemyArray[i]->footballTexture = footballSkin;
                            }
                            enemyArray[i]->enemyTexture = beaverSkin;
                            enemyArray[i]->footballScalar.w = 20*1.5;                                
                            enemyArray[i]->footballScalar.h = 14*1.5;
                            enemyArray[i]->enemyScalar.w = 15*scaling_factor;
                            enemyArray[i]->enemyScalar.h = 15*scaling_factor;
                        }
                        else if(chance >= 40 & chance < 75){
                            enemyArray[i] = new Husky(500, 50 * (rand()%9));
                            enemyArray[i]->enemyTexture = huskySkin;
                            enemyArray[i]->enemyScalar.w = 15*scaling_factor;
                            enemyArray[i]->enemyScalar.h = 15*scaling_factor;
                        }
                        else if(chance >= 75 & chance < 100){
                            enemyArray[i] = new Trojan(500, 50 * (rand()%9));
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

//Renders the title screen
void Mallard::render_title_screen(){
    // make sure the cursor is showing on title screen
    SDL_ShowCursor(1);
    if (scoresVisible) {
        render_blank_screen();
    }
    if (on_start) {
        SDL_RenderCopy(renderer, TST[1], NULL, NULL);
        SDL_RenderCopy(renderer, text, NULL, &textRect);
        SDL_RenderPresent(renderer);
    }
    else if (on_controls) {
        SDL_RenderCopy(renderer, controls, NULL, NULL);
        SDL_RenderCopy(renderer, text, NULL, &textRect);
        SDL_RenderPresent(renderer);
    }
    else if (on_highscores) {
        if (scoresVisible) {
            render_blank_screen();
            title_visible = false;
        } else {
            SDL_RenderCopy(renderer, TST[3], NULL, NULL);
            SDL_RenderCopy(renderer, text, NULL, &textRect);
            SDL_RenderPresent(renderer);
        }
    }
    else if (on_quit) {
        SDL_RenderCopy(renderer, TST[4], NULL, NULL);
        SDL_RenderCopy(renderer, text, NULL, &textRect);
        SDL_RenderPresent(renderer);
    } else {
        SDL_RenderCopy(renderer, TST[0], NULL, NULL);
        SDL_RenderCopy(renderer, text, NULL, &textRect);
        SDL_RenderPresent(renderer);
    }
}

//Renders the high scores
void Mallard::render_blank_screen(){
    SDL_RenderCopy(renderer, blank, NULL, NULL);
    SDL_Rect temp;
    temp.x = 50;
    temp.y = 50;
    temp.w = 400;
    temp.h = 72;
    SDL_Texture *temptext = renderText("Here are the top five recorded scores:", font_name, font_color, 72, renderer);
    SDL_RenderCopy(renderer, temptext, NULL, &temp);
    temp.y += 350;
    temp.w = 550;
    temptext = renderText("You may return back to the main menu by pressing Enter", font_name, font_color, 72, renderer);
    SDL_RenderCopy(renderer, temptext, NULL, &temp);
    for (int i=0; i < 5; i++) {
        highRect.y+=50;
        SDL_RenderCopy(renderer, highTextures[i], NULL, &highRect);
    }
    SDL_RenderPresent(renderer);
}

//Renders the first stage
void Mallard::render_first_stage(){
    // hide the cursor during the game 
    SDL_ShowCursor(0);
    double duck_scaling_factor = 3;
    double football_scaling_factor = 1.5;

    //width and height get scaled by scaling_factor
    duck->duckScalar.w = 34*duck_scaling_factor;
    duck->duckScalar.h = 24*duck_scaling_factor;
    duck->footballScalar.w = 20 * football_scaling_factor ;
    duck->footballScalar.h = 14 * football_scaling_factor ;

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
    
    SDL_RenderCopy(renderer, text, NULL, &textRect);
    if (duck->isDead) {
        SDL_Rect tempRect;
        tempRect.x = 20;
        tempRect.y = textRect.y + 50;
        tempRect.w = 500;
        tempRect.h = textRect.h;
        SDL_RenderCopy(renderer, losingText, NULL, &tempRect);
    }
    SDL_RenderPresent(renderer);
}

//Checks for collision
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
    
    if( bottomA <= topB + 20 )
        return false;
    
    if( topA >= bottomB + 20 )
        return false;
    
    if( rightA <= leftB + 10 )
        return false;
    
    if( leftA >= rightB + 10 )
        return false;
    
    //If none of the sides from A are outside B
    return true;
}

//Resets values
void Mallard::reset(){
    score = 0;
    count = 0;
    duck -> isDead = false;
    for (int i = 0; i < 5; i ++){
        delete enemyArray[i];
        enemyArray[i] = NULL;
    }
    text = renderText("", font_name, font_color, 72, renderer);
}

//Chooses appropriate render function
void Mallard::render(){
    if (title_visible) {
        render_title_screen();
    }
    if (first_stage_visible) {
        render_first_stage();
    }

}

//Records the score of the game, places into the high score file if it is high enough
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

//Main execution. Loops through input, update, and render, with SDL_Delay in between
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

//Clean up, clean up, everybody do your share!
void Mallard::clean_up(){
    //Delete titlescreen textures and enemy textures
    for (int i=0; i < 5; i++) {
        SDL_DestroyTexture(TST[i]);
        if (enemyArray[i] != NULL)
            SDL_DestroyTexture(enemyArray[i]->enemyTexture);
    }
    //Destroy duck textures
    for (int i=0; i < 4; i++) {
        SDL_DestroyTexture(duck->DST[i]);
    }
    SDL_DestroyTexture(first_stage_texture);
    SDL_DestroyTexture(duck->footballTexture);
    SDL_DestroyTexture(beaverSkin);
    SDL_DestroyTexture(huskySkin);
    SDL_DestroyTexture(trojanSkin);
    SDL_DestroyTexture(footballSkin);
    SDL_DestroyTexture(pauseTexture);
    SDL_DestroyTexture(losingText);
    SDL_DestroyTexture(text);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}