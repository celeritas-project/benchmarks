#include "../utils/Event.h"

#include <sstream>

TGraph* text_input_graph(ifstream& input);

//---------------------------------------------------------------------------//
double cutoff_err = 0.02;
double x_min = -25;
double x_max = 1050;
double y_min = -0.025;
double y_max = 0.6;

void g4_celeritas_edep()
{
    // Celeritas input
    ifstream celeritas_input("../celeritas-results/edep-100MeV-1.0cm.txt");
    ifstream g4_kn_app_input("g4-edep-100MeV-60k-1.0cm.txt");

    // Create graphs
    TGraph* gr_celeritas    = text_input_graph(celeritas_input);
    TGraph* gr_g4_kn_app    = text_input_graph(g4_kn_app_input);
    TGraph* gr_relative_err = new TGraph();

    // Calculate relative error and add points to the graph
    for (size_t i = 0; i < gr_celeritas->GetN(); i++)
    {
        double celeritas_bin_y = gr_celeritas->GetPointY(i);
        double g4_kn_app_bin_y = gr_g4_kn_app->GetPointY(i);
        double rel_err = (celeritas_bin_y - g4_kn_app_bin_y) / g4_kn_app_bin_y;
        rel_err        = TMath::Abs(rel_err) * 100;

        if (celeritas_bin_y < cutoff_err || g4_kn_app_bin_y < cutoff_err)
            rel_err = 0;

        gr_relative_err->SetPoint(i, i, rel_err);
    }

    double rel_err_avg = 0;
    int    n_points    = 0;
    for (size_t i = 0; i < gr_relative_err->GetN(); i++)
    {
        if (gr_relative_err->GetPointY(i) > 0)
        {
            rel_err_avg += gr_relative_err->GetPointY(i);
            n_points++;
        }
    }
    rel_err_avg /= n_points;

    // Close txt input files
    celeritas_input.close();
    g4_kn_app_input.close();

    // Set plot colors
    // Celeritas
    gr_celeritas->SetLineColorAlpha(9, 0.9);
    gr_celeritas->SetLineWidth(1);
    // Geant4-kn-app
    gr_g4_kn_app->SetLineColor(2);
    gr_g4_kn_app->SetLineWidth(1);

    // Plot comparison

    // Draw only overlay
    TCanvas* c1 = new TCanvas("", "", 700, 500);
    gr_g4_kn_app->GetXaxis()->SetTitle("z [cm]");
    gr_g4_kn_app->GetXaxis()->CenterTitle();
    gr_g4_kn_app->GetYaxis()->SetTitle("Energy deposition [MeV/cm]");
    gr_g4_kn_app->GetYaxis()->SetTitleOffset(0.8);
    gr_g4_kn_app->GetYaxis()->CenterTitle();
    gr_g4_kn_app->GetXaxis()->SetLimits(x_min, x_max);
    gr_g4_kn_app->GetYaxis()->SetRangeUser(y_min, y_max);
    gr_g4_kn_app->Draw("AL");
    gr_celeritas->Draw("same");

    // Plot legend
    TLegend* legend = new TLegend(0.52, 0.67, 0.77, 0.83);
    legend->SetTextSize(0.035);
    legend->SetHeader("Geant4 / Celeritas comparison", "L");
    legend->AddEntry(gr_celeritas, "Celeritas", "l");
    legend->AddEntry(gr_g4_kn_app, "Geant4", "l");
    legend->Draw();

    // Draw overaly and relative error
    TCanvas* c2 = new TCanvas("", "", 700, 500);
    c2->Divide(1, 2);

    // Create top pad
    TPad* pad1 = new TPad("pad1", "", 0.0, 0.3, 1.0, 1.0);
    pad1->SetBottomMargin(0.02);
    pad1->Draw();
    pad1->cd();

    // Add graphs to the top pad
    TMultiGraph* mgr_top = new TMultiGraph();
    mgr_top->Add(gr_g4_kn_app);
    mgr_top->Add(gr_celeritas);
    mgr_top->Draw("AL");

    mgr_top->GetYaxis()->SetTitle("Energy deposition [MeV/cm]");
    mgr_top->GetYaxis()->SetTitleOffset(0.8);
    mgr_top->GetYaxis()->CenterTitle();
    mgr_top->GetXaxis()->SetLabelOffset(99);
    mgr_top->GetXaxis()->SetLimits(x_min, x_max);
    mgr_top->GetYaxis()->SetRangeUser(y_min, y_max);

    // Draw legend
    legend->Draw();

    TLine* cutoff_line = new TLine(x_min, cutoff_err, x_max, cutoff_err);
    cutoff_line->SetLineStyle(3);
    cutoff_line->SetLineColor(15);
    cutoff_line->Draw();

    // Refresh axis
    pad1->RedrawAxis();

    // Move back to top canvas
    c2->cd();

    // Create bottom pad
    TPad* pad2 = new TPad("pad2", "", 0.0, 0.0, 1.0, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.4);
    pad2->SetGridx();
    pad2->Draw();
    pad2->cd();

    // Wat
    gr_relative_err->GetXaxis()->SetTitle("z [cm]");
    gr_relative_err->GetXaxis()->SetTitleSize(0.13);
    gr_relative_err->GetXaxis()->CenterTitle();
    gr_relative_err->GetXaxis()->SetLabelSize(0.1);
    gr_relative_err->GetXaxis()->SetLabelOffset(0.05);
    gr_relative_err->GetXaxis()->SetTitleOffset(1.2);
    gr_relative_err->GetXaxis()->SetLimits(x_min, x_max);
    gr_relative_err->GetYaxis()->SetTitle("Rel. Err (%)");
    gr_relative_err->GetYaxis()->SetTitleSize(0.11);
    gr_relative_err->GetYaxis()->CenterTitle();
    gr_relative_err->GetYaxis()->SetTitleOffset(0.35);
    gr_relative_err->GetYaxis()->SetLabelSize(0.1);
    gr_relative_err->Draw("AL");

    TLine* avg_line = new TLine(x_min, rel_err_avg, x_max, rel_err_avg);
    avg_line->SetLineStyle(7);
    avg_line->SetLineWidth(2);
    avg_line->SetLineColor(8);
    avg_line->Draw();

    // Refresh axis
    pad2->RedrawAxis();
}

//---------------------------------------------------------------------------//
TGraph* text_input_graph(ifstream& input)
{
    TGraph* graph = new TGraph();

    // Loop over data
    int         data_point_i = 0;
    std::string line;
    while (std::getline(input, line))
    {
        istringstream line_data(line);

        double value;
        line_data >> value;

        graph->SetPoint(data_point_i, data_point_i, value);
        data_point_i++;
    }

    return graph;
}