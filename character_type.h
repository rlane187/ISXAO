#pragma once

class CharacterType : public LSTypeDefinition
{
public:
	int PersistentClass;

	enum CharacterTypeMembers
	{
		Inventory,
		InventoryCount,
		GetInventory,
		NanoSpell,
		NanoSpellCount,
		GetNanoSpells,
		SpecialAction,
		SpecialActionCount,
		GetSpecialActions,
		ToActor,
	};

	enum CharacterTypeMethods
	{
		Activate,
		Attack,
		Cast,
		Sit,
		Stand
	};

	CharacterType() : LSType("character")
	{
		TypeMember(Inventory);
		TypeMember(InventoryCount);
		TypeMember(GetInventory);
		TypeMember(NanoSpell);
		TypeMember(NanoSpellCount);
		TypeMember(GetNanoSpells);
		TypeMember(SpecialAction);
		TypeMember(SpecialActionCount);
		TypeMember(GetSpecialActions);		
		TypeMember(ToActor);

		TypeMethod(Activate);
		TypeMethod(Attack);
		TypeMethod(Cast);
		TypeMethod(Sit);
		TypeMethod(Stand);
		

		PersistentClass = pISInterface->RegisterPersistentClass("character");
		pISInterface->SetPersistentClass(this, PersistentClass);
	}

	~CharacterType()
	{
		pISInterface->InvalidatePersistentClass(PersistentClass);
	}

	virtual bool GetMember(LSOBJECTDATA ObjectData, PLSTYPEMEMBER Member, int argc, char *argv[], LSOBJECT &Object);
	virtual bool GetMethod(LSOBJECTDATA &ObjectData, PLSTYPEMETHOD pMethod, int argc, char *argv[]);
	virtual bool ToText(LSOBJECTDATA ObjectData, char *buf, unsigned int buflen);

	INHERITDIRECT(pActorType)

};
