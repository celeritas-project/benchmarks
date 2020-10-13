

#include "../utils/Event.h"

void energy_deposition_per_bin()
{
    // Open ROOT file and TTree
    TFile* input = new TFile("../build/60k_evts_post.root", "read");
    TTree* tree_events = (TTree*)input->Get("events");

    Event* event = nullptr;
    tree_events->SetBranchAddress("event", &event);

    // Define binning
    double z_bin_size = 1;
    double z_bin_min  = 0;
    double z_bin_max  = 2000;

    int    z_n_bins = (int)((z_bin_max - z_bin_min) / z_bin_size);
    double z_bins[z_n_bins][2];
    double z_bins_center[z_n_bins];
    double energy_per_z_bin[z_n_bins];

    // Set bin min, max and center values
    for (int i = 0; i < z_n_bins; i++)
    {
        z_bins[i][0]     = i * z_bin_size;
        z_bins[i][1]     = i * z_bin_size + z_bin_size;
        z_bins_center[i] = i * z_bin_size + (z_bins[i][1] - z_bins[i][0]) / 2;

        energy_per_z_bin[i] = 0;
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
                    energy_per_z_bin[i] += vertex.secondary.energy;
                }
            }
        }
    }
    cout << endl;

    TGraph* gr = new TGraph();

    double total_energy = 0;


    ofstream output("g4-edep-100MeV-60k-1.0cm.txt");

    // Normalize bin by number of events
    // Normalize by bin size to get MeV/cm
    // Add data point to the plot
    for (int i = 0; i < z_n_bins; i++)
    {
        energy_per_z_bin[i] /= tree_events->GetEntries() - 1;
        energy_per_z_bin[i] /= z_bin_size;

        output << energy_per_z_bin[i] << endl;

        gr->SetPoint(i, z_bins_center[i], energy_per_z_bin[i]);
    }

    output.close();

    // Open canvas and draw the graph
    TCanvas* c = new TCanvas("", "", 700, 500);
    gr->SetLineColor(9);
    gr->SetLineWidth(3);
    gr->GetXaxis()->SetTitle("z [cm]");
    gr->GetXaxis()->CenterTitle();
    gr->GetYaxis()->SetTitle("Energy deposition [MeV/cm]");
    gr->GetYaxis()->SetTitleOffset(0.8);
    gr->GetYaxis()->CenterTitle();
    gr->GetXaxis()->SetLimits(-50, 2100);
    gr->GetYaxis()->SetRangeUser(-0.05, 0.7);
    gr->Draw("AL");
}
