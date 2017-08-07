#pragma once

#include "Packets.hpp"

#include <GuiStuff/GridDisplayer.hpp>

#include <wx/app.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

namespace gs
{
  class PictureInPictureWindow;
}

namespace vl
{
  class VideoPlayer;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class RobotControllerApp : public wxApp
{
  public:

    RobotControllerApp();

    ~RobotControllerApp();

    bool OnInit() override;

  private:

    bool SetupMap();

    void SetupVideo();

    void SetupDisplay();

    void SetupTelemetry();

  private:

    wxFrame* mpFrame;

    std::atomic<bool> mIsRunning;

    std::unique_ptr<std::thread> mpThread;

    std::mutex mMapVideoMutex;

    gs::PictureInPictureWindow* mpMapVideoWindow;

    gs::GridDisplayer<rc::MotorCommand, rc::Position>* mpGridDisplayer;

    std::unique_ptr<vl::VideoPlayer> mpVideoPlayer;

    wxImage mMapImage;
};

IMPLEMENT_APP(RobotControllerApp);

