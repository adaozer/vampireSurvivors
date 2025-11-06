#include "Helper.h"

bool circlesIntersect(int x1, int y1, int r1, int x2, int y2, int r2)
{
    int dx = x1 - x2;
    int dy = y1 - y2;
    int dist2 = dx * dx + dy * dy;
    int rsum = r1 + r2;
    return dist2 <= rsum * rsum;
}

bool onScreen(float x, float y, float w, float h, Camera& cam, const GamesEngineeringBase::Window& canvas)
{
    float sx = x - cam.getX();  
    float sy = y - cam.getY();
    return (sx + w >= 0 && sy + h >= 0 && sx <= canvas.getWidth() && sy <= canvas.getHeight());
}

std::string clearSpaces(std::string& s) {
    int i = 0;
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
        i++;
    return s.substr(i);
}

bool isEmpty(std::string& s) {
    std::string s_new = clearSpaces(s);
    if (s_new.empty()) return true;
    return false;
 }

bool firstWord(std::string& line, const char* word) {
    std::string s_clean = clearSpaces(line);
    int i = 0;
    while (word[i] && i < s_clean.size() && s_clean[i] == word[i])
        i++;
    if (word[i] == '\0') return true;
    return false;
}

bool parseCSV(std::string line, int* output, int tiles) {
    for (char& c : line) 
        if (c == ',') 
            c = ' ';
    std::istringstream iss(line);
    for (unsigned i = 0; i < tiles; ++i)
        if (!(iss >> output[i])) return false;
    int dummy;
    if (iss >> dummy) return false;
    return true;
}

int score = 0;
