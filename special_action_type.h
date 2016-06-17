#pragma once

class SpecialActionType : public LSTypeDefinition
{
public:
	int PersistentClass;

	enum SpecialActionTypeMembers
	{
		Identity,
		IsLocked,
		Item,
		LockOutRemaining,
		Name,
	};

	enum SpecialActionTypeMethods
	{
		Use,
	};

	SpecialActionType() : LSType("SpecialAction")
	{
		TypeMember(Identity);
		TypeMember(IsLocked);
		TypeMember(Item);
		TypeMember(LockOutRemaining);
		TypeMember(Name);

		TypeMethod(Use);

		PersistentClass = pISInterface->RegisterPersistentClass("SpecialAction");
		pISInterface->SetPersistentClass(this, PersistentClass);
	}

	~SpecialActionType()
	{
		pISInterface->InvalidatePersistentClass(PersistentClass);
	}

	virtual bool GetMember(LSOBJECTDATA ObjectData, PLSTYPEMEMBER Member, int argc, char *argv[], LSOBJECT &Object);
	virtual bool GetMethod(LSOBJECTDATA &ObjectData, PLSTYPEMETHOD pMethod, int argc, char *argv[]);
	virtual bool ToText(LSOBJECTDATA ObjectData, char *buf, unsigned int buflen);

};