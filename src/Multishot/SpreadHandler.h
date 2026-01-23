#pragma once



namespace PEE::MULT
{
	struct SpreadHandler
	{ 
		enum Enum
		{
			kAdjustsPos,
			kAdjustsRot,
		};

		inline static float maxSpreadAngle = 5.0f;//The base spread experienced with multiple projectiles.
		inline static float baseSpread = 4.f;//The base spread experienced with multiple projectiles.
		inline static float spreadDeviation = 0.f;//
	

		inline static std::mutex mutex;
		inline static bool isActive;
		inline static Enum adjustType = kAdjustsRot;

		inline static RE::TESObjectREFR* source = nullptr;
		//Set these when loading the initial
		inline static uint32_t projIndex = 0;
		inline static uint32_t projCount = 1;
		inline static float spreadValue = 0;

		static bool IsProjCountEven()
		{
		
			return projCount % 2 == 0;
		}
		static bool IsIndexEven()
		{

			return projIndex % 2 == 0;
		}
		

		static bool ShouldAdjust()
		{
			return projIndex || IsProjCountEven();
		}

		static uint8_t SetCount(int8_t num)
		{
			projIndex = 0;
			projCount = num;

			return num;
		}

		
		static float HandleAxisX(float axis, float degrees)
		{
			float abs_axis = std::abs(axis);
			float limit = 90;
			//I think this bit here can be simulated by getting whatever the angle for the up
			// arrow is, and adding that to the limit. This would be prefered actually.
			if (abs_axis > limit) {
				limit = abs_axis;
			}

			float a;
			float b;
			if (degrees >= 0) {
				a = axis;
				b = -axis;

			}
			else {
				a = -axis;
				b = axis;
			}

			//Use slerp instead
			
			return a + ((b - a) * (std::abs(degrees) / 180.f));
			if (degrees >= 0) {

				return axis + (((-axis) - axis) * (degrees / 180.f));
			}
			else {
				return -axis + (((axis)- -axis) * (std::abs(degrees) / 180.f));
			}


		}

		static float NormalizeAngle(float angle, float limit)
		{
			float abs_angle = std::abs(angle);
			
			
			if (abs_angle > limit) {
				auto trunc = std::trunc(abs_angle / limit);
				if (trunc > 1) trunc--;
				return angle -  ((abs_angle / angle)*(limit * trunc));
			}
			else {
				return angle;
			}

		}



		static RE::NiMatrix3 RotateX(RE::NiMatrix3& target, float rad)
		{
			float c = std::cos(rad);
			float s = std::sin(rad);

			RE::NiMatrix3 ret{};

			ret.entry[1][1] = c;
			ret.entry[1][2] = -s;

			ret.entry[2][1] = s;
			ret.entry[2][2] = c;

			return target * ret;
		}

		static RE::NiMatrix3 RotateY(RE::NiMatrix3& target, float rad)
		{
			float c = std::cos(rad);
			float s = std::sin(rad);

			RE::NiMatrix3 ret{};

			ret.entry[0][0] = c;
			ret.entry[0][2] = s;

			ret.entry[2][0] = -s;
			ret.entry[2][2] = c;

			return target * ret;
			
		}

		static RE::NiMatrix3 RotateZ(RE::NiMatrix3& target, float rad)
		{
			float c = std::cos(rad);
			float s = std::sin(rad);

			RE::NiMatrix3 ret{};

			ret.entry[0][0] = c;
			ret.entry[0][1] = -s;

			ret.entry[1][0] = s;
			ret.entry[1][1] = c;

			return target * ret;
		}
		static RE::NiPoint3 Normalize(RE::NiPoint3 result)
		{
			
			float len = result.Length();
			if (len > 0.0f)
			{
				result.x = result.x / len;
				result.y = result.y / len;
				result.z = result.z / len;
			}
			else
			{
				result.x = 0.0f;
				result.y = 0.0f;
				result.z = 0.0f;
			}
			return result;
		}
		template <typename T>static int sign(T val) {
			return (T(0) < val) - (val < T(0));
		}
		float percent_between(float val, float min, float max) {
			// Ensure the range is not zero to avoid division by zero
			if (max == min) {
				return 0.0; // Or handle as an error condition
			}

			// The formula: (current_value - min) / (max - min) * 100
			float range = max - min;
			float normalized_value = val - min;
			float percentage = (normalized_value / range);

			return percentage;
		}

		static float add_rot_z(float val, float d)
		{
			const float PI = 3.1415926f;
			// -pi/2..pi/2
			d = d * PI / 180.0f;
			val += d;
			while (val < 0) val += 2 * PI;
			while (val > 2 * PI) val -= 2 * PI;
			return val;
		}

		static void AdjustAngleAfter(RE::TESObjectREFR* user, RE::TESObjectWEAP* weapon, RE::Projectile* proj)
		{
			//I think this should adjust more than angle, I think 2 modes of multi shot should be used. Angle spread
			// and positional spread.
			//zAngle += 90;
			//return;
			auto index = ((projIndex - IsProjCountEven()) / 2) + 1;

			//evens are right, odds are left
			float dir = IsIndexEven() ? 1.f : -1.f;

			//float angle = NormalizeAngle(dir * index * baseSpread, 180);
			float angle = (dir * index * baseSpread);

			RE::NiPoint3 point = proj->GetAngle();
			point.z += DegToRad(angle);

			proj->SetAngle(point);
		}



		static void AdjustAngle(RE::TESObjectREFR* user, RE::TESObjectWEAP* weapon, float& xAngle, float& zAngle)
		{
			//All garbage.

			//I think this should adjust more than angle, I think 2 modes of multi shot should be used. Angle spread
			// and positional spread.
			//zAngle += 90;
			//return;
			auto index = ((projIndex - IsProjCountEven()) / 2) + 1;

			//evens are right, odds are left
			float dir = IsIndexEven() ? 1.f : -1.f;

			float angle = NormalizeAngle(dir * index * baseSpread, 180);
			//float angle = (dir * index * baseSpread);
			//float angle = NormalizeAngle(dir * 45, 180);

			
			zAngle = add_rot_z(zAngle, angle); DegToRad(angle);
			return;
			static float hard_elevation = 0;
			static bool use_elevation = true;


			float tilt = 0;

			//In the future, cache this.
			if (user->IsPlayerRef())
			{
				auto ammo = user->GetCurrentAmmo();
				static RE::INISettingCollection* collection = RE::INISettingCollection::GetSingleton();
				RE::Setting* setting;
				if (ammo->IsBolt() == true)
				{
					static auto bolt_tilt = collection->GetSetting("f1PBoltTiltUpAngle:Combat");
					setting = bolt_tilt;
				}
				else if (RE::PlayerCamera::GetSingleton()->IsInFirstPerson() == true)
				{
					static auto bolt_tilt = collection->GetSetting("f1PArrowTiltUpAngle:Combat");
					setting = bolt_tilt;
				}
				else
				{
					static auto bolt_tilt = collection->GetSetting("f3PArrowTiltUpAngle:Combat");
					setting = bolt_tilt;
				}

				tilt = setting->GetFloat();
			}

			if (1)
			{
				float x = RE::rad_to_deg(xAngle);
				float z = RE::rad_to_deg(zAngle);
				
				float old_x = x;
				//if (use_elevation)
				//	x = hard_elevation;

				//float addZ = std::lerp(angle, dir * 180, x / 90.f);
				//float addZ = angle * (std::abs(x) / 90.f);
				//float addZ = angle * std::clamp(x / 90.f, -1.f, 1.f);
				//float addZ = angle * std::clamp(std::abs(x) / 90.f, -1.f, 1.f);
				float addZ = angle * (std::abs(x) / 90.f);
				
				//float addX = std::lerp(x, -sign(x) * 90.f, std::abs(angle) / 180);
				//float setX = std::lerp(x, -x, std::abs(angle) / 180);
				//float addX = angle * (std::abs(x) / 90.f);

				//One thing that seems to be of a problem is that more needs to be taken from X
				// the closer it is to 45, the less changes it'll actually need
				//float wave_func = std::pow(std::abs(std::sin((angle * pi) / 180.f)), pi);
				//float wave_func = std::abs(std::sin((angle * pi) / 180.f));
				//float addX = (std::abs(x) - tilt) * wave_func;

				
				//I want to experiment with removing the tilt here, at least when it comes to  firing straight up

				float tilt_90 = tilt + 90;
				float tilt_180 = tilt_90 * 2;
				float tilt_360 = tilt_180 * 2;

				float line_wave_func = (std::abs(std::fmod(angle + tilt_180, tilt_360) - tilt_180)/ tilt_180);
				float addX = (std::abs(x)) * line_wave_func;

				
				float new_x = x + (-sign(x) * addX);

				addZ = 0;

				//Something suggests that lerp might want to increase z to 90 so left right is given to x to solve instead. Ex: works when looking straight up
				//*zAngle = RE::deg_to_rad(90 * dir); 
				//First test with the current x value, but after test with what the x value will be
				//float addZAngle = std::lerp(angle, dir * 90, std::abs(x) / 90.f);
				//float addZAngle = std::lerp(angle, dir * 90, std::pow(std::abs(x) / tilt_90, 2));

				float power = pi;
				float addZAngle = std::lerp(angle, dir * tilt_90, std::pow(std::abs(x - tilt) / 90, 6));

				//The above works if the angle is below 90, but what about if it's above it? Additionally, 


				//AddZ is what's taken, everything is handled somewhat correctly, but now we need to
				// have a formula to  rotate z all the way if need be.
				//This has the secondary problem of not working while closer to 0.
				//Fix: angle wasn't being added.
				//~Issue: The more an up or down angle goes the more s
				//~Issue: once more, this only works on extreme angles

				//float addX = 
				//xAngle = RE::deg_to_rad(setX);
				xAngle = RE::deg_to_rad(new_x);
				//zAngle = RE::deg_to_rad(z + addZ + angle);
				zAngle = RE::deg_to_rad(z + addZAngle);
				//zAngle = RE::deg_to_rad(90 * dir + z);//test only


				//These prove I'm operating on a misconception somewhere. It should be a straight line firing up. But it's not. 
				//xAngle = RE::deg_to_rad(-86);
				//zAngle = RE::deg_to_rad(z + 90  * dir + 5 * dir);

				//POST NOTE, please bare in mind that the use of power absolves the need to have abs be a thing.
				

			}

		}



		static void Thing()
		{
			auto num = projCount;

			if (IsProjCountEven())
			{
				num -= 1;
			}

			float maxSpeadValue = maxSpreadAngle / (num * 0.5f);
		}

		static void SetActive(bool value, RE::TESObjectREFR* src)
		{
			if (value)
			{
				//I want to use an owning thread situation here
				isActive = true;
				projIndex = 0;
				projCount = 1;
				source = src;
			}
			else
			{
				projIndex = projCount = 0;
				isActive = false;
				source = nullptr;
			}
		}
	};

}