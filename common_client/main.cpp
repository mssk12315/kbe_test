#include "common\common.h"
#include "common_client.h"

int main(int argc, char* argv[])
{
	//printf("getPySysResPath = %s\n", Resmgr::getSingleton().getPySysResPath().c_str());
	//printf("getPyUserResPath = %s\n", Resmgr::getSingleton().getPyUserResPath().c_str());
	//printf("getPyUserScriptsPath = %s\n", Resmgr::getSingleton().getPyUserScriptsPath().c_str());

	common_client client;
	client.run();

	return 0;
}