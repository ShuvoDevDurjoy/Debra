#include "graph.hpp"
#include "functions.cpp"

float line(float x)
{
    return x * tan(GraphUtilities::toRadians(27.2f));
}
float line3(float x)
{
    return x * tan(GraphUtilities::toRadians(-27.2f));
}
float line2(float x, Var v)
{
    return v[0] + tan(GraphUtilities::toRadians(135.0f)) * x;
}

float roseCurve(float theta)
{
    return cos(theta);
}

float cardroid(float theta)
{
    return 3.0f * (1.0f + 2.0f * cos(theta));
}
float cardroid2(float theta)
{
    return 3.0f * (1.0f - 2.0f * cos(theta));
}
float cardroid3(float theta)
{
    return 3.0f * (1.0f + 2.0f * sin(theta));
}
float cardroid4(float theta)
{
    return 3.0f * (1.0f - 2.0f * sin(theta));
}

std::pair<float, float> sunflowerPoint(float i)
{
    float c = 1.0f;
    constexpr float GOLDEN_ANGLE = 2.39996323f; // radians
    float theta = GraphUtilities::toRadians(i * 16.0f * GOLDEN_ANGLE);
    float r = c * std::sqrt(i);
    float x = r * std::cos(theta);
    float y = r * std::sin(theta);
    return {x, y};
}

float randomFunction(float t)
{
    return 2.0f * sin(t) + 2.0f * cos(t) + 2.0f * tan(t) + t * cos(t) + tan(t) * t - cos(t) / tan(t);
}

float para2(float t)
{
    return (cos(t) * tan(t) * sin(t) + t) / (t / 4.0f);
}

std::pair<float, float> para(float t)
{
    float x = (cos(t) * tan(t) * sin(t) * t);
    float y = 5.0f + ((sin(t) + tan(t) / cos(t) - tan(t)) / 600.0f * tan(t) * sin(t) * cos(t) + cos(t) + tan(t)) * 6.0f / 5.0f - 6.0f + 9.0f / tan(t);
    return {x, y};
}

// // float a = 27.28f;

float para3(float x, Var v)
{
    // return ( x / 4.0f) +  10.0f * pow(cos((1.0f - (1.0f / a)) * a * x), 2.0f);
    return v[1] * pow(cos((1.0f - (1.0f / v[0])) * x), 2.0f);
}

float fn(float x)
{
    float y = 2.0f;
    return y;
}

int main()
{
    Graph *graph = Graph::getInstance(1.0f);
    // Graph *graph2 = Graph::getInstance(4.0f);
    // graph->speed = 200;
    // Graph *graph2 = Graph::getInstance();

    //**************************************Logic to Draw Functions Starts Here**********************************************/

    // Functions to draw in Cartisian Coordinate System
    // graph->insertVerticesParametric(Functions::chaoticFractal, 0.0f, 5.0f, 8.0f);
    // graph->insertVerticesRadians(para3, 0.0f, 30.0f, 30.0f, 13.0f);
    // graph->insertVertices(fn, 2.0f);
    // graph->insertVertices({TO_CARTISAN_VARIANT(line2), TO_CARTISAN_VARIANT(line3)});

    // CartesianFunctionList t{line2, line3};
    // graph->insertVertices(roseCurve);

    graph->setAnimationMode(AnimationMode::ONCE);
    graph->insertVertices([](float x){return x * x;});
    // graph->insertVertices([](float x){return 15.0f;});
    // graph->insertVertices([](float x){return -15.0f;});
    graph->setAnimationMode(AnimationMode::INFINITE);
    graph->insertVertices([](float x){return Functions::sineModulated(x);});
    // graph->insertVertices([](float x){return x * x * x;});
    // graph->insertVertices([](float x){return cos(x);});
    // graph->insertVertices([](float x){return 1.0f / (3.0f * tan(x));});
    // graph->insertVertices([](float x){return tan(x);});
    
    // Functions to draw in Radia Coordinate System
    //  graph->insertVerticesRadians([](float r){return 9.0f;});
    // for (float a = 10.0f; a <= 70.0f; a+= 10.0f){
        // }
        // graph->insertVerticesRadians(para3, 0.0f, 10.0f, 10.0f, 13.0f);
        // graph->insertVerticesRadians(para3, 0.0f, 20.0f, 20.0f, 13.0f);
        // graph->insertVerticesRadians(para3, 0.0f, 30.0f, 30.0f, 13.0f);
        // graph->insertVerticesRadians(para3, 0.0f, 40.0f, 40.0f, 13.0f);
        // graph->insertVerticesRadians(para3, 0.0f, 50.0f, 50.0f, 13.0f);
        // graph->insertVerticesRadians(para3, 0.0f, 60.0f, 60.0f, 13.0f);
        // graph->insertVerticesRadians(para3, 0.0f, 70.0f, 70.0f, 13.0f);
        // graph->insertVerticesParametric(para, -25.0f, 25.0f);
        // RadianFunctionList rt{cardroid, cardroid2, cardroid3, cardroid4};
        // float a = 30.0f;
        // graph->insertVerticesRadiansList(rt);
        // graph->insertVertices({Functions::explodedTrigFusion2});
        // graph->insertVerticesRadians(para3, 0.0f, a, a, 13.0f);
        // graph->insertVerticesParametric(Functions::maurerRose, 0.0f, 2.0f, 10.0f, 14.0f);
        // graph->insertVerticesRadians([](float x){ return 14.0f; });
        
        // Functions to draw in Parametric Coordinate System
        //  graph->insertVerticesParametric(sunflowerPoint, 0.0f, 350.0f);
        //  graph->insertVerticesParametric(sunflowerPoint, 0.0f, 1.0f);
        //  graph->insertVerticesParametric(Functions::heartCurve, 0.0f, 2.0f);
        //  graph->insertVerticesParametric({Functions::hypotrochoid}, 0.0f, 6.0f);
        //  graph->insertVerticesParametric(Functions::riemannProjection, 0.0f, 10.0f);
    graph->setAnimationMode(AnimationMode::ONCE);
    graph->insertVerticesRadians(Functions::graphButterfly, 0.0f, 2.0f, -4.0f);
    //  graph->insertVerticesParametric(Functions::harmonograph, 0.0f, 4.0f);
    // graph->insertVerticesParametric(Functions::lissajous, 0.0f, 2.0f);

    //***************************************Logic to draw Functions Ends Here*************************************************/

    graph->run();

    return 0;
}
