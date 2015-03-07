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
    
    
    TTF_Init();
    font = TTF_OpenFont("resources/fonts/comic_sans.ttf", 72);
    
    font_color = {0, 0, 0, 0};
    font_name = "resources/fonts/comic_sans.ttf";
    
    swag = renderText("SWAG", font_name, font_color, 72, renderer);
    swagRect.x = 100;
    swagRect.y = 100;
    swagRect.w = 50;
    swagRect.h = 50;
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
    
    
    std::string DS[4] = {
        "single_duck",
        "double_duck",
        "triple_duck",
        "quadruple_duck",
    };
    
    for (int i=0; i < 4; i++) {
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
    //Loads individual image as texture
    SDL_Texture* loadTexture( std::string path );
    
    first_stage_surface = IMG_Load("resources/images/field3.jpg");
    first_stage_surface = SDL_ConvertSurfaceFormat(first_stage_surface, SDL_PIXELFORMAT_RGBA8888, 0);
    first_stage_texture = SDL_CreateTextureFromSurface(renderer, first_stage_surface);
    
    footballSurface = SDL_LoadBMP("resources/images/football.bmp");
    footballSurface = SDL_ConvertSurfaceFormat(footballSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    footballTexture = SDL_CreateTextureFromSurface(renderer, footballSurface);
    footballVisible = false;
    
    beaverSurface = SDL_LoadBMP("resources/images/beaver.bmp");
    beaverSurface = SDL_ConvertSurfaceFormat(beaverSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    beaverTexture = SDL_CreateTextureFromSurface(renderer, beaverSurface);
    beaverVisible = true;
    duckScalar.x = 0;
    duckScalar.y = 350;
    isDuckDead = false;
    beaverScalar.x = 500;
    beaverScalar.y = 370;
    jumping = false;
    yspeed = 0;
    count = 0;
    beaverCount = 0;
    
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, TST[0], NULL, NULL); // base title screen
    SDL_RenderPresent(renderer);
    title_visible = true;
    paused = false;
    gameBreaker = false;
    beaverStartPoint = 240;
}

void Mallard::getBools(int x, int y){
    on_start = (450 < x && x < 550) && (275 < y && y < 300);
    on_options = (435 < x && x < 565) && (320 < y && y < 345);
    on_credits = (435 < x && x < 565) && (365 < y && y < 385);
    on_quit = (465 < x && x < 535) && (410 < y && y < 430);
}

SDL_Texture* Mallard::renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer) {
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
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
        if (event.type == SDL_MOUSEMOTION && !isDuckDead){
            SDL_GetMouseState(NULL, &duckScalar.y);
            if (duckScalar.y < 0) {
                duckScalar.y = 0;
            }
            if (duckScalar.y > 400) {
                duckScalar.y = 400;
            }
        }
        
        if (event.type == SDL_KEYDOWN && first_stage_visible && !isDuckDead) {
            
            switch (event.key.keysym.sym) {
                case SDLK_p:
                    paused = true;
                case SDLK_UP:
                    if (duckScalar.y != 0){
                        duckScalar.y -= 50;
                    }
                    break;
                case SDLK_DOWN:
                    if (duckScalar.y != 400){
                        duckScalar.y += 50;
                    }
                    break;
                case SDLK_SPACE:
                    if (!footballVisible) {
                        Mix_PlayChannel(-1, quack, 0);
                        shootFootball();
                    }
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
    if (first_stage_visible) {
        if (didCollide(footballScalar, beaverScalar)) {
            footballScalar.x = 1000;
            footballScalar.y = 1000;
            // ^ need to hide the footballScalar so it doesn't
            // mess around with where the beaver currently is
            score += 420;
            std::string tempscore = "Score: ";
            tempscore += std::to_string(score);
            swagRect.w = 150;
            swag = renderText(tempscore, font_name, font_color, 72, renderer);
            beaverRespawn();
            beaverScalar.x += 50;
        }
        beaverScalar.x -= 1;
        
        
        
        /*
         * warning: the following two if statements are incredibly hacky
         * warning: it is super shitty code but i don't know how else
         * warning: to solve it, and i really don't care at the moment
         */
        if (didCollide(duckScalar, beaverScalar)) {
            gameBreaker++;
        }
        
        if (didCollide(duckScalar, beaverScalar) && gameBreaker > 1) {
            swag = renderText("YOU FUCKING LOSER, YOU LOST", font_name, font_color, 72, renderer);
            isDuckDead = true;
            swagRect.w = 500;
            //exit = true;
        }
        
        beaverScalar.y = beaverStartPoint + 50 * sin(beaverScalar.x * PI/30);
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

void Mallard::jump(){
    int speed = 20;
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
    footballScalar.x = duckScalar.x+70;
    footballScalar.y = duckScalar.y+10;
    footballScalar.w = 30;
    footballScalar.h = 30;
    footballVisible = true;
}

void Mallard::render_first_stage(){
    SDL_ShowCursor(0);
    int duck_scaling_factor = 3;
    int beaver_scaling_factor = 5;
    //width and height get scaled by scaling_factor
    duckScalar.w = 34*duck_scaling_factor;
    duckScalar.h = 24*duck_scaling_factor;
    
    
    beaverScalar.w = 15*beaver_scaling_factor;
    beaverScalar.h = 15*beaver_scaling_factor;
    
    
    SDL_RenderCopy(renderer, first_stage_texture, NULL, NULL);
    if (beaverVisible) {
        SDL_RenderCopy(renderer, beaverTexture, NULL, &beaverScalar);
        
    }
    count++;
    beaverCount++;
    if (footballVisible) {
        footballScalar.x +=10;
        SDL_RenderCopy(renderer, footballTexture, NULL, &footballScalar);
        SDL_RenderCopy(renderer, DST[1], NULL, &duckScalar);
        if (footballScalar.x > 640 || didCollide(footballScalar, beaverScalar)){
            footballVisible = false;
        }
    }
    else if (isDuckDead) {
        SDL_RenderCopy(renderer, DST[3], NULL, &duckScalar);
    }
    
    else if (count <= 15){
        SDL_RenderCopy(renderer, DST[2], NULL, &duckScalar);
    }
    else if (count < 30){
        SDL_RenderCopy(renderer, DST[0], NULL, &duckScalar);
    }
    
    count = count%30;
    
    swagRect.x = 50;
    swagRect.y = 50;
    SDL_RenderCopy(renderer, swag, NULL, &swagRect);
    
    SDL_RenderPresent(renderer);
}

void Mallard::beaverRespawn(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(50,430);
    beaverStartPoint = distribution(generator);
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
    }
    SDL_DestroyTexture(first_stage_texture);
    SDL_DestroyTexture(beaverTexture);
    SDL_DestroyTexture(footballTexture);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}