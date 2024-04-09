//
// Created by Radio on 04/04/2024.
//

#include "MainFrame.hpp"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Simplex", wxDefaultPosition, wxSize(640, 480)) {
    
    auto* vSizer = new wxBoxSizer(wxVERTICAL);
    auto* hSizer = new wxBoxSizer(wxHORIZONTAL);

    auto* rowsText = new wxStaticText(this, wxID_ANY, "Rows");
    hSizer->Add(rowsText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

    auto* plusButton1 = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(40, 40));
    hSizer->Add(plusButton1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    plusButton1->Bind(wxEVT_BUTTON, &MainFrame::incrementRows, this);

    auto* minusButton1 = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(40, 40));
    hSizer->Add(minusButton1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    minusButton1->Bind(wxEVT_BUTTON, &MainFrame::decrementRows, this);

    auto* columnsText = new wxStaticText(this, wxID_ANY, "Columns");
    hSizer->Add(columnsText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

    auto* plusButton2 = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(40, 40));
    hSizer->Add(plusButton2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    plusButton2->Bind(wxEVT_BUTTON, &MainFrame::incrementColumns, this);

    auto* minusButton2 = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(40, 40));
    hSizer->Add(minusButton2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    minusButton2->Bind(wxEVT_BUTTON, &MainFrame::decrementColumns, this);

    mainVSizer->Add(hSizer, 0, wxALIGN_CENTER);
    mainVSizer->Add(tableVSizer, 0, wxALIGN_CENTER);

    drawTable();

    this->SetSizer(mainVSizer);
    this->Centre();
}

void MainFrame::incrementRows(wxCommandEvent& event) {
    if(rows == UINT8_MAX) {
        return;
    }

    rows++;
    tableVSizer->Clear(true);
    drawTable();
    Layout();
}


void MainFrame::decrementRows(wxCommandEvent& event) {
    if(rows == 2) {
        return;
    }

    rows--;
    tableVSizer->Clear(true);
    drawTable();
    Layout();
}

void MainFrame::incrementColumns(wxCommandEvent& event) {
    if(columns == UINT8_MAX) {
        return;
    }

    columns++;
    tableVSizer->Clear(true);
    drawTable();
    Layout();
}

void MainFrame::decrementColumns(wxCommandEvent& event) {
    if(columns == 2) {
        return;
    }

    columns--;
    tableVSizer->Clear(true);
    drawTable();
    Layout();
}

void MainFrame::getValue(wxCommandEvent& event){
    wxString value;
    double* lastInputD = new double[rows];
    double** textCtrlD = new double*[rows];
    
    for (int i = 0; i < rows; ++i) {
        textCtrlD[i] = new double[columns];
    }


    for (int i = 0; i < rows; i++) {
        value = lastInput[i]->GetValue();
        double doubleValue;
        value.ToDouble(&doubleValue);
        lastInputD[i] = doubleValue;
    }


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            value = textCtrl[i][j]->GetValue();
            double doubleValue;
            value.ToDouble(&doubleValue);
            textCtrlD[i][j] = doubleValue;
        }
    }

    std::cout << "lastInputD:" << std::endl;
    for (int i = 0; i < rows; ++i) {
        std::cout << lastInputD[i] << " ";
    }
    std::cout << std::endl;


    std::cout << "textCtrlD:" << std::endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            std::cout << textCtrlD[i][j] << " ";
        }
        std::cout << std::endl;
    }

    

    for (int i = 0; i < rows; ++i) {
        wxString selectedOperator = operatorCombo[i]->GetStringSelection();
        std::cout << selectedOperator << " ";
    }
    std::cout << std::endl;


    delete[] lastInputD;
    for (int i = 0; i < rows; ++i) {
        delete[] textCtrlD[i];
    }
    delete[] textCtrlD;
    
}

void MainFrame::drawTable() {
    auto* vSizer = new wxBoxSizer(wxVERTICAL);

    table.resize(rows);
    for(auto& row : table) {
        row.resize(columns);
    }

    lastInputs.resize(rows);
    FxInput.resize(columns);

    operatorCombo = new wxComboBox*[rows];
    lastInput = new wxTextCtrl*[rows];
    textCtrl = new wxTextCtrl**[rows];
    
    for (int i = 0; i < rows; i++) {
        textCtrl[i] = new wxTextCtrl*[columns];
    }

    auto* hSizer = new wxBoxSizer(wxHORIZONTAL);
    auto* FxText = new wxStaticText(this, wxID_ANY, "F(x) = ");
    hSizer->Add(FxText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    for(int i=0; i < columns; i++){
        FxInput[i] = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(25, 25));
        hSizer->Add(FxInput[i], 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

        auto* xText = new wxStaticText(this, wxID_ANY, "x" + std::to_string(i+1));
        hSizer->Add(xText,0,wxALL|wxALIGN_CENTER_VERTICAL, 5);
    }
    wxString strMinMax[] = { "max", "min" };
    operatorMinMax = new wxComboBox(this, wxID_ANY, strMinMax[0], wxDefaultPosition, wxDefaultSize, WXSIZEOF(strMinMax), strMinMax, wxCB_READONLY);
    hSizer->Add(operatorMinMax,0,wxALL|wxALIGN_CENTER_VERTICAL, 5);

    vSizer->Add(hSizer, 0, wxALIGN_CENTER);

    for(int i = 0; i < rows; i++) {
        auto* hSizer = new wxBoxSizer(wxHORIZONTAL);

        for(int j = 0; j < columns; j++) {
            textCtrl[i][j] = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(25, 25));
            auto* xText = new wxStaticText(this, wxID_ANY, "x" + std::to_string(j+1));
        
            hSizer->Add(textCtrl[i][j], 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
            hSizer->Add(xText,0,wxALL|wxALIGN_CENTER_VERTICAL, 5);


            table[i][j] = textCtrl[i][j];
            textCtrl[i][j]->Bind(wxEVT_TEXT, &MainFrame::onTextChange, this);
        }
        wxString operators[] = { "=", "<", ">" };
        operatorCombo[i] = new wxComboBox(this, wxID_ANY, operators[0], wxDefaultPosition, wxDefaultSize, WXSIZEOF(operators), operators, wxCB_READONLY);
        hSizer->Add(operatorCombo[i],0,wxALL|wxALIGN_CENTER_VERTICAL, 5);
        
        lastInputs[i] = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(25, 25));
        lastInputs[i]->Bind(wxEVT_TEXT, &MainFrame::onTextChangeLastColumn, this);
        hSizer->Add(lastInputs[i],0,wxALL|wxALIGN_CENTER_VERTICAL, 5);

        vSizer->Add(hSizer, 0, wxALIGN_CENTER);
    }
    
    auto* calculateButton = new wxButton(this, wxID_ANY, wxT("Calculate"), wxDefaultPosition, wxSize(100, 50));
    calculateButton->Bind(wxEVT_BUTTON, &MainFrame::getValue, this);
        
    tableVSizer->Add(vSizer, 0, wxALIGN_CENTER);
    tableVSizer->Add(calculateButton, 0, wxALL|wxALIGN_CENTER, 5);
}

void MainFrame::onTextChangeLastColumn(wxCommandEvent& event) {
    int eventRow = -1;
    for(int i = 0; i < rows; i++) {
        if(lastInputs[i] == event.GetEventObject()) {
            eventRow = i;
            break;
        }
    }

    int maxWidth = 0;
    for(int i = 0; i < rows; i++) {
        int width = lastInputs[i]->GetTextExtent(lastInputs[i]->GetValue()).GetWidth();
        if(width > maxWidth) {
            maxWidth = width;
        }
    }

    for(int i = 0; i < rows; i++) {
        lastInputs[i]->SetMinSize(wxSize(maxWidth + 10, -1));
    }

    Layout();
}

void MainFrame::onTextChange(wxCommandEvent& event) {
    // Find the column of the text control that triggered the event
    int eventColumn = -1;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            if(table[i][j] == event.GetEventObject()) {
                eventColumn = j;
                break;
            }
        }
        if(eventColumn != -1) {
            break;
        }
    }

    // Calculate the maximum width of the text in the column
    int maxWidth = 0;
    for(int i = 0; i < rows; i++) {
        int width = table[i][eventColumn]->GetTextExtent(table[i][eventColumn]->GetValue()).GetWidth();
        if(width > maxWidth) {
            maxWidth = width;
        }
    }

    // Apply the maximum width to all text controls in the column
    for(int i = 0; i < rows; i++) {
        table[i][eventColumn]->SetMinSize(wxSize(maxWidth + 10, -1)); // +10 for padding
    }

    // Update the layout
    Layout();
}