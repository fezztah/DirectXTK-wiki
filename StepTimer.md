The StepTimer class is a helper for managing a timed game loop.

> The standard Universal Windows Platform app, Windows 8.1 Store DirectX, Windows phone 8.1, and Xbox One Visual Studio templates all make use of ``StepTimer`` as well.

# Usage

The typical usage for the StepTimer helper class is to call ``Tick`` once before you render each frame. The ``Tick`` method takes a callback which is invoked to perform time-based updates in your game or application.

```cpp
#include <Windows.h>
#include "StepTimer.h"

DX::StepTimer s_timer;

void Update(DX::StepTimer const& timer)
{
    float delta = float(timer.GetElapsedSeconds());
    // Do your game update here
}

void Render()
{
    // Do your frame render here
}

void Tick()
{
    s_timer.Tick([&]()
    {
        Update(s_timer);
    });

    Render();
}

// Your main loop then calls ``Tick`` in a loop
```

## Variable-step mode
By default, the StepTimer class will invoke the callback given to ``Tick`` once each frame, and the callback can query the StepTimer class for the duration of time since the last update.

> StepTimer will take a number of precautions to ensure the delta time is never negative, nor is it so large (1/10th of a second is the default upper-bound) as to cause huge jumps that might otherwise disrupt debugging or suspending behavior.

## Fixed-step mode
Alternatively, the StepTimer class can be put into a fixed-step mode as follows:

```cpp
s_timer.SetFixedTimeStep(true);
s_timer.SetTargetElapsedSeconds(1.f / 60.f);
```

In this mode, ``Tick`` will invoke the callback as many times as needed to 'catch-up' with the expected fixed-update time. The callback can then rely on a steady (and fixed) duration which can increase robustness of physics and other simulation processing.

The fixed-step duration can be set by using **SetTargetElapsedTicks** or **SetTargetElapsedSeconds**. Note that StepTimer defines ticks as 10,000,000 per second (see ``TicksPerSecond``).

If a larger than usual delay is expected, the code should call **ResetElapsedTime** to avoid the fixed-step ``Tick`` calling the update method a large number of times to 'catch-up'.

> The overall loop time is limited by the frame rate of ``Present`` for standard 'render loop' usage. Otherwise, you can 'Tick' an instance of ``StepTimer`` on it's own thread at your own rate limited only by the [system timer resolution](https://docs.microsoft.com/windows/win32/api/timeapi/nf-timeapi-timebeginperiod).

# StepTimer
The class provides the following accessors:

* **GetElapsedTicks**: Returns the elapsed time in ticks.
* **GetElapsedSeconds**: Returns the elapsed time in seconds.

* **GetTotalTicks**: Returns the total time in ticks.
* **GetTotalSeconds**: Returns the total time in seconds.

* **GetFrameCount**: Returns the frame count (i.e. one frame per call to ``Tick``).

* **GetFramesPerSecond**: Returns the frames per second. This value is updated once per second, so it's not instantaneous. It can return 0 if there's not been enough time since the start of the program or since the last call to **ResetElapsedTime**.

* **TicksToSeconds**: Static method to convert ticks to seconds.
* **SecondsToTicks**: Static method to covert seconds to ticks.

# Platform notes

## Universal Windows Platform (UWP) apps and XBox One
The ``CoreApplication::Resuming`` handler should call **ResetElapsedTime**.

# Notes
Ticks are stored in unsigned 64-bit integers and second values are stored in ``double``. Why it is important to avoid using 32-bit values or single-precision floats is addressed in this article: [Don’t Store That in a Float](https://randomascii.wordpress.com/2012/02/13/dont-store-that-in-a-float/)

# Source

[StepTimer.h](https://raw.githubusercontent.com/walbourn/directx-vs-templates/main/d3d12game_win32_dr/StepTimer.h)

# Further reading
[Understanding GameTime](http://www.shawnhargreaves.com/blog/understanding-gametime.html)  
[Understanding Game Time Revisited](https://walbourn.github.io/understanding-game-time-revisited/)  
