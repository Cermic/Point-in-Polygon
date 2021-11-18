#include <vector>
#include "point2D.h"

/**
 * Polygon object
 *
 * Contains X and Y coord, width and height
 *
 */
class Polygon: public Point2D {
    private:
    double width = 0, height = 0;

    std::vector<Point2D> vertices;
    // This number of vertices counter isn't const, since a polygon may have more vertices added to it in future
    int NUM_OF_VERTICES;
    public:
    Polygon(double xCoord, double yCoord, double width, double height) : Point2D (xCoord, yCoord)
        { 
          width = width;
          height = height;
          /* 
              The first set of coords is only the bottom left of the polygon, to figure out it's other vertices,
              we'll need to use it's width and height
          */
          vertices.push_back(Point2D(xCoord, yCoord));
          vertices.push_back(Point2D(xCoord, yCoord + height));
          vertices.push_back(Point2D(xCoord + width, yCoord + height));
          vertices.push_back(Point2D(xCoord + width, yCoord));
          NUM_OF_VERTICES = vertices.size();
        }
    /*
    *   Getters for properties
    */
    const double getWidth() const { return width; }
    const double getHeight() const { return height; }
    const std::vector<Point2D> getVertices() const { return vertices; }
    const int getNumVertices() const {return vertices.size();}
};