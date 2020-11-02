#include "xmas_light.h"

#include <iostream>

int main()
{
    LightManager mgr;

    mgr.openLightWithRange({0, 0}, {999, 999});
    mgr.switchLightWithRange({0, 0}, {999, 0});
    mgr.closeLightWithRange({499, 499}, {500, 500});

    std::cout << mgr.countOpenLight() << '\n';
}