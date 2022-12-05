class FieldMap {
public:
    FieldMap () {
    }
    
    void init(SDL_Renderer *renderer, int SCREEN_HEIGHT) {
        this->blocks = {};
        string path = "./static/lvl";
        this->SCREEN_HEIGHT = SCREEN_HEIGHT;
        
        for (const auto & entry : __fs::filesystem::directory_iterator(path)) {
            if (entry.path().extension() == ".svg") {
                vector<vector<string>> info = SVGParser(SCREEN_HEIGHT, entry.path());
                vector<shared_ptr<Sprite>> block;
                
                for (int i=0; i<info.size();i++) {
                    block.push_back(make_shared<basicTile>(stoi(info[i][0]), stoi(info[i][1])));
                    block[block.size()-1]->setImage(renderer);
                }
                this->blocks.push_back(block);
            }
        }
        
        this->map = blocks[0];  // 0 block is a start lvl always
        prevBlock = 0;
        curBlock = 0;
        for (int i=1; i<blocks.size(); i++) {
            this->freeBlocks.push_back(i);
        }
    }
    
    SDL_Rect *getView(SDL_Rect *texr, int camPadX, int camPadY) {             // shifts position of each tile
        SDL_Rect res = *texr;
        res.x += camPadX;
        res.y += camPadY;
        return &res;
    }
    
    vector<shared_ptr<Sprite>> translate(SDL_Rect t, vector<shared_ptr<Sprite>> block, SDL_Renderer *renderer) {
        
        int deltaX = block[0]->getTexr()->x - t.x;
        int deltaY = block[0]->getTexr()->y - t.y;
        block.erase(block.begin());
        vector<shared_ptr<Sprite>> newBlock;
        
        for (int i=0;i<block.size();i++) {
            newBlock.push_back(make_shared<basicTile>(block[i]->getRect().x, block[i]->getRect().y));
            newBlock[i]->setPosition(newBlock[i]->getTexr()->x - deltaX, newBlock[i]->getTexr()->y-deltaY);
            newBlock[newBlock.size()-1]->setImage(renderer);
        }
        return newBlock;
    }
    
    int rand(int min, int max) {
        random_device rd;
        mt19937 rng(rd());
        uniform_int_distribution<int> uni(min,max);
        auto random_integer = uni(rng);
        while (random_integer == this->curBlock) {
            random_integer = uni(rng);
        }
        this->curBlock = random_integer;
        return random_integer;
    }
    
    void updateMap(Player player, SDL_Renderer *renderer) {
        if (map[map.size()-1]->getTexr()->y > player.getTexr()->y - this->SCREEN_HEIGHT) {
            vector<shared_ptr<Sprite>> randBlock = this->blocks[rand(1,this->blocks.size()-1)];
            vector<shared_ptr<Sprite>> newBlock = translate(this->map[map.size()-1]->getRect(), randBlock, renderer);
            this->map.insert(this->map.end(), newBlock.begin(), newBlock.end());
            
        }  else if (map[blocks[prevBlock].size()-1]->getRect().y > player.getRect().y + this->SCREEN_HEIGHT) {
            if (prevBlock != curBlock) {
                map.erase(map.begin(), map.begin() + blocks[prevBlock].size()-1);
                this->collisionTile -= blocks[prevBlock].size()-1;
                prevBlock = curBlock;
            }
        }
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
    
    SDL_Rect collisionTileRect() {
        return this->map[collisionTile]->getRect();
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
    vector<shared_ptr<Sprite>> map;
    vector<vector<shared_ptr<Sprite>>> blocks;
    
    int collisionTile = 0;
    bool inPlace = true;
    int SCREEN_HEIGHT;
    SDL_Rect res;
    int prevBlock;
    int curBlock;
    vector<int> freeBlocks = {};
    
    
};
