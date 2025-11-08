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

bool firstWord(std::string& line, const char* word) {
    // Check the first word in a line
    // The output is a check of matching, not a check of what the first word is exactly
    int i = 0;
    while (word[i] && i < line.size() && line[i] == word[i]) // We increment i if its following the letter order of our word we inputted
        i++;
    if (word[i] == '\0') return true; // If the last space is \0 (final character) it must be equal to the same word
    return false;
}

void parseCSV(std::string line, int* output, int tiles) {
    for (char& c : line) // For char in line, replace commas with spaces
        if (c == ',') c = ' ';
    std::istringstream iss(line); // Make an istringstream to read the characters between the spaces (in our case the values in our csv)
    for (int i = 0; i < tiles; i++)
        iss >> output[i];
}

int score = 0;
int mode = -1;
