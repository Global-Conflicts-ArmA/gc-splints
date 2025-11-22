//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : SCR_ExtendedDamageManagerComponent
{
	//-----------------------------------------------------------------------------------------------------------
	/*! Determine which hitZones group has taken the highest damage
	\param onlyExtremities Only compare the 4 extremities (both arms, both legs)
	*/
	ECharacterHitZoneGroup GetCharMostDamageHitzoneGroup(bool onlyExtremities = false, bool ignoreIfBeingTreated = false)
	{
		array<float> DamageValues = {};
		typename groupEnum = ECharacterHitZoneGroup;
		int groupCount = groupEnum.GetVariableCount();

		for (int i; i < groupCount; i++)
			DamageValues.Insert(0);

		float highestDamage;
		array<HitZone> hitZones = {}; 
		
		GetPhysicalHitZones(hitZones);
		SCR_CharacterHitZone charHitZone;
		ECharacterHitZoneGroup group;
		
		foreach (HitZone hitZone : hitZones)
		{
			charHitZone = SCR_CharacterHitZone.Cast(hitZone);
			if (!charHitZone)
				continue;
		
			// Virtual hitZones don't count
			group = charHitZone.GetHitZoneGroup();
			if (group == EHitZoneGroup.VIRTUAL)
				continue;
			
			// If only extremities are desired for checking, continue if group is not an extremity
			if (onlyExtremities && !EXTREMITY_LIMB_GROUPS.Contains(group))
				continue;
			
			float health = GetGroupHealthScaled(group);
			if (health == 1)
				continue;
			
			// if desired, hitzones that are being treated are skipped so another hitZone will be healed by this inquiry
			if (GetGroupIsBeingHealed(group))
			{
				if (ignoreIfBeingTreated)
				{
					DamageValues[LIMB_GROUPS.Find(group)] = 0;
					continue;
				}
			}
			
			DamageValues[LIMB_GROUPS.Find(group)] = 1 - health;
		}
		
		ECharacterHitZoneGroup mostDamageHitZoneGroup;
		
		for (int i; i < groupCount; i++)
		{
			if (DamageValues[i] > highestDamage)
			{
				highestDamage = DamageValues[i];
				mostDamageHitZoneGroup = i;
			}
		}
		
		return LIMB_GROUPS.Get(mostDamageHitZoneGroup);
	}
};