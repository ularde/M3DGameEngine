function MainMenu_OnCreate()
    SetCursorVisible()
end

function MainMenu_OnDestroy()
end

function MainMenu_OnPreUpdate(dt)
end

function MainMenu_OnPreRender(dt)
    --local b1 = GuiButton("arial", )
end

function Start_OnClick()
    LoadScene("scenes\\simple_room.mmap")
end

function Settings_OnClick()
    LoadScene("scenes\\settings.mmap")
end

function MainMenu_OnPostRender()
    local windowSize = GetWindowSize()
    local startEvent = GuiRenderButton("arial", "Start", windowSize.x / 2.0 - 20.0 * 5.0, (windowSize.y / 5.0) + windowSize.y / 2.0,
     1.0, 1.0, 1.0,
     1.0, 0.5, 0.0,
      1.0, 10.0, 1)
      if (startEvent.click) then
        Start_OnClick()
      end
    settingsEvent = GuiRenderButton("arial", "Settings", windowSize.x / 2.0 - 20.0 * 5.0, (windowSize.y / 5.0) + windowSize.y / 2. - 100.0,
     1.0, 1.0, 1.0,
      1.0, 0.5, 0.0,
    1.0, 10.0, 1)
    if (settingsEvent.click) then
        Settings_OnClick()
    end 
    exitEvent = GuiRenderButton("arial", "Exit", windowSize.x / 2.0 - 20.0 * 5.0, (windowSize.y / 5.0) + windowSize.y / 2. - 200.0,
     1.0, 1.0, 1.0,
      1.0, 0.5, 0.0,
    1.0, 10.0, 1)
    if (exitEvent.click) then
        Exit(0)
    end
end