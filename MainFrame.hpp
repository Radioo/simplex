//
// Created by Radio on 04/04/2024.
//

#ifndef SIMPLEX_MAINFRAME_HPP
#define SIMPLEX_MAINFRAME_HPP

#include <wx/wx.h>
#include <cstdint>

#include "SimplexSolver.cpp"

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    wxBoxSizer* mainVSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* tableVSizer = new wxBoxSizer(wxVERTICAL);

    std::uint8_t rows = 2;
    std::uint8_t columns = 2;

    std::vector<wxTextCtrl*> lastInputs;
    std::vector<wxTextCtrl*> FxInput;
    wxTextCtrl** lastInput;
    wxTextCtrl*** textCtrl;
    wxComboBox** operatorCombo;
    wxComboBox* operatorMinMax;

    std::vector<std::vector<wxTextCtrl*>> table;

    void incrementRows(wxCommandEvent& event);
    void decrementRows(wxCommandEvent& event);
    void incrementColumns(wxCommandEvent& event);
    void decrementColumns(wxCommandEvent& event);
    void getValue(wxCommandEvent& event);
    void drawTable();
    void onTextChange(wxCommandEvent& event);
    void onTextChangeLastColumn(wxCommandEvent& event);
    void onTextChangeTargetFunction(wxCommandEvent& event);
    static void displayResult(SimplexSolver& solver);
    static std::string formatNumber(double number);
};


#endif //SIMPLEX_MAINFRAME_HPP
