

function Update()
	print("Before for loop")
	local Entities = EntityManager:GetEntities()
	print(Entities)
	for i=1, #Entities do
		print(Entities[i])
	end
	print("After for loop")
	local NewEntity = EntityManager:AddEntity("Test")
	print("After creating the entity")
	print(NewEntity)
	print(#Entities)
	print("Testing chang")
	
end
