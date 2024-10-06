function CinematicCamera0_OnCreate()

end

function CinematicCamera0_OnDestroy()

end

function CinematicCamera0_OnUpdate(dt)
    BindThisEntity()
    BindThisEntityComponent()
    SetCameraPosition(10.0, 20.0, 0.0)
    SetCameraFront(0.0, -1.0, 0.0)
end

function CinematicCamera0_OnRender()

end