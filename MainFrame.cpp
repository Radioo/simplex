//
// Created by Radio on 04/04/2024.
//

#include "MainFrame.hpp"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Simplex", wxDefaultPosition, wxSize(640, 480)) {
    auto* vSizer = new wxBoxSizer(wxVERTICAL);
    auto* hSizer = new wxBoxSizer(wxHORIZONTAL);

    auto* rowsText = new wxStaticText(this, wxID_ANY, "Rows");
    hSizer->Add(rowsText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

    auto* plusButton1 = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(30, 20));
    hSizer->Add(plusButton1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    plusButton1->Bind(wxEVT_BUTTON, &MainFrame::incrementRows, this);

    auto* minusButton1 = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(30, 20));
    hSizer->Add(minusButton1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    minusButton1->Bind(wxEVT_BUTTON, &MainFrame::decrementRows, this);

    auto* columnsText = new wxStaticText(this, wxID_ANY, "Columns");
    hSizer->Add(columnsText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

    auto* plusButton2 = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(30, 20));
    hSizer->Add(plusButton2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    plusButton2->Bind(wxEVT_BUTTON, &MainFrame::incrementColumns, this);

    auto* minusButton2 = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(30, 20));
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

void MainFrame::drawTable() {
    auto* vSizer = new wxBoxSizer(wxVERTICAL);

    table.resize(rows);
    for(auto& row : table) {
        row.resize(columns);
    }

    for(int i = 0; i < rows; i++) {
        auto* hSizer = new wxBoxSizer(wxHORIZONTAL);

        for(int j = 0; j < columns; j++) {
            auto* textCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(20, 20));
            hSizer->Add(textCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

            table[i][j] = textCtrl;

            textCtrl->Bind(wxEVT_TEXT, &MainFrame::onTextChange, this);
        }
        vSizer->Add(hSizer, 0, wxALIGN_CENTER);
    }

    tableVSizer->Add(vSizer, 0, wxALIGN_CENTER);
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