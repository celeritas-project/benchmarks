//---------------------------------------------------------------------------//
/*!
 * ROOT macro for plotting the z-cumulative energy deposition from gammas.
 *  
 * The kinetic energy assigned to every electron produced by the klein-nishina
 * benchmark app is considered to be deposited in the material.
 * 
 * To run this macro: 
 * $ root cumulative_z_edep.C
 */

void cumulative_z_edep()
{
    // Open ROOT file and TTree
    TFile* input  = new TFile("../build/100MeV_10k_evts.root", "read");
    TTree* vertex = (TTree*)input->Get("vertex");

    // Create graph
    TGraph* graph_z_edep = new TGraph();

    double z_limit    = 1000; // z max value
    double z_step     = 100;  // z bin size
    double z_integral = 0;    // cumulative energy
    int    i_point    = 0;    // graph i-th data point

    for (double z_i = 0; z_i < z_limit; z_i = z_i + z_step)
    {
        std::cout << "\r" << z_i << " of " << z_limit << std::flush;

        // bin limits [min, max]
        double z_bin[2] = {z_i, z_i + z_step};

        // Loop over entries
        for (int entry = 0; entry < vertex->GetEntries(); entry++)
        {
            vertex->GetEntry(entry);

            // Avoid counting any primary vertex kinetic energy at (0, 0, 0)
            if (vertex->GetLeaf("pdg")->GetValue() == 22)
                continue;

            // Fetch electron's z vertex and starting kinetic energy
            double k_energy   = vertex->GetLeaf("kinetic_energy")->GetValue();
            double z_position = vertex->GetLeaf("z")->GetValue();

            // If within bin limits, add electron energy
            if (z_position >= z_bin[0] && z_position < z_bin[1])
                z_integral += k_energy;
        }

        // Get last event number (which is the same as number of events + 1)
        vertex->GetEntry(vertex->GetEntries() - 1);
        int number_of_events = vertex->GetLeaf("event")->GetValue() + 1;

        // Get bin center, and average cumulative E deposition per particle
        double z_center       = z_bin[0] + ((z_bin[1] - z_bin[0]) / 2);
        double z_integral_avg = z_integral / number_of_events;

        // Fill graph
        graph_z_edep->SetPoint(i_point, z_center, z_integral_avg);
        i_point++;
    }
    std::cout << std::endl;

    // Open canvas
    TCanvas* canvas = new TCanvas("", "", 700, 500);

    // Edit graph and draw it in the canvas
    graph_z_edep->SetLineColor(9);
    graph_z_edep->SetLineWidth(3);
    graph_z_edep->GetXaxis()->SetTitle("z [cm]");
    graph_z_edep->GetXaxis()->CenterTitle();
    graph_z_edep->GetYaxis()->SetTitle("Cumulative energy deposition [MeV]");
    graph_z_edep->GetYaxis()->CenterTitle();
    graph_z_edep->Draw("AC");

    // canvas->SaveAs("cumulative_E_dep_10K_evts.pdf");
}