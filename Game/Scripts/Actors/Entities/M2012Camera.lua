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
        ratio = -0.2 * GetScrollOffsetY()
        this.position.x = this.position.x - ratio * this.front.x
        this.position.y = this.position.y - ratio * this.front.y
        this.position.z = this.position.z - ratio * this.front.z
    end
    --if (ratio < 0.5) then
    --    ratio = 0.5
    --end
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

    if (math.abs(pitch) >= 89.0) then
        if (pitch > 0.0) then
            pitch = 89.0
        else
            pitch = -89.0
        end
    end

    if (math.abs(yaw) >= 75.0) then
        if (yaw > 0.0) then
            yaw = 75.0
        else
            yaw = -75.0
        end
    end

    this.front.x = -math.cos(math.rad(yaw)) * math.cos(math.rad(pitch))
    this.front.y = -math.sin(math.rad(pitch))
    this.front.z = -math.sin(math.rad(yaw)) * math.cos(math.rad(pitch))

    if (GetMouseLButtonDown() == 1) then
        this.position.x = this.position.x + math.cos(math.rad(yaw)) * yoffset * 0.01
        this.position.y = this.position.y
        this.position.z = this.position.z + math.cos(math.rad(yaw)) * xoffset * 0.01
    end
end

function CinematicCamera0_OnRender()
end