#include "RobotControllerApp.hpp"
#include "Packets.hpp"

#include <DanLib/Random/Random.hpp>

#include <GuiStuff/GridDisplayer.hpp>
#include <GuiStuff/PictureInPictureWindow.hpp>
#include <wx/sizer.h>

#include <chrono>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace
{
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  rc::MotorCommand GetRandomMotorCommand()
  {
    return
    {
      dl::random::GetUniform<uint8_t>(),
      dl::random::GetUniform<uint8_t>(),
      dl::random::GetUniform<uint8_t>()
    };
  }

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  rc::Position GetRandomPosition()
  {
    return
    {
      dl::random::GetUniform<double>(),
      dl::random::GetUniform<double>(),
      dl::random::GetUniform<double>(),
      static_cast<double>(std::chrono::system_clock::now().time_since_epoch().count())
    };
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
RobotControllerApp::~RobotControllerApp()
{
  mIsRunning = false;

  if (mpThread && mpThread->joinable())
  {
    mpThread->join();
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool RobotControllerApp::OnInit()
{
  SetVendorName("Lomancer Heavy Industries");
  SetAppName("Robot Controller");

  auto pFrame = new wxFrame(nullptr, wxID_ANY, "Robot Controller");

  auto pGridDisplayer = new gs::GridDisplayer<rc::MotorCommand, rc::Position>(pFrame);

  auto pMainSizer = new wxBoxSizer(wxVERTICAL);

  pMainSizer->Add(pGridDisplayer, 0, wxEXPAND | wxALL, 1);

  wxImage Image, Temp;

  if (!Image.LoadFile("/home/dloman/SbhxMap.bmp", wxBITMAP_TYPE_ANY))
  {
    return false;
  }

  if (!Temp.LoadFile("/home/dloman/pic2.bmp", wxBITMAP_TYPE_ANY))
  {
    return false;
  }

  mpMapVideoWindow = new gs::PictureInPictureWindow(pFrame, Image, Temp);

  pMainSizer->Add(mpMapVideoWindow, 5, wxEXPAND | wxALL, 1);

  pFrame->SetSizer(pMainSizer);

  pFrame->Layout();

  pFrame->Show();

  mIsRunning = true;

  mpThread.reset(new std::thread([pGridDisplayer, this]
    {
      while (mIsRunning)
      {
        auto MotorCommand = GetRandomMotorCommand();

        pGridDisplayer->Set(MotorCommand);

        auto Position = GetRandomPosition();

        pGridDisplayer->Set(Position);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
    }));

  return true;
}
