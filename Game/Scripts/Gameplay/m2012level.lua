function M2012Level_OnCreate()
    SetWorldTime(2.5)
    SetCursorVisible()
end

function M2012Level_OnDestroy()
end

function M2012Level_OnPreUpdate(dt)
end

function M2012Level_OnPreRender(dt)
    CurrentScene:SetLocalCameraFPC(LocalPlayer:GetFirstPersonCamera("CinematicCamera0"))
end

function M2012Level_OnPostRender()
end