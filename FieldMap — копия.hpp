class FieldMap {
public:
    FieldMap () {
    }
    
    void init(SDL_Renderer *renderer, int SCREEN_HEIGHT) {
        string path = "/Users/iliakateshov/Desktop/SDL_gameProject_refactored/static/lvl";
        int blockIndex = 0;
        
        for (const auto & entry : __fs::filesystem::directory_iterator(path)) {
            vector<vector<string>> info = SVGParser(SCREEN_HEIGHT, entry.path());
            
            for (int i=0; i<info.size();i++) {
                //this->blocks[blockIndex].push_back(make_unique<basicTile>(stoi(info[i][0]), stoi(info[i][1])));
                //this->blocks[blockIndex][blocks[blockIndex].size()-1]->setImage(renderer);
            }
            blockIndex += 1;
        }
        //this->map = blocks[0];
    }
    
    SDL_Rect *getView(SDL_Rect *texr, int camPadX, int camPadY) {             // shifts position of each tile
        SDL_Rect res = *texr;
        res.x += camPadX;
        res.y += camPadY;
        return &res;
    }
    
    void render(SDL_Renderer *renderer, int camPadX, int camPadY) {
        for (int i=0;i<this->map.size();i++) SDL_RenderCopy(renderer, this->map[i]->getImage(), NULL, getView(this->map[i]->getTexr(), camPadX, camPadY));
    }
    
    void destroy() {
        for (int i=0;i<this->map.size();i++) SDL_DestroyTexture(this->map[i]->getImage());
    }
    
    int getTile() {
        return this->collisionTile;
    }
    
    bool isValid(SDL_Rect a) {
        return ((a.y + a.h) >= a.y) && ((a.x + a.w) >= a.x);
    }
    
    bool collision(SDL_Rect a, SDL_Rect b) {
        this->res.x = MAX(b.x, a.x);
        this->res.y = MAX(b.y, a.y);
        this->res.h = MIN(b.y+b.h, a.y+a.h) - this->res.y;
        this->res.w = MIN(b.x+b.w, a.x+a.w) - this->res.x;
        if (isValid(this->res)) return true;
        return false;
    }
    
    SDL_Rect getCollisionRect() {return this->res;}
    
    bool collisionCheck(Player player) {
        if (collision(player.getRect(), this->map[collisionTile]->getRect())) {
            this->inPlace = true;
            return true;                                                            // collision with same tile -> slide
        }
        
        if (map[this->collisionTile]->getRect().y > player.getRect().y) {
            for (int i=this->collisionTile;i<this->map.size();i++) {
                if ((this->map[i]->getRect().y + this->map[i]->getRect().h) < player.getRect().y) break;
                if (collision(player.getRect(), this->map[i]->getRect())) {
                    this->collisionTile = i;
                    this->inPlace = false;
                    return true;
                }
            }
        } else {
            for (int i=this->collisionTile;i>=0;i--) {
                if (this->map[i]->getRect().y > (player.getRect().y + player.getRect().h)) break;
                if (collision(player.getRect(), this->map[i]->getRect())) {
                    this->collisionTile = i;
                    this->inPlace = false;
                    return true;
                }
            }
        }
        return false;
    }
    
    bool isInPlace() {
        return this->inPlace;
    }
    
    
private:
    /*
    vector<shared_ptr<Sprite>> map;
    vector<vector<shared_ptr<Sprite>>> blocks;
    */
    vector<unique_ptr<Sprite>> map;
    vector<vector<shared_ptr<Sprite>>> blocks;
    int collisionTile = 0;
    bool inPlace = true;
    SDL_Rect res;
};
