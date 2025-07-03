local t = 0
local interval = 0.1
local timer = 0

function activate()
    t = 0
    timer = 0
end

function update(dt)
    t = t + dt
    timer = timer + dt
    if timer > interval then
        timer = timer - interval
        local px, py = get_player_pos()
        local base_angle = t * 2  -- 旋转速度
        local n = 24              -- 每批弹幕数量
        for i = 1, n do
            local angle = base_angle + math.pi * 2 * (i-1) / n
            local speed = 180 + 60 * math.sin(t + i)  -- 速度随时间波动
            emit_bullet(px, py, math.cos(angle) * speed, math.sin(angle) * speed, 1)
        end
    end
end