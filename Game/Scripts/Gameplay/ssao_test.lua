function SSAOTest_OnCreate()
    SetWorldTime(1000.0)
end

function SSAOTest_OnDestroy()
end

function SSAOTest_OnPreUpdate(dt)
end

function SSAOTest_OnPreRender(dt)
    CurrentScene:SetLocalCameraFPC(LocalPlayer:GetFirstPersonCamera("DefaultCamera"))
end

function SSAOTest_OnPostRender()
end