#pragma once

#include "Zydis/Decoder.h"
#include "zydis/DecoderTypes.h"

namespace PEE
{
	template<class A, class B>
	union ConversionCaster
	{
		A _a;
		B _b;

		ConversionCaster() = default;
		ConversionCaster(A a) : _a(a) {}
		ConversionCaster(B b) : _b(b) {}

		ConversionCaster& operator=(A a) { _a = a; return *this; }
		ConversionCaster& operator=(B b) { _b = b; return *this; }

		operator A() { return _a; }
		operator B() { return _b; }
	};

	template <class B>
	using VoidCaster = ConversionCaster<void*, B>;


	enum struct OperEnum
	{
		Jump = 1,
		Other = 0,
		Call = -1,
	};

	inline static OperEnum IsCallOrJump(uintptr_t addr)
	{
		//0x15 0xE8//These are calls, represented by negative numbers
		//0x25 0xE9//These are jumps, represented by positive numbers.
		//And zero represent it being neither.

		if (addr)
		{
			auto first_byte = reinterpret_cast<uint8_t*>(addr);

			switch (*first_byte)
			{
			case 0x15:
			case 0xE8:
				return OperEnum::Call;

			case 0x25:
			case 0xE9:
				return OperEnum::Jump;

			}
		}

		return OperEnum::Other;
	}

	//Need to move to a general use library
	struct Allocator
	{
		constexpr static size_t defaultSize = 14;

		static size_t GetAlloc()
		{
			return allocCount;
		}

		inline static size_t allocCount = 0;


		Allocator(size_t count = defaultSize)
		{
			allocCount += count;
		}

	};

#define DECLARE_ALLOC(...) inline static Allocator _alloc{ __VA_ARGS__ };

#define DECLARE_ALLOC_IF(mc_con, ...) inline static Allocator _alloc{ mc_con ?  size_t{__VA_ARGS__} : Allocator::defaultSize };


	struct ProloguePatchOld : Xbyak::CodeGenerator
	{
		explicit ProloguePatchOld(uintptr_t address, uintptr_t length)
		{
			// Hook returns here. Execute the restored bytes and jump back to the original function.
			for (size_t i = 0; i < length; i++)
				db(*reinterpret_cast<uint8_t*>(address + i));

			jmp(ptr[rip]);
			dq(address + length);
		}

		static uint8_t* GetInstructions(uintptr_t address, uintptr_t length)
		{
			ProloguePatchOld it{ address, length };
			it.ready();
			return it.GetInstructions();
		}

		uint8_t* GetInstructions()
		{
			auto size = getSize();
			uint8_t* result{ new uint8_t[size] };
			const uint8_t* from = getCode();
			uint8_t* in = result;
			while (size--) {
				*(in++) = *(from++);
			}

			//std::memcpy(result.get(), getCode(), size);
			return result;
		}
	};

	struct ProloguePatch : protected Xbyak::CodeGenerator
	{
	private:
		inline static ZydisDecoder decoder{};

	public:
		explicit ProloguePatch(uintptr_t address, const std::source_location& loc = std::source_location::current())
		{
			int bytes_remaining = 5;
			size_t length = 0;
			do
			{
				ZydisDecodedInstruction instr;

				auto status = ZydisDecoderDecodeInstruction(&decoder, nullptr, reinterpret_cast<const void*>(address + length), 5, &instr);

				if (!ZYAN_SUCCESS(status)) {
					spdlog::log(
						spdlog::source_loc{
							loc.file_name(),
							static_cast<int>(loc.line()),
							loc.function_name() },
							spdlog::level::critical,
							fmt::format_string<uint32_t>{"decoder unsuccessful. status {}"},
							static_cast<uint32_t>(status));

					throw std::exception("Prologue hook failure");
				}
				bytes_remaining -= instr.length;
				length += instr.length;
			} while (bytes_remaining > 0);

				
		
			// Hook returns here. Execute the restored bytes and jump back to the original function.
			for (size_t i = 0; i < length; i++)
				db(*reinterpret_cast<uint8_t*>(address + i));

			jmp(ptr[rip]);
			dq(address + length);
		}



		static uintptr_t GetInstructions(uintptr_t address, const std::source_location& loc = std::source_location::current())
		{
			auto& trampoline = SKSE::GetTrampoline();
			return GetInstructions(trampoline, address, loc);
		}

		static uintptr_t GetInstructions(SKSE::Trampoline& trampoline, uintptr_t address, const std::source_location& loc = std::source_location::current())
		{
			ProloguePatch it{ address, loc};

			return it.MoveInstructions(trampoline);
		}


		uintptr_t MoveInstructions(SKSE::Trampoline& trampoline)
		{
			ready();

			auto size = getSize();

			auto alloc = trampoline.allocate(size);
			std::memcpy(alloc, getCode(), size);
			return reinterpret_cast<uintptr_t>(alloc);
		}


	};

	template <typename T>
	void InstallHook()
	{
		logger::info("Installing {}. . .", typeid(T).name());
		T::Install();
		//If there's a cleaner way to get the name of this
		logger::info("Successfully installed {}", typeid(T).name());
	}


	struct Profiler
	{
		Profiler() : start{ std::chrono::high_resolution_clock::now() }
		{

		}


		double time()
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> duration = end - start;
			return duration.count();
		}

		std::chrono::steady_clock::time_point start;




	};

	struct AutoProfiler : public Profiler
	{
		struct Pause
		{
			bool old;

			Pause()
			{
				old = dontProfile;
				dontProfile = true;
			}

			~Pause()
			{
				dontProfile = old;
			}
		};

		inline static thread_local bool dontProfile = false;

		std::string_view name;
		std::source_location location;
		bool profiling{};

		AutoProfiler(const char* n, std::source_location loc) : name{ n }, location{ loc }, profiling{ !dontProfile }
		{
		}

		~AutoProfiler()
		{
			if (profiling) {
				auto t = time();

				std::string_view file = location.file_name();

				std::cout << "============" << name << " at " << file.substr(file.find_last_of("/\\") + 1) << "(" << location.line() << ") " << t << "ms============" << std::endl;
			}
		}


	};

	template <typename R>
	R ProfileCall(std::function<R()> func, std::source_location loc = std::source_location::current())
	{

		AutoProfiler profiler{ "Call", loc };

		return func();
	}

	constexpr float pi = std::numbers::pi_v<float>;


	constexpr float RadToDeg(float rad) noexcept
	{
		return (180.f / pi) * rad;

	}

	constexpr float DegToRad(float deg) noexcept
	{
		return (pi / 180.f) * deg;
	}

#ifdef NDEBUG

#define PROFILE(mc_expr) mc_expr
#define PROFILE_STATEMENT(mc_stat) mc_stat

#else

#define PROFILE(mc_expr) ProfileCall(std::function{[&]() -> auto { return mc_expr; }})
#define PROFILE_STATEMENT(mc_stat) ProfileCall(std::function{[&]() -> auto { mc_stat; }})

#endif

	inline RE::PlayerCharacter* GetPlayer()
	{
		static RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		return player;
	}

	inline RE::TESDataHandler* GetDataHandler()
	{
		static RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();

		if (!handler)
			handler = RE::TESDataHandler::GetSingleton();

		return handler;
	}
}