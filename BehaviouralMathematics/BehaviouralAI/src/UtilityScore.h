#ifndef UTILITY_SCORE_H_
#define UTILITY_SCORE_H_
#include <vector>


namespace UtilitySystem
{

	class UtilityValue;

	class UtilityScore
	{
	public:
		UtilityScore();
		~UtilityScore();

		void addUtilityValue(UtilityValue* a_pValue, float a_fModifier);

		float getUtilityScore();

		//void addUtilityScore(UtilityScore* a_pScore);
		
	private:
		struct UtilityInfo
		{
			UtilityValue* pValue;
			float		  fModifier;
		};
		std::vector<UtilityInfo> m_vUtilityValues;
		//std::vector<UtilityScore*> m_vUtilityScores;
	};

}

#endif