#ifndef __AUTOPATCHER_PATCH_CONTEXT_H
#define __AUTOPATCHER_PATCH_CONTEXT_H

enum PatchContext {
	PC_HASH_WITH_PATCH,
	PC_WRITE_FILE,
	PC_ERROR_FILE_WRITE_FAILURE,
	PC_ERROR_PATCH_TARGET_MISSING,
	PC_ERROR_PATCH_APPLICATION_FAILURE,
	PC_ERROR_PATCH_RESULT_CHECKSUM_FAILURE,
	PC_NOTICE_WILL_COPY_ON_RESTART,
};

#endif
