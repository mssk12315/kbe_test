#include "common\common.h"

using namespace KBEngine;

class KBEEnvInit
{
public:
	KBEEnvInit()
	{
		const char *strEnv_KBE_ROOT = NULL;
		setenv("KBE_ROOT", "E:/KBE/kbengine-0.8.10", 1);
		const char *strEnv_KBE_RES_PATH = NULL;
		setenv("KBE_RES_PATH", "E:/KBE/kbengine-0.8.10/kbe/res;E:/KBE/kbengine-0.8.10/assets(minimum);E:/KBE/kbengine-0.8.10/assets(minimum)/res;E:/KBE/kbengine-0.8.10/assets(minimum)/script", 1);
		const char *strEnv_KBE_BIN_PATH = NULL;
		setenv("KBE_BIN_PATH", "E:/KBE/kbengine-0.8.10/kbe/bin/server", 1);
	}

};

#pragma warning(disable:4073)
#pragma init_seg(lib)
static KBEEnvInit kbe_env_init;