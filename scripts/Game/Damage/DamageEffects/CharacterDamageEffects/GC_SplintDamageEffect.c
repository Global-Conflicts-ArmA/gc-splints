class GC_SplintDamageEffect : SCR_InstantDamageEffect
{
	//------------------------------------------------------------------------------------------------
	override void OnEffectAdded(SCR_ExtendedDamageManagerComponent dmgManager)			
	{
		super.OnEffectAdded(dmgManager);
		
		SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(dmgManager);
		if (!damageMgr)
			return;
		
		// if cast fails, HZ is probably virtual
		SCR_CharacterHitZone charHZ = SCR_CharacterHitZone.Cast(GetAffectedHitZone());
		if (!charHZ)
			return;
		
		ECharacterHitZoneGroup hzGroup;		
		array<HitZone> hitZones = {};
		
		damageMgr.GetHitZonesOfGroup(charHZ.GetHitZoneGroup(), hitZones);
		if (hitZones.IsEmpty())
		{
			return;
		}
		else
		{
			HitZone targetHitZone = hitZones.Get(0);
			if (!targetHitZone)
				return;
				
			SCR_CharacterHitZone charHitZone = SCR_CharacterHitZone.Cast(targetHitZone);
			if (!charHitZone)
				return;
			
			hzGroup = charHitZone.GetHitZoneGroup();
		}

		if (hzGroup == ECharacterHitZoneGroup.VIRTUAL)
			return;
		
		foreach (HitZone hitZone : hitZones)
		{
			// If hitzone is undamaged, go onto the next one
			float healthToAdd = hitZone.GetMaxHealth() - hitZone.GetHealth();
			if (healthToAdd <= 0)
				continue;

			// Heal hitZone
			hitZone.HandleDamage(-healthToAdd, EDamageType.HEALING, null);
		}
		
		return;
	}			
	
	//------------------------------------------------------------------------------------------------
	protected override void HandleConsequences(SCR_ExtendedDamageManagerComponent dmgManager, DamageEffectEvaluator evaluator)
	{
		super.HandleConsequences(dmgManager, evaluator);
		
		evaluator.HandleEffectConsequences(this, dmgManager);
	}
}