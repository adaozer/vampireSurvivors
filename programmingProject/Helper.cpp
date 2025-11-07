#include "Helper.h"
#include "Camera.h"

bool circlesIntersect(int x1, int y1, int r1, int x2, int y2, int r2) {
    // Similar to what we did in class. Find the distance between 2 objects
    int dx = x1 - x2;
    int dy = y1 - y2;
    int dist2 = dx * dx + dy * dy;
    int rsum = r1 + r2; // Add their radiuses
    return dist2 <= rsum * rsum; // If their distance isn't bigger than radiuses squared theyre colliding
}

bool onScreen(float x, float y, float w, float h, Camera& cam, const GamesEngineeringBase::Window& canvas) {
    // Check if an object is on screen
    float sx = x - cam.getX();  
    float sy = y - cam.getY(); // Find the position of the object in relation to the camera. Then check if they're on the screen (canvas height and width and bigger than 0) similar to what we did in class
    return (sx + w >= 0 && sy + h >= 0 && sx <= canvas.getWidth() && sy <= canvas.getHeight());
}

std::string clearSpaces(std::string& s) {
    // This is to clear the spaces to the left of a line when reading a file
    int i = 0;
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) // If a space is found, increment i by 1.
        i++;
    return s.substr(i); // When spaces end (on the left side) create a substring starting from i
}

bool isEmpty(std::string& s) {
    // Check if a line is empty when reading a file
    std::string s_new = clearSpaces(s); // Clear the spaces, and if the line is empty that means it was already empty (or just full of spaces)
    if (s_new.empty()) return true;
    return false;
 }

bool firstWord(std::string& line, const char* word) {
    // Check the first word in a line
    // The output is a check of matching, not a check of what the first word is exactly
    std::string s_clean = clearSpaces(line); // Clean the start so we don't get spaces before the first word
    int i = 0;
    while (word[i] && i < s_clean.size() && s_clean[i] == word[i]) // We increment i if its following the letter order of our word we inputted
        i++;
    if (word[i] == '\0') return true; // If the last space is \0 (final character) it must be equal to the same word
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
