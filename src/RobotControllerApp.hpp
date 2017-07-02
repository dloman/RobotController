#pragma once

#include <wx/app.h>

#include <atomic>
#include <memory>
#include <thread>

namespace gs
{
  class PictureInPictureWindow;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class RobotControllerApp : public wxApp
{
  public:

    ~RobotControllerApp();

    bool OnInit() override;

  private:

    std::atomic<bool> mIsRunning;

    std::unique_ptr<std::thread> mpThread;

    gs::PictureInPictureWindow* mpMapVideoWindow;
};

IMPLEMENT_APP(RobotControllerApp);
