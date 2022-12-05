class Camera {
public:
    Camera() {}
    
    void setPadding(int camPadX, int camPadY) {
        this->camPadX = camPadX; this->camPadY = camPadY;
    }
    
    void move(int x, int y) {
        this->camPadX -= x; this->camPadY -= y;
    }
    
    vector<int> getPadding() {
        return {this->camPadX, this->camPadY};
    }
    
    void trackPlayer(Player player, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        int maxShiftX = 50;
        
        int camX = (-1)*this->camPadX + SCREEN_WIDTH/2;
        int camY = (-1)*this->camPadY + SCREEN_HEIGHT/2;
        int camDeltaX = player.getPosition()[0] - camX;
        int camDeltaY = player.getPosition()[1] - camY;
        
        int maxDistanceY = 15;
        float shiftY = ((float) camDeltaY / (float) maxDistanceY);
        if (shiftY < -1) shiftY = -1;
        if (shiftY > 0) shiftY = 0;
        shiftY *= -1;
        
        if ((camDeltaX < maxShiftX) && (camDeltaX > (-1)*maxShiftX)) {
            move(0, shiftY*camDeltaY);
            
        } else if (camDeltaX > maxShiftX) {
            move(camDeltaX - maxShiftX, shiftY*camDeltaY);
            
        } else if (camDeltaX < (-1) * maxShiftX) {
            move(camDeltaX + maxShiftX, shiftY*camDeltaY);
        }
        
        // пока расстояние между камерой и обьектом по вертикали меньше заданного - камера двигается к цели с скоростью обьекта с понижающим коэффициентом от 0 до 1 в зависимости от процента заданного расстояния при достижении максималоьного расстояния камера начинает двигаться со скоростью обьекта - тем самым обеспечивается небольшое запаздывание камеры, которое дает возможность обьекту немного упасть после апогея не сблизившись с границей экрана
        // по горизонтали у обьекта есть диапазон координат - [x1, x2] - пока обьект в нем, камера не двигается - если выходит за него, камера начинает догонять обьект, необходимо для статичного продвижения по вертикальным участкам уровня и динамического для движенияв вбок
    }

    
private:
    // padding
    int camPadX;
    int camPadY;
};
