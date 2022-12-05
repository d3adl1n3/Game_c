string translateY(string y, int SCREEN_HEIGHT, int LVL_HEIGHT) {
    int y1 = stoi(y);
    y1 = y1 - LVL_HEIGHT + SCREEN_HEIGHT;
    return to_string(y1);
}

vector<vector<string>> SVGParser(int SCREEN_HEIGHT, string pwd) {
    std::string line;
    std::ifstream in(pwd);
    vector<vector<string>> info;
    int LVL_HEIGHT = 0;
    
    if (in.is_open()) {
        while (getline(in, line)) {
            if (line.substr(1, 3) == "svg") {
                string height;
                int startH = 0;
                for (int i=0; i<line.size(); i++) {
                    if (line.substr(i, 8) == " height=") {
                        startH = i+9;
                    }
                    if (startH) {
                        if (line[startH] == '"') {
                            startH = 0;
                        } else {
                            height += line[startH];
                            startH += 1;
                        }
                    }
                }
                LVL_HEIGHT = stoi(height);
            }
            if (line.substr(1, 4) == "rect") {
                string x; string y; string color;
                int startX = 0;
                int startY = 0;
                int startC = 0;
                
                for (int i=0; i<line.size(); i++) {
                    if (line.substr(i, 3) == " x=") {
                        startX = i+4;
                    }
                    
                    if (line.substr(i, 3) == " y=") {
                        startY = i+4;
                    }
                    
                    if (line.substr(i, 6) == " fill=") {
                        startC = i+7;
                    }
                    
                    if (startX) {
                        if (line[startX] == '"') {
                            startX = 0;
                        } else {
                            x += line[startX];
                            startX += 1;
                        }
                    }
                    
                    if (startY) {
                        if (line[startY] == '"') {
                            startY = 0;
                        } else {
                            y += line[startY];
                            startY += 1;
                        }
                    }

                    if (startC) {
                        if (line[startC] == '"') {
                            startC = 0;
                        } else {
                            color += line[startC];
                            startC += 1;
                        }
                    }
                }
                y = translateY(y, SCREEN_HEIGHT, LVL_HEIGHT);
                vector<string> block = {x, y, color};
                info.push_back(block);
            }
        }
    }
    in.close();
    return info;
}
