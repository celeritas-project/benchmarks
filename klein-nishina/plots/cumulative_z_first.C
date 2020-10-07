//---------------------------------------------------------------------------//
/*!
 * ROOT macro for plotting the z-cumulative interaction from gammas.
 *
 * Plots: 
 * 1 - First interaction distribution
 * 2 - Cumulative fraction of first interactions along the z-axis and compares
 *     the histogram with f = 1 - exp(-z/lambda) 
 *
 * To run this macro:
 * $ root cumulative_z_first.C
 */

void cumulative_z_first()
{
    TFile* input  = new TFile("../build/100MeV_10k_evts.root", "read");
    TTree* vertex = (TTree*)input->Get("vertex");

    TH1F* h = new TH1F("", "", 100, 0, 1000);

    int event = 0;

    for (int i = 0; i < vertex->GetEntries(); i++)
    {
        vertex->GetEntry(i);

        if (vertex->GetLeaf("pdg")->GetValue() == 22)
        {
            if (vertex->GetLeaf("event")->GetValue() == event)
            {
                vertex->GetEntry(i + 1);

                double z = vertex->GetLeaf("z")->GetValue();
                h->Fill(z);
                event++;
            }
        }
    }

    TCanvas* canvas = new TCanvas("", "", 700, 500);
    h->Draw();

    //-----------------------------------------------------------------------//

    TH1F* h2 = new TH1F("", "", 100, 0, 1000);

    for (int i = 0; i < h2->GetNbinsX(); i++)
    {
        double integral = 0;

        for (int j = 0; j <= i; j++)
        {
            integral += h->GetBinContent(j);
        }

        h2->SetBinContent(i, integral);
    }

    double norm = h->GetEntries();
    h2->Scale(1. / norm);

    TF1* f = new TF1("f", "1 - TMath::Exp(-x/200)", 0, 1000);
    f->SetLineWidth(3);

    TCanvas* canvas2 = new TCanvas("", "", 700, 500);
    h2->Draw("hist");
    f->Draw("SAME L");
}
