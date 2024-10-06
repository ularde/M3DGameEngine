local lastX = 0.0
local lastY = 0.0
local yaw = 0.0
local pitch = 0.0
local ratio = 0.0
local n = 0

function CinematicCamera0_OnCreate()
end

function CinematicCamera0_OnDestroy()

end

function CinematicCamera0_OnUpdate(dt)
    if (not(GetScrollOffsetY() == 0.0)) then
        ratio = ratio - 0.2 * GetScrollOffsetY()
    end
    if (ratio < 0.5) then
        ratio = 0.5
    end
    local xoffset = GetCursorPosX() - lastX
    local yoffset = lastY - GetCursorPosY()
    lastX = GetCursorPosX()
    lastY = GetCursorPosY()

    if (GetMouseRButtonDown() == 1) then
        xoffset = xoffset * 0.1
        yoffset = yoffset * 0.1
        yaw = yaw + xoffset
        pitch = pitch + yoffset
    end
    this.position.x = ratio * math.cos(math.rad(yaw)) * math.cos(math.rad(pitch))
    this.position.y = ratio * math.sin(math.rad(pitch))
    this.position.z = ratio * math.sin(math.rad(yaw)) * math.cos(math.rad(pitch))
    this.front.x = -this.position.x
    this.front.y = -this.position.y
    this.front.z = -this.position.z
end

function CinematicCamera0_OnRender()
end