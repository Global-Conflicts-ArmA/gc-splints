//! Splint effect
[BaseContainerProps()]
class GC_ConsumableSplint : SCR_ConsumableEffectHealthItems
{
	
	//------------------------------------------------------------------------------------------------
	override void ApplyEffect(notnull IEntity target, notnull IEntity user, IEntity item, ItemUseParameters animParams)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(target);
		if (!char)
			return;
		
		SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
		if (!damageMgr)
			return;
		
		m_eTargetHZGroup = damageMgr.FindAssociatedHitZoneGroup(animParams.GetIntParam());
		
		super.ApplyEffect(target, user, item, animParams);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanApplyEffect(notnull IEntity target, notnull IEntity user, out SCR_EConsumableFailReason failReason = SCR_EConsumableFailReason.NONE)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(target);
		if (!char)
			return false;
		
		SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
		if (!damageMgr)
			return false;
		
		if (damageMgr.GetAimingDamage() > 0 || damageMgr.GetMovementDamage() > 0)
			return true;
		
		return false;
	}	
	
	//------------------------------------------------------------------------------------------------
	override bool CanApplyEffectToHZ(notnull IEntity target, notnull IEntity user, ECharacterHitZoneGroup group, out SCR_EConsumableFailReason failReason = SCR_EConsumableFailReason.NONE)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(target);
		if (!char)
			return false;
		
		SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
		if (!damageMgr)
			return false;
		
		array<ECharacterHitZoneGroup> limbs = {};
		damageMgr.GetAllExtremities(limbs);
		
		if (!limbs.Contains(group))
			return false;
		
		if (damageMgr.GetGroupHealthScaled(group) < 1)
			return true;
		
		failReason = SCR_EConsumableFailReason.UNDAMAGED;
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override ItemUseParameters GetAnimationParameters(IEntity item, notnull IEntity target, ECharacterHitZoneGroup group = ECharacterHitZoneGroup.VIRTUAL)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(target);
		if (!char)
			return null;
		
		SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
		if (!damageMgr)
			return null;

		EBandagingAnimationBodyParts bodyPartToSplint = EBandagingAnimationBodyParts.Invalid;
		
		if (group != ECharacterHitZoneGroup.VIRTUAL)
		{
			if (!damageMgr.GetGroupIsBeingHealed(group))
				bodyPartToSplint = damageMgr.FindAssociatedBandagingBodyPart(group);
		}
		else
		{
			group = damageMgr.GetCharMostDamageHitzoneGroup(true, true);
			if (!group)
				return null;
			
			array<HitZone> groupHitZones = {};
			damageMgr.GetHitZonesOfGroup(group, groupHitZones);
			
			SCR_CharacterHitZone charHitZone = SCR_CharacterHitZone.Cast(groupHitZones[0]);
			if (!charHitZone)
				return null;
			
			bodyPartToSplint = charHitZone.GetBodyPartToHeal();
		}
		
		if (bodyPartToSplint == EBandagingAnimationBodyParts.Invalid)
				return null;
		
		bool allowMovement = bodyPartToSplint != EBandagingAnimationBodyParts.RightLeg && bodyPartToSplint != EBandagingAnimationBodyParts.LeftLeg;
		
		ItemUseParameters params = ItemUseParameters();
		params.SetEntity(item);
		params.SetAllowMovementDuringAction(allowMovement);
		params.SetKeepInHandAfterSuccess(false);
		params.SetCommandID(GetApplyToSelfAnimCmnd(target));
		params.SetCommandIntArg(1);
		params.SetCommandFloatArg(0.0);
		params.SetMaxAnimLength(m_fApplyToSelfDuration);
		params.SetIntParam(bodyPartToSplint);
		params.SetIntParam(bodyPartToSplint);

		return params;
	}
	
	//------------------------------------------------------------------------------------------------
	// constructor
	void GC_ConsumableSplint()
	{
		m_eConsumableType = SCR_EConsumableType.GC_SPLINT;
	}
}
