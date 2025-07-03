function activate()
    local px, py = get_player_pos()
    for i=1, 36 do
        local angle = math.rad(i*10)
        emit_bullet(px, py, math.cos(angle)*120, math.sin(angle)*120, 1)
    end
end

function update(dt)
    -- 可选：每帧弹幕逻辑
end