#include "RobotControllerApp.hpp"

#include <DanLib/Random/Random.hpp>
#include <VideoLib/VideoPlayer/VideoPlayer.hpp>

#include <GuiStuff/Helpers.hpp>
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
RobotControllerApp::RobotControllerApp()
  : mpFrame(nullptr),
    mIsRunning(false),
    mpThread(nullptr),
    mpMapVideoWindow(nullptr),
    mpGridDisplayer(nullptr),
    mpVideoPlayer(nullptr)
{
  wxInitAllImageHandlers();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
RobotControllerApp::~RobotControllerApp()
{
  std::lock_guard lock(mMapVideoMutex);

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

  mpFrame =
    new wxFrame(
      nullptr,
      wxID_ANY,
      "Robot Controller",
      wxDefaultPosition,
      wxSize(1000,800));

  mpMapVideoWindow = new gs::PictureInPictureWindow(mpFrame);

  mpGridDisplayer =
    new gs::GridDisplayer<rc::MotorCommand, rc::Position>(mpFrame);

  auto success = SetupMap();

  SetupVideo();

  SetupDisplay();

  SetupTelemetry();

  return success;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool RobotControllerApp::SetupMap()
{
  if (!mMapImage.LoadFile("/home/dloman/SbhxMap.bmp", wxBITMAP_TYPE_ANY))
  {
    return false;
  }

  auto pImageWrapper = std::make_shared<dl::image::Image>(
    mMapImage.GetWidth(),
    mMapImage.GetHeight(),
    std::experimental::make_observer(
      reinterpret_cast<std::byte*>(mMapImage.GetData())));

  mpMapVideoWindow->SetImage1(pImageWrapper);

  return true;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void RobotControllerApp::SetupVideo()
{
  mpVideoPlayer = std::make_unique<vl::VideoPlayer>("/dev/video0");

  mpVideoPlayer->GetSignalFrame().Connect(
    [this] (const std::shared_ptr<const vl::Frame>& pFrame)
    {
      const auto& pImage = pFrame->GetImage();

      std::lock_guard lock(mMapVideoMutex);

      if (mpMapVideoWindow)
      {
        mpMapVideoWindow->SetImage2(std::move(pImage));
      }
    });
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void RobotControllerApp::SetupDisplay()
{
  auto pMainSizer = new wxBoxSizer(wxVERTICAL);

  pMainSizer->Add(mpGridDisplayer, 0, wxEXPAND | wxALL, 1);

  pMainSizer->Add(mpMapVideoWindow, 5, wxEXPAND | wxALL, 1);

  mpFrame->SetSizer(pMainSizer);

  mpFrame->Layout();

  mpFrame->Show();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void RobotControllerApp::SetupTelemetry()
{
  mIsRunning = true;

  mpThread.reset(new std::thread([this]
    {
      while (mIsRunning)
      {
        auto MotorCommand = GetRandomMotorCommand();

        mpGridDisplayer->Set(MotorCommand);

        auto Position = GetRandomPosition();

        mpGridDisplayer->Set(Position);

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
      }
    }));
}
