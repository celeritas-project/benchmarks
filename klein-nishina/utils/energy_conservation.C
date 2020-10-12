

#include "../utils/Event.h"


void pre_processed();

void energy_conservation()
{
    // Open ROOT file and TTree
    TFile* input   = new TFile("../build/500evts_post.root", "read");
    TTree* tree_kn = (TTree*)input->Get("kn_interaction");

    Event* event = nullptr;
    tree_kn->SetBranchAddress("event", &event);

    // Loop over entries
    for (int entry = 0; entry < tree_kn->GetEntries(); entry++)
    {
        tree_kn->GetEntry(entry);

        cout << "EVENT " << event->event_id << endl;
        cout << "EVENT E " << event->start_energy << endl;

        double E_electrons_E_loss = 0;

        int n_vertices = event->vertices.size();

        for (int j = 0; j < n_vertices; j++)
        {
            E_electrons_E_loss += event->vertices.at(j).secondary.energy;
        }

        double gamma_final_E
            = event->vertices.at(n_vertices - 1).primary.energy;
        cout << "  Total: " << E_electrons_E_loss + gamma_final_E << endl;
    }

    //pre_processed();
}

void pre_processed()
{
    // Open ROOT file and TTree
    TFile* input     = new TFile("../build/out.root", "read");
    TTree* tree_step = (TTree*)input->Get("step");

    double totalE = 0;

    for (int i = 0; i < tree_step->GetEntries(); i++)
    {
        tree_step->GetEntry(i);

        if (tree_step->GetLeaf("pdg")->GetValue() == 22)
        {
            if (tree_step->GetLeaf("secondaries")->GetValue() == 0)
            {
                totalE += tree_step->GetLeaf("energy_loss")->GetValue();
            }
        }

        else
        {
            totalE += tree_step->GetLeaf("kinetic_energy")->GetValue();
        }
    }

    cout << "total -- " << totalE << endl;
}