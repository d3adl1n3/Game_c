class Timer {
public:
    Timer() {
        this->ticks = SDL_GetTicks();
    }
    
    int getTime() {
        return (SDL_GetTicks() - this->ticks);
    }
    
    void freeze() {
        if (!this->freezed) {
            delta = SDL_GetTicks();
            this->freezed = true;
        }
    }
    
    void run() {
        if (this->freezed) {
            this->ticks += SDL_GetTicks() - delta;
            this->freezed = false;
        }
    }
    
private:
    int ticks;
    int delta;
    int freezed = false;
};
