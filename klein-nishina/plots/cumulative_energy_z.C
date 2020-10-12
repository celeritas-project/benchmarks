

#include "../utils/Event.h"

void cumulative_energy_z()
{
    // Open ROOT file and TTree
    TFile* input = new TFile("../build/100MeV_10k_evts_post.root", "read");
    TTree* tree_events = (TTree*)input->Get("events");

    Event* event = nullptr;
    tree_events->SetBranchAddress("event", &event);

    // Define binning
    double z_bin_size = 10;
    double z_bin_min  = 0;
    double z_bin_max  = 2000;

    int    z_n_bins = (int)((z_bin_max - z_bin_min) / z_bin_size);
    double z_bins[z_n_bins][2];
    double z_bins_center[z_n_bins];
    double cumulative_E_per_z_bin[z_n_bins];

    // Set bin min, max and center values
    for (int i = 0; i < z_n_bins; i++)
    {
        z_bins[i][0]     = i * z_bin_size;
        z_bins[i][1]     = i * z_bin_size + z_bin_size;
        z_bins_center[i] = i * z_bin_size + (z_bins[i][1] - z_bins[i][0]) / 2;

        cumulative_E_per_z_bin[i] = 0;
    }

    // Loop over entries
    for (int entry = 0; entry < tree_events->GetEntries(); entry++)
    {
        cout << "\r" << entry << " of " << tree_events->GetEntries() << flush;
        tree_events->GetEntry(entry);

        // Loop over vertices
        for (auto vertex : event->vertices)
        {
            double z = vertex.z;

            // Save energy value in the correct bin
            for (int i = 0; i < z_n_bins; i++)
            {
                if (z_bins[i][0] < z && z_bins[i][1] > z)
                {
                    cumulative_E_per_z_bin[i] += vertex.secondary.energy;
                }
            }
        }
    }
    cout << endl;

    TGraph* gr = new TGraph();

    double total_energy = 0;

    // Normalize bin by number of events
    // Accumulate energy of all previous bins and add data point to the plot
    for (int i = 0; i < z_n_bins; i++)
    {
        cumulative_E_per_z_bin[i] /= (tree_events->GetEntries() - 1);
        total_energy += cumulative_E_per_z_bin[i];
        gr->SetPoint(i, z_bins_center[i], total_energy);
    }

    // Open canvas and draw the graph
    TCanvas* c = new TCanvas("", "", 700, 500);
    gr->SetLineColor(9);
    gr->SetLineWidth(3);
    gr->GetXaxis()->SetTitle("z [cm]");
    gr->GetXaxis()->CenterTitle();
    gr->GetYaxis()->SetTitle("Cumulative energy deposition [MeV]");
    gr->GetYaxis()->CenterTitle();
    gr->GetYaxis()->SetRangeUser(0, 110);
    gr->Draw("AC");
}
