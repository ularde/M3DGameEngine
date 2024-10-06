function AnimalCell_OnCreate()
    SetCursorVisible()
end

function AnimalCell_OnDestroy()
end

function AnimalCell_OnPreUpdate(dt)
end

function AnimalCell_OnPreRender(dt)
    SetWorldTime(4.0)
    SetDynamicTimeOfDay(0)
    CurrentScene:SetLocalCameraCC(MVCamera:GetCinematicCamera("CinematicCamera0"))
end

function AnimalCell_OnPostsRender()
end