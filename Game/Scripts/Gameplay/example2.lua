function Example2_OnCreate()
end

function Example2_OnDestroy()
end

function Example2_OnPreUpdate(dt)
end

function Example2_OnPreRender(dt)
    SetWorldTime(9.0)
    SetDynamicTimeOfDay(0)
    CurrentScene:SetLocalCameraFPC(LocalPlayer:GetFirstPersonCamera("DefaultCamera"))
end

function Example2_OnPostRender()
end