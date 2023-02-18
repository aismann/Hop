#ifndef MARCHINGWORLD_H
#define MARCHINGWORLD_H

#include <World/world.h>

#include <World/perlinSource.h>

class MarchingWorld : public World 
{

public:

    MarchingWorld(
        uint64_t s, 
        OrthoCam & c, 
        uint64_t renderRegion, 
        uint64_t dynamicsShell,
        MapSource * f,
        Boundary * b
    );

    void save(std::string filename){}
    void load(std::string filename){}

    void worldToTileData(float x, float y, Tile & h, float & x0, float & y0, float & s);
    Tile tileType(int & i, int & j);
    void tileToIdCoord(int ix, int iy, int & i, int & j);
    void updateRegion(float x, float y);

private:

    const uint64_t RENDER_REGION_BUFFER_SIZE, DYNAMICS_REGION_BUFFER_SIZE;
    const uint64_t RENDER_REGION_START;

    std::unique_ptr<bool[]> renderRegionBuffer;
    std::unique_ptr<bool[]> renderRegionBackBuffer;

    void processBufferToOffsets();

};

#endif /* MARCHINGWORLD_H */