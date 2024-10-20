local fsTempFlag = GetGlobalVariableInt("fullscreen")
local aqTemp = GetGlobalVariableInt("atomsphere_quality")
local sqTemp = GetGlobalVariableInt("shadow_quality")

function Settings_OnCreate()
    SetCursorVisible()
end

function Settings_OnDestroy()
end

function Settings_OnPreUpdate(dt)
end

function Settings_OnPreRender(dt)
    --local b1 = GuiButton("arial", )
end

function Apply_OnClick()
    SetGlobalVariableInt("fullscreen", fsTempFlag)
    SetGlobalVariableInt("shadow_quality", sqTemp)
    SetGlobalVariableInt("atomsphere_quality", aqTemp)
end

function Back_OnClick()
    LoadScene("scenes\\mainmenu.mmap")
end

function Settings_OnPostRender()
    local windowSize = GetWindowSize()

    if (sqTemp > 8) then
        sqTemp = 1
    end
    if (aqTemp > 3) then
        aqTemp = 1
    end

    GuiRenderText("arial96", "Settings", windowSize.x / 16.0, windowSize.y * 4.0 / 5.0, 
    1.0, 1.0, 1.0, 1.0)
    GuiRenderText("arial", "Full Screen", windowSize.x * 3.0 / 32.0, windowSize.y * 2.0 / 3.0, 
    1.0, 1.0, 1.0, 0.7)
    GuiRenderText("arial", "Atomsphere Quality", windowSize.x * 3.0 / 32.0, windowSize.y * 2.0 / 3.0 - 80.0, 
    1.0, 1.0, 1.0, 0.7)
    GuiRenderText("arial", "Shadow Quality", windowSize.x * 3.0 / 32.0, windowSize.y * 2.0 / 3.0 - 160.0, 
    1.0, 1.0, 1.0, 0.7)

    local fsString = ""
    if (fsTempFlag == 1) then
        fsString = "Enabled"
    else
        fsString = "Disabled"
    end
    local fsEvent = GuiRenderButton("arial", fsString, windowSize.x - windowSize.x * 3.0 / 32.0 - 14.0 * 18.0, windowSize.y * 2.0 / 3.0,
    1.0, 1.0, 1.0,
    1.0, 0.5, 0.0,
    0.7, 10.0, 1)
    if (fsEvent.click) then
        if (fsTempFlag == 1) then
            fsTempFlag = 0
        else
            fsTempFlag = 1
        end
    end


    local aqEvent = GuiRenderButton("arial", tostring(aqTemp), windowSize.x - windowSize.x * 3.0 / 32.0 - 14.0 * 18.0, windowSize.y * 2.0 / 3.0 - 80.0,
    1.0, 1.0, 1.0,
    1.0, 0.5, 0.0,
    0.7, 10.0, 1)
    if (aqEvent.click) then
        if (aqTemp < 4) then
            aqTemp = aqTemp + 1
        end
    end

    local sqEvent = GuiRenderButton("arial", tostring(sqTemp), windowSize.x - windowSize.x * 3.0 / 32.0 - 14.0 * 18.0, windowSize.y * 2.0 / 3.0 - 160.0,
    1.0, 1.0, 1.0,
    1.0, 0.5, 0.0,
    0.7, 10.0, 1)
    if (sqEvent.click) then
        if (sqTemp < 9) then
            sqTemp = sqTemp + 1
        end
    end

    local applyEvent = GuiRenderButton("arial", "Apply", windowSize.x / 2.0 - windowSize.x / 8.0 - 5.0 * 20.0, windowSize.y / 10.0,
    1.0, 1.0, 1.0,
    1.0, 0.5, 0.0,
    1.0, 10.0, 1)
    if (applyEvent.click) then
        Apply_OnClick()
    end
    backEvent = GuiRenderButton("arial", "Back", windowSize.x / 2.0 + windowSize.x / 8.0, windowSize.y / 10.0,
    1.0, 1.0, 1.0,
    1.0, 0.5, 0.0,
    1.0, 10.0, 1)
    if (backEvent.click) then
        Back_OnClick()
    end 
end