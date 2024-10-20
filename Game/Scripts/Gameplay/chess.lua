local n = 0
local nSphere = 0

function Chess_OnCreate()
    SetCursorVisible()
end

function Chess_OnDestroy()
end

function Chess_OnPreUpdate(dt)
end

function Chess_OnPreRender(dt)
    SetWorldTime(1.0)
    SetDynamicTimeOfDay(0)
    ray = CastRayByCursorPosition(GetCursorPosX(), GetCursorPosY())
    cc0 = LocalPlayer:GetCinematicCamera("CinematicCamera0")
    cpos = cc0.position
    bpos = GetRaycastingBlockPosition(cpos, ray, 1000.0)
    bpos.y = bpos.y + 0.50
    CurrentScene:SetLocalCameraCC(cc0)
    proxy = Entity0:GetRigidBodyPhysicsProxySphere("PhysicsProxy0")
    proxy:Invalidate()
    proxy:SetPosition(bpos.x, bpos.y, bpos.z)
    if (GetMouseLButtonDown() == 1 and n == 0) then
        n = n + 1
        nSphere = nSphere + 1
        CurrentScene:SpawnEntityAutoNameEx("Scripts\\Actors\\Entities\\BasicEntity.ment", bpos.x, bpos.y, bpos.z)
    end
    if (GetMouseLButtonUp() == 1 and n >= 1) then
        n = 0
    end
end

function Chess_OnPostRender()
    GuiRenderText("arial", "Number", 25.0, 75.0, 1.0, 1.0, 1.0, 0.5)
    GuiRenderText("arial", tostring(nSphere), 25.0, 25.0, 1.0, 1.0, 1.0, 1.0)
end