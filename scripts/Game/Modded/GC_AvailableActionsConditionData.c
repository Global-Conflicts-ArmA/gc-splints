modded class SCR_AvailableActionsConditionData
{
	protected bool m_bIsCharacterFractured;

	override private void Clear()
	{
		m_bIsCharacterFractured = false;
		super.Clear();
	}
	
	bool GetIsCharacterFractured()
	{
		return m_bIsCharacterFractured;
	}
	
	override protected void FetchHealthData(float timeSlice)
	{
		m_CharacterDamageComponent = SCR_CharacterDamageManagerComponent.Cast(m_Character.GetDamageManager());
		if (!m_CharacterDamageComponent)
			return;
		
		if (m_CharacterDamageComponent.GetAimingDamage() > 0 || m_CharacterDamageComponent.GetMovementDamage() > 0)
			m_bIsCharacterFractured = true;
		
		super.FetchHealthData(timeSlice);
	}
};
