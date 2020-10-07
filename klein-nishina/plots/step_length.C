//---------------------------------------------------------------------------//
/*!
 * ROOT macro for plotting the step length
 *
 * To run this macro:
 * $ root step_length.C
 */

//---------------------------------------------------------------------------//
struct position
{
    double x;
    double y;
    double z;
};

//---------------------------------------------------------------------------//
double distance(position& start, position& end)
{
    double delta_x_sq = TMath::Power((end.x - start.x), 2);
    double delta_y_sq = TMath::Power((end.y - start.y), 2);
    double delta_z_sq = TMath::Power((end.z - start.z), 2);

    return TMath::Sqrt(delta_x_sq + delta_y_sq + delta_z_sq);
}

//---------------------------------------------------------------------------//
void step_length()
{
    TFile* input     = new TFile("../build/100MeV_10k_evts.root", "read");
    TTree* tree_step = (TTree*)input->Get("step");

    TH1F* hist_step = new TH1F("step length", "", 100, 0, 1000);

    int currentPercent = 0;
    int printedPercent = 0;

    int total_entries = tree_step->GetEntries();

    for (size_t i = 1; i < total_entries; ++i)
    {
        // Print percent
        currentPercent = (int)(100 * i) / total_entries;
        if (currentPercent > printedPercent)
        {
            printedPercent = currentPercent;
            cout << "\r" << printedPercent << "%" << flush;
        }

        // start position
        tree_step->GetEntry(i - 1);

        position start;
        start.x = tree_step->GetLeaf("x")->GetValue();
        start.y = tree_step->GetLeaf("y")->GetValue();
        start.z = tree_step->GetLeaf("z")->GetValue();

        // end position
        tree_step->GetEntry(i);

        position end;
        end.x = tree_step->GetLeaf("x")->GetValue();
        end.y = tree_step->GetLeaf("y")->GetValue();
        end.z = tree_step->GetLeaf("z")->GetValue();

        // Fill histogram
        hist_step->Fill(distance(start, end));
    }
    cout << "\r100%" << endl;

    // Draw histogram
    TCanvas* canvas = new TCanvas("", "", 700, 500);
    hist_step->GetXaxis()->SetTitle("step length [cm]");
    hist_step->GetXaxis()->CenterTitle();
    hist_step->Draw();

    // Save plot
    canvas->SaveAs("step_length.pdf");
}
