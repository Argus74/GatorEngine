
local Transform
local Position
local Frame = 0
local MovingForward = true
local LoopingTime = 3
local FPS = 60

--First frame of the game
function Start()
    Transform = AttachedEntity:GetCTransform()
    Position = Transform.Position
end

--Each frame of the game
function Update()
	Frame = Frame + 1
    --If LoopingTime amount of seconds has elapsed,
    --then make the platform go the other direction
    if (Frame / FPS)  % LoopingTime == 0 then
        Frame = 0
        MovingForward = not MovingForward
    end
    
    if MovingForward then
        Position.X = Position.X + 1
    else
        Position.X = Position.X - 1
    end
end
