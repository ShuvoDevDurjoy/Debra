#include "graph.hpp"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template <typename... T>
void Graph::insertVertices(CartisanFunctionTypeVariant fn, T... t)
{
    float r = 0.2f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    float g = 0.2f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    float b = 0.2f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    GraphColor *gc = new GraphColor(r, g, b);
    singletonGraph graph;
    graph.setColor(gc);
    graph.setAnimationMode(Graph::ANIMATION_MODE);
    Var v;
    addToVar(v, t...);
    float i;
    int count = 0;
    for (i = -(steps / 2.0f) - 1.0f; i <= (steps / 2.0f) + 1.0f; i += 0.01f)
    {
        float x = normalizeX(i);
        float y;
        count++;
        std::visit([&](auto &&fn_inner)
                   {
            using FnType = std::decay_t<decltype(fn_inner)>;
            if constexpr (std::is_invocable_v<FnType, float, Var>)
            {
                y = fn_inner(i, v);
            }
            else if constexpr (std::is_invocable_v<FnType, float>)
            {
                y = fn_inner(i);
            }
            else
            {
                static_assert(false, "Function must be callable with float or (float, Var)");
            } }, fn);
        y = normalizeY(y);
        graph.add({x, y});
    }
    graph.setRangeSize(cartisanReset);
    graphs.push_back(graph);
}

template <typename... T>
void Graph::insertVerticesRadians(RadianFunctionTypeVariant fn, float s, float f, T... t)
{
    float r = 0.2f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    float g = 0.2f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    float b = 0.2f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    GraphColor *gc = new GraphColor(r, g, b);
    singletonGraph graph;
    graph.setColor(gc);
    graph.setAnimationMode(Graph::ANIMATION_MODE);
    Var v;
    addToVar(v, t...);
    float step = GraphUtilities::toRadians(0.010f);
    for (float i = s * M_PI; i <= f * M_PI; i += step)
    {
        float radius;

        std::visit([&](auto &&fn_inner)
                   {
            using FnType = std::decay_t<decltype(fn_inner)>;
            if constexpr (std::is_invocable_v<FnType, float, Var>)
            {
                radius = fn_inner(i, v);
            }
            else if constexpr (std::is_invocable_v<FnType, float>)
            {
                radius = fn_inner(i);
            }
            else
            {
                static_assert(false, "Function must be callable with float or (float, Var)");
            } }, fn);

        float x = normalizeX(radius * cos(i));
        float y = normalizeY(radius * sin(i));
        graph.add({x, y});
    }
    // int radianSteps = (((f * M_PI - s * M_PI) / (step * 1.0f)) / (speed * 1.0f));
    int radianSteps = 2 * (((f * M_PI - s * M_PI + 1.0f ) / step ) / speed);
    radianSteps = radianSteps < 1 ? 1 : radianSteps;
    graph.setRangeSize(radianSteps);
    graphs.push_back(graph);
}

template <typename... T>
void Graph::insertVerticesList(CartesianFunctionList fns, T... t)
{
    for (auto &fn : fns.funcs)
    {
        insertVertices(fn, t...);
    }
}

template <typename... T>
void Graph::insertVerticesRadiansList(RadianFunctionList fns, float minRange, float maxRange, T... t)
{
    for (auto &fn : fns.funcs)
    {
        insertVerticesRadians(fn, minRange, maxRange, t...);
    }
}

template <typename... T>
void Graph::insertVerticesParametric(ParametricFunctionTypeVariant fn, float minRange, float maxRange, T... t)
{
    float r = 0.2f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    float g = 0.2f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    float b = 0.2f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    GraphColor *gc = new GraphColor(r, g, b);
    singletonGraph graph;
    graph.setColor(gc);
    graph.setAnimationMode(Graph::ANIMATION_MODE);
    Var v;
    addToVar(v, t...);

    float step = 0.001f;
    for (float t = minRange * M_PI; t <= maxRange * M_PI; t += step)
    {
        std::pair<float, float> pt;
        std::visit([&](auto &&fn_inner)
                   {
            using FnType = std::decay_t<decltype(fn_inner)>;
            if constexpr (std::is_invocable_v<FnType, float, Var>)
            {
                pt = fn_inner(t, v);
            }
            else if constexpr (std::is_invocable_v<FnType, float>)
            {
                pt = fn_inner(t);
            }
            else
            {
                static_assert(false, "Function must be callable with float or (float, Var)");
            } }, fn);
        float x = normalizeX(pt.first);
        float y = normalizeY(pt.second);
        graph.add({x, y});
    }

    int parametricSteps = (((maxRange * M_PI - minRange * M_PI) / (step * 1.0f)) / (speed * 1.0f));
    parametricSteps = parametricSteps < 1 ? 1 : parametricSteps;
    graph.setRangeSize(parametricSteps);
    graphs.push_back(graph);
}

template <typename... T>
void Graph::insertVerticesParametricList(ParametricFunctionList fns, float minRange, float maxRange, T... t)
{
    for (auto &fn : fns.funcs)
    {
        insertVerticesParametric(fn, minRange, maxRange, t...);
    }
}