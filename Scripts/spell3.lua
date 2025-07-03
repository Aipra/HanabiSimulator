local t = 0
local active = false
local duration = 10.0  -- 持续时间（秒）
local interval = 0.12  -- 发射间隔
local timer = 0

function activate()
    t = 0
    timer = 0
    active = true
end

function update(dt)
    if not active then return end
    t = t + dt
    timer = timer + dt
    if t > duration then
        active = false
        return
    end
    -- 每隔interval秒发射一批弹幕
    if timer > interval then
        timer = timer - interval
        local px, py = get_player_pos()
        local n = 32  -- 每批弹幕数量
        local base_angle = t * 1.5  -- 旋转速度
        for i = 1, n do
            local angle = base_angle + math.pi * 2 * (i-1) / n
            local speed = 180 + 60 * math.sin(t + i)  -- 速度波动
            emit_bullet(px, py, math.cos(angle) * speed, math.sin(angle) * speed, 1)
        end
    end
end