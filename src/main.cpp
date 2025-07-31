#include "graph.hpp"
#include "functions.cpp"

float line(float x)
{
    return x;
}
float line2(float x)
{
    return tan(GraphUtilities::toRadians(135.0f)) * x;
}

float roseCurve(float theta)
{
    return 1.0f * cos(3.0f * theta);
}

float cardroid(float theta)
{
    return 7.0f * (1.0f + cos(theta));
}

std::pair<float, float> sunflowerPoint(float i) {
    float c = 1.0f;
    constexpr float GOLDEN_ANGLE = 2.39996323f; // radians
    float theta = i * GOLDEN_ANGLE;
    float r = c * std::sqrt(i);
    float x = r * std::cos(theta);
    float y = r * std::sin(theta);
    return {x, y};
}

float randomFunction(float t){
    return 3.0f + 5.0f * cos(t);
}


int main()
{

    Graph *graph = Graph::getInstance();
    // Graph *graph2 = Graph::getInstance();

    //**************************************Logic to Draw Functions Starts Here**********************************************/

    // Functions to draw in Cartisian Coordinate System
    // graph->insertVertices({line, [](float x){return x * x;}});
    // graph->insertVertices([](float x){return sin(x);});
     graph->insertVertices({Functions::explodedTrigFusion2});

    // Functions to draw in Radia Coordinate System
     graph->insertVerticesRadians([](float r){return 4.0f;});

    // Functions to draw in Parametric Coordinate System
    // graph->insertVerticesParametric(sunflowerPoint, 0.0f, 20.0f);
    // graph->insertVerticesParametric(sunflowerPoint, 0.0f, 1.0f);
    graph->insertVerticesParametric({Functions::lissajous, Functions::hypotrochoid, Functions::heartCurve}, 0.0f, 8.0f);

    //***************************************Logic to draw Functions Ends Here*************************************************/

    graph->run();

    return 0;
}
