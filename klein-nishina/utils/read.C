#include "Event.h"

void read()
{
    TFile* input_file  = new TFile("../build/out_post.root", "open");
    TTree* tree_events = (TTree*)input_file->Get("events");

    Event* event = nullptr;
    tree_events->SetBranchAddress("event", &event);

    for (int i = 0; i < tree_events->GetEntries(); i++)
    {
        tree_events->GetEntry(i);

        // Hard stop after 1 event
        if (event->event_id == 1)
            break;

        cout << "=================================" << endl;
        cout << "Event  " << event->event_id << endl;
        cout << "Start  " << event->start_x << ", " << event->start_y << ", "
             << event->start_z << endl;
        cout << "Dir    " << event->start_dir_x << ", " << event->start_dir_y
             << ", " << event->start_dir_z << endl;
        cout << "Energy " << event->start_energy << endl;
        cout << "=================================" << endl;

        for (int i = 0; i < event->vertices.size(); i++)
        {
            if (event->vertices.at(i).secondary.pdg == 0)
                continue;

            cout << "---------------------------------" << endl;

            cout << "  Vertex " << i << ": " << event->vertices.at(i).x << ", "
                 << event->vertices.at(i).y << ", " << event->vertices.at(i).z
                 << endl;
            cout << "  Step lenght " << event->vertices.at(i).step_length
                 << endl;
            cout << "  Primary " << event->vertices.at(i).primary.pdg << endl;
            cout << "    dir: ";
            cout << event->vertices.at(i).primary.dir_x << ", ";
            cout << event->vertices.at(i).primary.dir_y << ", ";
            cout << event->vertices.at(i).primary.dir_z << endl;
            cout << "    E_k: " << event->vertices.at(i).primary.energy << endl;
            cout << "  Secondary " << event->vertices.at(i).secondary.pdg
                 << endl;
            cout << "    dir: ";
            cout << event->vertices.at(i).secondary.dir_x << ", ";
            cout << event->vertices.at(i).secondary.dir_y << ", ";
            cout << event->vertices.at(i).secondary.dir_z << endl;
            cout << "    E_k: " << event->vertices.at(i).secondary.energy
                 << endl;
        }
        cout << "=================================" << endl;
    }
}