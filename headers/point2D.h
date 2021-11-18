#pragma once
/**
 * Point object
 *
 * Contains X and Y coord
 *
 */
class Point2D {
    protected:
    double xCoord = 0, yCoord = 0;
    
    public:
    Point2D(double xCoord, double yCoord)
        : xCoord{xCoord},
          yCoord{yCoord}
        { }
    /*
    *   Getters for properties
    */
    const double getXCoord() const { return xCoord; }
    const double getYCoord() const { return yCoord; }
};