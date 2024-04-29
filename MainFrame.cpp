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
    double* FxInputD = new double[columns];
    double* lastInputD = new double[rows];
    double** textCtrlD = new double*[rows];
    double doubleValue;

    std::vector<std::vector<double>> constraints(rows, std::vector<double>(columns));
    std::vector<double> rightHandSide(rows);
    std::vector<double> objectiveFunction(columns);
    std::vector<std::string> signs(rows);
    bool isMaximization = operatorMinMax->GetStringSelection() == "max";

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            value = textCtrl[i][j]->GetValue();
            value.ToDouble(&doubleValue);
            constraints[i][j] = doubleValue;
        }
    }

    for (int i = 0; i < rows; i++) {
        value = lastInputs[i]->GetValue();
        value.ToDouble(&doubleValue);
        rightHandSide[i] = doubleValue;
    }

    for (int i = 0; i < columns; i++) {
        value = FxInput[i]->GetValue();
        value.ToDouble(&doubleValue);
        objectiveFunction[i] = doubleValue;
    }

    for (int i = 0; i < rows; i++) {
        wxString selectedOperator = operatorCombo[i]->GetStringSelection();
        signs[i] = selectedOperator;
    }

    SimplexSolver solver(constraints, rightHandSide, objectiveFunction, signs, isMaximization);
    solver.solve();

    displayResult(solver);
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
        FxInput[i] = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(25, 25), wxTE_CENTRE);
        hSizer->Add(FxInput[i], 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

        auto* xText = new wxStaticText(this, wxID_ANY, "x" + std::to_string(i+1));
        hSizer->Add(xText,0,wxALL|wxALIGN_CENTER_VERTICAL, 5);
        FxInput[i]->Bind(wxEVT_TEXT, &MainFrame::onTextChangeTargetFunction, this);
    }
    wxString strMinMax[] = { "max", "min" };
    operatorMinMax = new wxComboBox(this, wxID_ANY, strMinMax[0], wxDefaultPosition, wxDefaultSize, WXSIZEOF(strMinMax), strMinMax, wxCB_READONLY);
    hSizer->Add(operatorMinMax,0,wxALL|wxALIGN_CENTER_VERTICAL, 5);

    vSizer->Add(hSizer, 0, wxALIGN_CENTER);

    for(int i = 0; i < rows; i++) {
        auto* hSizer = new wxBoxSizer(wxHORIZONTAL);

        for(int j = 0; j < columns; j++) {
            textCtrl[i][j] = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(25, 25), wxTE_CENTRE);
            auto* xText = new wxStaticText(this, wxID_ANY, "x" + std::to_string(j+1));
        
            hSizer->Add(textCtrl[i][j], 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
            hSizer->Add(xText,0,wxALL|wxALIGN_CENTER_VERTICAL, 5);


            table[i][j] = textCtrl[i][j];
            textCtrl[i][j]->Bind(wxEVT_TEXT, &MainFrame::onTextChange, this);
        }
        wxString operators[] = { "=", "<=", ">=" };
        operatorCombo[i] = new wxComboBox(this, wxID_ANY, operators[0], wxDefaultPosition, wxDefaultSize, WXSIZEOF(operators), operators, wxCB_READONLY);
        hSizer->Add(operatorCombo[i],0,wxALL|wxALIGN_CENTER_VERTICAL, 5);
        
        lastInputs[i] = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(25, 25), wxTE_CENTRE);
        lastInputs[i]->Bind(wxEVT_TEXT, &MainFrame::onTextChangeLastColumn, this);
        hSizer->Add(lastInputs[i],0,wxALL|wxALIGN_CENTER_VERTICAL, 5);

        vSizer->Add(hSizer, 0, wxALIGN_CENTER);
    }
    
    auto* calculateButton = new wxButton(this, wxID_ANY, wxT("Calculate"), wxDefaultPosition, wxSize(100, 50));
    calculateButton->Bind(wxEVT_BUTTON, &MainFrame::getValue, this);
        
    tableVSizer->Add(vSizer, 0, wxALIGN_CENTER);
    tableVSizer->Add(calculateButton, 0, wxALL|wxALIGN_CENTER, 5);
}

void MainFrame::onTextChangeTargetFunction(wxCommandEvent& event) {
    int eventColumn = -1;
    for(int i = 0; i < columns; i++) {
        if (FxInput[i] == event.GetEventObject()) {
            eventColumn = i;
            break;
        }
    }

    int maxWidth = 0;
    for(int i = 0; i < columns; i++) {
        int width = FxInput[i]->GetTextExtent(FxInput[i]->GetValue()).GetWidth();
        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    for(int i = 0; i < columns; i++) {
        FxInput[i]->SetMinSize(wxSize(maxWidth + 10, -1));
    }

    Layout();
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

void MainFrame::displayResult(SimplexSolver &solver) {
    wxString message = "Objective value: " + formatNumber(solver.getObjectiveValue()) + "\n";
    message += "Solution: \n";
    std::vector<double> solution = solver.getSolution();
    auto solutionCount = solution.size();

    for(int i = 0; i < solutionCount; i++) {
        message += "x" + std::to_string(i + 1) + " = " + formatNumber(solution[i]) + "\n";
    }

    wxMessageBox(message, "Result", wxOK | wxICON_INFORMATION);
}

std::string MainFrame::formatNumber(double number) {
    // Strip trailing zeros and remove the decimal point if the number is an integer
    std::string str = std::to_string(number);
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    if(str.back() == '.') {
        str.pop_back();
    }
    return str;
}
