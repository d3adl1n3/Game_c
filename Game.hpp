class Game {
public:
    Game() {
        this->SCREEN_WIDTH = 750;
        this->SCREEN_HEIGHT = 750;
        
        this->running = true;
        this->onPause = false;
        
        SDL_Init(SDL_INIT_VIDEO);
        this->mainWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->SCREEN_WIDTH, this->SCREEN_HEIGHT, 0);
        this->renderer = SDL_CreateRenderer(this->mainWindow, -1, SDL_RENDERER_ACCELERATED);
        if (this->renderer == NULL) {
            printf("renderer error\n");
        }
        this->fieldMap.init(this->renderer, SCREEN_HEIGHT);
        this->cam.setPadding(0, 0);
        
        this->player.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        this->player.setImage(this->renderer);
        this->player.setImageFlipped(this->renderer);
        
        this->bg.setSize(this->SCREEN_WIDTH, this->SCREEN_HEIGHT);
        this->bg.setImage(this->renderer);
        this->gameOver.setImage(this->renderer);
        this->pauseButton.setImage(this->renderer);
        
        this->score = 0;
    }
    
    void restart() {
        this->fieldMap.init(this->renderer, SCREEN_HEIGHT);
        this->cam.setPadding(0, 0);
        this->player = Player();
        this->player.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        this->player.setImage(this->renderer);
        this->player.setImageFlipped(this->renderer);
        this->score = 0;
    }
    
    void close() {
        SDL_DestroyWindow(this->mainWindow);
        SDL_DestroyTexture(this->player.getImage());
        SDL_DestroyTexture(this->player.getImageFlipped());
        this->fieldMap.destroy();
        SDL_DestroyTexture(this->bg.getImage());
        SDL_DestroyTexture(this->gameOver.getImage());
        SDL_DestroyTexture(this->pauseButton.getImage());
        SDL_DestroyRenderer(this->renderer);
        SDL_Quit();
    }
    
    void handleGestures() {
        while (SDL_PollEvent(&this->e)) {
            if (this->e.type == SDL_QUIT) {
                this->running = false;
            }
            
            if (this->e.type == SDL_KEYDOWN) {
                switch(this->e.key.keysym.scancode) {
                    case 0x2C:                              // spacebar_pressed
                        if (!this->onPause) {
                            this->player.jumpPressed(this->timer);
                        }
                        break;
                        
                    case 0x29:                              // esc_pressed
                        this->onPause = !this->onPause;
                        break;
                    
                    case 0x15:
                        if (!this->player.isAlive()) this->restart();
                        break;
                        
                    default:
                        break;
                }
            }
            
            if (this->e.type == SDL_KEYUP) {
                switch(this->e.key.keysym.scancode) {
                    case 0x2C:                              // spacebar_pressed
                        this->player.jumpReleased(this->timer);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    bool isRunning() {
        return this->running;
    }
    
    bool isOnPause() {
        return this->onPause;
    }
    
    void update() {
        if (this->onPause) {
            this->timer.freeze();
            return;
        } else {
            this->timer.run();
        }
        
        if (!this->player.isAlive()) return;
        
        switch (this->player.getJumpNumber()) {
            case 0:                                                                                 // no jumps -> slide or free fall
                if (this->fieldMap.collisionCheck(this->player) && this->fieldMap.isInPlace()) {
                    this->player.changePosSlide(this->timer);
                } else if (this->fieldMap.collisionCheck(this->player)) {
                    SDL_Rect colRect = this->fieldMap.getCollisionRect();
                    if (colRect.w != 0) {
                        this->player.collision(this->fieldMap.collisionTileRect(), this->fieldMap.getCollisionRect());
                    }
                } else {
                    this->player.changePosFall(this->timer);
                }
                break;
                
            case 1:                                                                                 // track 1 jump
                this->player.changePosJump(this->timer);
                if (this->fieldMap.collisionCheck(this->player)) this->player.collision(this->fieldMap.collisionTileRect(), this->fieldMap.getCollisionRect());
                break;
                
            case 2:                                                                                 // track 2 jump
                this->player.changePosJump(this->timer);
                if (this->fieldMap.collisionCheck(this->player)) this->player.collision(this->fieldMap.collisionTileRect(), this->fieldMap.getCollisionRect());
                break;
                
            default:
                break;
        }
            
        this->fieldMap.updateMap(this->player, this->renderer);
        this->cam.trackPlayer(this->player, this->SCREEN_WIDTH, this->SCREEN_HEIGHT);           // camera tracks player
        if (this->player.getKeyPressed()) this->player.calculateVelocity(this->timer);          // change velocity due to long press
        if ((-1) * this->cam.getPadding()[1] + SCREEN_HEIGHT < this->player.getRect().y) {      // player dead if crosses bottom
            this->player.death();
        }
    }
    
    
    void render() {
        SDL_RenderClear(this->renderer);        // clear the screen
        SDL_RenderCopy(this->renderer, this->bg.getImage(), NULL, this->bg.getTexr());      // add background
        
        if (this->player.isAlive()) {
            if (this->player.getJumpDirection() == -1) {
                SDL_RenderCopy(this->renderer, this->player.getImage(), NULL, this->player.getRenderTexr(this->cam.getPadding()));  // add player
            } else {
                SDL_RenderCopy(this->renderer, this->player.getImageFlipped(), NULL, this->player.getRenderTexr(this->cam.getPadding()));  // add playerFlipped
            }
            this->fieldMap.render(this->renderer, this->cam.getPadding()[0], this->cam.getPadding()[1]);        // add obstacles
            if (this->onPause) {
                SDL_RenderCopy(this->renderer, this->pauseButton.getImage(), NULL, this->pauseButton.getTexr());    // display pause button
            }
            
        } else {
            SDL_RenderCopy(this->renderer, this->gameOver.getImage(), NULL, this->gameOver.getTexr());
        }
        
        SDL_RenderPresent(this->renderer);      // flip the backbuffer, everything that we prepared behind the screens is actually shown
    }
    
private:
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    
    SDL_Window *mainWindow;
    SDL_Renderer *renderer;
    SDL_Surface *screen_surface;
    SDL_Event e;
    
    Player player;
    FieldMap fieldMap;
    Camera cam;
    Timer timer;
    
    bool flag = true;
    bool flag2 = true;
    
    Background bg;
    GameOver gameOver;
    PauseButton pauseButton;
    
    bool running;
    bool onPause;
    int score;
};
