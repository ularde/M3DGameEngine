function Example1_OnCreate()
    SetWorldTime(2.5)
end

function Example1_OnDestroy()
end

function Example1_OnPreUpdate(dt)
end

function Example1_OnPreRender(dt)
    CurrentScene:SetLocalCameraFPC(LocalPlayer:GetFirstPersonCamera("DefaultCamera"))
end

function Example1_OnPostRender()
end