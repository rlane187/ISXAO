#include "isxao_main.h"

namespace isxao_classes
{

	IDENTITY NanoTemplate::GetCasterIdentity() const
	{
		return GetNanoTemplateData().CasterIdentity;
	}

	DWORD NanoTemplate::GetDuration() const
	{
		return GetNanoTemplateData().Duration;
	}

	IDENTITY NanoTemplate::GetNanoIdentity() const
	{
		return GetNanoTemplateData().NanoIdentity;
	}

	NANOTEMPLATE NanoTemplate::GetNanoTemplateData() const
	{
		return nano_template_;
	}

	DWORD NanoTemplate::GetStartTime() const
	{
		return GetNanoTemplateData().StartTime;
	}

	float NanoTemplate::GetTimeRemaining() const
	{
		auto result = 0.0f;
		result = float(float((GetStartTime() + GetDuration()) / 100) - pGametime->GetNormalTime());
		if (result < 0.0f)
			result = 0.0f;
		return result;
	}

	bool NanoTemplate::NanoTemplateCompare(NanoTemplate &a, NanoTemplate &b)
	{
		return a.GetNanoIdentity().Id < b.GetNanoIdentity().Id;
	}

	bool NanoTemplate::Remove() const
	{
		return pEngineClientAnarchy->N3Msg_RemoveBuff(GetNanoIdentity());
	}

}