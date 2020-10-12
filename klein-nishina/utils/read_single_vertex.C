#include "Event.h"

void read_single_vertex()
{
    TFile* input_file  = new TFile("../build/500evts_post.root", "open");
    TTree* tree_events = (TTree*)input_file->Get("events");

    int event_id  = 0;
    int vertex_id = 0;

    Event* event = nullptr;
    tree_events->SetBranchAddress("event", &event);
    tree_events->GetEntry(event_id);

    cout << "=================================" << endl;
    cout << "Event  " << event->event_id << endl;
    cout << "Start  " << event->start_x << ", " << event->start_y << ", "
         << event->start_z << endl;
    cout << "Dir    " << event->start_dir_x << ", " << event->start_dir_y
         << ", " << event->start_dir_z << endl;
    cout << "Energy " << event->start_energy << endl;
    cout << "=================================" << endl;

    cout << "---------------------------------" << endl;

    cout << "  Vertex " << vertex_id << ": " << event->vertices.at(vertex_id).x
         << ", " << event->vertices.at(vertex_id).y << ", "
         << event->vertices.at(vertex_id).z << endl;
    cout << "  Step lenght " << event->vertices.at(vertex_id).step_length
         << endl;
    cout << "  Primary " << event->vertices.at(vertex_id).primary.pdg << endl;
    cout << "    dir: ";
    cout << event->vertices.at(vertex_id).primary.dir_x << ", ";
    cout << event->vertices.at(vertex_id).primary.dir_y << ", ";
    cout << event->vertices.at(vertex_id).primary.dir_z << endl;
    cout << "    E_k: " << event->vertices.at(vertex_id).primary.energy << endl;
    cout << "  Secondary " << event->vertices.at(vertex_id).secondary.pdg
         << endl;
    cout << "    dir: ";
    cout << event->vertices.at(vertex_id).secondary.dir_x << ", ";
    cout << event->vertices.at(vertex_id).secondary.dir_y << ", ";
    cout << event->vertices.at(vertex_id).secondary.dir_z << endl;
    cout << "    E_k: " << event->vertices.at(vertex_id).secondary.energy
         << endl;
    cout << "=================================" << endl;

    cout << "total energy = "
         << event->vertices.at(vertex_id).primary.energy
                + event->vertices.at(vertex_id).secondary.energy
         << endl;
}