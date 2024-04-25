

function Update()
	
end


function OnTouched(OtherEntity)
    if OtherEntity.Name == "Hazard" then
        local Health = AttachedEntity:GetCHealth()
        Health.CurrentHealth = Health.CurrentHealth - 1
    end
end