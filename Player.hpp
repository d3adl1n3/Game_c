class Player : public Sprite {
public:
    Player() : Sprite (0, 0, 45, 45, "./static/img/playerSkin.png") {
        this->xVel = 0; this->yVel = 0;
        this->jump=0; this->jumpDirection = 1;
    }
    
    void setImageFlipped(SDL_Renderer *renderer) {
        this->imgFlipped = IMG_LoadTexture(renderer, "./static/img/playerSkinFlipped.png");
        if (this->imgFlipped == NULL) {
            printf("Unable to load players flipped image %s\n", IMG_GetError());
        }
    }
    
    SDL_Texture *getImageFlipped() {
        return this->imgFlipped;
    }
    
    void setVelocity(float x, float y) {
        this->xVel = x; this->yVel = y;
    }
    
    vector<float> getVelocity() const {
        return {this->xVel, this->yVel};
    }
    
    void calculateVelocity(Timer timer) {
        int maxHold = 150;                          // player hold time allowed
        float acceleration = (timer.getTime() - this->jumpTime - 50);
        acceleration /= maxHold;
        float maxX = 0;
        float maxY = 0.05;

        if ((acceleration < 1) && (acceleration > 0)) {
            this->xVel = this->prevXVel + acceleration * maxX;
            this->yVel = this->prevYVel + acceleration * maxY;
        }
    }
    
    void jumpPressed(Timer timer) {
        if (!keyPressed) {
            if (this->jump < 2) {
                this->slideStartTime = 0;
                this->fallStartTime = 0;
                this->jump += 1;
                this->jumpTime = timer.getTime();
                this->xVel = 12;
                this->yVel = 15;
                this->prevXVel = 12;
                this->prevYVel = 15;
                if (this->jump == 2) jumpDirection *= -1;
            }
            keyPressed = true;
        }
    }
    
    void jumpReleased(Timer timer) {
        this->keyPressed = false;
    }
    
    bool getKeyPressed() {
        return this->keyPressed;
    }
    
    void changePosJump(Timer timer) {
        float a; float t;
        a = 1.5;
        t = 1 + (timer.getTime() - getJumpTime()) / 1000;
        t *= 0.5;
        setVelocity(getVelocity()[0], getVelocity()[1] - a * t);
        setPosition(getPosition()[0] + this->jumpDirection*getVelocity()[0]*t, getPosition()[1] - getVelocity()[1]*t);
    }
    
    void changePosSlide(Timer timer) {
        this->fallStartTime = 0;
        if (!this->slideStartTime) {
            this->slideStartTime = timer.getTime();
            this->slideStartPosY = getPosition()[1];
        }
        float slideVelocity = 0.04;
        float t; int shift;
        t = timer.getTime() - this->slideStartTime;
        shift = this->slideStartPosY + slideVelocity*t;
        setPosition(getPosition()[0], shift);
    }
    
    void changePosFall(Timer timer) {
        this->slideStartTime = 0;
        if (!this->fallStartTime) {
            this->fallStartTime = timer.getTime();
            this->fallStartPosY = getPosition()[1];
        }
        float fallVelocity = 0.5;
        float t; int shift;
        t = timer.getTime() - this->fallStartTime;
        shift = this->fallStartPosY + fallVelocity*t;
        setPosition(getPosition()[0], shift);
    }
    
    SDL_Rect *getRenderTexr(vector<int> camPad) {
        SDL_Rect res = *getTexr();
        res.x += camPad[0];
        res.y += camPad[1];
        return &res;
    }
    
    int getJumpTime() {
        return this->jumpTime;
    }
    
    int getJumpDirection() {
        return this->jumpDirection;
    }
    
    bool isHorizontal(SDL_Rect rect) {
        if (rect.w > rect.h) return true;
        return false;
    }
    
    void collision(SDL_Rect tile, SDL_Rect rect) {
        this->jump = 0;
        this->xVel = 0;
        this->yVel = 0;
        
        if (isHorizontal(rect) && (rect.y == tile.y)) {                                 // top collision
            if (this->getRect().x + this->getRect().w/2 > tile.x + tile.w/2) {        // right side
                if (this->jumpDirection == 1) {
                    this->setPosition(tile.x + tile.w, this->getRect().y);
                } else {
                    this->setPosition(tile.x + tile.w, this->getRect().y);
                    this->jumpDirection = 1;
                }
                return;
            } else {
                if (this->jumpDirection == 1) {
                    this->setPosition(tile.x - this->getRect().w, this->getRect().y);
                    this->jumpDirection = -1;
                } else {
                    this->setPosition(tile.x - this->getRect().w, this->getRect().y);
                }
                return;
            }
            
        } else if ((isHorizontal(rect) && ((rect.y + rect.h) == (tile.y + tile.h)))) {  // bottom collisions
            return;
            
        } else {
            this->jumpDirection *= -1;                                                  // side collisions
            if (this->jumpDirection == 1) {
                this->setPosition(rect.x + rect.w, this->getRect().y);
            } else {
                this->setPosition(rect.x - this->getRect().w, this->getRect().y);
            }
        }
        
    }
    
    int getJumpNumber() {
        return this->jump;
    }
    
    void death() {
        this->alive = false;
    }
    
    bool isAlive() {
        return this->alive;
    }
        
private:
    float xVel;
    float yVel;
    
    float prevXVel;
    float prevYVel;
    
    int jump; // 1 or 2
    int jumpTime;
    int jumpDirection; // -1 or 1
    
    int slideStartTime = 0;
    int slideStartPosY = 0;
    
    int fallStartTime = 0;
    int fallStartPosY = 0;
    bool keyPressed = false;
    
    bool alive = true;
    
    SDL_Texture *imgFlipped = NULL;
};
