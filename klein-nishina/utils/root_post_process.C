//---------------------------------------------------------------------------//
/*!
 * ROOT Macro for post processing the geant-kn-app ROOT output file into a
 * better and more readable ROOT file.
 *
 * The new output uses an Event based structure to save data
 */

#include "Event.h"

// I/O info
std::string input_file  = "../build/500evts.root";
std::string output_file = "../build/500evts_post.root";

//---------------------------------------------------------------------------//
void root_post_process()
{
    // Input
    TFile* input       = new TFile(input_file.c_str(), "open");
    TTree* tree_step   = (TTree*)input->Get("step");
    TTree* tree_vertex = (TTree*)input->Get("vertex");

    // Output
    TFile* output      = new TFile(output_file.c_str(), "recreate");
    TTree* tree_events = new TTree("events", "events");

    Event event;
    tree_events->Branch("event", &event);

    // Set event_id < 0 before the main loop to ensure it will work
    event.event_id      = -1;
    bool is_first_event = true;

    // Vertex branch index starting value
    size_t vertex_i_start = 0;

    int current_percent = 0;
    int printed_percent = -1;

    // Loop over step entries
    for (size_t step_i = 0; step_i < tree_step->GetEntries(); step_i++)
    {
        // Print percent
        current_percent = (int)(100 * step_i) / tree_step->GetEntries();
        if (current_percent > printed_percent)
        {
            printed_percent = current_percent;
            cout << "\rCreating " << output_file << "... " << printed_percent
                 << "%" << flush;
        }

        tree_step->GetEntry(step_i);

        // Skip non-primaries. They will be stored using the vertex branch
        if (tree_step->GetLeaf("parent_id")->GetValue() != 0)
            continue;

        double step_x = tree_step->GetLeaf("x")->GetValue();
        double step_y = tree_step->GetLeaf("y")->GetValue();
        double step_z = tree_step->GetLeaf("z")->GetValue();

        // If new event is found, save event info and move to next step
        if (event.event_id != tree_step->GetLeaf("event")->GetValue())
        {
            // Fill event tree when a new event is found
            // First loop skips the Fill because event struct is still empty
            if (!is_first_event)
            {
                tree_events->Fill();
            }

            event.event_id = tree_step->GetLeaf("event")->GetValue();
            event.vertices.clear();

            // Fetch event info in the vertex tree
            for (size_t vertex_i = vertex_i_start;
                 vertex_i < tree_vertex->GetEntries();
                 vertex_i++)
            {
                tree_vertex->GetEntry(vertex_i);

                if (tree_vertex->GetLeaf("event")->GetValue() != event.event_id)
                    continue;

                if (tree_vertex->GetLeaf("pdg")->GetValue() != 22)
                    continue;

                event.start_x     = tree_vertex->GetLeaf("x")->GetValue();
                event.start_y     = tree_vertex->GetLeaf("y")->GetValue();
                event.start_z     = tree_vertex->GetLeaf("z")->GetValue();
                event.start_dir_x = tree_vertex->GetLeaf("dir_x")->GetValue();
                event.start_dir_y = tree_vertex->GetLeaf("dir_y")->GetValue();
                event.start_dir_z = tree_vertex->GetLeaf("dir_z")->GetValue();
                event.start_energy
                    = tree_vertex->GetLeaf("kinetic_energy")->GetValue();

                // Save current vertex branch index for the next time this
                // loop is called
                vertex_i_start = vertex_i;

                break;
            }

            is_first_event = false;
        }

        // If no secondaries, store vertex with no secondary
        if (tree_step->GetLeaf("secondaries")->GetValue() == 0)
        {
            Vertex vertex_eloss;
            // Vertex info
            vertex_eloss.x           = tree_step->GetLeaf("x")->GetValue();
            vertex_eloss.y           = tree_step->GetLeaf("y")->GetValue();
            vertex_eloss.z           = tree_step->GetLeaf("z")->GetValue();
            vertex_eloss.step_length = tree_step->GetLeaf("length")->GetValue();
            // Primary
            vertex_eloss.primary.pdg = tree_step->GetLeaf("pdg")->GetValue();
            vertex_eloss.primary.energy
                = tree_step->GetLeaf("kinetic_energy")->GetValue();
            vertex_eloss.primary.dir_x
                = tree_step->GetLeaf("dir_x")->GetValue();
            vertex_eloss.primary.dir_y
                = tree_step->GetLeaf("dir_y")->GetValue();
            vertex_eloss.primary.dir_z
                = tree_step->GetLeaf("dir_z")->GetValue();
            // No secondary, but store energy_loss
            vertex_eloss.secondary.pdg   = 0;
            vertex_eloss.secondary.dir_x = 0;
            vertex_eloss.secondary.dir_y = 0;
            vertex_eloss.secondary.dir_z = 0;
            vertex_eloss.secondary.energy
                = tree_step->GetLeaf("energy_loss")->GetValue();

            // Save vertex to this event
            event.vertices.push_back(vertex_eloss);
        }

        // If there is a secondary
        else
        {
            Vertex vertex;
            // Vertex info
            vertex.x           = step_x;
            vertex.y           = step_y;
            vertex.z           = step_z;
            vertex.step_length = tree_step->GetLeaf("length")->GetValue();
            // Primary info
            vertex.primary.pdg   = tree_step->GetLeaf("pdg")->GetValue();
            vertex.primary.dir_x = tree_step->GetLeaf("dir_x")->GetValue();
            vertex.primary.dir_y = tree_step->GetLeaf("dir_y")->GetValue();
            vertex.primary.dir_z = tree_step->GetLeaf("dir_z")->GetValue();
            vertex.primary.energy
                = tree_step->GetLeaf("kinetic_energy")->GetValue();

            // Find secondary from vertex tree, which is much shorter
            for (size_t vertex_i = vertex_i_start;
                 vertex_i < tree_vertex->GetEntries();
                 vertex_i++)
            {
                tree_vertex->GetEntry(vertex_i);

                // Skip unwanted events
                if (tree_vertex->GetLeaf("event")->GetValue() < event.event_id)
                    continue;

                // Skip vertex from primary
                if (tree_vertex->GetLeaf("pdg")->GetValue() == 22)
                    continue;

                double this_x = tree_vertex->GetLeaf("x")->GetValue();
                double this_y = tree_vertex->GetLeaf("y")->GetValue();
                double this_z = tree_vertex->GetLeaf("z")->GetValue();

                // If secondary is found, save it, stop searching
                if (this_x == vertex.x && this_y == vertex.y
                    && this_z == vertex.z)
                {
                    vertex.secondary.pdg
                        = tree_vertex->GetLeaf("pdg")->GetValue();
                    vertex.secondary.dir_x
                        = tree_vertex->GetLeaf("dir_x")->GetValue();
                    vertex.secondary.dir_y
                        = tree_vertex->GetLeaf("dir_y")->GetValue();
                    vertex.secondary.dir_z
                        = tree_vertex->GetLeaf("dir_z")->GetValue();
                    vertex.secondary.energy
                        = tree_vertex->GetLeaf("kinetic_energy")->GetValue();

                    event.vertices.push_back(vertex);
                    break;
                }

            } // end for (vertex)

        } // end else

    } // end for (step)

    // Fill very last event
    tree_events->Fill();

    cout << "\rCreating " << output_file << "... Done!" << endl;

    tree_events->Write();
    output->Write();
    output->Close();
    input->Close();
}
