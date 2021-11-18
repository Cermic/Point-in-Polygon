#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cassert>

#include "headers/point2D.h"
#include "headers/polygon.h"

/**
* Reads a text file and parses out a list of numbers (doubles to avoid precision loss)
*
* @param fileName The name of the file to be read
* @return A list of rows, each containing the numbers on each line
*/
std::vector<std::vector<double>> readInputFile(const std::string fileName) {
    std::string line;
    std::ifstream file;
    std::vector<std::vector<double>> list = {};
    file.open(fileName);
    if (!file.is_open()) {
      std::cerr << "File failed to open: " << fileName;
    }
    while (std::getline(file, line)) {
        if (!line.empty() && line.front() != '#') {
            // Seperate out numbers in line
            line.erase(remove(line.begin(), line.end(), ','));
            std::stringstream lineStream(line);
            std::vector<double> lineValues = {};
            double value;
            while (lineStream >> value) {
                lineValues.push_back(value);
            }
            // Form the element with the numbers
            if (lineValues.size() == 2) { // If there only two numbers, it's a 2D Point
                list.push_back({lineValues[0], lineValues[1]});
            }
            else { // Otherwise, it'll be our polygon since we only have these two types
                list.push_back({lineValues[0], lineValues[1], lineValues[2], lineValues[3]});
            }
        }
    }
    file.close();
    line = "";
    return list;
}

/**
* Creates a text file with the results of the point in polygon checks
*
* @param fileName The name of the file to be created
* @param outputNumbers Numbers to write to file
*/
void writeOutputFile(const std::string fileName, std::vector<int> outputNumbers) {
    std::ofstream file(fileName);
    std::cout << "Writing results to " << fileName << std::endl;

    file << "# Results" << std::endl;

    for (auto number: outputNumbers) {
        file << "\n" << number;
    }
    file.close();
    std::cout << "Writing results complete." << std::endl;
}


/**
* Calculates where point lies with respect to a line between linepoints 1 & 2
*
* @param linePoint1 The first 2D point forming the test line
* @param linePoint2 The second 2D point forming the test line
* @param point The point being tested against the line
* @return >0 if point lies left of the line, 0 if point lies on the line & <0 if point lies on the right of the line 
*/
int isLeft(Point2D linePoint1, Point2D linePoint2, Point2D point)
{
    return ((linePoint2.getXCoord() - linePoint1.getXCoord()) * (point.getYCoord() - linePoint1.getYCoord()) 
            - (point.getXCoord() -  linePoint1.getXCoord()) * (linePoint2.getYCoord() - linePoint1.getYCoord()));
}

/**
* Calculates the winding number value of a point compared to a rectangular polygon
*
* @param point The 2D Point
* @param polygon The 2D Rectangle
* @return The winding number value of the point, if 0 then the point is outside the polygon, if non 0, it's inside.
*/
int pointInPolygon(Point2D point, Polygon polygon)
{
    int windingNumber = 0;
    // Loop through all edges of the polygon
    for (int i = 0; i < polygon.getNumVertices(); i++) {
        // First check to see if any of the polygon vertices are below the point so the point might be inside
        if (polygon.getVertices()[i].getYCoord() <= point.getYCoord()) {
            /* 
                Then, check if vertex is creating an upward crossing, that is the line looks like "\"
                 & Polygon Vertex V+1 will be to the left and above Polygon Vertex V 
            */
            if (polygon.getVertices()[i+1].getYCoord() > point.getYCoord())  {
                /* 
                    Now that we know we have an upward crossing, we check to see if point is left of the line, 
                    if so increment the winding number
                */
                if (isLeft(polygon.getVertices()[i], polygon.getVertices()[i+1], point) > 0)
                    windingNumber++;            
            }
        }
        /* 
            We are essentially creating triangles between the 3 points, polygon V & V+1 with respect to point
            and using this to determine the winding number value of the point
        */ 
        else if ((i + 1) < polygon.getNumVertices())
        {// We know here that the polygon vertices are above the point so no need to perform a check
            /* 
                Then, check if vertex is creating an downward crossing, that is the line looks like "/"
                 & Polygon Vertex V+1 will be left and below Polygon Vertex V 
            */
            if (polygon.getVertices()[i+1].getYCoord() <= point.getYCoord()) {
                /* 
                    Now that we know we have an downward crossing, we check to see if point is right of the line, 
                    if so decrement the winding number
                */
                if (isLeft(polygon.getVertices()[i], polygon.getVertices()[i+1], point) < 0)
                    windingNumber--;
            }     
        }
    }
    return windingNumber;
}

/**
* Checks a list of points against a list of polygons to see if the points are inside any of the polygons
*
* @param points The 2D Points
* @param polygons The polygons
* @return A list of numbers showing whether each point was inside any of the polygons (0 for inside, 1 for outside)
*/
std::vector<int> checkPoints(std::vector<Point2D> points, std::vector<Polygon> polygons) {
    std::vector<int> results = {};
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < polygons.size(); j++) {
            // If the winding number is not 0, as in inside - we write 0 to the output file as per requirements
            if (pointInPolygon(points[i], polygons[j]) !=0) {
                // Move onto next iteration of outer loop
                std::cout << "Line:" << i + 1 << " Point in Polygon: Adding 0 to output results {X:" 
                << points[i].getXCoord() << ", Y:" << points[i].getYCoord() << "}" 
                << " and Polygon {X:" << polygons[j].getXCoord() << ", Y:" << polygons[j].getYCoord()
                << ", W:" << polygons[j].getWidth() << ", H:" << polygons[j].getHeight() << "}"
                << std::endl;
                results.push_back(0);
                i++;
            }
        }
        std::cout << "Line:" << i + 1 << " Point not in polygon: Adding 1 to output results" << std::endl;
        results.push_back(1);
        // If the winding number is 0, as in outisde - we write 1 to the output file as per requirements
    }
    return results;
}

int main(void) {
    /////////////////
    // TESTS BEGIN //
    /////////////////
    Point2D p(1, 0);
    Point2D p2(10 , 10);
    Point2D p3(0, -1);
    Point2D p4(2.51584, 6.18651);
    ////////////////////////////
    Polygon poly(0, 0, 5, 5);
    Polygon poly2(-3, -4, 3, 3);
    Polygon poly3(0, 0, 3, 5);
    Polygon poly4(5.08222, 6.19908, 2.42599, 1.31244);
    // p should be inside poly
    assert (pointInPolygon(p, poly) != 0);
    // p2 should be outside poly on the right
    assert (pointInPolygon(p2, poly) == 0);
    // p3 should not be outside poly below
    assert (pointInPolygon(p3, poly) == 0);
    // p should not be inside poly2 above
    assert (pointInPolygon(p, poly2) == 0);
    // p3 should not be inside poly2, should be on the line
    assert (pointInPolygon(p3, poly2) == 0);
    // p should be inside poly3
    assert (pointInPolygon(p, poly3) != 0);
    // p3 should not be inside poly3
    assert (pointInPolygon(p3, poly3) == 0);
    // p4 should be outside poly on the left
    assert(pointInPolygon(p4, poly4) == 0);
    ///////////////
    // TESTS END //
    ///////////////

    std::vector<Polygon> polygons = {};
    std::vector<Point2D> points = {};
    // Read in polygon / point data
    auto filePolygons = readInputFile("Input-Files/Space-Definition.txt");
    auto filePoints = readInputFile("Input-Files/Ray-Definition.txt");
    // Determine which points lie in a polygon and save the result
    for (auto poly: filePolygons) {
        polygons.push_back(Polygon(poly[0], poly[1], poly[2], poly[3]));
    }
    for (auto point: filePoints) {
        points.push_back(Point2D(point[0], point[1]));
    }
    std::vector<int> outputNumbers = {};
    outputNumbers = checkPoints(points, polygons);
    writeOutputFile("Output-File/Results.txt", outputNumbers);
    std::cout << "This program cycle is complete, please close the program..." << std::endl;
    std::cin.get();
    return 0;
}