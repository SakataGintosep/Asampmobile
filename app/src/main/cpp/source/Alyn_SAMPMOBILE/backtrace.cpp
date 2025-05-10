#include "backtrace.h"
#include "main.h"

void crashLog(const char* fmt, ...)
{
	char buffer[0xFF];
	memset(buffer, 0, sizeof(buffer));

	va_list arg;
	va_start(arg, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, arg);
	va_end(arg);

	LOGE("%s", buffer);
}

struct BacktraceState {
	void** current;
	void** end;
};

_Unwind_Reason_Code unwind_callback(struct _Unwind_Context* context, void* arg)
{
	auto state = reinterpret_cast<BacktraceState*>(arg);
	uintptr_t pc = _Unwind_GetIP(context);
	if (pc) {
		if (state->current == state->end) {
			return _URC_END_OF_STACK;
		}
		else {
			*state->current++ = reinterpret_cast<void*>(pc);
		}
	}

	return _URC_NO_REASON;
}

void dump_register(int signal, siginfo_t* info, void* ctx)
{
	static const std::array<std::string, 12> signal_name = {
			"SIGHUP", "SIGINT", "SIGQUIT",
			"SIGILL", "SIGTRAP", "SIGABRT",
			"SIGIOT", "SIGBUS", "SIGFPE",
			"SIGKILL", "SIGUSR1", "SIGSEGV"};

	crashLog(" ");
	crashLog("========== SA-MP Mobile Crashed ==========");

	std::string si_code_name("UNKNOWN");
	if (info->si_code == SI_USER) {
		crashLog("Cause: killed by user");
		si_code_name = "SI_USER";
	}

	if (signal == SIGSEGV) {
		if (info->si_code == SEGV_MAPERR) {
			crashLog("Cause: de-referencing a null pointer");
			si_code_name = "SEGV_MAPERR";
		}
		else if (info->si_code == SEGV_ACCERR) {
			crashLog("Cause: read-only pointer");
			si_code_name = "SEGV_ACCERR";
		}
	}

	auto context = reinterpret_cast<ucontext*>(ctx);

	crashLog(" ");
	crashLog("Sigaction:");

	crashLog("\tsignal %d (%s), code %d (%s), fault address 0x%X", signal,
			signal_name[signal].c_str(),
			info->si_code, si_code_name.c_str(), context->uc_mcontext.fault_address);

	crashLog(" ");
	crashLog("Register states:");

	crashLog("\tr0: 0x%X, r1: 0x%x, r2: 0x%X, r3: 0x%X", context->uc_mcontext.arm_r0,
			context->uc_mcontext.arm_r1, context->uc_mcontext.arm_r2, context->uc_mcontext.arm_r3);
	crashLog("\tr4: 0x%X, r5: 0x%x, r6: 0x%X, r7: 0x%X", context->uc_mcontext.arm_r4,
			context->uc_mcontext.arm_r5, context->uc_mcontext.arm_r6, context->uc_mcontext.arm_r7);
	crashLog("\tr9: 0x%X, r9: 0x%x, r10: 0x%X, fp: 0x%X", context->uc_mcontext.arm_r8,
			context->uc_mcontext.arm_r9, context->uc_mcontext.arm_r10,
			context->uc_mcontext.arm_fp);
	crashLog("\tip: 0x%X, sp: 0x%x, lr: 0x%X, pc: 0x%X", context->uc_mcontext.arm_ip,
			context->uc_mcontext.arm_sp, context->uc_mcontext.arm_lr, context->uc_mcontext.arm_pc);

	crashLog(" ");
	crashLog("BackTrace:");

	Dl_info dl_info;

	if (dladdr((void*) (context->uc_mcontext.arm_pc), &dl_info) != 0) {
		//if (dl_info.dli_sname != nullptr)
		{
			crashLog("\tPC [\"%s\":0x%X] + 0x%X | [\"%s\"+%d]",
					dl_info.dli_fname ? dl_info.dli_fname : "unknown file",
					dl_info.dli_fbase,
					(uintptr_t) (void*) (context->uc_mcontext.arm_pc) -
							(uintptr_t) dl_info.dli_fbase,
					dl_info.dli_sname ? dl_info.dli_sname : "unknown symbol",
					(uintptr_t) (void*) (context->uc_mcontext.arm_pc) -
							(uintptr_t) dl_info.dli_saddr);
		}
	}

	if (dladdr((void*) (context->uc_mcontext.arm_lr), &dl_info) != 0) {
		//if (dl_info.dli_sname != nullptr)
		{
			crashLog("\tLR [\"%s\":0x%X] + 0x%X | [\"%s\"+%d]",
					dl_info.dli_fname ? dl_info.dli_fname : "unknown file",
					dl_info.dli_fbase,
					(uintptr_t) (void*) (context->uc_mcontext.arm_lr) -
							(uintptr_t) dl_info.dli_fbase,
					dl_info.dli_sname ? dl_info.dli_sname : "unknown symbol",
					(uintptr_t) (void*) (context->uc_mcontext.arm_lr) -
							(uintptr_t) dl_info.dli_saddr);
		}
	}
}

void dump_stack(int ignoreDepth, int maxDepth)
{
	crashLog(" ");
	crashLog("BackTrace:");

	const int max = 100;
	void* buffer[max];

	BacktraceState state{};
	state.current = buffer;
	state.end = buffer + max;

	_Unwind_Backtrace(unwind_callback, &state);

	int count = (int) (state.current - buffer);
	if (count > maxDepth) {
		count = maxDepth + 1;
	}

	for (int idx = ignoreDepth; idx < count; idx++) {
		const void* addr = buffer[idx];
		const char* symbol = "";

		Dl_info info;
		if (dladdr(addr, &info) && info.dli_sname) {
			symbol = info.dli_sname;
		}

		int status{0};
		char* demangled = __cxxabiv1::__cxa_demangle(symbol, nullptr, nullptr, &status);

		//if (info.dli_sname != nullptr)
		{
			crashLog("\t#%d [\"%s\":0x%X] + 0x%X | [\"%s\"+%d]",
					idx - ignoreDepth,
					info.dli_fname ? info.dli_fname : "unknown file",
					info.dli_fbase,
					(uintptr_t) addr - (uintptr_t) info.dli_fbase,
					info.dli_sname ? (status == 0 ? demangled : info.dli_sname) : "unknown symbol",
					(uintptr_t) addr - (uintptr_t) info.dli_saddr);
		}

		if (demangled) {
			free(demangled);
		}
	}
}
