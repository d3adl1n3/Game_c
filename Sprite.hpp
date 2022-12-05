class Sprite {
public:
    Sprite(int x, int y, int w, int h, string texture) {
        this->texr.x = x; this->texr.y = y;
        this->texr.w = w; this->texr.h = h;
        this->texture = texture;
    }
    
    vector<int> getPosition() const {
        return {this->texr.x, this->texr.y};
    }
    
    vector<int> getHitbox() const {
        return {this->texr.w, this->texr.h};
    }
    
    void setImage(SDL_Renderer *renderer) {
        this->img = IMG_LoadTexture(renderer, texture.c_str());
        if (this->img == NULL) {
            printf("Unable to load obstacle image %s\n", IMG_GetError());
        }
    }
    
    void setSize(int w, int h) {
        this->texr.w = w;
        this->texr.h = h;
    }
    
    void setPosition(float x, float y) {
        this->texr.x = x;
        this->texr.y = y;
    }
    
    SDL_Texture *getImage() {
        return this->img;
    }
    
    SDL_Rect *getTexr() {
        return &this->texr;
    }
    
    SDL_Rect getRect() {
        return this->texr;
    }
    
    Sprite copy() {
        return Sprite(this->texr.x, this->texr.y, this->texr.w, this->texr.h, this->texture);
    }
    
private:
    string texture;
    SDL_Texture *img = NULL;
    // size(hitbox) & position
    SDL_Rect texr;
};
