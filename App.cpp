//
// Created by Radio on 04/04/2024.
//

#include "App.hpp"
#include "MainFrame.hpp"

bool App::OnInit() {
    auto* mainFrame = new MainFrame();
    mainFrame->Show();
    return true;
}
