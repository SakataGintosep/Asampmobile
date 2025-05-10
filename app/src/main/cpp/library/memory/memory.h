#pragma once

#include <dobby.h>
#include <KittyMemory.hpp>

//#define MEMORY_DEBUG

#ifdef MEMORY_DEBUG
#define MEM_LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "Memory", __VA_ARGS__)
#else
#define MEM_LOGD(...) do {} while (0)
#endif

#define MEM_LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Memory", __VA_ARGS__)

#define DECL_HOOK(_ret, _name, ...) \
    _ret (*_name)(__VA_ARGS__);     \
    _ret _name##_hook(__VA_ARGS__)

#define HOOK(_addr, _name) \
    Memory::hook(_addr, (void*)(&_name##_hook), (void**)(&_name))

#define PLT_HOOK(_addr, _name) \
    Memory::plt_hook(_addr, (void*)(&_name##_hook), (void**)(&_name))

class Memory {
public:
	static uintptr_t getLibraryAddress(const char* lib_name)
	{
		MEM_LOGI("getLibraryAddress: %s", lib_name);

		KittyMemory::ProcMap map{};
		map = KittyMemory::getElfBaseMap(lib_name);

		if (!map.isValidELF()) {
			MEM_LOGI("No map for lib: ", lib_name);
			return 0;
		}

		MEM_LOGI("%s address: 0x%X ", lib_name, map.startAddress);

		return map.startAddress;
	}

	static int protectAddr(uintptr_t addr, size_t len = PAGE_SIZE)
	{
		MEM_LOGD("protectAddr: 0x%X", addr);

		if (KittyMemory::setAddressProtection(reinterpret_cast<void*>(addr), len, _PROT_RWX_) == 0) {
			MEM_LOGI("protectAddr (_PROT_RWX_) failed: 0x%X -> using _PROT_RW_", addr);
			return 0;
		}

		return KittyMemory::setAddressProtection(reinterpret_cast<void*>(addr), len, _PROT_RW_);
	}

	static void memWrite(uintptr_t addr, const void* buffer, size_t len)
	{
		MEM_LOGD("memWrite: 0x%X, 0x%X, %d", addr, buffer, len);
		KittyMemory::memWrite(reinterpret_cast<void*>(addr), buffer, len);

		/*protectAddr(addr);
		protectAddr(addr + len);
		memcpy(reinterpret_cast<void*>(addr), buffer, len);
		cacheflush(addr, addr + len, 0);*/
	}

	static void nop(uintptr_t addr, unsigned int word_count)
	{
		MEM_LOGD("nop: 0x%X, %d", addr, word_count);
		for (uintptr_t ptr = addr; ptr != (addr + (word_count * 2)); ptr += 2) {
			memWrite(ptr, "\x00\xBF", 2);
		}
	}

	static void ret(uintptr_t addr)
	{
		MEM_LOGD("ret: 0x%X", addr);
		memWrite(addr, "\x00\x20\xF7\x46", 4);
	}

	template<typename Ret = void*, typename Type, typename...Args>
	static Ret callFunction(Type addr, Args...args)
	{
		uintptr_t addr_ = reinterpret_cast<uintptr_t>(addr);
		return reinterpret_cast<Ret(*)(Args...)>(addr_)(args...);
	}

	static void plt_hook(uintptr_t addr, void* hook_func, void** orig_func = nullptr)
	{
		Dl_info info{};
		if (dladdr(reinterpret_cast<void*>(addr), &info)) {
			MEM_LOGI("PLT Hooking %s (0x%X) from %s (0x%X)",
					info.dli_sname ? info.dli_sname : "unknown symbol",
					info.dli_saddr,
					info.dli_fname ? info.dli_fname : "unknown file",
					info.dli_fbase
			);
		}

		protectAddr(addr);
		if (orig_func) *orig_func = reinterpret_cast<void*>(*(uintptr_t*) addr);
		*(uintptr_t*) addr = reinterpret_cast<uintptr_t>(hook_func);
	}

	static void hook(uintptr_t addr, void* hook_func, void** orig_func)
	{
		Dl_info info{};
		if (dladdr(reinterpret_cast<void*>(addr), &info)) {
			MEM_LOGI("Hooking %s (0x%X) from %s (0x%X)",
					info.dli_sname ? info.dli_sname : "unknown symbol",
					info.dli_saddr,
					info.dli_fname ? info.dli_fname : "unknown file",
					info.dli_fbase
			);
		}

		DobbyHook(reinterpret_cast<void*>(addr + 1), hook_func, orig_func);
	}
};
