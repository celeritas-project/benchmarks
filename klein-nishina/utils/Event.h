#include <vector>

struct Particle
{
    int    pdg;
    double energy;
    double dir_x;
    double dir_y;
    double dir_z;
};

struct Vertex
{
    double   x;
    double   y;
    double   z;
    double   step_length;
    Particle primary;
    Particle secondary;
};

struct Event
{
    int                 event_id;
    double              start_x;
    double              start_y;
    double              start_z;
    double              start_dir_x;
    double              start_dir_y;
    double              start_dir_z;
    double              start_energy;
    std::vector<Vertex> vertices;
};