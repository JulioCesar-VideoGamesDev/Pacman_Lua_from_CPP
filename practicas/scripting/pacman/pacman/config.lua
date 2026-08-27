-- config.lua
-- Configuration file for the Pacman game

-- Gained score when eating a powerUp
powerUpScore = 5000

-- Timme that lasts the powerUp (in seconds)
powerUpDuration = 5.0

-- Speed multiplier when eating a powerUp
powerUpSpeedMultiplier = 2.0

-- Number of points needed to gain a bronce medal
bronzeMedalPoints = 500

-- Number of points gained for each coin eaten
coinPoints = 50

-- Color logic based on the health
function getPowerUpColor(health)
    
    if health >= 1.5 then
        -- RED
        return {r = 255, g = 0, b = 0}
    elseif health >= 1.0 then
        -- ORANGE
        return {r = 255, g = 165, b = 0}
    elseif health >= 0.5 then
        -- GREEN
        return {r = 0, g = 255, b = 0}
    else
        -- AZUL
        return {r = 0, g = 0, b = 255}
    end
end
