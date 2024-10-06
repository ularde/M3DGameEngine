local n = 0

function Example3_OnCreate()
    SetWorldTime(2.5)
    SetCursorVisible()
end

function Example3_OnDestroy()
end

function Example3_OnPreUpdate(dt)
end

function Example3_OnPreRender(dt)
    CurrentScene:SetLocalCameraFPC(LocalPlayer:GetFirstPersonCamera("DefaultCamera"))
    --ray = CastRayByCursorPosition(GetCursorPosX(), GetCursorPosY())
    dc = LocalPlayer:GetFirstPersonCamera("DefaultCamera")
    local cpos = dc.position
    local dir = dc.front
    local new_cpos = CreateVec3(cpos.x + 0.5 * dir.x, cpos.y + 0.5 * dir.y, cpos.z + 0.5 * dir.z)
    bpos = GetRaycastingBlockPosition(new_cpos, dir, 1000.0)
    bpos.y = bpos.y + 0.5
    proxy = Entity0:GetRigidBodyPhysicsProxySphere("PhysicsProxy0")
    proxy:Invalidate()
    proxy:SetPosition(bpos.x, bpos.y, bpos.z)
    if (GetMouseLButtonDown() == 1 and n == 0) then
        n = n + 1
        CurrentScene:SpawnEntityAutoNameEx("Scripts\\Actors\\Entities\\BasicEntity.ment", bpos.x, bpos.y, bpos.z)
    end
    if (GetMouseLButtonUp() == 1 and n >= 1) then
        n = 0
    end
end

function Example3_OnPostRender()
end